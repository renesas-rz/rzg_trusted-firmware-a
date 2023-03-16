/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <lib/mmio.h>

#include "ddr_regs.h"
#include "ddr_private.h"
#include "ddr_param_def_lpddr4.h"

#define IMEM_BASE	(0x00060000)
#define DMEM_BASE	(0x00064000)

void setup_mc(void)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE(param_setup_mc_data); i++)
		DDRTOP_mc_apb_wr(param_setup_mc_data[i][0], param_setup_mc_data[i][1]);
}

void phyinit_configuration(void)
{
	size_t i;

	dwc_ddrphy_apb_wr(0x0006E000, 0x00000000);

	for (i = 0; i < ARRAY_SIZE(param_phyinit_c); i++)
		dwc_ddrphy_apb_wr(param_phyinit_c[i][0], param_phyinit_c[i][1]);

	dwc_ddrphy_apb_wr(0x0006E000, 0x00000001);
}

void phyinit_pin_swizzling(void)
{
	size_t i;

	dwc_ddrphy_apb_wr(0x0006E000, 0x00000000);

	for (i = 0; i < ARRAY_SIZE(param_phyinit_swizzle); i++)
		dwc_ddrphy_apb_wr(param_phyinit_swizzle[i][0], param_phyinit_swizzle[i][1]);

	dwc_ddrphy_apb_wr(0x0006E000, 0x00000001);
}

void phyinit_load_1d_image(void)
{
	size_t i;

	dwc_ddrphy_apb_wr(0x00058060, 0x00000002);
	dwc_ddrphy_apb_wr(0x0006E000, 0x00000000);

	for (i = 0; i < ARRAY_SIZE(param_phyinit_1d_imem); i++)
		dwc_ddrphy_apb_wr(IMEM_BASE + i, (uint32_t)param_phyinit_1d_imem[i]);

	for (i = i; (IMEM_BASE + i) < DMEM_BASE; i++)
		dwc_ddrphy_apb_wr(IMEM_BASE + i, 0);

	for (i = 0; i < ARRAY_SIZE(param_phyinit_1d_dmem); i++)
		dwc_ddrphy_apb_wr(DMEM_BASE + i, (uint32_t)param_phyinit_1d_dmem[i]);

	dwc_ddrphy_apb_wr(0x0006E000, 0x00000001);
}

void phyinit_exec_1d_image(void)
{
	dwc_ddrphy_apb_wr(0x0006E000, 0x1);
	dwc_ddrphy_apb_wr(0x0006E099, 0x9);
	dwc_ddrphy_apb_wr(0x0006E099, 0x1);
	dwc_ddrphy_apb_wr(0x0006E099, 0x0);
	dwc_ddrphy_phyinit_userCustom_G_waitFwDone();
	dwc_ddrphy_apb_wr(0x0006E099, 0x1);
}

void phyinit_load_2d_image(void)
{
	size_t i;

	dwc_ddrphy_apb_wr(0x0006E000, 0x00000000);

	for (i = 0; i < ARRAY_SIZE(param_phyinit_2d_imem); i++)
		dwc_ddrphy_apb_wr(IMEM_BASE + i, (uint32_t)param_phyinit_2d_imem[i]);

	for (i = i; (IMEM_BASE + i) < DMEM_BASE; i++)
		dwc_ddrphy_apb_wr(IMEM_BASE + i, 0);

	for (i = 0; i < ARRAY_SIZE(param_phyinit_2d_dmem); i++)
		dwc_ddrphy_apb_wr(DMEM_BASE + i, (uint32_t)param_phyinit_2d_dmem[i]);

	dwc_ddrphy_apb_wr(0x0006E000, 0x00000001);
}

void phyinit_exec_2d_image(void)
{
	dwc_ddrphy_apb_wr(0x0006E000, 0x1);
	dwc_ddrphy_apb_wr(0x0006E099, 0x9);
	dwc_ddrphy_apb_wr(0x0006E099, 0x1);
	dwc_ddrphy_apb_wr(0x0006E099, 0x0);
	dwc_ddrphy_phyinit_userCustom_G_waitFwDone();
	dwc_ddrphy_apb_wr(0x0006E099, 0x1);
}

void phyinit_load_eng_image(void)
{
	size_t i;

	dwc_ddrphy_apb_wr(0x0006E000, 0x00000000);

	for (i = 0; i < ARRAY_SIZE(param_phyinit_i); i++)
		dwc_ddrphy_apb_wr(param_phyinit_i[i][0], param_phyinit_i[i][1]);

	dwc_ddrphy_apb_wr(0x0006D080, 0x00000000);
	dwc_ddrphy_apb_wr(0x0006E000, 0x00000001);
}

void self_refresh_entry(void)
{
	DDRTOP_mc_param_wr(LP_CMD_ADDR, LP_CMD_OFFSET, LP_CMD_WIDTH, 0b1010001);
	DDRTOP_mc_param_poll(LP_STATE_ADDR, LP_STATE_OFFSET, LP_STATE_WIDTH, 0b1001111);
}
