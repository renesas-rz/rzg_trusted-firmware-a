/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <lib/mmio.h>
#include <sys_regs.h>

#include "ddr_regs.h"
#include "ddr_private.h"

#define DAT0_BASE	(0x00060000)
#define DAT1_BASE	(0x00064000)

extern const uint32_t param_phyinit_swizzle[][2];
extern const uint32_t param_phyinit_c[][2];
extern const uint32_t param_phyinit_i[][2];
extern const uint16_t param_phyinit_1d_dat0[];
extern const uint16_t param_phyinit_2d_dat0[];
extern const uint16_t param_phyinit_1d_dat1[];
extern const uint16_t param_phyinit_2d_dat1[];
extern const uint32_t param_setup_mc[][2];

extern const uint32_t param_phyinit_swizzle_size;
extern const uint32_t param_phyinit_c_size;
extern const uint32_t param_phyinit_i_size;
extern const uint32_t param_phyinit_1d_dat0_size;
extern const uint32_t param_phyinit_2d_dat0_size;
extern const uint32_t param_phyinit_1d_dat1_size;
extern const uint32_t param_phyinit_2d_dat1_size;
extern const uint32_t param_setup_mc_size;

static int8_t dwc_ddrphy_cdd_int(uint8_t val);
static int8_t dwc_ddrphy_cdd_abs(uint8_t val);

void setup_mc(void)
{
	size_t i;

	for (i = 0; i < param_setup_mc_size; i++)
		DDRTOP_mc_apb_wr(param_setup_mc[i][0], param_setup_mc[i][1]);
}

void update_mc(void)
{
	DDRTOP_mc_param_wr(INT_MASK_MASTER_ADDR, INT_MASK_MASTER_OFFSET+31, 1, 0);
	DDRTOP_mc_param_wr(LPI_WAKEUP_EN_ADDR, LPI_WAKEUP_EN_OFFSET, LPI_WAKEUP_EN_WIDTH, 0x1F);
}

void phyinit_configuration(void)
{
	size_t i;

	for (i = 0; i < param_phyinit_c_size; i++) {
		dwc_ddrphy_apb_wr(param_phyinit_c[i][0], param_phyinit_c[i][1]);
	}
}

void phyinit_pin_swizzling(void)
{
	size_t i;

	for (i = 0; i < param_phyinit_swizzle_size; i++) {
		dwc_ddrphy_apb_wr(param_phyinit_swizzle[i][0], param_phyinit_swizzle[i][1]);
	}
}

void phyinit_load_1d_image(void)
{
	size_t i;

	dwc_ddrphy_apb_wr(0x00058060, 0x00000002);
	dwc_ddrphy_apb_wr(0x0006E000, 0x00000000);

	for (i = 0; i < param_phyinit_1d_dat0_size; i++) {
		dwc_ddrphy_apb_wr((uint32_t)(0x60000+i), (uint32_t)(param_phyinit_1d_dat0[i]));
	}

	for (i = param_phyinit_1d_dat0_size; i < 0x4000; i++) {
		dwc_ddrphy_apb_wr((uint32_t)(0x60000+i), (uint32_t)(0));
	}

	dwc_ddrphy_apb_wr(0x0006E000, 0x00000001);

	dwc_ddrphy_apb_wr(0x0006E000, 0x0);
	for (i = 0; i < param_phyinit_1d_dat1_size; i++) {
		dwc_ddrphy_apb_wr(0x00064000+i, (uint32_t)param_phyinit_1d_dat1[i]);
	}

	dwc_ddrphy_apb_wr(0x0006E000, 0x1);
}

void phyinit_exec_1d_image(void)
{
	uint32_t val, sel_train, num_rank, use_ch_b;
	int8_t val0, val1, cdd_rr, cdd_rw_abs, cdd_ww, cdd_ww_abs;
	uint32_t r2r_adr, r2r_ofs, r2r_wid;
	uint32_t r2w_adr, r2w_ofs, r2w_wid;
	uint32_t w2r_adr, w2r_ofs, w2r_wid;
	uint32_t w2w_adr, w2w_ofs, w2w_wid;

	dwc_ddrphy_apb_wr(0x0006E000, 0x0);
	val = dwc_ddrphy_apb_rd(0x3005f);
	val = (val & 0x700) >> 8;
	sel_train = (val == 0b010) ? 1 : (val == 0b001) ? 3 : (val == 0b101) ? 5 : 0;
	dwc_ddrphy_apb_wr(0x0006E000, 0x1);

	dwc_ddrphy_apb_wr(0x0006E000, 0x1);
	dwc_ddrphy_apb_wr(0x0006E099, 0x9);
	dwc_ddrphy_apb_wr(0x0006E099, 0x1);
	dwc_ddrphy_apb_wr(0x0006E099, 0x0);
	dwc_ddrphy_phyinit_userCustom_G_waitDone(sel_train);
	dwc_ddrphy_apb_wr(0x0006E099, 0x1);

	dwc_ddrphy_apb_wr(0x0006E000, 0x0);
	val = DDRTOP_mc_param_rd(CS_MAP_ADDR, CS_MAP_OFFSET, CS_MAP_WIDTH);
	num_rank = (val == 3) ? 2 : 1;

	if (num_rank > 1) {
		if (sel_train <= 2) {
			val = dwc_ddrphy_apb_rd(0x64016);
			val1 = dwc_ddrphy_cdd_int((val >> 8) & 0xff);
			cdd_rr = val1;

			val = dwc_ddrphy_apb_rd(0x64018);
			val0 = dwc_ddrphy_cdd_int((val >> 0) & 0xff);
			cdd_rr = (val0 > cdd_rr) ? val0 : cdd_rr;

			val = dwc_ddrphy_apb_rd(0x64023);
			val1 = dwc_ddrphy_cdd_abs((val >> 8) & 0xff);
			cdd_rw_abs = val1;

			val = dwc_ddrphy_apb_rd(0x64024);
			val0 = dwc_ddrphy_cdd_abs((val >> 0) & 0xff);
			cdd_rw_abs = (val0 > cdd_rw_abs) ? val0 : cdd_rw_abs;

			val = dwc_ddrphy_apb_rd(0x64025);
			val1 = dwc_ddrphy_cdd_abs((val >> 8) & 0xff);
			cdd_rw_abs = (val1 > cdd_rw_abs) ? val1 : cdd_rw_abs;

			val = dwc_ddrphy_apb_rd(0x64026);
			val0 = dwc_ddrphy_cdd_abs((val >> 0) & 0xff);
			cdd_rw_abs = (val0 > cdd_rw_abs) ? val0 : cdd_rw_abs;

			val = dwc_ddrphy_apb_rd(0x6401c);
			val1 = dwc_ddrphy_cdd_int((val >> 8) & 0xff);
			cdd_ww = val1;
			cdd_ww = (val1 > cdd_ww) ? val1 : cdd_ww;
			val1 = dwc_ddrphy_cdd_abs((val >> 8) & 0xff);
			cdd_ww_abs = val1;
			cdd_ww_abs = (val1 > cdd_ww_abs) ? val1 : cdd_ww_abs;

			val = dwc_ddrphy_apb_rd(0x6401e);
			val0 = dwc_ddrphy_cdd_int((val >> 0) & 0xff);
			cdd_ww = (val0 > cdd_ww) ? val0 : cdd_ww;
			val0 = dwc_ddrphy_cdd_abs((val >> 0) & 0xff);
			cdd_ww_abs = (val0 > cdd_ww_abs) ? val0 : cdd_ww_abs;

		} else if (sel_train >= 3) {
			val = dwc_ddrphy_apb_rd(0x64013);
			val0 = dwc_ddrphy_cdd_int((val >> 0) & 0xff);
			val1 = dwc_ddrphy_cdd_int((val >> 8) & 0xff);
			cdd_rr = val0;
			cdd_rr = (val1 > cdd_rr) ? val1 : cdd_rr;

			val = dwc_ddrphy_apb_rd(0x64014);
			val0 = dwc_ddrphy_cdd_abs((val >> 0) & 0xff);
			val1 = dwc_ddrphy_cdd_abs((val >> 8) & 0xff);
			cdd_rw_abs = val0;
			cdd_rw_abs = (val1 > cdd_rw_abs) ? val1 : cdd_rw_abs;

			val = dwc_ddrphy_apb_rd(0x64015);
			val0 = dwc_ddrphy_cdd_abs((val >> 0) & 0xff);
			val1 = dwc_ddrphy_cdd_abs((val >> 8) & 0xff);
			cdd_rw_abs = (val0 > cdd_rw_abs) ? val0 : cdd_rw_abs;
			cdd_rw_abs = (val1 > cdd_rw_abs) ? val1 : cdd_rw_abs;

			val = dwc_ddrphy_apb_rd(0x64018);
			val0 = dwc_ddrphy_cdd_int((val >> 0) & 0xff);
			val1 = dwc_ddrphy_cdd_int((val >> 8) & 0xff);
			cdd_ww = val0;
			cdd_ww = (val1 > cdd_ww) ? val1 : cdd_ww;
			val0 = dwc_ddrphy_cdd_abs((val >> 0)&0xff);
			val1 = dwc_ddrphy_cdd_abs((val >> 8)&0xff);
			cdd_ww_abs = val0;
			cdd_ww_abs = (val1 > cdd_ww_abs) ? val1 : cdd_ww_abs;

			val = dwc_ddrphy_apb_rd(0x6402b);
			use_ch_b = (val >> 8);
			if (use_ch_b > 0) {
				val = dwc_ddrphy_apb_rd(0x6402c);
				val1 = dwc_ddrphy_cdd_int((val >> 8) & 0xff);
				cdd_rr = (val1 > cdd_rr) ? val1 : cdd_rr;

				val = dwc_ddrphy_apb_rd(0x6402d);
				val0 = dwc_ddrphy_cdd_int((val >> 0) & 0xff);
				cdd_rr = (val0 > cdd_rr) ? val0 : cdd_rr;
				val1 = dwc_ddrphy_cdd_abs((val >> 8) & 0xff);
				cdd_rw_abs = (val1 > cdd_rw_abs) ? val1 : cdd_rw_abs;

				val = dwc_ddrphy_apb_rd(0x6402e);
				val0 = dwc_ddrphy_cdd_abs((val >> 0) & 0xff);
				val1 = dwc_ddrphy_cdd_abs((val >> 8) & 0xff);
				cdd_rw_abs = (val0 > cdd_rw_abs) ? val0 : cdd_rw_abs;
				cdd_rw_abs = (val1 > cdd_rw_abs) ? val1 : cdd_rw_abs;

				val = dwc_ddrphy_apb_rd(0x6402f);
				val0 = dwc_ddrphy_cdd_abs((val >> 0) & 0xff);
				cdd_rw_abs = (val0 > cdd_rw_abs) ? val0 : cdd_rw_abs;

				val = dwc_ddrphy_apb_rd(0x64031);
				val1 = dwc_ddrphy_cdd_int((val >> 8) & 0xff);
				cdd_ww = (val1 > cdd_ww) ? val1 : cdd_ww;
				val1 = dwc_ddrphy_cdd_abs((val >> 8) & 0xff);
				cdd_ww_abs = (val1 > cdd_ww_abs) ? val1 : cdd_ww_abs;

				val = dwc_ddrphy_apb_rd(0x64032);
				val0 = dwc_ddrphy_cdd_int((val >> 0) & 0xff);
				cdd_ww = (val0 > cdd_ww) ? val0 : cdd_ww;
				val0 = dwc_ddrphy_cdd_abs((val >> 0) & 0xff);
				cdd_ww_abs = (val0 > cdd_ww_abs) ? val0 : cdd_ww_abs;
			}
		}

		r2r_adr = R2R_DIFFCS_DLY_F0_ADDR; r2r_ofs = R2R_DIFFCS_DLY_F0_OFFSET; r2r_wid = R2R_DIFFCS_DLY_F0_WIDTH;
		r2w_adr = R2W_DIFFCS_DLY_F0_ADDR; r2w_ofs = R2W_DIFFCS_DLY_F0_OFFSET; r2w_wid = R2W_DIFFCS_DLY_F0_WIDTH;
		w2r_adr = W2R_DIFFCS_DLY_F0_ADDR; w2r_ofs = W2R_DIFFCS_DLY_F0_OFFSET; w2r_wid = W2R_DIFFCS_DLY_F0_WIDTH;
		w2w_adr = W2W_DIFFCS_DLY_F0_ADDR; w2w_ofs = W2W_DIFFCS_DLY_F0_OFFSET; w2w_wid = W2W_DIFFCS_DLY_F0_WIDTH;

		if (cdd_rr > 0) {
			val = DDRTOP_mc_param_rd(r2r_adr, r2r_ofs, r2r_wid);
			val += cdd_rr;
			DDRTOP_mc_param_wr(r2r_adr, r2r_ofs, r2r_wid, val);
		}

		if (cdd_ww > 0) {
			val = DDRTOP_mc_param_rd(w2w_adr, w2w_ofs, w2w_wid);
			val += cdd_ww;
			DDRTOP_mc_param_wr(w2w_adr, w2w_ofs, w2w_wid, val);
		}

		val = DDRTOP_mc_param_rd(r2w_adr, r2w_ofs, r2w_wid);
		val += cdd_rw_abs;
		DDRTOP_mc_param_wr(r2w_adr, r2w_ofs, r2w_wid, val);

		val = DDRTOP_mc_param_rd(w2r_adr, w2r_ofs, w2r_wid);
		val += cdd_ww_abs;
		DDRTOP_mc_param_wr(w2r_adr, w2r_ofs, w2r_wid, val);
	}

	dwc_ddrphy_apb_wr(0x0006E000, 0x1);
}

void phyinit_load_2d_image(void)
{
	size_t i;

	dwc_ddrphy_apb_wr(0x0006E000, 0x00000000);

	for (i = 0; i < param_phyinit_2d_dat0_size; i++) {
		dwc_ddrphy_apb_wr(0x00060000 + i, (uint32_t)param_phyinit_2d_dat0[i]);
	}

	for (i = param_phyinit_2d_dat0_size; i < 0x4000; i++) {
		dwc_ddrphy_apb_wr(0x00060000 + i, 0x0);
	}

	dwc_ddrphy_apb_wr(0x0006E000, 0x00000001);

	dwc_ddrphy_apb_wr(0x0006E000, 0x0);
	for (i = 0; i < param_phyinit_2d_dat1_size; i++) {
		dwc_ddrphy_apb_wr(0x00064000 + i, (uint32_t)param_phyinit_2d_dat1[i]);
	}

	dwc_ddrphy_apb_wr(0x0006E000, 0x1);
}

void phyinit_exec_2d_image(void)
{
	uint32_t val, sel_train;

	dwc_ddrphy_apb_wr(0x6e000, 0x0);
	val = dwc_ddrphy_apb_rd(0x3005f);
	val = (val & 0x700) >> 8;
	sel_train = (val == 0b010) ? 2 : (val == 0b001) ? 4 : (val == 0b101) ? 6 : 0;
	dwc_ddrphy_apb_wr(0x6e000, 0x1);

	dwc_ddrphy_apb_wr(0x0006E000, 0x1);
	dwc_ddrphy_apb_wr(0x0006E099, 0x9);
	dwc_ddrphy_apb_wr(0x0006E099, 0x1);
	dwc_ddrphy_apb_wr(0x0006E099, 0x0);
	dwc_ddrphy_phyinit_userCustom_G_waitDone(sel_train);
	dwc_ddrphy_apb_wr(0x0006E099, 0x1);

	dwc_ddrphy_apb_wr(0x6e000, 0x0);
	dwc_ddrphy_apb_wr(0x6e000, 0x1);
}

void phyinit_load_eng_image(void)
{
	size_t i;

	for (i = 0; i < param_phyinit_i_size; i++) {
		dwc_ddrphy_apb_wr(param_phyinit_i[i][0], param_phyinit_i[i][1]);
	}
}

void self_refresh_entry(void)
{
	DDRTOP_mc_param_wr(LP_CMD_ADDR, LP_CMD_OFFSET, LP_CMD_WIDTH, 0b1010001);
	DDRTOP_mc_param_poll(LP_STATE_ADDR, LP_STATE_OFFSET, LP_STATE_WIDTH, 0b1001111);
}

static int8_t dwc_ddrphy_cdd_int(uint8_t val)
{
	return (int8_t)((((val >> 7) & 0x1) == 1) ? -((0x7f ^ ((val >> 0) & 0x7f)) + 1) : ((val >> 0) & 0x7f));
}

static int8_t dwc_ddrphy_cdd_abs(uint8_t val)
{
	return (uint8_t)((((val >> 7) & 0x1) == 1) ?  ((0x7f ^ ((val >> 0) & 0x7f)) + 1) : ((val >> 0) & 0x7f));
}
