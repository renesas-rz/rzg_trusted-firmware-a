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
#include <ddr.h>


#define MCAR_CTL				0x800


extern const uint32_t retention_phyreglist_1d[];
extern const uint32_t retention_phyreglist_2d[];
extern const uint32_t retention_mcreglist[];
extern const uint32_t retention_phyreglist_1d_size;
extern const uint32_t retention_phyreglist_2d_size;
extern const uint32_t retention_mcreglist_size;


static void ddr_init(uint64_t ddraddr);
static void phyinit_c(void);
static void phyinit_d2h_1d(void);
static void phyinit_d2h_2d(void);
static void phyinit_mc(void);
static void phyinit_i(void);
static void phyinit_j(void);
static void prog_all0(uint64_t start_addr, uint32_t addr_space);
static void save_retcsr(void);


void ddr_setup(void)
{
	INFO("DDR: Setup (Rev. %s)\n", DDR_VERSION);
	ddr_init(RZV2H_DDR0_BASE);
	ddr_init(RZV2H_DDR1_BASE);
}


static void ddr_init(uint64_t ddrbase)
{
	if (ddrbase == RZV2H_DDR0_BASE) {
		set_ddrtop_mc_base_addr(RZV2H_DDR0_MEMC_BASE);
		set_ddrphy_base_addr(RZV2H_DDR0_PHY_BASE);

		cpg_ddr0_part1();

		setup_mc();

		cpg_ddr0_part2();
	} else if (ddrbase == RZV2H_DDR1_BASE) {
		set_ddrtop_mc_base_addr(RZV2H_DDR1_MEMC_BASE);
		set_ddrphy_base_addr(RZV2H_DDR1_PHY_BASE);

		cpg_ddr1_part1();

		setup_mc();

		cpg_ddr1_part2();
	} else {
		panic();
	}

	phyinit_c();

	phyinit_d2h_1d();

	phyinit_d2h_2d();

	phyinit_mc();

	save_retcsr();

	phyinit_i();

	phyinit_j();

	prog_all0(ddrbase, 33);

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

	tx_dqs_dly = ((x >> 6) & 0xf) + (((x >> 4) & 0x01) + ((x >> 3) & 0x1));
	val = tctrl_delay + (6 + (bl / 2)) + tx_dqs_dly;
	ddrtop_mc_param_wr(TDFI_WRDATA_DELAY_ADDR, TDFI_WRDATA_DELAY_OFFSET, TDFI_WRDATA_DELAY_WIDTH, val);

	dwc_ddrphy_apb_wr(0x0d0000, 0x1);
}

static void save_retcsr(void)
{
	static bool saved;
	uint32_t i, j = 0;

	dwc_ddrphy_apb_wr(0x0d0000, 0);
	dwc_ddrphy_apb_wr(0x0c0080, 3);

	if (!saved) {
		/* Clear buffer */
		for (i = 0; i < ARRAY_SIZE(ddr_csr_table); i++)
			ddr_csr_table[i] = ~0x0;

		/* Read all the retention registers, and save them to the storage other than DRAM. */
		for (i = 0; i < retention_phyreglist_1d_size; i++, j++) {
			ddr_csr_table[j] = dwc_ddrphy_apb_rd(retention_phyreglist_1d[i]);
		}

		for (i = 0; i < retention_phyreglist_2d_size; i++, j++) {
			ddr_csr_table[j] = dwc_ddrphy_apb_rd(retention_phyreglist_2d[i]);
		}

		for (i = 0; i < retention_mcreglist_size; i++, j++) {
			ddr_csr_table[j] = ddrtop_mc_apb_rd(retention_mcreglist[i]);
		}

		saved = true;
	}

	dwc_ddrphy_apb_wr(0x0c0080, 2);
	dwc_ddrphy_apb_wr(0x0d0000, 1);
}

static void restore_retcsr(void)
{
	uint32_t i, j = 0;

	dwc_ddrphy_apb_wr(0x0d0000, 0);
	dwc_ddrphy_apb_wr(0x0c0080, 3);

	for (i = 0; i < retention_phyreglist_1d_size; i++, j++) {
		dwc_ddrphy_apb_wr(retention_phyreglist_1d[i], ddr_csr_table[j]);
	}

	for (i = 0; i < retention_phyreglist_2d_size; i++, j++) {
		dwc_ddrphy_apb_wr(retention_phyreglist_2d[i], ddr_csr_table[j]);
	}

	for (i = 0; i < retention_mcreglist_size; i++, j++) {
		dwc_ddrphy_apb_wr(retention_mcreglist[i], ddr_csr_table[j]);
	}

	dwc_ddrphy_apb_wr(0x0c0080, 2);
	dwc_ddrphy_apb_wr(0x0d0000, 1);
}

static void prog_all0(uint64_t start_addr, uint32_t addr_space)
{
#if PLAT_DDR_ECC
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

static void soft_delay(uint64_t usec)
{
	/* RZ/V2H: CPU Clock = 1.7G Hz*/
	const uint32_t cpuclk_freq = 1700000000;
	/* If the number of nop clock cycles is 4 */
	const uint32_t nop_clk_cycles = 4;
	/* Number of NOP instructions required for 1us */
	const uint32_t num_of_nop_needed = cpuclk_freq / (nop_clk_cycles * 1000000);

	volatile uint64_t timeout = num_of_nop_needed * usec;

	while (timeout--) {
		__asm__ ("nop");
		dsb();
	}
}

void wait_dficlk(uint32_t cycles)
{
	const uint32_t dficlk_freq = 400000000; /* dfiCLK = 400MHz */

	soft_delay((((uint64_t)cycles * 1000000) / dficlk_freq) + 1);
}

void wait_pclk(uint32_t cycles)
{
	const uint32_t pclk_freq = 100000000; /* PCLK = 100MHz */

	soft_delay((((uint64_t)cycles * 1000000) / pclk_freq) + 1);
}

static void dwc_ddrphy_apb_poll(uint32_t addr, uint32_t data, uint32_t mask)
{
	uint32_t tmp_data;

	tmp_data = dwc_ddrphy_apb_rd(addr);
	tmp_data &= mask;
	while (tmp_data != data) {
		wait_pclk(10);
		tmp_data = dwc_ddrphy_apb_rd(addr);
		tmp_data &= mask;
	}
}

static void ddr_retention_enter(uint8_t base)
{
	uint32_t val, num_rank;

	if (!base) {
		set_ddrtop_mc_base_addr(RZV2H_DDR0_MEMC_BASE);
		set_ddrphy_base_addr(RZV2H_DDR0_PHY_BASE);
	} else if (base == 1) {
		set_ddrtop_mc_base_addr(RZV2H_DDR1_MEMC_BASE);
		set_ddrphy_base_addr(RZV2H_DDR1_PHY_BASE);
	}
	/* 1. */

	/* 2. */
	val = ddrtop_mc_param_rd(CS_MAP_ADDR, CS_MAP_OFFSET, CS_MAP_WIDTH);
	num_rank = (val == 3) ? 2 : 1;

	/* 3. */
	dwc_ddrphy_apb_wr(0x020010, 0);

	/* 4. */
	ddrtop_mc_param_poll(CONTROLLER_BUSY_ADDR, CONTROLLER_BUSY_OFFSET, CONTROLLER_BUSY_WIDTH, 0);

	/* 5. */
	ddrtop_mc_param_wr(LP_AUTO_ENTRY_EN_ADDR, LP_AUTO_ENTRY_EN_OFFSET, LP_AUTO_ENTRY_EN_WIDTH, 0);
	ddrtop_mc_param_wr(LPI_WAKEUP_EN_OFFSET, LPI_WAKEUP_EN_OFFSET, LPI_WAKEUP_EN_WIDTH, 0);

	/* 6. */
	ddrtop_mc_param_wr(LP_CMD_ADDR, LP_CMD_OFFSET, LP_CMD_WIDTH, 0b1010001);
	ddrtop_mc_param_poll(LP_STATE_CS0_ADDR, LP_STATE_CS0_OFFSET, LP_STATE_CS0_WIDTH, 0b1001111);
	if (num_rank > 1) {
		ddrtop_mc_param_poll(LP_STATE_CS1_ADDR, LP_STATE_CS1_OFFSET, LP_STATE_CS1_WIDTH, 0b1001111);
	}

	/* 7. */
	if (!base) {
		ddrtop_mc_param_wr(DFIBUS_FREQ_F0_ADDR, DFIBUS_FREQ_F0_OFFSET, DFIBUS_FREQ_F0_WIDTH, 0x1F);
	} else {
		ddrtop_mc_param_wr(DFIBUS_FREQ_F0_ADDR, DFIBUS_FREQ_F1_OFFSET, DFIBUS_FREQ_F1_WIDTH, 0x1F);
	}

	/* 8. */
	ddrtop_mc_param_wr(MCAR_CTL, 16, 1, 1);

	/* 9. */
	dwc_ddrphy_apb_poll(0x0D00FA, 0, 1);

	/* 10. */
	ddrtop_mc_param_wr(MCAR_CTL, 16, 1, 0);

	/* 11. */
	dwc_ddrphy_apb_poll(0x0D00FA, 1, 1);

	/* 12. */
	cpg_ddr_pwrokin_off(base);

	/* 13. */
	wait_dficlk(18);
}

void ddr_retention_entry(void)
{
	ddr_retention_enter(1);
	ddr_retention_enter(0);
}

void ddr_retention_exit(uint8_t base)
{
	if (!base) {
		set_ddrtop_mc_base_addr(RZV2H_DDR0_MEMC_BASE);
		set_ddrphy_base_addr(RZV2H_DDR0_PHY_BASE);
		/* 1. */

		/* 2. to 8. */
		cpg_ddr0_part1();

		/* 9. */
		setup_mc();

		/* 10. */

		/* 11. to 14*/
		cpg_ddr0_part2();

	} else if (base == 1) {
		set_ddrtop_mc_base_addr(RZV2H_DDR1_MEMC_BASE);
		set_ddrphy_base_addr(RZV2H_DDR1_PHY_BASE);
		/* 1. */

		/* 2. to 8. */
		cpg_ddr1_part1();

		/* 9. */
		setup_mc();

		/* 10. */

		/* 11. to 14*/
		cpg_ddr1_part2();
	}

	/* 15. */
	phyinit_c();

	/* 16. */
	restore_retcsr();

	/* 17. */
	phyinit_i();

	/* 18. */
	phyinit_j();

	/* 19. */
	update_mc();
}
