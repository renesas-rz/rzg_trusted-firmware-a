#include <stdint.h>

#include <lib/mmio.h>

#include "cpg_settings.h"
#include "cpg_regs.h"


static const cpg_clk_data_t cpg_awo_clk_on_tbl[] = {

	{	/* MHU */
	.reg =  {
			.addr = (uintptr_t)CPG_CLKON_3,
			.val  = 0x00000001,
			},

	.mon =  {
			.addr = (uintptr_t)CPG_CLKMON_1,
			.val  = 0x00010000,
			},
	},

	{	/* DMAC0 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_0,
				.val  = 0x00000001,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_0,
				.val  = 0x00000001,
				},
	},

	{	/* GTM 0 and GTM1 (OSTM) */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_4,
				.val  = 0x00000018,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_2,
				.val  = 0x00000018,
				},
	},

		{	/* xSPI Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_9,
				.val  = 0x00008000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_4,
				.val  = 0x80000000,
				},
	},

	{	/* xSPI Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_10,
				.val  = 0x00000003,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_5,
				.val  = 0x00000007,
				},
	},

	{	/* I2C8 (RIIC) */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_9,
				.val  = 0x00000008,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_4,
				.val  = 0x00080000,
				},
	},

	{	/* ADC */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_16,
				.val  = 0x00000180,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_8,
				.val  = 0x00000180,
				},
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
	},

	{	/* CMTW 1 to 3 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_3,
				.val  = 0x00007800,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_1,
				.val  = 0x78000000,
				},
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
	},

	{	/* TSU0 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_16,
				.val  = 0x00000200,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_8,
				.val  = 0x00000200,
				},
	},
};

static const cpg_clk_data_t cpg_iso_clk_on_tbl[] = {
	{	/* DMAC 1 to 4 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_0,
				.val  = 0x0000001E,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_0,
				.val  = 0x0000001E,
				},
	},

	{	/* GTM 2 to GTM7 (OSTM) */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_4,
				.val  = 0x000007E0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_2,
				.val  = 0x000007E0,
				},
	},

	{	/* RIIC 0 to 7 (I2C) */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_9,
				.val  = 0x00000FF0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_4,
				.val  = 0x0FF00000,
				},
	},

	{	/* CMTW 4 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_3,
				.val  = 0x00008000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_1,
				.val  = 0x80000000,
				},
	},

	{	/* CMTW 5 to 7 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_4,
				.val  = 0x00000007,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_2,
				.val  = 0x00000007,
				},
	},

	{	/* ADMAC */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_15,
				.val  = 0x00000100,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_7,
				.val  = 0x01000000,
				},
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
	},

	{	/* WDT1, WDT2 Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_4,
				.val  = 0x0000E00,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_2,
				.val  = 0x0000E00,
				},
	},

	{	/* WDT2 Part 2, WDT3 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_5,
				.val  = 0x00000007,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_2,
				.val  = 0x00070000,
				},
	},

	{	/* RSPI */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_5,
				.val  = 0x00001FF0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_2,
				.val  = 0x1FF00000,
				},
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
	},

	{	/* ISU (Image Scaling Unit) */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_14,
				.val  = 0x000000C0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_7,
				.val  = 0x000000C0,
				},
	},

	{	/* CRU (Camera Data Receive Unit) */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_13,
				.val  = 0x000000FC,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_6,
				.val  = 0x00FC0000,
				},
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
	},

	{	/* SSIF (Serial Sound Interface) Part 1*/
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_15,
				.val  = 0x00000020,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_7,
				.val  = 0x00200000,
				},
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
	},

	{	/* USB2.0 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_11,
				.val  = 0x00000068,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_5,
				.val  = 0x00680000,
				},
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
	},

	{	/* USB3 Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_11,
				.val  = 0x00000001,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_5,
				.val  = 0x00010000,
				},
	},

	{	/* RSCI Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_5,
				.val  = 0x0000E000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_2,
				.val  = 0xE0000000,
				},
	},

	{	/* RSCI Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_6,
				.val  = 0x0000FFFF,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_3,
				.val  = 0x0000FFFF,
				},
	},

	{	/* RSCI Part 3 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_7,
				.val  = 0x0000FFFF,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_3,
				.val  = 0xFFFF0000,
				},
	},

	{	/* RSCI Part 4 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_8,
				.val  = 0x00007FFF,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_4,
				.val  = 0x00007FFF,
				},
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
	},

	{	/* CRC */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_0,
				.val  = 0x00000040,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_0,
				.val  = 0x00000040,
				},
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
	},

	{	/* GBETH Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_11,
				.val  = 0x0000FF00,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_5,
				/* Check status of active GBETH clocks */
				.val  = 0x75000000,
				},
	},

	{	/* GBETH Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_12,
				.val  = 0x0000000F,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_6,
				/* Check status of active GBETH clocks */
				.val  = 0x0000000D,
				},
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
	},

	{	/* ADG */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_15,
				.val  = 0x00003E00,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_7,
				/* Don't check for clock status of external clocks AUDIO_CLK A, B and C */
				.val  = 0x06000000
				},
	},

	{	/* SPDIF 0 and 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_15,
				.val  = 0x0000C000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_7,
				.val  = 0xC0000000,
				},
	},

	{	/* SPDIF 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_16,
				.val  = 0x00000001,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_8,
				.val  = 0x00000001,
				},
	},

	{	/* TSU 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_16,
				.val  = 0x00000400,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_8,
				.val  = 0x00000400,
				},
	},

	{	/* DRP */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_17,
				.val  = 0x00000078,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_8,
				.val  = 0x00780000,
				},
	}
};

static const cpg_clk_data_t cpg_wdt1_clk_tbl[] = {
	{
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_4,
				.val  = 0x0006000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_2,
				.val  = 0x0006000,
				},
	}
};

#if PLAT_M33_BOOT_SUPPORT
static const cpg_clk_data_t cpg_m33_clk_tbl[] = {
	{	/* CM33 */
		.reg =  {
				.addr = (uintptr_t)CPG_CLKON_1,
				.val  = 0x00000004,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_CLKMON_0,
				.val  = 0x00040000,
				},
	}
};
#endif /* PLAT_M33_BOOT_SUPPORT */


static void cpg_ctrl_clk(cpg_clk_data_t const *array, uint32_t num, cpg_clk_state_t clk_state)
{
	int i;
	uint32_t mask;
	uint32_t cmp;

	for (i = 0; i < num; i++, array++) {
		/*
		 * Upper 16bits are enables for lower 16bits so write the upper 16bits with same value as lower value
		 */
		uint32_t write_enable = (array->reg.val & 0xFFFF) << 16;
		uint32_t value_to_write = (array->reg.val & 0xFFFF) & ((uint32_t) clk_state);

		mmio_write_32(array->reg.addr, write_enable | value_to_write);

		/*
		 * This generic function needs to handle case where Montitor for clock
		 * is looking for a HIGH as clock active whereas Montitoring a reset
		 * it is looking for a LOW to indicate reset release.
		 */
		mask = array->mon.val;

		if (CPG_CLK_ENABLE == clk_state)
			cmp = mask;
		else
			cmp = 0;

		while ((mmio_read_32(array->mon.addr) & mask) != cmp)
			;
	}
}

void cpg_awo_set_clk(cpg_clk_state_t clk_state)
{
	cpg_ctrl_clk(cpg_awo_clk_on_tbl, ARRAY_SIZE(cpg_awo_clk_on_tbl), clk_state);
}

void cpg_iso_set_clk(cpg_clk_state_t clk_state)
{
	cpg_ctrl_clk(cpg_iso_clk_on_tbl, ARRAY_SIZE(cpg_iso_clk_on_tbl), clk_state);
}

void cpg_wdt1_set_clk(cpg_clk_state_t clk_state)
{
	cpg_ctrl_clk(cpg_wdt1_clk_tbl, ARRAY_SIZE(cpg_wdt1_clk_tbl), clk_state);
}

#if PLAT_M33_BOOT_SUPPORT
void cpg_cm33_set_clk(cpg_clk_state_t clk_state)
{
	cpg_ctrl_clk(cpg_m33_clk_tbl, ARRAY_SIZE(cpg_m33_clk_tbl), clk_state);
}
#endif /* PLAT_M33_BOOT_SUPPORT */
