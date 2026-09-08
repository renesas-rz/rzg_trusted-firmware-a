#include <stddef.h>
#include <stdint.h>
#include <lib/mmio.h>
#include "cpg_settings.h"
#include "cpg_regs.h"

static const cpg_pll_settings_t cpg_pll_tbl[] = {
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
				.val  = 0x00010001,
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

static const cpg_reg_setting_t cpg_static_select_tbl[] = {
	{ (uintptr_t)CPG_CSDIV0,				0x00000000 },
	{ (uintptr_t)CPG_CSDIV1,				0x00000000 },
};

static const cpg_reg_setting_t cpg_dynamic_select_tbl[] = {
	{ (uintptr_t)CPG_CDDIV0,				0x00000000 },
	{ (uintptr_t)CPG_CDDIV1,				0x00000000 },
	{ (uintptr_t)CPG_CDDIV2,				0x00000000 },
	{ (uintptr_t)CPG_CDDIV3,				0x10001000 },
	{ (uintptr_t)CPG_CDDIV4,				0x01110111 },
};

/* It is assumed that the PLL has stopped by the time this function is executed. */
void cpg_pll_setup(void)
{
	int i;
	int pll_num = ARRAY_SIZE(cpg_pll_tbl);
	cpg_pll_settings_t const *p_pll = &cpg_pll_tbl[0];
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

static void cpg_div_sel_setup(cpg_reg_setting_t const *tbl, uint32_t size)
{
	int cnt;

	for (cnt = 0; cnt < size; cnt++, tbl++) {
		mmio_write_32(tbl->addr, tbl->val);
	}
}

void cpg_div_sel_static_setup(void)
{
	cpg_div_sel_setup(cpg_static_select_tbl, ARRAY_SIZE(cpg_static_select_tbl));
}

void cpg_div_sel_dynamic_setup(void)
{
	cpg_div_sel_setup(cpg_dynamic_select_tbl, ARRAY_SIZE(cpg_dynamic_select_tbl));
}

