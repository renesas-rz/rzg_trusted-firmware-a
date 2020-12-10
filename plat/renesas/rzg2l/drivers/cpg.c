/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include "cpg_regs.h"
#include "cpg.h"
#include <lib/mmio.h>
#include <drivers/delay_timer.h>

#define	CPG_OFF			(0)
#define	CPG_ON			(1)

#define CPG_T_CLK		(0)
#define CPG_T_RST		(1)

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

#define	CPG_PLL1_INDEX					(0)
#define	CPG_PLL4_INDEX					(1)
#define	CPG_PLL6_INDEX					(2)

static CPG_PLL_SETDATA_146 cpg_pll_setdata_146[] = {
	{
		{ CPG_PLL1_CLK1, 0x00001901 },
		{ CPG_PLL1_CLK2, 0x00180601 },
		{ CPG_PLL1_STBY, 0x00000005 }
	},
	{
		{ CPG_PLL4_CLK1, 0x00003203 },
		{ CPG_PLL4_CLK2, 0x00082400 },
		{ CPG_PLL4_STBY, 0x00010005 }
	},
	{
		{ CPG_PLL6_CLK1, 0x00003E83 },
		{ CPG_PLL6_CLK2, 0x00082D02 },
		{ CPG_PLL6_STBY, 0x00010001 }
	}
};

#define	CPG_PLL2_INDEX					(0)
#define	CPG_PLL3_INDEX					(1)
#define	CPG_PLL5_INDEX					(2)

static CPG_PLL_SETDATA_235 cpg_pll_setdata_235[] = {
	{
		{ CPG_PLL2_CLK1, 0x01110111 },
		{ CPG_PLL2_CLK2, 0x01550100 },
		{ CPG_PLL2_CLK3, 0x55555506 },
		{ CPG_PLL2_CLK4, 0x008500FF },
		{ CPG_PLL2_CLK5, 0x00000016 },
		{ CPG_PLL2_STBY, 0x00000011 }
	},
	{
		{ CPG_PLL3_CLK1, 0x01110111 },
		{ CPG_PLL3_CLK2, 0x01550100 },
		{ CPG_PLL3_CLK3, 0x55555506 },
		{ CPG_PLL3_CLK4, 0x008500FF },
		{ CPG_PLL3_CLK5, 0x00000016 },
		{ CPG_PLL3_STBY, 0x00000011 }
	},
	{
		{ CPG_PLL5_CLK1, 0x01110111 },
		{ CPG_PLL5_CLK2, 0x01550100 },
		{ CPG_PLL5_CLK3, 0x00000006 },
		{ CPG_PLL5_CLK4, 0x007D00FF },
		{ CPG_PLL5_CLK5, 0x00000016 },
		{ CPG_PLL5_STBY, 0x00010015 }
	}
};

static const CPG_SETUP_DATA early_setup_tbl[] = {
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
	}
};

static CPG_SETUP_DATA cpg_clk_on_tbl[] = {
	{		/* CM33 */
		(uintptr_t)CPG_CLKON_CM33,
		(uintptr_t)CPG_CLKMON_CM33,
		0x00000000,
		CPG_T_CLK
	},
	{		/* ROM */
		(uintptr_t)CPG_CLKON_ROM,
		(uintptr_t)CPG_CLKMON_ROM,
		0x00010001,
		CPG_T_CLK
	},
	{		/* GIC600 */
		(uintptr_t)CPG_CLKON_GIC600,
		(uintptr_t)CPG_CLKMON_GIC600,
		0x00010001,
		CPG_T_CLK
	},
	{		/* IA55 */
		(uintptr_t)CPG_CLKON_IA55,
		(uintptr_t)CPG_CLKMON_IA55,
		0x00030003,
		CPG_T_CLK
	},
	{		/* IM33 */
		(uintptr_t)CPG_CLKON_IM33,
		(uintptr_t)CPG_CLKMON_IM33,
		0x00030003,
		CPG_T_CLK
	},
	{		/* MHU */
		(uintptr_t)CPG_CLKON_MHU,
		(uintptr_t)CPG_CLKMON_MHU,
		0x00000000,
		CPG_T_CLK
	},
	{		/* CST */
		(uintptr_t)CPG_CLKON_CST,
		(uintptr_t)CPG_CLKMON_CST,
		0x07ff07ff,
		CPG_T_CLK
	},
	{		/* DMAC */
		(uintptr_t)CPG_CLKON_DAMC_REG,
		(uintptr_t)CPG_CLKMON_DAMC_REG,
		0x00000000,
		CPG_T_CLK
	},
	{		/* SYSC */
		(uintptr_t)CPG_CLKON_SYSC,
		(uintptr_t)CPG_CLKMON_SYSC,
		0x00030003,
		CPG_T_CLK
	},
	{		/* OSTM */
		(uintptr_t)CPG_CLKON_OSTM,
		(uintptr_t)CPG_CLKMON_OSTM,
		0x00010001,
		CPG_T_CLK
	},
	{		/* MTU */
		(uintptr_t)CPG_CLKON_MTU,
		(uintptr_t)CPG_CLKMON_MTU,
		0x00000000,
		CPG_T_CLK
	},
	{		/* POE3 */
		(uintptr_t)CPG_CLKON_POE3,
		(uintptr_t)CPG_CLKMON_POE3,
		0x00000000,
		CPG_T_CLK
	},
	{		/* GPT */
		(uintptr_t)CPG_CLKON_GPT,
		(uintptr_t)CPG_CLKMON_GPT,
		0x00000000,
		CPG_T_CLK
	},
	{		/* POEG */
		(uintptr_t)CPG_CLKON_POEG,
		(uintptr_t)CPG_CLKMON_POEG,
		0x00000000,
		CPG_T_CLK
	},
	{		/* WDT */
		(uintptr_t)CPG_CLKON_WDT,
		(uintptr_t)CPG_CLKMON_WDT,
		0x00C300C3,
		CPG_T_CLK
	},
#if !DEBUG_RZG2L_FPGA
	{		/* DDR */
		(uintptr_t)CPG_CLKON_DDR,
		(uintptr_t)CPG_CLKMON_DDR,
		0x00000000,
		CPG_T_CLK
	},
#else
	{		/* DDR */
		(uintptr_t)CPG_CLKON_DDR,
		(uintptr_t)CPG_CLKMON_DDR,
		0x00030003,
		CPG_T_CLK
	},
#endif
	{		/* SPI_MULTI */
		(uintptr_t)CPG_CLKON_SPI_MULTI,
		(uintptr_t)CPG_CLKMON_SPI_MULTI,
		0x00030003,
		CPG_T_CLK
	},
	{		/* SDHI */
		(uintptr_t)CPG_CLKON_SDHI,
		(uintptr_t)CPG_CLKMON_SDHI,
		0x00ff00ff,
		CPG_T_CLK
	},
	{		/* GPU */
		(uintptr_t)CPG_CLKON_GPU,
		(uintptr_t)CPG_CLKMON_GPU,
		0x00000000,
		CPG_T_CLK
	},
	{		/* Image Scaling Unit */
		(uintptr_t)CPG_CLKON_ISU,
		(uintptr_t)CPG_CLKMON_ISU,
		0x00000000,
		CPG_T_CLK
	},
	{		/* H.264 codec */
		(uintptr_t)CPG_CLKON_H264,
		(uintptr_t)CPG_CLKMON_H264,
		0x00000000,
		CPG_T_CLK
	},
	{		/* Camera Data Receive Unit */
		(uintptr_t)CPG_CLKON_CRU,
		(uintptr_t)CPG_CLKMON_CRU,
		0x00000000,
		CPG_T_CLK
	},
	{		/* MIPI-DSI */
		(uintptr_t)CPG_CLKON_MIPI_DSI,
		(uintptr_t)CPG_CLKMON_MIPI_DSI,
		0x00000000,
		CPG_T_CLK
	},
	{		/* LCDC */
		(uintptr_t)CPG_CLKON_LCDC,
		(uintptr_t)CPG_CLKMON_LCDC,
		0x00000000,
		CPG_T_CLK
	},
	{		/* Serial Sound Interface */
		(uintptr_t)CPG_CLKON_SSI,
		(uintptr_t)CPG_CLKMON_SSI,
		0x00000000,
		CPG_T_CLK
	},
	{		/* Sampling Rate Converter */
		(uintptr_t)CPG_CLKON_SRC,
		(uintptr_t)CPG_CLKMON_SRC,
		0x00000000,
		CPG_T_CLK
	},
	{		/* USB2.0 */
		(uintptr_t)CPG_CLKON_USB,
		(uintptr_t)CPG_CLKMON_USB,
		0x00000000,
		CPG_T_CLK
	},
	{		/* ETHER */
		(uintptr_t)CPG_CLKON_ETH,
		(uintptr_t)CPG_CLKMON_ETH,
		0x00000000,
		CPG_T_CLK
	},
	{		/* I2C */
		(uintptr_t)CPG_CLKON_I2C,
		(uintptr_t)CPG_CLKMON_I2C,
		0x00000000,
		CPG_T_CLK
	},
	{		/* SCIF */
		(uintptr_t)CPG_CLKON_SCIF,
		(uintptr_t)CPG_CLKMON_SCIF,
		0x00010001,
		CPG_T_CLK
	},
	{		/* SCI */
		(uintptr_t)CPG_CLKON_SCI,
		(uintptr_t)CPG_CLKMON_SCI,
		0x00000000,
		CPG_T_CLK
	},
	{		/* IrDA */
		(uintptr_t)CPG_CLKON_IRDA,
		(uintptr_t)CPG_CLKMON_IRDA,
		0x00000000,
		CPG_T_CLK
	},
	{		/* SPI */
		(uintptr_t)CPG_CLKON_RSPI,
		(uintptr_t)CPG_CLKMON_RSPI,
		0x00000000,
		CPG_T_CLK
	},
	{		/* CAN */
		(uintptr_t)CPG_CLKON_CANFD,
		(uintptr_t)CPG_CLKMON_CANFD,
		0x00000000,
		CPG_T_CLK
	},
	{		/* GPIO */
		(uintptr_t)CPG_CLKON_GPIO,
		(uintptr_t)CPG_CLKMON_GPIO,
		0x00010001,
		CPG_T_CLK
	},
	{		/* TSIPG */
		(uintptr_t)CPG_CLKON_TSIPG,
		(uintptr_t)CPG_CLKMON_TSIPG,
		0x00000000,
		CPG_T_CLK
	},
	{		/* JAUTH */
		(uintptr_t)CPG_CLKON_JAUTH,
		(uintptr_t)CPG_CLKMON_JAUTH,
		0x00010001,
		CPG_T_CLK
	},
	{		/* OTP */
		(uintptr_t)CPG_CLKON_OTP,
		(uintptr_t)CPG_CLKMON_OTP,
		0x00030003,
		CPG_T_CLK
	},
	{		/* ADC */
		(uintptr_t)CPG_CLKON_ADC,
		(uintptr_t)CPG_CLKMON_ADC,
		0x00000000,
		CPG_T_CLK
	},
	{		/* Thermal Sensor Unit */
		(uintptr_t)CPG_CLKON_TSU,
		(uintptr_t)CPG_CLKMON_TSU,
		0x00000000,
		CPG_T_CLK
	},
	{		/* BBGU */
		(uintptr_t)CPG_CLKON_BBGU,
		(uintptr_t)CPG_CLKMON_BBGU,
		0x00010001,
		CPG_T_CLK
	},
	{		/* AXI_ACPU_BUS */
		(uintptr_t)CPG_CLKON_AXI_ACPU_BUS,
		(uintptr_t)CPG_CLKMON_AXI_ACPU_BUS,
		0x000f000f,
		CPG_T_CLK
	},
	{		/* AXI_MCPU_BUS */
		(uintptr_t)CPG_CLKON_AXI_MCPU_BUS,
		(uintptr_t)CPG_CLKMON_AXI_MCPU_BUS,
		0x07ff07ff,
		CPG_T_CLK
	},
	{		/* AXI_COM_BUS */
		(uintptr_t)CPG_CLKON_AXI_COM_BUS,
		(uintptr_t)CPG_CLKMON_AXI_COM_BUS,
		0x00030003,
		CPG_T_CLK
	},
	{		/* AXI_VIDEO_BUS */
		(uintptr_t)CPG_CLKON_AXI_VIDEO_BUS,
		(uintptr_t)CPG_CLKMON_AXI_VIDEO_BUS,
		0x00030003,
		CPG_T_CLK
	},
	{		/* PERI_COM */
		(uintptr_t)CPG_CLKON_PERI_COM,
		(uintptr_t)CPG_CLKMON_PERI_COM,
		0x00030003,
		CPG_T_CLK
	},
	{		/* REG0_BUS */
		(uintptr_t)CPG_CLKON_REG0_BUS,
		(uintptr_t)CPG_CLKMON_REG0_BUS,
		0x000f000f,
		CPG_T_CLK
	},
	{		/* REG1_BUS */
		(uintptr_t)CPG_CLKON_REG1_BUS,
		(uintptr_t)CPG_CLKMON_REG1_BUS,
		0x00030003,
		CPG_T_CLK
	},
	{		/* PERI_CPU */
		(uintptr_t)CPG_CLKON_PERI_CPU,
		(uintptr_t)CPG_CLKMON_PERI_CPU,
		0x000f000f,
		CPG_T_CLK
	},
	{		/* PERI_VIDEO */
		(uintptr_t)CPG_CLKON_PERI_VIDEO,
		(uintptr_t)CPG_CLKMON_PERI_VIDEO,
		0x00070007,
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
		0x001f001f,
		CPG_T_CLK
	},
	{		/* MTGPGS */
		(uintptr_t)CPG_CLKON_MTGPGS,
		(uintptr_t)CPG_CLKMON_MTGPGS,
		0x00030003,
		CPG_T_CLK
	},
	{		/* AXI_DEFAULT_SLV */
		(uintptr_t)CPG_CLKON_AXI_DEFAULT_SLV,
		(uintptr_t)CPG_CLKMON_AXI_DEFAULT_SLV,
		0x00010001,
		CPG_T_CLK
	}
};

static CPG_SETUP_DATA cpg_reset_tbl[] = {
	{		/* CM33 */
		(uintptr_t)CPG_RST_CM33,
		(uintptr_t)CPG_RSTMON_CM33,
		0x00000000,
		CPG_T_RST
	},
	{		/* ROM */
		(uintptr_t)CPG_RST_ROM,
		(uintptr_t)CPG_RSTMON_ROM,
		0x00010001,
		CPG_T_RST 
	},
	{		/* GIC600 */
		(uintptr_t)CPG_RST_GIC600,
		(uintptr_t)CPG_RSTMON_GIC600,
		0x00030003,
		CPG_T_RST
	},
	{		/* IA55 */
		(uintptr_t)CPG_RST_IA55,
		(uintptr_t)CPG_RSTMON_IA55,
		0x00010001,
		CPG_T_RST
	},
	{		/* IM33 */
		(uintptr_t)CPG_RST_IM33,
		(uintptr_t)CPG_RSTMON_IM33,
		0x00010001,
		CPG_T_RST
	},
	{		/* MHU */
		(uintptr_t)CPG_RST_MHU,
		(uintptr_t)CPG_RSTMON_MHU,
		0x00000000,
		CPG_T_RST
	},
	{		/* DMAC */
		(uintptr_t)CPG_RST_DMAC,
		(uintptr_t)CPG_RSTMON_DMAC,
		0x00010001,
		CPG_T_RST
	},
	{		/* SYSC */
		(uintptr_t)CPG_RST_SYSC,
		(uintptr_t)CPG_RSTMON_SYSC,
		0x00070007,
		CPG_T_RST
	},
	{		/* OSTM */
		(uintptr_t)CPG_RST_OSTM,
		(uintptr_t)CPG_RSTMON_OSTM,
		0x00010001,
		CPG_T_RST
	},
	{		/* MTU */
		(uintptr_t)CPG_RST_MTU,
		(uintptr_t)CPG_RSTMON_MTU,
		0x00000000,
		CPG_T_RST
	},
	{		/* POE3 */
		(uintptr_t)CPG_RST_POE3,
		(uintptr_t)CPG_RSTMON_POE3,
		0x00000000,
		CPG_T_RST
	},
	{		/* GPT */
		(uintptr_t)CPG_RST_GPT,
		(uintptr_t)CPG_RSTMON_GPT,
		0x00000000,
		CPG_T_RST
	},
	{		/* POEG */
		(uintptr_t)CPG_RST_POEG,
		(uintptr_t)CPG_RSTMON_POEG,
		0x00000000,
		CPG_T_RST
	},
	{		/* WDT */
		(uintptr_t)CPG_RST_WDT,
		(uintptr_t)CPG_RSTMON_WDT,
		0x00090009,
		CPG_T_RST
	},
#if !DEBUG_RZG2L_FPGA
	{		/* DDR */
		(uintptr_t)CPG_RST_DDR,
		(uintptr_t)CPG_RSTMON_DDR,
		0x00000000,
		CPG_T_RST
	},
#else
	{		/* DDR */
		(uintptr_t)CPG_RST_DDR,
		(uintptr_t)CPG_RSTMON_DDR,
		0x007F007F,
		CPG_T_RST
	},
#endif
	{		/* SPI_MULTI */
		(uintptr_t)CPG_RST_SPI,
		(uintptr_t)CPG_RSTMON_SPI,
		0x00010001,
		CPG_T_RST
	},
	{		/* SDHI */
		(uintptr_t)CPG_RST_SDHI,
		(uintptr_t)CPG_RSTMON_SDHI,
		0x00030003,
		CPG_T_RST
	},
	{		/* GPU */
		(uintptr_t)CPG_RST_GPU,
		(uintptr_t)CPG_RSTMON_GPU,
		0x00000000,
		CPG_T_RST
	},
	{		/* Image Scaling Unit */
		(uintptr_t)CPG_RST_ISU,
		(uintptr_t)CPG_RSTMON_ISU,
		0x00000000,
		CPG_T_RST
	},
	{		/* H.264 codec */
		(uintptr_t)CPG_RST_H264,
		(uintptr_t)CPG_RSTMON_H264,
		0x00000000,
		CPG_T_RST
	},
	{		/* Camera Data Receive Unit */
		(uintptr_t)CPG_RST_CRU,
		(uintptr_t)CPG_RSTMON_CRU,
		0x00000000,
		CPG_T_RST
	},
	{		/* MIPI-DSI */
		(uintptr_t)CPG_RST_MIPI_DSI,
		(uintptr_t)CPG_RSTMON_MIPI_DSI,
		0x00000000,
		CPG_T_RST
	},
	{		/* LCDC */
		(uintptr_t)CPG_RST_LCDC,
		(uintptr_t)CPG_RSTMON_LCDC,
		0x00000000,
		CPG_T_RST
	},
	{		/* Serial Sound Interface */
		(uintptr_t)CPG_RST_SSIF,
		(uintptr_t)CPG_RSTMON_SSIF,
		0x00000000,
		CPG_T_RST
	},
	{		/* Sampling Rate Converter */
		(uintptr_t)CPG_RST_SRC,
		(uintptr_t)CPG_RSTMON_SRC,
		0x00000000,
		CPG_T_RST
	},
	{		/* USB2.0 */
		(uintptr_t)CPG_RST_USB,
		(uintptr_t)CPG_RSTMON_USB,
		0x00000000,
		CPG_T_RST
	},
	{		/* ETHER */
		(uintptr_t)CPG_RST_ETH,
		(uintptr_t)CPG_RSTMON_ETH,
		0x00000000, 
		CPG_T_RST
	},
	{		/* I2C */
		(uintptr_t)CPG_RST_I2C,
		(uintptr_t)CPG_RSTMON_I2C,
		0x00000000,
		CPG_T_RST
	},
	{		/* SCIF */
		(uintptr_t)CPG_RST_SCIF,
		(uintptr_t)CPG_RSTMON_SCIF,
		0x00010001,
		CPG_T_RST
	},
	{		/* SCI */
		(uintptr_t)CPG_RST_SCI,
		(uintptr_t)CPG_RSTMON_SCI,
		0x00000000,
		CPG_T_RST
	},
	{		/* IrDA */
		(uintptr_t)CPG_RST_IRDA,
		(uintptr_t)CPG_RSTMON_IRDA,
		0x00000000,
		CPG_T_RST
	},
	{		/* SPI */
		(uintptr_t)CPG_RST_RSPI,
		(uintptr_t)CPG_RSTMON_RSPI,
		0x00000000,
		CPG_T_RST
	},
	{		/* CAN */
		(uintptr_t)CPG_RST_CANFD, 
		(uintptr_t)CPG_RSTMON_CANFD,
		0x00000000,
		CPG_T_RST
	},
	{		/* GPIO */
		(uintptr_t)CPG_RST_GPIO,
		(uintptr_t)CPG_RSTMON_GPIO,
		0x00070007,
		CPG_T_RST
	},
	{		/* TSIPG */
		(uintptr_t)CPG_RST_TSIPG,
		(uintptr_t)CPG_RSTMON_TSIPG,
		0x00000000,
		CPG_T_RST
	},
	{		/* JAUTH */
		(uintptr_t)CPG_RST_JAUTH,
		(uintptr_t)CPG_RSTMON_JAUTH,
		0x00010001,
		CPG_T_RST
	},
	{		/* OTP */
		(uintptr_t)CPG_RST_OTP,
		(uintptr_t)CPG_RSTMON_OTP,
		0x00010001,
		CPG_T_RST
	},
	{		/* ADC */
		(uintptr_t)CPG_RST_ADC,
		(uintptr_t)CPG_RSTMON_ADC,
		0x00000000,
		CPG_T_RST
	},
	{		/* Thermal Sensor Unit */
		(uintptr_t)CPG_RST_TSU,
		(uintptr_t)CPG_RSTMON_TSU,
		0x00000000,
		CPG_T_RST
	},
	{		/* BBGU */
		(uintptr_t)CPG_RST_BBGU,
		(uintptr_t)CPG_RSTMON_BBGU,
		0x00000000,
		CPG_T_RST
	},
	{		/* AXI_ACPU_BUS */
		(uintptr_t)CPG_RST_AXI_ACPU_BUS,
		(uintptr_t)CPG_RSTMON_AXI_ACPU_BUS,
		0x00010001,
		CPG_T_RST
	},
	{		/* AXI_MCPU_BUS */
		(uintptr_t)CPG_RST_AXI_MCPU_BUS,
		(uintptr_t)CPG_RSTMON_AXI_MCPU_BUS,
		0x00010001,
		CPG_T_RST
	},
	{		/* AXI_COM_BUS */
		(uintptr_t)CPG_RST_AXI_COM_BUS,
		(uintptr_t)CPG_RSTMON_AXI_COM_BUS,
		0x00010001,
		CPG_T_RST
	},
	{		/* AXI_VIDEO_BUS */
		(uintptr_t)CPG_RST_AXI_VIDEO_BUS,
		(uintptr_t)CPG_RSTMON_AXI_VIDEO_BUS,
		0x00010001,
		CPG_T_RST
	},
	{		/* PERI_COM */
		(uintptr_t)CPG_RST_PERI_COM,
		(uintptr_t)CPG_RSTMON_PERI_COM,
		0x00010001,
		CPG_T_RST
	},
	{		/* REG1_BUS */
		(uintptr_t)CPG_RST_REG1_BUS,
		(uintptr_t)CPG_RSTMON_REG1_BUS,
		0x00010001,
		CPG_T_RST
	},
	{		/* REG0_BUS */
		(uintptr_t)CPG_RST_REG0_BUS,
		(uintptr_t)CPG_RSTMON_REG0_BUS,
		0x00010001,
		CPG_T_RST
	},
	{		/* PERI_CPU */
		(uintptr_t)CPG_RST_PERI_CPU,
		(uintptr_t)CPG_RSTMON_PERI_CPU,
		0x00010001,
		CPG_T_RST
	},
	{		/* PERI_VIDEO */
		(uintptr_t)CPG_RST_PERI_VIDEO,
		(uintptr_t)CPG_RSTMON_PERI_VIDEO,
		0x00010001,
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
		0x001f001f,
		CPG_T_RST
	},
	{		/* MTGPGS */
		(uintptr_t)CPG_RST_MTGPGS,
		(uintptr_t)CPG_RSTMON_MTGPGS,
		0x00030003,
		CPG_T_RST
	},
	{		/* AXI_DEFAULT_SLV */
		(uintptr_t)CPG_RST_AXI_DEFAULT_SLV,
		(uintptr_t)CPG_RSTMON_AXI_DEFAULT_SLV,
		0x00010001,
		CPG_T_RST
	},
};

static CPG_REG_SETTING cpg_select_tbl[] = {
	{ (uintptr_t)CPG_PL1_DDIV,              0x00010000 },
	{ (uintptr_t)CPG_PL2_DDIV,              0x11110000 },
	{ (uintptr_t)CPG_PL3A_DDIV,             0x01110100 },
	{ (uintptr_t)CPG_PL3B_DDIV,             0x00010000 },
	{ (uintptr_t)CPG_PL5_SDIV,              0x01010000 },
	{ (uintptr_t)CPG_PL6_DDIV,              0x00010000 },
	{ (uintptr_t)CPG_PL2SDHI_DSEL,          0x00110022 },
	{ (uintptr_t)CPG_PL4_DSEL,              0x00010001 },
	{ (uintptr_t)CPG_PL3_SSEL,              0x01000000 },
	{ (uintptr_t)CPG_PL6_SSEL,              0x10000000 },
	{ (uintptr_t)CPG_PL6_ETH_SSEL,          0x00010000 },
	{ (uintptr_t)CPG_OTHERFUNC1_REG,        0x00010000 }
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
	{(uintptr_t)CPG_CLKON_CA55, 0x00000001 }
};

static CPG_REG_SETTING cpg_sel_pll2_1_on_off[] = {
	{(uintptr_t)CPG_CLKON_ADC, 0x00000001 },
	{(uintptr_t)CPG_CLKON_TSU, 0x00000001 },
	{(uintptr_t)CPG_CLKON_SDHI, 0x00000077 }
};

static CPG_REG_SETTING cpg_sel_pll2_2_on_off[] = {
	{(uintptr_t)CPG_CLKON_SDHI, 0x00000077 },
	{(uintptr_t)CPG_CLKON_MIPI_DSI, 0x00000020 },
};

static CPG_REG_SETTING cpg_sel_pll3_1_on_off[] = {
	{(uintptr_t)CPG_CLKON_AXI_ACPU_BUS, 0x0000000F },
	{(uintptr_t)CPG_CLKON_AXI_COM_BUS, 0x00000003 },
	{(uintptr_t)CPG_CLKON_AXI_DEFAULT_SLV, 0x00000001 },
	{(uintptr_t)CPG_CLKON_AXI_MCPU_BUS, 0x00000193 },
	{(uintptr_t)CPG_CLKON_AXI_TZCDDR, 0x0000001F },
	{(uintptr_t)CPG_CLKON_AXI_VIDEO_BUS, 0x00000003 },
	{(uintptr_t)CPG_CLKON_CA55, 0x0000001E },
	{(uintptr_t)CPG_CLKON_CM33, 0x00000001 },
	{(uintptr_t)CPG_CLKON_CRU, 0x0000000C },
	{(uintptr_t)CPG_CLKON_CST, 0x000007FD },
	{(uintptr_t)CPG_CLKON_DAMC_REG, 0x00000003 },
	{(uintptr_t)CPG_CLKON_DDR, 0x00000003 },
	{(uintptr_t)CPG_CLKON_ETH, 0x00000003 },
	{(uintptr_t)CPG_CLKON_GIC600, 0x00000001 },
	{(uintptr_t)CPG_CLKON_GPU, 0x00000007 },
	{(uintptr_t)CPG_CLKON_H264, 0x00000001 },
	{(uintptr_t)CPG_CLKON_IA55, 0x00000003 },
	{(uintptr_t)CPG_CLKON_IM33, 0x00000003 },
	{(uintptr_t)CPG_CLKON_ISU, 0x00000003 },
	{(uintptr_t)CPG_CLKON_JAUTH, 0x00000001 },
	{(uintptr_t)CPG_CLKON_LCDC, 0x00000001 },
	{(uintptr_t)CPG_CLKON_MIPI_DSI, 0x0000000C },
	{(uintptr_t)CPG_CLKON_OTP, 0x00000002 },
	{(uintptr_t)CPG_CLKON_PERI_COM, 0x00000003 },
	{(uintptr_t)CPG_CLKON_PERI_CPU, 0x0000000D },
	{(uintptr_t)CPG_CLKON_PERI_DDR, 0x00000001 },
	{(uintptr_t)CPG_CLKON_PERI_VIDEO, 0x00000007 },
	{(uintptr_t)CPG_CLKON_REG0_BUS, 0x00000001 },
	{(uintptr_t)CPG_CLKON_REG1_BUS, 0x00000003 },
	{(uintptr_t)CPG_CLKON_ROM, 0x00000001 },
	{(uintptr_t)CPG_CLKON_SDHI, 0x00000088 },
	{(uintptr_t)CPG_CLKON_SRAM_ACPU, 0x00000001 },
	{(uintptr_t)CPG_CLKON_SRAM_MCPU, 0x00000001 },
	{(uintptr_t)CPG_CLKON_SYSC, 0x00000002 },
	{(uintptr_t)CPG_CLKON_TSIPG, 0x00000003 },
	{(uintptr_t)CPG_CLKON_USB, 0x0000000F }
};

static CPG_REG_SETTING cpg_sel_pll3_2_on_off[] = {
	{(uintptr_t)CPG_CLKON_CRU, 0x00000003 },
	{(uintptr_t)CPG_CLKON_MIPI_DSI, 0x00000002 },
	{(uintptr_t)CPG_CLKON_GPU, 0x00000001 },
	{(uintptr_t)CPG_CLKON_SPI_MULTI, 0x00000003 },
	{(uintptr_t)CPG_CLKON_AXI_MCPU_BUS, 0x00000208 },
};

static CPG_REG_SETTING cpg_sel_pll3_3_on_off[] = {
	{(uintptr_t)CPG_CLKON_SPI_MULTI, 0x00000003 },
	{(uintptr_t)CPG_CLKON_AXI_MCPU_BUS, 0x00000208 },
};

static CPG_REG_SETTING cpg_sel_pll5_1_on_off[] = {
	{(uintptr_t)CPG_CLKON_MIPI_DSI, 0x00000001 },
	{(uintptr_t)CPG_CLKON_CRU, 0x00000010 },
	{(uintptr_t)CPG_CLKON_MIPI_DSI, 0x00000010 },
	{(uintptr_t)CPG_CLKON_LCDC, 0x00000002 }
};

static CPG_REG_SETTING cpg_sel_pll5_3_on_off[] = {
	{(uintptr_t)CPG_CLKON_MIPI_DSI, 0x00000010 },
	{(uintptr_t)CPG_CLKON_LCDC, 0x00000002 }
};

static CPG_REG_SETTING cpg_sel_pll5_4_on_off[] = {
	{(uintptr_t)CPG_CLKON_MIPI_DSI, 0x00000010 },
	{(uintptr_t)CPG_CLKON_LCDC, 0x00000002 }
};

static CPG_REG_SETTING cpg_sel_pll6_1_on_off[] = {
	{(uintptr_t)CPG_CLKON_GPU, 0x00000001 }
};

static CPG_REG_SETTING cpg_sel_gpu1_1_on_off[] = {
	{(uintptr_t)CPG_CLKON_GPU, 0x00000001 }
};

static CPG_REG_SETTING cpg_sel_gpu1_2_on_off[] = {
	{(uintptr_t)CPG_CLKON_GPU, 0x00000001 }
};

static CPG_REG_SETTING cpg_sel_gpu2_on_off[] = {
	{(uintptr_t)CPG_CLKON_GPU, 0x00000001 }
};


static void cpg_ctrl_clkrst(CPG_SETUP_DATA const *array, uint32_t num)
{
	int i;
	uint32_t mask;
	uint32_t cmp;

	for (i = 0; i < num; i++, array++) {
		mmio_write_32(array->reg, array->val);

		mask = (array->val >> 16) & 0xFFFF;
		cmp = array->val & 0xFFFF;
		if (array->type == CPG_T_RST) {
			cmp = ~(cmp);
		}
		while ((mmio_read_32(array->mon) & mask) != (cmp & mask))
			;
	}
}

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
			mmio_write_32(ptr[cnt].reg, (mmio_read_32(ptr[cnt].reg) | ptr[cnt].val));
		} else {
			mmio_write_32(ptr[cnt].reg, (mmio_read_32(ptr[cnt].reg) & ~ptr[cnt].val));
		}
	}

}

static void cpg_pll_start_146(uint32_t index)
{
	mmio_write_32(cpg_pll_setdata_146[index].clk1_dat.reg, cpg_pll_setdata_146[index].clk1_dat.val);
	mmio_write_32(cpg_pll_setdata_146[index].clk2_dat.reg, cpg_pll_setdata_146[index].clk2_dat.val);
	mmio_write_32(cpg_pll_setdata_146[index].stby_dat.reg, cpg_pll_setdata_146[index].stby_dat.val);
}

static void cpg_pll_start_235(uint32_t index)
{
	mmio_write_32(cpg_pll_setdata_235[index].clk1_dat.reg, cpg_pll_setdata_235[index].clk1_dat.val);
	mmio_write_32(cpg_pll_setdata_235[index].clk2_dat.reg, cpg_pll_setdata_235[index].clk2_dat.val);
	mmio_write_32(cpg_pll_setdata_235[index].clk3_dat.reg, cpg_pll_setdata_235[index].clk3_dat.val);
	mmio_write_32(cpg_pll_setdata_235[index].clk4_dat.reg, cpg_pll_setdata_235[index].clk4_dat.val);
	mmio_write_32(cpg_pll_setdata_235[index].clk5_dat.reg, cpg_pll_setdata_235[index].clk5_dat.val);
	mmio_write_32(cpg_pll_setdata_235[index].stby_dat.reg, cpg_pll_setdata_235[index].stby_dat.val);
}

/* It is assumed that the PLL has stopped by the time this function is executed. */
static void cpg_pll_setup(void)
{
#if !DEBUG_RZG2L_FPGA
	uint32_t val = 0;
#endif

	/* PLL4 startup */
	/* PLL4 standby mode transition confirmation */
#if !DEBUG_RZG2L_FPGA
	do {
		val = mmio_read_32(CPG_PLL4_MON);
	} while ((val & (PLL4_MON_PLL4_RESETB | PLL4_MON_PLL4_LOCK)) != 0);
#endif

	/* Set PLL4 to normal mode */
	/* mmio_write_32(CPG_PLL4_STBY, (PLL4_STBY_RESETB_WEN | PLL4_STBY_RESETB)); */
	cpg_pll_start_146(CPG_PLL4_INDEX);

	/* PLL4 normal mode transition confirmation */
#if !DEBUG_RZG2L_FPGA
	do {
		val = mmio_read_32(CPG_PLL4_MON);
	} while ((val & (PLL4_MON_PLL4_RESETB | PLL4_MON_PLL4_LOCK)) == 0);
#endif

	/* PLL5 startup */
	/* PLL5 standby mode transition confirmation */
#if !DEBUG_RZG2L_FPGA
	do {
		val = mmio_read_32(CPG_PLL5_MON);
	} while ((val & (PLL5_MON_PLL5_RESETB | PLL5_MON_PLL5_LOCK)) != 0);
#endif

	/* PLL5 set to normal mode */
	/* val = mmio_read_32(CPG_PLL5_STBY) | PLL5_STBY_RESETB_WEN | PLL5_STBY_RESETB; */
	/* mmio_write_32(CPG_PLL5_STBY, val); */
	cpg_pll_start_235(CPG_PLL5_INDEX);

	/* PLL5 Normal mode transition confirmation */
#if !DEBUG_RZG2L_FPGA
	do {
		val = mmio_read_32(CPG_PLL5_MON);
	} while ((val & (PLL5_MON_PLL5_RESETB | PLL5_MON_PLL5_LOCK)) == 0);
#endif

	/* PLL6 startup */
	/* PLL6 standby mode transition confirmation */
#if !DEBUG_RZG2L_FPGA
	do {
		val = mmio_read_32(CPG_PLL6_MON);
	} while ((val & (PLL6_MON_PLL6_RESETB | PLL6_MON_PLL6_LOCK)) != 0);
#endif

	/* Set PLL6 to normal mode */
	/* val = mmio_read_32(CPG_PLL6_STBY) | PLL6_STBY_RESETB_WEN | PLL6_STBY_RESETB; */
	/* mmio_write_32(CPG_PLL6_STBY, val); */
	cpg_pll_start_146(CPG_PLL6_INDEX);

	/* PLL6 Normal mode transition confirmation */
#if !DEBUG_RZG2L_FPGA
	do {
		val = mmio_read_32(CPG_PLL6_MON);
	} while ((val & (PLL6_MON_PLL6_RESETB | PLL6_MON_PLL6_LOCK)) == 0);
#endif
}

static void cpg_div_sel_setup(void)
{
	int cnt;

	for (cnt = 0; cnt < ARRAY_SIZE(cpg_select_tbl); cnt++) {
		mmio_write_32(cpg_select_tbl[cnt].reg, cpg_select_tbl[cnt].val);
	}

#if !DEBUG_RZG2L_FPGA
	/* Wait for completion of settings */
	while (mmio_read_32(CPG_CLKSTATUS) != 0)
		;
#endif
}

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
	/* Assert the reset of DDRTOP */
	mmio_write_32(CPG_RST_DDR, 0x007F0000);
	mmio_write_32(CPG_OTHERFUNC2_REG, 0x00010000);

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
	while ((mmio_read_32(CPG_RSTMON_DDR) & 0x00000002) != 0x00000002)
		;

	udelay(1);

	disable_phy();

	/* De-assert axiY_ARESETn, regARESETn, reset_n */
	mmio_write_32(CPG_RST_DDR, 0x007D007D);
	while ((mmio_read_32(CPG_RSTMON_DDR) & 0x0000007D) != 0x0000007D)
		;

	udelay(1);
}

void cpg_early_setup(void)
{
	cpg_ctrl_clkrst(&early_setup_tbl[0], ARRAY_SIZE(early_setup_tbl));
}

void cpg_setup(void)
{
	cpg_selector_on_off(CPG_SEL_PLL3_1_ON_OFF, CPG_OFF);
	cpg_selector_on_off(CPG_SEL_PLL3_2_ON_OFF, CPG_OFF);
	cpg_selector_on_off(CPG_SEL_PLL3_3_ON_OFF, CPG_OFF);
	cpg_div_sel_setup();
	cpg_selector_on_off(CPG_SEL_PLL3_1_ON_OFF, CPG_ON);
	cpg_selector_on_off(CPG_SEL_PLL3_2_ON_OFF, CPG_ON);
	cpg_selector_on_off(CPG_SEL_PLL3_3_ON_OFF, CPG_ON);
	cpg_pll_setup();
	cpg_clk_on_setup();
	cpg_reset_setup();
}
