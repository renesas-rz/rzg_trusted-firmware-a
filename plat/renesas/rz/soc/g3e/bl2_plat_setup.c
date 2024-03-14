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
#include <libfdt.h>


static console_t rzg3e_bl2_console;

int bl2_plat_handle_pre_image_load(unsigned int image_id)
{
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

void bl2_init_fdt(void)
{
	static uint64_t fdt_blob[PAGE_SIZE_4KB / sizeof(uint64_t)];
	void *fdt = (void *)fdt_blob;

	INFO("BL2: FDT init\n");
	memset((void *)FDT_BASE, 0, FDT_SIZE);

	/* Set up FDT */
	int ret = fdt_create_empty_tree(fdt, sizeof(fdt_blob));

	if (ret != 0) {
		NOTICE("BL2: Cannot allocate FDT for U-Boot (ret=%i)\n", ret);
		panic();
	}

	fdt_setprop_u32(fdt, 0, "#address-cells", 2);
	fdt_setprop_u32(fdt, 0, "#size-cells", 2);

	fdt_appendprop_string(fdt, 0, "compatible", "renesas,rzg3e-dev");
	fdt_appendprop_string(fdt, 0, "compatible", "renesas,r9a09g047e53");

	memcpy((void *)FDT_BASE, fdt_blob, sizeof(fdt_blob));

	flush_dcache_range((uintptr_t)FDT_BASE, (size_t)FDT_SIZE);

	INFO("BL2: RZ/G3E appended fdt prop\n");
}

void bl2_el3_early_platform_setup(u_register_t arg1, u_register_t arg2,
								u_register_t arg3, u_register_t arg4)
{
	int ret;

	/* early setup Clock and Reset */
	cpg_early_setup();

	/* initialize SYC */
	syc_init(RZG3E_SYC_INCK_HZ);

	/* initialize Timer */
	generic_delay_timer_init();

	/* setup PFC */
	pfc_setup();

	/* setup Clock and Reset */
	cpg_setup();

	/* initialize console driver */
	ret = console_rz_register(
							RZG3E_SCIF_BASE,
							RZG3E_UART_INCK_HZ,
							RZG3E_UART_BARDRATE,
							&rzg3e_bl2_console);
	if (!ret)
		panic();

	console_set_scope(&rzg3e_bl2_console,
			CONSOLE_FLAG_BOOT | CONSOLE_FLAG_CRASH);
}

void bl2_el3_plat_arch_setup(void)
{
	const mmap_region_t bl2_regions[] = {
		MAP_REGION_FLAT(BL2_BASE, BL2_END - BL2_BASE,
				MT_MEMORY | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(BL_CODE_BASE, BL_CODE_END - BL_CODE_BASE,
				MT_CODE | MT_SECURE),
		MAP_REGION_FLAT(RZG3E_BOOTINFO_BASE, RZG3E_BOOTINFO_SIZE,
				MT_MEMORY | MT_RO | MT_SECURE),
		MAP_REGION_FLAT(PARAMS_BASE, PARAMS_SIZE,
				MT_MEMORY | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(FDT_BASE, FDT_SIZE,
				MT_MEMORY | MT_RW | MT_SECURE),
#if SEPARATE_CODE_AND_RODATA
		MAP_REGION_FLAT(BL_RO_DATA_BASE, BL_RO_DATA_END - BL_RO_DATA_BASE,
				MT_RO_DATA | MT_SECURE),
#endif
		{0}
	};

	const mmap_region_t rzg3e_mmap[] = {
#if TRUSTED_BOARD_BOOT
		MAP_REGION_FLAT(RZG3E_BOOT_ROM_BASE, RZG3E_BOOT_ROM_SIZE,
				MT_MEMORY | MT_RO | MT_SECURE),
#endif
		MAP_REGION_FLAT(RZG3E_DEVICE_BASE, RZG3E_DEVICE_SIZE,
				MT_DEVICE | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZG3E_XSPI_MEMORY_MAP_BASE, RZG3E_XSPI_SIZE,
				MT_MEMORY | MT_RO | MT_SECURE),
		MAP_REGION_FLAT(RZG3E_DDR_BASE, RZG3E_DDR_SIZE,
				MT_MEMORY | MT_RW | MT_SECURE),
		{0}
	};

	setup_page_tables(bl2_regions, rzg3e_mmap);
	enable_mmu_el3(0);
}

void bl2_platform_setup(void)
{
	/* Setup TZC-400, Access Control */
	plat_security_setup();

	rz_io_setup();

	/* initialize DDR */
	ddr_setup();

	bl2_init_fdt();
}
