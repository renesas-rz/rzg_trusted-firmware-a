/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#define DDR_DEBUG

#include <stdint.h>
#include <stddef.h>
#include <drivers/delay_timer.h>
#include <common/debug.h>
#include <cpg.h>
#include <ddr.h>

#define	DDR_PHY_BASE	((uintptr_t)0x11400000)
#define	DDR_MC_BASE		((uintptr_t)0x11600000)

#define	MCAR_SRCTL0		0x304 // TODO:Check

#define	DFIBUS_FREQ_F0_ADDR			14
#define	DFIBUS_FREQ_F0_OFFSET		16
#define	DFIBUS_FREQ_F0_WIDTH		5
#define	LP_CMD_ADDR					112
#define	LP_CMD_OFFSET				8
#define	LP_CMD_WIDTH				7
#define	LP_STATE_ADDR				118
#define	LP_STATE_OFFSET				24
#define	LP_STATE_WIDTH				7
#define	CONTROLLER_BUSY_ADDR		205
#define	CONTROLLER_BUSY_OFFSET		16
#define	CONTROLLER_BUSY_WIDTH		1
// // 51_misc
static void DDRTOP_mc_apb_rmw (uint32_t addr, uint32_t data, uint32_t mask);
void DDRTOP_mc_param_wr(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data);
void DDRTOP_mc_param_poll(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data);
static void dwc_ddrphy_apb_poll(uint32_t addr, uint32_t data, uint32_t mask);

static inline void DDRTOP_mc_apb_wr(uint32_t addr, uint32_t data)
{
	*((volatile uint32_t *)(DDR_MC_BASE + (addr << 2))) = data;
}
static inline uint32_t DDRTOP_mc_apb_rd (uint32_t addr)
{
	return *((volatile uint32_t *)(DDR_MC_BASE + (addr << 2)));
}
static inline void dwc_ddrphy_apb_wr(uint32_t addr, uint32_t data)
{
	*((volatile uint32_t *)(DDR_PHY_BASE + (addr << 2))) = data;
}
static inline uint32_t dwc_ddrphy_apb_rd(uint32_t addr)
{
	return *((volatile uint32_t *)(DDR_PHY_BASE + (addr << 2)));
}

/////////////////////////////////////////////////////////////////////////////////
// 12_retention_entry
void	ddr_retention_entry(void)
{
	// step 1	Stop DRAM access.	
	// 			After this step, it is prohibited DRAM access until completing the retention exit sequence.	
	// step 2	Disable PhyMaster. PPTTrainSetup_p0.PhyMstrTrainInterval=0
	dwc_ddrphy_apb_wr(0x020010, 0);
	// step 3	Wait for MC to ready. while(controller_busy!=0)
	DDRTOP_mc_param_poll(CONTROLLER_BUSY_ADDR, CONTROLLER_BUSY_OFFSET, CONTROLLER_BUSY_WIDTH, 0);
	// step 4	Disable SREQ_REQ. SREQ_REQ_en=0b000
	DDRTOP_mc_param_wr(MCAR_SRCTL0, 0, 3, 0);
	// step 5	//Enter SR LONG MC GATE
	DDRTOP_mc_param_wr(LP_CMD_ADDR, LP_CMD_OFFSET, LP_CMD_WIDTH, 0b1010001);			// lp_cmd=0b10_100_01
	DDRTOP_mc_param_poll(LP_STATE_ADDR, LP_STATE_OFFSET, LP_STATE_WIDTH, 0b1001010);	// while(lp_state!=0b1001010)
	// step 6	DFI frequency change to LP3
	DDRTOP_mc_param_wr(DFIBUS_FREQ_F0_ADDR, DFIBUS_FREQ_F0_OFFSET, DFIBUS_FREQ_F0_WIDTH, 0x1f);
	// step 7	dfi_init_start=1
	DDRTOP_mc_param_wr(MCAR_SRCTL0, 16, 1, 1);
	// step 8	while(DWC_DDRPHYA_APBONLY0_DfiInitCompleteShadow!=0)
	dwc_ddrphy_apb_poll(0x06E0fa, (0 << 0), (1 << 0));
	// step 9	dfi_init_start=0
	DDRTOP_mc_param_wr(MCAR_SRCTL0, 16, 1, 0);
	// step 10	while(DWC_DDRPHYA_APBONLY0_DfiInitCompleteShadow!=1)
	dwc_ddrphy_apb_poll(0x06E0fa, (1 << 0), (1 << 0));
	// step 11	PwrOkIn=0
	// step 12	Wait 18 DfiClk.
	// step 13	Shutdown VDD, and/or VAA.
}

//DDRTOP_mc
static void DDRTOP_mc_apb_rmw (uint32_t addr, uint32_t data, uint32_t mask)
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
		// DDRTOP_proc_wait_PCLK(10);
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
	tmp_mask = ((1 << width) - 1) << offset;
	DDRTOP_mc_apb_rmw(addr, tmp_data, tmp_mask);
}

// task automatic DDRTOP_mc_param_poll(
// 		inpu	[31:0]	addr
// 	,	input	int		offset
// 	,	input	int		width
// 	,	input	[31:0]	data
void DDRTOP_mc_param_poll(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data)
{
	uint32_t tmp_data;
	uint32_t tmp_mask;
	tmp_data = data << offset;
	tmp_mask = ((1 << width) - 1) << offset;
	DDRTOP_mc_apb_poll(addr, tmp_data, tmp_mask);
}

//ddrphy_top_wrap
#if 0
static void dwc_ddrphy_apb_rmw(uint32_t addr, uint32_t data, uint32_t mask)
{
	uint32_t tmp_data;
	tmp_data = DDRTOP_mc_apb_rd(addr);
	data = (data & mask) | (tmp_data & (~mask));
	DDRTOP_mc_apb_wr(addr, data);
}
#endif

static void dwc_ddrphy_apb_poll(uint32_t addr, uint32_t data, uint32_t mask)
{
	uint32_t tmp_data;
	tmp_data = DDRTOP_mc_apb_rd(addr);
	tmp_data &= mask;
	while (tmp_data != data) {
		// DDRTOP_proc_wait_PCLK(10);
		udelay(1);
		tmp_data = DDRTOP_mc_apb_rd(addr);
		tmp_data &= mask;
	}
}
