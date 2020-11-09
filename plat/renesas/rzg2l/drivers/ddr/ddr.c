/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <lib/mmio.h>
#include <drivers/delay_timer.h>
#include <common/debug.h>

#include "ddr_mc_regs.h"
#include "ddr_phy_regs.h"
#include "cpg.h"

#define SIZEOF(array)   (sizeof(array)/sizeof(array[0]))
#define CEIL(a, div)	(((a) + ((div) - 1)) / (div))
#define _MIN(a, b)		((a) < (b) ? (a) : (b))
#define _MAX(a, b)		((a) > (b) ? (a) : (b))

#define DDR_TYPE_C_010_D4_01_1

#ifdef DDR_TYPE_C_010_D4_01_1
#include "param_mc_C-010_D4-01-1.c"
#include "param_swizzle_C-xx0_D4.c"
#endif

#ifdef DDR_C_111
#define SCL_LANES			(1U)
#define BYTE_LANES			(1U)
#else
#define SCL_LANES			(3U)
#define BYTE_LANES			(2U)
#endif

#define MAX_BEST_VREF_SAVED	(30U)
#define VREF_SETP			(1U)

static uintptr_t mc_reg;
static uintptr_t phy_reg;

static uint32_t read_mc_reg(uint32_t offset)
{
	return mmio_read_32(mc_reg + offset);
}

static void write_mc_reg(uint32_t offset, uint32_t val)
{
	mmio_write_32(mc_reg + offset, val);
}

static void rmw_mc_reg(uint32_t offset, uint32_t mask, uint32_t val)
{
	write_mc_reg(offset, (read_mc_reg(offset) & mask) | val);
}

static uint32_t read_phy_reg(uint32_t offset)
{
	return mmio_read_32(phy_reg + offset);
}

static void write_phy_reg(uint32_t offset, uint32_t val)
{
	mmio_write_32(phy_reg + offset, val);
}

static void rmw_phy_reg(uint32_t offset, uint32_t mask, uint32_t val)
{
	write_phy_reg(offset, (read_phy_reg(offset) & mask) | val);
}

static void disable_phy_clk(void)
{
	// Initialization Step9
	rmw_phy_reg(PHY_CTRL0, 0xFFFFF0FF, 0x00000000);
}

static void program_mc1(void)
{
	int i;

	// Step1
	for(i=0; i<SIZEOF(mc_init_tbl); i++){
		write_mc_reg(mc_init_tbl[i][0], mc_init_tbl[i][1]);
	}

	// Step2
	rmw_mc_reg(DENALI_CTL_176, 0xFCFFFFFF, mc_odt_pins_tbl[0]);
	rmw_mc_reg(DENALI_CTL_177, 0xFFFFFCFF, mc_odt_pins_tbl[1]);
	rmw_mc_reg(DENALI_CTL_176, 0xFFFCFFFF, mc_odt_pins_tbl[2]);
	rmw_mc_reg(DENALI_CTL_177, 0xFFFFFFFC, mc_odt_pins_tbl[3]);

	// Step3
	rmw_mc_reg(DENALI_CTL_70, mc_mr1_tbl[0], mc_mr1_tbl[1]);
	rmw_mc_reg(DENALI_CTL_73, mc_mr1_tbl[0], mc_mr1_tbl[1]);

	// Step4
	rmw_mc_reg(DENALI_CTL_71, mc_mr2_tbl[0], mc_mr2_tbl[1]);
	rmw_mc_reg(DENALI_CTL_74, mc_mr2_tbl[0], mc_mr2_tbl[1]);

	// Step5
	rmw_mc_reg(DENALI_CTL_81, mc_mr5_tbl[0], mc_mr5_tbl[1]);
	rmw_mc_reg(DENALI_CTL_82, mc_mr5_tbl[0], mc_mr5_tbl[1]);
	
	// Step6
	rmw_mc_reg(DENALI_CTL_83, mc_mr6_tbl[0], mc_mr6_tbl[1]);
	rmw_mc_reg(DENALI_CTL_84, mc_mr6_tbl[0], mc_mr6_tbl[1]);

	// Step7
	for(i=0; i<SIZEOF(mc_phy_settings_tbl); i++){
		write_mc_reg(mc_phy_settings_tbl[i][0], mc_phy_settings_tbl[i][1]);
	}
	
	// Step8 is skipped because ECC is unused.
}

static void program_mc2(void)
{
	uint32_t tphy_rdlat;

	// Step1
	tphy_rdlat = (((read_phy_reg(SCL_LATENCY) >> 4) & 0xF) + 1 + 1) * 2;

	// Step2
	rmw_mc_reg(DENALI_CTL_391, 0xFFFFFF80, tphy_rdlat & 0x7F);
}

static void program_phy1(void)
{
	uint16_t tCK;
	uint8_t dram, runDABCW;
	uint8_t odt_wr_map_cs0, odt_rd_map_cs0;
	uint8_t CL, CWL, AL, PL, RL, WL;
	uint32_t mr1, mr1_wl, mr1_wl_mask;
	uint32_t mr2, mr2_wl, mr2_wl_mask;
	uint8_t clk_drive, dq_dqs_drive, dq_dqs_term, vref_value, vref_ca_value;
	uint8_t read_lat, trim_lat;
	uint32_t tmp;
	int i;

	// Step1
	tmp = read_mc_reg(DENALI_CTL_413);
	tCK = tmp & 0xFFFF;
	dram = (tmp >> 16) & 0xF;
	runDABCW = (tmp >> 23) & 0x1;

	tmp = read_mc_reg(DENALI_CTL_176);
	odt_wr_map_cs0 = (tmp >> 24) & 0x3;
	odt_rd_map_cs0 = (tmp >> 16) & 0x3;

	// Step2
	tmp = read_mc_reg(DENALI_CTL_14);
	CL = (tmp >> 17) & 0x1F;
	CWL = (tmp >> 24) & 0x1F;

	tmp = read_mc_reg(DENALI_CTL_15);
	AL = tmp & 0x1F;
	PL = (tmp >> 8) & 0xF;

	RL = CL + AL + PL;
	WL = CWL + AL + PL;

	// Step3
	mr1 = read_mc_reg(DENALI_CTL_70) & 0xFFFF;
	mr2 = read_mc_reg(DENALI_CTL_71) & 0xFFFF;
	if(dram == 2){
		// DDR4
		mr1_wl_mask = (0x7 << 8) | (0x1 << 7);
		mr2_wl_mask = 0x7 << 9;
		switch((mr2 & mr2_wl_mask) >> 9){
		case 0:
			mr1_wl = 0x0;
			mr1_wl_mask = 0x0000;
			break;
		case 1:
			mr1_wl = 0x2 << 8;
			break;
		case 2:
			mr1_wl = 0x4 << 8;
			break;
		case 4:
			mr1_wl = 0x6 << 8;
			break;
		default:
			panic();
		}
	}
	else{
		// DDR3L, DDR3
		mr1_wl_mask = (0x1 << 9) | (0x1 << 7) | (0x1 << 6) | (0x1 << 2);
		mr2_wl_mask = 0x3 << 9;
		switch((mr2 & mr2_wl_mask) >> 9){
		case 0:
			mr1_wl = 0x0;
			mr1_wl_mask = 0x0000;
			break;
		case 1:
			mr1_wl = (0x0 << 9) | (0x0 << 6) | (0x1 << 2);
			break;
		case 2:
			mr1_wl = (0x0 << 9) | (0x1 << 6) | (0x1 << 2);
			break;
		default:
			panic();
		}
	}
	mr1_wl |= (mr1 & (0xFFFF ^ mr1_wl_mask)) | (0x1 << 7);
	mr2_wl = (mr2 & (0xFFFF ^ mr2_wl_mask)) | (0x0 << 9);

	// Step4
	tmp = read_mc_reg(DENALI_CTL_414);
	clk_drive = tmp & 0xF;
	dq_dqs_drive = (tmp >> 4) & 0xF;
	dq_dqs_term = (tmp >> 8) & 0xF;
	vref_value = (tmp >> 16) & 0xFF;
	vref_ca_value = (tmp >> 24) & 0xFF;
	read_lat = (dram == 2 ? 12 : 10) + (CEIL(RL, 2) * 2) - CEIL(WL, 2) + 28;
	trim_lat = (dram == 2 ? 11 : 9) + CEIL(RL, 2) - CEIL(WL, 2) + 29;

	// Step5
	tmp = ((WL == 5 ? 0x1 : 0x0) <<16) | 0x00100000;
	write_phy_reg(PHY_MODE0, tmp);

	// Step6
	write_phy_reg(DLLM_WINDOW_SIZE, 0x00000006);

	// Step7
	if(dram == 2){
		// DDR4
		write_phy_reg(DDR4_CONFIG_1, 0x00000009);
	}

	// Step8
	tmp = (dram == 2 ? 1 : 0) << 27;
	write_phy_reg(UNIQUIFY_IO_3, tmp);

	// Step9
	tmp = ((dram == 0 ? 0 : 1) << 15) | ((tCK < 1000 ? 1: 0) << 8) | 0x10004000;
	write_phy_reg(PHY_PAD_CTRL, tmp);

	// Step10
	write_phy_reg(PHY_PAD_CTRL_1,
		clk_drive | (clk_drive << 4) | (clk_drive << 8) | (clk_drive<<12) |
		(dq_dqs_drive << 16) | (dq_dqs_drive <<20));

	// Step11
	write_phy_reg(PHY_PAD_CTRL_2, dq_dqs_term | (dq_dqs_term << 4));

	// Step12
	write_phy_reg(PHY_PAD_CTRL_3, 0x00000000);

	// Step13
	for(i=0; i<BYTE_LANES; i++){
		write_phy_reg(PHY_LANE_SEL, 7 * i);
		write_phy_reg(VREF_TRAINING, (vref_value << 4) | 0x00000004);
	}

	// Step14
	write_phy_reg(VREF_CA_TRAINING, vref_ca_value);

	// Step15
	write_phy_reg(SCL_WINDOW_TRIM, 0x1A09002D);

	// Step16
	write_phy_reg(UNQ_ANALOG_DLL_1, 0x00000000);

	// Step17
	write_phy_reg(DYNAMIC_IE_TIMER, 0x0000001A);

	// Step18
	tmp = ((dram == 2 ? 0 : 1) << 2) | (CEIL(CL, 2) << 4) | (CEIL(AL, 2) << 8) |
			(odt_rd_map_cs0 << 16) | (odt_wr_map_cs0 << 24) | 0x00000001;
	write_phy_reg(SCL_CONFIG_1, tmp);

	// Step19
	tmp = (CEIL(CWL, 2) << 8) | ((((WL % 2) == 0) ? 0 : 1) << 24) | 0x80000001;
	write_phy_reg(SCL_CONFIG_2, tmp);

	// Step20
	write_phy_reg(SCL_CONFIG_3, SCL_LANES ^ 0x3);

	// Step21
	write_phy_reg(DYNAMIC_WRITE_BIT_LVL, 
		runDABCW | (trim_lat << 4) | (read_lat << 12));

	// Step22
	tmp = ((WL % 2) == 0) & 0x1;
	write_phy_reg(SCL_CONFIG_4, tmp);

	// Step23
	write_phy_reg(SCL_GATE_TIMING, 0x00000170);

	// Step24
	write_phy_reg(WRLVL_DYN_ODT, mr2_wl | (mr2 << 16));

	// Step25
	write_phy_reg(WRLVL_ON_OFF, mr1 | (mr1_wl << 16));

	// Step26
	write_phy_reg(PHY_DLL_RECALIB, 0xAC001000);

	// Step27
	udelay(10);

	// Step28 is skipped because dll_mas_dly is unused.

	// Step29
	write_phy_reg(PHY_DLL_INCR_TRIM_3, 0x00000000);

	// Step30
	write_phy_reg(PHY_DLL_INCR_TRIM_1, 0x00000000);

	// Step31
	for(i=0; i<BYTE_LANES; i++){
		write_phy_reg(PHY_LANE_SEL, 6 * i);
		write_phy_reg(PHY_DLL_TRIM_1, 9);
		write_phy_reg(PHY_DLL_TRIM_3, 10);
	}

	// Step32
	write_phy_reg(PHY_DLL_ADRCTRL, 26 | 0x00000200);

	// Step33
	write_phy_reg(PHY_LANE_SEL, 0);
	write_phy_reg(PHY_DLL_TRIM_CLK, 26 | 0x00000080);

	// Step34
	tmp = (uint32_t)_MIN(1000000000000 / (2 * tCK * 256), 0x3FFFF);
	write_phy_reg(PHY_DLL_RECALIB, 26 | (tmp << 8) | 0xAC000000);

	// Step35
	write_phy_reg(SCL_LATENCY, 0x00035076);

	// Step36
	write_phy_reg(BIT_LVL_CONFIG, 0x00000032);

	// Step37-2
	rmw_phy_reg(UNIQUIFY_IO_2, 0xFF800000, 0x00000000);

	// Step37-3
	switch(dram){
	case 0:
		tmp = 0x00003200;
		break;
	case 1:
		tmp = 0x00005200;
		break;
	case 2:
		tmp = 0x08009200;
		break;
	default:
		tmp = 0x00000000;
		break;
	}
	write_phy_reg(UNIQUIFY_IO_3, tmp);
	write_phy_reg(UNIQUIFY_IO_1, 0x00000002);
	while((read_phy_reg(UNIQUIFY_IO_1) & 0x00000008) != 0x00000008);
	write_phy_reg(UNIQUIFY_IO_1, 0x00000000);
	udelay(100);

	// Step37-4
	switch(dram){
	case 0:
	case 1:
		tmp = 0x00041200;
		break;
	case 2:
		tmp = 0x08081300;
		break;
	default:
		tmp = 0x00000000;
		break;
	}
	write_phy_reg(UNIQUIFY_IO_3, tmp);
	write_phy_reg(UNIQUIFY_IO_1, 0x00000001);
	while((read_phy_reg(UNIQUIFY_IO_1) & 0x00000004) != 0x00000004);
	rmw_phy_reg(UNIQUIFY_IO_1, 0xFFFFFFEF, 0x00000010);
	rmw_phy_reg(UNIQUIFY_IO_1, 0xFFFFFFEF, 0x00000000);
	udelay(1);

	// Step38
	rmw_phy_reg(PHY_DLL_RECALIB, 0xFBFFFFFF, 0x00000000);

	// Step39
	rmw_phy_reg(PHY_CTRL0, 0xFFFFF0FF, (SCL_LANES << 8));
}

static void program_phy2(void)
{
	uint32_t tmp = read_mc_reg(DENALI_CTL_413);
	uint32_t tCK = tmp & 0xFFFF;

	// Step1
	rmw_phy_reg(DYNAMIC_WRITE_BIT_LVL, 0xFFFFFFFE, (tmp >> 23) & 0x1);

	// Step2
	rmw_phy_reg(DYNAMIC_BIT_LVL, 0xFFFFFFFE, (tmp >> 22) & 0x1);

	// Step3
	write_phy_reg(DSCL_CNT, (((tmp >> 21) & 0x1) << 24) |
		_MIN(1000000000000 / (2 * tCK * 256), 0xFFFFFF));
}

static void exec_trainingWRLVL(void)
{
	// Step1
	rmw_phy_reg(SCL_CONFIG_1, 0xFEFFFFFF, 0x01000000);

	// Step2
	write_phy_reg(WRLVL_AUTOINC_TRIM, SCL_LANES);

	// Step3
	write_phy_reg(PHY_DLL_TRIM_2, 0x00010000);

	// Step4
	write_phy_reg(SCL_START, 0x50200000);

	// Step5
	while((read_phy_reg(SCL_START) & 0x10000000) != 0x00000000);

	// Step6 - Step8
	if(((read_phy_reg(WRLVL_CTRL) & SCL_LANES) != SCL_LANES) ||
	   ((read_phy_reg(WRLVL_AUTOINC_TRIM) & SCL_LANES) != 0)){
		panic();
	}

	// Step9
	rmw_phy_reg(SCL_CONFIG_1, 0xFEFFFFFF, 0x00000000);
}

static void write_mr(uint8_t cs, uint8_t mrw_sel, uint16_t mrw_data)
{
	uint8_t mrw_cs;
	uint8_t mrw_allcs;

	// Step1
	if(cs & 0x1){
		rmw_mc_reg(DENALI_CTL_75, 0xFFFF0000, mrw_data);
		mrw_cs = 0;
	}
	if(cs & 0x2){
		rmw_mc_reg(DENALI_CTL_76, 0xFFFF0000, mrw_data);
		mrw_cs = 1;
	}
	mrw_allcs = ((cs & 0x3) == 0x3) ? 1 : 0;

	// Step2
	rmw_mc_reg(DENALI_CTL_67, 0xFC000000,
		0x02800000 | (mrw_allcs << 24) | (mrw_cs << 8) | mrw_sel);

	// Step3
	while((read_mc_reg(DENALI_CTL_147) & (1 << 3)) != (1 << 3));

	// Step4
	rmw_mc_reg(DENALI_CTL_155, 0xFFF7, 0x0008);
}

static void setup_vref_training_registers(
	uint8_t vref_value, uint8_t cs, uint16_t turn_on_off_vref_training)
{
	uint8_t vref_op_code;
	uint16_t mr;

	// Step1
	if(vref_value > 50){
		vref_op_code = vref_value - 23;
	}
	else{
		vref_op_code = vref_value | (1 << 6);
	}

	// Step2
	mr = read_mc_reg(DENALI_CTL_83) & 0xFF00;
	write_mr(cs, 6,
		mr | (((turn_on_off_vref_training == 2) ? 0 : 1) << 7) | vref_op_code);

	// Step3
	udelay(1);
}

static void exec_trainingVREF(void)
{
#if 0
	double dval, vref_mid_level;
#else
	uint64_t dval, vref_mid_level, tmp64;	// x1,000,000
#endif
	uint32_t vref_mid_level_code;
	uint32_t vref_training_value;
	uint32_t sweep_range;
	uint32_t current_vref;
	uint32_t best_window_diff_so_far[BYTE_LANES];
	uint32_t num_best_vref_matches[BYTE_LANES];
	uint32_t all_best_vref_matches[BYTE_LANES][MAX_BEST_VREF_SAVED];
	uint8_t window_0, window_1, window_diff;
	uint32_t highest_best_vref_val, lowest_best_vref_val;
	uint8_t orig_cs_config;
	uint64_t val;
	int i, j;

	// Step1
#if 0
	dval = (double)((read_mc_reg(DENALI_CTL_415) >> 8) & 0xFF);
	val = (read_mc_reg(DENALI_CTL_416) >> 24) & 0xFF;
	if(val > 0){
		dval = 1 / ((1/dval) + (1/val));
	}
	val = read_mc_reg(DENALI_CTL_416) & 0xFF;
	vref_mid_level = (1 + val / (val + dval)) / 2.0;
	vref_mid_level_code = ((vref_mid_level - 0.507) / 0.0066) + 65 + 0.4999;
	sweep_range = (vref_mid_level * 0.15 / 0.0066) + 0.4999;
#else
	dval = (read_mc_reg(DENALI_CTL_415) >> 8) & 0xFF;
	val = (read_mc_reg(DENALI_CTL_416) >> 24) & 0xFF;
	if(val > 0){
		dval = 1000000000000 / ((1000000/dval) + (1000000/val));
	}
	val = read_mc_reg(DENALI_CTL_416) & 0xFF;
	vref_mid_level = (((val + 1) * 1000000000000) / ((val * 1000000) + dval)) / 2;
	tmp64 = (((vref_mid_level - 507000) * 1000000) / 6600) + 65000000 - 500000;
	vref_mid_level_code = CEIL(tmp64, 1000000);
	tmp64 = (((vref_mid_level * 150000) * 1000000) / 6600) - 500000;
	sweep_range = CEIL(tmp64, 1000000);
#endif

	// Step2
	for(i=0; i<BYTE_LANES; i++){
		best_window_diff_so_far[i] = 255;
		num_best_vref_matches[i] = 0;
	}

	// Step3
	for(vref_training_value = 0;
		vref_training_value < (sweep_range * 2) + 1;
		vref_training_value += VREF_SETP)
	{
		// Step3-1
		if((vref_mid_level_code - vref_training_value) < 2){
			vref_training_value = sweep_range;
			continue;
		}
		else if((vref_training_value + vref_mid_level_code) > 126){
			break;
		}
		for(i=0; i<BYTE_LANES; i++){
			if(vref_training_value < (sweep_range + 1)){
				current_vref = vref_mid_level_code - vref_training_value;
			}
			else{
				current_vref = vref_mid_level_code + vref_training_value - sweep_range;
			}
			write_phy_reg(PHY_LANE_SEL, 7 * i);
			write_phy_reg(VREF_TRAINING, (current_vref << 4) | 0x00000001);
		}

		// Step3-2
		write_phy_reg(SCL_START, 0x30800000);
		while((read_phy_reg(SCL_START) & 0x10000000) != 0x00000000);

		// Step3-3
		for(i=0; i<BYTE_LANES; i++){
			if(((read_phy_reg(DYNAMIC_BIT_LVL) >> (14 + i)) & 0x1) == 0x0){
				write_phy_reg(PHY_LANE_SEL, i * 6);
				window_0 = read_phy_reg(VREF_TRAINING_WINDOWS) & 0x3F;
				window_1 = (read_phy_reg(VREF_TRAINING_WINDOWS) >> 8) & 0x3F;
				window_diff = (window_0 > window_1) ?
								window_0 - window_1 : window_1 - window_0;
				if(window_diff < best_window_diff_so_far[i]){
					best_window_diff_so_far[i] = window_diff;
					all_best_vref_matches[i][0] = current_vref;
					num_best_vref_matches[i] = 1;
				}
				else if((window_diff == best_window_diff_so_far[i]) &&
						(num_best_vref_matches[i] < MAX_BEST_VREF_SAVED)){
					all_best_vref_matches[i][num_best_vref_matches[i]] = current_vref;
					num_best_vref_matches[i] += 1;
				}
				else{
					ERROR("PHY side VREF training failed lane %d, current_vref = %d",
						i, current_vref);
				}
			}
		}
	}

	// Step4
	for(i=0; i<BYTE_LANES; i++){
		highest_best_vref_val = 0x0;
		lowest_best_vref_val = 0x7F;
		for(j=0; j<num_best_vref_matches[i]; j++){
			highest_best_vref_val =
				_MAX(all_best_vref_matches[i][j], highest_best_vref_val);
			lowest_best_vref_val  =
				_MIN(all_best_vref_matches[i][j], lowest_best_vref_val);
		}
		current_vref = (highest_best_vref_val + lowest_best_vref_val) >> 1;
		write_phy_reg(PHY_LANE_SEL, 7 * i);
		write_phy_reg(VREF_TRAINING, current_vref << 4);
	}

	// Step5
	write_phy_reg(SCL_DATA_0, 0xFF00FF00);
	write_phy_reg(SCL_DATA_1, 0xFF00FF00);

	// Step6
	write_phy_reg(SCL_START, 0x30800000);
	while((read_phy_reg(SCL_START) & 0x10000000) != 0x00000000);

	// Step7
	if(((read_phy_reg(DYNAMIC_BIT_LVL) >> 14) & SCL_LANES) != SCL_LANES){
		panic();
	}

	// Step8
	rmw_phy_reg(IP_DQ_DQS_BITWISE_TRIM, 0xFFFFFF7F, 0x00000080);

	// Step9
#if 0
	dval = (double)((read_mc_reg(DENALI_CTL_416) >> 8) & 0xFF);
	val = (read_mc_reg(DENALI_CTL_416) >> 16) & 0xFF;
	if(val > 0){
		dval = 1 / ((1/dval) + (1/val));
	}
	val = read_mc_reg(DENALI_CTL_415) & 0xFF;
	vref_mid_level = (1 + val / (val + dval)) / 2.0;
	vref_mid_level_code = ((vref_mid_level - 0.450) / 0.0065) + 0.4999; 
	sweep_range = (vref_mid_level * 0.15 / 0.0065) + 0.4999;
#else
	dval = (read_mc_reg(DENALI_CTL_416) >> 8) & 0xFF;
	val = (read_mc_reg(DENALI_CTL_416) >> 16) & 0xFF;
	if(val > 0){
		dval = 1000000000000 / ((1000000/dval) + (1000000/val));
	}
	val = read_mc_reg(DENALI_CTL_415) & 0xFF;
	vref_mid_level = (((val + 1) * 1000000000000) / ((val * 1000000) + dval)) / 2;
	tmp64 = (((vref_mid_level - 450000) * 1000000) / 6500) - 500000;
	vref_mid_level_code = CEIL(tmp64, 1000000);
	tmp64 = (((vref_mid_level * 150000) * 1000000) / 6500) - 500000;
	sweep_range = CEIL(tmp64, 1000000);
#endif

	// Step10
	orig_cs_config = read_phy_reg(SCL_CONFIG_2) & 0x3;

	// Step11
	setup_vref_training_registers(vref_mid_level_code, SCL_LANES, 1);

	// Step12
	rmw_phy_reg(VREF_TRAINING, 0xFFFFFFFE, 0x00000001);

	// Step13
	for(i=0; i<BYTE_LANES; i++){
		best_window_diff_so_far[i] = 255;
		num_best_vref_matches[i] = 0;
	}

	// Step14
	for(vref_training_value = 0;
		vref_training_value < (sweep_range * 2) + 1;
		vref_training_value += VREF_SETP)
	{
		// Step14-1
		if(vref_training_value < (sweep_range + 1)){
			if(vref_training_value > vref_mid_level_code){
				vref_training_value = sweep_range;
				continue;
			}
			else{
				current_vref = vref_mid_level_code - vref_training_value;
			}
		}
		else{
			if((vref_mid_level_code + vref_training_value - sweep_range) > 73){
				break;
			}
			else{
				current_vref =
					vref_mid_level_code + vref_training_value - sweep_range;
			}
		}
		setup_vref_training_registers(current_vref, orig_cs_config, 0);

		// Step14-2
		write_phy_reg(SCL_START, 0x30500000);
		while((read_phy_reg(SCL_START) & 0x10000000) != 0x00000000);

		// Step14-3
		for(i=0; i<BYTE_LANES; i++){
			if(((read_phy_reg(DYNAMIC_WRITE_BIT_LVL) >> 20) & SCL_LANES) == 0x0){
				write_phy_reg(PHY_LANE_SEL, i * 6);
				window_0 = read_phy_reg(VREF_TRAINING_WINDOWS) & 0x3F;
				window_1 = (read_phy_reg(VREF_TRAINING_WINDOWS) >> 8) & 0x3F;
				window_diff = (window_0 > window_1) ?
								window_0 - window_1 : window_1 - window_0;
				if(window_diff < best_window_diff_so_far[i]){
					best_window_diff_so_far[i] = window_diff;
					all_best_vref_matches[i][0] = current_vref;
					num_best_vref_matches[i] = 1;
				}
				else if((window_diff == best_window_diff_so_far[i]) &&
						(num_best_vref_matches[i] < MAX_BEST_VREF_SAVED)){
					all_best_vref_matches[i][num_best_vref_matches[i]] = current_vref;
					num_best_vref_matches[i] += 1;
				}
				else{
					ERROR("VREF training failed during VrefDQ training DRAM side,\
							current_vref = %d", current_vref);
				}
			}
		}
	}

	// Step15
	highest_best_vref_val = 0x0;
	lowest_best_vref_val = 0x7F;
	for(i=0; i<BYTE_LANES; i++){
		for(j=0; j<num_best_vref_matches[i]; j++){
			highest_best_vref_val =
				_MAX(all_best_vref_matches[i][j], highest_best_vref_val);
			lowest_best_vref_val  =
				_MIN(all_best_vref_matches[i][j], lowest_best_vref_val);
		}
	}
	current_vref = (highest_best_vref_val + lowest_best_vref_val) >> 1;

	// Step16
	setup_vref_training_registers(current_vref, SCL_LANES, 0);

	// Step17
	rmw_phy_reg(VREF_TRAINING, 0xFFFFFFFE, 0x00000000);

	// Step18
	setup_vref_training_registers(current_vref, SCL_LANES, 2);

	// Step19
	rmw_phy_reg(IP_DQ_DQS_BITWISE_TRIM, 0xFFFFFF7F, 0x00000000);
}

static void exec_trainingBITLVL(void)
{
	// Step1
	write_phy_reg(DISABLE_GATING_FOR_SCL, 0x00000000);

	// Step2
	write_phy_reg(SCL_DATA_0, 0xFF00FF00);
	write_phy_reg(SCL_DATA_1, 0xFF00FF00);

	// Step3
	write_phy_reg(SCL_START, 0x30A00000);

	// Step4
	while((read_phy_reg(SCL_START) & 0x10000000) != 0x00000000);

	// Step5 - Step6
	if(((read_phy_reg(DYNAMIC_BIT_LVL) >> 14) & SCL_LANES) != SCL_LANES){
		panic();
	}

	// Step7
	rmw_phy_reg(IP_DQ_DQS_BITWISE_TRIM, 0xFFFFFF7F, 0x00000080);

	// Step8
	write_phy_reg(SCL_START, 0x30700000);

	// Step9
	while((read_phy_reg(SCL_START) & 0x10000000) != 0x00000000);

	// Step10 - Step11
	if(((read_phy_reg(DYNAMIC_WRITE_BIT_LVL) >> 20) & SCL_LANES) != SCL_LANES){
		panic();
	}

	// Step12
	rmw_phy_reg(IP_DQ_DQS_BITWISE_TRIM, 0xFFFFFF7F, 0x00000000);

	// Step13
	write_phy_reg(PHY_SCL_START_ADDR, 0x00080000);

	// Step14
	write_phy_reg(SCL_START, 0x11200000);

	// Step15
	while((read_phy_reg(SCL_START) & 0x10000000) != 0x00000000);

	// Step16
	write_phy_reg(PHY_SCL_START_ADDR, 0x00000000);

	// Step17
	write_phy_reg(SCL_START, 0x30600000);

	// Step18
	while((read_phy_reg(SCL_START) & 0x10000000) != 0x00000000);
}

static void opt_delay(void)
{
	uint32_t tmp;
	uint16_t dlls_trim_ca;
	uint16_t dlls_trim_2[BYTE_LANES];
	uint16_t op_dqs_trim[BYTE_LANES];
	uint16_t min_WL;
	uint16_t min_WD = 128;
	int i, j;

	// Step1
	rmw_phy_reg(PHY_DLL_RECALIB, 0xFBFFFFFF, 0x04000000);

	// Step2
	rmw_mc_reg(DENALI_CTL_56, 0xFFFFFF80, 0x00000011);
	while(((read_mc_reg(DENALI_CTL_59) >> 24) & 0x7F) != 0x48);

	// Step3
	write_phy_reg(PHY_LANE_SEL, 0);
	dlls_trim_ca = read_phy_reg(PHY_DLL_TRIM_CLK) & 0x7F;
	min_WL = dlls_trim_ca;

	for(i=0; i<BYTE_LANES; i++){
		write_phy_reg(PHY_LANE_SEL, 6 * i);
		dlls_trim_2[i] = read_phy_reg(PHY_DLL_TRIM_2) & 0x3F;
		min_WL = _MIN(min_WL, dlls_trim_2[i]);

		write_phy_reg(PHY_LANE_SEL, (7 * i) | 0x00000900);
		op_dqs_trim[i] = read_phy_reg(OP_DQ_DM_DQS_BITWISE_TRIM) & 0x3F;
		min_WD = _MIN(min_WD, op_dqs_trim[i]);
		for(j=0; j<9; j++){
			write_phy_reg(PHY_LANE_SEL, (i * 7) | (j << 8));
			tmp = read_phy_reg(OP_DQ_DM_DQS_BITWISE_TRIM) & 0x7F;
			tmp = (tmp & 0x40) ?
				(op_dqs_trim[i] + (tmp & 0x3F)) : (op_dqs_trim[i] - (tmp & 0x3F));
			min_WD = _MIN(min_WD, tmp);
		}
	}

	// Step4
	tmp = (dlls_trim_ca - min_WL) & 0x7F;
	write_phy_reg(PHY_LANE_SEL, 0);
	write_phy_reg(PHY_DLL_TRIM_CLK, tmp | 0x00000080);
	write_phy_reg(PHY_DLL_ADRCTRL, tmp | 0x00000200);
	rmw_phy_reg(PHY_DLL_RECALIB, 0xFFFFFF80, tmp);

	for(i=0; i<BYTE_LANES; i++){
		tmp = (dlls_trim_2[i] - min_WL) & 0x3F;
		write_phy_reg(PHY_LANE_SEL, 6 * i);
		rmw_phy_reg(PHY_DLL_TRIM_2, 0xFFFFFFC0, tmp);

		write_phy_reg(PHY_LANE_SEL, (7 * i) | 0x00000900);
		tmp = (op_dqs_trim[i] - min_WD) & 0x3F;
		rmw_phy_reg(OP_DQ_DM_DQS_BITWISE_TRIM, 0xFFFFFF80, tmp);
	}

	// Step5
	rmw_mc_reg(DENALI_CTL_56, 0xFFFFFF80, 0x00000002);
	while(((read_mc_reg(DENALI_CTL_59) >> 24) & 0x7F) != 0x40);

	// Step6
	rmw_phy_reg(PHY_DLL_RECALIB, 0xFBFFFFFF, 0x00000000);
	while((read_phy_reg(UNQ_ANALOG_DLL_2) & 0x3) != SCL_LANES);
}

static void exec_trainingSCL(void)
{
	// Step1
	write_phy_reg(DISABLE_GATING_FOR_SCL, 0x00000001);

	// Step2
	write_phy_reg(SCL_MAIN_CLK_DELTA, 0x00000010);

	// Step3
	write_phy_reg(SCL_DATA_0, 0x789B3DE0);
	write_phy_reg(SCL_DATA_1, 0xF10E4A56);

	// Step4
	write_phy_reg(SCL_START, 0x11200000);

	// Step5
	while((read_phy_reg(SCL_START) & 0x10000000) != 0x00000000);

	// Step6
	write_phy_reg(SCL_START, 0x34200000);

	// Step7
	while((read_phy_reg(SCL_START) & 0x10000000) != 0x00000000);

	// Step8 - Step9
	if((read_phy_reg(SCL_START) & SCL_LANES) != SCL_LANES){
		panic();
	}

	// Step10
	write_phy_reg(DISABLE_GATING_FOR_SCL, 0x00000003);
}

void ddr_setup(void)
{
	int i;

	// Step2 - Step11
	cpg_active_ddr(disable_phy_clk);

	// Step12
	program_mc1();

	// Step13 should be skipped

	// Step14
	for(i=0; i<SIZEOF(swizzle_mc_tbl); i++){
		write_mc_reg(swizzle_mc_tbl[i][0], swizzle_mc_tbl[i][1]);
	}
	for(i=0; i<SIZEOF(swizzle_phy_tbl); i++){
		write_phy_reg(swizzle_phy_tbl[i][0], swizzle_phy_tbl[i][1]);
	}

	// Step15
	rmw_phy_reg(PHY_CTRL0, 0xFFFEFFFF, 0x00010000);

	// Step16
	program_phy1();

	// Step17
	while((read_phy_reg(UNQ_ANALOG_DLL_2) & 0x00000003) != SCL_LANES);

	// Step18
	rmw_mc_reg(DENALI_CTL_00, 0xFFFFFFFE, 0x00000001);

	// Step19
	while((read_mc_reg(DENALI_CTL_146) & 0x02000000) != 0x02000000);

	// Step20
	rmw_mc_reg(DENALI_CTL_154, 0xFDFFFFFF, 0x02000000);

	// Step21
	exec_trainingWRLVL();

	// Step22
	exec_trainingVREF();
	
	// Step23
	exec_trainingBITLVL();

	// Step24
	opt_delay();

	// Step25
	exec_trainingSCL();

	// Step26
	program_phy2();

	// Step27
	program_mc2();

	// Step28 is skipped because ECC is unused.

	// Step29
	rmw_mc_reg(DENALI_CTL_60, 0xFFFFFFF0, 0<<4);
}

void ddr_init(uintptr_t mc_base, uintptr_t phy_base)
{
	mc_reg = mc_base;
	phy_reg = phy_base;

	ddr_setup();
}
