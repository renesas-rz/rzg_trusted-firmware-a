/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <cpg_regs.h>
#include <lib/mmio.h>
#include <drivers/delay_timer.h>
#include <cpg_opt.h>

#define	CPG_OFF			(0)
#define	CPG_ON			(1)

#define CPG_T_CLK		(0)
#define CPG_T_RST		(1)

typedef struct {
	uintptr_t addr;
	uint32_t  val;
} CPG_REG_SETTING;

typedef struct {
	CPG_REG_SETTING reg;
	CPG_REG_SETTING mon;
	uint32_t  type;
} CPG_SETUP_DATA;
#if 0
typedef struct {
	CPG_REG_SETTING		clk1_dat;
	CPG_REG_SETTING		clk2_dat;
	CPG_REG_SETTING		stby_dat;
} CPG_PLL_SETDATA_146;

typedef struct {
	CPG_REG_SETTING		clk1_dat;
	CPG_REG_SETTING		clk2_dat;
	CPG_REG_SETTING		clk3_dat;
	CPG_REG_SETTING		clk4_dat;
	CPG_REG_SETTING		clk5_dat;
	CPG_REG_SETTING		stby_dat;
} CPG_PLL_SETDATA_235;

static CPG_PLL_SETDATA_146 cpg_pll4_setdata = {
#if (DDR_PLL4 == 1600)
	{ CPG_PLL4_CLK1, 0xFAE13203 },
	{ CPG_PLL4_CLK2, 0x00081000 },
#elif (DDR_PLL4 == 1333)
	{ CPG_PLL4_CLK1, 0xA66629C3 },
	{ CPG_PLL4_CLK2, 0x00080D00 },
#else
#error "Unknown Board Type."
#endif
	{ CPG_PLL4_STBY, 0x00010001 }
};

static CPG_PLL_SETDATA_146 cpg_pll6_setdata = {
	{ CPG_PLL6_CLK1, 0x00003e83 },
	{ CPG_PLL6_CLK2, 0x00082D02 },
	{ CPG_PLL6_STBY, 0x00010001 }, /* SSC OFF */
};

#define	CPG_PLL2_INDEX					(0)
#define	CPG_PLL3_INDEX					(1)
#define	CPG_PLL5_INDEX					(2)
#endif
static const CPG_SETUP_DATA early_setup_tbl[] = {
	{	/* CGC_SYC_0_CNT_CLK */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_2,
				.val  = 0x00008000,
				},

		.mon = {
				.addr = (uintptr_t)CPG_CLKMON_1,
				.val  = 0x00008000,
				},

		.type = CPG_T_CLK
	},

	{	/* SYC_0_RESETN */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_5,
				.val  = 0x00000080,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_2,
				.val = 0x00000100,
				},

		.type = CPG_T_RST
	}
};

static CPG_SETUP_DATA cpg_clk_on_tbl[] = {
	{	/* CM33 Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_1,
				.val  = 0x00000004,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_0,
				.val  = 0x00040000,
				},

		.type = CPG_T_CLK
	},

	{	/* CM33 Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_2,
				.val  = 0x000008C0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_1,
				.val  = 0x000008C0,
				},

		.type = CPG_T_CLK
	},

	{	/* CR8 Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_0,
				.val  = 0x0000E000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_0,
				.val  = 0x0000E000,
				},

		.type = CPG_T_CLK
	},

	{	/* CR8 Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_1,
				.val  = 0x00000003,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_0,
				.val  = 0x00030000,
				},

		.type = CPG_T_CLK
	},

	{	/* CR8 Part 3 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_2,
				.val  = 0x00000420,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_1,
				.val  = 0x00000420,
				},

		.type = CPG_T_CLK
	},

	{	/* MHU */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_3,
				.val  = 0x00000001,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_1,
				.val  = 0x00010000,
				},

		.type = CPG_T_CLK
	},

	{	/* DMAC Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_0,
				.val  = 0x0000001F,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_0,
				.val  = 0x0000001F,
				},

		.type = CPG_T_CLK
	},

	{	/* DMAC Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_15,
				.val  = 0x00000100,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_7,
				.val  = 0x01000000,
				},

		.type = CPG_T_CLK
	},

	{	/* OSTM */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_4,
				.val  = 0x000007F8,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_2,
				.val  = 0x000007F8,
				},

		.type = CPG_T_CLK
	},

	{	/* GPT */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_3,
				.val  = 0x00000006,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_1,
				.val  = 0x00060000,
				},

		.type = CPG_T_CLK
	},

	{	/* POEG */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_3,
				.val  = 0x000007F1,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_1,
				.val  = 0x07F10000,
				},

		.type = CPG_T_CLK
	},

	{	/* WDT Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_4,
				.val  = 0x0000F800,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_2,
				.val  = 0x0000F800,
				},

		.type = CPG_T_CLK
	},

	{	/* WDT Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_5,
				.val  = 0x00000007,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_2,
				.val  = 0x00070000,
				},

		.type = CPG_T_CLK
	},

	{	/* DDR Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_12,
				.val  = 0x0000FFC0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_6,
				.val  = 0x0000FFC0,
				},

		.type = CPG_T_CLK
	},

	{	/* DDR Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_13,
				.val  = 0x00000003,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_6,
				.val  = 0x00030000,
				},

		CPG_T_CLK
	},

	{	/* DDR Part 3 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_19,
				.val  = 0x00000100,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_9,
				.val  = 0x01000000,
				},

		.type = CPG_T_CLK
	},

	{	/* DDR Part 4 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_20,
				.val  = 0x0000FFC0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_10,
				.val  = 0x0000FFC0,
				},

		.type = CPG_T_CLK
	},

	{	/* DDR Part 5 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_21,
				.val  = 0x0000000F,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_10,
				.val  = 0x000F0000,
				},

		.type = CPG_T_CLK
	},

	{	/* SPI Part 1*/
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_5,
				.val  = 0x00001FF0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_2,
				.val  = 0x1FF00000,
				},

		.type = CPG_T_CLK
	},

	{	/* SPI Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_9,
				.val  = 0x00008000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_4,
				.val  = 0x80000000,
				},

		.type = CPG_T_CLK
	},

	{	/* SPI Part 3 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_10,
				.val  = 0x00000007,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_5,
				.val  = 0x00000007,
				},

		.type = CPG_T_CLK
	},

	{	/* SDHI */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_10,
				.val  = 0x00007FF8,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_5,
				.val  = 0x00007FF8,
				},

		.type = CPG_T_CLK
	},

	{	/* GPU */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_15,
				.val  = 0x00000007,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_7,
				.val  = 0x00070000,
				},

		.type = CPG_T_CLK
	},

	{	/* Image Scaling Unit Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_14,
				.val  = 0x000000C0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_7,
				.val  = 0x000000C0,
				},

		.type = CPG_T_CLK
	},

	{	/* Image Scaling Unit Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_20,
				.val  = 0x00000001,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_10,
				.val  = 0x00000001,
				},

		.type = CPG_T_CLK
	},

	{	/* Camera Data Receive Unit (CRU) Part 1*/
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_13,
				.val  = 0x0000FFFC,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_6,
				.val  = 0xFFFC0000,
				},

		.type = CPG_T_CLK
	},

	{	/* Camera Data Receive Unit (CRU) Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_14,
				.val  = 0x00000003,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_7,
				.val  = 0x00000003,
				},

		.type = CPG_T_CLK
	},

	{	/* DSI */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_14,
				.val  = 0x00001F00,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_7,
				.val  = 0x00001F00,
				},

		.type = CPG_T_CLK
	},

	{	/* LCDC */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_14,
				.val  = 0x0000E000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_7,
				.val  = 0x0000E000,
				},

		.type = CPG_T_CLK
	},

	{	/* Serial Sound Interface (SSI) Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_15,
				.val  = 0x00000020,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_7,
				.val  = 0x00200000,
				},

		.type = CPG_T_CLK
	},

	{	/* Serial Sound Interface (SSI) Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_24,
				.val  = 0x000007FF,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_7,
				/* No monitor for the above clocks */
				.val  = 0x00000000,
				},

		.type = CPG_T_CLK
	},

	{	/* USB2.0 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_11,
				.val  = 0x000000F8,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_5,
				.val  = 0x00F80000,
				},

		.type = CPG_T_CLK
	},

	{	/* USB3 Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_10,
				.val  = 0x00008000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_5,
				.val  = 0x00008000,
				},

		.type = CPG_T_CLK
	},

	{	/* USB3 Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_11,
				.val  = 0x00000007,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_5,
				.val  = 0x00070000,
				},

		.type = CPG_T_CLK
	},

	{	/* I2C (IIC) */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_9,
				.val  = 0x0000FF80,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_4,
				.val  = 0x0FF80000,
				},

		.type = CPG_T_CLK
	},

	{	/* SCIF */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_8,
				.val  = 0x00008000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_4,
				.val  = 0x00008000,
				},

		.type = CPG_T_CLK
	},

	{	/* SCI Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_5,
				.val  = 0x0000E000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_2,
				.val  = 0xE0000000,
				},

		.type = CPG_T_CLK
	},

	{	/* SCI Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_6,
				.val  = 0x0000FFFF,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_3,
				.val  = 0x0000FFFF,
				},

		.type = CPG_T_CLK
	},

	{	/* SCI Part 3 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_7,
				.val  = 0x0000FFFF,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_3,
				.val  = 0xFFFF0000,
				},

		.type = CPG_T_CLK
	},

	{	/* SCI Part 4 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_8,
				.val  = 0x0000FFFF,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_4,
				.val  = 0x0000FFFF,
				},

		.type = CPG_T_CLK
	},

	{	/* SPI Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_5,
				.val  = 0x00001FF0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_2,
				.val  = 0x1FF00000,
				},

		.type = CPG_T_CLK
	},

	{	/* SPI Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_9,
				.val  = 0x00008000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_4,
				.val  = 0x80000000,
				},

		.type = CPG_T_CLK
	},

	{	/* SPI Part 3 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_10,
				.val  = 0x00000007,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_5,
				.val  = 0x00000007,
				},

		.type = CPG_T_CLK
	},

	{	/* CAN */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_9,
				.val  = 0x00007000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_4,
				.val  = 0x70000000,
				},

		.type = CPG_T_CLK
	},

	{	/* ADC Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_16,
				.val  = 0x00000180,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_8,
				.val  = 0x00000180,
				},

		.type = CPG_T_CLK
	},

	{	/* ADC Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_17,
				.val  = 0x00008000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_8,
				.val  = 0x80000000,
				},

		.type = CPG_T_CLK
	},
};

























static CPG_SETUP_DATA cpg_reset_tbl[] = {

	{	/* CM33 Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_1,
				.val  = 0x00000038,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_0,
				.val  = 0x000E0000,
				},

		.type = CPG_T_RST
	},

	{	/* CM33 Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_4,
				.val  = 0x00002800,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_1,
				.val  = 0x50000000,
				},

		.type = CPG_T_RST
	},

	{	/* CM33 Part 3 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_5,
				.val  = 0x00000001,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_2,
				.val  = 0x00000002,
				},

		.type = CPG_T_RST
	},

	{	/* CR8 Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_2,
				.val  = 0x00000FFF,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_0,
				.val  = 0xFFF00000,
				},

		.type = CPG_T_RST
	},

	{	/* CR8 Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_2,
				.val  = 0x00000001,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_1,
				.val  = 0x00000001,
				},

		.type = CPG_T_RST
	},

	{	/* MHU */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_5,
				.val  = 0x00000100,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_2,
				.val  = 0x00000200,
				},

		.type = CPG_T_RST
	},

	{	/* DMAC */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_3,
				.val  = 0x0000003E,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_1,
				.val  = 0x0000007C,
				},

		.type = CPG_T_RST
	},

	{	/* OSTM Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_6,
				.val  = 0x00006000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_2,
				.val  = 0xC0000000,
				},

		.type = CPG_T_RST
	},

	{	/* OSTM Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_6,
				.val  = 0x00008000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_3,
				.val  = 0x00000001,
				},

		.type = CPG_T_RST
	},

	{	/* OSTM Part 3 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_7,
				.val  = 0x0000001F,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_3,
				.val  = 0x0000003E,
				},

		.type = CPG_T_RST
	},

	{	/* GPT */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_5,
				.val  = 0x00001E00,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_2,
				.val  = 0x00003C00,
				},

		.type = CPG_T_RST
	},

	{	/* POEG Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_5,
				.val  = 0x0000E000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_2,
				.val  = 0x0001C000,
				},

		.type = CPG_T_RST
	},

	{	/* POEG Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_6,
				.val  = 0x0000001F,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_2,
				.val  = 0x003E0000,
				},

		.type = CPG_T_RST
	},

	{	/* WDT */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_7,
				.val  = 0x000001E0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_3,
				.val  = 0x000003C0,
				},

		.type = CPG_T_RST
	},

	{	/* DDR Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_11,
				.val  = 0x0000FFF8,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_5,
				.val  = 0x0001FFF0,
				},

		.type = CPG_T_RST
	},

	{	/* DDR Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_12,
				.val  = 0x0000001F,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_5,
				.val  = 0x003E0000,
				},

		CPG_T_RST
	},

	{	/* DDR Part 3 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_16,
				.val  = 0x00007080,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_7,
				.val  = 0xE1000000,
				},

		.type = CPG_T_RST
	},

	{	/* DDR Part 4 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_16,
				.val  = 0x00008000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_8,
				.val  = 0x00000001,
				},

		.type = CPG_T_RST
	},

	{	/* DDR Part 5 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_17,
				.val  = 0x000003FF,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_8,
				.val  = 0x000007FE,
				},

		.type = CPG_T_RST
	},

	{	/* SPI Part 1*/
		.reg =  {
				.addr = (uintptr_t)CPG_RST_7,
				.val  = 0x0000F800,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_3,
				.val  = 0x0001F000,
				},

		.type = CPG_T_RST
	},

	{	/* SPI Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_8,
				.val  = 0x00000001,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_3,
				.val  = 0x00020000,
				},

		.type = CPG_T_RST
	},

	{	/* SPI Part 3 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_10,
				.val  = 0x00000018,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_4,
				.val  = 0x00300000,
				},

		.type = CPG_T_RST
	},

	{	/* SDHI */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_10,
				.val  = 0x00000380,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_4,
				.val  = 0x07000000,
				},

		.type = CPG_T_RST
	},

	{	/* GPU */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_13,
				.val  = 0x0000E000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_6,
				.val  = 0x0001C000,
				},

		.type = CPG_T_RST
	},

	{	/* Image Scaling Unit */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_13,
				.val  = 0x00000060,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_6,
				.val  = 0x000000C0,
				},

		.type = CPG_T_RST
	},

	{	/* Camera Data Receive Unit (CRU) Part 1*/
		.reg =  {
				.addr = (uintptr_t)CPG_RST_12,
				.val  = 0x00007FE0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_5,
				.val  = 0xFFC00000,
				},

		.type = CPG_T_RST
	},

	{	/* Camera Data Receive Unit (CRU) Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_12,
				.val  = 0x00008000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_6,
				.val  = 0x00000001,
				},

		.type = CPG_T_RST
	},

	{	/* Camera Data Receive Unit (CRU) Part 3 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_13,
				.val  = 0x00000001,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_6,
				.val  = 0x00000002,
				},

		.type = CPG_T_RST
	},

	{	/* DSI */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_13,
				.val  = 0x00000180,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_6,
				.val  = 0x00000300,
				},

		.type = CPG_T_RST
	},

	{	/* LCDC */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_13,
				.val  = 0x00001000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_6,
				.val  = 0x00002000,
				},

		.type = CPG_T_RST
	},

	{	/* Serial Sound Interface (SSI) */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_14,
				.val  = 0x00000FFE,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_6,
				.val  = 0x1FFC0000,
				},

		.type = CPG_T_RST
	},

	{	/* USB2.0 Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_10,
				.val  = 0x00007000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_4,
				.val  = 0xE0000000,
				},

		.type = CPG_T_RST
	},

	{	/* USB2.0 Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_10,
				.val  = 0x00008000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_5,
				.val  = 0x00000001,
				},

		.type = CPG_T_RST
	},

	{	/* USB3 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_10,
				.val  = 0x00000C00,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_4,
				.val  = 0x18000000,
				},

		.type = CPG_T_RST
	},

	{	/* I2C (IIC) Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_9,
				.val  = 0x0000FF00,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_4,
				.val  = 0x0001FE00,
				},

		.type = CPG_T_RST
	},

	{	/* I2C (IIC) Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_10,
				.val  = 0x00000001,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_4,
				.val  = 0x00020000,
				},

		.type = CPG_T_RST
	},

	{	/* SCIF */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_9,
				.val  = 0x00000020,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_4,
				.val  = 0x00000040,
				},

		.type = CPG_T_RST
	},

	{	/* SCI Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_8,
				.val  = 0x00007FFE,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_3,
				.val  = 0xFFFC0000,
				},

		.type = CPG_T_RST
	},

	{	/* SCI Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_8,
				.val  = 0x00008000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_4,
				.val  = 0x00000001,
				},

		.type = CPG_T_RST
	},

	{	/* SCI Part 3 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_9,
				.val  = 0x0000003F,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_4,
				.val  = 0x0000007E,
				},

		.type = CPG_T_RST
	},

	{	/* SPI Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_7,
				.val  = 0x0000F800,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_3,
				.val  = 0x0001F000,
				},

		.type = CPG_T_RST
	},

	{	/* SPI Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_8,
				.val  = 0x00000001,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_3,
				.val  = 0x00020000,
				},

		.type = CPG_T_RST
	},

	{	/* SPI Part 3 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_10,
				.val  = 0x00000018,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_4,
				.val  = 0x00300000,
				},

		.type = CPG_T_RST
	},

	{	/* CAN */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_10,
				.val  = 0x00000006,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_4,
				.val  = 0x000C0000,
				},

		.type = CPG_T_RST
	},

	{	/* ADC */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_15,
				.val  = 0x00000040,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_7,
				.val  = 0x00000080,
				},

		.type = CPG_T_RST
	},


#if 0
	{	/*  */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_5,
				.val  = 0x00000000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_2,
				.val  = 0x00000000,
				},

		.type = CPG_T_RST
	},
#endif

#if 0
	{		/* MTU */
		(uintptr_t)CPG_RST_MTU,
		(uintptr_t)CPG_RSTMON_MTU,
		0x00010000,
		CPG_T_RST
	},

	{		/* POE3 */
		(uintptr_t)CPG_RST_POE3,
		(uintptr_t)CPG_RSTMON_POE3,
		0x00010000,
		CPG_T_RST
	},

	{		/* H.264 codec */
		(uintptr_t)CPG_RST_H264,
		(uintptr_t)CPG_RSTMON_H264,
		0x00030003,
		CPG_T_RST
	},

	{		/* Sampling Rate Converter */
		(uintptr_t)CPG_RST_SRC,
		(uintptr_t)CPG_RSTMON_SRC,
		0x00010000,
		CPG_T_RST
	},

	{		/* ETHER */
		(uintptr_t)CPG_RST_ETH,
		(uintptr_t)CPG_RSTMON_ETH,
		0x00030000,
		CPG_T_RST
	},

	{		/* IrDA */
		(uintptr_t)CPG_RST_IRDA,
		(uintptr_t)CPG_RSTMON_IRDA,
		0x00010000,
		CPG_T_RST
	},

	{		/* GPIO */
		(uintptr_t)CPG_RST_GPIO,
		(uintptr_t)CPG_RSTMON_GPIO,
		0x00070007,
		CPG_T_RST
	},

	{	/* Thermal Sensor Unit */
		(uintptr_t)CPG_RST_TSU,
		(uintptr_t)CPG_RSTMON_TSU,
		0x00010000,
		CPG_T_RST
	},
#endif
};
#if 0
static CPG_REG_SETTING cpg_static_select_tbl[] = {
	{ (uintptr_t)CPG_PL3A_DDIV,             0x01000100 },
	{ (uintptr_t)CPG_PL3_SSEL,              0x01000000 },
};

static CPG_REG_SETTING cpg_dynamic_select_tbl[] = {
	{ (uintptr_t)CPG_PL4_DSEL,              0x00010001 },
	{ (uintptr_t)CPG_PL2SDHI_DSEL,          0x00110022 },
};

#define CPG_SEL_PLL1_ON_OFF					(0)
#define CPG_SEL_PLL2_1_ON_OFF				(1)
#define CPG_SEL_PLL2_2_ON_OFF				(2)
#define CPG_SEL_PLL3_1_ON_OFF				(3)
#define CPG_SEL_PLL3_2_ON_OFF				(4)
#define CPG_SEL_PLL3_3_ON_OFF				(5)
#define CPG_SEL_PLL5_1_ON_OFF				(6)
#define CPG_SEL_PLL5_3_ON_OFF				(7)
#define CPG_SEL_PLL5_4_ON_OFF				(8)
#define CPG_SEL_PLL6_1_ON_OFF				(9)
#define CPG_SEL_GPU1_1_ON_OFF				(10)
#define CPG_SEL_GPU1_2_ON_OFF				(11)
#define CPG_SEL_GPU2_ON_OFF					(12)

static CPG_REG_SETTING cpg_sel_pll1_on_off[] = {
	{(uintptr_t)CPG_CLKON_CA55, 0x00010001 }
};

static CPG_REG_SETTING cpg_sel_pll2_1_on_off[] = {
	{(uintptr_t)CPG_CLKON_ADC, 0x00010001 },
	{(uintptr_t)CPG_CLKON_TSU, 0x00010001 },
	{(uintptr_t)CPG_CLKON_SDHI, 0x00770077 }
};

static CPG_REG_SETTING cpg_sel_pll2_2_on_off[] = {
	{(uintptr_t)CPG_CLKON_SDHI, 0x00770077 },
#if !RZG2UL
	{(uintptr_t)CPG_CLKON_MIPI_DSI, 0x00200020 },
#endif
};

static CPG_REG_SETTING cpg_sel_pll3_1_on_off[] = {
	{(uintptr_t)CPG_CLKON_AXI_ACPU_BUS, 0x000F000F },
	{(uintptr_t)CPG_CLKON_AXI_COM_BUS, 0x00030003 },
	{(uintptr_t)CPG_CLKON_AXI_DEFAULT_SLV, 0x00010001 },
	{(uintptr_t)CPG_CLKON_AXI_MCPU_BUS, 0x01930193 },
	{(uintptr_t)CPG_CLKON_AXI_TZCDDR, 0x001F001F },
	{(uintptr_t)CPG_CLKON_AXI_VIDEO_BUS, 0x00030003 },
	{(uintptr_t)CPG_CLKON_CA55, 0x001E001E },
	{(uintptr_t)CPG_CLKON_CM33, 0x00010001 },
	{(uintptr_t)CPG_CLKON_CRU, 0x000C000C },
	{(uintptr_t)CPG_CLKON_CST, 0x07FD07FD },
	{(uintptr_t)CPG_CLKON_DAMC_REG, 0x00030003 },
	{(uintptr_t)CPG_CLKON_DDR, 0x00030003 },
	{(uintptr_t)CPG_CLKON_ETH, 0x00030003 },
	{(uintptr_t)CPG_CLKON_GIC600, 0x00010001 },
	{(uintptr_t)CPG_CLKON_GPU, 0x00070007 },
	{(uintptr_t)CPG_CLKON_H264, 0x00010001 },
	{(uintptr_t)CPG_CLKON_IA55, 0x00030003 },
	{(uintptr_t)CPG_CLKON_IM33, 0x00030003 },
	{(uintptr_t)CPG_CLKON_ISU, 0x00030003 },
	{(uintptr_t)CPG_CLKON_JAUTH, 0x00010001 },
	{(uintptr_t)CPG_CLKON_LCDC, 0x00010001 },
	{(uintptr_t)CPG_CLKON_MIPI_DSI, 0x000C000C },
	{(uintptr_t)CPG_CLKON_OTP, 0x00020002 },
	{(uintptr_t)CPG_CLKON_PERI_COM, 0x00030003 },
	{(uintptr_t)CPG_CLKON_PERI_CPU, 0x000D000D },
	{(uintptr_t)CPG_CLKON_PERI_DDR, 0x00010001 },
	{(uintptr_t)CPG_CLKON_PERI_VIDEO, 0x00070007 },
	{(uintptr_t)CPG_CLKON_REG0_BUS, 0x00010001 },
	{(uintptr_t)CPG_CLKON_REG1_BUS, 0x00030003 },
	{(uintptr_t)CPG_CLKON_ROM, 0x00010001 },
	{(uintptr_t)CPG_CLKON_SDHI, 0x00880088 },
	{(uintptr_t)CPG_CLKON_SRAM_ACPU, 0x00010001 },
	{(uintptr_t)CPG_CLKON_SRAM_MCPU, 0x00010001 },
	{(uintptr_t)CPG_CLKON_SYSC, 0x00020002 },
	{(uintptr_t)CPG_CLKON_TSIPG, 0x00030003 },
	{(uintptr_t)CPG_CLKON_USB, 0x000F000F }
};

static CPG_REG_SETTING cpg_sel_pll3_2_on_off[] = {
	{(uintptr_t)CPG_CLKON_CRU, 0x00030003 },
	{(uintptr_t)CPG_CLKON_MIPI_DSI, 0x00020002 },
	{(uintptr_t)CPG_CLKON_GPU, 0x00010001 },
==
	{(uintptr_t)CPG_CLKON_SPI_MULTI, 0x00030003 },
	{(uintptr_t)CPG_CLKON_AXI_MCPU_BUS, 0x02080208 },
};

static CPG_REG_SETTING cpg_sel_pll3_3_on_off[] = {
	{(uintptr_t)CPG_CLKON_SPI_MULTI, 0x00030003 },
	{(uintptr_t)CPG_CLKON_AXI_MCPU_BUS, 0x02080208 },
};

static CPG_REG_SETTING cpg_sel_pll5_1_on_off[] = {
	{(uintptr_t)CPG_CLKON_MIPI_DSI, 0x00010001 },
	{(uintptr_t)CPG_CLKON_CRU, 0x00100010 },
	{(uintptr_t)CPG_CLKON_MIPI_DSI, 0x00100010 },
	{(uintptr_t)CPG_CLKON_LCDC, 0x00020002 }
};

static CPG_REG_SETTING cpg_sel_pll5_3_on_off[] = {
	{(uintptr_t)CPG_CLKON_MIPI_DSI, 0x00100010 },
	{(uintptr_t)CPG_CLKON_LCDC, 0x00020002 }
};

static CPG_REG_SETTING cpg_sel_pll5_4_on_off[] = {
	{(uintptr_t)CPG_CLKON_MIPI_DSI, 0x00100010 },
	{(uintptr_t)CPG_CLKON_LCDC, 0x00020002 }
};

static CPG_REG_SETTING cpg_sel_pll6_1_on_off[] = {
	{(uintptr_t)CPG_CLKON_GPU, 0x00010001 }
};

static CPG_REG_SETTING cpg_sel_gpu1_1_on_off[] = {
	{(uintptr_t)CPG_CLKON_GPU, 0x00010001 }
};

static CPG_REG_SETTING cpg_sel_gpu1_2_on_off[] = {
	{(uintptr_t)CPG_CLKON_GPU, 0x00010001 }
};

static CPG_REG_SETTING cpg_sel_gpu2_on_off[] = {
	{(uintptr_t)CPG_CLKON_GPU, 0x00010001 }
};
#endif

static void cpg_ctrl_clkrst(CPG_SETUP_DATA const *array, uint32_t num)
{
	int i;
	uint32_t mask;
	uint32_t cmp;

	for (i = 0; i < num; i++, array++) {
		mmio_write_32(array->reg.addr, array->reg.val);

		/*
		 * This generic function needs to handle case where Montitor for clock
		 * is looking for a HIGH as clock active whereas  Montitoring a reset
		 * it is looking for a LOW to indicate reset release.
		 */
		mask = array->mon.val;
		cmp  = array->mon.val;
		if (array->type == CPG_T_RST)
			cmp = ~cmp;

		while ((mmio_read_32(array->mon.addr) & mask) != (cmp & mask))
					;
	}
}
#if 0
static void cpg_selector_on_off(uint32_t sel, uint8_t flag)
{
	uint32_t cnt;
	uint32_t tbl_num;
	CPG_REG_SETTING *ptr;

	switch (sel) {
	case CPG_SEL_PLL1_ON_OFF:
		tbl_num = ARRAY_SIZE(cpg_sel_pll1_on_off);
		ptr = &cpg_sel_pll1_on_off[0];
		break;
	case CPG_SEL_PLL2_1_ON_OFF:
		tbl_num = ARRAY_SIZE(cpg_sel_pll2_1_on_off);
		ptr = &cpg_sel_pll2_1_on_off[0];
		break;
	case CPG_SEL_PLL2_2_ON_OFF:
		tbl_num = ARRAY_SIZE(cpg_sel_pll2_2_on_off);
		ptr = &cpg_sel_pll2_2_on_off[0];
		break;
	case CPG_SEL_PLL3_1_ON_OFF:
		tbl_num = ARRAY_SIZE(cpg_sel_pll3_1_on_off);
		ptr = &cpg_sel_pll3_1_on_off[0];
		break;
	case CPG_SEL_PLL3_2_ON_OFF:
		tbl_num = ARRAY_SIZE(cpg_sel_pll3_2_on_off);
		ptr = &cpg_sel_pll3_2_on_off[0];
		break;
	case CPG_SEL_PLL3_3_ON_OFF:
		tbl_num = ARRAY_SIZE(cpg_sel_pll3_3_on_off);
		ptr = &cpg_sel_pll3_3_on_off[0];
		break;
	case CPG_SEL_PLL5_1_ON_OFF:
		tbl_num = ARRAY_SIZE(cpg_sel_pll5_1_on_off);
		ptr = &cpg_sel_pll5_1_on_off[0];
		break;
	case CPG_SEL_PLL5_3_ON_OFF:
		tbl_num = ARRAY_SIZE(cpg_sel_pll5_3_on_off);
		ptr = &cpg_sel_pll5_3_on_off[0];
		break;
	case CPG_SEL_PLL5_4_ON_OFF:
		tbl_num = ARRAY_SIZE(cpg_sel_pll5_4_on_off);
		ptr = &cpg_sel_pll5_4_on_off[0];
		break;
	case CPG_SEL_PLL6_1_ON_OFF:
		tbl_num = ARRAY_SIZE(cpg_sel_pll6_1_on_off);
		ptr = &cpg_sel_pll6_1_on_off[0];
		break;
	case CPG_SEL_GPU1_1_ON_OFF:
		tbl_num = ARRAY_SIZE(cpg_sel_gpu1_1_on_off);
		ptr = &cpg_sel_gpu1_1_on_off[0];
		break;
	case CPG_SEL_GPU1_2_ON_OFF:
		tbl_num = ARRAY_SIZE(cpg_sel_gpu1_2_on_off);
		ptr = &cpg_sel_gpu1_2_on_off[0];
		break;
	case CPG_SEL_GPU2_ON_OFF:
		tbl_num = ARRAY_SIZE(cpg_sel_gpu2_on_off);
		ptr = &cpg_sel_gpu2_on_off[0];
		break;
	default:
		break;
	}

	for (cnt = 0; cnt < tbl_num; cnt++) {
		if (flag == CPG_ON) {
			mmio_write_32(ptr[cnt].addr, (mmio_read_32(ptr[cnt].addr) | ptr[cnt].val));
		} else {
			mmio_write_32(ptr[cnt].addr, (mmio_read_32(ptr[cnt].addr) | (ptr[cnt].val & 0xFFFF0000)));
		}
	}

}

static void cpg_pll_start_146(CPG_PLL_SETDATA_146 *pdata)
{
	mmio_write_32(pdata->clk1_dat.addr, pdata->clk1_dat.val);
	mmio_write_32(pdata->clk2_dat.addr, pdata->clk2_dat.val);
	mmio_write_32(pdata->stby_dat.addr, pdata->stby_dat.val);
}

/* It is assumed that the PLL has stopped by the time this function is executed. */
static void cpg_pll_setup(void)
{
#if !DEBUG_RZV2H_FPGA
	uint32_t val = 0;

	/* PLL4 startup */
	/* PLL4 standby mode transition confirmation */
	do {
		val = mmio_read_32(CPG_PLL4_MON);
	} while ((val & (PLL4_MON_PLL4_RESETB | PLL4_MON_PLL4_LOCK)) != 0);

	/* PLL6 startup */
	/* PLL6 standby mode transition confirmation */
	do {
		val = mmio_read_32(CPG_PLL6_MON);
	} while ((val & (PLL6_MON_PLL6_RESETB | PLL6_MON_PLL6_LOCK)) != 0);
#endif

	/* Set PLL4 to normal mode */
	cpg_pll_start_146(&cpg_pll4_setdata);
	/* Set PLL6 to normal mode */
	cpg_pll_start_146(&cpg_pll6_setdata);

#if !DEBUG_RZV2H_FPGA
	/* PLL4 normal mode transition confirmation */
	do {
		val = mmio_read_32(CPG_PLL4_MON);
	} while ((val & (PLL4_MON_PLL4_RESETB | PLL4_MON_PLL4_LOCK)) == 0);

	/* PLL6 normal mode transition confirmation */
	do {
		val = mmio_read_32(CPG_PLL6_MON);
	} while ((val & (PLL6_MON_PLL6_RESETB | PLL6_MON_PLL6_LOCK)) == 0);
#endif
}

static void cpg_div_sel_setup(CPG_REG_SETTING *tbl, uint32_t size)
{
	int cnt;

	for (cnt = 0; cnt < size; cnt++, tbl++) {
		mmio_write_32(tbl->addr, tbl->val);
	}

#if 0
#if !DEBUG_RZV2H_FPGA
	/* Wait for completion of settings */
	while (mmio_read_32(CPG_CLKSTATUS) != 0)
		;
#endif
#endif
}

static void cpg_div_sel_static_setup(void)
{
	cpg_div_sel_setup(cpg_static_select_tbl, ARRAY_SIZE(cpg_static_select_tbl));
}

static void cpg_div_sel_dynamic_setup(void)
{
	cpg_div_sel_setup(cpg_dynamic_select_tbl, ARRAY_SIZE(cpg_dynamic_select_tbl));
}
#endif
static void cpg_clk_on_setup(void)
{
	cpg_ctrl_clkrst(&cpg_clk_on_tbl[0], ARRAY_SIZE(cpg_clk_on_tbl));
}

static void cpg_reset_setup(void)
{
	cpg_ctrl_clkrst(&cpg_reset_tbl[0], ARRAY_SIZE(cpg_reset_tbl));
}

void cpg_active_ddr(void (*disable_phy)(void))
{
#if 0
	/* Assert the reset of DDRTOP */
	mmio_write_32(CPG_RST_DDR, 0x005F0000 | (CPG_RST_DDR_OPT_VALUE << 16));
	mmio_write_32(CPG_OTHERFUNC2_REG, 0x00010000);
	while ((mmio_read_32(CPG_RSTMON_DDR) & 0x0000005F) != 0x0000005F)
		;

	/* Start the clocks of DDRTOP */
	mmio_write_32(CPG_CLKON_DDR, 0x00030003);
	while ((mmio_read_32(CPG_CLKMON_DDR) & 0x00000003) != 0x00000003)
		;

	udelay(1);

	/* De-assert rst_n */
	mmio_write_32(CPG_OTHERFUNC2_REG, 0x00010001);

	udelay(1);

	/* De-assert PRESETN */
	mmio_write_32(CPG_RST_DDR, 0x00020002);
	while ((mmio_read_32(CPG_RSTMON_DDR) & 0x00000002) != 0x00000000)
		;

	udelay(1);

	disable_phy();

	/* De-assert axiY_ARESETn, regARESETn, reset_n */
	mmio_write_32(CPG_RST_DDR, 0x005D005D | (CPG_RST_DDR_OPT_VALUE << 16) | CPG_RST_DDR_OPT_VALUE);
	while ((mmio_read_32(CPG_RSTMON_DDR) & 0x0000005D) != 0x00000000)
		;
#endif
	udelay(1);
}

void cpg_reset_ddr_mc(void)
{
#if 0
	/* Assert rst_n, axiY_ARESETn, regARESETn */
	mmio_write_32(CPG_RST_DDR, 0x005C0000 | (CPG_RST_DDR_OPT_VALUE << 16));
	mmio_write_32(CPG_OTHERFUNC2_REG, 0x00010000);
	while ((mmio_read_32(CPG_RSTMON_DDR) & 0x0000005C) != 0x0000005C)
		;

	udelay(1);

	/* De-assert rst_n */
	mmio_write_32(CPG_OTHERFUNC2_REG, 0x00010001);

	udelay(1);

	/* De-assert axiY_ARESETn, regARESETn */
	mmio_write_32(CPG_RST_DDR, 0x005C005C | (CPG_RST_DDR_OPT_VALUE << 16) | CPG_RST_DDR_OPT_VALUE);
	while ((mmio_read_32(CPG_RSTMON_DDR) & 0x0000005C) != 0x00000000)
		;
#endif
	udelay(1);
}

void cpg_early_setup(void)
{
	cpg_ctrl_clkrst(&early_setup_tbl[0], ARRAY_SIZE(early_setup_tbl));
}
#if 0
static void cpg_wdtrst_sel_setup(void)
{
	uint32_t reg;

	reg = mmio_read_32(CPG_WDTRST_SEL);

	reg |=
		WDTRST_SEL_WDTRSTSEL0 | WDTRST_SEL_WDTRSTSEL0_WEN |
		WDTRST_SEL_WDTRSTSEL1 | WDTRST_SEL_WDTRSTSEL1_WEN |
		WDTRST_SEL_WDTRSTSEL2 | WDTRST_SEL_WDTRSTSEL2_WEN;
	mmio_write_32(CPG_WDTRST_SEL, reg);
}
#endif
void cpg_setup(void)
{
	//cpg_selector_on_off(CPG_SEL_PLL3_3_ON_OFF, CPG_OFF);
	//cpg_div_sel_static_setup();
	//cpg_selector_on_off(CPG_SEL_PLL3_3_ON_OFF, CPG_ON);
	//cpg_pll_setup();
	cpg_clk_on_setup();
	cpg_reset_setup();
	//cpg_div_sel_dynamic_setup();
	//cpg_wdtrst_sel_setup();
}

