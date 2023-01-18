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
extern const uint16_t ddrphy_train1d_imem_size;
extern const uint16_t ddrphy_train1d_imem[];
extern const uint16_t ddrphy_train1d_dmem_size;
extern const uint16_t ddrphy_train1d_dmem[];
extern const uint32_t ddrphy_train1d_data_f[][2];
extern const uint32_t ddrphy_train1d_data_f_size;
extern const uint32_t ddrphy_train1d_data_g1[][2];
extern const uint32_t ddrphy_train1d_data_g1_size;
extern const uint32_t ddrphy_train1d_data_g2[][2];
extern const uint32_t ddrphy_train1d_data_g2_size;
extern const uint32_t ddrphy_train1d_data_h[][2];
extern const uint32_t ddrphy_train1d_data_h_size;
extern const uint16_t ddrphy_train2d_imem_size;
extern const uint16_t ddrphy_train2d_imem[];
extern const uint16_t ddrphy_train2d_dmem_size;
extern const uint16_t ddrphy_train2d_dmem[];
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

uint16_t csr_table[SCR_SIZE];

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
static void dwc_ddrphy_phyinit_userCustom_G_waitFwDone(void);
static uint32_t get_mail(uint8_t mode_32bits);
static void DDRTOP_mc_apb_rmw (uint32_t addr, uint32_t data, uint32_t mask);
static void DDRTOP_mc_apb_poll(uint32_t addr, uint32_t data, uint32_t mask);
static void DDRTOP_mc_param_wr(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data);
static uint32_t DDRTOP_mc_param_rd(uint32_t addr, uint32_t offset, uint32_t width);
static void DDRTOP_mc_param_poll(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data);
// static void dwc_ddrphy_apb_rmw(uint32_t addr, uint32_t data, uint32_t mask);
static void dwc_ddrphy_apb_poll(uint32_t addr, uint32_t data, uint32_t mask);
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

	// param_phyinit_d_1d
	for (i = 0; i < ddrphy_train1d_data_d_size; i++)
	{
		// See "30_phyinit_param_DDR4" sheet for param_phyinit_d_1d.
		dwc_ddrphy_apb_wr(ddrphy_train1d_data_d[i][0], ddrphy_train1d_data_d[i][1]);
	}
	// 1d imem
	for (i = 0; i < (uint32_t)ddrphy_train1d_imem_size; i++)
	{
		dwc_ddrphy_apb_wr((0x060000 + i), (uint32_t)ddrphy_train1d_imem[i]);
	}
	for(; i < 0x4000; i++)
	{
		dwc_ddrphy_apb_wr((0x060000 + i), 0);
	}
	// Set DfiClk to the desired frequency for pstate 0.
	// See "30_phyinit_param_DDR4" sheet for param_phyinit_f_1d_0.
	// param_phyinit_f_1d_0
	// 1d dmem
	for (i = 0; i < (uint32_t)ddrphy_train1d_dmem_size; i++)
	{
		dwc_ddrphy_apb_wr((0x064000 + i), (uint32_t)ddrphy_train1d_dmem[i]);
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
	dwc_ddrphy_phyinit_userCustom_G_waitFwDone();
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
	// 2d imem
	for (i = 0; i < (uint32_t)ddrphy_train2d_imem_size; i++)
	{
		dwc_ddrphy_apb_wr((0x060000 + i), (uint32_t)ddrphy_train2d_imem[i]);
	}
	for(; i < 0x4000; i++)
	{
		dwc_ddrphy_apb_wr((0x060000 + i), 0);
	}
	// See "30_phyinit_param_DDR4" sheet for param_phyinit_f_2d_0.
	//param_phyinit_f_2d_0
	// 2d dmem
	for (i = 0; i < (uint32_t)ddrphy_train2d_dmem_size; i++)
	{
		dwc_ddrphy_apb_wr((0x064000 + i), (uint32_t)ddrphy_train2d_dmem[i]);
	}
	for (i = 0; i < ddrphy_train2d_data_f_size; i++)
	{
		dwc_ddrphy_apb_wr(ddrphy_train2d_data_f[i][0], ddrphy_train2d_data_f[i][1]);
	}
	for (i = 0; i < ddrphy_train2d_data_g1_size; i++)
	{
		dwc_ddrphy_apb_wr(ddrphy_train2d_data_g1[i][0], ddrphy_train2d_data_g1[i][1]);
	}
	dwc_ddrphy_phyinit_userCustom_G_waitFwDone();
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
	0x0200b2,	0x0200cb,	0x020024,	0x010043,	0x011043,	0x012043,	0x013043,	0x010143,	// 0x000
	0x011143,	0x012143,	0x013143,	0x000080,	0x001080,	0x002080,	0x003080,	0x004080,	// 0x008
	0x005080,	0x006080,	0x007080,	0x008080,	0x009080,	0x010080,	0x011080,	0x012080,	// 0x010
	0x013080,	0x010180,	0x011180,	0x012180,	0x013180,	0x010081,	0x011081,	0x012081,	// 0x018
	0x013081,	0x010181,	0x011181,	0x012181,	0x013181,	0x010082,	0x011082,	0x012082,	// 0x020
	0x013082,	0x010182,	0x011182,	0x012182,	0x013182,	0x010083,	0x011083,	0x012083,	// 0x028
	0x013083,	0x010183,	0x011183,	0x012183,	0x013183,	0x0101d0,	0x0111d0,	0x0121d0,	// 0x030
	0x0131d0,	0x0100d0,	0x0110d0,	0x0120d0,	0x0130d0,	0x0101d1,	0x0111d1,	0x0121d1,	// 0x038
	0x0131d1,	0x0100d1,	0x0110d1,	0x0120d1,	0x0130d1,	0x0101d2,	0x0111d2,	0x0121d2,	// 0x040
	0x0131d2,	0x0100d2,	0x0110d2,	0x0120d2,	0x0130d2,	0x0101d3,	0x0111d3,	0x0121d3,	// 0x048
	0x0131d3,	0x0100d3,	0x0110d3,	0x0120d3,	0x0130d3,	0x010868,	0x011868,	0x012868,	// 0x050
	0x013868,	0x010768,	0x011768,	0x012768,	0x013768,	0x010668,	0x011668,	0x012668,	// 0x058
	0x013668,	0x010568,	0x011568,	0x012568,	0x013568,	0x010468,	0x011468,	0x012468,	// 0x060
	0x013468,	0x010368,	0x011368,	0x012368,	0x013368,	0x010268,	0x011268,	0x012268,	// 0x068
	0x013268,	0x010168,	0x011168,	0x012168,	0x013168,	0x010068,	0x011068,	0x012068,	// 0x070
	0x013068,	0x010869,	0x011869,	0x012869,	0x013869,	0x010769,	0x011769,	0x012769,	// 0x078
	0x013769,	0x010669,	0x011669,	0x012669,	0x013669,	0x010569,	0x011569,	0x012569,	// 0x080
	0x013569,	0x010469,	0x011469,	0x012469,	0x013469,	0x010369,	0x011369,	0x012369,	// 0x088
	0x013369,	0x010269,	0x011269,	0x012269,	0x013269,	0x010169,	0x011169,	0x012169,	// 0x090
	0x013169,	0x010069,	0x011069,	0x012069,	0x013069,	0x01086a,	0x01186a,	0x01286a,	// 0x098
	0x01386a,	0x01076a,	0x01176a,	0x01276a,	0x01376a,	0x01066a,	0x01166a,	0x01266a,	// 0x0A0
	0x01366a,	0x01056a,	0x01156a,	0x01256a,	0x01356a,	0x01046a,	0x01146a,	0x01246a,	// 0x0A8
	0x01346a,	0x01036a,	0x01136a,	0x01236a,	0x01336a,	0x01026a,	0x01126a,	0x01226a,	// 0x0B0
	0x01326a,	0x01016a,	0x01116a,	0x01216a,	0x01316a,	0x01006a,	0x01106a,	0x01206a,	// 0x0B8
	0x01306a,	0x01086b,	0x01186b,	0x01286b,	0x01386b,	0x01076b,	0x01176b,	0x01276b,	// 0x0C0
	0x01376b,	0x01066b,	0x01166b,	0x01266b,	0x01366b,	0x01056b,	0x01156b,	0x01256b,	// 0x0C8
	0x01356b,	0x01046b,	0x01146b,	0x01246b,	0x01346b,	0x01036b,	0x01136b,	0x01236b,	// 0x0D0
	0x01336b,	0x01026b,	0x01126b,	0x01226b,	0x01326b,	0x01016b,	0x01116b,	0x01216b,	// 0x0D8
	0x01316b,	0x01006b,	0x01106b,	0x01206b,	0x01306b,	0x01008c,	0x01108c,	0x01208c,	// 0x0E0
	0x01308c,	0x01018c,	0x01118c,	0x01218c,	0x01318c,	0x01008d,	0x01108d,	0x01208d,	// 0x0E8
	0x01308d,	0x01018d,	0x01118d,	0x01218d,	0x01318d,	0x01008e,	0x01108e,	0x01208e,	// 0x0F0
	0x01308e,	0x01018e,	0x01118e,	0x01218e,	0x01318e,	0x01008f,	0x01108f,	0x01208f,	// 0x0F8
	0x01308f,	0x01018f,	0x01118f,	0x01218f,	0x01318f,	0x0108c0,	0x0118c0,	0x0128c0,	// 0x100
	0x0138c0,	0x0107c0,	0x0117c0,	0x0127c0,	0x0137c0,	0x0106c0,	0x0116c0,	0x0126c0,	// 0x108
	0x0136c0,	0x0105c0,	0x0115c0,	0x0125c0,	0x0135c0,	0x0104c0,	0x0114c0,	0x0124c0,	// 0x110
	0x0134c0,	0x0103c0,	0x0113c0,	0x0123c0,	0x0133c0,	0x0102c0,	0x0112c0,	0x0122c0,	// 0x118
	0x0132c0,	0x0101c0,	0x0111c0,	0x0121c0,	0x0131c0,	0x0100c0,	0x0110c0,	0x0120c0,	// 0x120
	0x0130c0,	0x0108c1,	0x0118c1,	0x0128c1,	0x0138c1,	0x0107c1,	0x0117c1,	0x0127c1,	// 0x128
	0x0137c1,	0x0106c1,	0x0116c1,	0x0126c1,	0x0136c1,	0x0105c1,	0x0115c1,	0x0125c1,	// 0x130
	0x0135c1,	0x0104c1,	0x0114c1,	0x0124c1,	0x0134c1,	0x0103c1,	0x0113c1,	0x0123c1,	// 0x138
	0x0133c1,	0x0102c1,	0x0112c1,	0x0122c1,	0x0132c1,	0x0101c1,	0x0111c1,	0x0121c1,	// 0x140
	0x0131c1,	0x0100c1,	0x0110c1,	0x0120c1,	0x0130c1,	0x0108c2,	0x0118c2,	0x0128c2,	// 0x148
	0x0138c2,	0x0107c2,	0x0117c2,	0x0127c2,	0x0137c2,	0x0106c2,	0x0116c2,	0x0126c2,	// 0x150
	0x0136c2,	0x0105c2,	0x0115c2,	0x0125c2,	0x0135c2,	0x0104c2,	0x0114c2,	0x0124c2,	// 0x158
	0x0134c2,	0x0103c2,	0x0113c2,	0x0123c2,	0x0133c2,	0x0102c2,	0x0112c2,	0x0122c2,	// 0x160
	0x0132c2,	0x0101c2,	0x0111c2,	0x0121c2,	0x0131c2,	0x0100c2,	0x0110c2,	0x0120c2,	// 0x168
	0x0130c2,	0x0108c3,	0x0118c3,	0x0128c3,	0x0138c3,	0x0107c3,	0x0117c3,	0x0127c3,	// 0x170
	0x0137c3,	0x0106c3,	0x0116c3,	0x0126c3,	0x0136c3,	0x0105c3,	0x0115c3,	0x0125c3,	// 0x178
	0x0135c3,	0x0104c3,	0x0114c3,	0x0124c3,	0x0134c3,	0x0103c3,	0x0113c3,	0x0123c3,	// 0x180
	0x0133c3,	0x0102c3,	0x0112c3,	0x0122c3,	0x0132c3,	0x0101c3,	0x0111c3,	0x0121c3,	// 0x188
	0x0131c3,	0x0100c3,	0x0110c3,	0x0120c3,	0x0130c3,	0x010020,	0x011020,	0x012020,	// 0x190
	0x013020,	0x02007c,	0x02007d,																// 0x198
};
// 2D Training IO Retention Register List
static uint32_t	reg_list_2d[] =
{
	0x01008c,	0x01108c,	0x01208c,	0x01308c,	0x01018c,	0x01118c,	0x01218c,	0x01318c,	// 0x200
	0x01008d,	0x01108d,	0x01208d,	0x01308d,	0x01018d,	0x01118d,	0x01218d,	0x01318d,	// 0x208
	0x01008e,	0x01108e,	0x01208e,	0x01308e,	0x01018e,	0x01118e,	0x01218e,	0x01318e,	// 0x210
	0x01008f,	0x01108f,	0x01208f,	0x01308f,	0x01018f,	0x01118f,	0x01218f,	0x01318f,	// 0x218
	0x010840,	0x011840,	0x012840,	0x013840,	0x010740,	0x011740,	0x012740,	0x013740,	// 0x220
	0x010640,	0x011640,	0x012640,	0x013640,	0x010540,	0x011540,	0x012540,	0x013540,	// 0x228
	0x010440,	0x011440,	0x012440,	0x013440,	0x010340,	0x011340,	0x012340,	0x013340,	// 0x230
	0x010240,	0x011240,	0x012240,	0x013240,	0x010140,	0x011140,	0x012140,	0x013140,	// 0x238
	0x010040,	0x011040,	0x012040,	0x013040,	0x010830,	0x011830,	0x012830,	0x013830,	// 0x240
	0x010730,	0x011730,	0x012730,	0x013730,	0x010630,	0x011630,	0x012630,	0x013630,	// 0x248
	0x010530,	0x011530,	0x012530,	0x013530,	0x010430,	0x011430,	0x012430,	0x013430,	// 0x250
	0x010330,	0x011330,	0x012330,	0x013330,	0x010230,	0x011230,	0x012230,	0x013230,	// 0x258
	0x010130,	0x011130,	0x012130,	0x013130,	0x010030,	0x011030,	0x012030,	0x013030,	// 0x260
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
	for(i = 0; i < SCR_SIZE; i += 4)
	{
		csr_table[i] = 0xffff;
	}
	for(i = 0; i < ARRAY_SIZE(reg_list_1d); i++)
	{
		csr_table[START_LIST_1D + i] = 	dwc_ddrphy_apb_rd(reg_list_1d[i]);
	}
	for(i = 0; i < ARRAY_SIZE(reg_list_2d); i++)
	{
		csr_table[START_LIST_2D + i] = 	dwc_ddrphy_apb_rd(reg_list_2d[i]);
	}
	for(i = 0; i < ARRAY_SIZE(reg_list_ctl); i++)
	{
		csr_table[START_LIST_CTL + i] = 	DDRTOP_mc_apb_rd(reg_list_ctl[i]);
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
		dwc_ddrphy_apb_wr(reg_list_1d[i], csr_table[START_LIST_1D + i]);
	}
	for(i = 0; i < ARRAY_SIZE(reg_list_2d); i++)
	{
		dwc_ddrphy_apb_wr(reg_list_2d[i], csr_table[START_LIST_2D + i]);
	}
	for(i = 0; i < ARRAY_SIZE(reg_list_ctl); i++)
	{
		DDRTOP_mc_apb_wr(reg_list_ctl[i], csr_table[START_LIST_CTL + i]);
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
static void dwc_ddrphy_phyinit_userCustom_G_waitFwDone(void)
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
