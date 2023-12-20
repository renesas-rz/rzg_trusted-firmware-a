/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <arch.h>
#include <arch_helpers.h>
#include <assert.h>
#include <common/bl_common.h>
#include <common/desc_image_load.h>
#include <drivers/generic_delay_timer.h>
#include <lib/xlat_tables/xlat_tables_compat.h>
#include <plat/common/common_def.h>
#include <lib/mmio.h>
#include <pfc.h>
#include <cpg.h>
#include <syc.h>
#include <scifa.h>
#include <ddr.h>
#include <sys_regs.h>
#include <plat_tzc_def.h>
#include <rz_soc_def.h>
#include <rz_private.h>
#include <sys.h>
#include <pwrc.h>

extern void bl2_enter_bl31(const struct entry_point_info *bl_ep_info);
static console_t rzv2h_bl2_console;

static uint32_t bl2_plat_get_boot_mode(void)
{
	if (sys_is_resume_reboot())
		return RZ_WARM_BOOT;

	return RZ_COLD_BOOT;
}

int bl2_plat_handle_pre_image_load(unsigned int image_id)
{
	if (image_id == BL31_IMAGE_ID) {
		bl2_to_bl31_params_mem_t *params = (bl2_to_bl31_params_mem_t *)PARAMS_BASE;

		params->boot_kind = bl2_plat_get_boot_mode();

		/* If a warm start is in progress then skip rest of intialisation and jump directly to BL31 */
		if (params->boot_kind == RZ_WARM_BOOT) {
			bl_mem_params_node_t *bl_mem_params = get_bl_mem_params_node(image_id);

			bl_mem_params->image_info.h.attr |= IMAGE_ATTRIB_SKIP_LOADING;
			flush_dcache_range((uintptr_t)PARAMS_BASE, sizeof(bl2_to_bl31_params_mem_t));
			bl2_enter_bl31(&bl_mem_params->ep_info);
		}
	}

	return 0;
}

int bl2_plat_handle_post_image_load(unsigned int image_id)
{
	static bl2_to_bl31_params_mem_t *params;
	bl_mem_params_node_t *bl_mem_params;

	if (!params) {
		params = (bl2_to_bl31_params_mem_t *) PARAMS_BASE;
		memset((void *)PARAMS_BASE, 0, sizeof(bl2_to_bl31_params_mem_t));
	}

	bl_mem_params = get_bl_mem_params_node(image_id);

	switch (image_id) {
	case BL31_IMAGE_ID:
		/* This function is only entered for BL31 image if it is the cold boot */
		params->boot_kind = RZ_COLD_BOOT;
		break;
	case BL32_IMAGE_ID:
		memcpy(&params->bl32_ep_info, &bl_mem_params->ep_info,
			sizeof(entry_point_info_t));
		break;
	case BL33_IMAGE_ID:
		memcpy(&params->bl33_ep_info, &bl_mem_params->ep_info,
			sizeof(entry_point_info_t));
		break;
	default:
		/* Do nothing in default case */
		break;
	}

	flush_dcache_range((uintptr_t)PARAMS_BASE, sizeof(bl2_to_bl31_params_mem_t));
	return 0;
}

void bl2_el3_early_platform_setup(u_register_t arg1, u_register_t arg2,
								u_register_t arg3, u_register_t arg4)
{
	int ret;

	/* early setup Clock and Reset */
	cpg_early_setup();

	/* initialize SYC */
	syc_init(RZV2H_SYC_INCK_HZ);

	/* initialize Timer */
	generic_delay_timer_init();

	/* setup PFC */
	pfc_setup();

	/* setup Clock and Reset */
	cpg_setup();

	/* initialize console driver */
	ret = console_rz_register(
							RZV2H_SCIF_BASE,
							RZV2H_UART_INCK_HZ,
							RZV2H_UART_BARDRATE,
							&rzv2h_bl2_console);
	if (!ret)
		panic();

	console_set_scope(&rzv2h_bl2_console,
			CONSOLE_FLAG_BOOT | CONSOLE_FLAG_CRASH);

	pwrc_setup();
}

void bl2_el3_plat_arch_setup(void)
{
	const mmap_region_t bl2_regions[] = {
		MAP_REGION_FLAT(BL2_BASE, BL2_END - BL2_BASE,
				MT_MEMORY | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(BL_CODE_BASE, BL_CODE_END - BL_CODE_BASE,
				MT_CODE | MT_SECURE),
		MAP_REGION_FLAT(RZV2H_BOOTINFO_BASE, RZV2H_BOOTINFO_SIZE,
				MT_MEMORY | MT_RO | MT_SECURE),
		MAP_REGION_FLAT(PARAMS_BASE, PARAMS_SIZE,
				MT_MEMORY | MT_RW | MT_SECURE),
#if SEPARATE_CODE_AND_RODATA
		MAP_REGION_FLAT(BL_RO_DATA_BASE, BL_RO_DATA_END - BL_RO_DATA_BASE,
				MT_RO_DATA | MT_SECURE),
#endif
		{0}
	};

	const mmap_region_t rzv2h_mmap[] = {
#if TRUSTED_BOARD_BOOT
		MAP_REGION_FLAT(RZV2H_BOOT_ROM_BASE, RZV2H_BOOT_ROM_SIZE,
				MT_MEMORY | MT_RO | MT_SECURE),
#endif
		MAP_REGION_FLAT(RZV2H_DEVICE_BASE, RZV2H_DEVICE_SIZE,
				MT_DEVICE | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZV2H_XSPI_MEMORY_MAP_BASE, RZV2H_XSPI_SIZE,
				MT_MEMORY | MT_RO | MT_SECURE),
		MAP_REGION_FLAT(RZV2H_DDR0_BASE, RZV2H_DDR0_SIZE,
				MT_MEMORY | MT_RW | MT_SECURE),
		{0}
	};

	setup_page_tables(bl2_regions, rzv2h_mmap);
	enable_mmu_el3(0);
}

void bl2_platform_setup(void)
{
	/* Setup TZC-400, Access Control */
	plat_security_setup();

	rz_io_setup();

	/* initialize DDR */
	plat_ddr_setup();
}
