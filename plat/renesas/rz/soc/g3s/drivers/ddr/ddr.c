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
#include <cpg.h>
#include <ddr.h>

#include "ddr_regs.h"
#include "ddr_private.h"

static void phyinit_c(void);
static void phyinit_d2h_1d(void);
static void phyinit_d2h_2d(void);
static void phyinit_i(void);
static void phyinit_j(void);
static void update_mc(void);
static void	save_retcsr(void);
static void	restore_retcsr(void);

void ddr_setup(void)
{
	INFO("DDR: Setup (Rev. %s)\n", DDR_VERSION);
	INFO("DDR: Activate CPG1\n");
	cpg_active_ddr1();
	/* Wait at least the longest cycles of the next: 2 axiY_ACLK, 2 PCLK */
	wait_pclk(2);
	INFO("DDR: Initialize MC registers.\n");
	setup_mc();
	INFO("DDR: Activate CPG2\n");
	cpg_active_ddr2();
	/* Wait at least 2 PCLK cycles */
	wait_pclk(2);
	INFO("DDR: Initialize PHY Configuration.\n");
	phyinit_c();
	INFO("DDR: Run 1D training.\n");
	phyinit_d2h_1d();
	INFO("DDR: Run 2D training.\n");
	phyinit_d2h_2d();
	INFO("DDR: Save the retention registers.\n");
	save_retcsr();
	INFO("DDR: Load PHY Init Engine Image.\n");
	phyinit_i();
	INFO("DDR: Initialize the PHY to Mission Mode through DFI Initialization.\n");
	phyinit_j();
	INFO("DDR: Update MC registers.\n");
	update_mc();
}

void ddr_retention_entry(void)
{
	/* Disable PhyMaster */
	dwc_ddrphy_apb_wr(0x00058010, 0);
	/* Wait for MC to ready */
	DDRTOP_mc_param_poll(CONTROLLER_BUSY_ADDR, CONTROLLER_BUSY_OFFSET, CONTROLLER_BUSY_WIDTH, 0);
	/* Disable SREQ_REQ */
	mmio_write_32(SYS_DDR_MCAR_SRCTL0, mmio_read_32(SYS_DDR_MCAR_SRCTL0) & ~0x00000007);
	/* Enter SR LONG MC GATE */
	self_refresh_entry();
	/* DFI frequency change to LP3 */
	DDRTOP_mc_param_wr(DFIBUS_FREQ_F0_ADDR, DFIBUS_FREQ_F0_OFFSET, DFIBUS_FREQ_F0_WIDTH, 0x1f);
	/* dfi_init_start = 0 */
	mmio_write_32(SYS_DDR_MCAR_CTRL, mmio_read_32(SYS_DDR_MCAR_CTRL) | 0x00010000);
	/* while(DWC_DDRPHYA_APBONLY0_DfiInitCompleteShadow != 0) */
	dwc_ddrphy_apb_poll(0x0006E0fa, 0 << 0, 1 << 0);
	/* dfi_init_start = 1 */
	mmio_write_32(SYS_DDR_MCAR_CTRL, mmio_read_32(SYS_DDR_MCAR_CTRL) & ~0x00010000);
	/* while(DWC_DDRPHYA_APBONLY0_DfiInitCompleteShadow != 1) */
	dwc_ddrphy_apb_poll(0x0006E0fa, 1 << 0, 1 << 0);
	/* PwrOkIn = 0 */
	mmio_write_32(CPG_RST_DDR, 0x01000000);
	/* DDRPHY Power Donw Ctrl */
	mmio_write_32(PWRDN_DDRPHY_CTRL, 0x00000311);
	/* Wait 18 DfiClk */
	wait_dficlk(18);
}

void ddr_retention_exit(void)
{
	INFO("DDR: Retention Exit (Rev. %s)\n", DDR_VERSION);
	/* DDRPHY Power Donw Ctrl */
	mmio_write_32(PWRDN_DDRPHY_CTRL, 0x00000200);
	INFO("DDR: Activate CPG1\n");
	cpg_active_ddr1();
	/* Wait at least the longest cycles of the next: 2 axiY_ACLK, 2 PCLK */
	wait_pclk(2);
	INFO("DDR: Initialize MC registers.\n");
	setup_mc();
	INFO("DDR: Activate CPG2\n");
	cpg_active_ddr2();
	/* Wait at least 2 PCLK cycles */
	wait_pclk(2);
	INFO("DDR: Initialize PHY Configuration.\n");
	phyinit_c();
	INFO("DDR: Restore the saved retention registers.\n");
	restore_retcsr();
	INFO("DDR: Load PHY Init Engine Image.\n");
	phyinit_i();
	INFO("DDR: Initialize the PHY to Mission Mode through DFI Initialization.\n");
	phyinit_j();
	INFO("DDR: Update MC registers.\n");
	update_mc();
}

static void phyinit_c(void)
{
	/* Initialize PHY Configuration */
	phyinit_configuration();
	/* Program pin swizzling registers */
	phyinit_pin_swizzling();
}

static void phyinit_d2h_1d(void)
{
	/* Load the 1D image */
	phyinit_load_1d_image();
	/* Execute the Training Firmware */
	phyinit_exec_1d_image();
}

static void phyinit_d2h_2d(void)
{
	/* Load the 2D image */
	phyinit_load_2d_image();
	/* Execute the Training Firmware */
	phyinit_exec_2d_image();
}

static void phyinit_i(void)
{
	/* Load PHY Init Engine Image */
	phyinit_load_eng_image();
}

static void phyinit_j(void)
{
	DDRTOP_mc_param_wr(START_ADDR, START_OFFSET, START_WIDTH, 1);
	DDRTOP_mc_param_poll(INT_STATUS_INIT_ADDR, INT_STATUS_INIT_OFFSET + 1, 1, 1);
}

static void update_mc(void)
{
	DDRTOP_mc_param_wr(LPI_WAKEUP_EN_ADDR, LPI_WAKEUP_EN_OFFSET, LPI_WAKEUP_EN_WIDTH, 0x1F);
}

static void	save_retcsr(void)
{
	size_t i;

	/* Clear buffer */
	for (i = 0; i < ARRAY_SIZE(ddr_csr_table); i++)
		ddr_csr_table[i] = ~0x0;

	retcsr_read_registers(ddr_csr_table, sizeof(ddr_csr_table));
}

static void	restore_retcsr(void)
{
	retcsr_write_registers(ddr_csr_table, sizeof(ddr_csr_table));
}
