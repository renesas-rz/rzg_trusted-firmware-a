/*
 * Copyright (c) 2024, Renesas Electronics Corporation. All rights reserved.
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
#include <rz_private.h>
#include <ddr.h>
#include <sys_regs.h>
#include <plat_tzc_def.h>
#include <rz_soc_def.h>
#include <cpg_regs.h>
#include <pfc_regs.h>
#include <sys.h>
#include <pwrc.h>
#include <plat_tbbr_img_def.h>

#define MMU_NO_FLAGS		0

static console_t rzv2n_bl2_console;

static uint32_t bl2_plat_get_boot_mode(void)
{
	if (sys_is_resume())
		return RZ_WARM_BOOT;

	return RZ_COLD_BOOT;
}

int bl2_plat_handle_pre_image_load(unsigned int image_id)
{
	bl_mem_params_node_t *bl_mem_params = get_bl_mem_params_node(image_id);
	if (bl_mem_params == NULL) {
		ERROR("%s: no mem params for image %u\n",
			  __func__, image_id);
		return -1;
	}

	if (image_id == BL22_IMAGE_ID) {
#if PLAT_M33_BOOT_SUPPORT
		if (cpg_is_m33_core_booted())
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
		memset((void *)PARAMS_BASE, 0, sizeof(bl2_to_bl31_params_mem_t));
	}

	bl_mem_params = get_bl_mem_params_node(image_id);
	if (bl_mem_params == NULL) {
		ERROR("%s: no mem params for image %u\n",
			  __func__, image_id);
		return -1;
	}

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
	/* early setup Clock and Reset */
	cpg_early_setup();

	/* initialize SYC */
	syc_init(RZV2N_SYC_INCK_HZ);

	/* initialize Timer */
	generic_delay_timer_init();

	/* GPIO setup */
	pfc_setup();

	/* setup Clock and Reset */
	cpg_setup();

	/* initialize console driver */
	int ret = console_rz_register(
							RZV2N_SCIF_BASE,
							RZV2N_UART_INCK_HZ,
							RZV2N_UART_BAUDRATE,
							&rzv2n_bl2_console);
	if (!ret)
		panic();

	console_set_scope(&rzv2n_bl2_console,
			CONSOLE_FLAG_BOOT | CONSOLE_FLAG_CRASH);

	pwrc_setup();
}

void bl2_el3_plat_arch_setup(void)
{
	const mmap_region_t bl2_regions[] = {
#if PLAT_M33_BOOT_SUPPORT
		MAP_REGION_FLAT(BL22_BASE, BL22_LIMIT - BL22_BASE,
				MT_CODE | MT_RW | MT_SECURE),
#endif
		MAP_REGION_FLAT(BL2_BASE, BL2_END - BL2_BASE,
				MT_MEMORY | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(BL_CODE_BASE, BL_CODE_END - BL_CODE_BASE,
				MT_CODE | MT_SECURE),
		MAP_REGION_FLAT(RZV2N_BOOTINFO_BASE, RZV2N_BOOTINFO_SIZE,
				MT_MEMORY | MT_RO | MT_SECURE),
#if SEPARATE_CODE_AND_RODATA
		MAP_REGION_FLAT(BL_RO_DATA_BASE, BL_RO_DATA_END - BL_RO_DATA_BASE,
				MT_RO_DATA | MT_SECURE),
#endif
		{0}
	};

	const mmap_region_t rzv2n_mmap[] = {
#if TRUSTED_BOARD_BOOT
		MAP_REGION_FLAT(RZV2N_BOOT_ROM_BASE, RZV2N_BOOT_ROM_SIZE,
				MT_MEMORY | MT_RO | MT_SECURE),
		MAP_REGION_FLAT(RZV2N_BOOT_RAM_BASE, RZV2N_BOOT_RAM_SIZE,
				MT_MEMORY | MT_RW | MT_SECURE),
#endif
		MAP_REGION_FLAT(RZV2N_SRAM_BASE, RZV2N_SRAM_TOTAL_SIZE,
				MT_MEMORY | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZV2N_DEVICE_BASE, RZV2N_DEVICE_SIZE,
				MT_DEVICE | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZV2N_XSPI_MEMORY_MAP_BASE, RZV2N_XSPI_SIZE,
				MT_MEMORY | MT_RO | MT_SECURE),
		MAP_REGION_FLAT(RZV2N_DDR_BASE, RZV2N_DDR_SIZE,
				MT_MEMORY | MT_RW | MT_SECURE),
		{0}
	};

	setup_page_tables(bl2_regions, rzv2n_mmap);
	enable_mmu_el3(MMU_NO_FLAGS);
}

void bl2_platform_setup(void)
{
	/* Setup TZC-400, Access Control */
	bl2_security_setup();

	rz_io_setup();

	NOTICE("BL2: SYS_LSI_MODE: 0X%x\n", mmio_read_32(SYS_LSI_MODE));
	NOTICE("BL2: SYS_LSI_DEVID: 0X%x\n", mmio_read_32(SYS_LSI_DEVID));
	NOTICE("BL2: SYS_LSI_PRR: 0X%x\n", mmio_read_32(SYS_LSI_PRR));

	/* initialize DDR */
	plat_ddr_setup();
}


void bl2_el3_plat_prepare_exit(void)
{
#if PLAT_M33_BOOT_SUPPORT
	if (!cpg_is_m33_core_booted()) {
		INFO("Booting Cortex-M33\n");
		bl_mem_params_node_t *bl22_mem_params = get_bl_mem_params_node(BL22_IMAGE_ID);

		if (bl22_mem_params != NULL) {
			sys_m33_core_boot_op();
		}
	}
#endif /* PLAT_M33_BOOT_SUPPORT */
	console_flush();
}

