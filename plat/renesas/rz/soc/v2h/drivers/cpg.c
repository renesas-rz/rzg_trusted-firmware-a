/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <cpg_regs.h>
#include <lib/mmio.h>
#include <drivers/delay_timer.h>
#include <rz_soc_def.h>


#define	CPG_OFF							(0)
#define	CPG_ON							(1)

#define CPG_T_CLK						(0)
#define CPG_T_RST						(1)

/*
 * Write given MSTOP register to remove module stops of bits in given 'val'. Corrosponding MSTOP bit
 * enable in top word needs to be set and just zero to MSTOP bits, therefore **no** Read-Modify-Write is required.
 */
#define REMOVE_MSTOPS_W(reg, val)		mmio_write_32((reg), ((val) << 16U))


typedef struct {
	uintptr_t addr;
	uint32_t  val;
} CPG_REG_SETTING;

typedef struct {
	CPG_REG_SETTING reg;
	CPG_REG_SETTING mon;
	uint32_t  type;
} CPG_SETUP_DATA;

typedef struct {
	CPG_REG_SETTING stby;
	CPG_REG_SETTING clk1;
	CPG_REG_SETTING clk2;
	CPG_REG_SETTING mon;
} CPG_PLL_SETTINGS;


static CPG_PLL_SETTINGS cpg_pll_tbl[] = {
	{	/* CM33 */
		.stby = {
				.addr = (uintptr_t)NULL,			/* Reserved */
				.val  = 0,
				},

		.clk1 = {
				.addr = (uintptr_t)NULL,			/* Reserved */
				.val  = 0,
				},


		.clk2 = {
				.addr = (uintptr_t)NULL,			/* Reserved */
				.val  = 0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_PLLCM33_MON,
				.val  = 0,
				},
	},

	{	/* CLN */
		.stby = {
				.addr = (uintptr_t)NULL,			/* Auto turn-on */
				.val  = 0,
				},

		.clk1 = {
				.addr = (uintptr_t)NULL,			/* Reserved */
				.val  = 0,
				},


		.clk2 = {
				.addr = (uintptr_t)NULL,			/* Reserved */
				.val  = 0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_PLLCLN_MON,
				.val  = 0,
				},
	},

	{	/* DTY */
		.stby = {
				.addr = (uintptr_t)NULL,			/* Auto turn-on */
				.val  = 0,
				},

		.clk1 = {
				.addr = (uintptr_t)NULL,			/* Reserved */
				.val  = 0,
				},


		.clk2 = {
				.addr = (uintptr_t)NULL,			/* Reserved */
				.val  = 0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_PLLDTY_MON,
				.val  = 0,
				},
	},

	{	/* CA55 */
		.stby = {
				.addr = (uintptr_t)NULL,			/* Auto turn-on */
				.val  = 0,
				},

		.clk1 = {
				.addr = (uintptr_t)NULL,			/* USe default value */
				.val  = 0,
				},

		.clk2 = {
				.addr = (uintptr_t)NULL,			/* USe default value */
				.val  = 0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_PLLCA55_MON,
				.val  = 0,
				},
	},

	{	/* VDO */
		.stby = {
				.addr = (uintptr_t)NULL,			/* Auto turn-on */
				.val  = 0,
				},

		.clk1 = {
				.addr = (uintptr_t)NULL,			/* Reserved */
				.val  = 0,
				},


		.clk2 = {
				.addr = (uintptr_t)NULL,			/* Reserved */
				.val  = 0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_PLLVDO_MON,
				.val  = 0,
				},
	},

	{	/* ETH */
		.stby = {
				.addr = (uintptr_t)CPG_PLLETH_STBY,
				.val  = 0x00010001,
				},

		.clk1 = {
				.addr = (uintptr_t)NULL,			/* Reserved */
				.val  = 0,
				},


		.clk2 = {
				.addr = (uintptr_t)NULL,			/* Reserved */
				.val  = 0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_PLLETH_MON,
				.val  = 0,
				},
	},

	{	/* DSI */
		.stby = {
				.addr = (uintptr_t)CPG_PLLDSI_STBY,
				.val  = 0x00050005,
				},

		.clk1 = {
				.addr = (uintptr_t)CPG_PLLDSI_CLK1,
				.val  = 0x00003182,
				},


		.clk2 = {
				.addr = (uintptr_t)CPG_PLLDSI_CLK2,
				.val  = 0x000C1803,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_PLLDSI_MON,
				.val  = 0,
				},
	},

	/* CPG_PLLDDR0 and CPG_PLLDDR1 are set in DDR setup routine */

	{	/* GPU */
		.stby = {
				.addr = (uintptr_t)CPG_PLLGPU_STBY,
				.val  = 0x00010001,
				},

		.clk1 = {
				.addr = (uintptr_t)CPG_PLLGPU_CLK1,
				.val  = 0x00003482,
				},


		.clk2 = {
				.addr = (uintptr_t)CPG_PLLGPU_CLK2,
				.val  = 0x000C1A01,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_PLLGPU_MON,
				.val  = 0,
				},
	},

	{	/* DRP */
		.stby = {
				.addr = (uintptr_t)NULL,		/* Auto turn-on */
				.val  = 0,
				},

		.clk1 = {
				.addr = (uintptr_t)NULL,		/* USe default value */
				.val  = 0,
				},


		.clk2 = {
				.addr = (uintptr_t)NULL,		/* USe default value */
				.val  = 0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_PLLDRP_MON,
				.val  = 0,
				},
	},
};

static CPG_SETUP_DATA cpg_clk_on_tbl[] = {
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
				.val  = 0x0000001E,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_0,
				.val  = 0x0000001E,
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
				.val  = 0x000007F8,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_1,
				.val  = 0x07F80000,
				},

		.type = CPG_T_CLK
	},

	{	/* WDT Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_4,
				.val  = 0x0000E00,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_2,
				.val  = 0x0000E00,
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
				.val  = 0x00000003,
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
				.val  = 0x00000FF8,
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
				.val  = 0x00007FFF,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_4,
				.val  = 0x00007FFF,
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

	{	/* CRC*/
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_0,
				.val  = 0x00000040,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_0,
				.val  = 0x00000040,
				},

		.type = CPG_T_CLK
	},

	{	/* CMTW Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_3,
				.val  = 0x0000F800,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_1,
				.val  = 0xF8000000,
				},

		.type = CPG_T_CLK
	},

	{	/* CMTW Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_4,
				.val  = 0x00000007,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_2,
				.val  = 0x00000007,
				},

		.type = CPG_T_CLK
	},

	{	/* RTC */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_5,
				.val  = 0x00000008,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_2,
				.val  = 0x00080000,
				},

		.type = CPG_T_CLK
	},

	{	/* I3C */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_9,
				.val  = 0x00000007,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_4,
				.val  = 0x00070000,
				},

		.type = CPG_T_CLK
	},

	{	/* GBETH Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_11,
				.val  = 0x0000FF00,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_5,
#if 0 //TODO: KTG: Kirk had difficulties with this setting on DevBoard
				.val  = 0xFF000000,
#else
				.val  = 0x75000000,
#endif
				},

		.type = CPG_T_CLK
	},

	{	/* GBETH Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_12,
				.val  = 0x0000000F,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_6,
#if 0 //TODO: KTG: Kirk had difficulties with this setting on DevBoard
				.val  = 0x0000000F,
#else
				.val  = 0x0000000D,
#endif
				},

		.type = CPG_T_CLK
	},

	{	/* PCIE Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_12,
				.val  = 0x00000030,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_6,
				.val  = 0x00000030,
				},

		.type = CPG_T_CLK
	},

	{	/* ISP */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_14,
				.val  = 0x0000003C,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_7,
				.val  = 0x0000003C,
				},

		.type = CPG_T_CLK
	},

	{	/* VCD */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_15,
				.val  = 0x00000018,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_7,
				.val  = 0x00180000,
				},

		.type = CPG_T_CLK
	},

	{	/* SCU */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_15,
				.val  = 0x000000C0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_7,
				.val  = 0x00C00000,
				},

		.type = CPG_T_CLK
	},

	{	/* ADG */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_15,
				.val  = 0x00003E00,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_7,
#if 0 //TODO: KTG: Kirk had difficulties with this setting on DevBoard
				.val  = 0x3E000000,
#else
				.val  = 0x06000000
#endif
				},

		.type = CPG_T_CLK
	},

	{	/* SPDIF Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_15,
				.val  = 0x0000C000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_7,
				.val  = 0xC0000000,
				},

		.type = CPG_T_CLK
	},

	{	/* SPDIF Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_16,
				.val  = 0x00000001,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_8,
				.val  = 0x00000001,
				},

		.type = CPG_T_CLK
	},

	{	/* PDM */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_16,
				.val  = 0x0000007E,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_8,
				.val  = 0x0000007E,
				},

		.type = CPG_T_CLK
	},

	{	/* TSU */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_16,
				.val  = 0x00000600,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_8,
				.val  = 0x00000600,
				},

		.type = CPG_T_CLK
	},

	{	/* DRP */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_17,
				.val  = 0x0000007F,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_8,
				.val  = 0x007F0000,
				},

		.type = CPG_T_CLK
	}
};

static CPG_SETUP_DATA cpg_reset_tbl[] = {

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
				.val  = 0x00001000,
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
				.val  = 0x00000008,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_1,
				.val  = 0x00000010,
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
				.val  = 0x000001C0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_3,
				.val  = 0x00000380,
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
				.val  = 0x0000001F,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_4,
				.val  = 0x0000003E,
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

	{	/* ICU */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_3,
				.val  = 0x00000040,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_1,
				.val  = 0x00000080,
				},

		.type = CPG_T_RST
	},

	{	/* CRC */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_3,
				.val  = 0x00000080,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_1,
				.val  = 0x00000100,
				},

		.type = CPG_T_RST
	},

	{	/* CMTW */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_6,
				.val  = 0x00001FE0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_2,
				.val  = 0x3FC00000,
				},

		.type = CPG_T_RST
	},

	{	/* RTC */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_7,
				.val  = 0x00000600,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_3,
				.val  = 0x00000C00,
				},

		.type = CPG_T_RST
	},

	{	/* I3C */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_9,
				.val  = 0x000000C0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_4,
				.val  = 0x00000180,
				},

		.type = CPG_T_RST
	},

	{	/* GBETH */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_11,
				.val  = 0x00000003,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_5,
				.val  = 0x00000006,
				},

		.type = CPG_T_RST
	},

	{	/* PCIE */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_11,
				.val  = 0x00000004,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_5,
				.val  = 0x00000008,
				},

		.type = CPG_T_RST
	},

	{	/* ISP */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_13,
				.val  = 0x0000001E,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_6,
				.val  = 0x0000003C,
				},

		.type = CPG_T_RST
	},

	{	/* VCD*/
		.reg =  {
				.addr = (uintptr_t)CPG_RST_14,
				.val  = 0x00000001,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_6,
				.val  = 0x00020000,
				},

		.type = CPG_T_RST
	},

	{	/* SCU */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_14,
				.val  = 0x00001000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_6,
				.val  = 0x20000000,
				},

		.type = CPG_T_RST
	},

	{	/* DMAC */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_14,
				.val  = 0x00002000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_6,
				.val  = 0x40000000,
				},

		.type = CPG_T_RST
	},

	{	/* ADG */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_14,
				.val  = 0x00004000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_6,
				.val  = 0x80000000,
				},

		.type = CPG_T_RST
	},

	{	/* SDPDIF Part 1*/
		.reg =  {
				.addr = (uintptr_t)CPG_RST_14,
				.val  = 0x00008000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_7,
				.val  = 0x00000001,
				},

		.type = CPG_T_RST
	},

	{	/* SPDIF Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_15,
				.val  = 0x00000003,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_7,
				.val  = 0x00000006,
				},

		.type = CPG_T_RST
	},

	{	/* PDM */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_15,
				.val  = 0x0000003C,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_7,
				.val  = 0x00000078,
				},

		.type = CPG_T_RST
	},

	{	/* TSU */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_15,
				.val  = 0x00000180,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_7,
				.val  = 0x00000300,
				},

		.type = CPG_T_RST
	},

	{	/* DRP */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_15,
				.val  = 0x00003000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_7,
				.val  = 0x00006000,
				},

		.type = CPG_T_RST
	}

};

static CPG_REG_SETTING cpg_static_select_tbl[] = {
	{ (uintptr_t)CPG_CSDIV0,				0x00000000 },
	{ (uintptr_t)CPG_CSDIV1,				0x00000000 },
};

static CPG_REG_SETTING cpg_dynamic_select_tbl[] = {
	{ (uintptr_t)CPG_CDDIV0,				0x00000000 },
	{ (uintptr_t)CPG_CDDIV1,				0x00000000 },
	{ (uintptr_t)CPG_CDDIV2,				0x00000000 },
	{ (uintptr_t)CPG_CDDIV3,				0x10001000 },
	{ (uintptr_t)CPG_CDDIV4,				0x01110111 },
};

static void cpg_ctrl_clkrst(CPG_SETUP_DATA const *array, uint32_t num)
{
	int i;
	uint32_t mask;
	uint32_t cmp;

	for (i = 0; i < num; i++, array++) {
		/*
		 * Upper 16bits are enables for lower 16bits so write the upper 16bits with same value as lower value
		 */
		uint32_t val = (array->reg.val & 0xFFFF) | ((array->reg.val & 0xFFFF) << 16);

		mmio_write_32(array->reg.addr, val);

		/*
		 * This generic function needs to handle case where Montitor for clock
		 * is looking for a HIGH as clock active whereas Montitoring a reset
		 * it is looking for a LOW to indicate reset release.
		 */
		mask = array->mon.val;
		cmp  = mask;
		if (array->type == CPG_T_RST)
			cmp = ~cmp;

		while ((mmio_read_32(array->mon.addr) & mask) != (cmp & mask))
			;
	}
}

/* It is assumed that the PLL has stopped by the time this function is executed. */
static void cpg_pll_setup(void)
{
	int i;
	int pll_num = ARRAY_SIZE(cpg_pll_tbl);
	CPG_PLL_SETTINGS const *p_pll = &cpg_pll_tbl[0];
	uint32_t val;

	for (i = 0; i < pll_num; i++, p_pll++) {
		if (p_pll->clk1.addr != (uintptr_t)NULL) {
			mmio_write_32(p_pll->clk1.addr, p_pll->clk1.val);
		}

		if (p_pll->clk2.addr != (uintptr_t)NULL) {
			mmio_write_32(p_pll->clk2.addr, p_pll->clk2.val);
		}

		if (p_pll->stby.addr != (uintptr_t)NULL) {
			mmio_write_32(p_pll->stby.addr, p_pll->stby.val);
		}

		/* PLL active confirmation */
		do {
			val = mmio_read_32(p_pll->mon.addr);
		} while ((val & (CPG_PLL_MON_PLLn_RESETB | CPG_PLL_MON_PLLn_LOCK))
					!= (CPG_PLL_MON_PLLn_RESETB | CPG_PLL_MON_PLLn_LOCK));
	}
}

static void cpg_div_sel_setup(CPG_REG_SETTING *tbl, uint32_t size)
{
	int cnt;

	for (cnt = 0; cnt < size; cnt++, tbl++) {
		mmio_write_32(tbl->addr, tbl->val);
	}
}

static void cpg_div_sel_static_setup(void)
{
	cpg_div_sel_setup(cpg_static_select_tbl, ARRAY_SIZE(cpg_static_select_tbl));
}

static void cpg_div_sel_dynamic_setup(void)
{
	cpg_div_sel_setup(cpg_dynamic_select_tbl, ARRAY_SIZE(cpg_dynamic_select_tbl));
}

static void cpg_mstop_setup(void)
{
	/* Remove all MSTOPS apart from reserved and those already removed at TF-A entry */
	REMOVE_MSTOPS_W(CPG_BUS_1_MSTOP,      CPG_BUS_1_MSTOP_WDT1
										| CPG_BUS_1_MSTOP_RIIC0
										| CPG_BUS_1_MSTOP_RIIC1
										| CPG_BUS_1_MSTOP_RIIC2
										| CPG_BUS_1_MSTOP_RIIC3
										| CPG_BUS_1_MSTOP_RIIC4
										| CPG_BUS_1_MSTOP_RIIC5
										| CPG_BUS_1_MSTOP_RIIC6
										| CPG_BUS_1_MSTOP_RIIC7
										| CPG_BUS_1_MSTOP_SPDIF0
										| CPG_BUS_1_MSTOP_SPDIF1
										| CPG_BUS_1_MSTOP_SPDIF2
										| CPG_BUS_1_MSTOP_TZC400_PCIE0
										| CPG_BUS_1_MSTOP_TZC400_PCIE1);

	REMOVE_MSTOPS_W(CPG_BUS_2_MSTOP,      CPG_BUS_2_MSTOP_SCU
										| CPG_BUS_2_MSTOP_SCU_DMAC
										| CPG_BUS_2_MSTOP_ADG
										| CPG_BUS_2_MSTOP_SSIU
										| CPG_BUS_2_MSTOP_SSIU_DMAC
										| CPG_BUS_2_MSTOP_ADMAC
										| CPG_BUS_2_MSTOP_GTM2
										| CPG_BUS_2_MSTOP_GTM3
										| CPG_BUS_2_MSTOP_TSU1);

	REMOVE_MSTOPS_W(CPG_BUS_3_MSTOP,      CPG_BUS_3_MSTOP_DMAC1
										| CPG_BUS_3_MSTOP_DMAC2
										| CPG_BUS_3_MSTOP_GE3D
										| CPG_BUS_3_MSTOP_ADC
										| CPG_BUS_3_MSTOP_WDT0
										| CPG_BUS_3_MSTOP_RTC_P0
										| CPG_BUS_3_MSTOP_RTC_P1
										| CPG_BUS_3_MSTOP_RIIC8
										| CPG_BUS_3_MSTOP_SCIF
										| CPG_BUS_3_MSTOP_CMTW0);

	REMOVE_MSTOPS_W(CPG_BUS_4_MSTOP,      CPG_BUS_4_MSTOP_CMTW1
										| CPG_BUS_4_MSTOP_CMTW2
										| CPG_BUS_4_MSTOP_CMTW3
										| CPG_BUS_4_MSTOP_XSPI
										| CPG_BUS_4_MSTOP_SECURE_IP_P0
										| CPG_BUS_4_MSTOP_SECURE_IP_P1
										| CPG_BUS_4_MSTOP_MHU);

	REMOVE_MSTOPS_W(CPG_BUS_5_MSTOP,      CPG_BUS_5_MSTOP_TSU0
										| CPG_BUS_5_MSTOP_XSPI_REG
										| CPG_BUS_5_MSTOP_PDM0
										| CPG_BUS_5_MSTOP_PDM1
										| CPG_BUS_5_MSTOP_DMAC0
										| CPG_BUS_5_MSTOP_GTM0
										| CPG_BUS_5_MSTOP_GTM1
										| CPG_BUS_5_MSTOP_WDT2
										| CPG_BUS_5_MSTOP_WDT3
										| CPG_BUS_5_MSTOP_CRC
										| CPG_BUS_5_MSTOP_CMTW4);

	REMOVE_MSTOPS_W(CPG_BUS_6_MSTOP,      CPG_BUS_6_MSTOP_CMTW5
										| CPG_BUS_6_MSTOP_CMTW6
										| CPG_BUS_6_MSTOP_CMTW7
										| CPG_BUS_6_MSTOP_POEG0A
										| CPG_BUS_6_MSTOP_POEG0B
										| CPG_BUS_6_MSTOP_POEG0C
										| CPG_BUS_6_MSTOP_POEG0D
										| CPG_BUS_6_MSTOP_POEG1A
										| CPG_BUS_6_MSTOP_POEG1B
										| CPG_BUS_6_MSTOP_POEG1C
										| CPG_BUS_6_MSTOP_POEG1D
										| CPG_BUS_6_MSTOP_GPT0
										| CPG_BUS_6_MSTOP_GPT1
										| CPG_BUS_6_MSTOP_DDR0_P0
										| CPG_BUS_6_MSTOP_DDR0_P1
										| CPG_BUS_6_MSTOP_DDR0_P2);

	REMOVE_MSTOPS_W(CPG_BUS_7_MSTOP,      CPG_BUS_7_MSTOP_DDR_0_P3
										| CPG_BUS_7_MSTOP_DDR_0_P4
										| CPG_BUS_7_MSTOP_DDR_1_P0
										| CPG_BUS_7_MSTOP_DDR_1_P1
										| CPG_BUS_7_MSTOP_DDR_1_P2
										| CPG_BUS_7_MSTOP_DDR_1_P3
										| CPG_BUS_7_MSTOP_DDR_1_P4
										| CPG_BUS_7_MSTOP_USB20_HOST
										| CPG_BUS_7_MSTOP_USB21_HOST
										| CPG_BUS_7_MSTOP_USB2_FUNC
										| CPG_BUS_7_MSTOP_USB20_PHY
										| CPG_BUS_7_MSTOP_USB21_PHY
										| CPG_BUS_7_MSTOP_USB30_HOST
										| CPG_BUS_7_MSTOP_USB31_HOST
										| CPG_BUS_7_MSTOP_USB30_PHY
										| CPG_BUS_7_MSTOP_USB31_PHY);

	REMOVE_MSTOPS_W(CPG_BUS_8_MSTOP,      CPG_BUS_8_MSTOP_PCIE_PHY
										| CPG_BUS_8_MSTOP_SD0
										| CPG_BUS_8_MSTOP_SD1
										| CPG_BUS_8_MSTOP_SD2
										| CPG_BUS_8_MSTOP_GBETH0
										| CPG_BUS_8_MSTOP_GBETH1
										| CPG_BUS_8_MSTOP_DRP_AI_MAC
										| CPG_BUS_8_MSTOP_DRP_AP_DRP0
										| CPG_BUS_8_MSTOP_DRP1);

	REMOVE_MSTOPS_W(CPG_BUS_9_MSTOP,      CPG_BUS_9_MSTOP_CRU0
										| CPG_BUS_9_MSTOP_CRU1
										| CPG_BUS_9_MSTOP_CRU2
										| CPG_BUS_9_MSTOP_CRU3
										| CPG_BUS_9_MSTOP_ISP_APB
										| CPG_BUS_9_MSTOP_ISP_AXI
										| CPG_BUS_9_MSTOP_VCD_P0
										| CPG_BUS_9_MSTOP_VCD_P1
										| CPG_BUS_9_MSTOP_VCD_P2
										| CPG_BUS_9_MSTOP_DSI_LINK
										| CPG_BUS_9_MSTOP_DSI_PHY);

	REMOVE_MSTOPS_W(CPG_BUS_10_MSTOP,     CPG_BUS_10_MSTOP_ISU
										| CPG_BUS_10_MSTOP_LCDC_DU
										| CPG_BUS_10_MSTOP_LCDC_FCPVD
										| CPG_BUS_10_MSTOP_LCDC_VSPD
										| CPG_BUS_10_MSTOP_DDR0_PHY
										| CPG_BUS_10_MSTOP_DDR1_PHY
										| CPG_BUS_10_MSTOP_DDR0_CTRL
										| CPG_BUS_10_MSTOP_DDR1_CTRL
										| CPG_BUS_10_MSTOP_CR8_TCM
										| CPG_BUS_10_MSTOP_DMAC3
										| CPG_BUS_10_MSTOP_DMAC4
										| CPG_BUS_10_MSTOP_CANFD
										| CPG_BUS_10_MSTOP_I3C0);

	REMOVE_MSTOPS_W(CPG_BUS_11_MSTOP,     CPG_BUS_11_MSTOP_RSPI0
										| CPG_BUS_11_MSTOP_RSPI1
										| CPG_BUS_11_MSTOP_RSPI2
										| CPG_BUS_11_MSTOP_RSCI0
										| CPG_BUS_11_MSTOP_RSCI1
										| CPG_BUS_11_MSTOP_RSCI2
										| CPG_BUS_11_MSTOP_RSCI3
										| CPG_BUS_11_MSTOP_RSCI4
										| CPG_BUS_11_MSTOP_RSCI5
										| CPG_BUS_11_MSTOP_RSCI6
										| CPG_BUS_11_MSTOP_RSCI7
										| CPG_BUS_11_MSTOP_RSCI8
										| CPG_BUS_11_MSTOP_RSCI9
										| CPG_BUS_11_MSTOP_GTM4
										| CPG_BUS_11_MSTOP_GTM5
										| CPG_BUS_11_MSTOP_GTM6);

	REMOVE_MSTOPS_W(CPG_BUS_12_MSTOP,     CPG_BUS_12_MSTOP_GTM7
										| CPG_BUS_12_MSTOP_MCPU_TO_ACPU);
}

static CPG_SETUP_DATA cpg_clk_sr2_tbl[] = {
	{	/* I2C8 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_9,
				.val  = 0x00000008,
			},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_4,
				.val  = 0x00080000,
			},

			.type = CPG_T_CLK
	},

	{	/* I2C8 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_10,
				.val  = 0x00000001,
			},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_4,
				.val  = 0x00020000,
			},

		.type = CPG_T_RST
	}
};

void cpg_prepare_suspend(void)
{
	cpg_ctrl_clkrst(&cpg_clk_sr2_tbl[0], ARRAY_SIZE(cpg_clk_sr2_tbl));
}

static void cpg_clk_on_setup(void)
{
	cpg_ctrl_clkrst(&cpg_clk_on_tbl[0], ARRAY_SIZE(cpg_clk_on_tbl));
}

static void cpg_reset_setup(void)
{
	cpg_ctrl_clkrst(&cpg_reset_tbl[0], ARRAY_SIZE(cpg_reset_tbl));
}

static void cpg_wdtrst_sel_setup(void)
{
	uint32_t val	= CPG_ERRORRST_SELx_ERRRSTSEL0
					| CPG_ERRORRST_SELx_ERRRSTSEL1
					| CPG_ERRORRST_SELx_ERRRSTSEL2
					| CPG_ERRORRST_SELx_ERRRSTSEL3;
	uint32_t ca33_w01, ca33_w23, ca55_w01, ca55_w23;

	ca33_w01 = mmio_read_32(RZV2H_ELC_ERINTM33CTL(0));
	ca33_w23 = mmio_read_32(RZV2H_ELC_ERINTM33CTL(1));
	ca55_w01 = mmio_read_32(RZV2H_ELC_ERINTA55CTL(0));
	ca55_w23 = mmio_read_32(RZV2H_ELC_ERINTA55CTL(1));

	/* Checking ICU interrupt WDT CM33 */
	if ((ca33_w01 == 0x40000000) || (ca33_w01 == 0x80000000) ||
			(ca33_w23 == 0x00000001) || (ca33_w23 == 0x00000002)) {
		/* ERINTM33CLR0 bit for clear 28-31 */
		mmio_write_32(RZV2H_ELC_ERINTM33CLR(0), 0xF0000000);
	}

	/* Checking ICU interrupt WDT CA55 */
	if ((ca55_w01 == 0x40000000) || (ca55_w01 == 0x80000000) ||
			(ca55_w23 == 0x00000001) || (ca55_w23 == 0x00000002)) {
		/* ERINTA55CLR0 bit for clear 28-31 */
		mmio_write_32(RZV2H_ELC_ERINTA55CLR(0), 0xF0000000);
	}

	/* Add in the WEN bits for the selected bits */
	val = (val & 0xFFFF) | ((val & 0xFFFF) << 16);

	mmio_write_32(CPG_ERRORRST_SEL2, val);
}


void cpg_ddr0_part1(void)
{
	/* 2. */
	mmio_write_32(CPG_RST_11, 0x0FF80000);

	mmio_write_32(CPG_LP_DDR_CTL1, mmio_read_32(CPG_LP_DDR_CTL1) & ~0x00000001);

	/* 3. */
	mmio_write_32(CPG_PLLDDR0_STBY, 0x00010001);	/* PLLDDR0 clock start */
	while ((mmio_read_32(CPG_PLLDDR0_MON) & 0x00000011) != 0x00000011)
		;

	mmio_write_32(CPG_CLKON_12, 0x0FC00FC0);

	/* 4. */
	udelay(1);

	/* 5. */
	mmio_write_32(CPG_RST_11, 0x00080008);
	mmio_write_32(CPG_LP_DDR_CTL1, mmio_read_32(CPG_LP_DDR_CTL1) | 0x00000001);

	/* 6. */
	udelay(1);

	/* 7. */
	mmio_write_32(CPG_RST_11, 0x03F003F0);

	/* 8. */
	udelay(1);
}

void cpg_ddr0_part2(void)
{
	mmio_write_32(CPG_RST_11, 0x08000800);

	udelay(10);

	mmio_write_32(CPG_RST_11, 0x04000400);

	udelay(10);
}

void cpg_ddr1_part1(void)
{
	mmio_write_32(CPG_RST_11, 0xF0000000);
	mmio_write_32(CPG_RST_12, 0x001F0000);

	mmio_write_32(CPG_LP_DDR_CTL1, mmio_read_32(CPG_LP_DDR_CTL1) & ~0x00000002);

	mmio_write_32(CPG_PLLDDR1_STBY, 0x00010001);
	while ((mmio_read_32(CPG_PLLDDR1_MON) & 0x00000011) != 0x00000011)
		;

	mmio_write_32(CPG_CLKON_12, 0xF000F000);
	mmio_write_32(CPG_CLKON_13, 0x00030003);

	udelay(1);

	mmio_write_32(CPG_RST_11, 0x10001000);
	mmio_write_32(CPG_LP_DDR_CTL1, mmio_read_32(CPG_LP_DDR_CTL1) | 0x00000002);

	udelay(1);

	mmio_write_32(CPG_RST_11, 0xE000E000);
	mmio_write_32(CPG_RST_12, 0x00070007);

	udelay(1);
}

void cpg_ddr1_part2(void)
{
	mmio_write_32(CPG_RST_12, 0x00100010);

	udelay(10);

	mmio_write_32(CPG_RST_12, 0x00080008);

	udelay(10);
}

void cpg_ddr_pwrokin_off(uint8_t base)
{
	if (!base)
		mmio_write_32(CPG_LP_DDR_CTL1, mmio_read_32(CPG_LP_DDR_CTL1) & ~0x00000001);	/* DDR0 */
	else
		mmio_write_32(CPG_LP_DDR_CTL1, mmio_read_32(CPG_LP_DDR_CTL1) & ~0x00000002);	/* DDR1 */
}

void cpg_early_setup(void)
{
	/* CGC_SYC_0_CNT_CLK and SYC_0_RESETN are 'forcibly modified by hardware' so do not need setting here */
}

void cpg_setup(void)
{
	cpg_div_sel_static_setup();
	cpg_pll_setup();
	cpg_mstop_setup();
	cpg_clk_on_setup();
	cpg_reset_setup();
	cpg_div_sel_dynamic_setup();
	cpg_wdtrst_sel_setup();
}
