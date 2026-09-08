/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <common/debug.h>
#include <drivers/delay_timer.h>
#include "ddr_regs.h"
#include "rz_soc_def.h"
#include "cpg.h"
#include "ddr_private.h"


extern const char ddr_version_str[];


static void phyinit_c(void);
static void phyinit_d2h_1d(void);
static void phyinit_d2h_2d(void);
static void phyinit_mc(void);
static void phyinit_i(void);
static void phyinit_j(void);
static void prog_all0(uint64_t start_addr);


void ddr_setup(void)
{
	INFO("DDR: Setup (Rev.%s)\n", ddr_version_str);

	cpg_ddr_part1();

	setup_mc();

	cpg_ddr_part2();

	phyinit_c();
	phyinit_d2h_1d();
	phyinit_d2h_2d();

	phyinit_mc();

	phyinit_i();
	phyinit_j();

	prog_all0(0);

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

static void phyinit_i(void)
{
	phyinit_load_eng_image();
}

static void phyinit_j(void)
{
	ddrtop_mc_param_wr(START_ADDR, START_OFFSET, START_WIDTH, 1);
	ddrtop_mc_param_poll(INT_STATUS_INIT_ADDR, INT_STATUS_INIT_OFFSET+1, 1, 1);
}

static void phyinit_mc(void)
{
	uint32_t val, num_rank, num_byte, tctrl_delay, bl, x, tx_dqs_dly;

	dwc_ddrphy_apb_wr(0x0d0000, 0x0);

	val = ddrtop_mc_param_rd(CS_MAP_ADDR, CS_MAP_OFFSET, CS_MAP_WIDTH);
	num_rank = (val == 3) ? 2 : 1;
	val = ddrtop_mc_param_rd(MEM_DP_REDUCTION_ADDR, MEM_DP_REDUCTION_OFFSET, MEM_DP_REDUCTION_WIDTH);
	num_byte = (val == 1) ? 2 : 4;
	val = dwc_ddrphy_apb_rd(0x02002e);
	tctrl_delay = ((val >> 1) + (val & 1)) + 8;
	val = ddrtop_mc_param_rd(BSTLEN_ADDR, BSTLEN_OFFSET, BSTLEN_WIDTH);
	bl = (1 << val);

	x = 0;
	if (num_byte > 0) {
		val = dwc_ddrphy_apb_rd(0x010020); x = (val > x) ? val : x;
	}

	if (num_byte > 1) {
		val = dwc_ddrphy_apb_rd(0x011020); x = (val > x) ? val : x;
	}

	if (num_byte > 2) {
		val = dwc_ddrphy_apb_rd(0x012020); x = (val > x) ? val : x;
	}

	if (num_byte > 3) {
		val = dwc_ddrphy_apb_rd(0x013020); x = (val > x) ? val : x;
	}

	val = 16 + tctrl_delay + (2 * x);
	ddrtop_mc_param_wr(TDFI_PHY_RDLAT_F0_ADDR, TDFI_PHY_RDLAT_F0_OFFSET, TDFI_PHY_RDLAT_F0_WIDTH, val);

	x = 0;
	if ((num_rank > 0) && (num_byte > 0)) {
		val = dwc_ddrphy_apb_rd(0x0100d0); x = (val > x) ? val : x;
		val = dwc_ddrphy_apb_rd(0x0101d0); x = (val > x) ? val : x;
	}

	if ((num_rank > 0) && (num_byte > 1)) {
		val = dwc_ddrphy_apb_rd(0x0110d0); x = (val > x) ? val : x;
		val = dwc_ddrphy_apb_rd(0x0111d0); x = (val > x) ? val : x;
	}

	if ((num_rank > 0) && (num_byte > 2)) {
		val = dwc_ddrphy_apb_rd(0x0120d0); x = (val > x) ? val : x;
		val = dwc_ddrphy_apb_rd(0x0121d0); x = (val > x) ? val : x;
	}

	if ((num_rank > 0) && (num_byte > 3)) {
		val = dwc_ddrphy_apb_rd(0x0130d0); x = (val > x) ? val : x;
		val = dwc_ddrphy_apb_rd(0x0131d0); x = (val > x) ? val : x;
	}

	if ((num_rank > 1) && (num_byte > 0)) {
		val = dwc_ddrphy_apb_rd(0x0100d1); x = (val > x) ? val : x;
		val = dwc_ddrphy_apb_rd(0x0101d1); x = (val > x) ? val : x;
	}

	if ((num_rank > 1) && (num_byte > 1)) {
		val = dwc_ddrphy_apb_rd(0x0110d1); x = (val > x) ? val : x;
		val = dwc_ddrphy_apb_rd(0x0111d1); x = (val > x) ? val : x;
	}

	if ((num_rank > 1) && (num_byte > 2)) {
		val = dwc_ddrphy_apb_rd(0x0120d1); x = (val > x) ? val : x;
		val = dwc_ddrphy_apb_rd(0x0121d1); x = (val > x) ? val : x;
	}

	if ((num_rank > 1) && (num_byte > 3)) {
		val = dwc_ddrphy_apb_rd(0x0130d1); x = (val > x) ? val : x;
		val = dwc_ddrphy_apb_rd(0x0131d1); x = (val > x) ? val : x;
	}

	tx_dqs_dly = ((x>>6)&0xf) + (((x>>4)&0x01) + ((x>>3)&0x1));
	val = tctrl_delay + (6 + (bl / 2)) + tx_dqs_dly;
	ddrtop_mc_param_wr(TDFI_WRDATA_DELAY_ADDR, TDFI_WRDATA_DELAY_OFFSET, TDFI_WRDATA_DELAY_WIDTH, val);

	dwc_ddrphy_apb_wr(0x0d0000, 0x1);
}

static void prog_all0(uint64_t start_addr)
{
#if PLAT_DDR_ECC
	val = DDRTOP_mc_param_rd(CS_MAP_ADDR, CS_MAP_OFFSET, CS_MAP_WIDTH);
	if (val == 0x01) {
		end_addr17 = DDRTOP_mc_param_rd(CS_VAL_UPPER_0_ADDR, CS_VAL_UPPER_0_OFFSET, CS_VAL_UPPER_0_WIDTH);
	} else {
		end_addr17 = DDRTOP_mc_param_rd(CS_VAL_UPPER_1_ADDR, CS_VAL_UPPER_1_OFFSET, CS_VAL_UPPER_1_WIDTH);
	}
	if (end_addr17 == 0xdfff) {
		addr_space = 33;
	} else if (end_addr17 == 0x6fff) {
		addr_space = 32;
	} else if (end_addr17 == 0x37ff) {
		addr_space = 31;
	} else if (end_addr17 == 0x1bff) {
		addr_space = 30;
	} else if (end_addr17 == 0x0dff) {
		addr_space = 29;
	} else {
		addr_space = 33;
	}

	uint32_t bak_lp_auto_entry_en;

	ddrtop_mc_param_wr(ECC_DISABLE_W_UC_ERR_ADDR, ECC_DISABLE_W_UC_ERR_OFFSET, ECC_DISABLE_W_UC_ERR_WIDTH, 1);

	bak_lp_auto_entry_en = ddrtop_mc_param_rd(LP_AUTO_ENTRY_EN_ADDR, LP_AUTO_ENTRY_EN_OFFSET, LP_AUTO_ENTRY_EN_WIDTH);
	ddrtop_mc_param_wr(LP_AUTO_ENTRY_EN_ADDR, LP_AUTO_ENTRY_EN_OFFSET, LP_AUTO_ENTRY_EN_WIDTH, 0x0);

	ddrtop_mc_param_wr(BIST_START_ADDRESS_ADDR+0, 0, 32, (start_addr&0xffffffff));
	ddrtop_mc_param_wr(BIST_START_ADDRESS_ADDR+1, 0, BIST_START_ADDRESS_WIDTH-32, ((start_addr>>32)&0x0ffffffff));
	ddrtop_mc_param_wr(ADDR_SPACE_ADDR, ADDR_SPACE_OFFSET, ADDR_SPACE_WIDTH, addr_space);
	ddrtop_mc_param_wr(BIST_DATA_CHECK_ADDR, BIST_DATA_CHECK_OFFSET, BIST_DATA_CHECK_WIDTH, 1);
	ddrtop_mc_param_wr(BIST_TEST_MODE_ADDR, BIST_TEST_MODE_OFFSET, BIST_TEST_MODE_WIDTH, 0b100);
	ddrtop_mc_param_wr(BIST_DATA_PATTERN_ADDR+0, 0, 32, 0x00000000);
	ddrtop_mc_param_wr(BIST_DATA_PATTERN_ADDR+1, 0, 32, 0x00000000);
	ddrtop_mc_param_wr(BIST_DATA_PATTERN_ADDR+2, 0, 32, 0x00000000);
	ddrtop_mc_param_wr(BIST_DATA_PATTERN_ADDR+3, 0, 32, 0x00000000);

	udelay(1);

	ddrtop_mc_param_wr(BIST_GO_ADDR, BIST_GO_OFFSET, BIST_GO_WIDTH, 1);
	ddrtop_mc_param_poll(INT_STATUS_BIST_ADDR, INT_STATUS_BIST_OFFSET+0, 1, 1);
	ddrtop_mc_param_wr(BIST_GO_ADDR, BIST_GO_OFFSET, BIST_GO_WIDTH, 0);
	ddrtop_mc_param_wr(INT_ACK_BIST_ADDR, INT_ACK_BIST_OFFSET+0, 1, 1);
	ddrtop_mc_param_wr(INT_ACK_ECC_ADDR, INT_ACK_ECC_OFFSET, INT_ACK_ECC_WIDTH, 0x000001CF);
	ddrtop_mc_param_poll(INT_STATUS_BIST_ADDR, INT_STATUS_BIST_OFFSET+0, 1, 0);
	ddrtop_mc_param_poll(INT_STATUS_ECC_ADDR, INT_STATUS_ECC_OFFSET, INT_STATUS_ECC_WIDTH, 0);
	ddrtop_mc_param_wr(LP_AUTO_ENTRY_EN_ADDR, LP_AUTO_ENTRY_EN_OFFSET, LP_AUTO_ENTRY_EN_WIDTH, bak_lp_auto_entry_en);
	ddrtop_mc_param_wr(ECC_DISABLE_W_UC_ERR_ADDR, ECC_DISABLE_W_UC_ERR_OFFSET, ECC_DISABLE_W_UC_ERR_WIDTH, 0);

	udelay(1);
#endif
}
