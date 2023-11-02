/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <cpg_regs.h>
#include <lib/mmio.h>
#include <drivers/delay_timer.h>

#define CPG_T_CLK						(0)
#define CPG_T_RST						(1)

typedef struct {
	uintptr_t reg;
	uintptr_t mon;
	uint32_t  val;
	uint32_t  type;
} CPG_SETUP_DATA;

typedef struct {
	uintptr_t reg;
	uint32_t  val;
} CPG_REG_SETTING;

static const CPG_REG_SETTING cpg_early_div_tbl[] = {
	{ (uintptr_t)CPG_PL1_DDIV,				0x00010000 },	// 2'b00:1/1
};

static const CPG_SETUP_DATA cpg_early_clkrst_tbl[] = {
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

static const CPG_REG_SETTING cpg_pll4_tbl[] = {
#if (DDR_PLL4 >= 800)
	{ CPG_PLL4_CLK1, 0x0498E000 },
	{ CPG_PLL4_CLK2, 0x00000002 },
#else
#error "Unknown DDR Type."
#endif
	{ CPG_PLL4_STBY, 0x00010001 }
};

static const CPG_SETUP_DATA cpg_static_clock_tbl[] = {
	{		/* OCTA */
		(uintptr_t)CPG_CLKON_OCTA,
		(uintptr_t)CPG_CLKMON_OCTA,
		0x00030000,
		CPG_T_CLK
	},
	{		/* SPI */
		(uintptr_t)CPG_CLKON_SPI,
		(uintptr_t)CPG_CLKMON_SPI,
		0x000F0000,
		CPG_T_CLK
	},
};

static const CPG_REG_SETTING cpg_static_select_tbl[] = {
	{ (uintptr_t)CPG_OCTA_SSEL,				0x00010002 },	// 2'b10:CLK266FIX_CD
	{ (uintptr_t)CPG_SPI_SSEL,				0x00010002 },	// 2'b10:CLK266FIX_CD
};

static const CPG_REG_SETTING cpg_dynamic_select_tbl[] = {
	{ (uintptr_t)CPG_PLL_DSEL,				0x00400040 },	// 1'b1:PLL4
	{ (uintptr_t)CPG_SDHI_DSEL,				0x01110333 },	// 2'b11:CLK266FIX_C, 2'b11:CLK266FIX_C, 2'b11:CLK266FIX_C
};

static const CPG_REG_SETTING cpg_dynamic_division_tbl[] = {
	{ (uintptr_t)CPG_PL2_DDIV,				0x00100000 },	// 3'b000:1/1(100MHz)
	{ (uintptr_t)CPG_PL3_DDIV,				0x01110000 },	// 3'b000:1/1(200MHz), 3'b000:1/1(100MHz), 3'b000:1/1(200MHz)
#if PLAT_M33_BOOT_SUPPORT
	{ (uintptr_t)CPG_PL6_DDIV,				0x00110000 },	// 3'b000:1/1(250MHz), 3'b000:1/1(250MHz)
#endif /* PLAT_M33_BOOT_SUPPORT */
	{ (uintptr_t)CPG_SDHI_DDIV,				0x01110111 },	// 1'b1:1/2, 1'b1:1/2, 1'b1:1/2
	{ (uintptr_t)CPG_OCTA_DDIV,				0x00010003 },	// 3'b011:1/8
	{ (uintptr_t)CPG_SPI_DDIV,				0x00010003 },	// 3'b011:1/8
};

static const CPG_REG_SETTING cpg_pwrdown_ip_tbl[] = {
	{ (uintptr_t)CPG_PWRDN_IP1,				0xFF7FFF7E },
	{ (uintptr_t)CPG_PWRDN_IP2,				0x001F001F },
};

#if defined(PLAT_SYSTEM_SUSPEND_awo)
static const CPG_REG_SETTING cpg_iso_mstop_tbl[] = {
	{ (uintptr_t)CPG_BUS_ACPU_MSTOP,		0x00080008 },
	{ (uintptr_t)CPG_BUS_PERI_COM_MSTOP,	0x0FFF0FFF },
	{ (uintptr_t)CPG_BUS_PERI_DDR_MSTOP,	0x00030003 },
	{ (uintptr_t)CPG_BUS_TZCDDR_MSTOP,		0x00070007 },
	{ (uintptr_t)CPG_MHU_MSTOP,				0x00010001 },
	{ (uintptr_t)CPG_PWRDN_MSTOP,			0x00010001 },
};
#endif

static const CPG_SETUP_DATA cpg_s2r_clkrst_tbl[] = {
#if PLAT_SYSTEM_SUSPEND
	{		/* I2C Ch1 */
		(uintptr_t)CPG_CLKON_I2C,
		(uintptr_t)CPG_CLKMON_I2C,
		0x00020002,
		CPG_T_CLK
	},
	{		/* I2C Ch1 */
		(uintptr_t)CPG_RST_I2C,
		(uintptr_t)CPG_RSTMON_I2C,
		0x00020002,
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
#endif
};

static const CPG_REG_SETTING cpg_s2r_mstop_tbl[] = {
#if PLAT_SYSTEM_SUSPEND
			/* I2C Ch1 */
	{ (uintptr_t)CPG_BUS_MCPU2_MSTOP,	0x08000000 },
			/* VBAT */
	{ (uintptr_t)CPG_BUS_MCPU3_MSTOP,	0x01000000 },
#endif
};

static const CPG_SETUP_DATA cpg_ddr_clkrst_tbl[] = {
	{		/* DDR */
		(uintptr_t)CPG_CLKON_DDR,
		(uintptr_t)CPG_CLKMON_DDR,
		0x000F000F,
		CPG_T_CLK
	},
	{		/* DDR */
		(uintptr_t)CPG_RST_DDR,
		(uintptr_t)CPG_RSTMON_DDR,
		0x01CF0000,
		CPG_T_RST
	},
	{		/* DDR */
		(uintptr_t)CPG_RST_DDR,
		(uintptr_t)CPG_RSTMON_DDR,
		0x01000100,
		CPG_T_RST
	},
	{		/* DDR */
		(uintptr_t)CPG_RST_DDR,
		(uintptr_t)CPG_RSTMON_DDR,
		0x004D004D,
		CPG_T_RST
	},
	{		/* DDR */
		(uintptr_t)CPG_RST_DDR,
		(uintptr_t)CPG_RSTMON_DDR,
		0x00820082,
		CPG_T_RST
	},
};

static const CPG_SETUP_DATA cpg_m33_clkrst_tbl[] = {
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

static const CPG_SETUP_DATA cpg_iso_clock_tbl[] = {
	{		/* GIC */
		(uintptr_t)CPG_CLKON_GIC600,
		(uintptr_t)CPG_CLKMON_GIC600,
		0x00010001,
		CPG_T_CLK
	},
	{		/* MHU */
		(uintptr_t)CPG_CLKON_MHU,
		(uintptr_t)CPG_CLKMON_MHU,
		0x00010000,
		CPG_T_CLK
	},
	{		/* SDHI */
		(uintptr_t)CPG_CLKON_SDHI,
		(uintptr_t)CPG_CLKMON_SDHI,
		0x0FFF0FFF,
		CPG_T_CLK
	},
	{		/* USB */
		(uintptr_t)CPG_CLKON_USB,
		(uintptr_t)CPG_CLKMON_USB,
		0x000F0000,
		CPG_T_CLK
	},
	{		/* ETHER */
		(uintptr_t)CPG_CLKON_ETH,
		(uintptr_t)CPG_CLKMON_ETH,
		0x03030000,
		CPG_T_CLK
	},
	{		/* DDR */
		(uintptr_t)CPG_CLKON_DDR,
		(uintptr_t)CPG_CLKMON_DDR,
		0x000F0000,
		CPG_T_CLK
	},
	{		/* AXI_COM_BUS */
		(uintptr_t)CPG_CLKON_AXI_COM_BUS,
		(uintptr_t)CPG_CLKMON_AXI_COM_BUS,
		0x03030303,
		CPG_T_CLK
	},
	{		/* PERI_COM */
		(uintptr_t)CPG_CLKON_PERI_COM,
		(uintptr_t)CPG_CLKMON_PERI_COM,
		0x03030303,
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
		0x000F000F,
		CPG_T_CLK
	},
	{		/* OTFDE_DDR */
		(uintptr_t)CPG_CLKON_OTFDE_DDR,
		(uintptr_t)CPG_CLKMON_OTFDE_DDR,
		0x00030000,
		CPG_T_CLK
	},
	{		/* PCI */
		(uintptr_t)CPG_CLKON_PCI,
		(uintptr_t)CPG_CLKMON_PCI,
		0x00030000,
		CPG_T_CLK
	},
};

static const CPG_SETUP_DATA cpg_iso_reset_tbl[] = {
	{		/* GIC */
		(uintptr_t)CPG_RST_GIC600,
		(uintptr_t)CPG_RSTMON_GIC600,
		0x00030003,
		CPG_T_RST
	},
	{		/* MHU */
		(uintptr_t)CPG_RST_MHU,
		(uintptr_t)CPG_RSTMON_MHU,
		0x00010000,
		CPG_T_RST
	},
	{		/* SDHI */
		(uintptr_t)CPG_RST_SDHI,
		(uintptr_t)CPG_RSTMON_SDHI,
		0x00070007,
		CPG_T_RST
	},
	{		/* USB */
		(uintptr_t)CPG_RST_USB,
		(uintptr_t)CPG_RSTMON_USB,
		0x000F0000,
		CPG_T_RST
	},
	{		/* ETHER */
		(uintptr_t)CPG_RST_ETH,
		(uintptr_t)CPG_RSTMON_ETH,
		0x00030000,
		CPG_T_RST
	},
	{		/* DDR */
		(uintptr_t)CPG_RST_DDR,
		(uintptr_t)CPG_RSTMON_DDR,
		0x01CF0000,
		CPG_T_RST
	},
	{		/* AXI_COM_BUS */
		(uintptr_t)CPG_RST_AXI_COM_BUS,
		(uintptr_t)CPG_RSTMON_AXI_COM_BUS,
		0x00030003,
		CPG_T_RST
	},
	{		/* PERI_COM */
		(uintptr_t)CPG_RST_PERI_COM,
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
		0x000F000F,
		CPG_T_RST
	},
	{		/* OTFDE_DDR */
		(uintptr_t)CPG_RST_OTFDE_DDR,
		(uintptr_t)CPG_RSTMON_OTFDE_DDR,
		0x00030000,
		CPG_T_RST
	},
	{		/* PCI */
		(uintptr_t)CPG_RST_PCI,
		(uintptr_t)CPG_RSTMON_PCI,
		0x007F0000,
		CPG_T_RST
	},
};

static const CPG_SETUP_DATA cpg_awo_clock_tbl[] = {
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
	{		/* DMAC_REG */
		(uintptr_t)CPG_CLKON_DMAC_REG,
		(uintptr_t)CPG_CLKMON_DMAC_REG,
		0x00030001,
		CPG_T_CLK
	},
	{		/* OSTM */
		(uintptr_t)CPG_CLKON_GTM,
		(uintptr_t)CPG_CLKMON_GTM,
		0x00FF0000,
		CPG_T_CLK
	},
	{		/* MTU */
		(uintptr_t)CPG_CLKON_MTU,
		(uintptr_t)CPG_CLKMON_MTU,
		0x00010000,
		CPG_T_CLK
	},
	{		/* POE3 */
		(uintptr_t)CPG_CLKON_POE3,
		(uintptr_t)CPG_CLKMON_POE3,
		0x00010000,
		CPG_T_CLK
	},
	{		/* GPT */
		(uintptr_t)CPG_CLKON_GPT,
		(uintptr_t)CPG_CLKMON_GPT,
		0x00010000,
		CPG_T_CLK
	},
	{		/* POEG */
		(uintptr_t)CPG_CLKON_POEG,
		(uintptr_t)CPG_CLKMON_POEG,
		0x000F0000,
		CPG_T_CLK
	},
	{		/* WDT */
		(uintptr_t)CPG_CLKON_WDT,
		(uintptr_t)CPG_CLKMON_WDT,
		0x00FF0000,
		CPG_T_CLK
	},
	{		/* SPI */
		(uintptr_t)CPG_CLKON_SPI,
		(uintptr_t)CPG_CLKMON_SPI,
		0x000F000F,
		CPG_T_CLK
	},
	{		/* SSI */
		(uintptr_t)CPG_CLKON_SSI,
		(uintptr_t)CPG_CLKMON_SSI,
		0x00FF0000,
		CPG_T_CLK
	},
	{		/* SRC */
		(uintptr_t)CPG_CLKON_SRC,
		(uintptr_t)CPG_CLKMON_SRC,
		0x00010000,
		CPG_T_CLK
	},
	{		/* SCIF */
		(uintptr_t)CPG_CLKON_SCIF,
		(uintptr_t)CPG_CLKMON_SCIF,
		0x003F0001,
		CPG_T_CLK
	},
	{		/* SCI */
		(uintptr_t)CPG_CLKON_SCI,
		(uintptr_t)CPG_CLKMON_SCI,
		0x00030000,
		CPG_T_CLK
	},
	{		/* IrDA */
		(uintptr_t)CPG_CLKON_IRDA,
		(uintptr_t)CPG_CLKMON_IRDA,
		0x00010000,
		CPG_T_CLK
	},
	{		/* RSPI */
		(uintptr_t)CPG_CLKON_RSPI,
		(uintptr_t)CPG_CLKMON_RSPI,
		0x001F0000,
		CPG_T_CLK
	},
	{		/* CANFD */
		(uintptr_t)CPG_CLKON_CANFD,
		(uintptr_t)CPG_CLKMON_CANFD,
		0x00030000,
		CPG_T_CLK
	},
	{		/* GPIO */
		(uintptr_t)CPG_CLKON_GPIO,
		(uintptr_t)CPG_CLKMON_GPIO,
		0x00010001,
		CPG_T_CLK
	},
	{		/* ADC */
		(uintptr_t)CPG_CLKON_ADC,
		(uintptr_t)CPG_CLKMON_ADC,
		0x00030000,
		CPG_T_CLK
	},
	{		/* TSU */
		(uintptr_t)CPG_CLKON_TSU,
		(uintptr_t)CPG_CLKMON_TSU,
		0x00010000,
		CPG_T_CLK
	},
	{		/* OCTA */
		(uintptr_t)CPG_CLKON_OCTA,
		(uintptr_t)CPG_CLKMON_OCTA,
		0x00030003,
		CPG_T_CLK
	},
	{		/* PDM */
		(uintptr_t)CPG_CLKON_PDM,
		(uintptr_t)CPG_CLKMON_PDM,
		0x003F0000,
		CPG_T_CLK
	},
	{		/* SPDIF */
		(uintptr_t)CPG_CLKON_SPDIF,
		(uintptr_t)CPG_CLKMON_SPDIF,
		0x00010000,
		CPG_T_CLK
	},
	{		/* I3C */
		(uintptr_t)CPG_CLKON_I3C,
		(uintptr_t)CPG_CLKMON_I3C,
		0x00030000,
		CPG_T_CLK
	},
};

static const CPG_SETUP_DATA cpg_awo_reset_tbl[] = {
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
	{		/* DMAC */
		(uintptr_t)CPG_RST_DMAC,
		(uintptr_t)CPG_RSTMON_DMAC,
		0x00030003,
		CPG_T_RST
	},
	{		/* OSTM */
		(uintptr_t)CPG_RST_GTM,
		(uintptr_t)CPG_RSTMON_GTM,
		0x00FF0000,
		CPG_T_RST
	},
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
	{		/* GPT */
		(uintptr_t)CPG_RST_GPT,
		(uintptr_t)CPG_RSTMON_GPT,
		0x00010000,
		CPG_T_RST
	},
	{		/* POEG */
		(uintptr_t)CPG_RST_POEG,
		(uintptr_t)CPG_RSTMON_POEG,
		0x000F0000,
		CPG_T_RST
	},
	{		/* WDT */
		(uintptr_t)CPG_RST_WDT,
		(uintptr_t)CPG_RSTMON_WDT,
		0x000F0000,
		CPG_T_RST
	},
	{		/* SPI */
		(uintptr_t)CPG_RST_SPI,
		(uintptr_t)CPG_RSTMON_SPI,
		0x00030003,
		CPG_T_RST
	},
	{		/* SSIF */
		(uintptr_t)CPG_RST_SSIF,
		(uintptr_t)CPG_RSTMON_SSIF,
		0x000F0000,
		CPG_T_RST
	},
	{		/* SRC */
		(uintptr_t)CPG_RST_SRC,
		(uintptr_t)CPG_RSTMON_SRC,
		0x00010000,
		CPG_T_RST
	},
	{		/* SCIF */
		(uintptr_t)CPG_RST_SCIF,
		(uintptr_t)CPG_RSTMON_SCIF,
		0x003F0001,
		CPG_T_RST
	},
	{		/* SCI */
		(uintptr_t)CPG_RST_SCI,
		(uintptr_t)CPG_RSTMON_SCI,
		0x00030000,
		CPG_T_RST
	},
	{		/* IrDA */
		(uintptr_t)CPG_RST_IRDA,
		(uintptr_t)CPG_RSTMON_IRDA,
		0x00010000,
		CPG_T_RST
	},
	{		/* RSPI */
		(uintptr_t)CPG_RST_RSPI,
		(uintptr_t)CPG_RSTMON_RSPI,
		0x001F0000,
		CPG_T_RST
	},
	{		/* CANFD */
		(uintptr_t)CPG_RST_CANFD,
		(uintptr_t)CPG_RSTMON_CANFD,
		0x00030000,
		CPG_T_RST
	},
	{		/* GPIO */
		(uintptr_t)CPG_RST_GPIO,
		(uintptr_t)CPG_RSTMON_GPIO,
		0x00010001,
		CPG_T_RST
	},
	{		/* ADC */
		(uintptr_t)CPG_RST_ADC,
		(uintptr_t)CPG_RSTMON_ADC,
		0x00030000,
		CPG_T_RST
	},
	{		/* TSU */
		(uintptr_t)CPG_RST_TSU,
		(uintptr_t)CPG_RSTMON_TSU,
		0x00010000,
		CPG_T_RST
	},
	{		/* OCTA */
		(uintptr_t)CPG_RST_OCTA,
		(uintptr_t)CPG_RSTMON_OCTA,
		0x00010001,
		CPG_T_RST
	},
	{		/* PDM */
		(uintptr_t)CPG_RST_PDM,
		(uintptr_t)CPG_RSTMON_PDM,
		0x00070000,
		CPG_T_RST
	},
	{		/* SPDIF */
		(uintptr_t)CPG_RST_SPDIF,
		(uintptr_t)CPG_RSTMON_SPDIF,
		0x00010000,
		CPG_T_RST
	},
	{		/* I3C */
		(uintptr_t)CPG_RST_I3C,
		(uintptr_t)CPG_RSTMON_I3C,
		0x00030000,
		CPG_T_RST
	},
};

static void cpg_sel_setup(const CPG_REG_SETTING *tbl, const uint32_t size)
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

static void cpg_div_setup(const CPG_REG_SETTING *tbl, const uint32_t size)
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

#if defined(PLAT_SYSTEM_SUSPEND_awo)
static void cpg_module_stop(const CPG_REG_SETTING *tbl, const uint32_t size)
{
	int cnt;

	for (cnt = 0; cnt < size; cnt++, tbl++) {
		mmio_write_32(tbl->reg, tbl->val);
	}
}
#endif

static void cpg_module_start(const CPG_REG_SETTING *tbl, const uint32_t size)
{
	int cnt;

	for (cnt = 0; cnt < size; cnt++, tbl++) {
		mmio_write_32(tbl->reg, tbl->val & 0xFFFF0000);
	}
}

static void cpg_clkrst_stop(const CPG_SETUP_DATA *tbl, const uint32_t size)
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

static void cpg_clkrst_start(const CPG_SETUP_DATA *tbl, const uint32_t size)
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
	const CPG_REG_SETTING *tbl = &cpg_pwrdown_ip_tbl[0];

	for (cnt = 0; cnt < ARRAY_SIZE(cpg_pwrdown_ip_tbl); cnt++, tbl++) {
		mmio_write_32(tbl->reg, tbl->val);
	}
}

void cpg_early_setup(void)
{
	cpg_div_setup(cpg_early_div_tbl, ARRAY_SIZE(cpg_early_div_tbl));
	cpg_clkrst_start(cpg_early_clkrst_tbl, ARRAY_SIZE(cpg_early_clkrst_tbl));
}

void cpg_setup(void)
{
	cpg_pll_setup();
	cpg_div_sel_static_setup();
	cpg_clock_on_setup();
	cpg_div_sel_dynamic_setup();
	cpg_wdtrst_sel_setup();
	cpg_pwrdown_ip_setup();
}

void cpg_active_ddr1(void)
{
	cpg_clkrst_start(&cpg_ddr_clkrst_tbl[0], 1);
	udelay(1);

	cpg_clkrst_stop(&cpg_ddr_clkrst_tbl[1], 1);
	mmio_write_32(CPG_OTHERFUNC2_REG, 0x00010000);
	udelay(1);

	cpg_clkrst_start(&cpg_ddr_clkrst_tbl[2], 1);
	mmio_write_32(CPG_OTHERFUNC2_REG, 0x00010001);
	udelay(1);

	cpg_clkrst_start(&cpg_ddr_clkrst_tbl[3], 1);
	udelay(1);
}

void cpg_active_ddr2(void)
{
	cpg_clkrst_start(&cpg_ddr_clkrst_tbl[4], 1);
	udelay(1);
}

void cpg_prepare_suspend(void)
{
	/* Enable IP to use for suspend */
	cpg_clkrst_start(cpg_s2r_clkrst_tbl, ARRAY_SIZE(cpg_s2r_clkrst_tbl));
	cpg_module_start(cpg_s2r_mstop_tbl,  ARRAY_SIZE(cpg_s2r_mstop_tbl));
}

void cpg_suspend_setup(void)
{
#if defined(PLAT_SYSTEM_SUSPEND_awo)
	cpg_module_stop(cpg_iso_mstop_tbl, ARRAY_SIZE(cpg_iso_mstop_tbl));
	cpg_clkrst_stop(cpg_iso_clock_tbl, ARRAY_SIZE(cpg_iso_clock_tbl));
	cpg_clkrst_stop(cpg_iso_reset_tbl, ARRAY_SIZE(cpg_iso_reset_tbl));
#endif
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
