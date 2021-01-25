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

static CPG_PLL_SETDATA_146 cpg_pll4_setdata = {
#if (BOARD_TYPE == BOARD_RZG2L_15MMSQ)
	{ CPG_PLL4_CLK1, 0x00003203 },
#elif (BOARD_TYPE == BOARD_RZG2LC_EVA)
	{ CPG_PLL4_CLK1, 0x40005346 },
#endif
	{ CPG_PLL4_CLK2, 0x00082400 },
	{ CPG_PLL4_STBY, 0x00010001 }
};

#define	CPG_PLL2_INDEX					(0)
#define	CPG_PLL3_INDEX					(1)
#define	CPG_PLL5_INDEX					(2)

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
		0x00030000,
		CPG_T_CLK
	},
	{		/* MHU */
		(uintptr_t)CPG_CLKON_MHU,
		(uintptr_t)CPG_CLKMON_MHU,
		0x00010000,
		CPG_T_CLK
	},
	{		/* DMAC */
		(uintptr_t)CPG_CLKON_DAMC_REG,
		(uintptr_t)CPG_CLKMON_DAMC_REG,
		0x00030003,
		CPG_T_CLK
	},
	{		/* OSTM */
		(uintptr_t)CPG_CLKON_OSTM,
		(uintptr_t)CPG_CLKMON_OSTM,
		0x00060000,
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
		0x003C0000,
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
		0x00030000,
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
		0x00070000,
		CPG_T_CLK
	},
	{		/* Image Scaling Unit */
		(uintptr_t)CPG_CLKON_ISU,
		(uintptr_t)CPG_CLKMON_ISU,
		0x00030000,
		CPG_T_CLK
	},
	{		/* H.264 codec */
		(uintptr_t)CPG_CLKON_H264,
		(uintptr_t)CPG_CLKMON_H264,
		0x00010000,
		CPG_T_CLK
	},
	{		/* Camera Data Receive Unit */
		(uintptr_t)CPG_CLKON_CRU,
		(uintptr_t)CPG_CLKMON_CRU,
		0x001f0000,
		CPG_T_CLK
	},
	{		/* MIPI-DSI */
		(uintptr_t)CPG_CLKON_MIPI_DSI,
		(uintptr_t)CPG_CLKMON_MIPI_DSI,
		0x003f0000,
		CPG_T_CLK
	},
	{		/* LCDC */
		(uintptr_t)CPG_CLKON_LCDC,
		(uintptr_t)CPG_CLKMON_LCDC,
		0x00030000,
		CPG_T_CLK
	},
	{		/* Serial Sound Interface */
		(uintptr_t)CPG_CLKON_SSI,
		(uintptr_t)CPG_CLKMON_SSI,
		0x00ff0000,
		CPG_T_CLK
	},
	{		/* Sampling Rate Converter */
		(uintptr_t)CPG_CLKON_SRC,
		(uintptr_t)CPG_CLKMON_SRC,
		0x00010000,
		CPG_T_CLK
	},
	{		/* USB2.0 */
		(uintptr_t)CPG_CLKON_USB,
		(uintptr_t)CPG_CLKMON_USB,
		0x000f0000,
		CPG_T_CLK
	},
	{		/* ETHER */
		(uintptr_t)CPG_CLKON_ETH,
		(uintptr_t)CPG_CLKMON_ETH,
		0x00030000,
		CPG_T_CLK
	},
	{		/* I2C */
		(uintptr_t)CPG_CLKON_I2C,
		(uintptr_t)CPG_CLKMON_I2C,
		0x000f0000,
		CPG_T_CLK
	},
	{		/* SCIF */
		(uintptr_t)CPG_CLKON_SCIF,
		(uintptr_t)CPG_CLKMON_SCIF,
		0x001f0001,
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
	{		/* SPI */
		(uintptr_t)CPG_CLKON_RSPI,
		(uintptr_t)CPG_CLKMON_RSPI,
		0x00070000,
		CPG_T_CLK
	},
	{		/* CAN */
		(uintptr_t)CPG_CLKON_CANFD,
		(uintptr_t)CPG_CLKMON_CANFD,
		0x00010000,
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
	{		/* Thermal Sensor Unit */
		(uintptr_t)CPG_CLKON_TSU,
		(uintptr_t)CPG_CLKMON_TSU,
		0x00010000,
		CPG_T_CLK
	}
};

static CPG_SETUP_DATA cpg_reset_tbl[] = {
	{		/* CM33 */
		(uintptr_t)CPG_RST_CM33,
		(uintptr_t)CPG_RSTMON_CM33,
		0x00070000,
		CPG_T_RST
	},
	{		/* MHU */
		(uintptr_t)CPG_RST_MHU,
		(uintptr_t)CPG_RSTMON_MHU,
		0x00010000,
		CPG_T_RST
	},
	{		/* DMAC */
		(uintptr_t)CPG_RST_DMAC,
		(uintptr_t)CPG_RSTMON_DMAC,
		0x00030003,
		CPG_T_RST
	},
	{		/* OSTM */
		(uintptr_t)CPG_RST_OSTM,
		(uintptr_t)CPG_RSTMON_OSTM,
		0x00060000,
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
		0x000f0000,
		CPG_T_RST
	},
	{		/* WDT */
		(uintptr_t)CPG_RST_WDT,
		(uintptr_t)CPG_RSTMON_WDT,
		0x00060000,
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
		0x007F0000,
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
		0x00070000,
		CPG_T_RST
	},
	{		/* Image Scaling Unit */
		(uintptr_t)CPG_RST_ISU,
		(uintptr_t)CPG_RSTMON_ISU,
		0x00030000,
		CPG_T_RST
	},
	{		/* H.264 codec */
		(uintptr_t)CPG_RST_H264,
		(uintptr_t)CPG_RSTMON_H264,
		0x00030000,
		CPG_T_RST
	},
	{		/* Camera Data Receive Unit */
		(uintptr_t)CPG_RST_CRU,
		(uintptr_t)CPG_RSTMON_CRU,
		0x00070000,
		CPG_T_RST
	},
	{		/* MIPI-DSI */
		(uintptr_t)CPG_RST_MIPI_DSI,
		(uintptr_t)CPG_RSTMON_MIPI_DSI,
		0x00070000,
		CPG_T_RST
	},
	{		/* LCDC */
		(uintptr_t)CPG_RST_LCDC,
		(uintptr_t)CPG_RSTMON_LCDC,
		0x00010000,
		CPG_T_RST
	},
	{		/* Serial Sound Interface */
		(uintptr_t)CPG_RST_SSIF,
		(uintptr_t)CPG_RSTMON_SSIF,
		0x000f0000,
		CPG_T_RST
	},
	{		/* Sampling Rate Converter */
		(uintptr_t)CPG_RST_SRC,
		(uintptr_t)CPG_RSTMON_SRC,
		0x00010000,
		CPG_T_RST
	},
	{		/* USB2.0 */
		(uintptr_t)CPG_RST_USB,
		(uintptr_t)CPG_RSTMON_USB,
		0x000f0000,
		CPG_T_RST
	},
	{		/* ETHER */
		(uintptr_t)CPG_RST_ETH,
		(uintptr_t)CPG_RSTMON_ETH,
		0x00030000,
		CPG_T_RST
	},
	{		/* I2C */
		(uintptr_t)CPG_RST_I2C,
		(uintptr_t)CPG_RSTMON_I2C,
		0x000f0000,
		CPG_T_RST
	},
	{		/* SCIF */
		(uintptr_t)CPG_RST_SCIF,
		(uintptr_t)CPG_RSTMON_SCIF,
		0x001f0001,
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
	{		/* SPI */
		(uintptr_t)CPG_RST_RSPI,
		(uintptr_t)CPG_RSTMON_RSPI,
		0x00070000,
		CPG_T_RST
	},
	{		/* CAN */
		(uintptr_t)CPG_RST_CANFD,
		(uintptr_t)CPG_RSTMON_CANFD,
		0x00030000,
		CPG_T_RST
	},
	{		/* GPIO */
		(uintptr_t)CPG_RST_GPIO,
		(uintptr_t)CPG_RSTMON_GPIO,
		0x00070007,
		CPG_T_RST
	},
	{		/* ADC */
		(uintptr_t)CPG_RST_ADC,
		(uintptr_t)CPG_RSTMON_ADC,
		0x00030000,
		CPG_T_RST
	},
	{		/* Thermal Sensor Unit */
		(uintptr_t)CPG_RST_TSU,
		(uintptr_t)CPG_RSTMON_TSU,
		0x00010000,
		CPG_T_RST
	},
};

static CPG_REG_SETTING cpg_static_select_tbl[] = {
	{ (uintptr_t)CPG_PL3A_DDIV,             0x01110100 },
};

static CPG_REG_SETTING cpg_dynamic_select_tbl[] = {
	{ (uintptr_t)CPG_PL4_DSEL,              0x00010001 },
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
	{(uintptr_t)CPG_CLKON_MIPI_DSI, 0x00200020 },
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
			mmio_write_32(ptr[cnt].reg, (mmio_read_32(ptr[cnt].reg) | (ptr[cnt].val & 0xFFFF0000)));
		}
	}

}

static void cpg_pll_start_146(CPG_PLL_SETDATA_146 *pdata)
{
	mmio_write_32(pdata->clk1_dat.reg, pdata->clk1_dat.val);
	mmio_write_32(pdata->clk2_dat.reg, pdata->clk2_dat.val);
	mmio_write_32(pdata->stby_dat.reg, pdata->stby_dat.val);
}

/* It is assumed that the PLL has stopped by the time this function is executed. */
static void cpg_pll_setup(void)
{
#if !DEBUG_RZG2L_FPGA
	uint32_t val = 0;

	/* PLL4 startup */
	/* PLL4 standby mode transition confirmation */
	do {
		val = mmio_read_32(CPG_PLL4_MON);
	} while ((val & (PLL4_MON_PLL4_RESETB | PLL4_MON_PLL4_LOCK)) != 0);
#endif

	/* Set PLL4 to normal mode */
	cpg_pll_start_146(&cpg_pll4_setdata);

#if !DEBUG_RZG2L_FPGA
	/* PLL4 normal mode transition confirmation */
	do {
		val = mmio_read_32(CPG_PLL4_MON);
	} while ((val & (PLL4_MON_PLL4_RESETB | PLL4_MON_PLL4_LOCK)) == 0);
#endif
}

static void cpg_div_sel_setup(CPG_REG_SETTING *tbl, uint32_t size)
{
	int cnt;

	for (cnt = 0; cnt < size; cnt++, tbl++) {
		mmio_write_32(tbl->reg, tbl->val);
	}

#if !DEBUG_RZG2L_FPGA
	/* Wait for completion of settings */
	while (mmio_read_32(CPG_CLKSTATUS) != 0)
		;
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
	while ((mmio_read_32(CPG_RSTMON_DDR) & 0x0000007F) != 0x0000007F)
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
	mmio_write_32(CPG_RST_DDR, 0x007D007D);
	while ((mmio_read_32(CPG_RSTMON_DDR) & 0x0000007D) != 0x00000000)
		;

	udelay(1);
}

void cpg_early_setup(void)
{
	cpg_ctrl_clkrst(&early_setup_tbl[0], ARRAY_SIZE(early_setup_tbl));
}

void cpg_setup(void)
{
	cpg_selector_on_off(CPG_SEL_PLL3_3_ON_OFF, CPG_OFF);
	cpg_div_sel_static_setup();
	cpg_selector_on_off(CPG_SEL_PLL3_3_ON_OFF, CPG_ON);
	cpg_pll_setup();
	cpg_clk_on_setup();
	cpg_reset_setup();
	cpg_div_sel_dynamic_setup();
}
