/*
 * Copyright (c) 2024, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdbool.h>
#include <lib/mmio.h>
#include <drivers/delay_timer.h>
#include <rz_soc_def.h>
#include <common/debug.h>
#include <lib/utils_def.h>

#include "cpg_regs.h"
#include "cpg_settings.h"

static void cpg_wdtrst_sel_setup(void)
{
	uint32_t val	= CPG_ERRORRST_SELx_ERRRSTSEL0
					| CPG_ERRORRST_SELx_ERRRSTSEL1
					| CPG_ERRORRST_SELx_ERRRSTSEL2
					| CPG_ERRORRST_SELx_ERRRSTSEL3;
	uint32_t ca33_w01, ca33_w23, ca55_w01, ca55_w23;

	/* Clear bit 28 interrupt source for both M33 and CA55 */
	mmio_write_32(RZV2N_ICU_ERINTM33CLR(0), 0x10000000);
	mmio_write_32(RZV2N_ICU_ERINTA55CLR(0), 0x10000000);

	ca33_w01 = mmio_read_32(RZV2N_ICU_ERINTM33CTL(0));
	ca33_w23 = mmio_read_32(RZV2N_ICU_ERINTM33CTL(1));
	ca55_w01 = mmio_read_32(RZV2N_ICU_ERINTA55CTL(0));
	ca55_w23 = mmio_read_32(RZV2N_ICU_ERINTA55CTL(1));

	/* Checking ICU interrupt WDT CM33 */
	if ((ca33_w01 == 0x40000000) || (ca33_w01 == 0x80000000) ||
			(ca33_w23 == 0x00000001) || (ca33_w23 == 0x00000002)) {
		/* ERINTM33CLR0 bit for clear 28-31 */
		mmio_write_32(RZV2N_ICU_ERINTM33CLR(0), 0xF0000000);
	}

	/* Checking ICU interrupt WDT CA55 */
	if ((ca55_w01 == 0x40000000) || (ca55_w01 == 0x80000000) ||
			(ca55_w23 == 0x00000001) || (ca55_w23 == 0x00000002)) {
		/* ERINTA55CLR0 bit for clear 28-31 */
		mmio_write_32(RZV2N_ICU_ERINTA55CLR(0), 0xF0000000);
	}

	/* Add in the WEN bits for the selected bits */
	val = (val & 0xFFFF) | ((val & 0xFFFF) << 16);

	mmio_write_32(CPG_ERRORRST_SEL2, val);
}

void cpg_reset_wdt1(void)
{
	/* WDT reset apply */
	cpg_wdt1_set_clk(CPG_CLK_DISABLE);
	cpg_wdt1_set_rst(CPG_RST_ASSERT);
	udelay(1);

	/* Release MSTOP  incase this is not yet used */
	mstop_remove(CPG_BUS_1_MSTOP, CPG_BUS_1_MSTOP_WDT1);
	udelay(1);

	cpg_wdt1_set_clk(CPG_CLK_ENABLE);
	cpg_wdt1_set_rst(CPG_RST_DEASSERT);
	udelay(1);

	mmio_write_32(RZV2N_ICU_ERINTA55CLR(0), 0x10000000);
}

void cpg_setup_wdt1(void)
{
	mmio_write_32(CPG_ERRORRST_SEL1, 0x000A000A);
	mmio_write_32(CPG_ERRORRST_SEL2, 0x00020002);
	mmio_write_32(CPG_ERROR_RST2, 0x00020002);
}

void cpg_ddr_part1(void)
{
	mmio_write_32(CPG_RST_11, 0x0FF80000);

	mmio_write_32(CPG_LP_DDR_CTL1, mmio_read_32(CPG_LP_DDR_CTL1) & ~0x00000001);

	mmio_write_32(CPG_PLLDDR0_STBY, 0x00010001);	/* PLLDDR0 clock start */
	while ((mmio_read_32(CPG_PLLDDR0_MON) & 0x00000011) != 0x00000011)
		;

	mmio_write_32(CPG_CLKON_12, 0x0FC00FC0);

	udelay(1);

	mmio_write_32(CPG_RST_11, 0x00080008);
	mmio_write_32(CPG_LP_DDR_CTL1, mmio_read_32(CPG_LP_DDR_CTL1) | 0x00000001);

	udelay(1);

	mmio_write_32(CPG_RST_11, 0x03F003F0);

	udelay(1);
}

void cpg_ddr_part2(void)
{
	mmio_write_32(CPG_RST_11, 0x08000800);

	udelay(10);

	mmio_write_32(CPG_RST_11, 0x04000400);

	udelay(10);
}

void cpg_ddr_set_pwrokin_off(void)
{
	mmio_write_32(CPG_LP_DDR_CTL1, mmio_read_32(CPG_LP_DDR_CTL1) & ~0x00000001);	/* DDR0 */
}

#if PLAT_SYSTEM_SUSPEND
void cpg_suspend_setup(void)
{
	cpg_enter_s2r_mstop();
	cpg_others_pd_set_clk(CPG_CLK_DISABLE);
	cpg_others_pd_set_rst(CPG_RST_ASSERT);
}
#endif

#if PLAT_M33_BOOT_SUPPORT
void cpg_cm33_setup(void)
{
	cpg_cm33_set_clk(CPG_CLK_ENABLE);

	/* CM33 Release Cold Reset */
	/* Step 1 */
	while ((mmio_read_32(CPG_RSTMON_0) & CM33_RSTMON_MASK) != (0b1110 << 16))
		;

	/* Step 2 */
	mmio_write_32(CPG_RST_1, (CM33_RST_MASK << 16) | (0b001000));

	/* Step 3 */
	while ((mmio_read_32(CPG_RSTMON_0) & CM33_RSTMON_MASK) != (0b1100 << 16))
		;

	/* Step 4 */
	mmio_write_32(CPG_RST_1, (CM33_RST_MASK << 16) | (0b111000));

	/* Step 5 */
	while ((mmio_read_32(CPG_RSTMON_0) & CM33_RSTMON_MASK) != (0b0000 << 16))
		;
}

bool cpg_is_m33_core_booted(void)
{
	bool is_booted = false;

	uint32_t clkmon = mmio_read_32(CPG_CLKMON_0) & CM33_CLKMON_MASK;
	uint32_t rstmon = mmio_read_32(CPG_RSTMON_0) & CM33_RSTMON_MASK;

	if ((clkmon == CM33_CLKMON_MASK) && (rstmon == 0x0))
		is_booted = true;

	INFO("CPG: CM33 CLKMON: 0x%x, RSTMON: 0x%x\n", clkmon, rstmon);

	return is_booted;
}

#endif /* PLAT_M33_BOOT_SUPPORT */


void cpg_early_setup(void)
{
	/* CGC_SYC_0_CNT_CLK and SYC_0_RESETN are 'forcibly modified by hardware' so do not need setting here */
}

void cpg_setup(void)
{
	cpg_div_sel_static_setup();
	cpg_pll_setup();
	cpg_awo_pd_set_clk(CPG_CLK_ENABLE);
	cpg_others_pd_set_clk(CPG_CLK_ENABLE);
	cpg_awo_pd_set_rst(CPG_RST_DEASSERT);
	cpg_others_pd_set_rst(CPG_RST_DEASSERT);
	cpg_awo_pd_mstop_setup();
	cpg_others_pd_mstop_setup();
	cpg_div_sel_dynamic_setup();
	cpg_wdtrst_sel_setup();
}
