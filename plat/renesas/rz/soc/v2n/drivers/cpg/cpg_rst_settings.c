#include <stdint.h>
#include <lib/mmio.h>
#include "cpg_settings.h"
#include "cpg_regs.h"

/* reset control for peripherals in the AWO power domain */
static const cpg_rst_data_t cpg_awo_pd_reset_tbl[] = {
	{	/* MHU */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_5,
				.val  = 0x00000100,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_2,
				.val  = 0x00000200,
				},
	},

	{	/* DMAC 0 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_3,
				.val  = 0x00000008,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_1,
				.val  = 0x00000010,
				},
	},

	{	/* GTM 0 and 1 (OSTM) */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_6,
				.val  = 0x00006000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_2,
				.val  = 0xC0000000,
				},
	},

	{	/* RIIC 8 (I2C) AWO*/
		.reg =  {
				.addr = (uintptr_t)CPG_RST_10,
				.val  = 0x00000001,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_4,
				.val  = 0x00020000,
				},
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
	},

	{	/* CMTW 1 to 4 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_6,
				.val  = 0x00001FE0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_2,
				.val  = 0x3FC00000,
				},
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
	},

	{	/* TSU0 (AWO) */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_15,
				.val  = 0x00000080,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_7,
				.val  = 0x00000100,
				},
	}
};

/* reset control for peripherals in the OTHERS power domain */
static const cpg_rst_data_t cpg_others_pd_reset_tbl[] = {
	{	/* OSTM ISO Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_6,
				.val  = 0x00008000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_3,
				.val  = 0x00000001,
				},
	},

	{	/* OSTM ISO Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_7,
				.val  = 0x0000001F,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_3,
				.val  = 0x0000003E,
				},
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
	},

	{	/* WDT1, WDT2, WDT3 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_7,
				.val  = 0x000001C0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_3,
				.val  = 0x00000380,
				},
	},

	{	/* RSPI Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_7,
				.val  = 0x0000F800,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_3,
				.val  = 0x0001F000,
				},
	},

	{	/* RSPI Part 2 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_8,
				.val  = 0x00000001,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_3,
				.val  = 0x00020000,
				},
	},

	{	/* xSPI */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_10,
				.val  = 0x00000018,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_4,
				.val  = 0x00300000,
				},
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
	},

	{	/* Camera Data Receive Unit */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_12,
				.val  = 0x000007E0,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_5,
				.val  = 0x0FC00000,
				},
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
	},

	{	/* USB2.0 Part 1 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_10,
				.val  = 0x00005000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_4,
				.val  = 0xA0000000,
				},
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
	},

	{	/* USB3 */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_10,
				.val  = 0x00000400,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_4,
				.val  = 0x08000000,
				},
	},

	{	/* I2C (IIC) ISO */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_9,
				.val  = 0x0000FF00,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_4,
				.val  = 0x0001FE00,
				},
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
	},

	{	/* VCD */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_14,
				.val  = 0x00000001,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_6,
				.val  = 0x00020000,
				},
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
	},

	{	/* ADMAC */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_14,
				.val  = 0x00002000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_6,
				.val  = 0x40000000,
				},
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
	},
	{	/* TSU1 (ISO) */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_15,
				.val  = 0x00000100,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_7,
				.val  = 0x00000200,
				},
	},

	{	/* DRP */
		.reg =  {
				.addr = (uintptr_t)CPG_RST_15,
				.val  = 0x00002000,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_7,
				.val  = 0x00004000,
				},
	}

};

static const cpg_rst_data_t cpg_wdt1_rest_tbl[] = {
	{
		.reg =  {
				.addr = (uintptr_t)CPG_RST_7,
				.val  = 0x00000040,
				},

		.mon =  {
				.addr = (uintptr_t)CPG_RSTMON_3,
				.val  = 0x00000080,
				},
	}
};

static void cpg_ctrl_rst(cpg_rst_data_t const *array, uint32_t num, cpg_rst_state_t rst_state)
{
	int i;
	uint32_t mask;
	uint32_t cmp;

	for (i = 0; i < num; i++, array++) {
		/*
		 * Upper 16bits are enables for lower 16bits so write the upper 16bits with same value as lower value
		 */
		uint32_t write_enable = (array->reg.val & 0xFFFF) << 16;
		uint32_t value_to_write = (array->reg.val & 0xFFFF) & ((uint32_t) rst_state);

		mmio_write_32(array->reg.addr, write_enable | value_to_write);

		/*
		 * This generic function needs to handle case where Montitor for clock
		 * is looking for a HIGH as clock active whereas Montitoring a reset
		 * it is looking for a LOW to indicate reset release.
		 */
		mask = array->mon.val;

		if (CPG_RST_ASSERT == rst_state) {
			cmp = mask;
		} else {
			cmp = 0;
		}

		while ((mmio_read_32(array->mon.addr) & mask) != cmp)
			;
	}
}

void cpg_awo_pd_set_rst(cpg_rst_state_t rst_state)
{
	cpg_ctrl_rst(cpg_awo_pd_reset_tbl, ARRAY_SIZE(cpg_awo_pd_reset_tbl), rst_state);
}

void cpg_others_pd_set_rst(cpg_rst_state_t rst_state)
{
	cpg_ctrl_rst(cpg_others_pd_reset_tbl, ARRAY_SIZE(cpg_others_pd_reset_tbl), rst_state);
}

void cpg_wdt1_set_rst(cpg_rst_state_t rst_state)
{
	cpg_ctrl_rst(cpg_wdt1_rest_tbl, ARRAY_SIZE(cpg_wdt1_rest_tbl), rst_state);
}
