/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <drivers/delay_timer.h>
#include <common/debug.h>
#include <cpg.h>

extern const char ddr_an_version[];
extern const uint32_t ddrmc_ini_dat[][2];
extern const uint32_t ddrmc_ini_dat_size;
extern const uint32_t ddrphy_ini_data_c[][2];
extern const uint32_t ddrphy_ini_data_c_size;
extern const uint32_t ddrphy_ini_swizzle[][2];
extern const uint32_t ddrphy_ini_swizzle_size;
extern const uint32_t ddrphy_train1d_data_d[][2];
extern const uint32_t ddrphy_train1d_data_d_size;
extern const uint32_t ddrphy_train1d_data_f[][2];
extern const uint32_t ddrphy_train1d_data_f_size;
extern const uint32_t ddrphy_train1d_data_g1[][2];
extern const uint32_t ddrphy_train1d_data_g1_size;
extern const uint32_t ddrphy_train1d_data_g2[][2];
extern const uint32_t ddrphy_train1d_data_g2_size;
extern const uint32_t ddrphy_train1d_data_h[][2];
extern const uint32_t ddrphy_train1d_data_h_size;
extern const uint32_t ddrphy_train2d_data_d[][2];
extern const uint32_t ddrphy_train2d_data_d_size;
extern const uint32_t ddrphy_train2d_data_f[][2];
extern const uint32_t ddrphy_train2d_data_f_size;
extern const uint32_t ddrphy_train2d_data_g1[][2];
extern const uint32_t ddrphy_train2d_data_g1_size;
extern const uint32_t ddrphy_train2d_data_g2[][2];
extern const uint32_t ddrphy_train2d_data_g2_size;
extern const uint32_t ddrphy_train2d_data_h[][2];
extern const uint32_t ddrphy_train2d_data_h_size;
extern const uint32_t ddrphy_train2d_data_i[][2];
extern const uint32_t ddrphy_train2d_data_i_size;

#define	DDR_PHY_BASE	((uintptr_t)0x11400000)
#define	DDR_MC_BASE		((uintptr_t)0x11600000)

#define	MCAR_SRCTL0		0x304 // TODO:Check

#define	PHY_INDEP_INIT_MODE_ADDR	14
#define	PHY_INDEP_INIT_MODE_OFFSET	8
#define	PHY_INDEP_INIT_MODE_WIDTH	1
#define	DFIBUS_FREQ_F0_ADDR			14
#define	DFIBUS_FREQ_F0_OFFSET		16
#define	DFIBUS_FREQ_F0_WIDTH		5
#define	PWRUP_SREFRESH_EXIT_ADDR	67
#define	PWRUP_SREFRESH_EXIT_OFFSET	16
#define	PWRUP_SREFRESH_EXIT_WIDTH	1
#define	LP_CMD_ADDR					112
#define	LP_CMD_OFFSET				8
#define	LP_CMD_WIDTH				7
#define	LPI_WAKEUP_EN_ADDR			192
#define	LPI_WAKEUP_EN_OFFSET		0
#define	LPI_WAKEUP_EN_WIDTH			6
#define	LP_STATE_ADDR				118
#define	LP_STATE_OFFSET				24
#define	LP_STATE_WIDTH				7
#define	MEM_DP_REDUCTION_ADDR		200
#define	MEM_DP_REDUCTION_OFFSET		24
#define	MEM_DP_REDUCTION_WIDTH		1
#define	CONTROLLER_BUSY_ADDR		205
#define	CONTROLLER_BUSY_OFFSET		16
#define	CONTROLLER_BUSY_WIDTH		1
#define	INT_STATUS_INIT_ADDR		370
#define	INT_STATUS_INIT_OFFSET		24
#define	INT_STATUS_INIT_WIDTH		8
#define	TDFI_PHY_RDLAT_F0_ADDR		541
#define	TDFI_PHY_RDLAT_F0_OFFSET	0
#define	TDFI_PHY_RDLAT_F0_WIDTH		7
#define	TDFI_WRDATA_DELAY_ADDR		570
#define	TDFI_WRDATA_DELAY_OFFSET	8
#define	TDFI_WRDATA_DELAY_WIDTH		8
#define	BSTLEN_ADDR					47
#define	BSTLEN_OFFSET				8
#define	BSTLEN_WIDTH				4
#define	START_ADDR					0
#define	START_OFFSET				0
#define	START_WIDTH					1
#define	DRAM_CLASS_ADDR				0
#define	DRAM_CLASS_OFFSET			8
#define	DRAM_CLASS_WIDTH			4


// 11_initialization
void ddr_setup(void);
// 12_retention_entry
void retention_entry(void);
// 13_retention_exit
void retention_exit(void);

// 21_setup_mc
static void setup_mc(void);
// 30_phyinit_param_ddr4
// 30_phyinit_param_lpddr4
// 31_phyinit
static void phyinit(void);
// 32_phyinit_swizzle_ddr4
// 32_phyinit_swizzle_lpddr4
// 33_phyinit_d2h_1d
static void phyinit_d2h_1d(void);
// 34_phyinit_d2h_2d
static void phyinit_d2h_2d(void);
// 35_phyinit_i
static void phyinit_i(void);
// 36_phyinit_j
static void phyinit_j(void);
// 37_phyinit_mc
static void	phyinit_mc(void);
// 41_save_rst_csr
static void	save_rst_csr(void);
// 42_restore_setcsr
static void	restore_setcsr(void);
// 43_update_mc
static void update_mc(void);
// 51_misc
// static int8_t dwc_ddrphy_CDD_int(uint8_t val);
// static int8_t dwc_ddrphy_CDD_abs(uint8_t val);
static void dwc_ddrphy_phyinit_userCustom_G_waitFwDone(uint32_t	sel_train);
static uint32_t get_mail(uint8_t mode_32bits);
static void DDRTOP_mc_apb_rmw (uint32_t addr, uint32_t data, uint32_t mask);
static void DDRTOP_mc_apb_poll(uint32_t addr, uint32_t data, uint32_t mask);
static void DDRTOP_mc_param_wr(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data);
static uint32_t DDRTOP_mc_param_rd(uint32_t addr, uint32_t offset, uint32_t width);
static void DDRTOP_mc_param_poll(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data);
// static void dwc_ddrphy_apb_rmw(uint32_t addr, uint32_t data, uint32_t mask);
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


static void decode_major_message (uint32_t mail);
static void decode_streaming_message (uint8_t sel_train);

/////////////////////////////////////////////////////////////////////////////////
// 11_initialization
void ddr_setup(void)
{
	INFO("BL2: setup DDR (Rev. %s)\n", ddr_an_version);
	// step 1	Bring up VDDQ, VDD, and VAA.
	// step 2	Assert all resets.
	//			rst_n = 0, axiY_ARESETn = 0, MC_PRESETn = 0, PwrOkIn = 0, Reset = 1, PHY_PRESETN = 0
	// step 3	Start the following clocks; DfiClk, axiY_ACLK, PCLK
	// step 4	Wait at least 8 DfiClk cycles.
	// step 5	De-assert the following resets; rst_n = 1, PwrOkIn = 1
	// step 6	Wait at least the longest cycles of the followings: 1 axiY_ACLK, 1 PCLK
	// step 7	De-assert the following resets; axiY_ARESETn = 1, MC_PRESETn = 1
	// step 8	Wait at least the longest cycles of the followings: 2 axiY_ACLK, 2 PCLK
	// step 9	Initialize MC registers.
	cpg_active_ddr1();
	// Initialize MC registers.
	setup_mc();
	// step 10
	DDRTOP_mc_param_wr(PWRUP_SREFRESH_EXIT_ADDR, PWRUP_SREFRESH_EXIT_OFFSET, PWRUP_SREFRESH_EXIT_WIDTH, 0);
	DDRTOP_mc_param_wr(PHY_INDEP_INIT_MODE_ADDR, PHY_INDEP_INIT_MODE_OFFSET, PHY_INDEP_INIT_MODE_WIDTH, 0);
	// step 11	De-assert PHY resets; Reset = 0, PHY_PRESETn = 1
	cpg_active_ddr2();
	// step 12	Wait at least 2 PCLK cycles.
	udelay(1);
	// step 13	Initialize PHY Configuration.
	phyinit();
	// step 14	Run 1D training.
	INFO("DDR: step 14: Run 1D training.\n");
	phyinit_d2h_1d();
	// step 15	Run 2D training.
	INFO("DDR: step 15: Run 2D training.\n");
	phyinit_d2h_2d();
	// step 16	Update MC registers after training.
	INFO("DDR: step 16: Update MC registers after training.\n");
	phyinit_mc();
	// step 17	Save the retention registers.
	INFO("DDR: step 17: Save the retention registers.\n");
	save_rst_csr();
	// step 18	Load PHY Init Engine Image.
	INFO("DDR: step 18: Load PHY Init Engine Image.\n");
	phyinit_i();
	// step 19	Initialize the PHY to Mission Mode through DFI Initialization.
	INFO("DDR: step 19: Initialize the PHY to Mission Mode through DFI Initialization.\n");
	phyinit_j();
	// step 20	Update MC registers.
	INFO("DDR: step 20: Update MC registers.\n");
	update_mc();
	//	Completed the initialization. DRAM accessable.
}

/////////////////////////////////////////////////////////////////////////////////
// 12_retention_entry
void	retention_entry(void)
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

/////////////////////////////////////////////////////////////////////////////////
// 13_retention_exit
void retention_exit(void)
{
	// step 1	Bring up VDD, and VAA. VAA are limited to a maximum supply ramp rate of 5 mV/us.
	// step 2	Assert all resets.
 	//			rst_n = 0, axiY_ARESETn = 0, MC_PRESETn = 0, PwrOkIn = 0, Reset = 1, PHY_PRESETN = 0
	// step 3	Start the following clocks; DfiClk, axiY_ACLK, PCLK
	// step 4	Wait at least 8 DfiClk cycles.
	// step 5	De-assert the following resets; rst_n = 1, PwrOkIn = 1
	// step 6	Wait at least the longest cycles of the followings: 1 axiY_ACLK, 1 PCLK
	// step 7	De-assert the following resets; axiY_ARESETn = 1, MC_PRESETn = 1
	// step 8	Wait at least the longest cycles of the followings: 2 axiY_ACLK, 2 PCLK
	// step 9	Initialize MC registers.
	// 			"See the following sheet according to use-case.
	cpg_active_ddr1();
	setup_mc();
	// step 10	De-assert PHY resets; Reset = 0, PHY_PRESETn = 1
	cpg_active_ddr2();
	// step 11	Wait at least 2 PCLK cycles.
	udelay(1);
	// step 12	Initialize PHY Configuration.
	phyinit();
	// step 13	Restore the saved retention registers
	restore_setcsr();
	// step 14	Load PHY Init Engine Image.
	phyinit_i();
	// step 15	Initialize the PHY to Mission Mode through DFI Initialization.
	phyinit_j();
	// step 16	Update MC registers.
	update_mc();
	// Completed the standby mode exit sequence. DRAM accessable.
}

/////////////////////////////////////////////////////////////////////////////////
// 21_setup_mc
static void setup_mc(void)
{
	uint32_t	i;

	for(i = 0; i < ddrmc_ini_dat_size; i++)
	{
		DDRTOP_mc_apb_wr(ddrmc_ini_dat[i][0], ddrmc_ini_dat[i][1]);
	}
}

/////////////////////////////////////////////////////////////////////////////////
// 31_phyinit
static void phyinit(void)
{
	uint32_t	i;
	// param_phyinit_c
	for (i = 0; i < ddrphy_ini_data_c_size; i++)
	{
		dwc_ddrphy_apb_wr(ddrphy_ini_data_c[i][0], ddrphy_ini_data_c[i][1]);
	}
	// phyinit_param();
	// Program pin swizzling registers.
	for (i = 0; i < ddrphy_ini_swizzle_size; i++)
	{
		dwc_ddrphy_apb_wr(ddrphy_ini_swizzle[i][0], ddrphy_ini_swizzle[i][1]);
	}
}

/////////////////////////////////////////////////////////////////////////////////
// 33_phyinit_d2h_1d
static void phyinit_d2h_1d(void)
{
	uint32_t	i;
//	uint32_t	val;
	uint32_t	sel_train;
	// param_phyinit_d_1d
	for (i = 0; i < ddrphy_train1d_data_d_size; i++)
	{
		// See "30_phyinit_param_DDR4" sheet for param_phyinit_d_1d.
		dwc_ddrphy_apb_wr(ddrphy_train1d_data_d[i][0], ddrphy_train1d_data_d[i][1]);
	}
	// Set DfiClk to the desired frequency for pstate 0.
	// See "30_phyinit_param_DDR4" sheet for param_phyinit_f_1d_0.
	// param_phyinit_f_1d_0
	for (i = 0; i < ddrphy_train1d_data_f_size; i++)
	{
		dwc_ddrphy_apb_wr(ddrphy_train1d_data_f[i][0], ddrphy_train1d_data_f[i][1]);
	}
//	val = DDRTOP_mc_param_rd(DRAM_CLASS_ADDR, DRAM_CLASS_OFFSET, DRAM_CLASS_WIDTH);
	sel_train = 3;//(val == 0x6) ? 0 : 1;
	for (i = 0; i < ddrphy_train1d_data_g1_size; i++)
	{
		dwc_ddrphy_apb_wr(ddrphy_train1d_data_g1[i][0], ddrphy_train1d_data_g1[i][1]);
	}
	dwc_ddrphy_phyinit_userCustom_G_waitFwDone(sel_train);
	for (i = 0; i < ddrphy_train1d_data_g2_size; i++)
	{
		dwc_ddrphy_apb_wr(ddrphy_train1d_data_g2[i][0], ddrphy_train1d_data_g2[i][1]);
	}
	for (i = 0; i < ddrphy_train1d_data_h_size; i++)
	{
		dwc_ddrphy_apb_wr(ddrphy_train1d_data_h[i][0], ddrphy_train1d_data_h[i][1]);
	}
}

/////////////////////////////////////////////////////////////////////////////////
// 34_phyinit_d2h_2d
static void phyinit_d2h_2d(void)
{
	uint32_t	i;
	// Set DfiClk to the desired frequency for pstate 0.
	//param_phyinit_d_2d
	for (i = 0; i < ddrphy_train2d_data_d_size; i++)
	{
		// See "30_phyinit_param_DDR4" sheet for param_phyinit_d_2d.
		dwc_ddrphy_apb_wr(ddrphy_train2d_data_d[i][0], ddrphy_train2d_data_d[i][1]);
	}
	// See "30_phyinit_param_DDR4" sheet for param_phyinit_f_2d_0.
	//param_phyinit_f_2d_0
	for (i = 0; i < ddrphy_train2d_data_f_size; i++)
	{
		dwc_ddrphy_apb_wr(ddrphy_train2d_data_f[i][0], ddrphy_train2d_data_f[i][1]);
	}
	for (i = 0; i < ddrphy_train2d_data_g1_size; i++)
	{
		dwc_ddrphy_apb_wr(ddrphy_train2d_data_g1[i][0], ddrphy_train2d_data_g1[i][1]);
	}
	dwc_ddrphy_phyinit_userCustom_G_waitFwDone(2);
	for (i = 0; i < ddrphy_train2d_data_g2_size; i++)
	{
		dwc_ddrphy_apb_wr(ddrphy_train2d_data_g2[i][0], ddrphy_train2d_data_g2[i][1]);
	}
	for (i = 0; i < ddrphy_train2d_data_h_size; i++)
	{
		dwc_ddrphy_apb_wr(ddrphy_train2d_data_h[i][0], ddrphy_train2d_data_h[i][1]);
	}
}

/////////////////////////////////////////////////////////////////////////////////
// 35_phyinit_i
static void phyinit_i(void)
{
	uint32_t	i;
	//param_phyinit_i
	for (i = 0; i < ddrphy_train2d_data_i_size; i++)
	{
		// See "30_phyinit_param_DDR4" sheet for param_phyinit_i.
		dwc_ddrphy_apb_wr(ddrphy_train2d_data_i[i][0], ddrphy_train2d_data_i[i][1]);
	}
}

/////////////////////////////////////////////////////////////////////////////////
// 36_phyinit_j
static void phyinit_j(void)
{
	DDRTOP_mc_param_wr(START_ADDR, START_OFFSET, START_WIDTH, 1);
	DDRTOP_mc_param_poll(INT_STATUS_INIT_ADDR, INT_STATUS_INIT_OFFSET+1, 1, 1);
}

/////////////////////////////////////////////////////////////////////////////////
// 37_phyinit_mc
static void	phyinit_mc(void)
{
	uint32_t x;
	uint32_t val;
	uint32_t num_rank;
	uint32_t num_byte;
	uint32_t tctrl_delay;
	uint32_t TxDqsDly;
	uint32_t BL;

	// step 1
	dwc_ddrphy_apb_wr(0x6E000, 0x0);
	// step 2
	num_rank = 1;
	val = DDRTOP_mc_param_rd(MEM_DP_REDUCTION_ADDR, MEM_DP_REDUCTION_OFFSET, MEM_DP_REDUCTION_WIDTH);
	num_byte = (val == 1) ? 2 : 4;
	val = dwc_ddrphy_apb_rd(0x02002e);
	tctrl_delay = ((val >> 1) + (val & 1)) + 8;
	val = DDRTOP_mc_param_rd(BSTLEN_ADDR, BSTLEN_OFFSET, BSTLEN_WIDTH);
	BL = (1 << val);
	// step 3
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
	DDRTOP_mc_param_wr(TDFI_PHY_RDLAT_F0_ADDR, TDFI_PHY_RDLAT_F0_OFFSET, TDFI_PHY_RDLAT_F0_WIDTH, val);
	// step 4
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
	TxDqsDly = ((x << 6) & 0xf) + (((x >> 4) & 0x01) + (( x >> 3 ) & 0x1)); //TxDqsDly = x[9:6] + ceil(x[4:0]/32)
	val = tctrl_delay + (6 + (BL / 2)) + TxDqsDly;
	DDRTOP_mc_param_wr(TDFI_WRDATA_DELAY_ADDR, TDFI_WRDATA_DELAY_OFFSET, TDFI_WRDATA_DELAY_WIDTH, val);
	// step 5
	dwc_ddrphy_apb_wr(0x6E000, 0x1);
}

/////////////////////////////////////////////////////////////////////////////////
// 41_save_rst_csr

// Register offset address	Filter
// 1D Training IO Retention Register List
static uint32_t	reg_list_1d[] =
{
	0x0200b2,	0x0200cb,	0x020024,	0x010043,	0x011043,	0x012043,	0x013043,	0x010143,
	0x011143,	0x012143,	0x013143,	0x000080,	0x001080,	0x002080,	0x003080,	0x004080,
	0x005080,	0x006080,	0x007080,	0x008080,	0x009080,	0x010080,	0x011080,	0x012080,
	0x013080,	0x010180,	0x011180,	0x012180,	0x013180,	0x010081,	0x011081,	0x012081,
	0x013081,	0x010181,	0x011181,	0x012181,	0x013181,	0x010082,	0x011082,	0x012082,
	0x013082,	0x010182,	0x011182,	0x012182,	0x013182,	0x010083,	0x011083,	0x012083,
	0x013083,	0x010183,	0x011183,	0x012183,	0x013183,	0x0101d0,	0x0111d0,	0x0121d0,
	0x0131d0,	0x0100d0,	0x0110d0,	0x0120d0,	0x0130d0,	0x0101d1,	0x0111d1,	0x0121d1,
	0x0131d1,	0x0100d1,	0x0110d1,	0x0120d1,	0x0130d1,	0x0101d2,	0x0111d2,	0x0121d2,
	0x0131d2,	0x0100d2,	0x0110d2,	0x0120d2,	0x0130d2,	0x0101d3,	0x0111d3,	0x0121d3,
	0x0131d3,	0x0100d3,	0x0110d3,	0x0120d3,	0x0130d3,	0x010868,	0x011868,	0x012868,
	0x013868,	0x010768,	0x011768,	0x012768,	0x013768,	0x010668,	0x011668,	0x012668,
	0x013668,	0x010568,	0x011568,	0x012568,	0x013568,	0x010468,	0x011468,	0x012468,
	0x013468,	0x010368,	0x011368,	0x012368,	0x013368,	0x010268,	0x011268,	0x012268,
	0x013268,	0x010168,	0x011168,	0x012168,	0x013168,	0x010068,	0x011068,	0x012068,
	0x013068,	0x010869,	0x011869,	0x012869,	0x013869,	0x010769,	0x011769,	0x012769,
	0x013769,	0x010669,	0x011669,	0x012669,	0x013669,	0x010569,	0x011569,	0x012569,
	0x013569,	0x010469,	0x011469,	0x012469,	0x013469,	0x010369,	0x011369,	0x012369,
	0x013369,	0x010269,	0x011269,	0x012269,	0x013269,	0x010169,	0x011169,	0x012169,
	0x013169,	0x010069,	0x011069,	0x012069,	0x013069,	0x01086a,	0x01186a,	0x01286a,
	0x01386a,	0x01076a,	0x01176a,	0x01276a,	0x01376a,	0x01066a,	0x01166a,	0x01266a,
	0x01366a,	0x01056a,	0x01156a,	0x01256a,	0x01356a,	0x01046a,	0x01146a,	0x01246a,
	0x01346a,	0x01036a,	0x01136a,	0x01236a,	0x01336a,	0x01026a,	0x01126a,	0x01226a,
	0x01326a,	0x01016a,	0x01116a,	0x01216a,	0x01316a,	0x01006a,	0x01106a,	0x01206a,
	0x01306a,	0x01086b,	0x01186b,	0x01286b,	0x01386b,	0x01076b,	0x01176b,	0x01276b,
	0x01376b,	0x01066b,	0x01166b,	0x01266b,	0x01366b,	0x01056b,	0x01156b,	0x01256b,
	0x01356b,	0x01046b,	0x01146b,	0x01246b,	0x01346b,	0x01036b,	0x01136b,	0x01236b,
	0x01336b,	0x01026b,	0x01126b,	0x01226b,	0x01326b,	0x01016b,	0x01116b,	0x01216b,
	0x01316b,	0x01006b,	0x01106b,	0x01206b,	0x01306b,	0x01008c,	0x01108c,	0x01208c,
	0x01308c,	0x01018c,	0x01118c,	0x01218c,	0x01318c,	0x01008d,	0x01108d,	0x01208d,
	0x01308d,	0x01018d,	0x01118d,	0x01218d,	0x01318d,	0x01008e,	0x01108e,	0x01208e,
	0x01308e,	0x01018e,	0x01118e,	0x01218e,	0x01318e,	0x01008f,	0x01108f,	0x01208f,
	0x01308f,	0x01018f,	0x01118f,	0x01218f,	0x01318f,	0x0108c0,	0x0118c0,	0x0128c0,
	0x0138c0,	0x0107c0,	0x0117c0,	0x0127c0,	0x0137c0,	0x0106c0,	0x0116c0,	0x0126c0,
	0x0136c0,	0x0105c0,	0x0115c0,	0x0125c0,	0x0135c0,	0x0104c0,	0x0114c0,	0x0124c0,
	0x0134c0,	0x0103c0,	0x0113c0,	0x0123c0,	0x0133c0,	0x0102c0,	0x0112c0,	0x0122c0,
	0x0132c0,	0x0101c0,	0x0111c0,	0x0121c0,	0x0131c0,	0x0100c0,	0x0110c0,	0x0120c0,
	0x0130c0,	0x0108c1,	0x0118c1,	0x0128c1,	0x0138c1,	0x0107c1,	0x0117c1,	0x0127c1,
	0x0137c1,	0x0106c1,	0x0116c1,	0x0126c1,	0x0136c1,	0x0105c1,	0x0115c1,	0x0125c1,
	0x0135c1,	0x0104c1,	0x0114c1,	0x0124c1,	0x0134c1,	0x0103c1,	0x0113c1,	0x0123c1,
	0x0133c1,	0x0102c1,	0x0112c1,	0x0122c1,	0x0132c1,	0x0101c1,	0x0111c1,	0x0121c1,
	0x0131c1,	0x0100c1,	0x0110c1,	0x0120c1,	0x0130c1,	0x0108c2,	0x0118c2,	0x0128c2,
	0x0138c2,	0x0107c2,	0x0117c2,	0x0127c2,	0x0137c2,	0x0106c2,	0x0116c2,	0x0126c2,
	0x0136c2,	0x0105c2,	0x0115c2,	0x0125c2,	0x0135c2,	0x0104c2,	0x0114c2,	0x0124c2,
	0x0134c2,	0x0103c2,	0x0113c2,	0x0123c2,	0x0133c2,	0x0102c2,	0x0112c2,	0x0122c2,
	0x0132c2,	0x0101c2,	0x0111c2,	0x0121c2,	0x0131c2,	0x0100c2,	0x0110c2,	0x0120c2,
	0x0130c2,	0x0108c3,	0x0118c3,	0x0128c3,	0x0138c3,	0x0107c3,	0x0117c3,	0x0127c3,
	0x0137c3,	0x0106c3,	0x0116c3,	0x0126c3,	0x0136c3,	0x0105c3,	0x0115c3,	0x0125c3,
	0x0135c3,	0x0104c3,	0x0114c3,	0x0124c3,	0x0134c3,	0x0103c3,	0x0113c3,	0x0123c3,
	0x0133c3,	0x0102c3,	0x0112c3,	0x0122c3,	0x0132c3,	0x0101c3,	0x0111c3,	0x0121c3,
	0x0131c3,	0x0100c3,	0x0110c3,	0x0120c3,	0x0130c3,	0x010020,	0x011020,	0x012020,
	0x013020,	0x02007c,	0x02007d,
};
// 2D Training IO Retention Register List
static uint32_t	reg_list_2d[] =
{
	0x01008c,	0x01108c,	0x01208c,	0x01308c,	0x01018c,	0x01118c,	0x01218c,	0x01318c,
	0x01008d,	0x01108d,	0x01208d,	0x01308d,	0x01018d,	0x01118d,	0x01218d,	0x01318d,
	0x01008e,	0x01108e,	0x01208e,	0x01308e,	0x01018e,	0x01118e,	0x01218e,	0x01318e,
	0x01008f,	0x01108f,	0x01208f,	0x01308f,	0x01018f,	0x01118f,	0x01218f,	0x01318f,
	0x010840,	0x011840,	0x012840,	0x013840,	0x010740,	0x011740,	0x012740,	0x013740,
	0x010640,	0x011640,	0x012640,	0x013640,	0x010540,	0x011540,	0x012540,	0x013540,
	0x010440,	0x011440,	0x012440,	0x013440,	0x010340,	0x011340,	0x012340,	0x013340,
	0x010240,	0x011240,	0x012240,	0x013240,	0x010140,	0x011140,	0x012140,	0x013140,
	0x010040,	0x011040,	0x012040,	0x013040,	0x010830,	0x011830,	0x012830,	0x013830,
	0x010730,	0x011730,	0x012730,	0x013730,	0x010630,	0x011630,	0x012630,	0x013630,
	0x010530,	0x011530,	0x012530,	0x013530,	0x010430,	0x011430,	0x012430,	0x013430,
	0x010330,	0x011330,	0x012330,	0x013330,	0x010230,	0x011230,	0x012230,	0x013230,
	0x010130,	0x011130,	0x012130,	0x013130,	0x010030,	0x011030,	0x012030,	0x013030,
};
//,	Controller Register List
static uint32_t	reg_list_ctl[] =
{
	541,	570,
};

static void	save_rst_csr(void)
{
	uint32_t i;
	dwc_ddrphy_apb_wr(0x06E000, 0); // Write the MicroContMuxSel CSR to 0x0 to allow access to the internal CSRs.
	dwc_ddrphy_apb_wr(0x066080, 3); // Write the UcclkHclkEnables CSR to 0x3 to enable all the clocks so the reads can complete.
	// The retention registers are listed in Table 41-1.
	// Read all the retention registers, and save them to the storage other than DRAM.
	for(i = 0; i < sizeof(reg_list_1d); i++)
	{
		;	// save_reg(reg_list_1d[i]);
	}
	for(i = 0; i < sizeof(reg_list_2d); i++)
	{
		;	// save_reg(reg_list_2d[i]);
	}
	for(i = 0; i < sizeof(reg_list_ctl); i++)
	{
		;	// save_reg(reg_list_ctl[i]);
	}
	dwc_ddrphy_apb_wr(0x066080, 0); // Write the UcclkHclkEnables CSR to disable the appropriate clocks after all reads done.
	dwc_ddrphy_apb_wr(0x06E000, 1); // Write the MicroContMuxSel CSR to 0x1 to isolate the internal CSRs during mission mode.
}


/////////////////////////////////////////////////////////////////////////////////
// 42_restore_setcsr
static void	restore_setcsr(void)
{
	uint32_t	i;
	dwc_ddrphy_apb_wr(0x06E000, 0);	// Write the MicroContMuxSel CSR to 0x0 to allow access to the internal CSRs.
	dwc_ddrphy_apb_wr(0x066080, 3);	// Write the UcclkHclkEnables CSR to 0x3 to enable all the clocks so the reads can complete.
	// See step 3 in "41_Save_RetCSR" sheet for the saved retention registers.
	// Read the saved retention registers, and write them to registers.
	for(i = 0; i < sizeof(reg_list_1d); i++)
	{
		;	// restore_reg(reg_list_1d[i]);
	}
	for(i = 0; i < sizeof(reg_list_2d); i++)
	{
		;	// restore_reg(reg_list_2d[i]);
	}
	for(i = 0; i < sizeof(reg_list_ctl); i++)
	{
		;	// restore_reg(reg_list_ctl[i]);
	}
	dwc_ddrphy_apb_wr(0x066080, 0);	// Write the UcclkHclkEnables CSR to disable the appropriate clocks after all reads done.
	dwc_ddrphy_apb_wr(0x06E000, 1);	// Write the MicroContMuxSel CSR to 0x1 to isolate the internal CSRs during mission mode.
}

/////////////////////////////////////////////////////////////////////////////////
// 43_update_mc

static void update_mc(void)
{
	DDRTOP_mc_param_wr(LPI_WAKEUP_EN_ADDR, LPI_WAKEUP_EN_OFFSET, LPI_WAKEUP_EN_WIDTH, 0x1F);
}

/////////////////////////////////////////////////////////////////////////////////
// 51_misc

//--------------------------------------
// dwc_ddrphy_top
//--------------------------------------
//For calculation of CDD
#if 0
static int8_t dwc_ddrphy_CDD_int(uint8_t val)
{
	return ( int8_t)((((val >> 7) & 0x1) == 1) ? -((0x7f ^ ((val >> 0) & 0x7f)) + 1) : ((val >> 0) & 0x7f));
}
#endif
#if 0
static int8_t dwc_ddrphy_CDD_abs(uint8_t val)
{
	return (uint8_t)((((val >> 7) & 0x1) == 1) ?  ((0x7f ^ ((val >> 0) & 0x7f)) + 1) : ((val >> 0) & 0x7f));
}
#endif

//---------------------------------------------------------------------------------------
//sel_train: 0=ddr3, 1=ddr4, 2=ddr4_2d
static void dwc_ddrphy_phyinit_userCustom_G_waitFwDone(uint32_t sel_train)
{
	uint32_t mail;

	mail = 0;
	// Wait at least 10 DfiClk cycles.
	do {
		// Wait at least 500 cycles.
		udelay(10);
		mail = get_mail(0);
#ifdef DEBUG
		// for debug
		decode_major_message(mail);
		if (mail == 0x08) {
		 	decode_streaming_message(sel_train);
		}

		INFO("INFO: Mail Box = %x(%x)\r", mail, sel_train);
#endif
	}	while ((mail != 0xff) && (mail != 0x07));
	INFO("\n");
	if (mail == 0xff) {
		ERROR("Error: Firmware training failed.\n");
		panic();
	}
}

//---------------------------------------------------------------------------------------
static uint32_t get_mail(uint8_t mode_32bits)
{
	uint32_t wd_timer2;
	uint32_t rd_data;
	uint32_t mail;

	wd_timer2 = 0;
	do {
		rd_data = dwc_ddrphy_apb_rd(0x06E004);
	}	while ((rd_data & 0x1) != 0);
	mail = dwc_ddrphy_apb_rd(0x06E032);
	if(mode_32bits) {
		uint32_t high_byte_data = dwc_ddrphy_apb_rd(0x0006E034);
		mail = (high_byte_data << 16) | mail;
	}
	dwc_ddrphy_apb_wr(0x06E031, 0x0000);

	do {
		rd_data = dwc_ddrphy_apb_rd(0x06E004);
		INFO("INFO: Msg read. Waiting acknowledgement from uCtl ... rd_data = 0x%x\n", rd_data);
		// Watchdog timer to ensure no infiit looping during polling
		//INFO("INFO: Watchdog timer2   = %d", wd_timer2);
		wd_timer2++;
		if(wd_timer2 > 1000) {
			ERROR("TC ERROR: Watchdog timer2 overflow\n");
			panic();
		}
	}	while ((rd_data & 0x1) == 0);
	dwc_ddrphy_apb_wr (0x06E031, 0x0001);

	return mail;
}


//DDRTOP_mc
static void DDRTOP_mc_apb_rmw (uint32_t addr, uint32_t data, uint32_t mask)
{
	uint32_t tmp_data;
	tmp_data = DDRTOP_mc_apb_rd(addr);
	data = (data & mask) | (tmp_data & (~mask));
	DDRTOP_mc_apb_wr(addr, data);
}

static void DDRTOP_mc_apb_poll(uint32_t addr, uint32_t data, uint32_t mask)
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

static void DDRTOP_mc_param_wr(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data)
{
	uint32_t tmp_data;
	uint32_t tmp_mask;
	tmp_data = data << offset;
	tmp_mask = ((1 << width) - 1) << offset;
	DDRTOP_mc_apb_rmw(addr, tmp_data, tmp_mask);
}

static uint32_t DDRTOP_mc_param_rd(uint32_t addr, uint32_t offset, uint32_t width)
{
	uint32_t tmp_data;
	uint32_t tmp_mask;
	tmp_data = DDRTOP_mc_apb_rd(addr);
	tmp_mask = ((1 << width) - 1) << offset;
	return (tmp_data & tmp_mask) >> offset;
}

// task automatic DDRTOP_mc_param_poll(
// 		inpu	[31:0]	addr
// 	,	input	int		offset
// 	,	input	int		width
// 	,	input	[31:0]	data
static void DDRTOP_mc_param_poll(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data)
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

static void decode_major_message (uint32_t mail)
{
  switch (mail) {
	case 0x00:
		INFO("PMU Major Msg: End of initialization                                         \n");
		break;
	case 0x01:
		INFO("PMU Major Msg: End of fine write leveling                                    \n");
		break;
	case 0x02:
		INFO("PMU Major Msg: End of read enable training                                   \n");
		break;
	case 0x03:
		INFO("PMU Major Msg: End of read delay center optimization                         \n");
		break;
	case 0x04:
		INFO("PMU Major Msg: End of write delay center optimization                        \n");
		break;
	case 0x05:
		INFO("PMU Major Msg: End of 2D read delay/voltage center optimization              \n");
		break;
	case 0x06:
		INFO("PMU Major Msg: End of 2D write delay /voltage center optimization            \n");
		break;
	case 0x07:
		INFO("PMU Major Msg: Firmware run has completed                                    \n");
		break;
	case 0x08:
    	INFO("PMU Major Msg: Enter streaming message mode                                  \n");
    	break;
	case 0x09:
		INFO("PMU Major Msg: End of max read latency training                              \n");
		break;
	case 0x0a:
		INFO("PMU Major Msg: End of read dq deskew training                                \n");
		break;
	case 0x0b:
		INFO("PMU Major Msg: End of LCDL offset calibration                                \n");
		break;
	case 0x0c:
		INFO("PMU Major Msg: End of LRDIMM Specific training (DWL, MREP, MRD and MWD)      \n");
		break;
	case 0x0d:
		INFO("PMU Major Msg: End of CA training                                            \n");
		break;
	case 0xfd:
		INFO("PMU Major Msg: End of MPR read delay center optimization                     \n");
		break;
	case 0xfe:
		INFO("PMU Major Msg: End of Write leveling coarse delay                            \n");
		break;
	case 0xff:
		INFO("PMU Major Msg: FATAL ERROR.                                                  \n");
		break;
	default:
		INFO("PMU Major Msg: Un-recognized message... !\n");
  }
}

static void decode_streaming_message_ddr4 (uint32_t codede_message_hex, uint16_t *args_list);
static void decode_streaming_message_lpddr4 (uint32_t codede_message_hex, uint16_t *args_list);

static void decode_streaming_message (uint8_t sel_train)
{
	uint32_t codede_message_hex;
	uint16_t num_args;
	uint16_t args_list[31];
	int       i;

	codede_message_hex = get_mail(1);
	// Get the number of argument need to be read from mailbox
	num_args = 0xFFFF & codede_message_hex;
	for (i = 0; i < num_args; i++) {
    	args_list[i] = get_mail(1);
	}

    INFO("Encode Message (0x%x)\n", codede_message_hex);
	for (i = 0; i < num_args; i++) {
        INFO("num_args %d: 0x%x\n", i, args_list[i]);
	}

	switch(sel_train) {
	case 1: //ddr4
		decode_streaming_message_ddr4(codede_message_hex, args_list);
		break;
	case 3: //lpddr4
		decode_streaming_message_lpddr4(codede_message_hex, args_list);
		break;
	default:
		panic();
		break;
	}
}

static void decode_streaming_message_ddr4 (uint32_t codede_message_hex, uint16_t *args_list)
{
#if 0
	switch (codede_message_hex) {
	case 0x00000001:
		INFO("PMU1:prbsGenCtl:%x\n", args_list[0]);
		break;
	case 0x00010000:
		INFO("PMU1: loading 2D acsm sequence\n");
		break;
	case 0x00020000:
		INFO("PMU1: loading 1D acsm sequence\n");
		break;
	case 0x00030002:
		INFO("PMU3: %d memclocks @ %d to get half of 300ns\n", args_list[0], args_list[1]);
		break;
	case 0x00040000:
		INFO("PMU: Error: User requested MPR read pattern for read DQS training in DDR3 Mode\n");
		break;
	case 0x00050000:
		INFO("PMU3: Running 1D search for left eye edge\n");
		break;
	case 0x00060001:
		INFO("PMU1: In Phase Left Edge Search cs %d\n", args_list[0]);
		break;
	case 0x00070001:
		INFO("PMU1: Out of Phase Left Edge Search cs %d\n", args_list[0]);
		break;
	case 0x00080000:
		INFO("PMU3: Running 1D search for right eye edge\n");
		break;
	case 0x00090001:
		INFO("PMU1: In Phase Right Edge Search cs %d\n", args_list[0]);
		break;
	case 0x000a0001:
		INFO("PMU1: Out of Phase Right Edge Search cs %d\n", args_list[0]);
		break;
	case 0x000b0001:
		INFO("PMU1: mxRdLat training pstate %d\n", args_list[0]);
		break;
	case 0x000c0001:
		INFO("PMU1: mxRdLat search for cs %d\n", args_list[0]);
		break;
	case 0x000d0001:
		INFO("PMU0: MaxRdLat non consistant DtsmLoThldXingInd 0x%03x\n", args_list[0]);
		break;
	case 0x000e0003:
		INFO("PMU4: CS %d Dbyte %d worked with DFIMRL = %d DFICLKs \n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x000f0004:
		INFO("PMU3: MaxRdLat Read Lane err mask for csn %d, DFIMRL %2d DFIClks, dbyte %d = 0x%03x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00100003:
		INFO("PMU3: MaxRdLat Read Lane err mask for csn %d DFIMRL %2d, All dbytes = 0x%03x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00110001:
		INFO("PMU: Error: CS%d failed to find a DFIMRL setting that worked for all bytes during MaxRdLat training\n", args_list[0]);
		break;
	case 0x00120002:
		INFO("PMU3: Smallest passing DFIMRL for all dbytes in CS%d = %d DFIClks\n", args_list[0], args_list[1]);
		break;
	case 0x00130000:
		INFO("PMU: Error: No passing DFIMRL value found for any chip select during MaxRdLat training\n");
		break;
	case 0x00140003:
		INFO("PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00150006:
		INFO("PMU4: Adjusting rxclkdly db %d nib %d from %d+%d=%d->%d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x00160000:
		INFO("PMU4: TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n");
		break;
	case 0x00170005:
		INFO("PMU4: DB %d Lane %d: %3d %3d -> %3d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00180002:
		INFO("PMU2: TXDQ delayLeft[%2d] = %3d (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x00190004:
		INFO("PMU2: TXDQ delayLeft[%2d] = %3d oopScaled = %3d selectOop %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x001a0002:
		INFO("PMU2: TXDQ delayRight[%2d] = %3d (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x001b0004:
		INFO("PMU2: TXDQ delayRight[%2d] = %3d oopScaled = %3d selectOop %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x001c0003:
		INFO("PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x001d0000:
		INFO("PMU4: TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n");
		break;
	case 0x001e0002:
		INFO("PMU4: DB %d Lane %d: (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x001f0005:
		INFO("PMU4: DB %d Lane %d: %3d %3d -> %3d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00200002:
		INFO("PMU3: Running 1D search csn %d for DM Right/NotLeft(%d) eye edge\n", args_list[0], args_list[1]);
		break;
	case 0x00210002:
		INFO("PMU3: WrDq DM byte%2d with Errcnt %d\n", args_list[0], args_list[1]);
		break;
	case 0x00220002:
		INFO("PMU3: WrDq DM byte%2d avgDly 0x%04x\n", args_list[0], args_list[1]);
		break;
	case 0x00230002:
		INFO("PMU1: WrDq DM byte%2d with Errcnt %d\n", args_list[0], args_list[1]);
		break;
	case 0x00240001:
		INFO("PMU: Error: Dbyte %d txDqDly DM training did not start inside the eye\n", args_list[0]);
		break;
	case 0x00250000:
		INFO("PMU4: DM TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n");
		break;
	case 0x00260002:
		INFO("PMU4: DB %d Lane %d: (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x00270005:
		INFO("PMU4: DB %d Lane %d: %3d %3d -> %3d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00280003:
		INFO("PMU: Error: Dbyte %d lane %d txDqDly DM passing region is too small (width = %d)\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00290004:
		INFO("PMU3: Errcnt for MRD/MWD search nib %2d delay = (%d, 0x%02x) = %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x002a0000:
		INFO("PMU3: Precharge all open banks\n");
		break;
	case 0x002b0002:
		INFO("PMU: Error: Dbyte %d nibble %d found multiple working coarse delay setting for MRD/MWD\n", args_list[0], args_list[1]);
		break;
	case 0x002c0000:
		INFO("PMU4: MRD Passing Regions (coarseVal, fineLeft fineRight -> fineCenter)\n");
		break;
	case 0x002d0000:
		INFO("PMU4: MWD Passing Regions (coarseVal, fineLeft fineRight -> fineCenter)\n");
		break;
	case 0x002e0004:
		INFO("PMU10: Warning: DB %d nibble %d has multiple working coarse positions, %d and %d, choosing the smaller delay\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x002f0003:
		INFO("PMU: Error: Dbyte %d nibble %d MRD/MWD passing region is too small (width = %d)\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00300006:
		INFO("PMU4: DB %d nibble %d: %3d, %3d %3d -> %3d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x00310002:
		INFO("PMU1: Start MRD/nMWD %d for csn %d\n", args_list[0], args_list[1]);
		break;
	case 0x00320002:
		INFO("PMU2: RXDQS delayLeft[%2d] = %3d (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x00330006:
		INFO("PMU2: RXDQS delayLeft[%2d] = %3d delayOop[%2d] = %3d OopScaled %4d, selectOop %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x00340002:
		INFO("PMU2: RXDQS delayRight[%2d] = %3d (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x00350006:
		INFO("PMU2: RXDQS delayRight[%2d] = %3d delayOop[%2d] = %4d OopScaled %4d, selectOop %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x00360000:
		INFO("PMU4: RxClkDly Passing Regions (EyeLeft EyeRight -> EyeCenter)\n");
		break;
	case 0x00370002:
		INFO("PMU4: DB %d nibble %d: (DISCONNECTED)\n", args_list[0], args_list[1]);
		break;
	case 0x00380005:
		INFO("PMU4: DB %d nibble %d: %3d %3d -> %3d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00390003:
		INFO("PMU: Error: Dbyte %d nibble %d rxClkDly passing region is too small (width = %d)\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x003a0002:
		INFO("PMU0: goodbar = %d for RDWR_BLEN %d\n", args_list[0], args_list[1]);
		break;
	case 0x003b0001:
		INFO("PMU3: RxClkDly = %d\n", args_list[0]);
		break;
	case 0x003c0005:
		INFO("PMU0: db %d l %d absLane %d -> bottom %d top %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x003d0009:
		INFO("PMU3: BYTE %d - %3d %3d %3d %3d %3d %3d %3d %3d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8]);
		break;
	case 0x003e0002:
		INFO("PMU: Error: dbyte %d lane %d's per-lane vrefDAC's had no passing region\n", args_list[0], args_list[1]);
		break;
	case 0x003f0004:
		INFO("PMU0: db%d l%d - %d %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00400002:
		INFO("PMU0: goodbar = %d for RDWR_BLEN %d\n", args_list[0], args_list[1]);
		break;
	case 0x00410004:
		INFO("PMU3: db%d l%d saw %d issues at rxClkDly %d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00420003:
		INFO("PMU3: db%d l%d first saw a pass->fail edge at rxClkDly %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00430002:
		INFO("PMU3: lane %d PBD = %d\n", args_list[0], args_list[1]);
		break;
	case 0x00440003:
		INFO("PMU3: db%d l%d first saw a DBI pass->fail edge at rxClkDly %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00450003:
		INFO("PMU2: db%d l%d already passed rxPBD = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00460003:
		INFO("PMU0: db%d l%d, PBD = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00470002:
		INFO("PMU: Error: dbyte %d lane %d failed read deskew\n", args_list[0], args_list[1]);
		break;
	case 0x00480003:
		INFO("PMU0: db%d l%d, inc PBD = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00490003:
		INFO("PMU1: Running lane deskew on pstate %d csn %d rdDBIEn %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x004a0000:
		INFO("PMU: Error: Read deskew training has been requested, but csrMajorModeDbyte[2] is set\n");
		break;
	case 0x004b0002:
		INFO("PMU1: AcsmCsMapCtrl%02d 0x%04x\n", args_list[0], args_list[1]);
		break;
	case 0x004c0002:
		INFO("PMU1: AcsmCsMapCtrl%02d 0x%04x\n", args_list[0], args_list[1]);
		break;
	case 0x004d0001:
		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D3U Type\n", args_list[0]);
		break;
	case 0x004e0001:
		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D3R Type\n", args_list[0]);
		break;
	case 0x004f0001:
		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4U Type\n", args_list[0]);
		break;
	case 0x00500001:
		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4R Type\n", args_list[0]);
		break;
	case 0x00510001:
		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4LR Type\n", args_list[0]);
		break;
	case 0x00520000:
		INFO("PMU: Error: Both 2t timing mode and ddr4 geardown mode specifed in the messageblock's PhyCfg and MR3 fields. Only one can be enabled\n");
		break;
	case 0x00530003:
		INFO("PMU10: PHY TOTALS - NUM_DBYTES %d NUM_NIBBLES %d NUM_ANIBS %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00540006:
		INFO("PMU10: CSA=0x%02X, CSB=0x%02X, TSTAGES=0x%04X, HDTOUT=%d, MMISC=%d DRAMFreq=%dMT DramType=LPDDR3\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x00550006:
		INFO("PMU10: CSA=0x%02X, CSB=0x%02X, TSTAGES=0x%04X, HDTOUT=%d, MMISC=%d DRAMFreq=%dMT DramType=LPDDR4\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5]);
		break;
	case 0x00560008:
		INFO("PMU10: CS=0x%02X, TSTAGES=0x%04X, HDTOUT=%d, 2T=%d, MMISC=%d AddrMirror=%d DRAMFreq=%dMT DramType=%d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7]);
		break;
	case 0x00570004:
		INFO("PMU10: Pstate%d MR0=0x%04X MR1=0x%04X MR2=0x%04X\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00580008:
		INFO("PMU10: Pstate%d MRS MR0=0x%04X MR1=0x%04X MR2=0x%04X MR3=0x%04X MR4=0x%04X MR5=0x%04X MR6=0x%04X\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7]);
		break;
	case 0x00590005:
		INFO("PMU10: Pstate%d MRS MR1_A0=0x%04X MR2_A0=0x%04X MR3_A0=0x%04X MR11_A0=0x%04X\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x005a0000:
		INFO("PMU10: UseBroadcastMR set. All ranks and channels use MRXX_A0 for MR settings.\n");
		break;
	case 0x005b0005:
		INFO("PMU10: Pstate%d MRS MR01_A0=0x%02X MR02_A0=0x%02X MR03_A0=0x%02X MR11_A0=0x%02X\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x005c0005:
		INFO("PMU10: Pstate%d MRS MR12_A0=0x%02X MR13_A0=0x%02X MR14_A0=0x%02X MR22_A0=0x%02X\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x005d0005:
		INFO("PMU10: Pstate%d MRS MR01_A1=0x%02X MR02_A1=0x%02X MR03_A1=0x%02X MR11_A1=0x%02X\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x005e0005:
		INFO("PMU10: Pstate%d MRS MR12_A1=0x%02X MR13_A1=0x%02X MR14_A1=0x%02X MR22_A1=0x%02X\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x005f0005:
		INFO("PMU10: Pstate%d MRS MR01_B0=0x%02X MR02_B0=0x%02X MR03_B0=0x%02X MR11_B0=0x%02X\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00600005:
		INFO("PMU10: Pstate%d MRS MR12_B0=0x%02X MR13_B0=0x%02X MR14_B0=0x%02X MR22_B0=0x%02X\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00610005:
		INFO("PMU10: Pstate%d MRS MR01_B1=0x%02X MR02_B1=0x%02X MR03_B1=0x%02X MR11_B1=0x%02X\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00620005:
		INFO("PMU10: Pstate%d MRS MR12_B1=0x%02X MR13_B1=0x%02X MR14_B1=0x%02X MR22_B1=0x%02X\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00630002:
		INFO("PMU1: AcsmOdtCtrl%02d 0x%02x\n", args_list[0], args_list[1]);
		break;
	case 0x00640002:
		INFO("PMU1: AcsmCsMapCtrl%02d 0x%04x\n", args_list[0], args_list[1]);
		break;
	case 0x00650002:
		INFO("PMU1: AcsmCsMapCtrl%02d 0x%04x\n", args_list[0], args_list[1]);
		break;
	case 0x00660000:
		INFO("PMU1: HwtCAMode set\n");
		break;
	case 0x00670001:
		INFO("PMU3: DDR4 infinite preamble enter/exit mode %d\n", args_list[0]);
		break;
	case 0x00680002:
		INFO("PMU1: In rxenb_train() csn=%d pstate=%d\n", args_list[0], args_list[1]);
		break;
	case 0x00690000:
		INFO("PMU3: Finding DQS falling edge\n");
		break;
	case 0x006a0000:
		INFO("PMU3: Searching for DDR3/LPDDR3/LPDDR4 read preamble\n");
		break;
	case 0x006b0009:
		INFO("PMU3: dtsm fails Even Nibbles : %2x %2x %2x %2x %2x %2x %2x %2x %2x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8]);
		break;
	case 0x006c0009:
		INFO("PMU3: dtsm fails Odd  Nibbles : %2x %2x %2x %2x %2x %2x %2x %2x %2x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8]);
		break;
	case 0x006d0002:
		INFO("PMU3: Preamble search pass=%d anyfail=%d\n", args_list[0], args_list[1]);
		break;
	case 0x006e0000:
		INFO("PMU: Error: RxEn training preamble not found\n");
		break;
	case 0x006f0000:
		INFO("PMU3: Found DQS pre-amble\n");
		break;
	case 0x00700001:
		INFO("PMU: Error: Dbyte %d couldn't find the rising edge of DQS during RxEn Training\n", args_list[0]);
		break;
	case 0x00710000:
		INFO("PMU3: RxEn aligning to first rising edge of burst\n");
		break;
	case 0x00720001:
		INFO("PMU3: Decreasing RxEn delay by %d fine step to allow full capture of reads\n", args_list[0]);
		break;
	case 0x00730001:
		INFO("PMU3: MREP Delay = %d\n", args_list[0]);
		break;
	case 0x00740003:
		INFO("PMU3: Errcnt for MREP nib %2d delay = %2d is %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00750002:
		INFO("PMU3: MREP nibble %d sampled a 1 at data buffer delay %d\n", args_list[0], args_list[1]);
		break;
	case 0x00760002:
		INFO("PMU3: MREP nibble %d saw a 0 to 1 transition at data buffer delay %d\n", args_list[0], args_list[1]);
		break;
	case 0x00770000:
		INFO("PMU2:  MREP did not find a 0 to 1 transition for all nibbles. Failing nibbles assumed to have rising edge close to fine delay 63\n");
		break;
	case 0x00780002:
		INFO("PMU2:  Rising edge found in alias window, setting rxDly for nibble %d = %d\n", args_list[0], args_list[1]);
		break;
	case 0x00790002:
		INFO("PMU: Error: Failed MREP for nib %d with %d one\n", args_list[0], args_list[1]);
		break;
	case 0x007a0003:
		INFO("PMU2:  Rising edge not found in alias window with %d one, leaving rxDly for nibble %d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x007b0002:
		INFO("PMU3: Training DIMM %d CSn %d\n", args_list[0], args_list[1]);
		break;
	case 0x007c0001:
		INFO("PMU3: exitCAtrain_lp3 cs 0x%x\n", args_list[0]);
		break;
	case 0x007d0001:
		INFO("PMU3: enterCAtrain_lp3 cs 0x%x\n", args_list[0]);
		break;
	case 0x007e0001:
		INFO("PMU3: CAtrain_switchmsb_lp3 cs 0x%x\n", args_list[0]);
		break;
	case 0x007f0001:
		INFO("PMU3: CATrain_rdwr_lp3 looking for pattern %x\n", args_list[0]);
		break;
	case 0x00800000:
		INFO("PMU3: exitCAtrain_lp4\n");
		break;
	case 0x00810001:
		INFO("PMU3: DEBUG enterCAtrain_lp4 1: cs 0x%x\n", args_list[0]);
		break;
	case 0x00820001:
		INFO("PMU3: DEBUG enterCAtrain_lp4 3: Put dbyte %d in async mode\n", args_list[0]);
		break;
	case 0x00830000:
		INFO("PMU3: DEBUG enterCAtrain_lp4 5: Send MR13 to turn on CA training\n");
		break;
	case 0x00840003:
		INFO("PMU3: DEBUG enterCAtrain_lp4 7: idx = %d vref = %x mr12 = %x \n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00850001:
		INFO("PMU3: CATrain_rdwr_lp4 looking for pattern %x\n", args_list[0]);
		break;
	case 0x00860004:
		INFO("PMU3: Phase %d CAreadbackA db:%d %x xo:%x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00870005:
		INFO("PMU3: DEBUG lp4SetCatrVref 1: cs=%d chan=%d mr12=%x vref=%d.%d%%\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00880003:
		INFO("PMU3: DEBUG lp4SetCatrVref 3: mr12 = %x send vref= %x to db=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00890000:
		INFO("PMU10:Optimizing vref\n");
		break;
	case 0x008a0004:
		INFO("PMU4:mr12:%2x cs:%d chan %d r:%4x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x008b0005:
		INFO("PMU3: i:%2d bstr:%2d bsto:%2d st:%d r:%d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x008c0002:
		INFO("Failed to find sufficient CA Vref Passing Region for CS %d channel %d\n", args_list[0], args_list[1]);
		break;
	case 0x008d0005:
		INFO("PMU3:Found %d.%d%% MR12:%x for cs:%d chan %d\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x008e0002:
		INFO("PMU3:Calculated %d for AtxImpedence from acx %d.\n", args_list[0], args_list[1]);
		break;
	case 0x008f0000:
		INFO("PMU3:CA Odt impedence ==0.  Use default vref.\n");
		break;
	case 0x00900003:
		INFO("PMU3:Calculated %d.%d%% for Vref MR12=0x%x.\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00910000:
		INFO("PMU3: CAtrain_lp\n");
		break;
	case 0x00920000:
		INFO("PMU3: CAtrain Begins.\n");
		break;
	case 0x00930001:
		INFO("PMU3: CAtrain_lp testing dly %d\n", args_list[0]);
		break;
	case 0x00940001:
		INFO("PMU5: CA bitmap dump for cs %x\n", args_list[0]);
		break;
	case 0x00950001:
		INFO("PMU5: CAA%d ", args_list[0]);
		break;
	case 0x00960001:
		INFO("%02x", args_list[0]);
		break;
	case 0x00970000:
		INFO("\n");
		break;
	case 0x00980001:
		INFO("PMU5: CAB%d ", args_list[0]);
		break;
	case 0x00990001:
		INFO("%02x", args_list[0]);
		break;
	case 0x009a0000:
		INFO("\n");
		break;
	case 0x009b0003:
		INFO("PMU3: anibi=%d, anibichan[anibi]=%d ,chan=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x009c0001:
		INFO("%02x", args_list[0]);
		break;
	case 0x009d0001:
		INFO("\nPMU3:Raw CA setting :%x", args_list[0]);
		break;
	case 0x009e0002:
		INFO("\nPMU3:ATxDly setting:%x margin:%d\n", args_list[0], args_list[1]);
		break;
	case 0x009f0002:
		INFO("\nPMU3:InvClk ATxDly setting:%x margin:%d\n", args_list[0], args_list[1]);
		break;
	case 0x00a00000:
		INFO("\nPMU3:No Range found!\n");
		break;
	case 0x00a10003:
		INFO("PMU3: 2 anibi=%d, anibichan[anibi]=%d ,chan=%d", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00a20002:
		INFO("\nPMU3: no neg clock => CA setting anib=%d, :%d\n", args_list[0], args_list[1]);
		break;
	case 0x00a30001:
		INFO("PMU3:Normal margin:%d\n", args_list[0]);
		break;
	case 0x00a40001:
		INFO("PMU3:Inverted margin:%d\n", args_list[0]);
		break;
	case 0x00a50000:
		INFO("PMU3:Using Inverted clock\n");
		break;
	case 0x00a60000:
		INFO("PMU3:Using normal clk\n");
		break;
	case 0x00a70003:
		INFO("PMU3: 3 anibi=%d, anibichan[anibi]=%d ,chan=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00a80002:
		INFO("PMU3: Setting ATxDly for anib %x to %x\n", args_list[0], args_list[1]);
		break;
	case 0x00a90000:
		INFO("PMU: Error: CA Training Failed.\n");
		break;
	case 0x00aa0000:
		INFO("PMU1: Writing MRs\n");
		break;
	case 0x00ab0000:
		INFO("PMU4:Using MR12 values from 1D CA VREF training.\n");
		break;
	case 0x00ac0000:
		INFO("PMU3:Writing all MRs to fsp 1\n");
		break;
	case 0x00ad0000:
		INFO("PMU10:Lp4Quickboot mode.\n");
		break;
	case 0x00ae0000:
		INFO("PMU3: Writing MRs\n");
		break;
	case 0x00af0001:
		INFO("PMU10: Setting boot clock divider to %d\n", args_list[0]);
		break;
	case 0x00b00000:
		INFO("PMU3: Resetting DRAM\n");
		break;
	case 0x00b10000:
		INFO("PMU3: setup for RCD initalization\n");
		break;
	case 0x00b20000:
		INFO("PMU3: pmu_exit_SR from dev_init()\n");
		break;
	case 0x00b30000:
		INFO("PMU3: initializing RCD\n");
		break;
	case 0x00b40000:
		INFO("PMU10: **** Executing 2D Image ****\n");
		break;
	case 0x00b50001:
		INFO("PMU10: **** Start DDR4 Training. PMU Firmware Revision 0x%04x ****\n", args_list[0]);
		break;
	case 0x00b60001:
		INFO("PMU10: **** Start DDR3 Training. PMU Firmware Revision 0x%04x ****\n", args_list[0]);
		break;
	case 0x00b70001:
		INFO("PMU10: **** Start LPDDR3 Training. PMU Firmware Revision 0x%04x ****\n", args_list[0]);
		break;
	case 0x00b80001:
		INFO("PMU10: **** Start LPDDR4 Training. PMU Firmware Revision 0x%04x ****\n", args_list[0]);
		break;
	case 0x00b90000:
		INFO("PMU: Error: Mismatched internal revision between DCCM and ICCM images\n");
		break;
	case 0x00ba0001:
		INFO("PMU10: **** Testchip %d Specific Firmware ****\n", args_list[0]);
		break;
	case 0x00bb0000:
		INFO("PMU1: LRDIMM with EncodedCS mode, one DIMM\n");
		break;
	case 0x00bc0000:
		INFO("PMU1: LRDIMM with EncodedCS mode, two DIMMs\n");
		break;
	case 0x00bd0000:
		INFO("PMU1: RDIMM with EncodedCS mode, one DIMM\n");
		break;
	case 0x00be0000:
		INFO("PMU2: Starting LRDIMM MREP training for all ranks\n");
		break;
	case 0x00bf0000:
		INFO("PMU199: LRDIMM MREP training for all ranks completed\n");
		break;
	case 0x00c00000:
		INFO("PMU2: Starting LRDIMM DWL training for all ranks\n");
		break;
	case 0x00c10000:
		INFO("PMU199: LRDIMM DWL training for all ranks completed\n");
		break;
	case 0x00c20000:
		INFO("PMU2: Starting LRDIMM MRD training for all ranks\n");
		break;
	case 0x00c30000:
		INFO("PMU199: LRDIMM MRD training for all ranks completed\n");
		break;
	case 0x00c40000:
		INFO("PMU2: Starting RXEN training for all ranks\n");
		break;
	case 0x00c50000:
		INFO("PMU2: Starting write leveling fine delay training for all ranks\n");
		break;
	case 0x00c60000:
		INFO("PMU2: Starting LRDIMM MWD training for all ranks\n");
		break;
	case 0x00c70000:
		INFO("PMU199: LRDIMM MWD training for all ranks completed\n");
		break;
	case 0x00c80000:
		INFO("PMU2: Starting write leveling fine delay training for all ranks\n");
		break;
	case 0x00c90000:
		INFO("PMU2: Starting read deskew training\n");
		break;
	case 0x00ca0000:
		INFO("PMU2: Starting SI friendly 1d RdDqs training for all ranks\n");
		break;
	case 0x00cb0000:
		INFO("PMU2: Starting write leveling coarse delay training for all ranks\n");
		break;
	case 0x00cc0000:
		INFO("PMU2: Starting 1d WrDq training for all ranks\n");
		break;
	case 0x00cd0000:
		INFO("PMU2: Running DQS2DQ Oscillator for all ranks\n");
		break;
	case 0x00ce0000:
		INFO("PMU2: Starting again read deskew training but with PRBS\n");
		break;
	case 0x00cf0000:
		INFO("PMU2: Starting 1d RdDqs training for all ranks\n");
		break;
	case 0x00d00000:
		INFO("PMU2: Starting again 1d WrDq training for all ranks\n");
		break;
	case 0x00d10000:
		INFO("PMU2: Starting MaxRdLat training\n");
		break;
	case 0x00d20000:
		INFO("PMU2: Starting 2d RdDqs training for all ranks\n");
		break;
	case 0x00d30000:
		INFO("PMU2: Starting 2d WrDq training for all ranks\n");
		break;
	case 0x00d40000:
		INFO("PMU2: Starting 2d RdDqs training for all ranks\n");
		break;
	case 0x00d50002:
		INFO("PMU3:read_fifo %x %x\n", args_list[0], args_list[1]);
		break;
	case 0x00d60001:
		INFO("PMU: Error: Invalid PhyDrvImpedance of 0x%x specified in message block.\n", args_list[0]);
		break;
	case 0x00d70001:
		INFO("PMU: Error: Invalid PhyOdtImpedance of 0x%x specified in message block.\n", args_list[0]);
		break;
	case 0x00d80001:
		INFO("PMU: Error: Invalid BPZNResVal of 0x%x specified in message block.\n", args_list[0]);
		break;
	case 0x00d90005:
		INFO("PMU3: fixRxEnBackOff csn:%d db:%d dn:%d bo:%d dly:%x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00da0001:
		INFO("PMU3: fixRxEnBackOff dly:%x\n", args_list[0]);
		break;
	case 0x00db0000:
		INFO("PMU3: Entering setupPpt\n");
		break;
	case 0x00dc0000:
		INFO("PMU3: Start lp4PopulateHighLowBytes\n");
		break;
	case 0x00dd0002:
		INFO("PMU3:Dbyte Detect: db%d received %x\n", args_list[0], args_list[1]);
		break;
	case 0x00de0002:
		INFO("PMU3:getDqs2Dq read %x from dbyte %d\n", args_list[0], args_list[1]);
		break;
	case 0x00df0002:
		INFO("PMU3:getDqs2Dq(2) read %x from dbyte %d\n", args_list[0], args_list[1]);
		break;
	case 0x00e00001:
		INFO("PMU: Error: Dbyte %d read 0 from the DQS oscillator it is connected to\n", args_list[0]);
		break;
	case 0x00e10002:
		INFO("PMU4: Dbyte %d dqs2dq = %d/32 UI\n", args_list[0], args_list[1]);
		break;
	case 0x00e20003:
		INFO("PMU3:getDqs2Dq set dqs2dq:%d/32 ui (%d ps) from dbyte %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00e30003:
		INFO("PMU3: Setting coarse delay in AtxDly chiplet %d from 0x%02x to 0x%02x \n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00e40003:
		INFO("PMU3: Clearing coarse delay in AtxDly chiplet %d from 0x%02x to 0x%02x \n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00e50000:
		INFO("PMU3: Performing DDR4 geardown sync sequence\n");
		break;
	case 0x00e60000:
		INFO("PMU1: Enter self refresh\n");
		break;
	case 0x00e70000:
		INFO("PMU1: Exit self refresh\n");
		break;
	case 0x00e80000:
		INFO("PMU: Error: No dbiEnable with lp4\n");
		break;
	case 0x00e90000:
		INFO("PMU: Error: No dbiDisable with lp4\n");
		break;
	case 0x00ea0001:
		INFO("PMU1: DDR4 update Rx DBI Setting disable %d\n", args_list[0]);
		break;
	case 0x00eb0001:
		INFO("PMU1: DDR4 update 2nCk WPre Setting disable %d\n", args_list[0]);
		break;
	case 0x00ec0005:
		INFO("PMU1: read_delay: db%d lane%d delays[%2d] = 0x%02x (max 0x%02x)\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x00ed0004:
		INFO("PMU1: write_delay: db%d lane%d delays[%2d] = 0x%04x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x00ee0001:
		INFO("PMU5: ID=%d -- db0  db1  db2  db3  db4  db5  db6  db7  db8  db9 --\n", args_list[0]);
		break;
	case 0x00ef000b:
		INFO("PMU5: [%d]:0x %4x %4x %4x %4x %4x %4x %4x %4x %4x %4x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8], args_list[9], args_list[10]);
		break;
	case 0x00f00003:
		INFO("PMU2: dump delays - pstate=%d dimm=%d csn=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00f10000:
		INFO("PMU3: Printing Mid-Training Delay Information\n");
		break;
	case 0x00f20001:
		INFO("PMU5: CS%d <<KEY>> 0 TrainingCntr <<KEY>> coarse(15:10) fine(9:0)\n", args_list[0]);
		break;
	case 0x00f30001:
		INFO("PMU5: CS%d <<KEY>> 0 RxEnDly, 1 RxClkDly <<KEY>> coarse(10:6) fine(5:0)\n", args_list[0]);
		break;
	case 0x00f40001:
		INFO("PMU5: CS%d <<KEY>> 0 TxDqsDly, 1 TxDqDly <<KEY>> coarse(9:6) fine(5:0)\n", args_list[0]);
		break;
	case 0x00f50001:
		INFO("PMU5: CS%d <<KEY>> 0 RxPBDly <<KEY>> 1 Delay Unit ~= 7ps \n", args_list[0]);
		break;
	case 0x00f60000:
		INFO("PMU5: all CS <<KEY>> 0 DFIMRL <<KEY>> Units = DFI clocks\n");
		break;
	case 0x00f70000:
		INFO("PMU5: all CS <<KEY>> VrefDACs <<KEY>> DAC(6:0)\n");
		break;
	case 0x00f80000:
		INFO("PMU1: Set DMD in MR13 and wrDBI in MR3 for training\n");
		break;
	case 0x00f90000:
		INFO("PMU: Error: getMaxRxen() failed to find largest rxen nibble delay\n");
		break;
	case 0x00fa0003:
		INFO("PMU2: getMaxRxen(): maxDly %d maxTg %d maxNib %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00fb0003:
		INFO("PMU2: getRankMaxRxen(): maxDly %d Tg %d maxNib %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00fc0000:
		INFO("PMU1: skipping CDD calculation in 2D image\n");
		break;
	case 0x00fd0001:
		INFO("PMU3: Calculating CDDs for pstate %d\n", args_list[0]);
		break;
	case 0x00fe0003:
		INFO("PMU3: rxFromDly[%d][%d] = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x00ff0003:
		INFO("PMU3: rxToDly  [%d][%d] = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01000003:
		INFO("PMU3: rxDly	[%d][%d] = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01010003:
		INFO("PMU3: txDly	[%d][%d] = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01020003:
		INFO("PMU3: allFine CDD_RR_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01030003:
		INFO("PMU3: allFine CDD_WW_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01040003:
		INFO("PMU3: CDD_RR_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01050003:
		INFO("PMU3: CDD_WW_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01060003:
		INFO("PMU3: allFine CDD_RW_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01070003:
		INFO("PMU3: allFine CDD_WR_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01080003:
		INFO("PMU3: CDD_RW_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01090003:
		INFO("PMU3: CDD_WR_%d_%d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x010a0004:
		INFO("PMU3: F%dBC2x_B%d_D%d = 0x%02x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x010b0004:
		INFO("PMU3: F%dBC3x_B%d_D%d = 0x%02x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x010c0004:
		INFO("PMU3: F%dBC4x_B%d_D%d = 0x%02x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x010d0004:
		INFO("PMU3: F%dBC5x_B%d_D%d = 0x%02x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x010e0004:
		INFO("PMU3: F%dBC8x_B%d_D%d = 0x%02x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x010f0004:
		INFO("PMU3: F%dBC9x_B%d_D%d = 0x%02x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01100004:
		INFO("PMU3: F%dBCAx_B%d_D%d = 0x%02x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01110004:
		INFO("PMU3: F%dBCBx_B%d_D%d = 0x%02x\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01120000:
		INFO("PMU10: Entering context_switch_postamble\n");
		break;
	case 0x01130003:
		INFO("PMU10: context_switch_postamble is enabled for DIMM %d, RC0A=0x%x, RC3x=0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01140000:
		INFO("PMU10: Setting bcw fspace 0\n");
		break;
	case 0x01150001:
		INFO("PMU10: Sending BC0A = 0x%x\n", args_list[0]);
		break;
	case 0x01160001:
		INFO("PMU10: Sending BC6x = 0x%x\n", args_list[0]);
		break;
	case 0x01170001:
		INFO("PMU10: Sending RC0A = 0x%x\n", args_list[0]);
		break;
	case 0x01180001:
		INFO("PMU10: Sending RC3x = 0x%x\n", args_list[0]);
		break;
	case 0x01190001:
		INFO("PMU10: Sending RC0A = 0x%x\n", args_list[0]);
		break;
	case 0x011a0001:
		INFO("PMU1: enter_lp3: DEBUG: pstate = %d\n", args_list[0]);
		break;
	case 0x011b0001:
		INFO("PMU1: enter_lp3: DEBUG: dfifreqxlat_pstate = %d\n", args_list[0]);
		break;
	case 0x011c0001:
		INFO("PMU1: enter_lp3: DEBUG: pllbypass = %d\n", args_list[0]);
		break;
	case 0x011d0001:
		INFO("PMU1: enter_lp3: DEBUG: forcecal = %d\n", args_list[0]);
		break;
	case 0x011e0001:
		INFO("PMU1: enter_lp3: DEBUG: pllmaxrange = 0x%x\n", args_list[0]);
		break;
	case 0x011f0001:
		INFO("PMU1: enter_lp3: DEBUG: dacval_out = 0x%x\n", args_list[0]);
		break;
	case 0x01200001:
		INFO("PMU1: enter_lp3: DEBUG: pllctrl3 = 0x%x\n", args_list[0]);
		break;
	case 0x01210000:
		INFO("PMU3: Loading DRAM with BIOS supplied MR values and entering self refresh prior to exiting PMU code.\n");
		break;
	case 0x01220002:
		INFO("PMU3: Setting DataBuffer function space of dimmcs 0x%02x to %d\n", args_list[0], args_list[1]);
		break;
	case 0x01230002:
		INFO("PMU4: Setting RCW FxRC%Xx = 0x%02x\n", args_list[0], args_list[1]);
		break;
	case 0x01240002:
		INFO("PMU4: Setting RCW FxRC%02X = 0x%02x\n", args_list[0], args_list[1]);
		break;
	case 0x01250001:
		INFO("PMU1: DDR4 update Rd Pre Setting disable %d\n", args_list[0]);
		break;
	case 0x01260002:
		INFO("PMU2: Setting BCW FxBC%Xx = 0x%02x\n", args_list[0], args_list[1]);
		break;
	case 0x01270002:
		INFO("PMU2: Setting BCW BC%02X = 0x%02x\n", args_list[0], args_list[1]);
		break;
	case 0x01280002:
		INFO("PMU2: Setting BCW PBA mode FxBC%Xx = 0x%02x\n", args_list[0], args_list[1]);
		break;
	case 0x01290002:
		INFO("PMU2: Setting BCW PBA mode BC%02X = 0x%02x\n", args_list[0], args_list[1]);
		break;
	case 0x012a0003:
		INFO("PMU4: BCW value for dimm %d, fspace %d, addr 0x%04x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x012b0002:
		INFO("PMU4: DB %d, value 0x%02x\n", args_list[0], args_list[1]);
		break;
	case 0x012c0000:
		INFO("PMU6: WARNING MREP underflow, set to min value -2 coarse, 0 fine\n");
		break;
	case 0x012d0004:
		INFO("PMU6: LRDIMM Writing final data buffer fine delay value nib %2d, trainDly %3d, fineDly code %2d, new MREP fine %2d\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x012e0003:
		INFO("PMU6: LRDIMM Writing final data buffer fine delay value nib %2d, trainDly %3d, fineDly code %2d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x012f0003:
		INFO("PMU6: LRDIMM Writing data buffer fine delay type %d nib %2d, code %2d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01300002:
		INFO("PMU6: Writing final data buffer coarse delay value dbyte %2d, coarse = 0x%02x\n", args_list[0], args_list[1]);
		break;
	case 0x01310003:
		INFO("PMU4: data 0x%04x at MB addr 0x%08x saved at CSR addr 0x%08x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01320003:
		INFO("PMU4: data 0x%04x at MB addr 0x%08x restored from CSR addr 0x%08x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01330003:
		INFO("PMU4: data 0x%04x at MB addr 0x%08x saved at CSR addr 0x%08x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01340003:
		INFO("PMU4: data 0x%04x at MB addr 0x%08x restored from CSR addr 0x%08x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01350001:
		INFO("PMU3: Update BC00, BC01, BC02 for rank-dimm 0x%02x\n", args_list[0]);
		break;
	case 0x01360000:
		INFO("PMU3: Writing D4 RDIMM RCD Control words F0RC00 -> F0RC0F\n");
		break;
	case 0x01370000:
		INFO("PMU3: Disable parity in F0RC0E\n");
		break;
	case 0x01380000:
		INFO("PMU3: Writing D4 RDIMM RCD Control words F1RC00 -> F1RC05\n");
		break;
	case 0x01390000:
		INFO("PMU3: Writing D4 RDIMM RCD Control words F1RC1x -> F1RC9x\n");
		break;
	case 0x013a0000:
		INFO("PMU3: Writing D4 Data buffer Control words BC00 -> BC0E\n");
		break;
	case 0x013b0002:
		INFO("PMU1: setAltCL Sending MR0 0x%x cl=%d\n", args_list[0], args_list[1]);
		break;
	case 0x013c0002:
		INFO("PMU1: restoreFromAltCL Sending MR0 0x%x cl=%d\n", args_list[0], args_list[1]);
		break;
	case 0x013d0002:
		INFO("PMU1: restoreAcsmFromAltCL Sending MR0 0x%x cl=%d\n", args_list[0], args_list[1]);
		break;
	case 0x013e0002:
		INFO("PMU2: Setting D3R RC%d = 0x%01x\n", args_list[0], args_list[1]);
		break;
	case 0x013f0000:
		INFO("PMU3: Writing D3 RDIMM RCD Control words RC0 -> RC11\n");
		break;
	case 0x01400002:
		INFO("PMU0: VrefDAC0/1 vddqStart %d dacToVddq %d\n", args_list[0], args_list[1]);
		break;
	case 0x01410001:
		INFO("PMU: Error: Messageblock phyVref=0x%x is above the limit for TSMC28's attenuated LPDDR4 receivers. Please see the pub databook\n", args_list[0]);
		break;
	case 0x01420001:
		INFO("PMU: Error: Messageblock phyVref=0x%x is above the limit for TSMC28's attenuated DDR4 receivers. Please see the pub databook\n", args_list[0]);
		break;
	case 0x01430001:
		INFO("PMU0: PHY VREF @ (%d/1000) VDDQ\n", args_list[0]);
		break;
	case 0x01440002:
		INFO("PMU0: initalizing phy vrefDacs to %d ExtVrefRange %x\n", args_list[0], args_list[1]);
		break;
	case 0x01450002:
		INFO("PMU0: initalizing global vref to %d range %d\n", args_list[0], args_list[1]);
		break;
	case 0x01460002:
		INFO("PMU4: Setting initial device vrefDQ for CS%d to MR6 = 0x%04x\n", args_list[0], args_list[1]);
		break;
	case 0x01470003:
		INFO("PMU1: In write_level_fine() csn=%d dimm=%d pstate=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01480000:
		INFO("PMU3: Fine write leveling hardware search increasing TxDqsDly until full bursts are seen\n");
		break;
	case 0x01490000:
		INFO("PMU4: WL normalized pos   : ........................|........................\n");
		break;
	case 0x014a0007:
		INFO("PMU4: WL margin for nib %2d: %08x%08x%08x%08x%08x%08x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6]);
		break;
	case 0x014b0000:
		INFO("PMU4: WL normalized pos   : ........................|........................\n");
		break;
	case 0x014c0000:
		INFO("PMU3: Exiting write leveling mode\n");
		break;
	case 0x014d0001:
		INFO("PMU3: got %d for cl in load_wrlvl_acsm\n", args_list[0]);
		break;
	case 0x014e0003:
		INFO("PMU1: In write_level_coarse() csn=%d dimm=%d pstate=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x014f0003:
		INFO("PMU3: left eye edge search db:%d ln:%d dly:0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01500003:
		INFO("PMU3: right eye edge search db:%d ln:%d dly:0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01510004:
		INFO("PMU3: eye center db:%d ln:%d dly:0x%x (maxdq:%x)\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x01520003:
		INFO("PMU3: Wrote to TxDqDly db:%d ln:%d dly:0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01530003:
		INFO("PMU3: Wrote to TxDqDly db:%d ln:%d dly:0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01540002:
		INFO("PMU3: Coarse write leveling dbyte%2d is still failing for TxDqsDly=0x%04x\n", args_list[0], args_list[1]);
		break;
	case 0x01550002:
		INFO("PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n", args_list[0], args_list[1]);
		break;
	case 0x01560000:
		INFO("PMU: Error: Failed write leveling coarse\n");
		break;
	case 0x01570001:
		INFO("PMU3: got %d for cl in load_wrlvl_acsm\n", args_list[0]);
		break;
	case 0x01580003:
		INFO("PMU3: In write_level_coarse() csn=%d dimm=%d pstate=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01590003:
		INFO("PMU3: left eye edge search db:%d ln:%d dly:0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x015a0003:
		INFO("PMU3: right eye edge search db: %d ln: %d dly: 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x015b0004:
		INFO("PMU3: eye center db: %d ln: %d dly: 0x%x (maxdq: 0x%x)\n", args_list[0], args_list[1], args_list[2], args_list[3]);
		break;
	case 0x015c0003:
		INFO("PMU3: Wrote to TxDqDly db: %d ln: %d dly: 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x015d0003:
		INFO("PMU3: Wrote to TxDqDly db: %d ln: %d dly: 0x%x\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x015e0002:
		INFO("PMU3: Coarse write leveling nibble%2d is still failing for TxDqsDly=0x%04x\n", args_list[0], args_list[1]);
		break;
	case 0x015f0002:
		INFO("PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n", args_list[0], args_list[1]);
		break;
	case 0x01600000:
		INFO("PMU: Error: Failed write leveling coarse\n");
		break;
	case 0x01610000:
		INFO("PMU4: WL normalized pos   : ................................|................................\n");
		break;
	case 0x01620009:
		INFO("PMU4: WL margin for nib %2d: %08x%08x%08x%08x%08x%08x%08x%08x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8]);
		break;
	case 0x01630000:
		INFO("PMU4: WL normalized pos   : ................................|................................\n");
		break;
	case 0x01640001:
		INFO("PMU8: Adjust margin after WL coarse to be larger than %d\n", args_list[0]);
		break;
	case 0x01650001:
		INFO("PMU: Error: All margin after write leveling coarse are smaller than minMargin %d\n", args_list[0]);
		break;
	case 0x01660002:
		INFO("PMU8: Decrement nib %d TxDqsDly by %d fine step\n", args_list[0], args_list[1]);
		break;
	case 0x01670003:
		INFO("PMU3: In write_level_coarse() csn=%d dimm=%d pstate=%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01680005:
		INFO("PMU2: Write level: dbyte %d nib%d dq/dmbi %2d dqsfine 0x%04x dqDly 0x%04x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4]);
		break;
	case 0x01690002:
		INFO("PMU3: Coarse write leveling nibble%2d is still failing for TxDqsDly=0x%04x\n", args_list[0], args_list[1]);
		break;
	case 0x016a0002:
		INFO("PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n", args_list[0], args_list[1]);
		break;
	case 0x016b0000:
		INFO("PMU: Error: Failed write leveling coarse\n");
		break;
	case 0x016c0001:
		INFO("PMU3: DWL delay = %d\n", args_list[0]);
		break;
	case 0x016d0003:
		INFO("PMU3: Errcnt for DWL nib %2d delay = %2d is %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x016e0002:
		INFO("PMU3: DWL nibble %d sampled a 1 at delay %d\n", args_list[0], args_list[1]);
		break;
	case 0x016f0003:
		INFO("PMU3: DWL nibble %d passed at delay %d. Rising edge was at %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x01700000:
		INFO("PMU2: DWL did nto find a rising edge of memclk for all nibbles. Failing nibbles assumed to have rising edge close to fine delay 63\n");
		break;
	case 0x01710002:
		INFO("PMU2:  Rising edge found in alias window, setting wrlvlDly for nibble %d = %d\n", args_list[0], args_list[1]);
		break;
	case 0x01720002:
		INFO("PMU: Error: Failed DWL for nib %d with %d one\n", args_list[0], args_list[1]);
		break;
	case 0x01730003:
		INFO("PMU2:  Rising edge not found in alias window with %d one, leaving wrlvlDly for nibble %d = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x04000000:
		INFO("PMU: Error:Mailbox Buffer Overflowed.\n");
		break;
	case 0x04010000:
		INFO("PMU: Error:Mailbox Buffer Overflowed.\n");
		break;
	case 0x04020000:
		INFO("PMU: ***** Assertion Error - terminating *****\n");
		break;
	case 0x04030002:
		INFO("PMU1: swapByte db %d by %d\n", args_list[0], args_list[1]);
		break;
	case 0x04040003:
		INFO("PMU3: get_cmd_dly max(%d ps, %d memclk) = %d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x04050002:
		INFO("PMU0: Write CSR 0x%06x 0x%04x\n", args_list[0], args_list[1]);
		break;
	case 0x04060002:
		INFO("PMU0: hwt_init_ppgc_prbs(): Polynomial: %x, Deg: %d\n", args_list[0], args_list[1]);
		break;
	case 0x04070001:
		INFO("PMU: Error: acsm_set_cmd to non existant instruction adddress %d\n", args_list[0]);
		break;
	case 0x04080001:
		INFO("PMU: Error: acsm_set_cmd with unknown ddr cmd 0x%x\n", args_list[0]);
		break;
	case 0x0409000c:
		INFO("PMU1: acsm_addr %02x, acsm_flgs %04x, ddr_cmd %02x, cmd_dly %02x, ddr_addr %04x, ddr_bnk %02x, ddr_cs %02x, cmd_rcnt %02x, AcsmSeq0/1/2/3 %04x %04x %04x %04x\n", args_list[0], args_list[1], args_list[2], args_list[3], args_list[4], args_list[5], args_list[6], args_list[7], args_list[8], args_list[9], args_list[10], args_list[11]);
		break;
	case 0x040a0000:
		INFO("PMU: Error: Polling on ACSM done failed to complete in acsm_poll_done()...\n");
		break;
	case 0x040b0000:
		INFO("PMU1: acsm RUN\n");
		break;
	case 0x040c0000:
		INFO("PMU1: acsm STOPPED\n");
		break;
	case 0x040d0002:
		INFO("PMU1: acsm_init: acsm_mode %04x mxrdlat %04x\n", args_list[0], args_list[1]);
		break;
	case 0x040e0002:
		INFO("PMU: Error: setAcsmCLCWL: cl and cwl must be each >= 2 and 5, resp. CL=%d CWL=%d\n", args_list[0], args_list[1]);
		break;
	case 0x040f0002:
		INFO("PMU: Error: setAcsmCLCWL: cl and cwl must be each >= 5. CL=%d CWL=%d\n", args_list[0], args_list[1]);
		break;
	case 0x04100002:
		INFO("PMU1: setAcsmCLCWL: CASL %04d WCASL %04d\n", args_list[0], args_list[1]);
		break;
	case 0x04110001:
		INFO("PMU: Error: Reserved value of register F0RC0F found in message block: 0x%04x\n", args_list[0]);
		break;
	case 0x04120001:
		INFO("PMU3: Written MRS to CS=0x%02x\n", args_list[0]);
		break;
	case 0x04130001:
		INFO("PMU3: Written MRS to CS=0x%02x\n", args_list[0]);
		break;
	case 0x04140000:
		INFO("PMU3: Entering Boot Freq Mode.\n");
		break;
	case 0x04150001:
		INFO("PMU: Error: Boot clock divider setting of %d is too small\n", args_list[0]);
		break;
	case 0x04160000:
		INFO("PMU3: Exiting Boot Freq Mode.\n");
		break;
	case 0x04170002:
		INFO("PMU3: Writing MR%d OP=%x\n", args_list[0], args_list[1]);
		break;
	case 0x04180000:
		INFO("PMU: Error: Delay too large in slomo\n");
		break;
	case 0x04190001:
		INFO("PMU3: Written MRS to CS=0x%02x\n", args_list[0]);
		break;
	case 0x041a0000:
		INFO("PMU3: Enable Channel A\n");
		break;
	case 0x041b0000:
		INFO("PMU3: Enable Channel B\n");
		break;
	case 0x041c0000:
		INFO("PMU3: Enable All Channels\n");
		break;
	case 0x041d0002:
		INFO("PMU2: Use PDA mode to set MR%d with value 0x%02x\n", args_list[0], args_list[1]);
		break;
	case 0x041e0001:
		INFO("PMU3: Written Vref with PDA to CS=0x%02x\n", args_list[0]);
		break;
	case 0x041f0000:
		INFO("PMU1: start_cal: DEBUG: setting CalRun to 1\n");
		break;
	case 0x04200000:
		INFO("PMU1: start_cal: DEBUG: setting CalRun to 0\n");
		break;
	case 0x04210001:
		INFO("PMU1: lock_pll_dll: DEBUG: pstate = %d\n", args_list[0]);
		break;
	case 0x04220001:
		INFO("PMU1: lock_pll_dll: DEBUG: dfifreqxlat_pstate = %d\n", args_list[0]);
		break;
	case 0x04230001:
		INFO("PMU1: lock_pll_dll: DEBUG: pllbypass = %d\n", args_list[0]);
		break;
	case 0x04240001:
		INFO("PMU3: SaveLcdlSeed: Saving seed seed %d\n", args_list[0]);
		break;
	case 0x04250000:
		INFO("PMU1: in phy_defaults()\n");
		break;
	case 0x04260003:
		INFO("PMU3: ACXConf:%d MaxNumDbytes:%d NumDfi:%d\n", args_list[0], args_list[1], args_list[2]);
		break;
	case 0x04270005:
		INFO("PMU1: setAltAcsmCLCWL setting cl=%d cwl=%d %d %d %d \n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
		break;
	default:
		INFO("PMU Streaming Msg: Debug message not recognized !!  code: %x", codede_message_hex);
		panic();
  }
#endif
}

static void decode_streaming_message_lpddr4 (uint32_t codede_message_hex, uint16_t *args_list)
{
//   switch (codede_message_hex) {
// 	case(0x00000001) :
// 		INFO("PMU1:prbsGenCtl:%x\n",args_list[0]);
// 		break;
// 	case(0x00010000) :
// 		INFO("PMU1: loading 2D acsm sequence\n");
// 		break;
// 	case(0x00020000) :
// 		INFO("PMU1: loading 1D acsm sequence\n");
// 		break;
// 	case(0x00030002) :
// 		INFO("PMU3: %d memclocks @ %d to get half of 300ns\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00040000) :
// 		INFO("PMU: Error: User requested MPR read pattern for read DQS training in DDR3 Mode\n");
// 		break;
// 	case(0x00050000) :
// 		INFO("PMU3: Running 1D search for left eye edge\n");
// 		break;
// 	case(0x00060001) :
// 		INFO("PMU1: In Phase Left Edge Search cs %d\n",args_list[0]);
// 		break;
// 	case(0x00070001) :
// 		INFO("PMU1: Out of Phase Left Edge Search cs %d\n",args_list[0]);
// 		break;
// 	case(0x00080000) :
// 		INFO("PMU3: Running 1D search for right eye edge\n");
// 		break;
// 	case(0x00090001) :
// 		INFO("PMU1: In Phase Right Edge Search cs %d\n",args_list[0]);
// 		break;
// 	case(0x000a0001) :
// 		INFO("PMU1: Out of Phase Right Edge Search cs %d\n",args_list[0]);
// 		break;
// 	case(0x000b0001) :
// 		INFO("PMU1: mxRdLat training pstate %d\n",args_list[0]);
// 		break;
// 	case(0x000c0001) :
// 		INFO("PMU1: mxRdLat search for cs %d\n",args_list[0]);
// 		break;
// 	case(0x000d0001) :
// 		INFO("PMU0: MaxRdLat non consistant DtsmLoThldXingInd 0x%03x\n",args_list[0]);
// 		break;
// 	case(0x000e0003) :
// 		INFO("PMU4: CS %d Dbyte %d worked with DFIMRL = %d DFICLKs \n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x000f0004) :
// 		INFO("PMU3: MaxRdLat Read Lane err mask for csn %d, DFIMRL %2d DFIClks, dbyte %d = 0x%03x\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x00100003) :
// 		INFO("PMU3: MaxRdLat Read Lane err mask for csn %d DFIMRL %2d, All dbytes = 0x%03x\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00110001) :
// 		INFO("PMU: Error: CS%d failed to find a DFIMRL setting that worked for all bytes during MaxRdLat training\n",args_list[0]);
// 		break;
// 	case(0x00120002) :
// 		INFO("PMU3: Smallest passing DFIMRL for all dbytes in CS%d = %d DFIClks\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00130000) :
// 		INFO("PMU: Error: No passing DFIMRL value found for any chip select during MaxRdLat training\n");
// 		break;
// 	case(0x00140003) :
// 		INFO("PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00150006) :
// 		INFO("PMU4: Adjusting rxclkdly db %d nib %d from %d+%d=%d->%d\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4],args_list[5]);
// 		break;
// 	case(0x00160000) :
// 		INFO("PMU4: TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n");
// 		break;
// 	case(0x00170005) :
// 		INFO("PMU4: DB %d Lane %d: %3d %3d -> %3d\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	case(0x00180002) :
// 		INFO("PMU2: TXDQ delayLeft[%2d] = %3d (DISCONNECTED)\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00190004) :
// 		INFO("PMU2: TXDQ delayLeft[%2d] = %3d oopScaled = %3d selectOop %d\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x001a0002) :
// 		INFO("PMU2: TXDQ delayRight[%2d] = %3d (DISCONNECTED)\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x001b0004) :
// 		INFO("PMU2: TXDQ delayRight[%2d] = %3d oopScaled = %3d selectOop %d\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x001c0003) :
// 		INFO("PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x001d0000) :
// 		INFO("PMU4: TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n");
// 		break;
// 	case(0x001e0002) :
// 		INFO("PMU4: DB %d Lane %d: (DISCONNECTED)\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x001f0005) :
// 		INFO("PMU4: DB %d Lane %d: %3d %3d -> %3d\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	case(0x00200002) :
// 		INFO("PMU3: Running 1D search csn %d for DM Right/NotLeft(%d) eye edge\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00210002) :
// 		INFO("PMU3: WrDq DM byte%2d with Errcnt %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00220002) :
// 		INFO("PMU3: WrDq DM byte%2d avgDly 0x%04x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00230002) :
// 		INFO("PMU1: WrDq DM byte%2d with Errcnt %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00240001) :
// 		INFO("PMU: Error: Dbyte %d txDqDly DM training did not start inside the eye\n",args_list[0]);
// 		break;
// 	case(0x00250000) :
// 		INFO("PMU4: DM TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n");
// 		break;
// 	case(0x00260002) :
// 		INFO("PMU4: DB %d Lane %d: (DISCONNECTED)\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00270005) :
// 		INFO("PMU4: DB %d Lane %d: %3d %3d -> %3d\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	case(0x00280003) :
// 		INFO("PMU: Error: Dbyte %d lane %d txDqDly DM passing region is too small (width = %d)\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00290004) :
// 		INFO("PMU3: Errcnt for MRD/MWD search nib %2d delay = (%d, 0x%02x) = %d\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x002a0000) :
// 		INFO("PMU3: Precharge all open banks\n");
// 		break;
// 	case(0x002b0002) :
// 		INFO("PMU: Error: Dbyte %d nibble %d found multiple working coarse delay setting for MRD/MWD\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x002c0000) :
// 		INFO("PMU4: MRD Passing Regions (coarseVal, fineLeft fineRight -> fineCenter)\n");
// 		break;
// 	case(0x002d0000) :
// 		INFO("PMU4: MWD Passing Regions (coarseVal, fineLeft fineRight -> fineCenter)\n");
// 		break;
// 	case(0x002e0004) :
// 		INFO("PMU10: Warning: DB %d nibble %d has multiple working coarse positions, %d and %d, choosing the smaller delay\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x002f0003) :
// 		INFO("PMU: Error: Dbyte %d nibble %d MRD/MWD passing region is too small (width = %d)\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00300006) :
// 		INFO("PMU4: DB %d nibble %d: %3d, %3d %3d -> %3d\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4],args_list[5]);
// 		break;
// 	case(0x00310002) :
// 		INFO("PMU1: Start MRD/nMWD %d for csn %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00320002) :
// 		INFO("PMU2: RXDQS delayLeft[%2d] = %3d (DISCONNECTED)\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00330006) :
// 		INFO("PMU2: RXDQS delayLeft[%2d] = %3d delayOop[%2d] = %3d OopScaled %4d, selectOop %d\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4],args_list[5]);
// 		break;
// 	case(0x00340002) :
// 		INFO("PMU2: RXDQS delayRight[%2d] = %3d (DISCONNECTED)\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00350006) :
// 		INFO("PMU2: RXDQS delayRight[%2d] = %3d delayOop[%2d] = %4d OopScaled %4d, selectOop %d\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4],args_list[5]);
// 		break;
// 	case(0x00360000) :
// 		INFO("PMU4: RxClkDly Passing Regions (EyeLeft EyeRight -> EyeCenter)\n");
// 		break;
// 	case(0x00370002) :
// 		INFO("PMU4: DB %d nibble %d: (DISCONNECTED)\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00380005) :
// 		INFO("PMU4: DB %d nibble %d: %3d %3d -> %3d\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	case(0x00390003) :
// 		INFO("PMU: Error: Dbyte %d nibble %d rxClkDly passing region is too small (width = %d)\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x003a0002) :
// 		INFO("PMU0: goodbar = %d for RDWR_BLEN %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x003b0001) :
// 		INFO("PMU3: RxClkDly = %d\n",args_list[0]);
// 		break;
// 	case(0x003c0005) :
// 		INFO("PMU0: db %d l %d absLane %d -> bottom %d top %d\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	case(0x003d0009) :
// 		INFO("PMU3: BYTE %d - %3d %3d %3d %3d %3d %3d %3d %3d\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4],args_list[5],args_list[6],args_list[7],args_list[8]);
// 		break;
// 	case(0x003e0002) :
// 		INFO("PMU: Error: dbyte %d lane %d's per-lane vrefDAC's had no passing region\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x003f0004) :
// 		INFO("PMU0: db%d l%d - %d %d\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x00400002) :
// 		INFO("PMU0: goodbar = %d for RDWR_BLEN %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00410004) :
// 		INFO("PMU3: db%d l%d saw %d issues at rxClkDly %d\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x00420003) :
// 		INFO("PMU3: db%d l%d first saw a pass->fail edge at rxClkDly %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00430002) :
// 		INFO("PMU3: lane %d PBD = %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00440003) :
// 		INFO("PMU3: db%d l%d first saw a DBI pass->fail edge at rxClkDly %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00450003) :
// 		INFO("PMU2: db%d l%d already passed rxPBD = %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00460003) :
// 		INFO("PMU0: db%d l%d, PBD = %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00470002) :
// 		INFO("PMU: Error: dbyte %d lane %d failed read deskew\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00480003) :
// 		INFO("PMU0: db%d l%d, inc PBD = %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00490003) :
// 		INFO("PMU1: Running lane deskew on pstate %d csn %d rdDBIEn %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x004a0000) :
// 		INFO("PMU: Error: Read deskew training has been requested, but csrMajorModeDbyte[2] is set\n");
// 		break;
// 	case(0x004b0002) :
// 		INFO("PMU1: AcsmCsMapCtrl%02d 0x%04x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x004c0002) :
// 		INFO("PMU1: AcsmCsMapCtrl%02d 0x%04x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x004d0001) :
// 		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D3U Type\n",args_list[0]);
// 		break;
// 	case(0x004e0001) :
// 		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D3R Type\n",args_list[0]);
// 		break;
// 	case(0x004f0001) :
// 		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4U Type\n",args_list[0]);
// 		break;
// 	case(0x00500001) :
// 		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4R Type\n",args_list[0]);
// 		break;
// 	case(0x00510001) :
// 		INFO("PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4LR Type\n",args_list[0]);
// 		break;
// 	case(0x00520000) :
// 		INFO("PMU: Error: Both 2t timing mode and ddr4 geardown mode specifed in the messageblock's PhyCfg and MR3 fields. Only one can be enabled\n");
// 		break;
// 	case(0x00530003) :
// 		INFO("PMU10: PHY TOTALS - NUM_DBYTES %d NUM_NIBBLES %d NUM_ANIBS %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00540006) :
// 		//INFO("PMU10: CSA=0x%02X, CSB=0x%02X, TSTAGES=0x%04X, HDTOUT=%d, MMISC=%d DRAMFreq=%dMT DramType=LPDDR3\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4],args_list[5]);
// 		INFO("\n");
// 		break;
// 	case(0x00550006) :
// 		INFO("PMU10: CSA=0x%02X, CSB=0x%02X, TSTAGES=0x%04X, HDTOUT=%d, MMISC=%d DRAMFreq=%dMT DramType=LPDDR4\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4],args_list[5]);
// 		break;
// 	case(0x00560008) :
// 		INFO("PMU10: CS=0x%02X, TSTAGES=0x%04X, HDTOUT=%d, 2T=%d, MMISC=%d AddrMirror=%d DRAMFreq=%dMT DramType=%d\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4],args_list[5],args_list[6],args_list[7]);
// 		break;
// 	case(0x00570004) :
// 		//INFO("PMU10: Pstate%d MR0=0x%04X MR1=0x%04X MR2=0x%04X\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		INFO("\n");
// 		break;
// 	case(0x00580008) :
// 		//INFO("PMU10: Pstate%d MRS MR0=0x%04X MR1=0x%04X MR2=0x%04X MR3=0x%04X MR4=0x%04X MR5=0x%04X MR6=0x%04X\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4],args_list[5],args_list[6],args_list[7]);
// 		INFO("\n");
// 		break;
// 	case(0x00590005) :
// 		//INFO("PMU10: Pstate%d MRS MR1_A0=0x%04X MR2_A0=0x%04X MR3_A0=0x%04X MR11_A0=0x%04X\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		INFO("\n");
// 		break;
// 	case(0x005a0000) :
// 		INFO("PMU10: UseBroadcastMR set. All ranks and channels use MRXX_A0 for MR settings.\n");
// 		break;
// 	case(0x005b0005) :
// 		INFO("PMU10: Pstate%d MRS MR01_A0=0x%02X MR02_A0=0x%02X MR03_A0=0x%02X MR11_A0=0x%02X\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	case(0x005c0005) :
// 		INFO("PMU10: Pstate%d MRS MR12_A0=0x%02X MR13_A0=0x%02X MR14_A0=0x%02X MR22_A0=0x%02X\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	case(0x005d0005) :
// 		INFO("PMU10: Pstate%d MRS MR01_A1=0x%02X MR02_A1=0x%02X MR03_A1=0x%02X MR11_A1=0x%02X\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	case(0x005e0005) :
// 		INFO("PMU10: Pstate%d MRS MR12_A1=0x%02X MR13_A1=0x%02X MR14_A1=0x%02X MR22_A1=0x%02X\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	case(0x005f0005) :
// 		INFO("PMU10: Pstate%d MRS MR01_B0=0x%02X MR02_B0=0x%02X MR03_B0=0x%02X MR11_B0=0x%02X\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	case(0x00600005) :
// 		INFO("PMU10: Pstate%d MRS MR12_B0=0x%02X MR13_B0=0x%02X MR14_B0=0x%02X MR22_B0=0x%02X\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	case(0x00610005) :
// 		INFO("PMU10: Pstate%d MRS MR01_B1=0x%02X MR02_B1=0x%02X MR03_B1=0x%02X MR11_B1=0x%02X\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	case(0x00620005) :
// 		INFO("PMU10: Pstate%d MRS MR12_B1=0x%02X MR13_B1=0x%02X MR14_B1=0x%02X MR22_B1=0x%02X\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	case(0x00630002) :
// 		INFO("PMU1: AcsmOdtCtrl%02d 0x%02x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00640002) :
// 		INFO("PMU1: AcsmCsMapCtrl%02d 0x%04x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00650002) :
// 		INFO("PMU1: AcsmCsMapCtrl%02d 0x%04x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00660000) :
// 		INFO("PMU1: HwtCAMode set\n");
// 		break;
// 	case(0x00670001) :
// 		INFO("PMU3: DDR4 infinite preamble enter/exit mode %d\n",args_list[0]);
// 		break;
// 	case(0x00680002) :
// 		INFO("PMU1: In rxenb_train() csn=%d pstate=%d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00690000) :
// 		INFO("PMU3: Finding DQS falling edge\n");
// 		break;
// 	case(0x006a0000) :
// 		INFO("PMU3: Searching for DDR3/LPDDR3/LPDDR4 read preamble\n");
// 		break;
// 	case(0x006b0009) :
// 		INFO("PMU3: dtsm fails Even Nibbles : %2x %2x %2x %2x %2x %2x %2x %2x %2x\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4],args_list[5],args_list[6],args_list[7],args_list[8]);
// 		break;
// 	case(0x006c0009) :
// 		INFO("PMU3: dtsm fails Odd  Nibbles : %2x %2x %2x %2x %2x %2x %2x %2x %2x\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4],args_list[5],args_list[6],args_list[7],args_list[8]);
// 		break;
// 	case(0x006d0002) :
// 		INFO("PMU3: Preamble search pass=%d anyfail=%d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x006e0000) :
// 		INFO("PMU: Error: RxEn training preamble not found\n");
// 		break;
// 	case(0x006f0000) :
// 		INFO("PMU3: Found DQS pre-amble\n");
// 		break;
// 	case(0x00700001) :
// 		INFO("PMU: Error: Dbyte %d couldn't find the rising edge of DQS during RxEn Training\n",args_list[0]);
// 		break;
// 	case(0x00710000) :
// 		INFO("PMU3: RxEn aligning to first rising edge of burst\n");
// 		break;
// 	case(0x00720001) :
// 		INFO("PMU3: Decreasing RxEn delay by %d fine step to allow full capture of reads\n",args_list[0]);
// 		break;
// 	case(0x00730001) :
// 		INFO("PMU3: MREP Delay = %d\n",args_list[0]);
// 		break;
// 	case(0x00740003) :
// 		INFO("PMU3: Errcnt for MREP nib %2d delay = %2d is %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00750002) :
// 		INFO("PMU3: MREP nibble %d sampled a 1 at data buffer delay %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00760002) :
// 		INFO("PMU3: MREP nibble %d saw a 0 to 1 transition at data buffer delay %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00770000) :
// 		INFO("PMU2:  MREP did not find a 0 to 1 transition for all nibbles. Failing nibbles assumed to have rising edge close to fine delay 63\n");
// 		break;
// 	case(0x00780002) :
// 		INFO("PMU2:  Rising edge found in alias window, setting rxDly for nibble %d = %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00790002) :
// 		INFO("PMU: Error: Failed MREP for nib %d with %d one\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x007a0003) :
// 		INFO("PMU2:  Rising edge not found in alias window with %d one, leaving rxDly for nibble %d = %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x007b0002) :
// 		INFO("PMU3: Training DIMM %d CSn %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x007c0001) :
// 		INFO("PMU3: exitCAtrain_lp3 cs 0x%x\n",args_list[0]);
// 		break;
// 	case(0x007d0001) :
// 		INFO("PMU3: enterCAtrain_lp3 cs 0x%x\n",args_list[0]);
// 		break;
// 	case(0x007e0001) :
// 		INFO("PMU3: CAtrain_switchmsb_lp3 cs 0x%x\n",args_list[0]);
// 		break;
// 	case(0x007f0001) :
// 		INFO("PMU3: CATrain_rdwr_lp3 looking for pattern %x\n",args_list[0]);
// 		break;
// 	case(0x00800000) :
// 		INFO("PMU3: exitCAtrain_lp4\n");
// 		break;
// 	case(0x00810001) :
// 		INFO("PMU3: DEBUG enterCAtrain_lp4 1: cs 0x%x\n",args_list[0]);
// 		break;
// 	case(0x00820001) :
// 		INFO("PMU3: DEBUG enterCAtrain_lp4 3: Put dbyte %d in async mode\n",args_list[0]);
// 		break;
// 	case(0x00830000) :
// 		INFO("PMU3: DEBUG enterCAtrain_lp4 5: Send MR13 to turn on CA training\n");
// 		break;
// 	case(0x00840003) :
// 		INFO("PMU3: DEBUG enterCAtrain_lp4 7: idx = %d vref = %x mr12 = %x \n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00850001) :
// 		INFO("PMU3: CATrain_rdwr_lp4 looking for pattern %x\n",args_list[0]);
// 		break;
// 	case(0x00860004) :
// 		INFO("PMU3: Phase %d CAreadbackA db:%d %x xo:%x\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x00870005) :
// 		INFO("PMU3: DEBUG lp4SetCatrVref 1: cs=%d chan=%d mr12=%x vref=%d.%d%%\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	case(0x00880003) :
// 		INFO("PMU3: DEBUG lp4SetCatrVref 3: mr12 = %x send vref= %x to db=%d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00890000) :
// 		INFO("PMU10:Optimizing vref\n");
// 		break;
// 	case(0x008a0004) :
// 		INFO("PMU4:mr12:%2x cs:%d chan %d r:%4x\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x008b0005) :
// 		INFO("PMU3: i:%2d bstr:%2d bsto:%2d st:%d r:%d\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	case(0x008c0002) :
// 		INFO("Failed to find sufficient CA Vref Passing Region for CS %d channel %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x008d0005) :
// 		INFO("PMU3:Found %d.%d%% MR12:%x for cs:%d chan %d\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	case(0x008e0002) :
// 		INFO("PMU3:Calculated %d for AtxImpedence from acx %d.\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x008f0000) :
// 		INFO("PMU3:CA Odt impedence ==0.  Use default vref.\n");
// 		break;
// 	case(0x00900003) :
// 		INFO("PMU3:Calculated %d.%d%% for Vref MR12=0x%x.\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00910000) :
// 		INFO("PMU3: CAtrain_lp\n");
// 		break;
// 	case(0x00920000) :
// 		INFO("PMU3: CAtrain Begins.\n");
// 		break;
// 	case(0x00930001) :
// 		INFO("PMU3: CAtrain_lp testing dly %d\n",args_list[0]);
// 		break;
// 	case(0x00940001) :
// 		INFO("PMU5: CA bitmap dump for cs %x\n",args_list[0]);
// 		break;
// 	case(0x00950001) :
// 		INFO("PMU5: CAA%d \n",args_list[0]);
// 		break;
// 	case(0x00960001) :
// 		INFO("%02x",args_list[0]);
// 		break;
// 	case(0x00970000) :
// 		INFO("\n");
// 		break;
// 	case(0x00980001) :
// 		INFO("PMU5: CAB%d ",args_list[0]);
// 		break;
// 	case(0x00990001) :
// 		INFO("%02x",args_list[0]);
// 		break;
// 	case(0x009a0000) :
// 		INFO("\n");
// 		break;
// 	case(0x009b0003) :
// 		INFO("PMU3: anibi=%d, anibichan[anibi]=%d ,chan=%d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x009c0001) :
// 		INFO("%02x",args_list[0]);
// 		break;
// 	case(0x009d0001) :
// 		INFO("\nPMU3:Raw CA setting :%x",args_list[0]);
// 		break;
// 	case(0x009e0002) :
// 		INFO("\nPMU3:ATxDly setting:%x margin:%d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x009f0002) :
// 		INFO("\nPMU3:InvClk ATxDly setting:%x margin:%d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00a00000) :
// 		INFO("\nPMU3:No Range found!\n");
// 		break;
// 	case(0x00a10003) :
// 		INFO("PMU3: 2 anibi=%d, anibichan[anibi]=%d ,chan=%d",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00a20002) :
// 		INFO("\nPMU3: no neg clock => CA setting anib=%d, :%d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00a30001) :
// 		INFO("PMU3:Normal margin:%d\n",args_list[0]);
// 		break;
// 	case(0x00a40001) :
// 		INFO("PMU3:Inverted margin:%d\n",args_list[0]);
// 		break;
// 	case(0x00a50000) :
// 		INFO("PMU3:Using Inverted clock\n");
// 		break;
// 	case(0x00a60000) :
// 		INFO("PMU3:Using normal clk\n");
// 		break;
// 	case(0x00a70003) :
// 		INFO("PMU3: 3 anibi=%d, anibichan[anibi]=%d ,chan=%d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00a80002) :
// 		INFO("PMU3: Setting ATxDly for anib %x to %x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00a90000) :
// 		INFO("PMU: Error: CA Training Failed.\n");
// 		break;
// 	case(0x00aa0000) :
// 		INFO("PMU1: Writing MRs\n");
// 		break;
// 	case(0x00ab0000) :
// 		INFO("PMU4:Using MR12 values from 1D CA VREF training.\n");
// 		break;
// 	case(0x00ac0000) :
// 		INFO("PMU3:Writing all MRs to fsp 1\n");
// 		break;
// 	case(0x00ad0000) :
// 		INFO("PMU10:Lp4Quickboot mode.\n");
// 		break;
// 	case(0x00ae0000) :
// 		INFO("PMU3: Writing MRs\n");
// 		break;
// 	case(0x00af0001) :
// 		INFO("PMU10: Setting boot clock divider to %d\n",args_list[0]);
// 		break;
// 	case(0x00b00000) :
// 		INFO("PMU3: Resetting DRAM\n");
// 		break;
// 	case(0x00b10000) :
// 		INFO("PMU3: setup for RCD initalization\n");
// 		break;
// 	case(0x00b20000) :
// 		INFO("PMU3: pmu_exit_SR from dev_init()\n");
// 		break;
// 	case(0x00b30000) :
// 		INFO("PMU3: initializing RCD\n");
// 		break;
// 	case(0x00b40000) :
// 		INFO("PMU10: **** Executing 2D Image ****\n");
// 		break;
// 	case(0x00b50001) :
// 		INFO("PMU10: **** Start DDR4 Training. PMU Firmware Revision 0x%04x ****\n",args_list[0]);
// 		break;
// 	case(0x00b60001) :
// 		INFO("PMU10: **** Start DDR3 Training. PMU Firmware Revision 0x%04x ****\n",args_list[0]);
// 		break;
// 	case(0x00b70001) :
// 		INFO("PMU10: **** Start LPDDR3 Training. PMU Firmware Revision 0x%04x ****\n",args_list[0]);
// 		break;
// 	case(0x00b80001) :
// 		INFO("PMU10: **** Start LPDDR4 Training. PMU Firmware Revision 0x%04x ****\n",args_list[0]);
// 		break;
// 	case(0x00b90000) :
// 		INFO("PMU: Error: Mismatched internal revision between DCCM and ICCM images\n");
// 		break;
// 	case(0x00ba0001) :
// 		INFO("PMU10: **** Testchip %d Specific Firmware ****\n",args_list[0]);
// 		break;
// 	case(0x00bb0000) :
// 		INFO("PMU1: LRDIMM with EncodedCS mode, one DIMM\n");
// 		break;
// 	case(0x00bc0000) :
// 		INFO("PMU1: LRDIMM with EncodedCS mode, two DIMMs\n");
// 		break;
// 	case(0x00bd0000) :
// 		INFO("PMU1: RDIMM with EncodedCS mode, one DIMM\n");
// 		break;
// 	case(0x00be0000) :
// 		INFO("PMU2: Starting LRDIMM MREP training for all ranks\n");
// 		break;
// 	case(0x00bf0000) :
// 		INFO("PMU199: LRDIMM MREP training for all ranks completed\n");
// 		break;
// 	case(0x00c00000) :
// 		INFO("PMU2: Starting LRDIMM DWL training for all ranks\n");
// 		break;
// 	case(0x00c10000) :
// 		INFO("PMU199: LRDIMM DWL training for all ranks completed\n");
// 		break;
// 	case(0x00c20000) :
// 		INFO("PMU2: Starting LRDIMM MRD training for all ranks\n");
// 		break;
// 	case(0x00c30000) :
// 		INFO("PMU199: LRDIMM MRD training for all ranks completed\n");
// 		break;
// 	case(0x00c40000) :
// 		INFO("PMU2: Starting RXEN training for all ranks\n");
// 		break;
// 	case(0x00c50000) :
// 		INFO("PMU2: Starting write leveling fine delay training for all ranks\n");
// 		break;
// 	case(0x00c60000) :
// 		INFO("PMU2: Starting LRDIMM MWD training for all ranks\n");
// 		break;
// 	case(0x00c70000) :
// 		INFO("PMU199: LRDIMM MWD training for all ranks completed\n");
// 		break;
// 	case(0x00c80000) :
// 		INFO("PMU2: Starting write leveling fine delay training for all ranks\n");
// 		break;
// 	case(0x00c90000) :
// 		INFO("PMU2: Starting read deskew training\n");
// 		break;
// 	case(0x00ca0000) :
// 		INFO("PMU2: Starting SI friendly 1d RdDqs training for all ranks\n");
// 		break;
// 	case(0x00cb0000) :
// 		INFO("PMU2: Starting write leveling coarse delay training for all ranks\n");
// 		break;
// 	case(0x00cc0000) :
// 		INFO("PMU2: Starting 1d WrDq training for all ranks\n");
// 		break;
// 	case(0x00cd0000) :
// 		INFO("PMU2: Running DQS2DQ Oscillator for all ranks\n");
// 		break;
// 	case(0x00ce0000) :
// 		INFO("PMU2: Starting again read deskew training but with PRBS\n");
// 		break;
// 	case(0x00cf0000) :
// 		INFO("PMU2: Starting 1d RdDqs training for all ranks\n");
// 		break;
// 	case(0x00d00000) :
// 		INFO("PMU2: Starting again 1d WrDq training for all ranks\n");
// 		break;
// 	case(0x00d10000) :
// 		INFO("PMU2: Starting MaxRdLat training\n");
// 		break;
// 	case(0x00d20000) :
// 		INFO("PMU2: Starting 2d RdDqs training for all ranks\n");
// 		break;
// 	case(0x00d30000) :
// 		INFO("PMU2: Starting 2d WrDq training for all ranks\n");
// 		break;
// 	case(0x00d40000) :
// 		INFO("PMU2: Starting 2d RdDqs training for all ranks\n");
// 		break;
// 	case(0x00d50002) :
// 		INFO("PMU3:read_fifo %x %x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00d60001) :
// 		INFO("PMU: Error: Invalid PhyDrvImpedance of 0x%x specified in message block.\n",args_list[0]);
// 		break;
// 	case(0x00d70001) :
// 		INFO("PMU: Error: Invalid PhyOdtImpedance of 0x%x specified in message block.\n",args_list[0]);
// 		break;
// 	case(0x00d80001) :
// 		INFO("PMU: Error: Invalid BPZNResVal of 0x%x specified in message block.\n",args_list[0]);
// 		break;
// 	case(0x00d90005) :
// 		INFO("PMU3: fixRxEnBackOff csn:%d db:%d dn:%d bo:%d dly:%x\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	case(0x00da0001) :
// 		INFO("PMU3: fixRxEnBackOff dly:%x\n",args_list[0]);
// 		break;
// 	case(0x00db0000) :
// 		INFO("PMU3: Entering setupPpt\n");
// 		break;
// 	case(0x00dc0000) :
// 		INFO("PMU3: Start lp4PopulateHighLowBytes\n");
// 		break;
// 	case(0x00dd0002) :
// 		INFO("PMU3:Dbyte Detect: db%d received %x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00de0002) :
// 		INFO("PMU3:getDqs2Dq read %x from dbyte %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00df0002) :
// 		INFO("PMU3:getDqs2Dq(2) read %x from dbyte %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00e00001) :
// 		INFO("PMU: Error: Dbyte %d read 0 from the DQS oscillator it is connected to\n",args_list[0]);
// 		break;
// 	case(0x00e10002) :
// 		INFO("PMU4: Dbyte %d dqs2dq = %d/32 UI\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x00e20003) :
// 		INFO("PMU3:getDqs2Dq set dqs2dq:%d/32 ui (%d ps) from dbyte %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00e30003) :
// 		INFO("PMU3: Setting coarse delay in AtxDly chiplet %d from 0x%02x to 0x%02x \n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00e40003) :
// 		INFO("PMU3: Clearing coarse delay in AtxDly chiplet %d from 0x%02x to 0x%02x \n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00e50000) :
// 		INFO("PMU3: Performing DDR4 geardown sync sequence\n");
// 		break;
// 	case(0x00e60000) :
// 		INFO("PMU1: Enter self refresh\n");
// 		break;
// 	case(0x00e70000) :
// 		INFO("PMU1: Exit self refresh\n");
// 		break;
// 	case(0x00e80000) :
// 		INFO("PMU: Error: No dbiEnable with lp4\n");
// 		break;
// 	case(0x00e90000) :
// 		INFO("PMU: Error: No dbiDisable with lp4\n");
// 		break;
// 	case(0x00ea0001) :
// 		INFO("PMU1: DDR4 update Rx DBI Setting disable %d\n",args_list[0]);
// 		break;
// 	case(0x00eb0001) :
// 		INFO("PMU1: DDR4 update 2nCk WPre Setting disable %d\n",args_list[0]);
// 		break;
// 	case(0x00ec0005) :
// 		INFO("PMU1: read_delay: db%d lane%d delays[%2d] = 0x%02x (max 0x%02x)\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	case(0x00ed0004) :
// 		INFO("PMU1: write_delay: db%d lane%d delays[%2d] = 0x%04x\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x00ee0001) :
// 		INFO("PMU5: ID=%d -- db0  db1  db2  db3  db4  db5  db6  db7  db8  db9 --\n",args_list[0]);
// 		break;
// 	case(0x00ef000b) :
// 		INFO("PMU5: [%d]:0x %4x %4x %4x %4x %4x %4x %4x %4x %4x %4x\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4],args_list[5],args_list[6],args_list[7],args_list[8],args_list[9],args_list[10]);
// 		break;
// 	case(0x00f00003) :
// 		INFO("PMU2: dump delays - pstate=%d dimm=%d csn=%d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00f10000) :
// 		INFO("PMU3: Printing Mid-Training Delay Information\n");
// 		break;
// 	case(0x00f20001) :
// 		INFO("PMU5: CS%d <<KEY>> 0 TrainingCntr <<KEY>> coarse(15:10) fine(9:0)\n",args_list[0]);
// 		break;
// 	case(0x00f30001) :
// 		INFO("PMU5: CS%d <<KEY>> 0 RxEnDly, 1 RxClkDly <<KEY>> coarse(10:6) fine(5:0)\n",args_list[0]);
// 		break;
// 	case(0x00f40001) :
// 		INFO("PMU5: CS%d <<KEY>> 0 TxDqsDly, 1 TxDqDly <<KEY>> coarse(9:6) fine(5:0)\n",args_list[0]);
// 		break;
// 	case(0x00f50001) :
// 		INFO("PMU5: CS%d <<KEY>> 0 RxPBDly <<KEY>> 1 Delay Unit ~= 7ps \n",args_list[0]);
// 		break;
// 	case(0x00f60000) :
// 		INFO("PMU5: all CS <<KEY>> 0 DFIMRL <<KEY>> Units = DFI clocks\n");
// 		break;
// 	case(0x00f70000) :
// 		INFO("PMU5: all CS <<KEY>> VrefDACs <<KEY>> DAC(6:0)\n");
// 		break;
// 	case(0x00f80000) :
// 		INFO("PMU1: Set DMD in MR13 and wrDBI in MR3 for training\n");
// 		break;
// 	case(0x00f90000) :
// 		INFO("PMU: Error: getMaxRxen() failed to find largest rxen nibble delay\n");
// 		break;
// 	case(0x00fa0003) :
// 		INFO("PMU2: getMaxRxen(): maxDly %d maxTg %d maxNib %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00fb0003) :
// 		INFO("PMU2: getRankMaxRxen(): maxDly %d Tg %d maxNib %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00fc0000) :
// 		INFO("PMU1: skipping CDD calculation in 2D image\n");
// 		break;
// 	case(0x00fd0001) :
// 		INFO("PMU3: Calculating CDDs for pstate %d\n",args_list[0]);
// 		break;
// 	case(0x00fe0003) :
// 		INFO("PMU3: rxFromDly[%d][%d] = %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x00ff0003) :
// 		INFO("PMU3: rxToDly  [%d][%d] = %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01000003) :
// 		INFO("PMU3: rxDly	[%d][%d] = %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01010003) :
// 		INFO("PMU3: txDly	[%d][%d] = %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01020003) :
// 		INFO("PMU3: allFine CDD_RR_%d_%d = %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01030003) :
// 		INFO("PMU3: allFine CDD_WW_%d_%d = %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01040003) :
// 		INFO("PMU3: CDD_RR_%d_%d = %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01050003) :
// 		INFO("PMU3: CDD_WW_%d_%d = %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01060003) :
// 		INFO("PMU3: allFine CDD_RW_%d_%d = %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01070003) :
// 		INFO("PMU3: allFine CDD_WR_%d_%d = %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01080003) :
// 		INFO("PMU3: CDD_RW_%d_%d = %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01090003) :
// 		INFO("PMU3: CDD_WR_%d_%d = %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x010a0004) :
// 		INFO("PMU3: F%dBC2x_B%d_D%d = 0x%02x\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x010b0004) :
// 		INFO("PMU3: F%dBC3x_B%d_D%d = 0x%02x\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x010c0004) :
// 		INFO("PMU3: F%dBC4x_B%d_D%d = 0x%02x\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x010d0004) :
// 		INFO("PMU3: F%dBC5x_B%d_D%d = 0x%02x\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x010e0004) :
// 		INFO("PMU3: F%dBC8x_B%d_D%d = 0x%02x\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x010f0004) :
// 		INFO("PMU3: F%dBC9x_B%d_D%d = 0x%02x\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x01100004) :
// 		INFO("PMU3: F%dBCAx_B%d_D%d = 0x%02x\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x01110004) :
// 		INFO("PMU3: F%dBCBx_B%d_D%d = 0x%02x\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x01120000) :
// 		INFO("PMU10: Entering context_switch_postamble\n");
// 		break;
// 	case(0x01130003) :
// 		INFO("PMU10: context_switch_postamble is enabled for DIMM %d, RC0A=0x%x, RC3x=0x%x\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01140000) :
// 		INFO("PMU10: Setting bcw fspace 0\n");
// 		break;
// 	case(0x01150001) :
// 		INFO("PMU10: Sending BC0A = 0x%x\n",args_list[0]);
// 		break;
// 	case(0x01160001) :
// 		INFO("PMU10: Sending BC6x = 0x%x\n",args_list[0]);
// 		break;
// 	case(0x01170001) :
// 		INFO("PMU10: Sending RC0A = 0x%x\n",args_list[0]);
// 		break;
// 	case(0x01180001) :
// 		INFO("PMU10: Sending RC3x = 0x%x\n",args_list[0]);
// 		break;
// 	case(0x01190001) :
// 		INFO("PMU10: Sending RC0A = 0x%x\n",args_list[0]);
// 		break;
// 	case(0x011a0001) :
// 		INFO("PMU1: enter_lp3: DEBUG: pstate = %d\n",args_list[0]);
// 		break;
// 	case(0x011b0001) :
// 		INFO("PMU1: enter_lp3: DEBUG: dfifreqxlat_pstate = %d\n",args_list[0]);
// 		break;
// 	case(0x011c0001) :
// 		INFO("PMU1: enter_lp3: DEBUG: pllbypass = %d\n",args_list[0]);
// 		break;
// 	case(0x011d0001) :
// 		INFO("PMU1: enter_lp3: DEBUG: forcecal = %d\n",args_list[0]);
// 		break;
// 	case(0x011e0001) :
// 		INFO("PMU1: enter_lp3: DEBUG: pllmaxrange = 0x%x\n",args_list[0]);
// 		break;
// 	case(0x011f0001) :
// 		INFO("PMU1: enter_lp3: DEBUG: dacval_out = 0x%x\n",args_list[0]);
// 		break;
// 	case(0x01200001) :
// 		INFO("PMU1: enter_lp3: DEBUG: pllctrl3 = 0x%x\n",args_list[0]);
// 		break;
// 	case(0x01210000) :
// 		INFO("PMU3: Loading DRAM with BIOS supplied MR values and entering self refresh prior to exiting PMU code.\n");
// 		break;
// 	case(0x01220002) :
// 		INFO("PMU3: Setting DataBuffer function space of dimmcs 0x%02x to %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x01230002) :
// 		INFO("PMU4: Setting RCW FxRC%Xx = 0x%02x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x01240002) :
// 		INFO("PMU4: Setting RCW FxRC%02X = 0x%02x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x01250001) :
// 		INFO("PMU1: DDR4 update Rd Pre Setting disable %d\n",args_list[0]);
// 		break;
// 	case(0x01260002) :
// 		INFO("PMU2: Setting BCW FxBC%Xx = 0x%02x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x01270002) :
// 		INFO("PMU2: Setting BCW BC%02X = 0x%02x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x01280002) :
// 		INFO("PMU2: Setting BCW PBA mode FxBC%Xx = 0x%02x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x01290002) :
// 		INFO("PMU2: Setting BCW PBA mode BC%02X = 0x%02x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x012a0003) :
// 		INFO("PMU4: BCW value for dimm %d, fspace %d, addr 0x%04x\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x012b0002) :
// 		INFO("PMU4: DB %d, value 0x%02x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x012c0000) :
// 		INFO("PMU6: WARNING MREP underflow, set to min value -2 coarse, 0 fine\n");
// 		break;
// 	case(0x012d0004) :
// 		INFO("PMU6: LRDIMM Writing final data buffer fine delay value nib %2d, trainDly %3d, fineDly code %2d, new MREP fine %2d\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x012e0003) :
// 		INFO("PMU6: LRDIMM Writing final data buffer fine delay value nib %2d, trainDly %3d, fineDly code %2d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x012f0003) :
// 		INFO("PMU6: LRDIMM Writing data buffer fine delay type %d nib %2d, code %2d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01300002) :
// 		INFO("PMU6: Writing final data buffer coarse delay value dbyte %2d, coarse = 0x%02x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x01310003) :
// 		INFO("PMU4: data 0x%04x at MB addr 0x%08x saved at CSR addr 0x%08x\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01320003) :
// 		INFO("PMU4: data 0x%04x at MB addr 0x%08x restored from CSR addr 0x%08x\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01330003) :
// 		INFO("PMU4: data 0x%04x at MB addr 0x%08x saved at CSR addr 0x%08x\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01340003) :
// 		INFO("PMU4: data 0x%04x at MB addr 0x%08x restored from CSR addr 0x%08x\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01350001) :
// 		INFO("PMU3: Update BC00, BC01, BC02 for rank-dimm 0x%02x\n",args_list[0]);
// 		break;
// 	case(0x01360000) :
// 		INFO("PMU3: Writing D4 RDIMM RCD Control words F0RC00 -> F0RC0F\n");
// 		break;
// 	case(0x01370000) :
// 		INFO("PMU3: Disable parity in F0RC0E\n");
// 		break;
// 	case(0x01380000) :
// 		INFO("PMU3: Writing D4 RDIMM RCD Control words F1RC00 -> F1RC05\n");
// 		break;
// 	case(0x01390000) :
// 		INFO("PMU3: Writing D4 RDIMM RCD Control words F1RC1x -> F1RC9x\n");
// 		break;
// 	case(0x013a0000) :
// 		INFO("PMU3: Writing D4 Data buffer Control words BC00 -> BC0E\n");
// 		break;
// 	case(0x013b0002) :
// 		INFO("PMU1: setAltCL Sending MR0 0x%x cl=%d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x013c0002) :
// 		INFO("PMU1: restoreFromAltCL Sending MR0 0x%x cl=%d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x013d0002) :
// 		INFO("PMU1: restoreAcsmFromAltCL Sending MR0 0x%x cl=%d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x013e0002) :
// 		INFO("PMU2: Setting D3R RC%d = 0x%01x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x013f0000) :
// 		INFO("PMU3: Writing D3 RDIMM RCD Control words RC0 -> RC11\n");
// 		break;
// 	case(0x01400002) :
// 		INFO("PMU0: VrefDAC0/1 vddqStart %d dacToVddq %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x01410001) :
// 		INFO("PMU: Error: Messageblock phyVref=0x%x is above the limit for TSMC28's attenuated LPDDR4 receivers. Please see the pub databook\n",args_list[0]);
// 		break;
// 	case(0x01420001) :
// 		INFO("PMU: Error: Messageblock phyVref=0x%x is above the limit for TSMC28's attenuated DDR4 receivers. Please see the pub databook\n",args_list[0]);
// 		break;
// 	case(0x01430001) :
// 		INFO("PMU0: PHY VREF @ (%d/1000) VDDQ\n",args_list[0]);
// 		break;
// 	case(0x01440002) :
// 		INFO("PMU0: initalizing phy vrefDacs to %d ExtVrefRange %x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x01450002) :
// 		INFO("PMU0: initalizing global vref to %d range %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x01460002) :
// 		INFO("PMU4: Setting initial device vrefDQ for CS%d to MR6 = 0x%04x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x01470003) :
// 		INFO("PMU1: In write_level_fine() csn=%d dimm=%d pstate=%d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01480000) :
// 		INFO("PMU3: Fine write leveling hardware search increasing TxDqsDly until full bursts are seen\n");
// 		break;
// 	case(0x01490000) :
// 		INFO("PMU4: WL normalized pos   : ........................|........................\n");
// 		break;
// 	case(0x014a0007) :
// 		INFO("PMU4: WL margin for nib %2d: %08x%08x%08x%08x%08x%08x\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4],args_list[5],args_list[6]);
// 		break;
// 	case(0x014b0000) :
// 		INFO("PMU4: WL normalized pos   : ........................|........................\n");
// 		break;
// 	case(0x014c0000) :
// 		INFO("PMU3: Exiting write leveling mode\n");
// 		break;
// 	case(0x014d0001) :
// 		INFO("PMU3: got %d for cl in load_wrlvl_acsm\n",args_list[0]);
// 		break;
// 	case(0x014e0003) :
// 		INFO("PMU1: In write_level_coarse() csn=%d dimm=%d pstate=%d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x014f0003) :
// 		INFO("PMU3: left eye edge search db:%d ln:%d dly:0x%x\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01500003) :
// 		INFO("PMU3: right eye edge search db:%d ln:%d dly:0x%x\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01510004) :
// 		INFO("PMU3: eye center db:%d ln:%d dly:0x%x (maxdq:%x)\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x01520003) :
// 		INFO("PMU3: Wrote to TxDqDly db:%d ln:%d dly:0x%x\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01530003) :
// 		INFO("PMU3: Wrote to TxDqDly db:%d ln:%d dly:0x%x\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01540002) :
// 		INFO("PMU3: Coarse write leveling dbyte%2d is still failing for TxDqsDly=0x%04x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x01550002) :
// 		INFO("PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x01560000) :
// 		INFO("PMU: Error: Failed write leveling coarse\n");
// 		break;
// 	case(0x01570001) :
// 		INFO("PMU3: got %d for cl in load_wrlvl_acsm\n",args_list[0]);
// 		break;
// 	case(0x01580003) :
// 		INFO("PMU3: In write_level_coarse() csn=%d dimm=%d pstate=%d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01590003) :
// 		INFO("PMU3: left eye edge search db:%d ln:%d dly:0x%x\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x015a0003) :
// 		INFO("PMU3: right eye edge search db: %d ln: %d dly: 0x%x\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x015b0004) :
// 		INFO("PMU3: eye center db: %d ln: %d dly: 0x%x (maxdq: 0x%x)\n",args_list[0],args_list[1],args_list[2],args_list[3]);
// 		break;
// 	case(0x015c0003) :
// 		INFO("PMU3: Wrote to TxDqDly db: %d ln: %d dly: 0x%x\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x015d0003) :
// 		INFO("PMU3: Wrote to TxDqDly db: %d ln: %d dly: 0x%x\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x015e0002) :
// 		INFO("PMU3: Coarse write leveling nibble%2d is still failing for TxDqsDly=0x%04x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x015f0002) :
// 		INFO("PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x01600000) :
// 		INFO("PMU: Error: Failed write leveling coarse\n");
// 		break;
// 	case(0x01610000) :
// 		INFO("PMU4: WL normalized pos   : ................................|................................\n");
// 		break;
// 	case(0x01620009) :
// 		INFO("PMU4: WL margin for nib %2d: %08x%08x%08x%08x%08x%08x%08x%08x\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4],args_list[5],args_list[6],args_list[7],args_list[8]);
// 		break;
// 	case(0x01630000) :
// 		INFO("PMU4: WL normalized pos   : ................................|................................\n");
// 		break;
// 	case(0x01640001) :
// 		INFO("PMU8: Adjust margin after WL coarse to be larger than %d\n",args_list[0]);
// 		break;
// 	case(0x01650001) :
// 		INFO("PMU: Error: All margin after write leveling coarse are smaller than minMargin %d\n",args_list[0]);
// 		break;
// 	case(0x01660002) :
// 		INFO("PMU8: Decrement nib %d TxDqsDly by %d fine step\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x01670003) :
// 		INFO("PMU3: In write_level_coarse() csn=%d dimm=%d pstate=%d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01680005) :
// 		INFO("PMU2: Write level: dbyte %d nib%d dq/dmbi %2d dqsfine 0x%04x dqDly 0x%04x\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	case(0x01690002) :
// 		INFO("PMU3: Coarse write leveling nibble%2d is still failing for TxDqsDly=0x%04x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x016a0002) :
// 		INFO("PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x016b0000) :
// 		INFO("PMU: Error: Failed write leveling coarse\n");
// 		break;
// 	case(0x016c0001) :
// 		INFO("PMU3: DWL delay = %d\n",args_list[0]);
// 		break;
// 	case(0x016d0003) :
// 		INFO("PMU3: Errcnt for DWL nib %2d delay = %2d is %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x016e0002) :
// 		INFO("PMU3: DWL nibble %d sampled a 1 at delay %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x016f0003) :
// 		INFO("PMU3: DWL nibble %d passed at delay %d. Rising edge was at %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x01700000) :
// 		INFO("PMU2: DWL did nto find a rising edge of memclk for all nibbles. Failing nibbles assumed to have rising edge close to fine delay 63\n");
// 		break;
// 	case(0x01710002) :
// 		INFO("PMU2:  Rising edge found in alias window, setting wrlvlDly for nibble %d = %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x01720002) :
// 		INFO("PMU: Error: Failed DWL for nib %d with %d one\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x01730003) :
// 		INFO("PMU2:  Rising edge not found in alias window with %d one, leaving wrlvlDly for nibble %d = %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x04000000) :
// 		INFO("PMU: Error:Mailbox Buffer Overflowed.\n");
// 		break;
// 	case(0x04010000) :
// 		INFO("PMU: Error:Mailbox Buffer Overflowed.\n");
// 		break;
// 	case(0x04020000) :
// 		INFO("PMU: ***** Assertion Error - terminating *****\n");
// 		break;
// 	case(0x04030002) :
// 		INFO("PMU1: swapByte db %d by %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x04040003) :
// 		INFO("PMU3: get_cmd_dly max(%d ps, %d memclk) = %d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x04050002) :
// 		INFO("PMU0: Write CSR 0x%06x 0x%04x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x04060002) :
// 		INFO("PMU0: hwt_init_ppgc_prbs(): Polynomial: %x, Deg: %d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x04070001) :
// 		INFO("PMU: Error: acsm_set_cmd to non existant instruction adddress %d\n",args_list[0]);
// 		break;
// 	case(0x04080001) :
// 		INFO("PMU: Error: acsm_set_cmd with unknown ddr cmd 0x%x\n",args_list[0]);
// 		break;
// 	case(0x0409000c) :
// 		INFO("PMU1: acsm_addr %02x, acsm_flgs %04x, ddr_cmd %02x, cmd_dly %02x, ddr_addr %04x, ddr_bnk %02x, ddr_cs %02x, cmd_rcnt %02x, AcsmSeq0/1/2/3 %04x %04x %04x %04x\n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4],args_list[5],args_list[6],args_list[7],args_list[8],args_list[9],args_list[10],args_list[11]);
// 		break;
// 	case(0x040a0000) :
// 		INFO("PMU: Error: Polling on ACSM done failed to complete in acsm_poll_done()...\n");
// 		break;
// 	case(0x040b0000) :
// 		INFO("PMU1: acsm RUN\n");
// 		break;
// 	case(0x040c0000) :
// 		INFO("PMU1: acsm STOPPED\n");
// 		break;
// 	case(0x040d0002) :
// 		INFO("PMU1: acsm_init: acsm_mode %04x mxrdlat %04x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x040e0002) :
// 		INFO("PMU: Error: setAcsmCLCWL: cl and cwl must be each >= 2 and 5, resp. CL=%d CWL=%d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x040f0002) :
// 		INFO("PMU: Error: setAcsmCLCWL: cl and cwl must be each >= 5. CL=%d CWL=%d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x04100002) :
// 		INFO("PMU1: setAcsmCLCWL: CASL %04d WCASL %04d\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x04110001) :
// 		INFO("PMU: Error: Reserved value of register F0RC0F found in message block: 0x%04x\n",args_list[0]);
// 		break;
// 	case(0x04120001) :
// 		INFO("PMU3: Written MRS to CS=0x%02x\n",args_list[0]);
// 		break;
// 	case(0x04130001) :
// 		INFO("PMU3: Written MRS to CS=0x%02x\n",args_list[0]);
// 		break;
// 	case(0x04140000) :
// 		INFO("PMU3: Entering Boot Freq Mode.\n");
// 		break;
// 	case(0x04150001) :
// 		INFO("PMU: Error: Boot clock divider setting of %d is too small\n",args_list[0]);
// 		break;
// 	case(0x04160000) :
// 		INFO("PMU3: Exiting Boot Freq Mode.\n");
// 		break;
// 	case(0x04170002) :
// 		INFO("PMU3: Writing MR%d OP=%x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x04180000) :
// 		INFO("PMU: Error: Delay too large in slomo\n");
// 		break;
// 	case(0x04190001) :
// 		INFO("PMU3: Written MRS to CS=0x%02x\n",args_list[0]);
// 		break;
// 	case(0x041a0000) :
// 		INFO("PMU3: Enable Channel A\n");
// 		break;
// 	case(0x041b0000) :
// 		INFO("PMU3: Enable Channel B\n");
// 		break;
// 	case(0x041c0000) :
// 		INFO("PMU3: Enable All Channels\n");
// 		break;
// 	case(0x041d0002) :
// 		INFO("PMU2: Use PDA mode to set MR%d with value 0x%02x\n",args_list[0],args_list[1]);
// 		break;
// 	case(0x041e0001) :
// 		INFO("PMU3: Written Vref with PDA to CS=0x%02x\n",args_list[0]);
// 		break;
// 	case(0x041f0000) :
// 		INFO("PMU1: start_cal: DEBUG: setting CalRun to 1\n");
// 		break;
// 	case(0x04200000) :
// 		INFO("PMU1: start_cal: DEBUG: setting CalRun to 0\n");
// 		break;
// 	case(0x04210001) :
// 		INFO("PMU1: lock_pll_dll: DEBUG: pstate = %d\n",args_list[0]);
// 		break;
// 	case(0x04220001) :
// 		INFO("PMU1: lock_pll_dll: DEBUG: dfifreqxlat_pstate = %d\n",args_list[0]);
// 		break;
// 	case(0x04230001) :
// 		INFO("PMU1: lock_pll_dll: DEBUG: pllbypass = %d\n",args_list[0]);
// 		break;
// 	case(0x04240001) :
// 		INFO("PMU3: SaveLcdlSeed: Saving seed seed %d\n",args_list[0]);
// 		break;
// 	case(0x04250000) :
// 		INFO("PMU1: in phy_defaults()\n");
// 		break;
// 	case(0x04260003) :
// 		INFO("PMU3: ACXConf:%d MaxNumDbytes:%d NumDfi:%d\n",args_list[0],args_list[1],args_list[2]);
// 		break;
// 	case(0x04270005) :
// 		INFO("PMU1: setAltAcsmCLCWL setting cl=%d cwl=%d %d %d %d \n",args_list[0],args_list[1],args_list[2],args_list[3],args_list[4]);
// 		break;
// 	default:
// 		INFO("PMU Streaming Msg: Debug message not recognized !!  code: %x", codede_message_hex);
// 		panic();
//   }
}
