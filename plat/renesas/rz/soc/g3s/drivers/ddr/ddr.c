/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <drivers/delay_timer.h>
#include <common/debug.h>
#include <platform_def.h>
#include <cpg.h>
#include <ddr.h>

extern const char ddr_an_version[];
extern const uint32_t ddrmc_ini_dat[][2];
extern const uint32_t ddrmc_ini_dat_size;
extern const uint32_t ddrphy_ini_data_c[][2];
extern const uint32_t ddrphy_ini_data_c_size;
extern const uint32_t ddrphy_ini_swizzle[][2];
extern const uint32_t ddrphy_ini_swizzle_size;
extern const uint32_t ddrphy_train1d_data_d[][2];
extern const uint32_t ddrphy_train1d_data_d_size;
extern const uint16_t ddrphy_train1d_dat0_size;
extern const uint16_t ddrphy_train1d_dat0[];
extern const uint16_t ddrphy_train1d_dat1_size;
extern const uint16_t ddrphy_train1d_dat1[];
extern const uint32_t ddrphy_train1d_data_f[][2];
extern const uint32_t ddrphy_train1d_data_f_size;
extern const uint32_t ddrphy_train1d_data_g1[][2];
extern const uint32_t ddrphy_train1d_data_g1_size;
extern const uint32_t ddrphy_train1d_data_g2[][2];
extern const uint32_t ddrphy_train1d_data_g2_size;
extern const uint32_t ddrphy_train1d_data_h[][2];
extern const uint32_t ddrphy_train1d_data_h_size;
extern const uint16_t ddrphy_train2d_dat0_size;
extern const uint16_t ddrphy_train2d_dat0[];
extern const uint16_t ddrphy_train2d_dat1_size;
extern const uint16_t ddrphy_train2d_dat1[];
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

#define	DDR_PHY_BASE	((uintptr_t)RZ_SOC_DDR_PHY_BASE)
#define	DDR_MC_BASE		((uintptr_t)RZ_SOC_DDR_MC_BASE)

#define MCAR_CTL		0x800
#define MCAR_MON		0x801
#define	MCAR_SRCTL0		0x814 // TODO:Check

#define	INT_STATUS_INIT_ADDR		370
#define	INT_STATUS_INIT_OFFSET		24
#define	INT_STATUS_INIT_WIDTH		8
#define	LPI_WAKEUP_EN_ADDR			192
#define	LPI_WAKEUP_EN_OFFSET		0
#define	LPI_WAKEUP_EN_WIDTH			6

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
#define	LP_STATE_ADDR				118
#define	LP_STATE_OFFSET				24
#define	LP_STATE_WIDTH				7
#define	MEM_DP_REDUCTION_ADDR		200
#define	MEM_DP_REDUCTION_OFFSET		24
#define	MEM_DP_REDUCTION_WIDTH		1
#define	CONTROLLER_BUSY_ADDR		205
#define	CONTROLLER_BUSY_OFFSET		16
#define	CONTROLLER_BUSY_WIDTH		1
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
// 12_retention_entry
// 13_retention_exit

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
static void dwc_ddrphy_phyinit_userCustom_G_waitDone(void);
static uint32_t get_mail(uint8_t mode_32bits);
// static void DDRTOP_mc_apb_rmw (uint32_t addr, uint32_t data, uint32_t mask);
// void DDRTOP_mc_apb_poll(uint32_t addr, uint32_t data, uint32_t mask);
void DDRTOP_mc_param_wr(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data);
uint32_t DDRTOP_mc_param_rd(uint32_t addr, uint32_t offset, uint32_t width);
void DDRTOP_mc_param_poll(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data);
// static void dwc_ddrphy_apb_rmw(uint32_t addr, uint32_t data, uint32_t mask);
// void dwc_ddrphy_apb_poll(uint32_t addr, uint32_t data, uint32_t mask);
static void decode_major_message (uint32_t mail);
static void decode_streaming_message (void);
void decode_streaming_message_dec (uint32_t codede_message_hex, uint16_t *args_list);

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
// 13_retention_exit
void ddr_retention_exit(void)
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

	// param_phyinit_d_1d
	for (i = 0; i < ddrphy_train1d_data_d_size; i++)
	{
		// See "30_phyinit_param_DDR4" sheet for param_phyinit_d_1d.
		dwc_ddrphy_apb_wr(ddrphy_train1d_data_d[i][0], ddrphy_train1d_data_d[i][1]);
	}
	// 1d dat0
	for (i = 0; i < (uint32_t)ddrphy_train1d_dat0_size; i++)
	{
		dwc_ddrphy_apb_wr((0x060000 + i), (uint32_t)ddrphy_train1d_dat0[i]);
	}
	for(; i < 0x4000; i++)
	{
		dwc_ddrphy_apb_wr((0x060000 + i), 0);
	}
	// Set DfiClk to the desired frequency for pstate 0.
	// See "30_phyinit_param_DDR4" sheet for param_phyinit_f_1d_0.
	// param_phyinit_f_1d_0
	// 1d dat1
	for (i = 0; i < (uint32_t)ddrphy_train1d_dat1_size; i++)
	{
		dwc_ddrphy_apb_wr((0x064000 + i), (uint32_t)ddrphy_train1d_dat1[i]);
	}
	for (i = 0; i < ddrphy_train1d_data_f_size; i++)
	{
		dwc_ddrphy_apb_wr(ddrphy_train1d_data_f[i][0], ddrphy_train1d_data_f[i][1]);
	}
//	val = DDRTOP_mc_param_rd(DRAM_CLASS_ADDR, DRAM_CLASS_OFFSET, DRAM_CLASS_WIDTH);
	for (i = 0; i < ddrphy_train1d_data_g1_size; i++)
	{
		dwc_ddrphy_apb_wr(ddrphy_train1d_data_g1[i][0], ddrphy_train1d_data_g1[i][1]);
	}
	dwc_ddrphy_phyinit_userCustom_G_waitDone();
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
	// 2d dat0
	for (i = 0; i < (uint32_t)ddrphy_train2d_dat0_size; i++)
	{
		dwc_ddrphy_apb_wr((0x060000 + i), (uint32_t)ddrphy_train2d_dat0[i]);
	}
	for(; i < 0x4000; i++)
	{
		dwc_ddrphy_apb_wr((0x060000 + i), 0);
	}
	// See "30_phyinit_param_DDR4" sheet for param_phyinit_f_2d_0.
	//param_phyinit_f_2d_0
	// 2d dat1
	for (i = 0; i < (uint32_t)ddrphy_train2d_dat1_size; i++)
	{
		dwc_ddrphy_apb_wr((0x064000 + i), (uint32_t)ddrphy_train2d_dat1[i]);
	}
	for (i = 0; i < ddrphy_train2d_data_f_size; i++)
	{
		dwc_ddrphy_apb_wr(ddrphy_train2d_data_f[i][0], ddrphy_train2d_data_f[i][1]);
	}
	for (i = 0; i < ddrphy_train2d_data_g1_size; i++)
	{
		dwc_ddrphy_apb_wr(ddrphy_train2d_data_g1[i][0], ddrphy_train2d_data_g1[i][1]);
	}
	dwc_ddrphy_phyinit_userCustom_G_waitDone();
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
	dwc_ddrphy_apb_wr(0x06E000, 0); // Write the MicroContMuxSel CSR to 0x0 to allow access to the internal CSRs.
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
	dwc_ddrphy_apb_wr(0x06E000, 1);	// Write the MicroContMuxSel CSR to 0x1 to isolate the internal CSRs during mission mode.
}

/////////////////////////////////////////////////////////////////////////////////
// 41_save_rst_csr

// Register offset address	Filter
// 1D Training IO Retention Register List
static uint32_t	reg_list_1d[] =
{
	0x0580b2, 0x0580cb, 0x058024, 0x030043, 0x031043, 0x032043, 0x033043, 0x030143, 
	0x031143, 0x032143, 0x033143, 0x000080, 0x001080, 0x002080, 0x003080, 0x004080, 
	0x005080, 0x006080, 0x007080, 0x008080, 0x009080, 0x030080, 0x031080, 0x032080, 
	0x033080, 0x030180, 0x031180, 0x032180, 0x033180, 0x030081, 0x031081, 0x032081, 
	0x033081, 0x030181, 0x031181, 0x032181, 0x033181, 0x030082, 0x031082, 0x032082, 
	0x033082, 0x030182, 0x031182, 0x032182, 0x033182, 0x030083, 0x031083, 0x032083, 
	0x033083, 0x030183, 0x031183, 0x032183, 0x033183, 0x0301d0, 0x0311d0, 0x0321d0, 
	0x0331d0, 0x0300d0, 0x0310d0, 0x0320d0, 0x0330d0, 0x0301d1, 0x0311d1, 0x0321d1, 
	0x0331d1, 0x0300d1, 0x0310d1, 0x0320d1, 0x0330d1, 0x0301d2, 0x0311d2, 0x0321d2, 
	0x0331d2, 0x0300d2, 0x0310d2, 0x0320d2, 0x0330d2, 0x0301d3, 0x0311d3, 0x0321d3, 
	0x0331d3, 0x0300d3, 0x0310d3, 0x0320d3, 0x0330d3, 0x030868, 0x031868, 0x032868, 
	0x033868, 0x030768, 0x031768, 0x032768, 0x033768, 0x030668, 0x031668, 0x032668, 
	0x033668, 0x030568, 0x031568, 0x032568, 0x033568, 0x030468, 0x031468, 0x032468, 
	0x033468, 0x030368, 0x031368, 0x032368, 0x033368, 0x030268, 0x031268, 0x032268, 
	0x033268, 0x030168, 0x031168, 0x032168, 0x033168, 0x030068, 0x031068, 0x032068, 
	0x033068, 0x030869, 0x031869, 0x032869, 0x033869, 0x030769, 0x031769, 0x032769, 
	0x033769, 0x030669, 0x031669, 0x032669, 0x033669, 0x030569, 0x031569, 0x032569, 
	0x033569, 0x030469, 0x031469, 0x032469, 0x033469, 0x030369, 0x031369, 0x032369, 
	0x033369, 0x030269, 0x031269, 0x032269, 0x033269, 0x030169, 0x031169, 0x032169, 
	0x033169, 0x030069, 0x031069, 0x032069, 0x033069, 0x03086a, 0x03186a, 0x03286a, 
	0x03386a, 0x03076a, 0x03176a, 0x03276a, 0x03376a, 0x03066a, 0x03166a, 0x03266a, 
	0x03366a, 0x03056a, 0x03156a, 0x03256a, 0x03356a, 0x03046a, 0x03146a, 0x03246a, 
	0x03346a, 0x03036a, 0x03136a, 0x03236a, 0x03336a, 0x03026a, 0x03126a, 0x03226a, 
	0x03326a, 0x03016a, 0x03116a, 0x03216a, 0x03316a, 0x03006a, 0x03106a, 0x03206a, 
	0x03306a, 0x03086b, 0x03186b, 0x03286b, 0x03386b, 0x03076b, 0x03176b, 0x03276b, 
	0x03376b, 0x03066b, 0x03166b, 0x03266b, 0x03366b, 0x03056b, 0x03156b, 0x03256b, 
	0x03356b, 0x03046b, 0x03146b, 0x03246b, 0x03346b, 0x03036b, 0x03136b, 0x03236b, 
	0x03336b, 0x03026b, 0x03126b, 0x03226b, 0x03326b, 0x03016b, 0x03116b, 0x03216b, 
	0x03316b, 0x03006b, 0x03106b, 0x03206b, 0x03306b, 0x03008c, 0x03108c, 0x03208c, 
	0x03308c, 0x03018c, 0x03118c, 0x03218c, 0x03318c, 0x03008d, 0x03108d, 0x03208d, 
	0x03308d, 0x03018d, 0x03118d, 0x03218d, 0x03318d, 0x03008e, 0x03108e, 0x03208e, 
	0x03308e, 0x03018e, 0x03118e, 0x03218e, 0x03318e, 0x03008f, 0x03108f, 0x03208f, 
	0x03308f, 0x03018f, 0x03118f, 0x03218f, 0x03318f, 0x0308c0, 0x0318c0, 0x0328c0, 
	0x0338c0, 0x0307c0, 0x0317c0, 0x0327c0, 0x0337c0, 0x0306c0, 0x0316c0, 0x0326c0, 
	0x0336c0, 0x0305c0, 0x0315c0, 0x0325c0, 0x0335c0, 0x0304c0, 0x0314c0, 0x0324c0, 
	0x0334c0, 0x0303c0, 0x0313c0, 0x0323c0, 0x0333c0, 0x0302c0, 0x0312c0, 0x0322c0, 
	0x0332c0, 0x0301c0, 0x0311c0, 0x0321c0, 0x0331c0, 0x0300c0, 0x0310c0, 0x0320c0, 
	0x0330c0, 0x0308c1, 0x0318c1, 0x0328c1, 0x0338c1, 0x0307c1, 0x0317c1, 0x0327c1, 
	0x0337c1, 0x0306c1, 0x0316c1, 0x0326c1, 0x0336c1, 0x0305c1, 0x0315c1, 0x0325c1, 
	0x0335c1, 0x0304c1, 0x0314c1, 0x0324c1, 0x0334c1, 0x0303c1, 0x0313c1, 0x0323c1, 
	0x0333c1, 0x0302c1, 0x0312c1, 0x0322c1, 0x0332c1, 0x0301c1, 0x0311c1, 0x0321c1, 
	0x0331c1, 0x0300c1, 0x0310c1, 0x0320c1, 0x0330c1, 0x0308c2, 0x0318c2, 0x0328c2, 
	0x0338c2, 0x0307c2, 0x0317c2, 0x0327c2, 0x0337c2, 0x0306c2, 0x0316c2, 0x0326c2, 
	0x0336c2, 0x0305c2, 0x0315c2, 0x0325c2, 0x0335c2, 0x0304c2, 0x0314c2, 0x0324c2, 
	0x0334c2, 0x0303c2, 0x0313c2, 0x0323c2, 0x0333c2, 0x0302c2, 0x0312c2, 0x0322c2, 
	0x0332c2, 0x0301c2, 0x0311c2, 0x0321c2, 0x0331c2, 0x0300c2, 0x0310c2, 0x0320c2, 
	0x0330c2, 0x0308c3, 0x0318c3, 0x0328c3, 0x0338c3, 0x0307c3, 0x0317c3, 0x0327c3, 
	0x0337c3, 0x0306c3, 0x0316c3, 0x0326c3, 0x0336c3, 0x0305c3, 0x0315c3, 0x0325c3, 
	0x0335c3, 0x0304c3, 0x0314c3, 0x0324c3, 0x0334c3, 0x0303c3, 0x0313c3, 0x0323c3, 
	0x0333c3, 0x0302c3, 0x0312c3, 0x0322c3, 0x0332c3, 0x0301c3, 0x0311c3, 0x0321c3, 
	0x0331c3, 0x0300c3, 0x0310c3, 0x0320c3, 0x0330c3, 0x030020, 0x031020, 0x032020, 
	0x033020, 0x05807c, 0x05807d,
};
// 2D Training IO Retention Register List
static uint32_t	reg_list_2d[] =
{
	0x03008c, 0x03108c, 0x03208c, 0x03308c, 0x03018c, 0x03118c, 0x03218c, 0x03318c, 
	0x03008d, 0x03108d, 0x03208d, 0x03308d, 0x03018d, 0x03118d, 0x03218d, 0x03318d, 
	0x03008e, 0x03108e, 0x03208e, 0x03308e, 0x03018e, 0x03118e, 0x03218e, 0x03318e, 
	0x03008f, 0x03108f, 0x03208f, 0x03308f, 0x03018f, 0x03118f, 0x03218f, 0x03318f, 
	0x030840, 0x031840, 0x032840, 0x033840, 0x030740, 0x031740, 0x032740, 0x033740, 
	0x030640, 0x031640, 0x032640, 0x033640, 0x030540, 0x031540, 0x032540, 0x033540, 
	0x030440, 0x031440, 0x032440, 0x033440, 0x030340, 0x031340, 0x032340, 0x033340, 
	0x030240, 0x031240, 0x032240, 0x033240, 0x030140, 0x031140, 0x032140, 0x033140, 
	0x030040, 0x031040, 0x032040, 0x033040, 0x030830, 0x031830, 0x032830, 0x033830, 
	0x030730, 0x031730, 0x032730, 0x033730, 0x030630, 0x031630, 0x032630, 0x033630, 
	0x030530, 0x031530, 0x032530, 0x033530, 0x030430, 0x031430, 0x032430, 0x033430, 
	0x030330, 0x031330, 0x032330, 0x033330, 0x030230, 0x031230, 0x032230, 0x033230, 
	0x030130, 0x031130, 0x032130, 0x033130, 0x030030, 0x031030, 0x032030, 0x033030, 
};
//,	Controller Register List
static uint32_t	reg_list_ctl[] =
{
	541,	570,																					// 0x300
};

static void	save_rst_csr(void)
{
	uint32_t i;
	dwc_ddrphy_apb_wr(0x06E000, 0); // Write the MicroContMuxSel CSR to 0x0 to allow access to the internal CSRs.
	dwc_ddrphy_apb_wr(0x066080, 3); // Write the UcclkHclkEnables CSR to 0x3 to enable all the clocks so the reads can complete.
	// The retention registers are listed in Table 41-1.
	// Read all the retention registers, and save them to the storage other than DRAM.
	for(i = 0; i < ARRAY_SIZE(ddr_csr_table); i ++)
	{
		ddr_csr_table[i] = 0xffff;
	}
	for(i = 0; i < ARRAY_SIZE(reg_list_1d); i++)
	{
		ddr_csr_table[START_LIST_1D + i] = 	dwc_ddrphy_apb_rd(reg_list_1d[i]);
	}
	for(i = 0; i < ARRAY_SIZE(reg_list_2d); i++)
	{
		ddr_csr_table[START_LIST_2D + i] = 	dwc_ddrphy_apb_rd(reg_list_2d[i]);
	}
	for(i = 0; i < ARRAY_SIZE(reg_list_ctl); i++)
	{
		ddr_csr_table[START_LIST_CTL + i] = 	DDRTOP_mc_apb_rd(reg_list_ctl[i]);
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
	for(i = 0; i < ARRAY_SIZE(reg_list_1d); i++)
	{
		dwc_ddrphy_apb_wr(reg_list_1d[i], ddr_csr_table[START_LIST_1D + i]);
	}
	for(i = 0; i < ARRAY_SIZE(reg_list_2d); i++)
	{
		dwc_ddrphy_apb_wr(reg_list_2d[i], ddr_csr_table[START_LIST_2D + i]);
	}
	for(i = 0; i < ARRAY_SIZE(reg_list_ctl); i++)
	{
		DDRTOP_mc_apb_wr(reg_list_ctl[i], ddr_csr_table[START_LIST_CTL + i]);
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
static void dwc_ddrphy_phyinit_userCustom_G_waitDone(void)
{
	uint32_t mail;

	mail = 0;
	// Wait at least 10 DfiClk cycles.
	do {
		// Wait at least 500 cycles.
		udelay(1000);
		mail = get_mail(0);
		// for debug
		decode_major_message(mail);
		if (mail == 0x08) {
		 	decode_streaming_message();
		}
#ifdef DDR_DEBUG
		INFO("INFO: Mail Box = %x\r", mail);
#endif
	}	while ((mail != 0xff) && (mail != 0x07));
	INFO("\n");
	if (mail == 0xff) {
		ERROR("Error: Training failed.\n");
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
		//INFO("INFO: Msg read. Waiting acknowledgement from uCtl ... rd_data = 0x%x\n", rd_data);
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

uint32_t DDRTOP_mc_param_rd(uint32_t addr, uint32_t offset, uint32_t width)
{
	uint32_t tmp_data;
	uint32_t tmp_mask;
	tmp_data = DDRTOP_mc_apb_rd(addr);
	tmp_mask = ((1 << width) - 1) << offset;
	return (tmp_data & tmp_mask) >> offset;
}

static void decode_major_message (uint32_t mail)
{
#ifdef DDR_DEBUG
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
		INFO("PMU Major Msg: Training run has completed                                    \n");
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
#endif
}
static void decode_streaming_message (void)
{
	uint32_t codede_message_hex;
	uint16_t num_args;
	uint16_t args_list[31];
	int i;

	codede_message_hex = get_mail(1);
	// Get the number of argument need to be read from mailbox
	num_args = 0xFFFF & codede_message_hex;
	for (i = 0; i < num_args; i++) {
		args_list[i] = get_mail(1);
	}
#ifdef DDR_DEBUG
	INFO("Encode Message (0x%x)\n", codede_message_hex);
#endif
	for (i = 0; i < num_args; i++) {
#ifdef DDR_DEBUG
		INFO("num_args %d: 0x%x\n", i, args_list[i]);
#endif
	}
	decode_streaming_message_dec(codede_message_hex, args_list);
}
