/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include <lib/mmio.h>
#include "include/RZx/DDR/ddr_func.h"

#include "rz_soc_def.h"

#include "sys_regs.h"
#include "sys.h"
#include "include/RZx/DDR/RZT2N_DDRTOP.h"

#define BIT_PLL2MON     (0)
#define BIT_PMSEL2_MON (11)

#define NUM_DUMMY_RD_DDRCLKSEL (30)
#define NUM_DUMMY_RD_MSTOP     (13)

int DDR_init(void)
{
	int ret = DDR_OK;
	int ddrtop_ret = DDRTOP_OK;

	sys_safetybase_unlock(PRCRx_CLOCK_GEN);
	/***************************************
	 * PLL2 Setup
	 **************************************/
	/* SYS_CTRL::PLL2EN
	 * - b0 : PLL2EN : 0 STANDBY
	 */
	mmio_write_32(PLL2EN, 0x00000001);
	while ((mmio_read_32(PLL2MON) & (1 << BIT_PLL2MON)) == (0 << BIT_PLL2MON)) {
		;
	}
	sys_safetybase_lock(PRCRx_CLOCK_GEN);

	/* SYS_CTRL::SCKCR4
	 * - b28     : DDRCLKSEL : 0x0:ddr_clk=12.5kHz, 0x1:(Refer DDRDIVSEL)
	 * - b31-b29 : DDRDIVSEL : 0x0:ddr_clk= 800MHz, 0x4:ddr_clk=50MHz
	 * Initial setting here should be 50MHz.
	 * Subsequent DDR initialization code changes it to 800MHz as FSP operation
	 */
	sys_base_unlock(PRCRx_CLOCK_GEN);
	sys_read_modify_32bits(SCKCR4, (0x4U << 29) | (0x1U << 28), (0x7U << 29) | (0x1U << 28));
	for (int i = 0; i < NUM_DUMMY_RD_DDRCLKSEL; i++) {
		mmio_read_32(SCKCR4);
	}
	sys_base_lock(PRCRx_CLOCK_GEN);

	/***************************************
	 * Releasing DDR module stop
	 **************************************/
	/* SYS_CTRL::MSTPCRM
	 * b0 : DDR module stop: 0: release
	 */
	sys_base_unlock(PRCRx_LOW_POWER);
	sys_read_modify_32bits(MSTPCRM, 0x0 << 0, 0x1 << 0);

	/***************************************
	 * Releasing OTFDE module stop
	 **************************************/
	/* SYS_CTRL::MSTPCRP
	 * - b4 : OTFDE module stop: 0: release
	 */
	sys_read_modify_32bits(MSTPCRP, 0x0 << 4, 0x1 << 4);

	/* Dummy read until making SYS_CTRL effect */
	for (int i = 0; i < NUM_DUMMY_RD_MSTOP; i++) {
		mmio_read_32(MSTPCRM);
	}
	sys_base_lock(PRCRx_LOW_POWER);

	/***************************************
	 * Releasing AMBA bus stop
	 **************************************/
	/* SYS_REG::SSTPCR0
	 * - b0: DDR1A_REQ (DDR1 AXI port Request)
	 * - b1: DDR1A_ACK (DDR1 AXI port Acknowledge)
	 */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);
	sys_read_modify_32bits(SSTPCR0, 0x0 << 0, 0x1 << 0);
	while ((mmio_read_32(SSTPCR0) & (0x1 << 1)) == (0x1 << 1)) {
		;
	}

	/* SYS_REG::SSTPCR4
	 * - b0: DDR0R_REQ (DDR0 AXI port Request)
	 * - b1: DDR0R_ACK (DDR0 AXI port Acknowledge)
	 * - b8: DDRAPB (DDR APB IF)
	 */
	sys_read_modify_32bits(SSTPCR4, (0x0 << 8) | (0x0 << 0), (0x1 << 8) | (0x1 << 0));
	while ((mmio_read_32(SSTPCR4) & (0x1 << 1)) == (0x1 << 1)) {
		;
	}
	sys_safetybase_lock(PRCRx_SYS_CTRL);

	/***************************************
	 * DDR Initialization
	 **************************************/
	ddrtop_ret = DDRTOP_proc_init();
	if (ddrtop_ret != DDRTOP_OK)
		ret = DDR_ERR;

	return ret;
}
