/*
 * Copyright (c) 2024, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <drivers/delay_timer.h>
#include <common/debug.h>

#include "sys_regs.h"
#include "sys.h"

#include "include/RZx/DDR/RZT2N_DDRTOP.h"

#include "include/RZx/DDR/RZT2N_DDRTOP_iecc.h"


const char ddr_driver_version_str[] = "01.00";

uint8_t unswizzle_bit(uint8_t dq_bit, uint8_t lane)
{
	return dq_bit;
}

uint8_t swizzle_bit(uint8_t dq_bit, uint8_t lane)
{
	uint8_t swizzle_bit;

	swizzle_bit = dq_bit;
	for (int i = 0; i < 8; i = i + 1) {
		swizzle_bit = (unswizzle_bit(i, lane) == dq_bit ? i : swizzle_bit);
	}
	return swizzle_bit;
}

uint8_t bit_unswizzle(uint8_t int_ip_dq, uint8_t lane)
{
	uint8_t bit_unswizzle = 0;

	for (int i = 0; i < 8; i = i + 1) {
		bit_unswizzle = bit_unswizzle | (((int_ip_dq >> (swizzle_bit(i, lane))) & 1) << i);
	}
	return bit_unswizzle;
}
uint8_t bit_swizzle(uint8_t op_dq, uint8_t lane)
{
	uint8_t bit_swizzle = 0;

	for (int i = 0; i < 8; i = i + 1) {
		bit_swizzle = bit_swizzle | (((op_dq >> (unswizzle_bit(i, lane))) & 1) << i);
	}
	return bit_swizzle;
}

uint8_t ext_unswizzle_ca(unsigned int ca_bit)
{
	uint8_t ext_unswizzle_ca;

	ext_unswizzle_ca = param_ddr_swizzle_ca[ca_bit];
	return ext_unswizzle_ca;
}

uint8_t ext_unswizzle_cs(unsigned int cs_bit)
{
	uint8_t ext_unswizzle_cs;

	ext_unswizzle_cs = param_ddr_swizzle_cs[cs_bit];
	return ext_unswizzle_cs;
}

uint8_t ext_unswizzle_bit(uint8_t dq_bit, uint8_t lane)
{
	uint8_t ext_unswizzle_bit;

	ext_unswizzle_bit = param_ddr_swizzle_bit[(lane * 8) + dq_bit] % 8;
	return ext_unswizzle_bit;
}

uint8_t ext_swizzle_bit(uint8_t dq_bit, uint8_t lane)
{
	uint8_t ext_swizzle_bit;

	ext_swizzle_bit = dq_bit;
	for (int i = 0; i < 8; i = i + 1) {
		ext_swizzle_bit = (ext_unswizzle_bit(i, lane) == dq_bit ? i : ext_swizzle_bit);
	}
	return ext_swizzle_bit;
}

uint8_t ext_bit_unswizzle(uint8_t ip_dq, uint8_t lane)
{
	uint8_t ext_bit_unswizzle = 0;

	for (int i = 0; i < 8; i = i + 1) {
		ext_bit_unswizzle = ext_bit_unswizzle | (((ip_dq >> ext_swizzle_bit(i, lane)) & 1) << i);
	}
	return ext_bit_unswizzle;
}
uint8_t ext_bit_swizzle(uint8_t op_dq, uint8_t lane)
{
	uint8_t ext_bit_swizzle = 0;

	for (int i = 0; i < 8; i = i + 1) {
		ext_bit_swizzle = ext_bit_swizzle | (((op_dq >> ext_unswizzle_bit(i, lane)) & 1) << i);
	}
	return ext_bit_swizzle;
}

uint8_t ext_swizzle_ca(unsigned int ca_bit)
{
	uint8_t ext_swizzle_ca;

	ext_swizzle_ca = ca_bit;
	for (int i = 0; i < 6; i = i + 1) {
		ext_swizzle_ca = (ext_unswizzle_ca(i) == ca_bit ? i : ext_swizzle_ca);
	}
	return ext_swizzle_ca;
}
uint8_t ext_swizzle_cs(unsigned int cs_bit)
{
	uint8_t ext_swizzle_cs;

	ext_swizzle_cs = cs_bit;
	for (int i = 0; i < 2; i = i + 1) {
		ext_swizzle_cs = (ext_unswizzle_cs(i) == cs_bit ? i : ext_swizzle_cs);
	}
	return ext_swizzle_cs;
}

uint8_t sw_bit_unswizzle(uint8_t ip_dq, uint8_t lane)
{
	uint8_t sw_bit_unswizzle;

	sw_bit_unswizzle = bit_unswizzle(ext_bit_unswizzle(ip_dq, lane), lane);
	return sw_bit_unswizzle;
}
uint8_t sw_bit_swizzle(uint8_t op_dq, uint8_t lane)
{
	uint8_t sw_bit_swizzle;

	sw_bit_swizzle = ext_bit_swizzle(bit_swizzle(op_dq, lane), lane);
	return sw_bit_swizzle;
}

void DDRTOP_proc_MC_APBPOLL(uint32_t addr, uint32_t data, uint32_t mask)
{
	uint32_t rddata;

	rddata = mmio_read_32(addr) & mask;
	while (rddata != data) {
		udelay(1);
		rddata = mmio_read_32(addr) & mask;
	}
}
void DDRTOP_proc_PHY_APBPOLL(uint32_t addr, uint32_t data, uint32_t mask)
{
	uint32_t rddata;

	rddata = mmio_read_32(addr) & mask;
	while (rddata != data) {
		udelay(1);
		rddata = mmio_read_32(addr) & mask;
	}
}


void DDRTOP_mc_apb_wr(uint32_t addr, uint32_t data)
{
	mmio_write_32(RZT2N_DDR_MEMC_BASE + addr * 4, data);
}

uint32_t DDRTOP_mc_apb_rd(uint32_t addr)
{
	return mmio_read_32(RZT2N_DDR_MEMC_BASE + addr * 4);
}

void DDRTOP_mc_apb_rmw(uint32_t addr, uint32_t data, uint32_t mask)
{
	uint32_t tmp_data;

	tmp_data = DDRTOP_mc_apb_rd(addr);
	data = (data & mask) | (tmp_data & (~mask));
	DDRTOP_mc_apb_wr(addr, data);
}

void DDRTOP_mc_apb_poll(uint32_t addr, uint32_t data, uint32_t mask)
{
	uint32_t tmp_data;

	tmp_data = DDRTOP_mc_apb_rd(addr);
	tmp_data &= mask;
	while (tmp_data != data) {
		udelay(1);
		tmp_data = DDRTOP_mc_apb_rd(addr);
		tmp_data &= mask;
	}
}

void DDRTOP_mc_param_wr(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data)
{
	uint32_t tmp_data;
	uint32_t tmp_mask;

	tmp_data = data << offset;
	tmp_mask = (width < 32) ? ((1UL << width) - 1) << offset : 0xFFFFFFFFUL;
	DDRTOP_mc_apb_rmw(addr, tmp_data, tmp_mask);
}

uint32_t DDRTOP_mc_param_rd(uint32_t addr, uint32_t offset, uint32_t width)
{
	uint32_t data;
	uint32_t tmp_data;
	uint32_t tmp_mask;

	tmp_data = DDRTOP_mc_apb_rd(addr);
	tmp_mask = (width < 32) ? ((1UL << width) - 1) << offset : 0xFFFFFFFFUL;
	data = (tmp_data & tmp_mask) >> offset;
	return data;
}

void DDRTOP_mc_param_poll(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data)
{
	uint32_t tmp_data;
	uint32_t tmp_mask;

	tmp_data = data << offset;
	tmp_mask = (width < 32) ? ((1UL << width) - 1) << offset : 0xFFFFFFFFUL;
	DDRTOP_mc_apb_poll(addr, tmp_data, tmp_mask);
}

/**
 * @brief Issue MRW command to DRAM through Memory Controller
 * @param mr Mode Register Number
 * @param op opcode for MR
 * @param cs Ranks for all rank MRW
 */
void DDRTOP_MC_MRW(uint8_t mr, uint8_t op, uint8_t cs)
{
	uint32_t wrdata;

	DDRTOP_mc_param_wr(MRSINGLE_DATA_0_ADDR, MRSINGLE_DATA_0_OFFSET, MRSINGLE_DATA_0_WIDTH, op);
	DDRTOP_mc_param_wr(MRSINGLE_DATA_1_ADDR, MRSINGLE_DATA_1_OFFSET, MRSINGLE_DATA_1_WIDTH, op);

	wrdata = mr <<  0 |
			 cs <<  8 |
			  1 << 23 |
			  0 << 24 |
			  1 << 25;
	DDRTOP_mc_param_wr(WRITE_MODEREG_ADDR, WRITE_MODEREG_OFFSET, WRITE_MODEREG_WIDTH, wrdata);
}

/**
 * @brief Switch Frequency Set Point
 * @param target_fsp the target FSP number
 */
void freq_change_init_lp4(unsigned int target_fsp)
{
	/* dfi_dram_clk_disable=1 */
	sys_read_modify_32bits(PHY_CTRL0, 1 << 4, (1 << 4));
	udelay(1);
	DDRTOP_proc_switch_fsp(target_fsp);

	{
		/* re-calibrate master DLL after clock frequency change */
		mmio_write_32(PHY_DLLM_WINDOW_SIZE, (0 << 0) | (1 << 8));
		DDRTOP_proc_PHY_APBPOLL(PHY_DLLM_WINDOW_SIZE, 0, (1 << 8) | (DDRTOP_SCL_LANES << 12));

		mmio_write_32(PHY_DLLM_WINDOW_SIZE, (0 << 0) | (1 << 8));
		DDRTOP_proc_PHY_APBPOLL(PHY_DLLM_WINDOW_SIZE, 0, (1 << 8) | (DDRTOP_SCL_LANES << 12));

		mmio_write_32(PHY_PHY_DLL_FORCE_CALIB, 1);
		mmio_write_32(PHY_DPHY_DLL_FORCE_CALIB, DDRTOP_SCL_LANES);

		DDRTOP_proc_PHY_APBPOLL(PHY_PHY_DLL_FORCE_CALIB, 0, 1);
		DDRTOP_proc_PHY_APBPOLL(PHY_DPHY_DLL_FORCE_CALIB, 0, DDRTOP_SCL_LANES);

		mmio_write_32(PHY_DLLM_WINDOW_SIZE, 4 << 0);
	}

	/* dfi_dram_clk_disable=0 */
	sys_read_modify_32bits(PHY_CTRL0, 0 << 4, (1 << 4));
	udelay(1);
}

/**
 * @brief Change clock frequency from SYS
 * @param target_fsp the target FSP number
 */
void DDRTOP_proc_switch_fsp(unsigned int target_fsp)
{
	sys_base_unlock(PRCRx_CLOCK_GEN);
	/* SYS_CTRL::SCKCR4
	 * - b28     : DDRCLKSEL : 0x0:ddr_clk=12.5kHz, 0x1:(Refer DDRDIVSEL)
	 * - b31-b29 : DDRDIVSEL : 0x0:ddr_clk= 800MHz, 0x4:ddr_clk=50MHz
	 */
	if (target_fsp == 1) {
		sys_read_modify_32bits(SCKCR4, (0x0U << 29) | (0x1U << 28), (0x7U << 29) | (0x1U << 28));
	} else {
		sys_read_modify_32bits(SCKCR4, (0x4U << 29) | (0x1U << 28), (0x7U << 29) | (0x1U << 28));
	}
	/* wait for clock change by dummy read */
	mmio_read_32(SCKCR4);
	sys_base_unlock(PRCRx_CLOCK_GEN);
}

/**
 * @brief Release DDR MC and PHY reset
 */
void DDRTOP_init_reset_release(void)
{
	/* DDR Module Reset Release: SYS_CTRL::MRCTLM */
	sys_base_unlock(PRCRx_LOW_POWER);
	/* Assert all DDR reset */
	sys_read_modify_32bits(MRCTLM, 0x031D << 16, 0x031D << 16);

	/* De-assert DDR reset in order */
	udelay(1);                                  /* 5 half_rate_clk or more */
	sys_read_modify_32bits(MRCTLM, 0 << 16, 1 << 16); /* rst_n */
	udelay(1);                                  /* 2 half_rate_clk or more */
	sys_read_modify_32bits(MRCTLM, 0 << 19, 3 << 19); /* axi0 & axi1 */
	sys_read_modify_32bits(MRCTLM, 0 << 24, 1 << 24); /* MC_PRESETn */
	udelay(2);                                  /* 40 half_rate_clk or more */
	sys_read_modify_32bits(MRCTLM, 0 << 25, 1 << 25); /* PHY_RESETn */
	udelay(1);
	sys_read_modify_32bits(PHY_CTRL0, 1 << 0, 1 << 0);         /* turn_off_adrctrl_drivers = 1 */
	sys_read_modify_32bits(MRCTLM, 0 << 18, 1 << 18); /* reset_n */
	sys_read_modify_32bits(PHY_CTRL0, 1 << 16, 1 << 16);       /* ctl_presetn = 1 */
	sys_base_lock(PRCRx_LOW_POWER);
}

/**
 * @brief PHY register settings after training
 */
void DDRTOP_init_phy_update_after_training(void)
{

	const uint32_t DSCL_SHORTEN = 0;
	const uint32_t BIT_LVL_WR_DYNAMIC = 1;
	const uint32_t BIT_LVL_DYNAMIC = 1;
	const uint32_t DLL_RECALIB_CNT = 0x3FFFF;
	const uint32_t AUTO_SCL_ENABLE = 0;
	const uint32_t ALT_CTRLUPD_DLL_DSCL = 0;
	const uint32_t PHY_AUTO_SCL = 1;
	const uint32_t BIT_LVL_FROM_SAVED = 1;
	const uint32_t DSCL_INTERVAL = 0x3D09;

	uint32_t num_ranks;
	uint32_t rddata;

	rddata = DDRTOP_mc_param_rd(CS_MAP_ADDR, CS_MAP_OFFSET, CS_MAP_WIDTH);
	num_ranks = (rddata == 3) ? 2 : 1;

	/* Enable xBTI countermeasure */
	mmio_write_32(PHY_WRIDLE1, 3 << 16 | 3 << 8 | 3 << 0);
	if (num_ranks == 2) {
		mmio_write_32(PHY_WRIDLE2, 3 << 16 | 3 << 8 | 3 << 0);
	}
	mmio_write_32(PHY_WRIDLE0, 7 << 16 | 1 << 0);

	/* DABC step size modification */
	sys_read_modify_32bits(PHY_WRLVL_STEP_SIZE, 2 << 0, 0b11111111 << 0);

	/* SCL setting */
	mmio_write_32(PHY_SCL_START, (0 << 24) | (0 << 25) | (BIT_LVL_FROM_SAVED << 21));

	/* Dynamic read and write bit-leveling */
	sys_read_modify_32bits(PHY_DYNAMIC_WRITE_BIT_LVL, BIT_LVL_WR_DYNAMIC << 0, (1 << 0));
	sys_read_modify_32bits(PHY_DYNAMIC_BIT_LVL, BIT_LVL_DYNAMIC << 0, (1 << 0));

	/* Setup multi-rank dynamic calibration settings */
	mmio_write_32(PHY_RANK_SEL, DDRTOP_PRIMARY_RANK <<  8 |
						  (((num_ranks - 1) != 0) ? 2 : 1) <<  4 |
														 0 <<  0);

	sys_read_modify_32bits(PHY_PHY_DLL_RECALIB, (DLL_RECALIB_CNT << 8) | ((AUTO_SCL_ENABLE && ALT_CTRLUPD_DLL_DSCL) << 26), ((((1 << 18) - 1) << 8) | (1 << 26)));

	/* Setup DSCL parameters after second rank has been calibrated */
	mmio_write_32(PHY_DSCL_CNT, (DSCL_SHORTEN  << 26) |
						  (PHY_AUTO_SCL  << 24) |
						  (DSCL_INTERVAL <<  0));
}

/**
 * @brief MC register settings after training
 */
void DDRTOP_init_mc_update_after_training(void)
{
	uint32_t rddata;
	uint32_t num_ranks;
	uint8_t main_clk_dly;
	uint8_t main_clk_dly_pre;
	uint8_t tphy_rdlat;

	rddata = DDRTOP_mc_param_rd(CS_MAP_ADDR, CS_MAP_OFFSET, CS_MAP_WIDTH);
	num_ranks = (rddata == 3) ? 2 : 1;

	/* Update DFI timing after SCL */
	mmio_write_32(PHY_PHY_LANE_SEL, 0);
	rddata = mmio_read_32(PHY_SCL_LATENCY);
	main_clk_dly = rddata >> 16;

	if (num_ranks == 2) {
		/* Use larger main_clk_dly value among ranks */
		mmio_write_32(PHY_PHY_LANE_SEL, 1 << 16);
		rddata = mmio_read_32(PHY_SCL_LATENCY);
		main_clk_dly_pre = rddata >> 16;

		if (main_clk_dly_pre > main_clk_dly) {
			main_clk_dly = main_clk_dly_pre;
		}
		mmio_write_32(PHY_PHY_LANE_SEL, 0);
	}

	tphy_rdlat = (main_clk_dly + 2 + 2) * 2 + 2;

	DDRTOP_mc_param_wr(TDFI_PHY_RDLAT_F1_ADDR, TDFI_PHY_RDLAT_F1_OFFSET, TDFI_PHY_RDLAT_F1_WIDTH, tphy_rdlat);

	/* Enable interrupts */
	DDRTOP_mc_param_wr(INT_MASK_MASTER_ADDR, INT_MASK_MASTER_OFFSET + 31, 1, 0);

	/* Enable lp_auto */
	rddata = (DDRTOP_mc_apb_rd(0xa1) >> 8) & 0x1;
	if (rddata == 0) {
		DDRTOP_mc_apb_rmw(0x9b, 0x100 << 16 | 0x1 << 0, 0xfff << 16 | 0xf << 0);
		DDRTOP_mc_apb_rmw(0x9a, 0x1 << 24, 0xf << 24);
	}
}

/**
 * @brief Initialize HW-DFS parameter and start HW-DFS. Run until dfi_init_start=1
 * @param target_fsp
 * @return result of HWDFS start
 */
int DDRTOP_init_hwdfs_start(unsigned int target_fsp)
{
	int ret = DDRTOP_OK;

	uint32_t rddata;
	uint32_t rddata2;

	rddata  = DDRTOP_mc_param_rd(MR3_DATA_F0_0_ADDR, MR3_DATA_F0_0_OFFSET, MR3_DATA_F0_0_WIDTH);
	rddata2 = DDRTOP_mc_param_rd(MR3_DATA_F1_0_ADDR, MR3_DATA_F1_0_OFFSET, MR3_DATA_F1_0_WIDTH);
	if (rddata != rddata2) {
		ERROR("MR3 register mismatch in rank 0: F0=0x%08x, F1=0x%08x\n", rddata, rddata2);
		return DDRTOP_ERR;
	}
	rddata  = DDRTOP_mc_param_rd(MR3_DATA_F0_1_ADDR, MR3_DATA_F0_1_OFFSET, MR3_DATA_F0_1_WIDTH);
	rddata2 = DDRTOP_mc_param_rd(MR3_DATA_F1_1_ADDR, MR3_DATA_F1_1_OFFSET, MR3_DATA_F1_1_WIDTH);
	if (rddata != rddata2) {
		ERROR("MR3 register mismatch in rank 1: F0=0x%08x, F1=0x%08x\n", rddata, rddata2);
		return DDRTOP_ERR;
	}

	sys_read_modify_32bits(PHY_MODE0, 0x01000000, 0x01000000);
	mmio_write_32(MC_HWIF0, 0x04 << 16 | target_fsp << 8);
	DDRTOP_mc_param_poll(CONTROLLER_BUSY_ADDR, CONTROLLER_BUSY_OFFSET, CONTROLLER_BUSY_WIDTH, 0b0);

	sys_read_modify_32bits(MC_HWIF0, 0x00000001, 0x00000001);
	DDRTOP_proc_MC_APBPOLL(MC_HWIF1, 0x00000001, 0x00000001);

	sys_read_modify_32bits(MC_HWIF0, 0x10000000, 0x10000000);
	DDRTOP_proc_MC_APBPOLL(MC_HWIF1, 0x00000000, 0x00000010);
	DDRTOP_proc_MC_APBPOLL(MCAR_MON, 0x01000000, 0x01000000);

	return ret;
}

/**
 * @brief Change clock frequency and handshaking via HWDFS logic. Run until new clock is on CK
 */
void DDRTOP_init_hwdfs_clock_change(void)
{
	uint32_t rddata;
	uint32_t cntrl_freq_change_req_type;

	sys_read_modify_32bits(MCAR_CTL, 0x01000000, 0x01000000);
	sys_read_modify_32bits(MCAR_CTL, 0x00000000, 0x10000000);

	DDRTOP_proc_MC_APBPOLL(MC_HWDFS0, 0x00000001, 0x00000001);

	rddata = mmio_read_32(MC_HWDFS0);
	cntrl_freq_change_req_type = (rddata >> 4) & 3;

	/* Change clock frequency and re-calibrate master DLL */
	freq_change_init_lp4(cntrl_freq_change_req_type);

	udelay(1);

	sys_read_modify_32bits(MC_HWDFS1, 0x00000001, 0x00000001);
	DDRTOP_proc_MC_APBPOLL(MC_HWDFS0, 0x00000000, 0x00000001);
	DDRTOP_proc_MC_APBPOLL(MCAR_MON, 0x00000000, 0x01000000);
}

/**
 * @brief Finish HWDFS operation. Set dfi_init_complete=1. Run until lp_ext_ack=0
 * @return result of HWDFS finish
 */
int DDRTOP_init_hwdfs_finish(void)
{
	int ret = DDRTOP_OK;

	uint32_t rddata;
	uint32_t lp_ext_resp;
	uint32_t ctl_lp_ext_fc_reg_copy;

	sys_read_modify_32bits(MCAR_CTL, 0x10000000, 0x10000000);
	sys_read_modify_32bits(MCAR_CTL, 0x00000000, 0x01000000);

	DDRTOP_proc_MC_APBPOLL(MC_HWIF1, 0x00000100, 0x00000100);

	rddata = mmio_read_32(MC_HWIF1);
	lp_ext_resp = (rddata >> 12) & 3;
	if (lp_ext_resp != 0b00) {
		ERROR("Status error in HWDFS: lp_ext_resp=0x%08x(!= 2'b00)\n", lp_ext_resp);
		return DDRTOP_ERR;
	}

	sys_read_modify_32bits(MC_HWIF0, 0x00000000, 0x01000001);
	DDRTOP_proc_MC_APBPOLL(MC_HWIF1, 0x00000000, 0x00000001);

	rddata = mmio_read_32(MC_HWIF0);
	ctl_lp_ext_fc_reg_copy = (rddata >> 8) & 3;

	rddata = DDRTOP_mc_param_rd(FSP_OP_CURRENT_ADDR, FSP_OP_CURRENT_OFFSET, FSP_OP_CURRENT_WIDTH);
	if ((rddata & 0x00000001) != (ctl_lp_ext_fc_reg_copy & 0x00000001)) {
		ERROR("Status error in FSP_OP_CURRENT after HWDFS: FSP_OP_CURRENT=0x%08x\n", rddata);
		return DDRTOP_ERR;
	}

	rddata = DDRTOP_mc_param_rd(CURRENT_REG_COPY_ADDR, CURRENT_REG_COPY_OFFSET, CURRENT_REG_COPY_WIDTH);
	if ((rddata & 0x00000001) != (ctl_lp_ext_fc_reg_copy & 0x00000001)) {
		ERROR("Status error in CURRENT_REG_COPY after HWDFS: CURRENT_REG_COPY=0x%08x\n", rddata);
		return DDRTOP_ERR;
	}

	sys_read_modify_32bits(PHY_MODE0, 0x00000000, 0x01000000);

	return ret;
}

/**
 * @brief PHY swizzle setting
 */
void DDRTOP_init_phy_swizzle(void)
{
	uint32_t pval;

	/* pad_mem_address[3:0] */
	pval = ext_unswizzle_ca(0) <<  0 |
		   ext_unswizzle_ca(1) <<  8 |
		   ext_unswizzle_ca(2) << 16 |
		   ext_unswizzle_ca(3) << 24;
	mmio_write_32(PHY_PHY_LANE_SEL, 0);
	sys_read_modify_32bits(PHY_CA_SWIZZLE_POSITION, pval, 0xFFFFFFFF);
	/* pad_mem_address[5:4] */
	pval = ext_unswizzle_ca(4) <<  0 |
		   ext_unswizzle_ca(5) <<  8;
	mmio_write_32(PHY_PHY_LANE_SEL, 1);
	sys_read_modify_32bits(PHY_CA_SWIZZLE_POSITION, pval, 0x0000FFFF);
	/* pad_mem_cs_n[1:0] */
	pval = (ext_unswizzle_cs(0) + 26) << 16 |
		   (ext_unswizzle_cs(1) + 26) << 24;
	mmio_write_32(PHY_PHY_LANE_SEL, 6);
	sys_read_modify_32bits(PHY_CA_SWIZZLE_POSITION, pval, 0xFFFF0000);

	mmio_write_32(PHY_PHY_LANE_SEL, 0);
}

/**
 * @brief MC swizzle setting
 */
void DDRTOP_init_mc_swizzle(void)
{
	uint32_t pval;

	/* SWIZZLE_SEL_0 */
	pval = (ext_swizzle_cs(0) + 1) <<  0 |
		   (ext_swizzle_cs(1) + 1) <<  8;
	sys_read_modify_32bits(SWIZZLE_SEL_0, pval, 0x3F << 8 | 0x3F);
	/* SWIZZLE_SEL_1 */
	pval = (ext_swizzle_ca(0) + 18) <<  0 |
		   (ext_swizzle_ca(1) + 18) <<  8 |
		   (ext_swizzle_ca(2) + 18) << 16 |
		   (ext_swizzle_ca(3) + 18) << 24;
	sys_read_modify_32bits(SWIZZLE_SEL_1, pval, 0x3F << 24 | 0x3F << 16 | 0x3F << 8 | 0x3F);
	/* SWIZZLE_SEL_2 */
	pval = (ext_swizzle_ca(4) + 18) <<  0 |
		   (ext_swizzle_ca(5) + 18) <<  8;
	sys_read_modify_32bits(SWIZZLE_SEL_2, pval, 0x3F << 8 | 0x3F);
}

/**
 * @brief Intialize MC register setting
 * @param proc_status DDR process status
 */
void DDRTOP_init_mc_WRITE_REGMAP(uint32_t proc_status)
{
	size_t i;

	for (i = 0; i < param_setup_mc_size; i++) {
		DDRTOP_mc_apb_wr(param_setup_mc[i][0], param_setup_mc[i][1]);
	}

	if (proc_status != 0) {
		DDRTOP_mc_param_wr(PHY_INDEP_INIT_MODE_ADDR, PHY_INDEP_INIT_MODE_OFFSET, PHY_INDEP_INIT_MODE_WIDTH, 1);
	}
}

/**
 * @brief Initialize DRAM MR setting
 * @param num_ranks  Number of Ranks
 */
void DDRTOP_init_dram_sequence(unsigned int num_ranks)
{
	uint32_t rddata;

	/* This setting should be done after finishing all MC setting */
	rddata = DDRTOP_mc_param_rd(NO_MEMORY_DM_ADDR, NO_MEMORY_DM_OFFSET, NO_MEMORY_DM_WIDTH);
	if (rddata == 0) {
		rddata = DDRTOP_mc_param_rd(WR_DBI_EN_ADDR, WR_DBI_EN_OFFSET, WR_DBI_EN_WIDTH);
		if (rddata == 1) {
			/* Enable DM control and mask pattern on PHY_DMPAT and make the PHY DQ bits corresponding to DRAM DQ7-3 in each lane high */
			INFO("Enable DM control and mask patterns for PCB DQ swizzling\n");
			mmio_write_32(PHY_DMPAT,
					(1 << 24)                            |
					(1 << (ext_unswizzle_bit(7, 1) + 8)) |
					(1 << (ext_unswizzle_bit(6, 1) + 8)) |
					(1 << (ext_unswizzle_bit(5, 1) + 8)) |
					(1 << (ext_unswizzle_bit(4, 1) + 8)) |
					(1 << (ext_unswizzle_bit(3, 1) + 8)) |
					(1 << (ext_unswizzle_bit(7, 0) + 0)) |
					(1 << (ext_unswizzle_bit(6, 0) + 0)) |
					(1 << (ext_unswizzle_bit(5, 0) + 0)) |
					(1 << (ext_unswizzle_bit(4, 0) + 0)) |
					(1 << (ext_unswizzle_bit(3, 0) + 0)));
		}
	}

	/* Mask dfi_ctrlupd_req */
	sys_read_modify_32bits(MCAR_CTL, 1 << 16, 1 << 16);

	/* Enable IO output */
	sys_read_modify_32bits(PHY_CTRL0, 0 << 0, 1 << 0);

	/* Wait for Analog DLL to lock */
	DDRTOP_proc_PHY_APBPOLL(PHY_UNQ_ANALOG_DLL_2, 3, 0x00000003);

	/* Wait until PHY is ready to accept commands on the DFI interface */
	DDRTOP_proc_PHY_APBPOLL(PHY_PHY_DLL_RISE_FALL, 1 << 12, 0x00001000);

	/* Manually update the UCAL I/O */
	sys_read_modify_32bits(PHY_UNIQUIFY_IO_1, 1 << 4, 0x00000010);
	sys_read_modify_32bits(PHY_UNIQUIFY_IO_1, 0 << 4, 0x00000010);

	/* Initiate command processing in the controller */
	DDRTOP_mc_param_wr(START_ADDR, START_OFFSET, START_WIDTH, 1);

	/* Wait for controller to complete memory initialization */
	DDRTOP_mc_param_poll(INT_STATUS_INIT_ADDR, INT_STATUS_INIT_OFFSET, INT_STATUS_INIT_WIDTH, 0x02);

	/* MEMC MR write */
	DDRTOP_MC_MRW(0x0F, 0x00, 0b11);
	DDRTOP_MC_MRW(0x14, 0x00, 0b11);
	DDRTOP_MC_MRW(0x20, 0xAA, 0b11);
	DDRTOP_MC_MRW(0x28, 0xAA, 0b11);
}

/**
 * @brief Read Bit Leveling
 * @param bit_lvl_from_saved Use the saved trim trained by bit leveling before
 * @return result of read bit leveling
 */
int DDRTOP_init_read_bit_leveling(
	uint32_t bit_lvl_from_saved)
{
	int ret = DDRTOP_OK;

	uint32_t rddata;
	uint8_t bit_lvl_failure_status;

	/* Bit-Leveling read MPR */
	mmio_write_32(PHY_SCL_START, (0b11000010 << 22) | (bit_lvl_from_saved << 21));
	DDRTOP_proc_PHY_APBPOLL(PHY_SCL_START, 0x00000000, 1 << 28);

	/* FAILURE CHECKING */
	rddata = mmio_read_32(PHY_DYNAMIC_BIT_LVL);
	bit_lvl_failure_status = (rddata >> 16) & 3;

	if (bit_lvl_failure_status == 0) {
		INFO("RBL: PASSED on all lanes\n");
	} else {
		ERROR("RBL: FAILED: lanes failed(1=fail) = 0x%08x\n", bit_lvl_failure_status);
		return DDRTOP_ERR;
	}

	return ret;
}

/**
 * @brief Write Bit Leveling
 * @param bit_lvl_from_saved Use the saved trim trained by bit leveling before
 * @return result of Write Bit Leveling
 */
int DDRTOP_init_write_bit_leveling(uint32_t bit_lvl_from_saved)
{
	int ret = DDRTOP_OK;

	uint32_t rddata;
	uint8_t bit_lvl_wr_failure_status;

	/* Bit-lvl from MPR */
	mmio_write_32(PHY_SCL_START, (0b11000010 << 22) | (bit_lvl_from_saved << 21) | (1 << 20));
	DDRTOP_proc_PHY_APBPOLL(PHY_SCL_START, 0x00000000, 1 << 28);

	/* FAILURE CHECKING */
	rddata = mmio_read_32(PHY_DYNAMIC_WRITE_BIT_LVL);
	bit_lvl_wr_failure_status = (rddata >> 20) & 3;

	if (bit_lvl_wr_failure_status == 0) {
		INFO("WBL: PASSED on all lanes\n");
	} else {
		ERROR("WBL: FAILED: lanes failed(1=fail) = 0x%08x\n", bit_lvl_wr_failure_status);
		udelay(10);
		return DDRTOP_ERR;
	}

	return ret;
}

/**
 * @brief Run SCL
 * @param rank                Specify RANK-0 or RANK-1
 * @param bit_lvl_from_saved  Use the saved trim trained by bit leveling before.
 *                            Used in SCL routines and should not be set by the user.
 *                            If set, dynamic bit-leveling will use previously saved bit-leveled values.
 *                            Otherwise, each time dynamic bit-leveling is run, it will start bit-leveling
 *                            from scratch (i.e. will set all initial delay values for all bits to 0)
 * @param scl_second_try      Set to 1 to inhibit retry-on-fail for 2nd call of this task/sequence
 * @param run_WR_BIT_LEVELING Set to 1 to perform Write Bit Leveling before SCL retry
 * @param scl_start_addr_X    Bank, Column, and Row address bits
 * @param val_NATIVE_BURST8   Value to add in col_addr: Always set to 16 for LPDDR4/DDR5; set to 8 for half-rate controllers
 * @return result of SCL
 */

int DDRTOP_init_scl(
	uint32_t rank,
	uint32_t bit_lvl_from_saved,
	uint32_t scl_second_try,
	uint32_t run_WR_BIT_LEVELING,
	uint32_t scl_start_addr_X,
	uint32_t val_NATIVE_BURST8)
{
	int ret = DDRTOP_OK;

	uint32_t dll_mas_dly, scl_rslt, trim2_dly, dqs_total_dly;

	uint32_t rddata;
	uint32_t main_clk_dly;

	/* Default pattern to read that is different from SCL data */
	for (int i = 0; i < 4; i++) {
		mmio_write_32(PHY_PHY_LANE_SEL, i);
		mmio_write_32(PHY_SCL_DATA_0, 0xaa00aa00);
	}

	/* Setup scl_start_row_addr, scl_start_col_addr, scl_start_bank_addr */
	mmio_write_32(PHY_PHY_SCL_START_ADDR, scl_start_addr_X + (val_NATIVE_BURST8 << 16));
	sys_read_modify_32bits(PHY_DDR4_CONFIG_1, 1 << 2 | ((DDRTOP_SCL_ROW_ADDR >> 16) & 1) << 1, 0b11 << 1);

	/* SCL write pattern data */
	mmio_write_32(PHY_SCL_START, (0b01000100 << 22) | (bit_lvl_from_saved << 21));
	DDRTOP_proc_PHY_APBPOLL(PHY_SCL_START, 0x00000000, 1 << 28);

	/* Replace 0 with the required value X described above */
	mmio_write_32(PHY_PHY_SCL_START_ADDR, scl_start_addr_X);

	/* Restore the default pattern SCL data */
	for (int i = 0; i < 4; i++) {
		mmio_write_32(PHY_PHY_LANE_SEL, i);
		switch (i) {
		case 0:
			mmio_write_32(PHY_SCL_DATA_0, 0x789a3ce0);
			break;
		case 1:
			mmio_write_32(PHY_SCL_DATA_0, 0x710e4a56);
			break;
		case 2:
			mmio_write_32(PHY_SCL_DATA_0, 0x0e71b1a9);
			break;
		case 3:
			mmio_write_32(PHY_SCL_DATA_0, 0x8764c232);
			break;
		}
	}

	/* Write the SCL data to the SCL address location in DRAM */
	mmio_write_32(PHY_SCL_START, 1 << 28 | 1 << 24 | bit_lvl_from_saved << 21);
	DDRTOP_proc_PHY_APBPOLL(PHY_SCL_START, 0x00000000, 1 << 28);

	/* Run SCL phase and latency calibration as an atomic operation */
	mmio_write_32(PHY_SCL_START, 1 << 29 | 1 << 28 | 1 << 26 | bit_lvl_from_saved << 21);
	DDRTOP_proc_PHY_APBPOLL(PHY_SCL_START, 0x00000000, 1 << 28);

	/* FAILURE CHECKING */
	/* Check whether SCL passed or failed on each byte lane */
	rddata = mmio_read_32(PHY_SCL_START);
	scl_rslt = rddata & 0x00000003;

	if (scl_rslt == 0b11) { /* PASS */
		INFO("SCL: PASSED for all lanes: rank = %d\n", rank);
		if (rank == 0) {
			mmio_write_32(PHY_PHY_LANE_SEL, 0);
			rddata = mmio_read_32(PHY_SCL_LATENCY);
			main_clk_dly = (rddata >> 16) & 0x1F;

			mmio_write_32(PHY_PHY_LANE_SEL, 1 << 16);
			sys_read_modify_32bits(PHY_SCL_LATENCY, main_clk_dly << 16, 0x1F << 16);
		}
	} else { /* FAIL */
		rddata = mmio_read_32(PHY_PHY_DLL_ADRCTRL);
		dll_mas_dly = rddata >> 24;

		if ((dll_mas_dly > 200) && !scl_second_try) {
			INFO("SCL: Retrying SCL for rank %d since it may have failed at low frequency due to write leveling delay jump issue\n", rank);
			/* Set-up RETRY for LANE-0 and LANE-1 */
			for (int i = 0; i < 2; i = i + 1) {
				if (((scl_rslt >> i) & 1) == 0) {
					mmio_write_32(PHY_PHY_LANE_SEL, rank << 16 | (i * (8 + 1)));
					rddata = mmio_read_32(PHY_PHY_DLL_TRIM_2);
					trim2_dly = rddata;

					mmio_write_32(PHY_PHY_LANE_SEL, rank << 16 | i);
					rddata = mmio_read_32(PHY_PHY_DLL_TRIM_OP_DQS);
					dqs_total_dly = trim2_dly + (rddata & 0x000000FF) * (rddata & 0x00000100 ? 4 : -4);

					rddata = mmio_read_32(PHY_DQS_TRIM);
					dqs_total_dly = dqs_total_dly + (rddata & 0x000000FF) * 4;

					if ((dqs_total_dly & 0x0000003F) == 0) {
						/* Confirmed that fine delay boundary was crossed and we can proceed with retry */
						mmio_write_32(PHY_PHY_LANE_SEL, rank << 16 | (i * (8 + 1)));
						mmio_write_32(PHY_PHY_DLL_TRIM_2, (trim2_dly - 1) & 0x1FF);
					} else {
						ERROR("SCL: FAILED: An unrecoverable error: dqs_total_dly = 0x%08x\n", dqs_total_dly);
						udelay(10);
						return DDRTOP_ERR;
					}
				}
			}

			if (run_WR_BIT_LEVELING) {
				DDRTOP_init_write_bit_leveling(bit_lvl_from_saved);
			}

			DDRTOP_init_scl(rank,
							bit_lvl_from_saved,
							1,
							run_WR_BIT_LEVELING,
							scl_start_addr_X,
							val_NATIVE_BURST8);
		} else {
			ERROR("SCL: FAILED: rank = %d, lanes failed(1=fail) = %x, SCL_LANES = %x, dll_mas_dly = %d\n", rank, ~scl_rslt, DDRTOP_SCL_LANES, dll_mas_dly);
			udelay(10);
			return DDRTOP_ERR;
		}
	}

	return ret;
}

#define PHY_PULL_UP_ODT      0 	/* LPDDR4 uses pull down ODT on PHY side */
#define DRAM_PULL_UP_ODT     0 	/* LPDDR4 uses pull down ODT on DRAM side */

#define MAX_VREF_TRIM_PHY   64	/* Max VREF trim value supported by PHY selected based on pull down or pull up ODT
								 * Because UVREF supports 2 trim ranges depending on pull down or pull up ODT
								 */
#define SWEEP_LIMIT         80	/* Total number of unique VREF trim settings
								 * supported by DRAM including both range 0 and range 1
								 */
#define MAX_BEST_VREF_SAVED 30	/* Maximum number of BEST VREF settings that we need to save */

#define VREF_STEP_SIZE       1
#define DLY_STEP_SIZE        1

#define PHY_MAX_VREF_TRIM   (PHY_PULL_UP_ODT ? 126 : 64)
#define DRAM_MAX_VREF_TRIM  (DRAM_PULL_UP_ODT ? 126 : 64)
#define DRAM_SWEEP_LIMIT    80 /* Total number of unique VREF trim settings for LPDDR4 */

static const uint32_t sweep_range_ratio = 15;

/**
 * @brief Wrapper function of DQ VREF Training for SoC and DRAM
 * @param rank      Target rank
 * @param num_ranks Number of ranks
 */
void DDRTOP_init_vref_dq_training_wrapper(uint32_t rank, uint32_t num_ranks)
{
	uint32_t DQ_vref_mid_level;
	uint8_t DQ_vref_mid_level_code;
	uint8_t DQ_sweep_range;

	uint32_t DRAM_vref_mid_level;
	uint8_t DRAM_vref_mid_level_code;
	uint8_t DRAM_sweep_range;

	uint32_t tmp_data;

	if (rank == DDRTOP_PRIMARY_RANK) {
		sys_read_modify_32bits(PHY_RANK_SEL, rank, (((1 << 4) - 1)));
		sys_read_modify_32bits(PHY_SCL_CONFIG_2, (1 << rank), (((1 << 2) - 1)));

		/* Calculations for PHY side */
		DQ_vref_mid_level = param_phyinit_11;
		tmp_data = ((DQ_vref_mid_level - 1000) << 8);
		tmp_data = (uint32_t)((tmp_data / 66) + 512 - 128);
		DQ_vref_mid_level_code = (uint8_t)((tmp_data >> 8) + ((tmp_data & 0xff) > 0 ? 1 : 0));

		tmp_data = ((DQ_vref_mid_level * sweep_range_ratio) << 8);
		tmp_data = (uint32_t)((tmp_data / 6600) - 128);
		DQ_sweep_range = (uint8_t)((tmp_data >> 8) + ((tmp_data & 0xff) > 0 ? 1 : 0));

		/* Calculations for DRAM side */
		DRAM_vref_mid_level = param_phyinit_12;
		tmp_data = ((DRAM_vref_mid_level - 1000) << 8);
		tmp_data = (uint32_t)((tmp_data / 40) - 128);
		DRAM_vref_mid_level_code = (uint8_t)((tmp_data >> 8) + ((tmp_data & 0xff) > 0 ? 1 : 0));
		tmp_data = ((DRAM_vref_mid_level * sweep_range_ratio) << 8);
		tmp_data = (uint32_t)((tmp_data / 4000) - 128);
		DRAM_sweep_range = (uint8_t)((tmp_data >> 8) + ((tmp_data & 0xff) > 0 ? 1 : 0));

		/* VREF DQ Training */
		DDRTOP_init_vref_dq_training(
			DQ_vref_mid_level,
			DQ_vref_mid_level_code,
			DQ_sweep_range,
			VREF_STEP_SIZE,
			PHY_MAX_VREF_TRIM,
			DRAM_vref_mid_level,
			DRAM_vref_mid_level_code,
			DRAM_sweep_range,
			VREF_STEP_SIZE,
			DRAM_MAX_VREF_TRIM,
			DRAM_SWEEP_LIMIT,
			num_ranks,
			1, /* RUN_PHY_VREF */
			1, /* RUN_DRAM_VREF */
			4  /* bit_lvl_sample_qty */
		);
	}
}

/**
 * @brief DQ VREF Training for SoC and DRAM
 * @details Pass drive strength and ODT settings on SoC and DRAM side as input to the task
 *          so it can calculate the expected read and write VREF center points
 * @param DQ_vref_mid_level
 * @param DQ_vref_mid_level_code
 * @param DQ_sweep_range
 * @param DQ_vref_step_size      Step size of VREF code in SoC
 * @param DQ_max_vref_trim_phy   Max VREF trim value supported by PHY selected based on pull down or pull up ODT
 *                               Because UVREF supports 2 trim ranges depending on pull down or pull up ODT
 * @param DRAM_vref_mid_level
 * @param DRAM_vref_mid_level_code
 * @param DRAM_sweep_range
 * @param DRAM_vref_step_size    Step size of VREF code in DRAM
 * @param DRAM_max_vref_trim_phy Max VREF trim value supported by PHY selected based on pull down or pull up ODT
 *                               Because UVREF supports 2 trim ranges depending on pull down or pull up ODT
 * @param DRAM_sweep_limit       Total number of unique VREF trim settings
 * @param DRAM_mem_chip_selects  Set by MEM_CHIP_SELECTS
 * @param RUN_PHY_VREF           PHY side VREF training is always expected to be enabled
 * @param RUN_DRAM_VREF          Enable DRAM side (i.e. Write) VREF training only for DRAM types that support it
 * @param bit_lvl_sample_qty     Number of samples taken by bit-leveling (1 per half rate clock cycle)
 *                               to detect the edges of the bit-leveling window
 *
 */
void DDRTOP_init_vref_dq_training(
	uint32_t DQ_vref_mid_level,
	uint8_t DQ_vref_mid_level_code,
	uint8_t DQ_sweep_range,
	uint8_t DQ_vref_step_size,
	uint8_t DQ_max_vref_trim_phy,
	uint32_t DRAM_vref_mid_level,
	uint8_t DRAM_vref_mid_level_code,
	uint8_t DRAM_sweep_range,
	uint8_t DRAM_vref_step_size,
	uint8_t DRAM_max_vref_trim_phy,
	uint8_t DRAM_sweep_limit,
	uint8_t DRAM_mem_chip_selects,
	uint32_t RUN_PHY_VREF,
	uint32_t RUN_DRAM_VREF,
	uint32_t bit_lvl_sample_qty)
{
	uint32_t DQ_best_vref_setting_lane[2];
	uint32_t DRAM_best_vref_setting_lane[2];

	/* For VREF training, we only sample DQ0 on rising edge and DQ1 on falling edge of DQS */
	for (int i = 0; i < 2; i = i + 1) {
		mmio_write_32(PHY_PHY_LANE_SEL, i);
		mmio_write_32(PHY_BIT_LVL_CONFIG, 1 << 4 | bit_lvl_sample_qty);
	}

	if (RUN_PHY_VREF) {
		DDRTOP_init_phy_vref_dq_training(
			DQ_vref_mid_level,
			DQ_vref_mid_level_code,
			DQ_sweep_range,
			DQ_vref_step_size,
			DQ_max_vref_trim_phy,
			DQ_best_vref_setting_lane);
	}

	if (RUN_DRAM_VREF) {
		DDRTOP_init_dram_vref_dq_training(
			DRAM_vref_mid_level,
			DRAM_vref_mid_level_code,
			DRAM_sweep_range,
			DRAM_vref_step_size,
			DRAM_max_vref_trim_phy,
			DRAM_sweep_limit,
			DRAM_mem_chip_selects,
			DRAM_best_vref_setting_lane);
	}

	/* For normal bit-leveling we sample every DQ bit on both rising and falling edges of DQS,
	 * so restore the original value
	 */
	for (int i = 0; i < 2; i = i + 1) {
		mmio_write_32(PHY_PHY_LANE_SEL, i);
		mmio_write_32(PHY_BIT_LVL_CONFIG, 3 << 4 | bit_lvl_sample_qty);
	}
}

/**
 * @brief PHY DQ VREF training (Read side)
 * @param vref_mid_level
 * @param vref_mid_level_code
 * @param sweep_range
 * @param vref_step_size step size of VREF code
 * @param max_vref_trim_phy Max VREF trim value supported by PHY selected based on pull down or pull up ODT
 *                          Because UVREF supports 2 trim ranges depending on pull down or pull up ODT
 * @param best_vref_setting_lane Best VREF setting
 */
void DDRTOP_init_phy_vref_dq_training(
	uint32_t vref_mid_level,
	uint32_t vref_mid_level_code,
	uint32_t sweep_range,
	uint32_t vref_step_size,
	uint32_t max_vref_trim_phy,
	uint32_t best_vref_setting_lane[2])
{

	uint8_t window_diff[2];

	uint8_t best_window_diff_so_far[2];
	uint8_t num_best_vref_matches[2];
	uint8_t all_best_vref_matches[2][MAX_BEST_VREF_SAVED];

	uint8_t highest_best_vref_val;
	uint8_t lowest_best_vref_val;

	uint32_t current_vref;

	/* Set initial value prior to training */
	for (int lane = 0; lane < 2; lane = lane + 1) {
		best_window_diff_so_far[lane] = 255;
		num_best_vref_matches[lane] = 0;
	}

	/* j = 0 loop sweeps VREF from mid-level value to lower values and
	 * j = 1 loop sweeps from mid-level + 1 to higher values
	 * i loop runs for each individual VREF setting
	 */
	for (int j = 0; j < 2; j = j + 1) {
		for (int i = 0; i < sweep_range + 1; i = i + vref_step_size) {
			if ((j == 1) && (i == 0)) {
				continue;
			} else if ((j == 0) && (i + 2 > vref_mid_level_code)) {
				break;
			} else if ((j == 1) && (vref_mid_level_code + i > MAX_VREF_TRIM_PHY)) {
				break;
			}

			if (j == 1)
				current_vref = vref_mid_level_code + i;
			else
				current_vref = vref_mid_level_code - i;

			/* Change UVREF trim code for each lane */
			for (int lane = 0; lane < 2; lane = lane + 1) {
				mmio_write_32(PHY_PHY_LANE_SEL, (7 * lane) & 0xFF);
				mmio_write_32(PHY_VREF_TRAINING, ((current_vref & 0x7F) << 4) | 1);

				udelay(1);
			}

			DDRTOP_init_find_vref_window(window_diff, current_vref);

			/* Check if window_diff we got is the best so far
			 * and if so save the information for later evaluation
			 */
			for (int lane = 0; lane < 2; lane = lane + 1) {
				if (window_diff[lane] < best_window_diff_so_far[lane]) {
					best_window_diff_so_far[lane] = window_diff[lane];
					all_best_vref_matches[lane][0] = current_vref;
					num_best_vref_matches[lane] = 1;
				} else if ((window_diff[lane] == best_window_diff_so_far[lane]) &&
						 (num_best_vref_matches[lane] < MAX_BEST_VREF_SAVED)) {
					all_best_vref_matches[lane][num_best_vref_matches[lane]] = current_vref;
					num_best_vref_matches[lane] = num_best_vref_matches[lane] + 1;
				}
			}
		}
	}

	/* For LANE-0 and 1:  Select / calculate the best VREF setting */
	for (int lane = 0; lane < 2; lane = lane + 1) {
		highest_best_vref_val = 0;
		lowest_best_vref_val  = 0x7F;

		/* Iterate over ALL VREF settings that gave smallest window_diff in each lane
		 * to find the highest and lowest such VREF value for each lane
		 */
		for (int i = 0; i < num_best_vref_matches[lane]; i = i + 1) {
			highest_best_vref_val = all_best_vref_matches[lane][i] > highest_best_vref_val ? all_best_vref_matches[lane][i] : highest_best_vref_val;
			lowest_best_vref_val  = all_best_vref_matches[lane][i] < lowest_best_vref_val  ? all_best_vref_matches[lane][i] : lowest_best_vref_val;
		}

		/* Convert from unsigned char to unsigned int to avoid overflow on following line */
		current_vref = highest_best_vref_val;

		/* Find midpoint of highest_best_vref_val and lowest_best_vref_val */
		current_vref = (current_vref + lowest_best_vref_val) >> 1;

		best_vref_setting_lane[lane] = current_vref;

		/* Program the final result of VREF training and turn off VREF training */
		mmio_write_32(PHY_PHY_LANE_SEL, (7 * lane) & 0xFF);
		mmio_write_32(PHY_VREF_TRAINING, (current_vref & 0x7F) << 4);

		udelay(1);
		INFO("Read VREF: lane = %d, Programmed VREF = %d\n", lane, current_vref);
	}
	INFO("Read VREF: Finished VREF DQ Training\n");
}

/**
 * @brief Find VREF Window
 * @param window_diff
 * @param current_vref
 */
void DDRTOP_init_find_vref_window(uint8_t window_diff[2], uint32_t current_vref)
{
	uint8_t window_0;
	uint8_t window_1;

	uint32_t rddata;

	/* Run VREF training using DRAM MPR read pattern register,
	 * so we don't need to initialize any data pattern into the DRAM
	 */
	mmio_write_32(PHY_SCL_START, (0b11000010 << 22) | 0 << 21);
	DDRTOP_proc_PHY_APBPOLL(PHY_SCL_START, 0x00000000, 1 << 28);

	/* Force update of VREF training result values from all the data lanes to the address PHY */
	mmio_write_32(PHY_A2D_D2A_CONFIG, 1 << 2);
	DDRTOP_proc_PHY_APBPOLL(PHY_A2D_D2A_CONFIG, 0x00000000, 1 << 2);

	/* Check VREF training windows for each byte lane */
	for (int lane = 0; lane < 2; lane = lane + 1) {
		rddata = mmio_read_32(PHY_DYNAMIC_BIT_LVL);
		if (rddata >> (16 + lane) == 0) {
			/* Read out VREF training windows for this byte lane */
			mmio_write_32(PHY_PHY_LANE_SEL, ((lane * 2 * 7) & 0xFF));
			rddata = mmio_read_32(PHY_VREF_TRAINING);
			window_0 = (rddata >> 12) & 0xFF;
			window_1 = (rddata >> 20) & 0xFF;

			window_diff[lane] = (window_0 > window_1) ? window_0 - window_1 : window_1 - window_0;
			INFO("Read VREF: PASSED, VREF = %d, lane = %d, win0 = %d, win1 = %d, win_diff = %d\n", current_vref, lane, window_0, window_1, window_diff[lane]);
		} else { /* FALSE */
			window_diff[lane] = 255;
			ERROR("Read VREF: FAILED, VREF = %d, lane = %d\n", current_vref, lane);
		}
	}
}

/**
 * @brief DRAM DQ VREF training (Write side)
 * @param vref_mid_level      DQ VREF value
 * @param vref_mid_level_code DQ VREF code
 * @param sweep_range         DQ VREF range
 * @param vref_step_size      Step size of DQ VREF code
 * @param max_vref_trim_phy   Max VREF trim value supported by PHY selected based on pull down or pull up ODT
 *                            Because UVREF supports 2 trim ranges depending on pull down or pull up ODT
 * @param sweep_limit         Total number of unique VREF trim settings
 * @param mem_chip_selects    Rank
 * @param best_vref_setting_lane Best VREF setting
 */
void DDRTOP_init_dram_vref_dq_training(
	uint32_t vref_mid_level,
	uint8_t vref_mid_level_code,
	uint8_t sweep_range,
	uint8_t vref_step_size,
	uint8_t max_vref_trim_phy,
	uint8_t sweep_limit,
	uint8_t mem_chip_selects,
	uint32_t best_vref_setting_lane[2])
{
	uint8_t window_0;
	uint8_t window_1;
	uint8_t window_diff;

	uint8_t best_window_diff_so_far[2];
	uint8_t num_best_vref_matches[2];
	uint8_t all_best_vref_matches[2][MAX_BEST_VREF_SAVED];

	uint8_t highest_best_vref_val;
	uint8_t lowest_best_vref_val;

	uint8_t orig_cs_config;

	uint32_t current_vref;

	uint32_t rddata;
	uint32_t wr_bit_lvl_failure;

	rddata = mmio_read_32(PHY_SCL_CONFIG_2);
	orig_cs_config = rddata & 0x3;

	/* Setup scl_test_cs such that when DRAM mode register is programmed,
	 * it is programmed for all ranks together
	 */
	sys_read_modify_32bits(PHY_SCL_CONFIG_2, (1 << mem_chip_selects) - 1, 0x3);

	/* Set the initial VREF value and turn on DRAM mode register bit(s) required for VREF training */
	DDRTOP_init_setup_vref_training_registers(
		vref_mid_level_code,
		0,
		1
	);

	/* Restore scl_test_cs to the value of the rank we are using for training */
	sys_read_modify_32bits(PHY_SCL_CONFIG_2, orig_cs_config & 0x3, 0x3);

	/* Set the PHY to operate in VREF training mode:
	 * Turn on VREF training mode in the PHY
	 */
	sys_read_modify_32bits(PHY_VREF_TRAINING, 1, 1);

	/* Set initial value prior to training */
	for (int lane = 0; lane < 2; lane = lane + 1) {
		best_window_diff_so_far[lane] = 255;
		num_best_vref_matches[lane] = 0;
	}

	/* j = 0 loop sweeps VREF from mid-level value to lower values and
	 * j = 1 loop sweeps from mid-level + 1 to higher values
	 * i loop runs for each individual VREF setting
	 */
	for (int j = 0; j < 2; j = j + 1) {
		for (int i = 0; i < sweep_range + 1; i = i + VREF_STEP_SIZE) {
			if ((j == 1) && (i == 0)) {
				continue;
			} else if ((j == 0) && (i > vref_mid_level_code)) {
				break;
			} else if ((j == 1) && (vref_mid_level_code + i > SWEEP_LIMIT)) {
				break;
			}

			if (j == 1)
				current_vref = vref_mid_level_code + i;
			else
				current_vref = vref_mid_level_code - i;

			DDRTOP_init_setup_vref_training_registers(
				current_vref,
				0,
				0
			);

			/* Run VREF training for write side. Use MPC FIFO for training */
			mmio_write_32(PHY_SCL_START, 0b11000010 << 22 | 0 << 21 | 1 << 20);
			DDRTOP_proc_PHY_APBPOLL(PHY_SCL_START, 0x00000000, 1 << 28);

			/* Force update of VREF training result values from all the data lanes to the address PHY */
			mmio_write_32(PHY_A2D_D2A_CONFIG, 1 << 2);
			DDRTOP_proc_PHY_APBPOLL(PHY_A2D_D2A_CONFIG, 0x00000000, 1 << 2);

			/* Check if all lanes passed training */
			rddata = mmio_read_32(PHY_DYNAMIC_WRITE_BIT_LVL);
			wr_bit_lvl_failure = (rddata >> 20) & 0x3;

			/* Check VREF training windows for each byte lane */
			for (int lane = 0; lane < 2 && ((DDRTOP_SCL_LANES >> lane) & 1); lane = lane + 1) {
				if (!wr_bit_lvl_failure) {
					/* Read out VREF training windows for this byte lane */
					mmio_write_32(PHY_PHY_LANE_SEL, (lane * 2 * 7) & 0xFF);
					rddata = mmio_read_32(PHY_VREF_TRAINING);
					window_0 = (rddata >> 12) & 0xFF;
					window_1 = (rddata >> 20) & 0xFF;

					window_diff = (window_0 > window_1) ? window_0 - window_1 : window_1 - window_0;
					INFO("Write VREF: PASSED, VREF = %d, lane = %d: win_0 = %d, win_1 = %d, win_diff = %d\n", current_vref, lane, window_0, window_1, window_diff);

					/* Check if window_diff we got is the best so far and if so save the information for later evaluation */
					if (window_diff < best_window_diff_so_far[lane]) {
						best_window_diff_so_far[lane] = window_diff;
						all_best_vref_matches[lane][0] = current_vref;
						num_best_vref_matches[lane] = 1;
					} else if ((window_diff == best_window_diff_so_far[lane]) &&
							 (num_best_vref_matches[lane] < MAX_BEST_VREF_SAVED)) {
						all_best_vref_matches[lane][num_best_vref_matches[lane]] = current_vref;
						num_best_vref_matches[lane] = num_best_vref_matches[lane] + 1;
					}
				} else {
					ERROR("Write VREF: FAILED, VREF = %d, lane = %d\n", current_vref, lane);
				}
			}
		}
	}

	highest_best_vref_val = 0x0;
	lowest_best_vref_val  = 0x7F;

	/* Iterate over ALL VREF settings that gave smallest window_diff in each lane
	 * to find the highest and lowest such VREF value across all lanes
	 */
	for (int lane = 0; lane < 2; lane = lane + 1) {
		for (int i = 0; i < num_best_vref_matches[lane]; i = i + 1) {
			highest_best_vref_val = all_best_vref_matches[lane][i] > highest_best_vref_val ? all_best_vref_matches[lane][i] : highest_best_vref_val;
			lowest_best_vref_val  = all_best_vref_matches[lane][i] < lowest_best_vref_val  ? all_best_vref_matches[lane][i] : lowest_best_vref_val;
		}
	}

	/* Convert from unsigned char to unsigned int to avoid overflow on following line */
	current_vref = highest_best_vref_val;
	current_vref = (current_vref + lowest_best_vref_val) >> 1;

	/* Program trained VREF to all ranks */
	sys_read_modify_32bits(PHY_SCL_CONFIG_2, ((1 << mem_chip_selects) - 1) & 0x3, 0x3);
	DDRTOP_init_setup_vref_training_registers(
		current_vref,
		0,
		0
	);
	INFO("Write VREF: Programmed VREF = %d\n", current_vref);

	/* Turn off VREF training mode in the PHY */
	mmio_write_32(PHY_PHY_LANE_SEL, 0);
	sys_read_modify_32bits(PHY_VREF_TRAINING, 0, 1);
	INFO("Write VREF: Finished VREF DQ Training\n");

	/* Keep BEST VREF while turning off VrefDQ training mode in mode register */
	DDRTOP_init_setup_vref_training_registers(
		current_vref,
		0,
		2
	);

	/* Restore scl_test_cs to the value of the rank we are using for training */
	sys_read_modify_32bits(PHY_SCL_CONFIG_2, orig_cs_config & 0x3, 0x3);
}

/**
 * @brief Setup VREF training registers
 * @param vref_value           VREF value to be programmed in DRAM
 * @param mpr_mode             Set to 1 to issue a multi-purpose register read command to DRAM
 * @param turn_on_off_training VREF training mode in DRAM: 1=Turn ON, 2=Turn OFF, others=Program VREF trim in MR14
 */
void DDRTOP_init_setup_vref_training_registers(
	uint8_t vref_value,
	uint8_t mpr_mode,
	uint8_t turn_on_off_training)
{

	uint8_t vref_op;
	uint32_t pval;
	uint8_t mr13;

	pval = DDRTOP_mc_param_rd(MR13_DATA_0_ADDR, MR13_DATA_0_OFFSET, MR13_DATA_0_WIDTH);
	mr13 = (pval & 0x34);

	if (turn_on_off_training == 1) {
		/* Turn on VRCG in MR13 */
		pval = (200000 << 8);
		pval = (uint32_t)(pval / (2 * param_dram_clk_period));
		pval = ((pval >> 8) + ((pval & 0xff) > 0 ? 1 : 0));
		pval = (pval > 0x1ff) ? 0x1ff : pval;
		mmio_write_32(PHY_MRW_CTRL, pval << 20 | 13 << 12 | (0b11001000 | mr13) << 4 | 1);
	} else if (turn_on_off_training == 2) {
		/* Disable VRCG in MR13 */
		pval = (100000 << 8);
		pval = (uint32_t)(pval / (2 * param_dram_clk_period));
		pval = ((pval >> 8) + ((pval & 0xff) > 0 ? 1 : 0));
		pval = (pval > 0x1ff) ? 0x1ff : pval;
		mmio_write_32(PHY_MRW_CTRL, pval << 20 | 13 << 12 | (0b11000000 | mr13) << 4 | 1);
	} else {
		/* Program VREF trim in MR14 */
		if (vref_value > 50) {
			vref_op = (vref_value - 30) | (1 << 6);
		} else {
			vref_op = vref_value;
		}
		pval = (250000 << 8);
		pval = (uint32_t)(pval / (2 * param_dram_clk_period));
		pval = ((pval >> 8) + ((pval & 0xff) > 0 ? 1 : 0));
		pval = (pval > 0x1ff) ? 0x1ff : pval;

		mmio_write_32(PHY_MRW_CTRL, pval << 20 | 14 << 12 | vref_op << 4 | 1);
	}
	DDRTOP_proc_PHY_APBPOLL(PHY_SCL_START, 0x00000000, 1 << 28);
}

/**
 * @brief Write Levening
 * @param rank               Target rank
 * @param bit_lvl_from_saved Use the saved trim trained by bit leveling before
 * @return result of Write Leveling
 */
int DDRTOP_init_write_leveling(
	uint32_t rank,
	uint32_t bit_lvl_from_saved)
{
	int ret = DDRTOP_OK;

	uint32_t rddata;
	uint32_t look_for_zero, wrlvl_result;

	mmio_write_32(PHY_PHY_LANE_SEL, rank << 16);

	sys_read_modify_32bits(PHY_RANK_SEL, rank, 0b1111);

	sys_read_modify_32bits(PHY_SCL_CONFIG_2, 1 << rank, 0b11);
	mmio_write_32(PHY_WRLVL_AUTOINC_TRIM, 0b11);
	mmio_write_32(PHY_PHY_DLL_TRIM_2, 1 << 18);
	mmio_write_32(PHY_SCL_START, 1 << 30 | 1 << 28 | bit_lvl_from_saved << 21);

	/* Check for completion */
	DDRTOP_proc_PHY_APBPOLL(PHY_SCL_START, 0 << 28, 1 << 28);

	INFO("Write Leveling: Completed\n");

	mmio_write_32(PHY_PHY_LANE_SEL, rank << 16 | (0 * (8 + 1)) << 0);
	rddata = mmio_read_32(PHY_PHY_DLL_TRIM_2);
	INFO("Write Leveling: rank = %d, lane = %d: PHY_DLL_TRIM_2 = 0x%08x\n", rank, 0, rddata);

	mmio_write_32(PHY_PHY_LANE_SEL, rank << 16 | (1 * (8 + 1)) << 0);
	rddata = mmio_read_32(PHY_PHY_DLL_TRIM_2);
	INFO("Write Leveling: rank = %d, lane = %d: PHY_DLL_TRIM_2 = 0x%08x\n", rank, 1, rddata);

	/* FAILURE CHECKING */
	mmio_write_32(PHY_PHY_LANE_SEL, rank << 16);

	/* Ensure 0 was detected during write leveling before 1 to confirm we locked on rising edge */
	rddata = mmio_read_32(PHY_WRLVL_AUTOINC_TRIM);
	look_for_zero = rddata & 0x00000003;

	/* Final DQ sampling value during write leveling should be 1 */
	rddata = mmio_read_32(PHY_WRLVL_CTRL);
	wrlvl_result = rddata & 0x00000003;

	if ((wrlvl_result == 0b11) && (look_for_zero == 0b00)) {
		INFO("Write Leveling: PASSED on all lanes: rank = %d\n", rank);
	} else {
		ERROR("Write leveling: FAILED: rank = %d\n", rank);
		udelay(10);
		return DDRTOP_ERR;
	}

	/* Re-write MR2 and MR11 with MC parameter values since they were overwritten by PHY during write-leveling */
	if (rank == 0) {
		rddata = DDRTOP_mc_param_rd(MR2_DATA_F1_0_ADDR, MR2_DATA_F1_0_OFFSET, MR2_DATA_F1_0_WIDTH);
		DDRTOP_MC_MRW(02, rddata, 0b01);
		rddata = DDRTOP_mc_param_rd(MR11_DATA_F1_0_ADDR, MR11_DATA_F1_0_OFFSET, MR11_DATA_F1_0_WIDTH);
		DDRTOP_MC_MRW(11, rddata, 0b01);
	} else {
		rddata = DDRTOP_mc_param_rd(MR2_DATA_F1_1_ADDR, MR2_DATA_F1_1_OFFSET, MR2_DATA_F1_1_WIDTH);
		DDRTOP_MC_MRW(02, rddata, 0b10);
		rddata = DDRTOP_mc_param_rd(MR11_DATA_F1_1_ADDR, MR11_DATA_F1_1_OFFSET, MR11_DATA_F1_1_WIDTH);
		DDRTOP_MC_MRW(11, rddata, 0b10);
	}

	return ret;
}

/**
 * @brief PHY register initialization
 * @param num_ranks Number of ranks
 */
void DDRTOP_phy_init_routine(unsigned int num_ranks)
{
	uint8_t rl, wl;
	uint8_t mr2, mr11;
	uint32_t rd_cas_latency;
	uint32_t wr_cas_latency;
	uint32_t tmp_data;

	rl   = DDRTOP_mc_param_rd(CASLAT_LIN_F1_ADDR, CASLAT_LIN_F1_OFFSET + 1, CASLAT_LIN_F1_WIDTH - 1);
	wl   = DDRTOP_mc_param_rd(WRLAT_F1_ADDR, WRLAT_F1_OFFSET, WRLAT_F1_WIDTH);
	mr2  = DDRTOP_mc_param_rd(MR2_DATA_F1_0_ADDR, MR2_DATA_F1_0_OFFSET, MR2_DATA_F1_0_WIDTH);
	mr11 = DDRTOP_mc_param_rd(MR11_DATA_F1_0_ADDR, MR11_DATA_F1_0_OFFSET, MR11_DATA_F1_0_WIDTH);
	/* Ceiling */
	rd_cas_latency = (rl >> 1) + (rl & 1);
	wr_cas_latency = (wl >> 1) + (wl & 1);

	mmio_write_32(PHY_DQS_TRIM, 0x00002028);

	mmio_write_32(PHY_DLLM_WINDOW_SIZE, 0x00000004);
	mmio_write_32(PHY_PHY_DLL_INCR_TRIM_3, 0x00000000);
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000000);
	mmio_write_32(PHY_PHY_DLL_TRIM_3, 0x00000000);
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000006);
	mmio_write_32(PHY_PHY_DLL_TRIM_3, 0x00000000);
	mmio_write_32(PHY_SCL_WINDOW_TRIM, 0x140b002d);
	mmio_write_32(PHY_UNQ_ANALOG_DLL_1, 0x00000000);
	mmio_write_32(PHY_PHY_PAD_CTRL, 0x1000c000);

	/* Drive/ODT setting */
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000000);
	mmio_write_32(PHY_PHY_PAD_CTRL_1, (param_phyinit_02 << 16) |
									 (param_phyinit_01 <<  8) |
									 (param_phyinit_00 <<  0));
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000001);
	mmio_write_32(PHY_PHY_PAD_CTRL_1, (param_phyinit_02 << 16) |
									 (param_phyinit_01 <<  8) |
									 (param_phyinit_00 <<  0));
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000000);
	mmio_write_32(PHY_PHY_PAD_CTRL_2, (param_phyinit_03 << 0));
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000001);
	mmio_write_32(PHY_PHY_PAD_CTRL_2, (param_phyinit_03 << 0));

	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000000);
	mmio_write_32(PHY_VREF_TRAINING, (param_phyinit_10 << 4) | (1 << 2));
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000007);
	mmio_write_32(PHY_VREF_TRAINING, (param_phyinit_10 << 4) | (1 << 2));
	mmio_write_32(PHY_VREF_CA_TRAINING, 0x0000007f);
	if (param_dram_clk_period >= 1500) {
		mmio_write_32(PHY_DYNAMIC_IE_TIMER, 0x00000027);
	} else {
		mmio_write_32(PHY_DYNAMIC_IE_TIMER, 0x00000028);
	}
	tmp_data = 1000 << 8;
	tmp_data = (uint32_t)(tmp_data / param_dram_clk_period);
	tmp_data = ((tmp_data >> 8) + ((tmp_data & 0xff) > 0 ? 1 : 0));
	mmio_write_32(PHY_SCL_CONFIG_1, ((rd_cas_latency & 0xf) << 4) | (tmp_data << 12));
	mmio_write_32(PHY_DDR4_CONFIG_1, 0x00000004);
	mmio_write_32(PHY_SCL_CONFIG_2, (1 << 0) | ((wr_cas_latency & 0xf) << 8) | ((1 - (wl & 1)) << 24) | ((wl >> 5) << 25) | ((rl >> 5) << 26) | (1 << 30));
	mmio_write_32(PHY_SCL_CONFIG_3, 0x00000000);
	tmp_data = 2500 << 8;
	tmp_data = (uint32_t)(tmp_data / (param_dram_clk_period * 2));
	tmp_data -= 0x100;
	tmp_data = ((tmp_data >> 8) + ((tmp_data & 0xff) > 0 ? 1 : 0));
	mmio_write_32(PHY_DYNAMIC_WRITE_BIT_LVL, ((62 + wr_cas_latency) << 4) | ((51 + wr_cas_latency + rd_cas_latency + tmp_data) << 12));
	mmio_write_32(PHY_DYNAMIC_BIT_LVL, 0x00003e90);
	mmio_write_32(PHY_SCL_CONFIG_4, 0x00000000);
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000000);

	mmio_write_32(PHY_SCL_GATE_TIMING, 0x00000003);
	if (num_ranks == 2) {
		mmio_write_32(PHY_PHY_LANE_SEL, 0x00010000);
		mmio_write_32(PHY_SCL_GATE_TIMING, 0x00000003);
	}

	/* Use MC parameter values for Write-Leveling */
	mmio_write_32(PHY_WRLVL_DYN_ODT, (mr11 << 16) | ((mr11 & 0xf8) << 0));
	mmio_write_32(PHY_WRLVL_ON_OFF, ((mr2 | 0x80) << 16) | (mr2 << 0));

	mmio_write_32(PHY_WRLVL_STEP_SIZE, 0x00001801);
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000000);
	mmio_write_32(PHY_PHY_DLL_TRIM_CLK, 0x00000100);
	if (param_dram_clk_period < 1364) {
		tmp_data = 0x000c0104;
	} else if (param_dram_clk_period < 1500) {
		tmp_data = 0x000a0106;
	} else if (param_dram_clk_period < 2500) {
		tmp_data = 0x00080108;
	} else {
		tmp_data = 0x0006010a;
	}
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000000);
	mmio_write_32(PHY_PHY_DLL_TRIM_OP_DQS, tmp_data & 0x000001FF);
	sys_read_modify_32bits(PHY_OP_DQ_DQS_OEN_TRIM, tmp_data, 0x01FF0000);
	if (num_ranks == 2) {
		mmio_write_32(PHY_PHY_LANE_SEL, 0x00010000);
		mmio_write_32(PHY_PHY_DLL_TRIM_OP_DQS, tmp_data & 0x000001FF);
		sys_read_modify_32bits(PHY_OP_DQ_DQS_OEN_TRIM, tmp_data, 0x01FF0000);
	}
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000001);
	mmio_write_32(PHY_PHY_DLL_TRIM_OP_DQS, tmp_data & 0x000001FF);
	sys_read_modify_32bits(PHY_OP_DQ_DQS_OEN_TRIM, tmp_data, 0x01FF0000);
	if (num_ranks == 2) {
		mmio_write_32(PHY_PHY_LANE_SEL, 0x00010001);
		mmio_write_32(PHY_PHY_DLL_TRIM_OP_DQS, tmp_data & 0x000001FF);
		sys_read_modify_32bits(PHY_OP_DQ_DQS_OEN_TRIM, tmp_data, 0x01FF0000);
	}
	mmio_write_32(PHY_PHY_DLL_ADRCTRL, 0x00000200);
	mmio_write_32(PHY_PHY_DLL_RISE_FALL, 0x00000006);
	tmp_data = (uint32_t)(param_dram_clk_period * 3840 / 1736);
	tmp_data = ((tmp_data >> 8) + ((tmp_data & 0xff) > 0 ? 1 : 0));
	tmp_data = (tmp_data > 0xf) ? 0xf : tmp_data;
	mmio_write_32(PHY_PHY_DLL_RECALIB, (0x3ffff << 8) | (1 << 26) | (1 << 27) | (tmp_data << 28));
	DDRTOP_proc_PHY_APBPOLL(PHY_PHY_DLL_RISE_FALL, 0x00001000, 0x00001000);
	tmp_data = 18560 << 8;
	tmp_data = (uint32_t)(tmp_data / param_dram_clk_period) - 2176;
	tmp_data = ((tmp_data >> 8) + ((tmp_data & 0xff) > 0 ? 1 : 0));
	if (param_dram_clk_period < 1364) {
		tmp_data += 4;
	} else if (param_dram_clk_period < 1500) {
		tmp_data += 6;
	} else if (param_dram_clk_period < 2500) {
		tmp_data += 8;
	} else {
		tmp_data += 10;
	}
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000000);
	mmio_write_32(PHY_OP_DQ_TDQS2DQ, tmp_data);
	mmio_write_32(PHY_OP_DM_TDQS2DQ, tmp_data);
	if (num_ranks == 2) {
		mmio_write_32(PHY_PHY_LANE_SEL, 0x00010000);
		mmio_write_32(PHY_OP_DQ_TDQS2DQ, tmp_data);
		mmio_write_32(PHY_OP_DM_TDQS2DQ, tmp_data);
	}
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000001);
	mmio_write_32(PHY_OP_DQ_TDQS2DQ, tmp_data);
	mmio_write_32(PHY_OP_DM_TDQS2DQ, tmp_data);
	if (num_ranks == 2) {
		mmio_write_32(PHY_PHY_LANE_SEL, 0x00010001);
		mmio_write_32(PHY_OP_DQ_TDQS2DQ, tmp_data);
		mmio_write_32(PHY_OP_DM_TDQS2DQ, tmp_data);
	}
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000000);
	sys_read_modify_32bits(PHY_OP_DQ_DQS_OEN_TRIM, 0x00000123, 0x000001FF);
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000001);
	sys_read_modify_32bits(PHY_OP_DQ_DQS_OEN_TRIM, 0x00000123, 0x000001FF);
	if (num_ranks == 2) {
		mmio_write_32(PHY_PHY_LANE_SEL, 0x00010000);
		sys_read_modify_32bits(PHY_OP_DQ_DQS_OEN_TRIM, 0x00000123, 0x000001FF);
		mmio_write_32(PHY_PHY_LANE_SEL, 0x00010001);
		sys_read_modify_32bits(PHY_OP_DQ_DQS_OEN_TRIM, 0x00000123, 0x000001FF);
	}
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000000);
	mmio_write_32(PHY_SCL_LATENCY, 0x000a5066);
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00010000);
	mmio_write_32(PHY_SCL_LATENCY, 0x000a5066);
	mmio_write_32(PHY_A2D_D2A_CONFIG, 0x00000002);
	mmio_write_32(PHY_RANK_SEL, 0x00000000);
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000000);
	mmio_write_32(PHY_BIT_LVL_CONFIG, 0x00000034);
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000001);
	mmio_write_32(PHY_BIT_LVL_CONFIG, 0x00000034);
	mmio_write_32(PHY_INT_WRLVL_CTRL, 0x00000000);
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000000);
	mmio_write_32(PHY_BIT_LVL_MASK, 0x00000000);
	mmio_write_32(PHY_PHY_LANE_SEL, 0x00000001);
	mmio_write_32(PHY_BIT_LVL_MASK, 0x00000000);
	mmio_write_32(PHY_PDA_CTRL, 0x00000400);
	mmio_write_32(PHY_SCL_MASK_0, 0x00000000);

	sys_read_modify_32bits(PHY_UNIQUIFY_IO_2, 0x00000000, 0x007FFFFF);
	sys_read_modify_32bits(PHY_UNIQUIFY_IO_1, 0x00000010, 0x00000010);
	sys_read_modify_32bits(PHY_UNIQUIFY_IO_1, 0x00000000, 0x00000010);
	mmio_write_32(PHY_UNIQUIFY_IO_3, 0x05011000);
	mmio_write_32(PHY_UNIQUIFY_IO_1, 0x00000002);
	DDRTOP_proc_PHY_APBPOLL(PHY_UNIQUIFY_IO_1, 0x00000008, 0x00000008);
	mmio_write_32(PHY_UNIQUIFY_IO_1, 0x00000000);
	udelay(100);
	sys_read_modify_32bits(PHY_UNIQUIFY_IO_1, 0x9f000000, 0xBF000000);
	sys_read_modify_32bits(PHY_UNIQUIFY_IO_1, 0x00000010, 0x00000010);
	sys_read_modify_32bits(PHY_UNIQUIFY_IO_1, 0x00000000, 0x00000010);
	sys_read_modify_32bits(PHY_UNIQUIFY_IO_1, 0x00000000, 0xBF000000);
	mmio_write_32(PHY_UNIQUIFY_IO_3, 0x05009000);
	mmio_write_32(PHY_UNIQUIFY_IO_1, 0x00000001);
	DDRTOP_proc_PHY_APBPOLL(PHY_UNIQUIFY_IO_1, 0x00000004, 0x00000004);
	sys_read_modify_32bits(PHY_UNIQUIFY_IO_1, 0x00000010, 0x00000010);
	sys_read_modify_32bits(PHY_UNIQUIFY_IO_1, 0x00000000, 0x00000010);
	udelay(1);
}

/**
 * @brief DDR Initialization
 */
int DDRTOP_proc_init(void)
{
	int ret = DDRTOP_OK;

	volatile uint32_t rddata;
	uint32_t num_ranks;
	uint32_t DDRTOP_proc_status = 0;
	uint32_t scl_start_addr_X;

	INFO("DDR Initialization: driver_ver=%s, param_ver=%s\n", ddr_driver_version_str, ddr_version_str);

	INFO("Release MC and PHY reset\n");
	DDRTOP_init_reset_release();

	INFO("Initialize MC registers\n");
	DDRTOP_init_mc_swizzle();
	DDRTOP_init_mc_WRITE_REGMAP(DDRTOP_proc_status);

	rddata = DDRTOP_mc_param_rd(CS_MAP_ADDR, CS_MAP_OFFSET, CS_MAP_WIDTH);
	num_ranks = (rddata == 3) ? 2 : 1;

	INFO("Initialize PHY registers\n");
	DDRTOP_init_phy_swizzle();
	DDRTOP_phy_init_routine(num_ranks);

	INFO("Initialize DRAM Mode Registers\n");
	DDRTOP_init_dram_sequence(num_ranks);

	INFO("MC HW-DFS: Start\n");
	ret = DDRTOP_init_hwdfs_start(1);
	if (ret != DDRTOP_OK)
		return ret;


	INFO("MC HW-DFS: Change Clock Frequency\n");
	DDRTOP_init_hwdfs_clock_change();


	INFO("MC HW-DFS: Finish\n");
	ret = DDRTOP_init_hwdfs_finish();
	if (ret != DDRTOP_OK)
		return ret;

	INFO("Enable WDQS Control\n");
	sys_read_modify_32bits(PHY_PHY_PAD_CTRL, 1 << 11, 1 << 11);

	INFO("Enable Periodic Refresh\n");
	DDRTOP_mc_param_wr(TREF_ENABLE_ADDR, TREF_ENABLE_OFFSET, TREF_ENABLE_WIDTH, 1);

	/* Issue REF command twice */
	DDRTOP_mc_param_wr(AREFRESH_ADDR, AREFRESH_OFFSET, AREFRESH_WIDTH, 1);
	DDRTOP_mc_param_wr(AREFRESH_ADDR, AREFRESH_OFFSET, AREFRESH_WIDTH, 1);

	for (int rank = 0; rank < num_ranks; rank++) {
		INFO("Training: Start: rank = %d\n", rank);

		mmio_write_32(PHY_RANK_SEL, rank);
		sys_read_modify_32bits(PHY_SCL_CONFIG_2, (1 << rank), 3);
		mmio_write_32(PHY_A2D_D2A_CONFIG, (0 << 0) | (0 << 1));
		udelay(1);

		sys_read_modify_32bits(PHY_UNIQUIFY_IO_1, (1 << 4), (1 << 4));
		sys_read_modify_32bits(PHY_UNIQUIFY_IO_1, (0 << 4), (1 << 4));

		INFO("Write Leveling: Start: rank = %d\n", rank);
		ret = DDRTOP_init_write_leveling(rank, 1);
		if (ret != DDRTOP_OK)
			return ret;

		INFO("VREF DQ Training: Start: rank = %d\n", rank);
		DDRTOP_init_vref_dq_training_wrapper(rank, num_ranks);

		mmio_write_32(PHY_PHY_LANE_SEL, rank << 16);
		mmio_write_32(PHY_DISABLE_GATING_FOR_SCL, 0);

		INFO("RBL: Start: rank = %d\n", rank);
		ret = DDRTOP_init_read_bit_leveling(1);
		if (ret != DDRTOP_OK)
			return ret;

		INFO("WBL: Start: rank = %d\n", rank);
		ret = DDRTOP_init_write_bit_leveling(1);
		if (ret != DDRTOP_OK)
			return ret;

		mmio_write_32(PHY_PHY_LANE_SEL, rank << 16);
		mmio_write_32(PHY_DISABLE_GATING_FOR_SCL, 0b01);

		INFO("SCL: Start: rank = %d\n", rank);
		scl_start_addr_X = (DDRTOP_SCL_BA_ADDR << 29) | (0b000 << 26) | ((DDRTOP_SCL_COL_ADDR & 0x3FF) << 16) | (DDRTOP_SCL_ROW_ADDR & 0xFFFF);
		ret = DDRTOP_init_scl(rank,
							  1,
							  0,
							  1,
							  scl_start_addr_X,
							  16
		);
		if (ret != DDRTOP_OK)
			return ret;

		mmio_write_32(PHY_PHY_LANE_SEL, rank << 16);
		mmio_write_32(PHY_DISABLE_GATING_FOR_SCL, 0b11);
	}

	mmio_write_32(PHY_RANK_SEL, 0);
	sys_read_modify_32bits(PHY_SCL_CONFIG_2, (1 << 0), 0b11);

	INFO("Training: Finished SCL Training / Calibration\n");

	rddata = DDRTOP_mc_param_rd(ECC_ENABLE_ADDR, ECC_ENABLE_OFFSET, ECC_ENABLE_WIDTH);
	if (rddata != 0) {
		/* ECC address region */
		uint64_t ecc_prog0_range[1][2];

		/* For start address */
		ecc_prog0_range[0][0] = DDRTOP_ECC_start_addr;
		/* For end address */
		rddata = DDRTOP_mc_param_rd(CS_MAP_ADDR, CS_MAP_OFFSET, CS_MAP_WIDTH);
		if (rddata == 0x01) {
			rddata = DDRTOP_mc_param_rd(CS_VAL_UPPER_0_ADDR, CS_VAL_UPPER_0_OFFSET, CS_VAL_UPPER_0_WIDTH);
		} else {
			rddata = DDRTOP_mc_param_rd(CS_VAL_UPPER_1_ADDR, CS_VAL_UPPER_1_OFFSET, CS_VAL_UPPER_1_WIDTH);
		}
		ecc_prog0_range[0][1] = (((uint64_t)rddata << 16) | 0x00000ffffULL);
		INFO("Memory initialization for ECC: start_addr = %08" PRIx64 ", addr_space = %08" PRIx64 "\n", ecc_prog0_range[0][0], ecc_prog0_range[0][1]);

		/* Memory initialization with Zero data pattern */
		DDRTOP_proc_ecc_init0(ecc_prog0_range, (sizeof(ecc_prog0_range) / sizeof(ecc_prog0_range[0])));
	}

	INFO("Set MC registers after training\n");
	DDRTOP_init_mc_update_after_training();
	INFO("Set PHY registers after training\n");
	DDRTOP_init_phy_update_after_training();

	INFO("DDR Initialization: Finished\n");

	DDRTOP_proc_status = 1;

	return ret;
}
