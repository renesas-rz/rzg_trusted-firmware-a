/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <common/debug.h>
#include <sys_regs.h>
#include <cpg_regs.h>
#include <vbatt_regs.h>
#include <cpg.h>
#include <ddr.h>

#include "ddr_regs.h"
#include "ddr_private.h"

static void phyinit_c(void);
static void phyinit_d2h_1d(void);
static void phyinit_d2h_2d(void);
static void phyinit_mc(void);
static void phyinit_i(void);
static void phyinit_j(void);
static void	save_retcsr(void);
static void	restore_retcsr(void);

void ddr_setup(void)
{
	INFO("DDR: Setup (Rev. %s)\n", DDR_VERSION);
	cpg_active_ddr1();
	wait_pclk(2);
	setup_mc();
	cpg_active_ddr2();
	wait_pclk(2);
	phyinit_c();
	phyinit_d2h_1d();
	phyinit_d2h_2d();
	phyinit_mc();
	save_retcsr();
	phyinit_i();
	phyinit_j();
	update_mc();
}

void ddr_retention_entry(void)
{
	dwc_ddrphy_apb_wr(0x00058010, 0);
	DDRTOP_mc_param_poll(CONTROLLER_BUSY_ADDR, CONTROLLER_BUSY_OFFSET, CONTROLLER_BUSY_WIDTH, 0);
	mmio_write_32(SYS_DDR_MCAR_SRCTL0, mmio_read_32(SYS_DDR_MCAR_SRCTL0) & ~0x00000007);
	self_refresh_entry();
	DDRTOP_mc_param_wr(DFIBUS_FREQ_F0_ADDR, DFIBUS_FREQ_F0_OFFSET, DFIBUS_FREQ_F0_WIDTH, 0x1f);
	mmio_write_32(SYS_DDR_MCAR_CTRL, mmio_read_32(SYS_DDR_MCAR_CTRL) | 0x00010000);
	dwc_ddrphy_apb_poll(0x0006E0fa, 0 << 0, 1 << 0);
	mmio_write_32(SYS_DDR_MCAR_CTRL, mmio_read_32(SYS_DDR_MCAR_CTRL) & ~0x00010000);
	dwc_ddrphy_apb_poll(0x0006E0fa, 1 << 0, 1 << 0);
	mmio_write_32(PWRDN_DDRPHY_CTRL, 0x00000311);
	mmio_write_32(CPG_RST_DDR, 0x01000000);
	wait_dficlk(18);
#if defined(PLAT_SYSTEM_SUSPEND_vbat)
	mmio_write_32(VBATT_BKPSR, 0x00000080);
#endif
}

void ddr_retention_exit(void)
{
	INFO("DDR: Retention Exit (Rev. %s)\n", DDR_VERSION);
#if defined(PLAT_SYSTEM_SUSPEND_vbat)
	mmio_write_32(PWRDN_DDRPHY_CTRL, 0x00000311);
	mmio_write_32(VBATT_BKPSR, 0x00000000);
#elif defined(PLAT_SYSTEM_SUSPEND_awo)
	mmio_write_32(PWRDN_DDRPHY_CTRL, 0x00000301);
#endif
	wait_dficlk(18);
	mmio_write_32(PWRDN_DDRPHY_CTRL, 0x00000200);
	cpg_active_ddr1();
	wait_pclk(2);
	setup_mc();
	cpg_active_ddr2();
	wait_pclk(2);
	phyinit_c();
	restore_retcsr();
	phyinit_i();
	phyinit_j();
	update_mc();
}

static void phyinit_c(void)
{
	phyinit_configuration();
	phyinit_pin_swizzling();
}

static void phyinit_d2h_1d(void)
{
	phyinit_load_1d_image();
	phyinit_exec_1d_image();
}

static void phyinit_d2h_2d(void)
{
	phyinit_load_2d_image();
	phyinit_exec_2d_image();
}

static void phyinit_mc(void)
{
	uint32_t val, num_rank, num_byte, tctrl_delay, bl, x, tx_dqs_dly;

	dwc_ddrphy_apb_wr(0x6E000, 0x0);

	num_rank = 1;
	val = DDRTOP_mc_param_rd(MEM_DP_REDUCTION_ADDR, MEM_DP_REDUCTION_OFFSET, MEM_DP_REDUCTION_WIDTH);
	num_byte = 2;
	val = dwc_ddrphy_apb_rd(0x05802e);
	tctrl_delay = ((val >> 1) + (val & 1)) + 8;
	val = DDRTOP_mc_param_rd(BSTLEN_ADDR, BSTLEN_OFFSET, BSTLEN_WIDTH);
	bl = (1 << val);

	x = 0;
	if (num_byte > 0) {
		val = dwc_ddrphy_apb_rd(0x030020); x = (val > x) ? val : x;
	}

	if (num_byte > 1) {
		val = dwc_ddrphy_apb_rd(0x031020); x = (val > x) ? val : x;
	}

	if (num_byte > 2) {
		val = dwc_ddrphy_apb_rd(0x032020); x = (val > x) ? val : x;
	}

	if (num_byte > 3) {
		val = dwc_ddrphy_apb_rd(0x033020); x = (val > x) ? val : x;
	}

	val = 16 + tctrl_delay + (2 * x);

	DDRTOP_mc_param_wr(TDFI_PHY_RDLAT_F0_ADDR, TDFI_PHY_RDLAT_F0_OFFSET, TDFI_PHY_RDLAT_F0_WIDTH, val);

	x = 0;
	if ((num_rank > 0) && (num_byte > 0)) {
		val = dwc_ddrphy_apb_rd(0x0300d0); x = (val > x) ? val : x;
		val = dwc_ddrphy_apb_rd(0x0301d0); x = (val > x) ? val : x;
	}

	if ((num_rank > 0) && (num_byte > 1)) {
		val = dwc_ddrphy_apb_rd(0x0310d0); x = (val > x) ? val : x;
		val = dwc_ddrphy_apb_rd(0x0311d0); x = (val > x) ? val : x;
	}

	if ((num_rank > 0) && (num_byte > 2)) {
		val = dwc_ddrphy_apb_rd(0x0320d0); x = (val > x) ? val : x;
		val = dwc_ddrphy_apb_rd(0x0321d0); x = (val > x) ? val : x;
	}

	if ((num_rank > 0) && (num_byte > 3)) {
		val = dwc_ddrphy_apb_rd(0x0330d0); x = (val > x) ? val : x;
		val = dwc_ddrphy_apb_rd(0x0331d0); x = (val > x) ? val : x;
	}

	if ((num_rank > 1) && (num_byte > 0)) {
		val = dwc_ddrphy_apb_rd(0x0300d1); x = (val > x) ? val : x;
		val = dwc_ddrphy_apb_rd(0x0301d1); x = (val > x) ? val : x;
	}

	if ((num_rank > 1) && (num_byte > 1)) {
		val = dwc_ddrphy_apb_rd(0x0310d1); x = (val > x) ? val : x;
		val = dwc_ddrphy_apb_rd(0x0311d1); x = (val > x) ? val : x;
	}

	if ((num_rank > 1) && (num_byte > 2)) {
		val = dwc_ddrphy_apb_rd(0x0320d1); x = (val > x) ? val : x;
		val = dwc_ddrphy_apb_rd(0x0321d1); x = (val > x) ? val : x;
	}

	if ((num_rank > 1) && (num_byte > 3)) {
		val = dwc_ddrphy_apb_rd(0x0330d1); x = (val > x) ? val : x;
		val = dwc_ddrphy_apb_rd(0x0331d1); x = (val > x) ? val : x;
	}

	tx_dqs_dly = ((x<<6)&0xf) + (((x>>4)&0x01) + ((x>>3)&0x1));
	val = tctrl_delay + (6 + (bl / 2)) + tx_dqs_dly;
	DDRTOP_mc_param_wr(TDFI_WRDATA_DELAY_ADDR, TDFI_WRDATA_DELAY_OFFSET, TDFI_WRDATA_DELAY_WIDTH, val);

	dwc_ddrphy_apb_wr(0x58021, 0xff00);

#if !LPDDR4
	dwc_ddrphy_apb_wr(0x58019, 0x0005);
#endif

	dwc_ddrphy_apb_wr(0x6E000, 0x1);
}

static void phyinit_i(void)
{
	phyinit_load_eng_image();
}

static void phyinit_j(void)
{
	DDRTOP_mc_param_wr(START_ADDR, START_OFFSET, START_WIDTH, 1);
	DDRTOP_mc_param_poll(INT_STATUS_INIT_ADDR, INT_STATUS_INIT_OFFSET + 1, 1, 1);
}

static void	save_retcsr(void)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE(ddr_csr_table); i++)
		ddr_csr_table[i] = ~0x0;

	retcsr_read_registers(ddr_csr_table, sizeof(ddr_csr_table));
}

static void	restore_retcsr(void)
{
	retcsr_write_registers(ddr_csr_table, sizeof(ddr_csr_table));
}
