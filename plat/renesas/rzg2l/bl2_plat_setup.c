/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <arch_helpers.h>
#include <common/bl_common.h>
#include <pfc.h>
#include <cpg.h>
#include <assert.h>
#include <system_regs.h>
/* #include <spi_multi.h> */
#include <syc.h>
#include <sys.h>
#include <rzg2l_def.h>
#include <lib/mmio.h>
#include <drivers/io/io_driver.h>
#include <rzg2l_private.h>
#include <common/desc_image_load.h>
#include <lib/xlat_tables/xlat_tables_defs.h>
#include "scifa.h"

extern void rzg2l_io_setup(void);

static console_t rzg2l_bl31_console;

/* FDT with DRAM configuration */
uint64_t fdt_blob[PAGE_SIZE_4KB / sizeof(uint64_t)];

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
	case BL31_IMAGE_ID:
		break;
	case BL33_IMAGE_ID:
		memcpy(&params->bl33_ep_info, &bl_mem_params->ep_info,
			sizeof(entry_point_info_t));
		break;
	}

	return 0;
}


void bl2_el3_early_platform_setup(u_register_t arg1, u_register_t arg2,
								u_register_t arg3, u_register_t arg4)
{
	uint32_t boot_dev, ret;
	
	/* generic timer */
	syc_init(RZG2L_SYSC_BASE, RZG2L_SYC_INCK_HZ);

	boot_dev = sys_get_mode_mr();

	cpg_setup();

	pfc_setup();

	/* initialize console driver */
	ret = console_rzg2l_register(
							RZG2L_SCIF0_BASE,
							RZG2L_UART_INCK_HZ,
							RZG2L_UART_BARDRATE,
							&rzg2l_bl31_console);
	if(!ret)
		panic();

	console_set_scope(&rzg2l_bl31_console,
			CONSOLE_FLAG_BOOT | CONSOLE_FLAG_RUNTIME | CONSOLE_FLAG_CRASH);
	
	/* spi_multi_setup(); */
	
	if(boot_dev == BOOT_MODE_SPI_1_8 ||
			boot_dev == BOOT_MODE_SPI_3_3) {
		rzg2l_io_setup();
	}
	else
		panic();

}

void bl2_el3_plat_arch_setup(void)
{
}

void bl2_platform_setup(void)
{
}

