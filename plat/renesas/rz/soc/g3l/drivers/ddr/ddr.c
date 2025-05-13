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

#define MCAR_CTRL		(0x400)

static void phyinit_c(void);
static void phyinit_d2h_1d(void);
static void phyinit_d2h_2d(void);
static void phyinit_mc(void);
static void phyinit_i(void);
static void phyinit_j(void);
static void	save_retcsr(void);
static void	restore_retcsr(void);
#if PLAT_DDR_ECC
	static void prog_all0(void);
#endif

extern const char ddr_version_str[];

void ddr_setup(void)
{
	INFO("DDR: Setup (Rev. %s)\n", ddr_version_str);
	cpg_active_ddr1();
	setup_mc();
	cpg_active_ddr2();
	phyinit_c();
	phyinit_d2h_1d();
	phyinit_d2h_2d();
	phyinit_mc();
	save_retcsr();
	phyinit_i();
	phyinit_j();
#if PLAT_DDR_ECC
	prog_all0();
#endif
	update_mc();
}

void ddr_retention_entry(void)
{
	uint32_t dram_class = DDRTOP_mc_param_rd(DRAM_CLASS_ADDR, DRAM_CLASS_OFFSET, DRAM_CLASS_WIDTH);

	DDRTOP_mc_param_poll(CONTROLLER_BUSY_ADDR, CONTROLLER_BUSY_OFFSET, CONTROLLER_BUSY_WIDTH, 0);

	DDRTOP_mc_param_wr(LP_AUTO_ENTRY_EN_ADDR, LP_AUTO_ENTRY_EN_OFFSET, LP_AUTO_ENTRY_EN_WIDTH, 0);
	DDRTOP_mc_param_wr(LPI_WAKEUP_EN_ADDR, LPI_WAKEUP_EN_OFFSET, LPI_WAKEUP_EN_WIDTH, 0);

	if (dram_class == 0b1011) {
		DDRTOP_mc_param_wr(LP_CMD_ADDR, LP_CMD_OFFSET, LP_CMD_WIDTH, 0b1010001);
		DDRTOP_mc_param_poll(LP_STATE_ADDR, LP_STATE_OFFSET, LP_STATE_WIDTH, 0b1001111);
	} else if (dram_class == 0b1010) {
		DDRTOP_mc_param_wr(LP_CMD_ADDR, LP_CMD_OFFSET, LP_CMD_WIDTH, 0b1010001);
		DDRTOP_mc_param_poll(LP_STATE_ADDR, LP_STATE_OFFSET, LP_STATE_WIDTH, 0b1001010);
	}


	DDRTOP_mc_param_wr(DFIBUS_FREQ_F0_ADDR, DFIBUS_FREQ_F0_OFFSET, DFIBUS_FREQ_F0_WIDTH, 0x1F);

	DDRTOP_mc_param_wr(MCAR_CTRL, 16, 1, 1);

	dwc_ddrphy_apb_poll(0x0006E0FA, 0 << 0, 1 << 0);

	DDRTOP_mc_param_wr(MCAR_CTRL, 16, 1, 0);

	dwc_ddrphy_apb_poll(0x0006E0FA, 1 << 0, 1 << 0);

	mmio_write_32(CPG_RST_DDR, 0x01000000);

	wait_dficlk(18);

	mmio_write_32(SYS_PWRDN_DDRPHY_CTRL, 0x00000311);

#if defined(PLAT_SYSTEM_SUSPEND_vbat)
	mmio_write_32(VBATT_BKPSR, 0x00000080);
#endif
}

void ddr_retention_exit(void)
{
	INFO("DDR: Retention Exit (Rev. %s)\n", ddr_version_str);
#if defined(PLAT_SYSTEM_SUSPEND_vbat)
	mmio_write_32(SYS_PWRDN_DDRPHY_CTRL, 0x00000311);
	mmio_write_32(VBATT_BKPSR, 0x00000000);
#elif defined(PLAT_SYSTEM_SUSPEND_awo)
	mmio_write_32(SYS_PWRDN_DDRPHY_CTRL, 0x00000301);
#endif

	wait_dficlk(18);
	mmio_write_32(SYS_PWRDN_DDRPHY_CTRL, 0x00000200);

	cpg_active_ddr1();
	setup_mc();
	cpg_active_ddr2();
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
	uint32_t val, num_rank, num_byte, tctrl_delay, bl, x, tx_dqs_dly, dram_class;

	dwc_ddrphy_apb_wr(0x6E000, 0x0);

	val = DDRTOP_mc_param_rd(CS_MAP_ADDR, CS_MAP_OFFSET, CS_MAP_WIDTH);
	num_rank = (val == 3) ? 2 : 1;
	val = DDRTOP_mc_param_rd(MEM_DP_REDUCTION_ADDR, MEM_DP_REDUCTION_OFFSET, MEM_DP_REDUCTION_WIDTH);
	num_byte = (val == 1) ? 1 : 2;
	val = dwc_ddrphy_apb_rd(0x05802e);
	tctrl_delay = ((val >> 1) + (val & 1)) + 6;
	val = DDRTOP_mc_param_rd(BSTLEN_ADDR, BSTLEN_OFFSET, BSTLEN_WIDTH);
	bl = (1 << val);
	dram_class = DDRTOP_mc_param_rd(DRAM_CLASS_ADDR, DRAM_CLASS_OFFSET, DRAM_CLASS_WIDTH);

	x = 0;
	if (num_byte > 0) {
		val = dwc_ddrphy_apb_rd(0x030020); x = (val > x) ? val : x;
	}

	if (num_byte > 1) {
		val = dwc_ddrphy_apb_rd(0x031020); x = (val > x) ? val : x;
	}

	val = 12 + tctrl_delay + (2 * x) + ((dram_class  == 0b1011) ? 2 : 1);
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

	if ((num_rank > 1) && (num_byte > 0)) {
		val = dwc_ddrphy_apb_rd(0x0300d1); x = (val > x) ? val : x;
		val = dwc_ddrphy_apb_rd(0x0301d1); x = (val > x) ? val : x;
	}

	if ((num_rank > 1) && (num_byte > 1)) {
		val = dwc_ddrphy_apb_rd(0x0310d1); x = (val > x) ? val : x;
		val = dwc_ddrphy_apb_rd(0x0311d1); x = (val > x) ? val : x;
	}

	tx_dqs_dly = ((x >> 6) & 0xf) + ((x & 0x1f) > 0 ? 1 : 0);
	val = tctrl_delay + (6 + (bl >> 1)) + tx_dqs_dly;
	DDRTOP_mc_param_wr(TDFI_WRDATA_DELAY_ADDR, TDFI_WRDATA_DELAY_OFFSET, TDFI_WRDATA_DELAY_WIDTH, val);

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

#if PLAT_DDR_ECC
static void DDRTOP_bist_prog0(uint64_t start_address, uint32_t addr_space)
{
	uint32_t bak_lp_auto_entry_en;

	ddrtop_mc_param_wr(ECC_DISABLE_W_UC_ERR_ADDR, ECC_DISABLE_W_UC_ERR_OFFSET, ECC_DISABLE_W_UC_ERR_WIDTH, 1);

	bak_lp_auto_entry_en = ddrtop_mc_param_rd(LP_AUTO_ENTRY_EN_ADDR, LP_AUTO_ENTRY_EN_OFFSET, LP_AUTO_ENTRY_EN_WIDTH);
	ddrtop_mc_param_wr(LP_AUTO_ENTRY_EN_ADDR, LP_AUTO_ENTRY_EN_OFFSET, LP_AUTO_ENTRY_EN_WIDTH, 0x0);

	ddrtop_mc_param_wr(BIST_START_ADDRESS_ADDR + 0, 0, 32, (start_address & 0xffffffff));
	ddrtop_mc_param_wr(BIST_START_ADDRESS_ADDR + 1, 0, BIST_START_ADDRESS_WIDTH - 32, ((start_address >> 32) & 0x0ffffffff));
	ddrtop_mc_param_wr(ADDR_SPACE_ADDR, ADDR_SPACE_OFFSET, ADDR_SPACE_WIDTH, addr_space);
	ddrtop_mc_param_wr(BIST_DATA_CHECK_ADDR, BIST_DATA_CHECK_OFFSET, BIST_DATA_CHECK_WIDTH, 1);
	ddrtop_mc_param_wr(BIST_TEST_MODE_ADDR, BIST_TEST_MODE_OFFSET, BIST_TEST_MODE_WIDTH, 0b100);
	ddrtop_mc_param_wr(BIST_DATA_PATTERN_ADDR + 0, 0, 32, 0x00000000);
	ddrtop_mc_param_wr(BIST_DATA_PATTERN_ADDR + 1, 0, 32, 0x00000000);

	wait_regaclk(10);

	ddrtop_mc_param_wr(BIST_GO_ADDR, BIST_GO_OFFSET, BIST_GO_WIDTH, 1);
	ddrtop_mc_param_poll(INT_STATUS_BIST_ADDR, INT_STATUS_BIST_OFFSET + 0, 1, 1);
	ddrtop_mc_param_wr(BIST_GO_ADDR, BIST_GO_OFFSET, BIST_GO_WIDTH, 0);
	ddrtop_mc_param_wr(INT_ACK_BIST_ADDR, INT_ACK_BIST_OFFSET + 0, 1, 1);
	ddrtop_mc_param_wr(INT_ACK_ECC_ADDR, INT_ACK_ECC_OFFSET, INT_ACK_ECC_WIDTH, 0x000001CF);
	ddrtop_mc_param_poll(INT_STATUS_BIST_ADDR, INT_STATUS_BIST_OFFSET + 0, 1, 0);
	ddrtop_mc_param_poll(INT_STATUS_ECC_ADDR, INT_STATUS_ECC_OFFSET, INT_STATUS_ECC_WIDTH, 0);
	ddrtop_mc_param_wr(LP_AUTO_ENTRY_EN_ADDR, LP_AUTO_ENTRY_EN_OFFSET, LP_AUTO_ENTRY_EN_WIDTH, bak_lp_auto_entry_en);
	ddrtop_mc_param_wr(ECC_DISABLE_W_UC_ERR_ADDR, ECC_DISABLE_W_UC_ERR_OFFSET, ECC_DISABLE_W_UC_ERR_WIDTH, 0);

	wait_regaclk(10);
}

static void prog_all0(void)
{
	uint32_t val = ddrtop_mc_param_rd(CS_MAP_ADDR, CS_MAP_OFFSET, CS_MAP_WIDTH);
	uint32_t end_addr_u16;

	if (val == 0x01) {
		end_addr_u16 = ddrtop_mc_param_rd(CS_VAL_UPPER_0_ADDR, CS_VAL_UPPER_0_OFFSET, CS_VAL_UPPER_0_WIDTH);
	} else {
		end_addr_u16 = ddrtop_mc_param_rd(CS_VAL_UPPER_1_ADDR, CS_VAL_UPPER_1_OFFSET, CS_VAL_UPPER_1_WIDTH);
	}

	uint32_t addr_space = 0;

	for (uint8_t i = 10; i < 16; i++) {
		if ((end_addr_u16 & (1 << i)) != 0) {
			addr_space = 18 + i;
		}
	}

	DDRTOP_bist_prog0(0, addr_space);
}
#endif
