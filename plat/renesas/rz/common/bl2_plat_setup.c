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
#include <cpg_regs.h>
#include <cpg.h>
#include <syc.h>
#include <scifa.h>
#include <ddr.h>
#include <sys_regs.h>
#include <plat_tzc_def.h>
#include <rz_soc_def.h>
#include <rz_private.h>
#include <drivers/delay_timer.h>
#include <libfdt.h>


/* FDT with DRAM configuration */
uint64_t fdt_blob[PAGE_SIZE_4KB / sizeof(uint64_t)];
static void *fdt = (void *)fdt_blob;

static console_t rzg2l_bl31_console;

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
		memset((void *)PARAMS_BASE, 0, sizeof(*params));
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
	int ret;

	memset((void *)FDT_BASE, 0, FDT_SIZE);

	/* Set up FDT */
	ret = fdt_create_empty_tree(fdt, sizeof(fdt_blob));
	if (ret != 0) {
		NOTICE("BL2: Cannot allocate FDT for U-Boot (ret=%i)\n", ret);
		panic();
	}

	fdt_setprop_u32(fdt, 0, "#address-cells", 2);
	fdt_setprop_u32(fdt, 0, "#size-cells", 2);

	switch (mmio_read_32(SYS_LSI_DEVID) & 0x0FFFFFFF) {
	case 0x841C447:
		switch (mmio_read_32(0x11861178)) {
		case 0:
			NOTICE("BL2: RZ/G2L\n");
			fdt_appendprop_string(fdt, 0, "compatible", "renesas,smarc-rzg2l");
			fdt_appendprop_string(fdt, 0, "compatible", "renesas,r9a07g044l2");
			break;
		case 1:
			NOTICE("BL2: RZ/G2LC\n");
			fdt_appendprop_string(fdt, 0, "compatible", "renesas,smarc-rzg2lc");
			fdt_appendprop_string(fdt, 0, "compatible", "renesas,r9a07g044c2");
			break;
		default:
			NOTICE("BL2: Unknown RZ/G2L variant\n");
			panic();
		}
		break;
	case 0x8450447:
		switch (mmio_read_32(0x11861178)) {
		case 0:
			NOTICE("BL2: RZ/G2UL Type 1\n");
			fdt_appendprop_string(fdt, 0, "compatible", "renesas,smarc-rzg2ul");
			fdt_appendprop_string(fdt, 0, "compatible", "renesas,r9a07g043u11");
			break;
		case 1:
			NOTICE("BL2: RZ/G2UL Type 2 (unsupported)\n");
			panic();
			break;
		default:
			NOTICE("BL2: Unknown RZ/G2UL variant\n");
			panic();
		}
		break;
	case 0x8447447:
		NOTICE("BL2: RZ/V2L\n");
		fdt_appendprop_string(fdt, 0, "compatible", "renesas,smarc-rzv2l");
		fdt_appendprop_string(fdt, 0, "compatible", "renesas,r9a07g054l2");
		break;
	default:
		NOTICE("BL2: Unknown SoC\n");
		panic();
	}

	memcpy((void *)FDT_BASE, fdt_blob, sizeof(fdt_blob));
	flush_dcache_range((uintptr_t)FDT_BASE, (size_t)FDT_SIZE);
}

void bl2_el3_early_platform_setup(u_register_t arg1, u_register_t arg2,
								u_register_t arg3, u_register_t arg4)
{
	int ret;

	/* early setup Clock and Reset */
	cpg_early_setup();

	/* initialize SYC */
	syc_init(RZG2L_SYC_INCK_HZ);

	/* initialize Timer */
	generic_delay_timer_init();

	/* setup PFC */
	pfc_setup();

	/* setup Clock and Reset */
	cpg_setup();

	/* USB 2.0 Phy workaround for RZ/G2L,LC	*/
	if (((mmio_read_32(SYS_LSI_DEVID) & 0x0FFFFFFF) == 0x841C447) &&
		((mmio_read_32(0x11861124) & 0xf00) == 0x700) &&
		((mmio_read_32(0x11861128) & 0xf00) == 0x700)) {
		mmio_write_32(CPG_CLKON_USB, 0x000F000F);
		while ((mmio_read_32(CPG_CLKMON_USB) & 0x0000000F) != 0x0000000F)
			;
		mmio_write_32(CPG_RST_USB, 0x000F000F);
		while ((mmio_read_32(CPG_RSTMON_USB) & 0x0000000F) != 0x00000000)
			;
		mmio_write_32(0x11c40014, 0x00021506);
		mmio_write_32(0x11c40010, 0x01021506);

		mmio_write_32(CPG_CLKON_USB, 0x000F0000);
		while ((mmio_read_32(CPG_CLKMON_USB) & 0x00000000) != 0x00000000)
			;
	}

	/* initialize console driver */
	ret = console_rz_register(
							RZG2L_SCIF0_BASE,
							RZG2L_UART_INCK_HZ,
							RZG2L_UART_BARDRATE,
							&rzg2l_bl31_console);
	if (!ret)
		panic();

	console_set_scope(&rzg2l_bl31_console, CONSOLE_FLAG_BOOT | CONSOLE_FLAG_CRASH);
}

void bl2_el3_plat_arch_setup(void)
{
	const mmap_region_t bl2_regions[] = {
		MAP_REGION_FLAT(BL2_BASE, BL2_END - BL2_BASE,
			MT_MEMORY | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(BL_CODE_BASE, BL_CODE_END - BL_CODE_BASE,
			MT_CODE | MT_SECURE),
		MAP_REGION_FLAT(BL_RO_DATA_BASE, BL_RO_DATA_END - BL_RO_DATA_BASE,
			MT_RO_DATA | MT_SECURE),
		{0}
	};

	static const mmap_region_t rzg2l_mmap[] = {
	#if TRUSTED_BOARD_BOOT
		MAP_REGION_FLAT(RZG2L_BOOT_ROM_BASE, RZG2L_BOOT_ROM_SIZE,
				MT_MEMORY | MT_RO | MT_SECURE),
	#endif
		MAP_REGION_FLAT(RZG2L_SRAM_BASE, RZG2L_SRAM_SIZE,
				MT_MEMORY | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZG2L_DEVICE_BASE, RZG2L_DEVICE_SIZE,
				MT_DEVICE | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZG2L_SPIROM_BASE, RZG2L_SPIROM_SIZE,
				MT_MEMORY | MT_RO | MT_SECURE),
		MAP_REGION_FLAT(RZG2L_DDR1_BASE, RZG2L_DDR1_SIZE,
				MT_MEMORY | MT_RW | MT_SECURE),
		{0}
	};

	setup_page_tables(bl2_regions, rzg2l_mmap);
	enable_mmu_el3(0);
}

void bl2_platform_setup(void)
{
	/* Setup TZC-400, Access Control */
	plat_security_setup();

#if !DEBUG_FPGA
	/* initialize DDR */
	ddr_setup();
#endif /* DEBUG_FPGA */

	bl2_init_fdt();

	rz_io_setup();

	NOTICE("BL2: SYS_LSI_MODE: 0x%x\n", mmio_read_32(SYS_LSI_MODE));
	NOTICE("BL2: SYS_LSI_DEVID: 0x%x\n", mmio_read_32(SYS_LSI_DEVID));
	NOTICE("BL2: SYS_LSI_PRR: 0x%x\n", mmio_read_32(SYS_LSI_PRR));
}
