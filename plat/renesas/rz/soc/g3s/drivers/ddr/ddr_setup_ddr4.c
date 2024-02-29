/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <lib/mmio.h>
#include <sys_regs.h>

#include "ddr_regs.h"
#include "ddr_private.h"

#define DAT0_BASE	(0x00060000)
#define DAT1_BASE	(0x00064000)

extern const uint32_t param_phyinit_swizzle[][2];
extern const uint32_t param_phyinit_c[][2];
extern const uint32_t param_phyinit_i[][2];
extern const uint16_t phyinit_1d[];
extern const uint16_t phyinit_2d[];
extern const uint16_t param_phyinit_f_1d_0[];
extern const uint16_t param_phyinit_f_2d_0[];
extern const uint32_t param_setup_mc[][2];

extern const uint32_t param_phyinit_swizzle_size;
extern const uint32_t param_phyinit_c_size;
extern const uint32_t param_phyinit_i_size;
extern const uint32_t phyinit_1d_size;
extern const uint32_t phyinit_2d_size;
extern const uint32_t param_phyinit_f_1d_0_size;
extern const uint32_t param_phyinit_f_2d_0_size;
extern const uint32_t param_setup_mc_size;

void setup_mc(void)
{
	size_t i;

	for (i = 0; i < param_setup_mc_size; i++)
		DDRTOP_mc_apb_wr(param_setup_mc[i][0], param_setup_mc[i][1]);
}

void update_mc(void)
{
	DDRTOP_mc_param_wr(LPI_WAKEUP_EN_ADDR, LPI_WAKEUP_EN_OFFSET, LPI_WAKEUP_EN_WIDTH, 0x1F);
	mmio_write_32(SYS_DDR_CFG, 0x00000000);
}

void phyinit_configuration(void)
{
	size_t i;

	dwc_ddrphy_apb_wr(0x0006E000, 0x00000000);

	for (i = 0; i < param_phyinit_c_size; i++)
		dwc_ddrphy_apb_wr(param_phyinit_c[i][0], param_phyinit_c[i][1]);

	dwc_ddrphy_apb_wr(0x0006E000, 0x00000001);
}

void phyinit_pin_swizzling(void)
{
	size_t i;

	dwc_ddrphy_apb_wr(0x0006E000, 0x00000000);

	for (i = 0; i < param_phyinit_swizzle_size; i++)
		dwc_ddrphy_apb_wr(param_phyinit_swizzle[i][0], param_phyinit_swizzle[i][1]);

	dwc_ddrphy_apb_wr(0x0006E000, 0x00000001);
}

void phyinit_load_1d_image(void)
{
	size_t i;

	dwc_ddrphy_apb_wr(0x00058060, 0x00000002);
	dwc_ddrphy_apb_wr(0x0006E000, 0x00000000);

	for (i = 0; i < phyinit_1d_size; i++)
		dwc_ddrphy_apb_wr(DAT0_BASE + i, (uint32_t)phyinit_1d[i]);

	for (i = i; (DAT0_BASE + i) < DAT1_BASE; i++)
		dwc_ddrphy_apb_wr(DAT0_BASE + i, 0);

	for (i = 0; i < param_phyinit_f_1d_0_size; i++)
		dwc_ddrphy_apb_wr(DAT1_BASE + i, (uint32_t)param_phyinit_f_1d_0[i]);

	dwc_ddrphy_apb_wr(0x0006E000, 0x00000001);
}

void phyinit_exec_1d_image(void)
{
	dwc_ddrphy_apb_wr(0x0006E000, 0x1);
	dwc_ddrphy_apb_wr(0x0006E099, 0x9);
	dwc_ddrphy_apb_wr(0x0006E099, 0x1);
	dwc_ddrphy_apb_wr(0x0006E099, 0x0);
	dwc_ddrphy_phyinit_userCustom_G_waitDone(1);
	dwc_ddrphy_apb_wr(0x0006E099, 0x1);
}

void phyinit_load_2d_image(void)
{
	size_t i;

	dwc_ddrphy_apb_wr(0x0006E000, 0x00000000);

	for (i = 0; i < phyinit_2d_size; i++)
		dwc_ddrphy_apb_wr(DAT0_BASE + i, (uint32_t)phyinit_2d[i]);

	for (i = i; (DAT0_BASE + i) < DAT1_BASE; i++)
		dwc_ddrphy_apb_wr(DAT0_BASE + i, 0);

	for (i = 0; i < param_phyinit_f_2d_0_size; i++)
		dwc_ddrphy_apb_wr(DAT1_BASE + i, (uint32_t)param_phyinit_f_2d_0[i]);

	dwc_ddrphy_apb_wr(0x0006E000, 0x00000001);
}

void phyinit_exec_2d_image(void)
{
	dwc_ddrphy_apb_wr(0x0006E000, 0x1);
	dwc_ddrphy_apb_wr(0x0006E099, 0x9);
	dwc_ddrphy_apb_wr(0x0006E099, 0x1);
	dwc_ddrphy_apb_wr(0x0006E099, 0x0);
	dwc_ddrphy_phyinit_userCustom_G_waitDone(2);
	dwc_ddrphy_apb_wr(0x0006E099, 0x1);
}

void phyinit_load_eng_image(void)
{
	size_t i;

	dwc_ddrphy_apb_wr(0x0006E000, 0x00000000);

	for (i = 0; i < param_phyinit_i_size; i++)
		dwc_ddrphy_apb_wr(param_phyinit_i[i][0], param_phyinit_i[i][1]);

	dwc_ddrphy_apb_wr(0x0006D080, 0x00000000);
	dwc_ddrphy_apb_wr(0x0006E000, 0x00000001);
}

void self_refresh_entry(void)
{
	DDRTOP_mc_param_wr(LP_CMD_ADDR, LP_CMD_OFFSET, LP_CMD_WIDTH, 0b1010001);
	DDRTOP_mc_param_poll(LP_STATE_ADDR, LP_STATE_OFFSET, LP_STATE_WIDTH, 0b1001010);
}
