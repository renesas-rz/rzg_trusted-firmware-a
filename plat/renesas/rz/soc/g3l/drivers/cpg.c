/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <cpg_regs.h>
#include <lib/mmio.h>
#include <drivers/delay_timer.h>
#include <ddr_private.h>

#define CPG_T_CLK						(0)
#define CPG_T_RST						(1)

typedef struct {
	uintptr_t reg;
	uintptr_t mon;
	uint32_t  val;
	uint32_t  type;
} cpg_setup_data_t;

typedef struct {
	uintptr_t reg;
	uint32_t  val;
} cpg_reg_setting_t;

static const cpg_reg_setting_t cpg_early_div_tbl[] = {
	{ (uintptr_t)CPG_PL1_DDIV,				0x00010000 },	/* 1200MHz */
};

static const cpg_setup_data_t cpg_early_clkrst_tbl[] = {
	{
		(uintptr_t)CPG_CLKON_SYC,
		(uintptr_t)CPG_CLKMON_SYC,
		0x00010001,
		CPG_T_CLK
	},
	{
		(uintptr_t)CPG_RST_SYC,
		(uintptr_t)CPG_RSTMON_SYC,
		0x00010001,
		CPG_T_RST
	},
	{		/* I2C */
		(uintptr_t)CPG_CLKON_I2C,
		(uintptr_t)CPG_CLKMON_I2C,
		0x000F0003,
		CPG_T_CLK
	},
	{		/* I2C */
		(uintptr_t)CPG_RST_I2C,
		(uintptr_t)CPG_RSTMON_I2C,
		0x000F0003,
		CPG_T_RST
	},
	{		/* VBAT */
		(uintptr_t)CPG_CLKON_VBAT,
		(uintptr_t)CPG_CLKMON_VBAT,
		0x00010001,
		CPG_T_CLK
	},
	{		/* VBAT */
		(uintptr_t)CPG_RST_VBAT,
		(uintptr_t)CPG_RSTMON_VBAT,
		0x00010001,
		CPG_T_RST
	},
};

static const cpg_reg_setting_t cpg_pll4_tbl[] = {
#if (DDR_PLL4 == 2133)
	{ CPG_PLL4_CLK1, 0x00908000 },
#elif (DDR_PLL4 == 1600)
	{ CPG_PLL4_CLK1, 0x0498E000 },
#else
#error "Unknown DDR Type."
#endif
	{ CPG_PLL4_CLK2, 0x00000002 },
	{ CPG_PLL4_STBY, 0x00010001 }
};

static const cpg_setup_data_t cpg_static_clock_tbl[] = {
	{		/* XSPI */
		(uintptr_t)CPG_CLKON_XSPI,
		(uintptr_t)CPG_CLKMON_XSPI,
		0x000F0000,
		CPG_T_CLK
	},
};

static const cpg_reg_setting_t cpg_static_select_tbl[] = {
	{ (uintptr_t)CPG_XSPI_SSEL,				0x00010002 },	/* 2'b10:CLK522 */
};

static const cpg_reg_setting_t cpg_dynamic_select_tbl[] = {
	{ (uintptr_t)CPG_PLL_DSEL,				0x00400040 },	/* 1'b1:PLL4 */
	{ (uintptr_t)CPG_SDHI_DSEL,				0x01110333 },	/* 2'b11:CLK266FIX_C, 2'b11:CLK266FIX_C, 2'b11:CLK266FIX_C */
};

static const cpg_reg_setting_t cpg_dynamic_division_tbl[] = {
	{ (uintptr_t)CPG_PL2_DDIV,				0x01110000 },	/* 3'b000:1/4(200MHz), 3'b000:1/8(100MHz), 3'b000:1/4(200MHz) */
	{ (uintptr_t)CPG_PL3_DDIV,				0x00110000 },	/* 3'b000:1/4(200MHz), 3'b000:1/8(100MHz) */
	{ (uintptr_t)CPG_SDHI_DDIV,				0x01110111 },	/* 1'b1:1/2, 1'b1:1/2, 1'b1:1/2 */
	{ (uintptr_t)CPG_XSPI_DDIV,				0x00010007 }	/* 3'b111:1/16 */
};

/* Power down everything except for SRAM_ACPU in CPG_PWRDN_IP1 */
static const cpg_reg_setting_t cpg_pwrdown_ip_tbl[] = {
	{ (uintptr_t)CPG_PWRDN_IP1,				0xFFEFFFEE },
	{ (uintptr_t)CPG_PWRDN_IP2,				0x3FF33FF3 },
	{ (uintptr_t)CPG_PWRDN_IP3,				0x3FFF3FFF },
	{ (uintptr_t)CPG_PWRDN_IP4,				0x03FF03FF }
};

#if defined(PLAT_SYSTEM_SUSPEND_awo)
static const cpg_reg_setting_t cpg_iso_mstop_tbl[] = {
	{ (uintptr_t)CPG_BUS_ACPU_MSTOP,		0x00070007 },
	{ (uintptr_t)CPG_BUS_PERI_COM_MSTOP,	0x3FFF3FFF },
	{ (uintptr_t)CPG_BUS_PERI_DDR_MSTOP,	0x00030003 },
	{ (uintptr_t)CPG_BUS_PERI_VIDEO1_MSTOP,	0xFFFFFFFF },
	{ (uintptr_t)CPG_BUS_TZCDDR_MSTOP,		0x000F000F },
	{ (uintptr_t)CPG_MHU_MSTOP,				0x00010001 },
	{ (uintptr_t)CPG_BUS_PERI_VIDEO2_MSTOP, 0x00010001 },
	{ (uintptr_t)CPG_PWRDN_MSTOP,			0x00010001 },
};
#endif

#if PLAT_SYSTEM_SUSPEND_vbat
static const cpg_setup_data_t cpg_s2r_clkrst_tbl[] = {

	{		/* I2C Ch0 */
		(uintptr_t)CPG_CLKON_I2C,
		(uintptr_t)CPG_CLKMON_I2C,
		0x0010001,
		CPG_T_CLK
	},
	{		/* I2C Ch0 */
		(uintptr_t)CPG_RST_I2C,
		(uintptr_t)CPG_RSTMON_I2C,
		0x00010001,
		CPG_T_RST
	},
	{		/* PCI */
		(uintptr_t)CPG_RST_PCI,
		(uintptr_t)CPG_RSTMON_PCI,
		0x00010000,
		CPG_T_RST
	},
	{		/* VBAT */
		(uintptr_t)CPG_CLKON_VBAT,
		(uintptr_t)CPG_CLKMON_VBAT,
		0x00010001,
		CPG_T_CLK
	},
	{		/* VBAT */
		(uintptr_t)CPG_RST_VBAT,
		(uintptr_t)CPG_RSTMON_VBAT,
		0x00010001,
		CPG_T_RST
	},
};

static const cpg_reg_setting_t cpg_s2r_mstop_tbl[] = {

			/* I2C Ch0 */
	{ (uintptr_t)CPG_BUS_MCPU2_MSTOP,	0x04000000 },
			/* VBAT */
	{ (uintptr_t)CPG_BUS_MCPU3_MSTOP,	0x01000000 },

};
#endif /* PLAT_SYSTEM_SUSPEND */

static const cpg_setup_data_t cpg_m33_clkrst_tbl[] = {
#if PLAT_M33_BOOT_SUPPORT
	{		/* CM33 */
		(uintptr_t)CPG_CLKON_CM33,
		(uintptr_t)CPG_CLKMON_CM33,
		0x00010001,
		CPG_T_CLK
	},
	{		/* CM33 */
		(uintptr_t)CPG_RST_CM33,
		(uintptr_t)CPG_RSTMON_CM33,
		0x00070007,
		CPG_T_RST
	},
#endif /* PLAT_M33_BOOT_SUPPORT */
};

static const cpg_setup_data_t cpg_iso_clock_tbl[] = {
	{		/* SDHI */
		(uintptr_t)CPG_CLKON_SDHI,
		(uintptr_t)CPG_CLKMON_SDHI,
		0x0FFF0FFF,
		CPG_T_CLK
	},
	{		/* USB */
		(uintptr_t)CPG_CLKON_USB,
		(uintptr_t)CPG_CLKMON_USB,
		0x001F001F,
		CPG_T_CLK
	},
	{		/* DDR */
		(uintptr_t)CPG_CLKON_DDR,
		(uintptr_t)CPG_CLKMON_DDR,
		0x003F0000,
		CPG_T_CLK
	},
	{		/* AXI_COM_BUS */
		(uintptr_t)CPG_CLKON_AXI_COM_BUS,
		(uintptr_t)CPG_CLKMON_AXI_COM_BUS,
		0x01010101,
		CPG_T_CLK
	},
	{		/* PERI_COM */
		(uintptr_t)CPG_CLKON_PERI_COM,
		(uintptr_t)CPG_CLKMON_PERI_COM,
		0x01010101,
		CPG_T_CLK
	},
	{		/* PERI_DDR */
		(uintptr_t)CPG_CLKON_PERI_DDR,
		(uintptr_t)CPG_CLKMON_PERI_DDR,
		0x00010001,
		CPG_T_CLK
	},
	{		/* AXI_TZCDDR */
		(uintptr_t)CPG_CLKON_AXI_TZCDDR,
		(uintptr_t)CPG_CLKMON_AXI_TZCDDR,
		0x007F007F,
		CPG_T_CLK
	}
};

static const cpg_setup_data_t cpg_iso_reset_tbl[] = {
	{		/* SDHI */
		(uintptr_t)CPG_RST_SDHI,
		(uintptr_t)CPG_RSTMON_SDHI,
		0x01FF01FF,
		CPG_T_RST
	},
	{		/* USB */
		(uintptr_t)CPG_RST_USB,
		(uintptr_t)CPG_RSTMON_USB,
		0x001F001F,
		CPG_T_RST
	},
	{		/* DDR */
		(uintptr_t)CPG_RST_DDR,
		(uintptr_t)CPG_RSTMON_DDR,
		0x01FE0000,
		CPG_T_RST
	},
	{		/* AXI_COM_BUS */
		(uintptr_t)CPG_RST_AXI_COM_BUS,
		(uintptr_t)CPG_RSTMON_AXI_COM_BUS,
		0x00030003,
		CPG_T_RST
	},
	{		/* PERI_COM */
		(uintptr_t)CPG_RST_AXI_PERI_COM,
		(uintptr_t)CPG_RSTMON_PERI_COM,
		0x00030003,
		CPG_T_RST
	},
	{		/* PERI_DDR */
		(uintptr_t)CPG_RST_PERI_DDR,
		(uintptr_t)CPG_RSTMON_PERI_DDR,
		0x00010001,
		CPG_T_RST
	},
	{		/* AXI_TZCDDR */
		(uintptr_t)CPG_RST_AXI_TZCDDR,
		(uintptr_t)CPG_RSTMON_AXI_TZCDDR,
		0x003F003F,
		CPG_T_RST
	},
};

static const cpg_setup_data_t cpg_awo_clock_tbl[] = {
#if PLAT_M33_BOOT_SUPPORT
	{		/* CM33 */
		(uintptr_t)CPG_CLKON_CM33,
		(uintptr_t)CPG_CLKMON_CM33,
		0x03030000,
		CPG_T_CLK
	},
#endif /* PLAT_M33_BOOT_SUPPORT */
	{
			/* IA55 */
		(uintptr_t)CPG_CLKON_IA55,
		(uintptr_t)CPG_CLKMON_IA55,
		0x00030003,
		CPG_T_CLK
	},
	{		/* WDT */
		(uintptr_t)CPG_CLKON_WDT,
		(uintptr_t)CPG_CLKMON_WDT,
		0x003F003F,
		CPG_T_CLK
	},
	{		/* XSPI */
		(uintptr_t)CPG_CLKON_XSPI,
		(uintptr_t)CPG_CLKMON_XSPI,
		0x000F000F,
		CPG_T_CLK
	},
	{		/* SCIF */
		(uintptr_t)CPG_CLKON_SCIF,
		(uintptr_t)CPG_CLKMON_SCIF,
		0x003F0001,
		CPG_T_CLK
	},
	{		/* GPIO */
		(uintptr_t)CPG_CLKON_GPIO,
		(uintptr_t)CPG_CLKMON_GPIO,
		0x00010001,
		CPG_T_CLK
	}
};

static const cpg_setup_data_t cpg_awo_reset_tbl[] = {
#if PLAT_M33_BOOT_SUPPORT
	{		/* CM33 */
		(uintptr_t)CPG_RST_CM33,
		(uintptr_t)CPG_RSTMON_CM33,
		0x07070000,
		CPG_T_RST
	},
#endif /* PLAT_M33_BOOT_SUPPORT */
	{		/* IA55 */
		(uintptr_t)CPG_RST_IA55,
		(uintptr_t)CPG_RSTMON_IA55,
		0x00010001,
		CPG_T_RST
	},
	{		/* WDT */
		(uintptr_t)CPG_RST_WDT,
		(uintptr_t)CPG_RSTMON_WDT,
		0x00070007,
		CPG_T_RST
	},
	{		/* XSPI */
		(uintptr_t)CPG_RST_XSPI,
		(uintptr_t)CPG_RSTMON_XSPI,
		0x00030003,
		CPG_T_RST
	},
	{		/* SCIF */
		(uintptr_t)CPG_RST_SCIF,
		(uintptr_t)CPG_RSTMON_SCIF,
		0x003F0001,
		CPG_T_RST
	},
	{		/* GPIO */
		(uintptr_t)CPG_RST_GPIO,
		(uintptr_t)CPG_RSTMON_GPIO,
		0x00070007,
		CPG_T_RST
	}
};

static cpg_setup_data_t cpg_clk_rst_wdt0_ctrl_tbl[] = {
	{		/* WDT */
		(uintptr_t)CPG_CLKON_WDT,
		(uintptr_t)CPG_CLKMON_WDT,
		0x00030003,
		CPG_T_CLK
	},

	{		/* WDT */
		(uintptr_t)CPG_RST_WDT,
		(uintptr_t)CPG_RSTMON_WDT,
		0x00010001,
		CPG_T_RST
	}
};

static void cpg_sel_setup(const cpg_reg_setting_t *tbl, const uint32_t size)
{
	int cnt;

	while (mmio_read_32(CPG_CLKSELSTATUS) != 0)
		;

	for (cnt = 0; cnt < size; cnt++, tbl++) {
		mmio_write_32(tbl->reg, tbl->val);
	}

	/* Wait for completion of settings */
	while (mmio_read_32(CPG_CLKSELSTATUS) != 0)
		;
}

static void cpg_div_setup(const cpg_reg_setting_t *tbl, const uint32_t size)
{
	int cnt;

	while (mmio_read_32(CPG_CLKDIVSTATUS) != 0)
		;

	for (cnt = 0; cnt < size; cnt++, tbl++) {
		mmio_write_32(tbl->reg, tbl->val);
	}

	/* Wait for completion of settings */
	while (mmio_read_32(CPG_CLKDIVSTATUS) != 0)
		;
}

#if PLAT_SYSTEM_SUSPEND
static void cpg_module_start(const cpg_reg_setting_t *tbl, const uint32_t size)
{
	int cnt;

	for (cnt = 0; cnt < size; cnt++, tbl++) {
		mmio_write_32(tbl->reg, tbl->val & 0xFFFF0000);
	}
}
#endif

static void cpg_clkrst_stop(const cpg_setup_data_t *tbl, const uint32_t size)
{
	int cnt;
	uint32_t mask;
	uint32_t cmp;

	for (cnt = 0; cnt < size; cnt++, tbl++) {
		mmio_write_32(tbl->reg, tbl->val & 0xFFFF0000);

		mask = (tbl->val >> 16) & 0xFFFF;
		cmp = 0;
		if (tbl->type == CPG_T_RST) {
			cmp = ~(cmp);
		}
		while ((mmio_read_32(tbl->mon) & mask) != (cmp & mask))
			;
	}
}

static void cpg_clkrst_start(const cpg_setup_data_t *tbl, const uint32_t size)
{
	int cnt;
	uint32_t mask;
	uint32_t cmp;

	for (cnt = 0; cnt < size; cnt++, tbl++) {
		mmio_write_32(tbl->reg, tbl->val);

		mask = (tbl->val >> 16) & 0xFFFF;
		cmp = tbl->val & 0xFFFF;
		if (tbl->type == CPG_T_RST) {
			cmp = ~(cmp);
		}
		while ((mmio_read_32(tbl->mon) & mask) != (cmp & mask))
			;
	}
}

/* It is assumed that the PLL has stopped by the time this function is executed. */
static void cpg_pll_setup(void)
{
	int cnt;
	uint32_t val = 0;

	do {
		val = mmio_read_32(CPG_PLL4_MON);
	} while ((val & (PLL4_MON_PLL4_RESETB | PLL4_MON_PLL4_LOCK)) != 0);

	for (cnt = 0 ; cnt < ARRAY_SIZE(cpg_pll4_tbl); cnt++) {
		mmio_write_32(cpg_pll4_tbl[cnt].reg, cpg_pll4_tbl[cnt].val);
	}

	do {
		val = mmio_read_32(CPG_PLL4_MON);
	} while ((val & (PLL4_MON_PLL4_RESETB | PLL4_MON_PLL4_LOCK)) == 0);
}

void cpg_reset_wdt0(void)
{
	/* WDT reset apply */
	cpg_clkrst_stop(&cpg_clk_rst_wdt0_ctrl_tbl[0], ARRAY_SIZE(cpg_clk_rst_wdt0_ctrl_tbl));
	udelay(1);

	cpg_clkrst_start(&cpg_clk_rst_wdt0_ctrl_tbl[0], ARRAY_SIZE(cpg_clk_rst_wdt0_ctrl_tbl));
	udelay(1);
}

void cpg_setup_wdt0(void)
{
	mmio_write_32(CPG_WDTRST_SEL, mmio_read_32(CPG_WDTRST_SEL) | WDTRST_SEL_WDTRSTSEL0 | WDTRST_SEL_WDTRSTSEL0_WEN);
	mmio_write_32(CPG_BUS_REG0_MSTOP, BUS_MSTOP_MSTOP0_ON_WEN);
}

static void cpg_div_sel_static_setup(void)
{
	cpg_clkrst_stop(cpg_static_clock_tbl, ARRAY_SIZE(cpg_static_clock_tbl));
	cpg_sel_setup(cpg_static_select_tbl, ARRAY_SIZE(cpg_static_select_tbl));
}

static void cpg_div_sel_dynamic_setup(void)
{
	cpg_sel_setup(cpg_dynamic_select_tbl, ARRAY_SIZE(cpg_dynamic_select_tbl));
	cpg_div_setup(cpg_dynamic_division_tbl, ARRAY_SIZE(cpg_dynamic_division_tbl));
}

static void cpg_clock_on_setup(void)
{
	cpg_clkrst_start(cpg_iso_clock_tbl, ARRAY_SIZE(cpg_iso_clock_tbl));
	cpg_clkrst_start(cpg_awo_clock_tbl, ARRAY_SIZE(cpg_awo_clock_tbl));

	cpg_clkrst_start(cpg_iso_reset_tbl, ARRAY_SIZE(cpg_iso_reset_tbl));
	cpg_clkrst_start(cpg_awo_reset_tbl, ARRAY_SIZE(cpg_awo_reset_tbl));
}

static void cpg_wdtrst_sel_setup(void)
{
	uintptr_t reg = mmio_read_32(CPG_WDTRST_SEL);

	reg |=  WDTRST_SEL_WDTRSTSEL0 | WDTRST_SEL_WDTRSTSEL0_WEN |
			WDTRST_SEL_WDTRSTSEL1 | WDTRST_SEL_WDTRSTSEL1_WEN |
			WDTRST_SEL_WDTRSTSEL2 | WDTRST_SEL_WDTRSTSEL2_WEN;

	mmio_write_32(CPG_WDTRST_SEL, reg);
}

static void cpg_pwrdown_ip_setup(void)
{
	int cnt;
	const cpg_reg_setting_t *tbl = &cpg_pwrdown_ip_tbl[0];

	for (cnt = 0; cnt < ARRAY_SIZE(cpg_pwrdown_ip_tbl); cnt++, tbl++) {
		mmio_write_32(tbl->reg, tbl->val);
	}
}

void cpg_early_setup(void)
{
	cpg_div_setup(cpg_early_div_tbl, ARRAY_SIZE(cpg_early_div_tbl));
	cpg_clkrst_start(cpg_early_clkrst_tbl, ARRAY_SIZE(cpg_early_clkrst_tbl));
}

#if PLAT_SYSTEM_SUSPEND_vbat
void cpg_setup_vbat_suspend(void)
{
	/* Enable IP to use for suspend */
	cpg_clkrst_start(cpg_s2r_clkrst_tbl, ARRAY_SIZE(cpg_s2r_clkrst_tbl));
	cpg_module_start(cpg_s2r_mstop_tbl,  ARRAY_SIZE(cpg_s2r_mstop_tbl));
}
#endif

void cpg_setup(void)
{
	mmio_write_32(CPG_PLL6_STBY, 0x10001);
	cpg_pll_setup();
	cpg_div_sel_static_setup();
	cpg_clock_on_setup();
	cpg_div_sel_dynamic_setup();
	cpg_wdtrst_sel_setup();
	cpg_pwrdown_ip_setup();
}

void cpg_active_ddr1(void)
{
	/* 2 */
	mmio_write_32(CPG_RST_DDR, 0x01FE0000);
	mmio_write_32(CPG_OTHERFUNC2_REG, 0x00010000);

	/* 3 */
	/* PLL functions performed in cpg_pll_setup*/
	/* CLKSELSTATUS and DSEL functions performed in cpg_div_sel_dynamic_setup*/
	mmio_write_32(CPG_CLKON_DDR, 0x003F003F);
	while ((mmio_read_32(CPG_CLKMON_DDR) & 0x0000003F) != 0x0000003F)
		;

	/* 4 */
	wait_regaclk(5);

	/* 5 */
	mmio_write_32(CPG_RST_DDR, 0x01000100);
	mmio_write_32(CPG_OTHERFUNC2_REG, 0x00010001);
	/* 6 */
	wait_regaclk(1);
	/* 7 */
	mmio_write_32(CPG_RST_DDR, 0x007C007C);
	/* 8 */
	wait_regaclk(2);
}

void cpg_active_ddr2(void)
{
	/* 13.11 */
	mmio_write_32(CPG_RST_DDR, 0x00800080);
	/* 13.12 */
	wait_pclk(2);
	/* 13.13 */
	mmio_write_32(CPG_RST_DDR, 0x00020002);
	/* 13.14 */
	wait_pclk(5);
}

void cpg_resume_setup(void)
{
#if defined(PLAT_SYSTEM_SUSPEND_awo)
	cpg_clkrst_start(cpg_iso_reset_tbl, ARRAY_SIZE(cpg_iso_reset_tbl));
	cpg_clkrst_start(cpg_iso_clock_tbl, ARRAY_SIZE(cpg_iso_clock_tbl));
	cpg_module_start(cpg_iso_mstop_tbl, ARRAY_SIZE(cpg_iso_mstop_tbl));
#endif
}

void cpg_m33_setup(void)
{
	cpg_clkrst_start(cpg_m33_clkrst_tbl, ARRAY_SIZE(cpg_m33_clkrst_tbl));
}

#if PLAT_SYSTEM_SUSPEND
static const cpg_reg_setting_t cpg_gpio_mstop_tbl[] = {
	{ (uintptr_t)CPG_BUS_PERI_CPU_MSTOP, 0x00400000 },
};

void cpg_prepare_suspend(void)
{
	cpg_module_start(cpg_gpio_mstop_tbl, ARRAY_SIZE(cpg_gpio_mstop_tbl));
}
#endif /* PLAT_SYSTEM_SUSPEND */
