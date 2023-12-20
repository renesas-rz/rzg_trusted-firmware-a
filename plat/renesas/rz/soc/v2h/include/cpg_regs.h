/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CPG_REGS_H__
#define __CPG_REGS_H__

#include <rz_soc_def.h>				/* Get the CPG base address */


#define	CPG_PLLCM33_MON				(CPG_BASE + 0x010)	/* PLLCM33 monitor register */

#define	CPG_PLLCLN_STBY				(CPG_BASE + 0x020)	/* PLLCLN standby control register */
#define	CPG_PLLCLN_MON				(CPG_BASE + 0x030)	/* PLLCLN monitor register */

#define	CPG_PLLDTY_STBY				(CPG_BASE + 0x040)	/* PLLDTY standby control register */
#define	CPG_PLLDTY_MON				(CPG_BASE + 0x050)	/* PLLDTY monitor register */

#define	CPG_PLLCA55_STBY			(CPG_BASE + 0x060)	/* PLLCA55 standby control register */
#define	CPG_PLLCA55_CLK1			(CPG_BASE + 0x064)	/* PLLCA55 output clock setting register 1 */
#define	CPG_PLLCA55_CLK2			(CPG_BASE + 0x068)	/* PLLCA55 output clock setting register 2 */
#define	CPG_PLLCA55_MON				(CPG_BASE + 0x070)	/* PLLCA55 monitor register */

#define	CPG_PLLVDO_STBY				(CPG_BASE + 0x080)	/* PLLVDO standby control register */
#define	CPG_PLLVDO_MON				(CPG_BASE + 0x090)	/* PLLVDO monitor register */

#define	CPG_PLLETH_STBY				(CPG_BASE + 0x0A0)	/* PLLETH standby control register */
#define	CPG_PLLETH_MON				(CPG_BASE + 0x0B0)	/* PLLETH monitor register */

#define	CPG_PLLDSI_STBY				(CPG_BASE + 0x0C0)	/* PLLDSI standby control register */
#define	CPG_PLLDSI_CLK1				(CPG_BASE + 0x0C4)	/* PLLDSI output clock setting register 1 */
#define	CPG_PLLDSI_CLK2				(CPG_BASE + 0x0C8)	/* PLLDSI output clock setting register 2 */
#define	CPG_PLLDSI_MON				(CPG_BASE + 0x0D0)	/* PLLDSI monitor register */

#define	CPG_PLLDDR0_STBY			(CPG_BASE + 0x0E0)	/* PLLDDR0 standby control register */
#define	CPG_PLLDDR0_CLK1			(CPG_BASE + 0x0E4)	/* PLLDDR0 output clock setting register 1 */
#define	CPG_PLLDDR0_CLK2			(CPG_BASE + 0x0E8)	/* PLLDDR0 output clock setting register 2 */
#define	CPG_PLLDDR0_MON				(CPG_BASE + 0x0F0)	/* PLLDDR0 monitor register*/

#define	CPG_PLLDDR1_STBY			(CPG_BASE + 0x100)	/* PLLDDR1standby control register*/
#define	CPG_PLLDDR1_CLK1			(CPG_BASE + 0x104)	/* PLLDDR1output clock setting register 1 */
#define	CPG_PLLDDR1_CLK2			(CPG_BASE + 0x108)	/* PLLDDR1output clock setting register 2 */
#define	CPG_PLLDDR1_MON				(CPG_BASE + 0x110)	/* PLLDDR1monitor register */

#define	CPG_PLLGPU_STBY				(CPG_BASE + 0x120)	/* PLLGPU standby control register */
#define	CPG_PLLGPU_CLK1				(CPG_BASE + 0x124)	/* PLLGPU output clock setting register 1 */
#define	CPG_PLLGPU_CLK2				(CPG_BASE + 0x128)	/* PLLGPU output clock setting register 2 */
#define	CPG_PLLGPU_MON				(CPG_BASE + 0x130)	/* PLLGPU monitor register */

#define	CPG_PLLDRP_STBY				(CPG_BASE + 0x140)	/* PLLDRP standby control register */
#define	CPG_PLLDRP_CLK1				(CPG_BASE + 0x144)	/* PLLDRP output clock setting register 1 */
#define	CPG_PLLDRP_CLK2				(CPG_BASE + 0x148)	/* PLLDRP output clock setting register 2 */
#define	CPG_PLLDRP_MON				(CPG_BASE + 0x150)	/* PLLDRP monitor register */

#define	CCLMA0_CTL					(CPG_BASE + 0x200)	/* CLMA0 control register */
#define	CCLMA1_CTL					(CPG_BASE + 0x204)	/* CLMA1 control register */
#define	CCLMA2_CTL					(CPG_BASE + 0x208)	/* CLMA2 control register */
#define	CCLMA3_CTL					(CPG_BASE + 0x20C)	/* CLMA3 control register */
#define	CCLMA4_CTL					(CPG_BASE + 0x210)	/* CLMA4 control register */
#define	CCLMA5_CTL					(CPG_BASE + 0x214)	/* CLMA5 control register */
#define	CCLMA6_CTL					(CPG_BASE + 0x218)	/* CLMA6 control register */
#define	CCLMA7_CTL					(CPG_BASE + 0x21C)	/* CLMA7 control register */
#define	CCLMA8_CTL					(CPG_BASE + 0x220)	/* CLMA8 control register */
#define	CCLMA9_CTL					(CPG_BASE + 0x224)	/* CLMA9 control register */
#define	CCLMA10_CTL					(CPG_BASE + 0x228)	/* CLMA10 control register */
#define	CCLMA11_CTL					(CPG_BASE + 0x22C)	/* CLMA11 control register */
#define	CCLMA12_CTL					(CPG_BASE + 0x230)	/* CLMA12 control register */
#define	CCLMA13_CTL					(CPG_BASE + 0x234)	/* CLMA13 control register */
#define	CCLMA14_CTL					(CPG_BASE + 0x238)	/* CLMA14 control register */
#define	CCLMA_MON					(CPG_BASE + 0x280)	/* CLMA monitor register */
#define	CPG_SSEL0					(CPG_BASE + 0x300)	/* Static Mux control */
#define	CPG_SSEL1					(CPG_BASE + 0x304)	/* Static Mux control */
#define	CPG_SSEL2					(CPG_BASE + 0x308)	/* Static Mux control */
#define	CPG_CDDIV0					(CPG_BASE + 0x400)	/* Dynamic Gear control(Counter type) */
#define	CPG_CDDIV1					(CPG_BASE + 0x404)	/* Dynamic Gear control(Counter type) */
#define	CPG_CDDIV2					(CPG_BASE + 0x408)	/* Dynamic Gear control(Counter type) */
#define	CPG_CDDIV3					(CPG_BASE + 0x40c)	/* Dynamic Gear control(Counter type) */
#define	CPG_CDDIV4					(CPG_BASE + 0x410)	/* Dynamic Gear control(Counter type) */
#define	CPG_CSDIV0					(CPG_BASE + 0x500)	/* Static Gear control(Sparse type) */
#define	CPG_CSDIV1					(CPG_BASE + 0x504)	/* Static Gear control(Sparse type) */

#define	CPG_CLKON_0					(CPG_BASE + 0x600)	/* CGC control */
#define	CPG_CLKON_1					(CPG_BASE + 0x604)	/* CGC control */
#define	CPG_CLKON_2					(CPG_BASE + 0x608)	/* CGC control */
#define	CPG_CLKON_3					(CPG_BASE + 0x60c)	/* CGC control */
#define	CPG_CLKON_4					(CPG_BASE + 0x610)	/* CGC control */
#define	CPG_CLKON_5					(CPG_BASE + 0x614)	/* CGC control */
#define	CPG_CLKON_6					(CPG_BASE + 0x618)	/* CGC control */
#define	CPG_CLKON_7					(CPG_BASE + 0x61c)	/* CGC control */
#define	CPG_CLKON_8					(CPG_BASE + 0x620)	/* CGC control */
#define	CPG_CLKON_9					(CPG_BASE + 0x624)	/* CGC control */
#define	CPG_CLKON_10				(CPG_BASE + 0x628)	/* CGC control */
#define	CPG_CLKON_11				(CPG_BASE + 0x62c)	/* CGC control */
#define	CPG_CLKON_12				(CPG_BASE + 0x630)	/* CGC control */
#define	CPG_CLKON_13				(CPG_BASE + 0x634)	/* CGC control */
#define	CPG_CLKON_14				(CPG_BASE + 0x638)	/* CGC control */
#define	CPG_CLKON_15				(CPG_BASE + 0x63c)	/* CGC control */
#define	CPG_CLKON_16				(CPG_BASE + 0x640)	/* CGC control */
#define	CPG_CLKON_17				(CPG_BASE + 0x644)	/* CGC control */
#define	CPG_CLKON_18				(CPG_BASE + 0x648)	/* CGC control */
#define	CPG_CLKON_19				(CPG_BASE + 0x64c)	/* CGC control */
#define	CPG_CLKON_20				(CPG_BASE + 0x650)	/* CGC control */
#define	CPG_CLKON_21				(CPG_BASE + 0x654)	/* CGC control */
#define	CPG_CLKON_22				(CPG_BASE + 0x658)	/* CGC control */
#define	CPG_CLKON_23				(CPG_BASE + 0x65C)	/* CGC control */
#define	CPG_CLKON_24				(CPG_BASE + 0x660)	/* CGC control */

#define	CPG_CLKSTATUS0				(CPG_BASE + 0x700)	/* Dynamic gear/mux status monitor */
#define	CPG_CLKMON_0				(CPG_BASE + 0x800)	/* CGC monitor */
#define	CPG_CLKMON_1				(CPG_BASE + 0x804)	/* CGC monitor */
#define	CPG_CLKMON_2				(CPG_BASE + 0x808)	/* CGC monitor */
#define	CPG_CLKMON_3				(CPG_BASE + 0x80c)	/* CGC monitor */
#define	CPG_CLKMON_4				(CPG_BASE + 0x810)	/* CGC monitor */
#define	CPG_CLKMON_5				(CPG_BASE + 0x814)	/* CGC monitor */
#define	CPG_CLKMON_6				(CPG_BASE + 0x818)	/* CGC monitor */
#define	CPG_CLKMON_7				(CPG_BASE + 0x81c)	/* CGC monitor */
#define	CPG_CLKMON_8				(CPG_BASE + 0x820)	/* CGC monitor */
#define	CPG_CLKMON_9				(CPG_BASE + 0x824)	/* CGC monitor */
#define	CPG_CLKMON_10				(CPG_BASE + 0x828)	/* CGC monitor */
#define	CPG_RST_0					(CPG_BASE + 0x900)	/* RESET ON/OFF control */
#define	CPG_RST_1					(CPG_BASE + 0x904)	/* RESET ON/OFF control */
#define	CPG_RST_2					(CPG_BASE + 0x908)	/* RESET ON/OFF control */
#define	CPG_RST_3					(CPG_BASE + 0x90C)	/* RESET ON/OFF control */
#define	CPG_RST_4					(CPG_BASE + 0x910)	/* RESET ON/OFF control */
#define	CPG_RST_5					(CPG_BASE + 0x914)	/* RESET ON/OFF control */
#define	CPG_RST_6					(CPG_BASE + 0x918)	/* RESET ON/OFF control */
#define	CPG_RST_7					(CPG_BASE + 0x91C)	/* RESET ON/OFF control */
#define	CPG_RST_8					(CPG_BASE + 0x920)	/* RESET ON/OFF control */
#define	CPG_RST_9					(CPG_BASE + 0x924)	/* RESET ON/OFF control */
#define	CPG_RST_10					(CPG_BASE + 0x928)	/* RESET ON/OFF control */
#define	CPG_RST_11					(CPG_BASE + 0x92C)	/* RESET ON/OFF control */
#define	CPG_RST_12					(CPG_BASE + 0x930)	/* RESET ON/OFF control */
#define	CPG_RST_13					(CPG_BASE + 0x934)	/* RESET ON/OFF control */
#define	CPG_RST_14					(CPG_BASE + 0x938)	/* RESET ON/OFF control */
#define	CPG_RST_15					(CPG_BASE + 0x93C)	/* RESET ON/OFF control */
#define	CPG_RST_16					(CPG_BASE + 0x940)	/* RESET ON/OFF control */
#define	CPG_RST_17					(CPG_BASE + 0x944)	/* RESET ON/OFF control */
#define	CPG_RSTMON_0				(CPG_BASE + 0xA00)	/* RESET monitor */
#define	CPG_RSTMON_1				(CPG_BASE + 0xA04)	/* RESET monitor */
#define	CPG_RSTMON_2				(CPG_BASE + 0xA08)	/* RESET monitor */
#define	CPG_RSTMON_3				(CPG_BASE + 0xA0C)	/* RESET monitor */
#define	CPG_RSTMON_4				(CPG_BASE + 0xA10)	/* RESET monitor */
#define	CPG_RSTMON_5				(CPG_BASE + 0xA14)	/* RESET monitor */
#define	CPG_RSTMON_6				(CPG_BASE + 0xA18)	/* RESET monitor */
#define	CPG_RSTMON_7				(CPG_BASE + 0xA1C)	/* RESET monitor */
#define	CPG_RSTMON_8				(CPG_BASE + 0xA20)	/* RESET monitor */
#define	CPG_ERRORRST_SEL1			(CPG_BASE + 0xB00)	/* Error reset selection register */
#define	CPG_ERRORRST_SEL2			(CPG_BASE + 0xB04)	/* Error reset selection register */
#define	CPG_ERRORRST_SEL3			(CPG_BASE + 0xB08)	/* Error reset selection register */
#define	CPG_ERRORRST_SEL4			(CPG_BASE + 0xB0C)	/* Error reset selection register */
#define	CPG_ERRORRST_SEL5			(CPG_BASE + 0xB10)	/* Error reset selection register */
#define	CPG_ERRORRST_SEL6			(CPG_BASE + 0xB14)	/* Error reset selection register */
#define	CPG_ERRORRST_SEL7			(CPG_BASE + 0xB18)	/* Error reset selection register */
#define	CPG_ERRORRST_SEL8			(CPG_BASE + 0xB1C)	/* Error reset selection register */
#define	CPG_ERROR_RST2				(CPG_BASE + 0xB40)	/* Error reset register */
#define	CPG_ERROR_RST3				(CPG_BASE + 0xB44)	/* Error reset register */
#define	CPG_ERROR_RST4				(CPG_BASE + 0xB48)	/* Error reset register */
#define	CPG_ERROR_RST5				(CPG_BASE + 0xB4C)	/* Error reset register */
#define	CPG_ERROR_RST6				(CPG_BASE + 0xB50)	/* Error reset register */
#define	CPG_ERROR_RST7				(CPG_BASE + 0xB54)	/* Error reset register */
#define	CPG_ERROR_RST8				(CPG_BASE + 0xB58)	/* Error reset register */
#define	CPG_LP_CTL1					(CPG_BASE + 0xC00)	/* Lowpower Sequence Control Register 1 */
#define	CPG_LP_CTL2					(CPG_BASE + 0xC04)	/* Lowpower Sequence Control Register 2 */
#define	CPG_LP_GPU_CTL				(CPG_BASE + 0xC08)	/* GPU Lowpower Sequence Control Register */
#define	CPG_CM33_CTL				(CPG_BASE + 0xC0C)	/* CM33 Control register */
#define	CPG_CR8_CORESTATUS			(CPG_BASE + 0xC10)	/* CR8 core status register */
#define	CPG_CR8_CONFIG1				(CPG_BASE + 0xC14)	/* CR8 core configuration register1 */
#define	CPG_LP_CM33CTL0				(CPG_BASE + 0xC18)	/* Lowpower Sequence Cortex-M33 Control Register 0 */
#define	CPG_LP_CM33CTL1				(CPG_BASE + 0xC1C)	/* Lowpower Sequence Cortex-M33 Control Register 1 */
#define	CPG_LP_CA55_CTL1			(CPG_BASE + 0xC20)	/* Cortex-A55 Clock Control Register 1 */
#define	CPG_LP_CA55_CTL2			(CPG_BASE + 0xC24)	/* Cortex-A55 Clock Control Register 2 */
#define	CPG_LP_CA55_CTL3			(CPG_BASE + 0xC28)	/* Cortex-A55 Clock Control Register 3 */
#define	CPG_LP_CA55_CTL4			(CPG_BASE + 0xC2C)	/* Cortex-A55 Clock Control Register 4 */
#define	CPG_LP_CA55_CTL5			(CPG_BASE + 0xC30)	/* Cortex-A55 Clock Control Register 5 */
#define	CPG_LP_CA55_CTL6			(CPG_BASE + 0xC34)	/* Cortex-A55 Clock Control Register 6 */
#define	CPG_LP_CA55_CTL7			(CPG_BASE + 0xC38)	/* Cortex-A55 Clock Control Register 7 */
#define	CPG_LP_CR8_CTL1				(CPG_BASE + 0xC3C)	/* CR8SS control register 1 */
#define	CPG_LP_CR8_CTL3				(CPG_BASE + 0xC44)	/* CR8SS control register 3 */
#define	CPG_LP_CR8_CTL4				(CPG_BASE + 0xC48)	/* CR8SS control register 4 */
#define	CPG_LP_PMU_CTL1				(CPG_BASE + 0xC4C)	/* Lowpower Sequence Control Register @ */
#define	CPG_LP_SRAM_STBY_CTL1		(CPG_BASE + 0xC50)	/* DRP SRAM standby control */
#define	CPG_LP_SRAM_STBY_CTL2		(CPG_BASE + 0xC54)	/* Shared SRAM standby control 0 */
#define	CPG_LP_SRAM_STBY_CTL3		(CPG_BASE + 0xC58)	/* Shared SRAM standby control 8 */
#define	CPG_LP_GIC_CTL1				(CPG_BASE + 0xC5C)	/* GIC control */
#define	CPG_LP_DDR_CTL1				(CPG_BASE + 0xC60)	/* DDR retention control */
#define	CPG_LP_OTP_CTL1				(CPG_BASE + 0xC64)	/* OTP control register 1 */
#define	CPG_LP_CST_CTL1				(CPG_BASE + 0xC6C)	/* CST control register 1 */
#define	CPG_LP_CST_CTL2				(CPG_BASE + 0xC70)	/* CST control register 2 */
#define	CPG_LP_CST_CTL3				(CPG_BASE + 0xC74)	/* CST control register 3 */
#define	CPG_LP_PWC_CTL1				(CPG_BASE + 0xC78)	/* PWC control register 1 */
#define	CPG_LP_PWC_CTL2				(CPG_BASE + 0xC7C)	/* PWC control register 2 */
#define	CPG_OSTMTCKE				(CPG_BASE + 0xC80)	/* OSTM enable control */
#define	CPG_DBGRST					(CPG_BASE + 0xC84)	/* Reset control register when CA55 is debugged */
#define	OTP_HANDSHAKE_MON			(CPG_BASE + 0xC88)	/* OTP handshake monitor register */
#define	CPG_OTHERS_INI				(CPG_BASE + 0xC8C)	/* Others area reset control */
#define	CPG_BUS_1_MSTOP				(CPG_BASE + 0xD00)	/* MSTOP register 1 */
#define	CPG_BUS_2_MSTOP				(CPG_BASE + 0xD04)	/* MSTOP register 2 */
#define	CPG_BUS_3_MSTOP				(CPG_BASE + 0xD08)	/* MSTOP register 3 */
#define	CPG_BUS_4_MSTOP				(CPG_BASE + 0xD0C)	/* MSTOP register 4 */
#define	CPG_BUS_5_MSTOP				(CPG_BASE + 0xD10)	/* MSTOP register 5 */
#define	CPG_BUS_6_MSTOP				(CPG_BASE + 0xD14)	/* MSTOP register 6 */
#define	CPG_BUS_7_MSTOP				(CPG_BASE + 0xD18)	/* MSTOP register 7 */
#define	CPG_BUS_8_MSTOP				(CPG_BASE + 0xD1C)	/* MSTOP register 8 */
#define	CPG_BUS_9_MSTOP				(CPG_BASE + 0xD20)	/* MSTOP register 9 */
#define	CPG_BUS_10_MSTOP			(CPG_BASE + 0xD24)	/* MSTOP register 10*/
#define	CPG_BUS_11_MSTOP			(CPG_BASE + 0xD28)	/* MSTOP register 11 */
#define	CPG_BUS_12_MSTOP			(CPG_BASE + 0xD2C)	/* MSTOP register 12 */
#define	CPG_RSV1					(CPG_BASE + 0xE00)	/* Booking register 1 */
#define	CPG_RSV2					(CPG_BASE + 0xE04)	/* Booking register 2 */
#define	CPG_RSV3					(CPG_BASE + 0xE08)	/* Booking register 3 */
#define	CPG_RSV4					(CPG_BASE + 0xE0C)	/* Booking register 4 */
#define	CPG_RSV5					(CPG_BASE + 0xE10)	/* Booking register 5 */
#define	CPG_RSV6					(CPG_BASE + 0xE14)	/* Booking register 6 */

#define CPG_LP_CTL1_STBY					(0x00000001UL)
#define CPG_LP_CTL1_STBY_MSK				(0x00000001UL)
#define CPG_LP_CTL1_CA55SLEEP_REQ			(0x00000100UL)
#define CPG_LP_CTL1_CA55SLEEP_REQ_MSK		(0x00000F00UL)
#define CPG_LP_CTL1_STBY_CA55ST				(0x00010000UL)
#define CPG_LP_CTL1_STBY_CA55ST_MSK			(0x00010000UL)
#define CPG_LP_CTL1_CA55SLEEP_ACK			(0x01000000UL)
#define CPG_LP_CTL1_CA55SLEEP_ACK_MSK		(0x0F000000UL)

#define CPG_PLL_STBY_RESETB					(1UL << 0)
#define CPG_PLL_STBY_SSC_EN					(1UL << 2)
#define CPG_PLL_STBY_SSC_MODE				(1UL << 4)
#define CPG_PLL_STBY_RESETB_WEN				(1UL << 16)
#define CPG_PLL_STBY_SSC_EN_WEN				(1UL << 18)
#define CPG_PLL_STBY_SSC_MODE_WEN			(1UL << 20)

#define CPG_PLL_CLK1_DIV_P					(1UL << 0)
#define CPG_PLL_CLK1_DIV_M					(1UL << 6)
#define CPG_PLL_CLK1_DIV_K					(1UL << 16)

#define CPG_PLL_CLK2_DIV_S					(1UL << 0)
#define CPG_PLL_CLK2_MRR					(1UL << 8)
#define CPG_PLL_CLK2_MFR					(1UL << 16)

#define CPG_PLL_MON_PLLn_RESETB				(1UL << 0)
#define CPG_PLL_MON_PLLn_LOCK				(1UL << 4)

#define CPG_BUS_1_MSTOP_WDT1				(1UL << 0)
#define CPG_BUS_1_MSTOP_RIIC0				(1UL << 1)
#define CPG_BUS_1_MSTOP_RIIC1				(1UL << 2)
#define CPG_BUS_1_MSTOP_RIIC2				(1UL << 3)
#define CPG_BUS_1_MSTOP_RIIC3				(1UL << 4)
#define CPG_BUS_1_MSTOP_RIIC4				(1UL << 5)
#define CPG_BUS_1_MSTOP_RIIC5				(1UL << 6)
#define CPG_BUS_1_MSTOP_RIIC6				(1UL << 7)
#define CPG_BUS_1_MSTOP_RIIC7				(1UL << 8)
#define CPG_BUS_1_MSTOP_SPDIF0				(1UL << 9)
#define CPG_BUS_1_MSTOP_SPDIF1				(1UL << 10)
#define CPG_BUS_1_MSTOP_SPDIF2				(1UL << 11)
#define CPG_BUS_1_MSTOP_TMZ400_SRAM2		(1UL << 12)
#define CPG_BUS_1_MSTOP_TZC400_PCIE0		(1UL << 13)
#define CPG_BUS_1_MSTOP_TZC400_ACPU_RCPU	(1UL << 14)
#define CPG_BUS_1_MSTOP_TZC400_PCIE1		(1UL << 15)

#define CPG_BUS_2_MSTOP_SCU					(1UL << 0)
#define CPG_BUS_2_MSTOP_SCU_DMAC			(1UL << 1)
#define CPG_BUS_2_MSTOP_ADG					(1UL << 2)
#define CPG_BUS_2_MSTOP_SSIU				(1UL << 3)
#define CPG_BUS_2_MSTOP_SSIU_DMAC			(1UL << 4)
#define CPG_BUS_2_MSTOP_ADMAC				(1UL << 5)
#define CPG_BUS_2_MSTOP_TZC400_DDR00		(1UL << 6)
#define CPG_BUS_2_MSTOP_TZC400_DDR01		(1UL << 7)
#define CPG_BUS_2_MSTOP_TZC400_DDR10		(1UL << 8)
#define CPG_BUS_2_MSTOP_TZC400_DDR11		(1UL << 9)
#define CPG_BUS_2_MSTOP_TZC400_AXI_RCPU		(1UL << 10)
#define CPG_BUS_2_MSTOP_TZC400_SRAMA		(1UL << 11)
#define CPG_BUS_2_MSTOP_TZC400_PCIE			(1UL << 12)
#define CPG_BUS_2_MSTOP_GTM2				(1UL << 13)
#define CPG_BUS_2_MSTOP_GTM3				(1UL << 14)
#define CPG_BUS_2_MSTOP_TSU1				(1UL << 15)

#define CPG_BUS_3_MSTOP_SYC					(1UL << 0)
#define CPG_BUS_3_MSTOP_SRAM2_REG			(1UL << 1)
#define CPG_BUS_3_MSTOP_DMAC1				(1UL << 2)
#define CPG_BUS_3_MSTOP_DMAC2				(1UL << 3)
#define CPG_BUS_3_MSTOP_GE3D				(1UL << 4)
#define CPG_BUS_3_MSTOP_GIC					(1UL << 5)
#define CPG_BUS_3_MSTOP_GPV_ACPU			(1UL << 6)
#define CPG_BUS_3_MSTOP_GPV_ACPU_REG0		(1UL << 7)
#define CPG_BUS_3_MSTOP_GPV_ACPU_REG1		(1UL << 8)
#define CPG_BUS_3_MSTOP_ADC					(1UL << 9)
#define CPG_BUS_3_MSTOP_WDT0				(1UL << 10)
#define CPG_BUS_3_MSTOP_RTC_P0				(1UL << 11)
#define CPG_BUS_3_MSTOP_RTC_P1				(1UL << 12)
#define CPG_BUS_3_MSTOP_RIIC8				(1UL << 13)
#define CPG_BUS_3_MSTOP_SCIF				(1UL << 14)
#define CPG_BUS_3_MSTOP_CMTW0				(1UL << 15)

#define CPG_BUS_4_MSTOP_CMTW1				(1UL << 0)
#define CPG_BUS_4_MSTOP_CMTW2				(1UL << 1)
#define CPG_BUS_4_MSTOP_CMTW3				(1UL << 2)
#define CPG_BUS_4_MSTOP_SRAM0				(1UL << 3)
#define CPG_BUS_4_MSTOP_SRAM1				(1UL << 4)
#define CPG_BUS_4_MSTOP_XSPI				(1UL << 5)
/* Bit6 is reserved */
#define CPG_BUS_4_MSTOP_PFC					(1UL << 7)
/* Bit8 is reserved */
/* Bit9 is reserved */
#define CPG_BUS_4_MSTOP_SECURE_IP_P0		(1UL << 10)
#define CPG_BUS_4_MSTOP_SECURE_IP_P1		(1UL << 11)
#define CPG_BUS_4_MSTOP_TZC400_SRAMM		(1UL << 12)
#define CPG_BUS_4_MSTOP_TZC400_XSPI			(1UL << 13)
#define CPG_BUS_4_MSTOP_MHU					(1UL << 14)
/* Bit15 is reserved */

/* Bit0 is reserved */
#define CPG_BUS_5_MSTOP_CST					(1UL << 1)
#define CPG_BUS_5_MSTOP_TSU0				(1UL << 2)
#define CPG_BUS_5_MSTOP_SRAM0_REG			(1UL << 3)
#define CPG_BUS_5_MSTOP_SRAM1_REG			(1UL << 4)
#define CPG_BUS_5_MSTOP_XSPI_REG			(1UL << 5)
#define CPG_BUS_5_MSTOP_PDM0				(1UL << 6)
#define CPG_BUS_5_MSTOP_PDM1				(1UL << 7)
#define CPG_BUS_5_MSTOP_GPV_MCPU			(1UL << 8)
#define CPG_BUS_5_MSTOP_DMAC0				(1UL << 9)
#define CPG_BUS_5_MSTOP_GTM0				(1UL << 10)
#define CPG_BUS_5_MSTOP_GTM1				(1UL << 11)
#define CPG_BUS_5_MSTOP_WDT2				(1UL << 12)
#define CPG_BUS_5_MSTOP_WDT3				(1UL << 13)
#define CPG_BUS_5_MSTOP_CRC					(1UL << 14)
#define CPG_BUS_5_MSTOP_CMTW4				(1UL << 15)

#define CPG_BUS_6_MSTOP_CMTW5				(1UL << 0)
#define CPG_BUS_6_MSTOP_CMTW6				(1UL << 1)
#define CPG_BUS_6_MSTOP_CMTW7				(1UL << 2)
#define CPG_BUS_6_MSTOP_POEG0A				(1UL << 3)
#define CPG_BUS_6_MSTOP_POEG0B				(1UL << 4)
#define CPG_BUS_6_MSTOP_POEG0C				(1UL << 5)
#define CPG_BUS_6_MSTOP_POEG0D				(1UL << 6)
#define CPG_BUS_6_MSTOP_POEG1A				(1UL << 7)
#define CPG_BUS_6_MSTOP_POEG1B				(1UL << 8)
#define CPG_BUS_6_MSTOP_POEG1C				(1UL << 9)
#define CPG_BUS_6_MSTOP_POEG1D				(1UL << 10)
#define CPG_BUS_6_MSTOP_GPT0				(1UL << 11)
#define CPG_BUS_6_MSTOP_GPT1				(1UL << 12)
#define CPG_BUS_6_MSTOP_DDR0_P0				(1UL << 13)
#define CPG_BUS_6_MSTOP_DDR0_P1				(1UL << 14)
#define CPG_BUS_6_MSTOP_DDR0_P2				(1UL << 15)

#define CPG_BUS_7_MSTOP_DDR_0_P3			(1UL << 0)
#define CPG_BUS_7_MSTOP_DDR_0_P4			(1UL << 1)
#define CPG_BUS_7_MSTOP_DDR_1_P0			(1UL << 2)
#define CPG_BUS_7_MSTOP_DDR_1_P1			(1UL << 3)
#define CPG_BUS_7_MSTOP_DDR_1_P2			(1UL << 4)
#define CPG_BUS_7_MSTOP_DDR_1_P3			(1UL << 5)
#define CPG_BUS_7_MSTOP_DDR_1_P4			(1UL << 6)
#define CPG_BUS_7_MSTOP_USB20_HOST			(1UL << 7)
#define CPG_BUS_7_MSTOP_USB21_HOST			(1UL << 8)
#define CPG_BUS_7_MSTOP_USB2_FUNC			(1UL << 9)
#define CPG_BUS_7_MSTOP_USB20_PHY			(1UL << 10)
#define CPG_BUS_7_MSTOP_USB21_PHY			(1UL << 11)
#define CPG_BUS_7_MSTOP_USB30_HOST			(1UL << 12)
#define CPG_BUS_7_MSTOP_USB31_HOST			(1UL << 13)
#define CPG_BUS_7_MSTOP_USB30_PHY			(1UL << 14)
#define CPG_BUS_7_MSTOP_USB31_PHY			(1UL << 15)

#define CPG_BUS_8_MSTOP_PCIE_PHY			(1UL << 0)
#define CPG_BUS_8_MSTOP_GPV_COM_SUB			(1UL << 1)
#define CPG_BUS_8_MSTOP_SD0					(1UL << 2)
#define CPG_BUS_8_MSTOP_SD1					(1UL << 3)
#define CPG_BUS_8_MSTOP_SD2					(1UL << 4)
#define CPG_BUS_8_MSTOP_GBETH0				(1UL << 5)
#define CPG_BUS_8_MSTOP_GBETH1				(1UL << 6)
#define CPG_BUS_8_MSTOP_GPV_COM				(1UL << 7)
#define CPG_BUS_8_MSTOP_DRP_AI_MAC			(1UL << 8)
#define CPG_BUS_8_MSTOP_DRP_AP_DRP0			(1UL << 9)
#define CPG_BUS_8_MSTOP_DRP1				(1UL << 10)
#define CPG_BUS_8_MSTOP_GPV_DRP				(1UL << 11)
#define CPG_BUS_8_MSTOP_SRAM4_REG			(1UL << 12)
#define CPG_BUS_8_MSTOP_SRAM5_REG			(1UL << 13)
#define CPG_BUS_8_MSTOP_SRAM6_REG			(1UL << 14)
#define CPG_BUS_8_MSTOP_SRAM7_REG			(1UL << 15)

#define CPG_BUS_9_MSTOP_SRAM8_REG			(1UL << 0)
#define CPG_BUS_9_MSTOP_SRAM9_REG			(1UL << 1)
#define CPG_BUS_9_MSTOP_SRAM10_REG			(1UL << 2)
#define CPG_BUS_9_MSTOP_SRAM11_REG			(1UL << 3)
#define CPG_BUS_9_MSTOP_CRU0				(1UL << 4)
#define CPG_BUS_9_MSTOP_CRU1				(1UL << 5)
#define CPG_BUS_9_MSTOP_CRU2				(1UL << 6)
#define CPG_BUS_9_MSTOP_CRU3				(1UL << 7)
#define CPG_BUS_9_MSTOP_ISP_APB				(1UL << 8)
#define CPG_BUS_9_MSTOP_ISP_AXI				(1UL << 9)
#define CPG_BUS_9_MSTOP_GPV_VIDEO0			(1UL << 10)
#define CPG_BUS_9_MSTOP_VCD_P0				(1UL << 11)
#define CPG_BUS_9_MSTOP_VCD_P1				(1UL << 12)
#define CPG_BUS_9_MSTOP_VCD_P2				(1UL << 13)
#define CPG_BUS_9_MSTOP_DSI_LINK			(1UL << 14)
#define CPG_BUS_9_MSTOP_DSI_PHY				(1UL << 15)

#define CPG_BUS_10_MSTOP_ISU				(1UL << 0)
#define CPG_BUS_10_MSTOP_LCDC_DU			(1UL << 1)
#define CPG_BUS_10_MSTOP_LCDC_FCPVD			(1UL << 2)
#define CPG_BUS_10_MSTOP_LCDC_VSPD			(1UL << 3)
#define CPG_BUS_10_MSTOP_GPV_VIDEO1			(1UL << 4)
#define CPG_BUS_10_MSTOP_DDR0_PHY			(1UL << 5)
#define CPG_BUS_10_MSTOP_DDR1_PHY			(1UL << 6)
#define CPG_BUS_10_MSTOP_DDR0_CTRL			(1UL << 7)
#define CPG_BUS_10_MSTOP_DDR1_CTRL			(1UL << 8)
#define CPG_BUS_10_MSTOP_SRAM3				(1UL << 9)
#define CPG_BUS_10_MSTOP_CR8_TCM			(1UL << 10)
#define CPG_BUS_10_MSTOP_DMAC3				(1UL << 11)
#define CPG_BUS_10_MSTOP_DMAC4				(1UL << 12)
#define CPG_BUS_10_MSTOP_SRAM3_REG			(1UL << 13)
#define CPG_BUS_10_MSTOP_CANFD				(1UL << 14)
#define CPG_BUS_10_MSTOP_I3C0				(1UL << 15)

#define CPG_BUS_11_MSTOP_RSPI0				(1UL << 0)
#define CPG_BUS_11_MSTOP_RSPI1				(1UL << 1)
#define CPG_BUS_11_MSTOP_RSPI2				(1UL << 2)
#define CPG_BUS_11_MSTOP_RSCI0				(1UL << 3)
#define CPG_BUS_11_MSTOP_RSCI1				(1UL << 4)
#define CPG_BUS_11_MSTOP_RSCI2				(1UL << 5)
#define CPG_BUS_11_MSTOP_RSCI3				(1UL << 6)
#define CPG_BUS_11_MSTOP_RSCI4				(1UL << 7)
#define CPG_BUS_11_MSTOP_RSCI5				(1UL << 8)
#define CPG_BUS_11_MSTOP_RSCI6				(1UL << 9)
#define CPG_BUS_11_MSTOP_RSCI7				(1UL << 10)
#define CPG_BUS_11_MSTOP_RSCI8				(1UL << 11)
#define CPG_BUS_11_MSTOP_RSCI9				(1UL << 12)
#define CPG_BUS_11_MSTOP_GTM4				(1UL << 13)
#define CPG_BUS_11_MSTOP_GTM5				(1UL << 14)
#define CPG_BUS_11_MSTOP_GTM6				(1UL << 15)

#define CPG_BUS_12_MSTOP_GTM7				(1UL << 0)
#define CPG_BUS_12_MSTOP_SRAM4				(1UL << 1)
#define CPG_BUS_12_MSTOP_SRAM5				(1UL << 2)
#define CPG_BUS_12_MSTOP_SRAM6				(1UL << 3)
#define CPG_BUS_12_MSTOP_SRAM7				(1UL << 4)
#define CPG_BUS_12_MSTOP_SRAM8				(1UL << 5)
#define CPG_BUS_12_MSTOP_SRAM9				(1UL << 6)
#define CPG_BUS_12_MSTOP_SRAM10				(1UL << 7)
#define CPG_BUS_12_MSTOP_SRAM11				(1UL << 8)
#define CPG_BUS_12_MSTOP_MCPU_TO_ACPU		(1UL << 9)
#define CPG_BUS_12_MSTOP_ACPU_TO_MCPU		(1UL << 10)
/* Bit11 - Bit15 is reserved */

#define CPG_ERRORRST_SELx_ERRRSTSEL0		(1UL << 0)
#define CPG_ERRORRST_SELx_ERRRSTSEL1		(1UL << 1)
#define CPG_ERRORRST_SELx_ERRRSTSEL2		(1UL << 2)
#define CPG_ERRORRST_SELx_ERRRSTSEL3		(1UL << 3)

#define CPG_LP_CA55_CTL2_COREPREQ0			(1UL << 0)
#define CPG_LP_CA55_CTL2_COREPREQ1			(1UL << 16)
#define CPG_LP_CA55_CTL3_COREPREQ2			(1UL << 0)
#define CPG_LP_CA55_CTL3_COREPREQ3			(1UL << 16)

#define CPG_LP_CA55_CTL2_CORESTATE0			(1UL << 1)
#define CPG_LP_CA55_CTL2_CORESTATE1			(1UL << 17)
#define CPG_LP_CA55_CTL3_CORESTATE2			(1UL << 1)
#define CPG_LP_CA55_CTL3_CORESTATE3			(1UL << 17)

#define CPG_LP_CA55_CTL2_CORESTATE0_ON_MASK	(1UL << 4)
#define CPG_LP_CA55_CTL2_CORESTATE1_ON_MASK	(1UL << 20)
#define CPG_LP_CA55_CTL3_CORESTATE2_ON_MASK	(1UL << 4)
#define CPG_LP_CA55_CTL3_CORESTATE3_ON_MASK	(1UL << 20)

#define CPG_LP_CA55_CTL2_COREACCEPT0		(1UL << 8)
#define CPG_LP_CA55_CTL2_COREACCEPT1		(1UL << 24)
#define CPG_LP_CA55_CTL3_COREACCEPT2		(1UL << 8)
#define CPG_LP_CA55_CTL3_COREACCEPT3		(1UL << 24)

#endif	/* __CPG_REGS_H__ */
