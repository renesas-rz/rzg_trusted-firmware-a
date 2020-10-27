/*
 * Copyright (c) 2020, Renesas Electronics Corporation.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#define BOARDNUM 5
#define BOARD_JUDGE_AUTO

#ifdef BOARD_JUDGE_AUTO
static uint32_t _board_judge(void);

static uint32_t boardcnf_get_brd_type(void)
{
	return _board_judge();
}
#else
static uint32_t boardcnf_get_brd_type(void)
{
	return 1;
}
#endif

#define DDR_FAST_INIT

struct _boardcnf_ch {
	uint8_t ddr_density[CS_CNT];
	uint64_t ca_swap;
	uint16_t dqs_swap;
	uint32_t dq_swap[SLICE_CNT];
	uint8_t dm_swap[SLICE_CNT];
	uint16_t wdqlvl_patt[16];
	int8_t cacs_adj[16];
	int8_t dm_adj_w[SLICE_CNT];
	int8_t dq_adj_w[SLICE_CNT * 8];
	int8_t dm_adj_r[SLICE_CNT];
	int8_t dq_adj_r[SLICE_CNT * 8];
};

struct _boardcnf {
	uint8_t phyvalid;
	uint8_t dbi_en;
	uint16_t cacs_dly;
	int16_t cacs_dly_adj;
	uint16_t dqdm_dly_w;
	uint16_t dqdm_dly_r;
	struct _boardcnf_ch ch[DRAM_CH_CNT];
};

#define WDQLVL_PAT {\
	0x00AA,\
	0x0055,\
	0x00AA,\
	0x0155,\
	0x01CC,\
	0x0133,\
	0x00CC,\
	0x0033,\
	0x00F0,\
	0x010F,\
	0x01F0,\
	0x010F,\
	0x00F0,\
	0x00F0,\
	0x000F,\
	0x010F}

static const struct _boardcnf boardcnfs[BOARDNUM] = {
/* boardcnf[0] Jiangsu HopeRun Software Co., Ltd. HiHope RZ/G2M board 16Gbit/1rank/2ch board with G2M/SOC */
	{
	 .phyvalid = 0x03,
	 .dbi_en = 0x01,
	 .cacs_dly = 0x02c0,
	 .cacs_dly_adj = 0,
	 .dqdm_dly_w = 0x0300,
	 .dqdm_dly_r = 0x00a0,
	 .ch = {
		{
		 {0x04, 0xff},
		  0x00345201,
		  0x3201,
		 {0x01672543, 0x45361207, 0x45632107, 0x60715234},
		 {0x08, 0x08, 0x08, 0x08},
		 WDQLVL_PAT,
		 {0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0},
		 {0, 0, 0, 0},
		 {0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0},
		 {0, 0, 0, 0 },
		 {0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0}
		},
		{
		 {0x04, 0xff},
		  0x00302154,
		  0x2310,
		 {0x01672543, 0x45361207, 0x45632107, 0x60715234},
		 {0x08, 0x08, 0x08, 0x08},
		 WDQLVL_PAT,
		 {0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0},
		 {0, 0, 0, 0},
		 {0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0},
		 {0, 0, 0, 0 },
		 {0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0}
		}
	       }
	},

/* boardcnf[1] Jiangsu HopeRun Software Co., Ltd. HiHope RZ/G2M board 8Gbit/2rank/2ch board with G2M/SOC */
	{
	 0x03,
	 0x01,
	 0x02c0,
	 0,
	 0x0300,
	 0x00a0,
	{
	{
		{0x02, 0x02},
		 0x00345201,
		 0x3201,
		{0x01672543, 0x45361207, 0x45632107, 0x60715234},
		{0x08, 0x08, 0x08, 0x08},
		WDQLVL_PAT,
		{0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0}
		},
	{
		{0x02, 0x02},
		 0x00302154,
		 0x2310,
		{0x01672543, 0x45361207, 0x45632107, 0x60715234},
		{0x08, 0x08, 0x08, 0x08},
		WDQLVL_PAT,
		{0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0}
	}
	}
	},

/* boardcnf[2] Jiangsu HopeRun Software Co., Ltd. HiHope RZ/G2N board 16Gbit/2rank/1ch board with G2N/SOC */
	{
	 0x01,
	 0x01,
	 0x0300,
	 0,
	 0x0300,
	 0x00a0,
	{
	{
	   {0x04, 0x04},
	    0x00345201,
	    0x3201,
	   {0x01672543, 0x45361207, 0x45632107, 0x60715234},
	   {0x08, 0x08, 0x08, 0x08},
	   WDQLVL_PAT,
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0},
	   {0, 0, 0, 0},
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0},
	   {0, 0, 0, 0},
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0}
	}
	}
},

/* boardcnf[3] Jiangsu HopeRun Software Co., Ltd. HiHope RZ/G2H board 8Gbit/2rank/2ch board with G2H/SOC */
{
	0x05,
	0x01,
	0x0300,
	0,
	0x0300,
	0x00a0,
	{
	{
	   {0x02, 0x02},
	   0x00345201,
	   0x3201,
	   {0x01672543, 0x45367012, 0x45632107, 0x60715234},
	   {0x08, 0x08, 0x08, 0x08},
	   WDQLVL_PAT,
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0},
	   {0, 0, 0, 0},
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0},
	   {0, 0, 0, 0},
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0}
	   },
	{
	   {0x02, 0x02},
	    0x00302154,
	    0x2310,
	   {0x01672543, 0x45361207, 0x45632107, 0x60715234},
	   {0x08, 0x08, 0x08, 0x08},
	   WDQLVL_PAT,
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0},
	   {0, 0, 0, 0},
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0},
	   {0, 0, 0, 0},
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0}
	   },
	{
	   {0x02, 0x02},
	    0x00302154,
	    0x2310,
	   {0x01672543, 0x45361207, 0x45632107, 0x60715234},
	   {0x08, 0x08, 0x08, 0x08},
	   WDQLVL_PAT,
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0},
	   {0, 0, 0, 0},
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0},
	   {0, 0, 0, 0},
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0}
	   },
	{
	   {0xff, 0xff},
	    0,
	    0,
	   {0, 0, 0, 0},
	   {0, 0, 0, 0},
	   WDQLVL_PAT,
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0},
	   {0, 0, 0, 0},
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0},
	   {0, 0, 0, 0},
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0}
	   }
	}
},

/* boardcnf[4] Jiangsu HopeRun Software Co., Ltd. HiHope RZ/G2H board 16Gbit/1rank/2ch board with G2H/SOC */
{
	0x05,
	0x01,
	0x0300,
	0,
	0x0300,
	0x00a0,
	{
	{
	   {0x04, 0xff},
	   0x00345201,
	   0x3201,
	   {0x01672543, 0x45367012, 0x45632107, 0x60715234},
	   {0x08, 0x08, 0x08, 0x08},
	   WDQLVL_PAT,
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0},
	   {0, 0, 0, 0},
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0},
	   {0, 0, 0, 0},
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
	   {0x04, 0xff},
	    0x00302154,
	    0x2310,
	   {0x01672543, 0x45361207, 0x45632107, 0x60715234 },
	   {0x08, 0x08, 0x08, 0x08},
	   WDQLVL_PAT,
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0},
	   {0, 0, 0, 0 },
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0},
	   {0, 0, 0, 0},
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0}
	   },
	{
	   {0x04, 0xff},
	    0x00302154,
	    0x2310,
	   {0x01672543, 0x45361207, 0x45632107, 0x60715234 },
	   {0x08, 0x08, 0x08, 0x08},
	   WDQLVL_PAT,
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0},
	   {0, 0, 0, 0},
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0},
	   {0, 0, 0, 0},
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0}
	},
	{
	   {0xff, 0xff},
	    0,
	    0,
	   {0, 0, 0, 0},
	   {0, 0, 0, 0},
	   WDQLVL_PAT,
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0},
	   {0, 0, 0, 0},
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0},
	   {0, 0, 0, 0},
	   {0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0}
	}
	}
},
};

void boardcnf_get_brd_clk(uint32_t brd, uint32_t *clk, uint32_t *div)
{
	uint32_t md;

	md = (mmio_read_32(RST_MODEMR) >> 13) & 0x3;
	switch (md) {
	case 0x0:
		*clk = 50;
		*div = 3;
		break;
	case 0x1:
		*clk = 60;
		*div = 3;
		break;
	case 0x2:
		*clk = 75;
		*div = 3;
		break;
	case 0x3:
		*clk = 100;
		*div = 3;
		break;
	}
	(void)brd;
}

void boardcnf_get_ddr_mbps(uint32_t brd, uint32_t *mbps, uint32_t *div)
{
	uint32_t md;

	md = (mmio_read_32(RST_MODEMR) >> 17) & 0x5;
	md = (md | (md >> 1)) & 0x3;
	switch (md) {
	case 0x0:
		*mbps = 3200;
		*div = 1;
		break;
	case 0x1:
		*mbps = 2800;
		*div = 1;
		break;
	case 0x2:
		*mbps = 2400;
		*div = 1;
		break;
	case 0x3:
		*mbps = 1600;
		*div = 1;
		break;
	}
	(void)brd;
}

#define _def_REFPERIOD  1890

#define G2M_SAMPLE_TT_A84	0xB866CC10, 0x3B250421
#define G2M_SAMPLE_TT_A85	0xB866CC10, 0x3AA50421
#define G2M_SAMPLE_TT_A86	0xB866CC10, 0x3AA48421
#define G2M_SAMPLE_FF_B45	0xB866CC10, 0x3AB00C21
#define G2M_SAMPLE_FF_B49	0xB866CC10, 0x39B10C21
#define G2M_SAMPLE_FF_B56	0xB866CC10, 0x3AAF8C21
#define G2M_SAMPLE_SS_E24	0xB866CC10, 0x3BA39421
#define G2M_SAMPLE_SS_E28	0xB866CC10, 0x3C231421
#define G2M_SAMPLE_SS_E32	0xB866CC10, 0x3C241421

static const uint32_t termcode_by_sample[20][3] = {
	{G2M_SAMPLE_TT_A84, 0x000158D5},
	{G2M_SAMPLE_TT_A85, 0x00015955},
	{G2M_SAMPLE_TT_A86, 0x00015955},
	{G2M_SAMPLE_FF_B45, 0x00015690},
	{G2M_SAMPLE_FF_B49, 0x00015753},
	{G2M_SAMPLE_FF_B56, 0x00015793},
	{G2M_SAMPLE_SS_E24, 0x00015996},
	{G2M_SAMPLE_SS_E28, 0x000159D7},
	{G2M_SAMPLE_SS_E32, 0x00015997},
	{0xFFFFFFFF, 0xFFFFFFFF, 0x0001554F}
};

#ifdef BOARD_JUDGE_AUTO
/*
 * SAMPLE board detect function
 */
#define PFC_PMMR	0xE6060000U
#define PFC_PUEN5	0xE6060414U
#define PFC_PUEN6	0xE6060418U
#define PFC_PUD5	0xE6060454U
#define PFC_PUD6	0xE6060458U
#define GPIO_INDT5	0xE605500CU
#define GPIO_GPSR6	0xE6060118U

static uint32_t _board_judge(void)
{
	uint32_t brd;

	if (prr_product == PRR_PRODUCT_G2H) {
		brd = 4;
	} else if (prr_product == PRR_PRODUCT_G2M) {
		if (prr_cut <= RZG_G2M_CUT_VER11) {
			brd = 1;
		} else {
			uint32_t lpddr4_2rank = 0x01 << 25;
			if (mmio_read_32(GPIO_INDT5) & lpddr4_2rank) {
				brd = 1;
			} else {
				brd = 0;
			}
		}
	} else {
		brd = 2;
	}

	return brd;
}
#endif
