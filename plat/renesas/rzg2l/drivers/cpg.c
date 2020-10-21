/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include "cpg_regs.h"
#include "cpg.h"
#include <lib/mmio.h>

static CPG_REG_SETTING cpg_clk_on_tbl[CPG_CLK_ON_TBL_NUM] = {
	{ (uintptr_t)CPG_CLKON_ROM,             0x00010001 },		/* ROM */
	{ (uintptr_t)CPG_CLKON_GIC600,          0x00010001 },		/* GIC600 */
	{ (uintptr_t)CPG_CLKON_IA55,            0x00030003 },		/* IA55 */
	{ (uintptr_t)CPG_CLKON_IM33,            0x00030003 },		/* IM33 */
	{ (uintptr_t)CPG_CLKON_CST,             0x07ff07ff },		/* CST */
	{ (uintptr_t)CPG_CLKON_SYC,             0x00010001 },		/* SYC */
	{ (uintptr_t)CPG_CLKON_SYSC,            0x00030003 },		/* SYSC */
	{ (uintptr_t)CPG_CLKON_OSTM,            0x00010001 },		/* OSTM */
	{ (uintptr_t)CPG_CLKON_WDT,             0x000f000f },		/* WDT */
	{ (uintptr_t)CPG_CLKON_SPI_MULTI,       0x00030003 },		/* SPI_MULTI */
	{ (uintptr_t)CPG_CLKON_SDHI,            0x00ff00ff },		/* SDHI */
	{ (uintptr_t)CPG_CLKON_SCIF,            0x00010001 },		/* SCIF */
	{ (uintptr_t)CPG_CLKON_GPIO,            0x00010001 },		/* GPIO */
	{ (uintptr_t)CPG_CLKON_JAUTH,           0x00010001 },		/* JAUTH */
	{ (uintptr_t)CPG_CLKON_OTP,             0x00030003 },		/* OTP */
	{ (uintptr_t)CPG_CLKON_AXI_ACPU_BUS,    0x000f000f },		/* AXI_ACPU_BUS */
	{ (uintptr_t)CPG_CLKON_AXI_MCPU_BUS,    0x07ff07ff },		/* AXI_MCPU_BUS */
	{ (uintptr_t)CPG_CLKON_AXI_COM_BUS,     0x00030003 },		/* AXI_COM_BUS */
	{ (uintptr_t)CPG_CLKON_AXI_VIDEO_BUS,   0x00030003 },		/* AXI_VIDEO_BUS */
	{ (uintptr_t)CPG_CLKON_PERI_COM,        0x00030003 },		/* PERI_COM */
	{ (uintptr_t)CPG_CLKON_REG1_BUS,        0x00030003 },		/* REG1_BUS */
	{ (uintptr_t)CPG_CLKON_REG0_BUS,        0x000f000f },		/* REG0_BUS */
	{ (uintptr_t)CPG_CLKON_PERI_CPU,        0x000f000f },		/* PERI_CPU */
	{ (uintptr_t)CPG_CLKON_PERI_VIDEO,      0x00070007 },		/* PERI_VIDEO */
	{ (uintptr_t)CPG_CLKON_PERI_DDR,        0x00010001 },		/* PERI_DDR */
	{ (uintptr_t)CPG_CLKON_AXI_TZCDDR,      0x001f001f },		/* AXI_TZCDDR */
	{ (uintptr_t)CPG_CLKON_MTGPGS,          0x00030003 },		/* MTGPGS */
	{ (uintptr_t)CPG_CLKON_AXI_DEFAULT_SLV, 0x00010001 },		/* AXI_DEFAULT_SLV */
};

static CPG_REG_SETTING cpg_reset_tbl[CPG_RESET_TBL_NUM] = {
	{ (uintptr_t)CPG_RST_ROM,               0x00010001 },		/* ROM */
	{ (uintptr_t)CPG_RST_GIC600,            0x00030003 },		/* GIC600 */
	{ (uintptr_t)CPG_RST_IA55,              0x00010001 },		/* IA55 */
	{ (uintptr_t)CPG_RST_IM33,              0x00010001 },		/* IM33 */
	{ (uintptr_t)CPG_RST_SYC,               0x00010001 },		/* SYC */
	{ (uintptr_t)CPG_RST_SYSC,              0x00070007 },		/* SYSC */
	{ (uintptr_t)CPG_RST_OSTM,              0x00010001 },		/* OSTM */
	{ (uintptr_t)CPG_RST_WDT,               0x00050005 },		/* WDT */
	{ (uintptr_t)CPG_RST_SPI,               0x00010001 },		/* SPI_MULTI */
	{ (uintptr_t)CPG_RST_SDHI,              0x00030003 },		/* SDHI */
	{ (uintptr_t)CPG_RST_SCIF,              0x00010001 },		/* SCIF */
	{ (uintptr_t)CPG_RST_GPIO,              0x00070007 },		/* GPIO */
	{ (uintptr_t)CPG_RST_JAUTH,             0x00010001 },		/* JAUTH */
	{ (uintptr_t)CPG_RST_OTP,               0x00010001 },		/* OTP */
	{ (uintptr_t)CPG_RST_AXI_ACPU_BUS,      0x00010001 },		/* AXI_ACPU_BUS */
	{ (uintptr_t)CPG_RST_AXI_MCPU_BUS,      0x00010001 },		/* AXI_MCPU_BUS */
	{ (uintptr_t)CPG_RST_AXI_COM_BUS,       0x00010001 },		/* AXI_COM_BUS */
	{ (uintptr_t)CPG_RST_AXI_VIDEO_BUS,     0x00010001 },		/* AXI_VIDEO_BUS */
	{ (uintptr_t)CPG_RST_PERI_COM,          0x00010001 },		/* PERI_COM */
	{ (uintptr_t)CPG_RST_REG1_BUS,          0x00010001 },		/* REG1_BUS */
	{ (uintptr_t)CPG_RST_REG0_BUS,          0x00010001 },		/* REG0_BUS */
	{ (uintptr_t)CPG_RST_PERI_CPU,          0x00010001 },		/* PERI_CPU */
	{ (uintptr_t)CPG_RST_PERI_VIDEO,        0x00010001 },		/* PERI_VIDEO */
	{ (uintptr_t)CPG_RST_PERI_DDR,          0x00010001 },		/* PERI_DDR */
	{ (uintptr_t)CPG_RST_AXI_TZCDDR,        0x001f001f },		/* AXI_TZCDDR */
	{ (uintptr_t)CPG_RST_MTGPGS,            0x00030003 },		/* MTGPGS */
	{ (uintptr_t)CPG_RST_AXI_DEFAULT_SLV,   0x00010001 },		/* AXI_DEFAULT_SLV */
};

static CPG_REG_SETTING cpg_select_tbl[CPG_CELECT_TBL_NUM] = {
	{ (uintptr_t)CPG_PLL4_CLK2,             0x00000004 },
	{ (uintptr_t)CPG_PL4_DSEL,              0x00010001 }
};

/* It is assumed that the PLL has stopped by the time this function is executed. */
static void cpg_pll_setup(void)
{
	uint32_t val = 0;
	
	/* PLL4 startup */
	/* PLL4 standby mode transition confirmation */
	do {
		val = mmio_read_32(CPG_PLL4_MON);
	} while((val & (PLL4_MON_PLL4_RESETB | PLL4_MON_PLL4_LOCK)) != 0);
	
	/* Set PLL4 to normal mode */
	mmio_write_32(CPG_PLL4_STBY, (PLL4_STBY_RESETB_WEN | PLL4_STBY_RESETB));
	
	/* PLL4 normal mode transition confirmation */
	do {
		val = mmio_read_32(CPG_PLL4_MON);
	} while((val & (PLL4_MON_PLL4_RESETB | PLL4_MON_PLL4_LOCK)) == 0);
	
	/* PLL5 startup */
	/* PLL5 standby mode transition confirmation */
	do {
		val = mmio_read_32(CPG_PLL5_MON);
	} while((val & (PLL5_MON_PLL5_RESETB | PLL5_MON_PLL5_LOCK)) != 0);
	
#if 0
	/* Divided ratio setting of PLL5 Divide 3000MHz by 32 and set it within 5.08-148.5MHz. */
	/* val = PL5_SDIV_DIVSDIB_WEN | PL5_SDIV_DIVDSA_WEN | PL5_SDIV_DIVDSIA_SET_1_4 | PL5_SDIV_DIVDSIB_SET_1_8; */
	val = PL5_SDIV_DIVSDIB_WEN | PL5_SDIV_DIVDSA_WEN | PL5_SDIV_DIVDSIA_SET_1_1 | PL5_SDIV_DIVDSIB_SET_1_1;
	mmio_write_32(CPG_PL5_SDIV, val);
#endif
	
	/* PLL5 set to normal mode */
	val = mmio_read_32(CPG_PLL5_STBY) | PLL5_STBY_RESETB_WEN | PLL5_STBY_RESETB;
	mmio_write_32(CPG_PLL5_STBY, val);
	
	/* PLL5 Normal mode transition confirmation */
	do {
		val = mmio_read_32(CPG_PLL5_MON);
	} while((val & (PLL5_MON_PLL5_RESETB | PLL5_MON_PLL5_LOCK)) == 0);
	
	/* PLL6 startup */
	/* PLL6 standby mode transition confirmation */
	do {
		val = mmio_read_32(CPG_PLL6_MON);
	} while((val & (PLL6_MON_PLL6_RESETB | PLL6_MON_PLL6_LOCK)) != 0);
	
	/* Set PLL6 to normal mode */
	val = mmio_read_32(CPG_PLL6_STBY) | PLL6_STBY_RESETB_WEN | PLL6_STBY_RESETB;
	mmio_write_32(CPG_PLL6_STBY, val);
	
	/* PLL6 Normal mode transition confirmation */
	do {
		val = mmio_read_32(CPG_PLL6_MON);
	} while((val & (PLL6_MON_PLL6_RESETB | PLL6_MON_PLL6_LOCK)) == 0);
	
}

static void cpg_div_sel_setup(void)
{
	int cnt;
	
	for(cnt = 0; cnt < CPG_CELECT_TBL_NUM; cnt++) {
		mmio_write_32(cpg_select_tbl[cnt].reg, cpg_select_tbl[cnt].val);
	}
}

static void cpg_clk_on_setup(void)
{
	int cnt;
	
	for(cnt = 0; cnt < CPG_CLK_ON_TBL_NUM; cnt++) {
		mmio_write_32(cpg_clk_on_tbl[cnt].reg, cpg_clk_on_tbl[cnt].val);
	}
		
	while((mmio_read_32(CPG_CLKMON_AXI_DEFAULT_SLV) & BIT0_ON) == 0 );
}

static void cpg_reset_setup(void)
{
	int cnt;
	
	for(cnt = 0; cnt < CPG_RESET_TBL_NUM; cnt++) {
		mmio_write_32(cpg_reset_tbl[cnt].reg, cpg_reset_tbl[cnt].val);
	}
		
	while((mmio_read_32(CPG_RSTMON_AXI_DEFAULT_SLV) & BIT0_ON) == 0 );
}

void cpg_setup(void)
{
	cpg_pll_setup();
	cpg_div_sel_setup();
	cpg_clk_on_setup();
	cpg_reset_setup();
}
