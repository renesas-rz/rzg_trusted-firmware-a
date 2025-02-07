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
#include <sys.h>
#include <scifa.h>
#include <ddr.h>
#include <sys_regs.h>
#include <plat_tzc_def.h>
#include <rz_soc_def.h>
#include <rz_private.h>


static console_t rzt2h_bl2_console;

/*
 * Note: no implementation is required for bl2_plat_handle_pre_image_load()
 * So, use the TF-A core default 'weak' implementation in plat/common/plat_bl_common.c
 */

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

	/* initialize SYC */
	syc_init(RZT2H_SYC_INCK_HZ);

	/* initialize Timer */
	generic_delay_timer_init();

	/* setup PFC */
	pfc_setup();

	/* setup Clock and Reset */
	cpg_setup();

	/* initialize console driver */
	ret = console_rz_register(
							RZT2H_SCIF_BASE,
							RZT2H_UART_INCK_HZ,
							RZT2H_UART_BARDRATE,
							&rzt2h_bl2_console);
	if (!ret)
		panic();

	console_set_scope(&rzt2h_bl2_console,
			CONSOLE_FLAG_BOOT | CONSOLE_FLAG_CRASH);
}

void bl2_el3_plat_arch_setup(void)
{
	const mmap_region_t bl2_regions[] = {
		MAP_REGION_FLAT(BL2_BASE, BL2_END - BL2_BASE,
				MT_MEMORY | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(BL_CODE_BASE, BL_CODE_END - BL_CODE_BASE,
				MT_CODE | MT_SECURE),
		MAP_REGION_FLAT(PARAMS_BASE, PARAMS_SIZE,
				MT_MEMORY | MT_RW | MT_SECURE),
#if SEPARATE_CODE_AND_RODATA
		MAP_REGION_FLAT(BL_RO_DATA_BASE, BL_RO_DATA_END - BL_RO_DATA_BASE,
				MT_RO_DATA | MT_SECURE),
#endif
		{0}
	};

	const mmap_region_t rzt2h_mmap[] = {
		MAP_REGION_FLAT(RZT2H_R_BUS_NON_SAFETY_DEVICE_BASE, RZT2H_R_BUS_NON_SAFETY_DEVICE_SIZE,
				MT_DEVICE | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZT2H_DDR_MEMC_BASE, RZT2H_DDR_MEMC_DEVICE_SIZE,
				MT_DEVICE | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZT2H_R_BUS_SAFETY_DEVICE_BASE, RZT2H_R_BUS_SAFETY_DEVICE_SIZE,
				MT_DEVICE | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZT2H_DDR_PHY_BASE, RZT2H_DDR_PHY_DEVICE_SIZE,
				MT_DEVICE | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZT2H_A_BUS_DEVICE_BASE, RZT2H_A_BUS_DEVICE_SIZE,
				MT_DEVICE | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZT2H_XSPI0_MEMORY_MAP_BASE, RZT2H_XSPI_MEMORY_MAP_SIZE,
				MT_MEMORY | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZT2H_XSPI1_MEMORY_MAP_BASE, RZT2H_XSPI_MEMORY_MAP_SIZE,
				MT_MEMORY | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZT2H_DDR_MIRROR_0_BASE, RZT2H_DDR_MIRROR_SIZE,
				MT_MEMORY | MT_RW | MT_SECURE),
		{0}
	};

	setup_page_tables(bl2_regions, rzt2h_mmap);
	enable_mmu_el3(0);
}

void bl2_platform_setup(void)
{
	/* Setup TZC-400, Access Control */
	plat_security_setup();

	/* Confirm build CPU count */
	sys_check_platform_core_count();

	rz_io_setup();

	/* initialize DDR */
	ddr_setup();
}
