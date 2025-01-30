/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
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
#include <sys.h>
#include <scifa.h>
#include <pwrc.h>
#include <plat_tzc_def.h>
#include <rz_soc_def.h>
#include <rz_private.h>

static console_t rzg3s_bl2_console;

static uint32_t bl2_plat_get_boot_mode(void)
{
	if (sys_is_resume_reboot())
		return RZ_WARM_BOOT;
	else
		return RZ_COLD_BOOT;
}

int bl2_plat_handle_pre_image_load(unsigned int image_id)
{
	bl_mem_params_node_t *bl_mem_params = get_bl_mem_params_node(image_id);

	if (image_id == BL22_IMAGE_ID) {
#if PLAT_M33_BOOT_SUPPORT
		if (sys_is_m33_core_booted())
			bl_mem_params->image_info.h.attr |= IMAGE_ATTRIB_SKIP_LOADING;
#endif
	} else {
		if (bl2_plat_get_boot_mode() == RZ_WARM_BOOT)
			bl_mem_params->image_info.h.attr |= IMAGE_ATTRIB_SKIP_LOADING;
	}

	/* Clean next_params_info in BL image node */
	bl_mem_params->params_node_mem.next_params_info = NULL;

	return 0;
}

int bl2_plat_handle_post_image_load(unsigned int image_id)
{
	static bl2_to_bl31_params_mem_t *params;
	bl_mem_params_node_t *bl_mem_params;

	if (!params) {
		params = (bl2_to_bl31_params_mem_t *) PARAMS_BASE;
		memset((void *)PARAMS_BASE, 0, sizeof(*params));
	}

	bl_mem_params = get_bl_mem_params_node(image_id);

	switch (image_id) {
	case BL31_IMAGE_ID:
		params->boot_kind = bl2_plat_get_boot_mode();
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

	return 0;
}

void bl2_el3_early_platform_setup(u_register_t arg1, u_register_t arg2,
								  u_register_t arg3, u_register_t arg4)
{
	int ret;

	/* early setup Clock and Reset */
	cpg_early_setup();

	if (!sys_is_resume_peripheral()) {
		/* initialize SYC */
		syc_init(RZG3S_SYC_INCK_HZ);

		/* initialize Timer */
		generic_delay_timer_init();

		/* setup PFC */
		pfc_setup();

		/* setup Clock and Reset */
		cpg_setup();

		/* initialize console driver */
		ret = console_rz_register(
								RZG3S_SCIF_0_BASE,
								RZG3S_UART_INCK_HZ,
								RZG3S_UART_BARDRATE,
								&rzg3s_bl2_console);
		if (!ret)
			panic();

		console_set_scope(&rzg3s_bl2_console,
				CONSOLE_FLAG_BOOT | CONSOLE_FLAG_CRASH);
	} else {
		/* initialize Timer */
		generic_delay_timer_init();

		/* resume Clock and Reset */
		cpg_resume_setup();

		/* initialize console driver */
		ret = console_rz_register(
								RZG3S_SCIF_0_BASE,
								RZG3S_UART_INCK_HZ,
								RZG3S_UART_BARDRATE,
								&rzg3s_bl2_console);
		if (!ret)
			panic();

		console_set_scope(&rzg3s_bl2_console,
				CONSOLE_FLAG_BOOT | CONSOLE_FLAG_CRASH);
	}
	pwrc_setup();
}

void bl2_el3_plat_arch_setup(void)
{
	if (!sys_is_resume_peripheral()) {
		const mmap_region_t bl2_regions[] = {
			MAP_REGION_FLAT(BL2_BASE, BL2_END - BL2_BASE,
				MT_MEMORY | MT_RW | MT_SECURE),
			MAP_REGION_FLAT(BL_CODE_BASE, BL_CODE_END - BL_CODE_BASE,
				MT_CODE | MT_SECURE),
			MAP_REGION_FLAT(BL_RO_DATA_BASE, BL_RO_DATA_END - BL_RO_DATA_BASE,
				MT_RO_DATA | MT_SECURE),
			{0}
		};

		const mmap_region_t rzg3s_mmap[] = {
	#if TRUSTED_BOARD_BOOT
			MAP_REGION_FLAT(RZG3S_BOOT_ROM_BASE, RZG3S_BOOT_ROM_SIZE,
					MT_MEMORY | MT_RO | MT_SECURE),
	#endif
			MAP_REGION_FLAT(RZG3S_SRAM_BASE, RZG3S_SRAM_SIZE,
					MT_MEMORY | MT_RW | MT_SECURE),
			MAP_REGION_FLAT(PARAMS_BASE, PARAMS_SIZE,
					MT_MEMORY | MT_RW | MT_SECURE),
			MAP_REGION_FLAT(RZG3S_DEVICE_BASE, RZG3S_DEVICE_SIZE,
					MT_DEVICE | MT_RW | MT_SECURE),
			MAP_REGION_FLAT(RZG3S_SPIROM_BASE, RZG3S_SPIROM_SIZE,
					MT_MEMORY | MT_RO | MT_SECURE),
			MAP_REGION_FLAT(RZG3S_DDR0_BASE, RZG3S_DDR0_SIZE,
					MT_MEMORY | MT_RW | MT_SECURE),
			{0}
		};

		setup_page_tables(bl2_regions, rzg3s_mmap);
		enable_mmu_el3(0);
	}
}

void bl2_platform_setup(void)
{
	/* Setup TZC-400, Access Control */
	plat_security_setup();

	rz_io_setup();

	plat_ddr_setup();
}

void bl2_el3_plat_prepare_exit(void)
{
#if PLAT_M33_BOOT_SUPPORT
	if (!sys_is_m33_core_booted()) {
		bl_mem_params_node_t *bl22_mem_params = get_bl_mem_params_node(BL22_IMAGE_ID);

		if (bl22_mem_params != NULL) {
			sys_m33_core_boot_op((bl22_mem_params->ep_info).pc);
		}
	}
#endif
}
