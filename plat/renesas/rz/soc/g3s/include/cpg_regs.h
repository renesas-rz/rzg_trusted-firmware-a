/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CPG_REGS_H__
#define __CPG_REGS_H__

#include <rz_soc_def.h>				/* Get the CPG base address */

#define CPG_PLL1_STBY							(CPG_BASE + 0x0000)	/* PLL1 standby control register */
#define CPG_PLL1_CLK1							(CPG_BASE + 0x0004)	/* PLL1 output clock setting register 1 */
#define CPG_PLL1_CLK2							(CPG_BASE + 0x0008)	/* PLL1 output clock setting register 2 */
#define CPG_PLL1_MON							(CPG_BASE + 0x000C)	/* PLL1 monitor register */

#define CPG_PLL4_STBY							(CPG_BASE + 0x0030)	/* PLL4 standby control register */
#define CPG_PLL4_CLK1							(CPG_BASE + 0x0034)	/* PLL4 output clock setting register 1 */
#define CPG_PLL4_CLK2							(CPG_BASE + 0x0038)	/* PLL4 output clock setting register 2 */
#define CPG_PLL4_MON							(CPG_BASE + 0x003C)	/* PLL4 monitor register */

#define CPG_PLL6_STBY							(CPG_BASE + 0x0050)	/* PLL6 standby control register */
#define CPG_PLL6_CLK1							(CPG_BASE + 0x0054)	/* PLL6 output clock setting register 1 */
#define CPG_PLL6_CLK2							(CPG_BASE + 0x0058)	/* PLL6 output clock setting register 2 */
#define CPG_PLL6_MON							(CPG_BASE + 0x005C)	/* PLL6 monitor register */

#define CPG_PLL1_SETTING						(CPG_BASE + 0x0100)	/* PLL1_SEL_SETTING */
#define CPG_OTPPLL0_MON							(CPG_BASE + 0x0104)	/* OTP_OTPPLL0 monitor register */
#define CPG_OTPPLL1_MON							(CPG_BASE + 0x0108)	/* OTP_OTPPLL1 monitor register */
#define CPG_OTPPLL2_MON							(CPG_BASE + 0x010C)	/* OTP_OTPPLL2 monitor register */

#define CPG_PLL2_MON							(CPG_BASE + 0x001C) /* PLL2(SSCG) Monitor Register */
#define CPG_PLL3_MON							(CPG_BASE + 0x002C) /* PLL3(SSCG) Monitor Register */

#define CPG_PL1_DDIV							(CPG_BASE + 0x0200)	/* Division ratio setting register */
#define CPG_PL2_DDIV							(CPG_BASE + 0x0204)	/* Division ratio setting register */
#define CPG_PL3_DDIV							(CPG_BASE + 0x0208)	/* Division ratio setting register */
#define CPG_PL6_DDIV							(CPG_BASE + 0x0214)	/* Division ratio setting register */

#define CPG_SDHI_DDIV							(CPG_BASE + 0x0218)	/* Division ratio setting register */
#define CPG_OCTA_DDIV							(CPG_BASE + 0x021C) /* Division ratio setting register */
#define CPG_SPI_DDIV							(CPG_BASE + 0x0220) /* Division ratio setting register */
#define CPG_PLL_DSEL							(CPG_BASE + 0x0240)	/* Source clock setting register */
#define CPG_SDHI_DSEL							(CPG_BASE + 0x0244)	/* Source clock setting register */
#define CPG_CLKDIVSTATUS						(CPG_BASE + 0x0280)	/* CLK status register */
#define CPG_CLKSELSTATUS						(CPG_BASE + 0x0284)	/* CLK status register */

#define CPG_OCTA_SSEL							(CPG_BASE + 0x0400)	/* Source clock setting register */
#define CPG_SPI_SSEL							(CPG_BASE + 0x0404)	/* Source clock setting register */

#define CPG_CLKON_CA55							(CPG_BASE + 0x0500)	/* Clock ON / OFF register CA55 */
#define CPG_CLKON_CM33							(CPG_BASE + 0x0504)	/* Clock ON / OFF register CM33 */
#define CPG_CLKON_SRAM_ACPU						(CPG_BASE + 0x0508)	/* Clock ON / OFF register SRAM_ACPU */
#define CPG_CLKON_SRAM_MCPU						(CPG_BASE + 0x050C)	/* Clock ON / OFF register SRAM_MCPU */
#define CPG_CLKON_GIC600						(CPG_BASE + 0x0514)	/* Clock ON / OFF register GIC600 */
#define CPG_CLKON_IA55							(CPG_BASE + 0x0518)	/* Clock ON / OFF register IA55 */
#define CPG_CLKON_IM33							(CPG_BASE + 0x051C)	/* Clock ON / OFF register IM33 */
#define CPG_CLKON_MHU							(CPG_BASE + 0x0520)	/* Clock ON / OFF register MHU */
#define CPG_CLKON_CST							(CPG_BASE + 0x0524)	/* Clock ON / OFF register CST */
#define CPG_CLKON_SYC							(CPG_BASE + 0x0528)	/* Clock ON / OFF register SYC */
#define CPG_CLKON_DMAC_REG						(CPG_BASE + 0x052C)	/* Clock ON / OFF register DMAC_REG */
#define CPG_CLKON_GTM							(CPG_BASE + 0x0534)	/* Clock ON / OFF register GTM */
#define CPG_CLKON_MTU							(CPG_BASE + 0x0538)	/* Clock ON / OFF register MTU */
#define CPG_CLKON_POE3							(CPG_BASE + 0x053C)	/* Clock ON / OFF register POE3 */
#define CPG_CLKON_GPT							(CPG_BASE + 0x0540)	/* Clock ON / OFF register GPT */
#define CPG_CLKON_POEG							(CPG_BASE + 0x0544)	/* Clock ON / OFF register POEG */
#define CPG_CLKON_WDT							(CPG_BASE + 0x0548)	/* Clock ON / OFF register WDT */
#define CPG_CLKON_DDR							(CPG_BASE + 0x054C)	/* Clock ON / OFF register DDR */
#define CPG_CLKON_SPI							(CPG_BASE + 0x0550)	/* Clock ON / OFF register SPI */
#define CPG_CLKON_SDHI							(CPG_BASE + 0x0554)	/* Clock ON / OFF register SDHI */
#define CPG_CLKON_SSI							(CPG_BASE + 0x0570)	/* Clock ON / OFF register SSI */
#define CPG_CLKON_SRC							(CPG_BASE + 0x0574)	/* Clock ON / OFF register SRC */
#define CPG_CLKON_USB							(CPG_BASE + 0x0578)	/* Clock ON / OFF register USB */
#define CPG_CLKON_ETH							(CPG_BASE + 0x057C)	/* Clock ON / OFF register ETH */
#define CPG_CLKON_I2C							(CPG_BASE + 0x0580)	/* Clock ON / OFF register I2C */
#define CPG_CLKON_SCIF							(CPG_BASE + 0x0584)	/* Clock ON / OFF register SCIF */
#define CPG_CLKON_SCI							(CPG_BASE + 0x0588)	/* Clock ON / OFF register SCI */
#define CPG_CLKON_IRDA							(CPG_BASE + 0x058C)	/* Clock ON / OFF register IRDA */
#define CPG_CLKON_RSPI							(CPG_BASE + 0x0590)	/* Clock ON / OFF register RSPI */
#define CPG_CLKON_CANFD							(CPG_BASE + 0x0594)	/* Clock ON / OFF register CANFD */
#define CPG_CLKON_GPIO							(CPG_BASE + 0x0598)	/* Clock ON / OFF register GPIO */
#define CPG_CLKON_ADC							(CPG_BASE + 0x05A8)	/* Clock ON / OFF register ADC */
#define CPG_CLKON_TSU							(CPG_BASE + 0x05AC)	/* Clock ON / OFF register TSU */

#define CPG_CLKON_AXI_ACPU_BUS					(CPG_BASE + 0x05B4)	/* Clock ON / OFF register AXI_ACPU_BUS */
#define CPG_CLKON_AXI_MCPU_BUS					(CPG_BASE + 0x05B8)	/* Clock ON / OFF register AXI_MCPU_BUS */
#define CPG_CLKON_AXI_COM_BUS					(CPG_BASE + 0x05BC)	/* Clock ON / OFF register AXI_COM_BUS */
#define CPG_CLKON_PERI_COM						(CPG_BASE + 0x05C4)	/* Clock ON / OFF register PERI_COM */
#define CPG_CLKON_REG1_BUS						(CPG_BASE + 0x05C8)	/* Clock ON / OFF register REG1_BUS */
#define CPG_CLKON_REG0_BUS						(CPG_BASE + 0x05CC)	/* Clock ON / OFF register REG0_BUS */
#define CPG_CLKON_PERI_CPU						(CPG_BASE + 0x05D0)	/* Clock ON / OFF register PERI_CPU */
#define CPG_CLKON_PERI_DDR						(CPG_BASE + 0x05D8)	/* Clock ON / OFF register PERI_DDR */
#define CPG_CLKON_AXI_TZCDDR					(CPG_BASE + 0x05DC)	/* Clock ON / OFF register AXI_TZCDDR */
#define CPG_CLKON_MTGPGS						(CPG_BASE + 0x05E0)	/* Clock ON / OFF register MTGPGS */

#define CPG_CLKON_OCTA							(CPG_BASE + 0x05F4)	/* Clock ON / OFF register AXI_DEFAULT_SLV */
#define CPG_CLKON_OTFDE_DDR						(CPG_BASE + 0x05F8)	/* Clock ON / OFF register OTFDE DDR */
#define CPG_CLKON_OTFDE_SPI						(CPG_BASE + 0x0600)	/* Clock ON / OFF register OTFDE SPI */
#define CPG_CLKON_PDM							(CPG_BASE + 0x0604)	/* Clock ON / OFF register PDM */
#define CPG_CLKON_PCI							(CPG_BASE + 0x0608)	/* Clock ON / OFF register PCI */
#define CPG_CLKON_SPDIF							(CPG_BASE + 0x060C)	/* Clock ON / OFF register SPDIF */
#define CPG_CLKON_I3C							(CPG_BASE + 0x0610)	/* Clock ON / OFF register I3C */
#define CPG_CLKON_VBAT							(CPG_BASE + 0x0614)	/* Clock ON / OFF register VBAT */

#define CPG_CLKMON_CA55							(CPG_BASE + 0x0680)	/* Clock monitor register CA55 */
#define CPG_CLKMON_CM33							(CPG_BASE + 0x0684)	/* Clock monitor register CM33 */
#define CPG_CLKMON_SRAM_ACPU					(CPG_BASE + 0x0688)	/* Clock monitor register SRAM_ACPU */
#define CPG_CLKMON_SRAM_MCPU					(CPG_BASE + 0x068C)	/* Clock monitor register SRAM_MCPU */
#define CPG_CLKMON_GIC600						(CPG_BASE + 0x0694)	/* Clock monitor register GIC600 */
#define CPG_CLKMON_IA55							(CPG_BASE + 0x0698)	/* Clock monitor register IA55 */
#define CPG_CLKMON_IM33							(CPG_BASE + 0x069C)	/* Clock monitor register IM33 */
#define CPG_CLKMON_MHU							(CPG_BASE + 0x06A0)	/* Clock monitor register MHU */
#define CPG_CLKMON_CST							(CPG_BASE + 0x06A4)	/* Clock monitor register CST */
#define CPG_CLKMON_SYC							(CPG_BASE + 0x06A8)	/* Clock monitor register SYC */
#define CPG_CLKMON_DMAC_REG						(CPG_BASE + 0x06AC)	/* Clock monitor register DMAC_REG */
#define CPG_CLKMON_GTM							(CPG_BASE + 0x06B4)	/* Clock monitor register GTM */
#define CPG_CLKMON_MTU							(CPG_BASE + 0x06B8)	/* Clock monitor register MTU */
#define CPG_CLKMON_POE3							(CPG_BASE + 0x06BC)	/* Clock monitor register POE3 */
#define CPG_CLKMON_GPT							(CPG_BASE + 0x06C0)	/* Clock monitor register GPT */
#define CPG_CLKMON_POEG							(CPG_BASE + 0x06C4)	/* Clock monitor register POEG */
#define CPG_CLKMON_WDT							(CPG_BASE + 0x06C8)	/* Clock monitor register WDT */
#define CPG_CLKMON_DDR							(CPG_BASE + 0x06CC)	/* Clock monitor register DDR */
#define CPG_CLKMON_SPI							(CPG_BASE + 0x06D0)	/* Clock monitor register SPI */
#define CPG_CLKMON_SDHI							(CPG_BASE + 0x06D4)	/* Clock monitor register SDHI */
#define CPG_CLKMON_SSI							(CPG_BASE + 0x06F0)	/* Clock monitor register SSI */
#define CPG_CLKMON_SRC							(CPG_BASE + 0x06F4)	/* Clock monitor register SRC */
#define CPG_CLKMON_USB							(CPG_BASE + 0x06F8)	/* Clock monitor register USB */
#define CPG_CLKMON_ETH							(CPG_BASE + 0x06FC)	/* Clock monitor register ETH */
#define CPG_CLKMON_I2C							(CPG_BASE + 0x0700)	/* Clock monitor register I2C */
#define CPG_CLKMON_SCIF							(CPG_BASE + 0x0704)	/* Clock monitor register SCIF */
#define CPG_CLKMON_SCI							(CPG_BASE + 0x0708)	/* Clock monitor register SCI */
#define CPG_CLKMON_IRDA							(CPG_BASE + 0x070C)	/* Clock monitor register IRDA */
#define CPG_CLKMON_RSPI							(CPG_BASE + 0x0710)	/* Clock monitor register RSPI */
#define CPG_CLKMON_CANFD						(CPG_BASE + 0x0714)	/* Clock monitor register CANFD */
#define CPG_CLKMON_GPIO							(CPG_BASE + 0x0718)	/* Clock monitor register GPIO */
#define CPG_CLKMON_ADC							(CPG_BASE + 0x0728)	/* Clock monitor register ADC */
#define CPG_CLKMON_TSU							(CPG_BASE + 0x072C)	/* Clock monitor register TSU */

#define CPG_CLKMON_AXI_ACPU_BUS					(CPG_BASE + 0x0734)	/* Clock monitor register AXI_ACPU_BUS */
#define CPG_CLKMON_AXI_MCPU_BUS					(CPG_BASE + 0x0738)	/* Clock monitor register AXI_MCPU_BUS */
#define CPG_CLKMON_AXI_COM_BUS					(CPG_BASE + 0x073C)	/* Clock monitor register AXI_COM_BUS */
#define CPG_CLKMON_PERI_COM						(CPG_BASE + 0x0744)	/* Clock monitor register PERI_COM */
#define CPG_CLKMON_REG1_BUS						(CPG_BASE + 0x0748)	/* Clock monitor register REG1_BUS */
#define CPG_CLKMON_REG0_BUS						(CPG_BASE + 0x074C)	/* Clock monitor register REG0_BUS */
#define CPG_CLKMON_PERI_CPU						(CPG_BASE + 0x0750)	/* Clock monitor register PERI_CPU */
#define CPG_CLKMON_PERI_DDR						(CPG_BASE + 0x0758)	/* Clock monitor register PERI_DDR */
#define CPG_CLKMON_AXI_TZCDDR					(CPG_BASE + 0x075C)	/* Clock monitor register AXI_TZCDDR */
#define CPG_CLKMON_MTGPGS						(CPG_BASE + 0x0760)	/* Clock monitor register MTGPGS */

#define CPG_CLKMON_OCTA							(CPG_BASE + 0x0774)	/* Clock monitor register OCTA */
#define CPG_CLKMON_OTFDE_DDR					(CPG_BASE + 0x0778)	/* Clock monitor register OTFDE DDR */
#define CPG_CLKMON_OTFDE_SPI					(CPG_BASE + 0x0780)	/* Clock monitor register OTFDE SPI */
#define CPG_CLKMON_PDM							(CPG_BASE + 0x0784)	/* Clock monitor register PDM */
#define CPG_CLKMON_PCI							(CPG_BASE + 0x0788)	/* Clock monitor register PCI */
#define CPG_CLKMON_SPDIF						(CPG_BASE + 0x078C)	/* Clock monitor register SPDIF */
#define CPG_CLKMON_I3C							(CPG_BASE + 0x0790)	/* Clock monitor register I3C */
#define CPG_CLKMON_VBAT							(CPG_BASE + 0x0794)	/* Clock monitor register VBAT */

#define CPG_RST_CA55							(CPG_BASE + 0x0800)	/* Reset ON / OFF register CA55 */
#define CPG_RST_CM33							(CPG_BASE + 0x0804)	/* Reset ON / OFF register CM33 */
#define CPG_RST_SRAM_ACPU						(CPG_BASE + 0x0808)	/* Reset ON / OFF register SRAM_ACPU */
#define CPG_RST_SRAM_MCPU						(CPG_BASE + 0x080C)	/* Reset ON / OFF register SRAM_MCPU */
#define CPG_RST_GIC600							(CPG_BASE + 0x0814)	/* Reset ON / OFF register GIC600 */
#define CPG_RST_IA55							(CPG_BASE + 0x0818)	/* Reset ON / OFF register IA55 */
#define CPG_RST_IM33							(CPG_BASE + 0x081C)	/* Reset ON / OFF register IM33 */
#define CPG_RST_MHU								(CPG_BASE + 0x0820)	/* Reset ON / OFF register MHU */
#define CPG_RST_SYC								(CPG_BASE + 0x0828)	/* Reset ON / OFF register SYC */
#define CPG_RST_DMAC							(CPG_BASE + 0x082C)	/* Reset ON / OFF register DMAC */
#define CPG_RST_GTM								(CPG_BASE + 0x0834)	/* Reset ON / OFF register GTM */
#define CPG_RST_MTU								(CPG_BASE + 0x0838)	/* Reset ON / OFF register MTU */
#define CPG_RST_POE3							(CPG_BASE + 0x083C)	/* Reset ON / OFF register POE3 */
#define CPG_RST_GPT								(CPG_BASE + 0x0840)	/* Reset ON / OFF register GPT */
#define CPG_RST_POEG							(CPG_BASE + 0x0844)	/* Reset ON / OFF register POEG */
#define CPG_RST_WDT								(CPG_BASE + 0x0848)	/* Reset ON / OFF register WDT */
#define CPG_RST_DDR								(CPG_BASE + 0x084C)	/* Reset ON / OFF register DDR */
#define CPG_RST_SPI								(CPG_BASE + 0x0850)	/* Reset ON / OFF register SPI */
#define CPG_RST_SDHI							(CPG_BASE + 0x0854)	/* Reset ON / OFF register SDHI */
#define CPG_RST_SSIF							(CPG_BASE + 0x0870)	/* Reset ON / OFF register SSIF */
#define CPG_RST_SRC								(CPG_BASE + 0x0874)	/* Reset ON / OFF register SRC */
#define CPG_RST_USB								(CPG_BASE + 0x0878)	/* Reset ON / OFF register USB */
#define CPG_RST_ETH								(CPG_BASE + 0x087C)	/* Reset ON / OFF register ETH */
#define CPG_RST_I2C								(CPG_BASE + 0x0880)	/* Reset ON / OFF register I2C */
#define CPG_RST_SCIF							(CPG_BASE + 0x0884)	/* Reset ON / OFF register SCIF */
#define CPG_RST_SCI								(CPG_BASE + 0x0888)	/* Reset ON / OFF register SCI */
#define CPG_RST_IRDA							(CPG_BASE + 0x088C)	/* Reset ON / OFF register IRDA */
#define CPG_RST_RSPI							(CPG_BASE + 0x0890)	/* Reset ON / OFF register RSPI */
#define CPG_RST_CANFD							(CPG_BASE + 0x0894)	/* Reset ON / OFF register CANFD */
#define CPG_RST_GPIO							(CPG_BASE + 0x0898)	/* Reset ON / OFF register GPIO */
#define CPG_RST_ADC								(CPG_BASE + 0x08A8)	/* Reset ON / OFF register ADC */
#define CPG_RST_TSU								(CPG_BASE + 0x08AC)	/* Reset ON / OFF register TSU */

#define CPG_RST_AXI_ACPU_BUS					(CPG_BASE + 0x08B4)	/* Reset ON / OFF register AXI_ACPU_BUS */
#define CPG_RST_AXI_MCPU_BUS					(CPG_BASE + 0x08B8)	/* Reset ON / OFF register AXI_MCPU_BUS */
#define CPG_RST_AXI_COM_BUS						(CPG_BASE + 0x08BC)	/* Reset ON / OFF register AXI_COM_BUS */
#define CPG_RST_PERI_COM						(CPG_BASE + 0x08C4)	/* Reset ON / OFF register PERI_COM */
#define CPG_RST_REG1_BUS						(CPG_BASE + 0x08C8)	/* Reset ON / OFF register REG1_BUS */
#define CPG_RST_REG0_BUS						(CPG_BASE + 0x08CC)	/* Reset ON / OFF register REG0_BUS */
#define CPG_RST_PERI_CPU						(CPG_BASE + 0x08D0)	/* Reset ON / OFF register PERI_CPU */
#define CPG_RST_PERI_DDR						(CPG_BASE + 0x08D8)	/* Reset ON / OFF register PERI_DDR */
#define CPG_RST_AXI_TZCDDR						(CPG_BASE + 0x08DC)	/* Reset ON / OFF register AXI_TZCDDR */
#define CPG_RST_MTGPGS							(CPG_BASE + 0x08E0)	/* Reset ON / OFF register MTGPGS */

#define CPG_RST_OCTA							(CPG_BASE + 0x08F4)	/* Reset ON / OFF register OCTA */
#define CPG_RST_OTFDE_DDR						(CPG_BASE + 0x08F8)	/* Reset ON / OFF register OTFDE DDR */
#define CPG_RST_OTFDE_SPI						(CPG_BASE + 0x0900)	/* Reset ON / OFF register OTFDE SPI */
#define CPG_RST_PDM								(CPG_BASE + 0x0904)	/* Reset ON / OFF register PDM */
#define CPG_RST_PCI								(CPG_BASE + 0x0908)	/* Reset ON / OFF register PCI */
#define CPG_RST_SPDIF							(CPG_BASE + 0x090C)	/* Reset ON / OFF Register SPDIF */
#define CPG_RST_I3C								(CPG_BASE + 0x0910)	/* Reset ON / OFF register I3C */
#define CPG_RST_VBAT							(CPG_BASE + 0x0914)	/* Reset ON / OFF register VBAT */

#define CPG_RSTMON_CA55							(CPG_BASE + 0x0980)	/* Reset monitor register CA55 */
#define CPG_RSTMON_CM33							(CPG_BASE + 0x0984)	/* Reset monitor register CM33 */
#define CPG_RSTMON_SRAM_ACPU					(CPG_BASE + 0x0988)	/* Reset monitor register SRAM_ACPU */
#define CPG_RSTMON_SRAM_MCPU					(CPG_BASE + 0x098C)	/* Reset monitor register SRAM_MCPU */
#define CPG_RSTMON_GIC600						(CPG_BASE + 0x0994)	/* Reset monitor register GIC600 */
#define CPG_RSTMON_IA55							(CPG_BASE + 0x0998)	/* Reset monitor register IA55 */
#define CPG_RSTMON_IM33							(CPG_BASE + 0x099C)	/* Reset monitor register IM33 */
#define CPG_RSTMON_MHU							(CPG_BASE + 0x09A0)	/* Reset monitor register MHU */
#define CPG_RSTMON_SYC							(CPG_BASE + 0x09A8)	/* Reset monitor register SYC */
#define CPG_RSTMON_DMAC							(CPG_BASE + 0x09AC)	/* Reset monitor register DMAC */
#define CPG_RSTMON_GTM							(CPG_BASE + 0x09B4)	/* Reset monitor register GTM */
#define CPG_RSTMON_MTU							(CPG_BASE + 0x09B8)	/* Reset monitor register MTU */
#define CPG_RSTMON_POE3							(CPG_BASE + 0x09BC)	/* Reset monitor register POE3 */
#define CPG_RSTMON_GPT							(CPG_BASE + 0x09C0)	/* Reset monitor register GPT */
#define CPG_RSTMON_POEG							(CPG_BASE + 0x09C4)	/* Reset monitor register POEG */
#define CPG_RSTMON_WDT							(CPG_BASE + 0x09C8)	/* Reset monitor register WDT */
#define CPG_RSTMON_DDR							(CPG_BASE + 0x09CC)	/* Reset monitor register DDR */
#define CPG_RSTMON_SPI							(CPG_BASE + 0x09D0)	/* Reset monitor register SPI */
#define CPG_RSTMON_SDHI							(CPG_BASE + 0x09D4)	/* Reset monitor register SDHI */
#define CPG_RSTMON_SSIF							(CPG_BASE + 0x09F0)	/* Reset monitor register SSIF */
#define CPG_RSTMON_SRC							(CPG_BASE + 0x09F4)	/* Reset monitor register SRC */
#define CPG_RSTMON_USB							(CPG_BASE + 0x09F8)	/* Reset monitor register USB */
#define CPG_RSTMON_ETH							(CPG_BASE + 0x09FC)	/* Reset monitor register ETH */
#define CPG_RSTMON_I2C							(CPG_BASE + 0x0A00)	/* Reset monitor register I2C */
#define CPG_RSTMON_SCIF							(CPG_BASE + 0x0A04)	/* Reset monitor register SCIF */
#define CPG_RSTMON_SCI							(CPG_BASE + 0x0A08)	/* Reset monitor register SCI */
#define CPG_RSTMON_IRDA							(CPG_BASE + 0x0A0C)	/* Reset monitor register IRDA */
#define CPG_RSTMON_RSPI							(CPG_BASE + 0x0A10)	/* Reset monitor register RSPI */
#define CPG_RSTMON_CANFD						(CPG_BASE + 0x0A14)	/* Reset monitor register CANFD */
#define CPG_RSTMON_GPIO							(CPG_BASE + 0x0A18)	/* Reset monitor register GPIO */
#define CPG_RSTMON_ADC							(CPG_BASE + 0x0A28)	/* Reset monitor register ADC */
#define CPG_RSTMON_TSU							(CPG_BASE + 0x0A2C)	/* Reset monitor register TSU */

#define CPG_RSTMON_AXI_ACPU_BUS					(CPG_BASE + 0x0A34)	/* Reset monitor register AXI_ACPU_BUS */
#define CPG_RSTMON_AXI_MCPU_BUS					(CPG_BASE + 0x0A38)	/* Reset monitor register AXI_MCPU_BUS */
#define CPG_RSTMON_AXI_COM_BUS					(CPG_BASE + 0x0A3C)	/* Reset monitor register AXI_COM_BUS */
#define CPG_RSTMON_PERI_COM						(CPG_BASE + 0x0A44)	/* Reset monitor register PERI_COM */
#define CPG_RSTMON_REG1_BUS						(CPG_BASE + 0x0A48)	/* Reset monitor register REG1_BUS */
#define CPG_RSTMON_REG0_BUS						(CPG_BASE + 0x0A4C)	/* Reset monitor register REG0_BUS */
#define CPG_RSTMON_PERI_CPU						(CPG_BASE + 0x0A50)	/* Reset monitor register PERI_CPU */
#define CPG_RSTMON_PERI_DDR						(CPG_BASE + 0x0A58)	/* Reset monitor register PERI_DDR */
#define CPG_RSTMON_AXI_TZCDDR					(CPG_BASE + 0x0A5C)	/* Reset monitor register AXI_TZCDDR */
#define CPG_RSTMON_MTGPGS						(CPG_BASE + 0x0A60)	/* Reset monitor register MTGPGS */

#define CPG_RSTMON_OCTA							(CPG_BASE + 0x0A74)	/* Reset monitor register OCTA */
#define CPG_RSTMON_OTFDE_DDR					(CPG_BASE + 0x0A78)	/* Reset monitor register OTFDE DDR */
#define CPG_RSTMON_OTFDE_SPI					(CPG_BASE + 0x0A80)	/* Reset monitor register OTFDE SPI */
#define CPG_RSTMON_PDM							(CPG_BASE + 0x0A84)	/* Reset monitor register PDM */
#define CPG_RSTMON_PCI							(CPG_BASE + 0x0A88)	/* Reset monitor register PCI */
#define CPG_RSTMON_SPDIF						(CPG_BASE + 0x0A8C)	/* Reset monitor register SPDIF */
#define CPG_RSTMON_I3C							(CPG_BASE + 0x0A90)	/* Reset monitor register I3C */
#define CPG_RSTMON_VBAT							(CPG_BASE + 0x0A94)	/* Reset monitor register VBAT */

#define CPG_WDTOVF_RST							(CPG_BASE + 0x0B10)	/* WDT overflow system reset register */
#define CPG_WDTRST_SEL							(CPG_BASE + 0x0B14)	/* WDT reset selector register */
#define CPG_CLUSTER_PCHMON						(CPG_BASE + 0x0B30)	/* CA55 Cluster Power Status Monitor Register */
#define CPG_CLUSTER_PCHCTL						(CPG_BASE + 0x0B34)	/* CA55 Cluster Power Status Control Register */
#define CPG_CORE0_PCHMON						(CPG_BASE + 0x0B38)	/* CA55 Core0 Power Status Monitor Register */
#define CPG_CORE0_PCHCTL						(CPG_BASE + 0x0B3C)	/* CA55 Core0 Power Status Control Register */

#define CPG_BUS_ACPU_MSTOP						(CPG_BASE + 0x0B60)	/* MSTOP registerBUS_ACPU */
#define CPG_BUS_MCPU1_MSTOP						(CPG_BASE + 0x0B64)	/* MSTOP registerBUS_MCPU1 */
#define CPG_BUS_MCPU2_MSTOP						(CPG_BASE + 0x0B68)	/* MSTOP registerBUS_MCPU2 */
#define CPG_BUS_PERI_COM_MSTOP					(CPG_BASE + 0x0B6C)	/* MSTOP registerBUS_PERI_COM */
#define CPG_BUS_PERI_CPU_MSTOP					(CPG_BASE + 0x0B70)	/* MSTOP registerBUS_PERI_CPU */
#define CPG_BUS_PERI_DDR_MSTOP					(CPG_BASE + 0x0B74)	/* MSTOP registerBUS_PERI_DDR */

#define CPG_BUS_REG0_MSTOP						(CPG_BASE + 0x0B7C)	/* MSTOP registerBUS_REG0 */
#define CPG_BUS_REG1_MSTOP						(CPG_BASE + 0x0B80)	/* MSTOP registerBUS_REG1 */
#define CPG_BUS_TZCDDR_MSTOP					(CPG_BASE + 0x0B84)	/* MSTOP registerBUS_TZCDDR */
#define CPG_MHU_MSTOP							(CPG_BASE + 0x0B88)	/* MSTOP registerMHU */

#define CPG_BUS_MCPU3_MSTOP						(CPG_BASE + 0x0B90)
#define CPG_BUS_PERI_CPU2_MSTOP					(CPG_BASE + 0x0B94)

#define CPG_PWRDN_IP1							(CPG_BASE + 0x0BB0)
#define CPG_PWRDN_IP2							(CPG_BASE + 0x0BB4)
#define CPG_PWRDN_MSTOP							(CPG_BASE + 0x0BC0)
#define CPG_PWRDN_CLKON							(CPG_BASE + 0x0BC4)
#define CPG_PWRDN_RST							(CPG_BASE + 0x0BC8)
#define CPG_OTHERFUNC2_REG						(CPG_BASE + 0x0BEC)	/* Other function registers2 */


#define PLL1_STBY_RESETB						(1 << 0)
#define PLL1_STBY_SSCG_EN						(1 << 2)
#define PLL1_STBY_SSCG_MODE						(1 << 4)
#define PLL1_STBY_RESETB_WEN					(1 << 16)
#define PLL1_STBY_SSCG_EN_WEN					(1 << 18)
#define PLL1_STBY_SSCG_MODE_WEN					(1 << 20)
#define PLL1_CLK1_RANGESEL						(1 << 0)
#define PLL1_CLK1_DIV_P_RATIO1					(0 << 26)
#define PLL1_CLK1_DIV_P_RATIO2					(1 << 26)
#define PLL1_CLK1_DIV_P_RATIO4					(2 << 26)
#define PLL1_CLK1_DIV_P_RATIO8					(3 << 26)
#define PLL1_CLK1_DIV_P_RATIO16					(4 << 26)
#define PLL4_STBY_RESETB						(1 << 0)
#define PLL4_STBY_SSCG_EN						(1 << 2)
#define PLL4_STBY_SSCG_MODE						(1 << 4)
#define PLL4_STBY_RESETB_WEN					(1 << 16)
#define PLL4_STBY_SSCG_EN_WEN					(1 << 18)
#define PLL4_STBY_SSCG_MODE_WEN					(1 << 20)
#define PLL4_CLK1_RANGESEL						(1 << 0)
#define PLL4_CLK1_DIV_P_RATIO1					(0 << 26)
#define PLL4_CLK1_DIV_P_RATIO2					(1 << 26)
#define PLL4_CLK1_DIV_P_RATIO4					(2 << 26)
#define PLL4_CLK1_DIV_P_RATIO8					(3 << 26)
#define PLL4_CLK1_DIV_P_RATIO16					(4 << 26)
#define PLL6_STBY_RESETB						(1 << 0)
#define PLL6_STBY_SSCG_EN						(1 << 2)
#define PLL6_STBY_SSCG_MODE						(1 << 4)
#define PLL6_STBY_RESETB_WEN					(1 << 16)
#define PLL6_STBY_SSCG_EN_WEN					(1 << 18)
#define PLL6_STBY_SSCG_MODE_WEN					(1 << 20)
#define PLL6_CLK1_RANGESEL						(1 << 0)
#define PLL6_CLK1_DIV_P_RATIO1					(0 << 26)
#define PLL6_CLK1_DIV_P_RATIO2					(1 << 26)
#define PLL6_CLK1_DIV_P_RATIO4					(2 << 26)
#define PLL6_CLK1_DIV_P_RATIO8					(3 << 26)
#define PLL6_CLK1_DIV_P_RATIO16					(4 << 26)
#define PLL_CLK1_DIV_NF_SET(a, b)				(mmio_clrsetbits_32(a, 0xFFF << 1, (b) << 1))
#define PLL_CLK1_DIV_NI_SET(a, b)				(mmio_clrsetbits_32(a, 0x1FF << 13, (b) << 13))
#define PLL_CLK1_DIV_M_SET(a, b)				(mmio_clrsetbits_32(a, 0xF << 22, (b) << 22))
#define PLL_CLK2_SELMPERCENT_SET(a, b)			(mmio_clrsetbits_32(a, 0x7 << 0, (b) << 0))
#define PLL_CLK2_SELMFREQ_SET(a, b)				(mmio_clrsetbits_32(a, 0x1F << 3, (b) << 3))

#define PLL1_MON_PLL1_RESETB					(1 << 0)
#define PLL1_MON_PLL1_LOCK						(1 << 4)
#define PLL2_MON_PLL2_RESETB					(1 << 0)
#define PLL2_MON_PLL2_LOCK						(1 << 4)
#define PLL3_MON_PLL3_RESETB					(1 << 0)
#define PLL3_MON_PLL3_LOCK						(1 << 4)
#define PLL4_MON_PLL4_RESETB					(1 << 0)
#define PLL4_MON_PLL4_LOCK						(1 << 4)
#define PLL6_MON_PLL6_RESETB					(1 << 0)
#define PLL6_MON_PLL6_LOCK						(1 << 4)

#define PLL1_SETTING_SEL_PLL1					(1 << 0)
#define PLL1_SETTING_SEL_PLL1_WEN				(1 << 16)

#define PL1_DDIV_DIVPL1_SET_1_1					(0 << 0)
#define PL1_DDIV_DIVPL1_SET_1_2					(1 << 0)
#define PL1_DDIV_DIVPL1_SET_1_4					(2 << 0)
#define PL1_DDIV_DIVPL1_SET_1_8					(3 << 0)
#define PL1_DDIV_DIV_PLL1SET_WEN					(1 << 16)
#define PL2_DDIV_DIVPL2B_SET_1_1				(0 << 4)
#define PL2_DDIV_DIVPL2B_SET_1_2				(1 << 4)
#define PL2_DDIV_DIVPL2B_SET_1_4				(2 << 4)
#define PL2_DDIV_DIVPL2B_SET_1_8				(3 << 4)
#define PL2_DDIV_DIVPL2B_SET_1_32				(4 << 4)
#define PL2_DDIV_DIV_PLL2_B_WEN					(1 << 20)
#define PL3_DDIV_DIVPL3A_SET_1_1				(0 << 0)
#define PL3_DDIV_DIVPL3A_SET_1_2				(1 << 0)
#define PL3_DDIV_DIVPL3A_SET_1_4				(2 << 0)
#define PL3_DDIV_DIVPL3A_SET_1_8				(3 << 0)
#define PL3_DDIV_DIVPL3A_SET_1_32				(4 << 0)
#define PL3_DDIV_DIVPL3B_SET_1_1				(0 << 4)
#define PL3_DDIV_DIVPL3B_SET_1_2				(1 << 4)
#define PL3_DDIV_DIVPL3B_SET_1_4				(2 << 4)
#define PL3_DDIV_DIVPL3B_SET_1_8				(3 << 4)
#define PL3_DDIV_DIVPL3B_SET_1_32				(4 << 4)
#define PL3_DDIV_DIVPL3C_SET_1_1				(0 << 8)
#define PL3_DDIV_DIVPL3C_SET_1_2				(1 << 8)
#define PL3_DDIV_DIVPL3C_SET_1_4				(2 << 8)
#define PL3_DDIV_DIVPL3C_SET_1_8				(3 << 8)
#define PL3_DDIV_DIVPL3C_SET_1_32				(4 << 8)
#define PL3_DDIV_DIV_PLL3_A_WEN					(1 << 16)
#define PL3_DDIV_DIV_PLL3_B_WEN					(1 << 20)
#define PL3_DDIV_DIV_PLL3_C_WEN					(1 << 24)
#define PL6_DDIV_DIVPL6A_SET_1_1				(0 << 0)
#define PL6_DDIV_DIVPL6A_SET_1_2				(1 << 0)
#define PL6_DDIV_DIVPL6A_SET_1_4				(2 << 0)
#define PL6_DDIV_DIVPL6A_SET_1_8				(3 << 0)
#define PL6_DDIV_DIVPL6A_SET_1_32				(4 << 0)
#define PL6_DDIV_DIVPL6B_SET_1_1				(0 << 4)
#define PL6_DDIV_DIVPL6B_SET_1_2				(1 << 4)
#define PL6_DDIV_DIVPL6B_SET_1_4				(2 << 4)
#define PL6_DDIV_DIVPL6B_SET_1_8				(3 << 4)
#define PL6_DDIV_DIVPL6B_SET_1_32				(4 << 4)
#define PL6_DDIV_DIV_PLL6_A_WEN					(1 << 16)
#define PL6_DDIV_DIV_PLL6_B_WEN					(1 << 20)

#define SDHI_DDIV_DIVSDHI0_SET_1_1				(0 << 0)
#define SDHI_DDIV_DIVSDHI0_SET_1_2				(1 << 0)
#define SDHI_DDIV_DIVSDHI1_SET_1_1				(0 << 4)
#define SDHI_DDIV_DIVSDHI1_SET_1_2				(1 << 4)
#define SDHI_DDIV_DIVSDHI2_SET_1_1				(0 << 8)
#define SDHI_DDIV_DIVSDHI2_SET_1_2				(1 << 8)
#define SDHI_DDIV_DIVSDHI0_WEN					(1 << 16)
#define SDHI_DDIV_DIVSDHI1_WEN					(1 << 20)
#define SDHI_DDIV_DIVSDHI2_WEN					(1 << 24)
#define OCTA_DDIV_DIVOCTA_SET_1_1				(0 << 0)
#define OCTA_DDIV_DIVOCTA_SET_1_2				(1 << 0)
#define OCTA_DDIV_DIVOCTA_SET_1_4				(2 << 0)
#define OCTA_DDIV_DIVOCTA_SET_1_8				(3 << 0)
#define OCTA_DDIV_DIVOCTA_SET_1_32				(4 << 0)
#define OCTA_DDIV_DIVOCTA_WEN					(1 << 16)
#define SPI_DDIV_DIVSPI_SET_1_1					(0 << 0)
#define SPI_DDIV_DIVSPI_SET_1_2					(1 << 0)
#define SPI_DDIV_DIVSPI_SET_1_4					(2 << 0)
#define SPI_DDIV_DIVSPI_SET_1_8					(3 << 0)
#define SPI_DDIV_DIVSPI_SET_1_32				(4 << 0)
#define SPI_DDIV_DIVSPI_WEN						(1 << 16)

#define PLL_DSEL_SELPL4_SET						(1 << 6)
#define PLL_DSEL_SELPL4_WEN						(1 << 22)
#define SDHI_DSEL_SEL_SDHI0_SET_800M			(0 << 0)
#define SDHI_DSEL_SEL_SDHI0_SET_500M			(2 << 0)
#define SDHI_DSEL_SEL_SDHI0_SET_266M			(3 << 0)
#define SDHI_DSEL_SEL_SDHI1_SET_800M			(0 << 4)
#define SDHI_DSEL_SEL_SDHI1_SET_500M			(2 << 4)
#define SDHI_DSEL_SEL_SDHI1_SET_266M			(3 << 4)
#define SDHI_DSEL_SEL_SDHI2_SET_800M			(0 << 8)
#define SDHI_DSEL_SEL_SDHI2_SET_500M			(2 << 8)
#define SDHI_DSEL_SEL_SDHI2_SET_266M			(3 << 8)
#define SDHI_DSEL_SEL_SDHI0_WEN					(1 << 16)
#define SDHI_DSEL_SEL_SDHI1_WEN					(1 << 20)
#define SDHI_DSEL_SEL_SDHI2_WEN					(1 << 24)

#define CLKDIVSTATUS_DIVPL1_STS					(1 << 0)
#define CLKDIVSTATUS_DIVPL2B_STS				(1 << 5)
#define CLKDIVSTATUS_DIVPL3A_STS				(1 << 8)
#define CLKDIVSTATUS_DIVPL3B_STS				(1 << 9)
#define CLKDIVSTATUS_DIVPL3C_STS				(1 << 10)
#define CLKDIVSTATUS_DIVPL6A_STS				(1 << 20)
#define CLKDIVSTATUS_DIVPL6B_STS				(1 << 21)
#define CLKDIVSTATUS_DIVSDHI0_STS				(1 << 24)
#define CLKDIVSTATUS_DIVSDHI1_STS				(1 << 25)
#define CLKDIVSTATUS_DIVSDHI2_STS				(1 << 26)
#define CLKDIVSTATUS_DIVOCTA_STS				(1 << 28)
#define CLKDIVSTATUS_DIVSPI_STS					(1 << 29)

#define CLKSELSTATUS_SELPL1_STS					(1 << 0)
#define CLKSELSTATUS_SELPL2_STS					(1 << 2)
#define CLKSELSTATUS_SELPL3_STS					(1 << 4)
#define CLKSELSTATUS_SELPL4_STS					(1 << 6)
#define CLKSELSTATUS_SELPL6_STS					(1 << 10)
#define CLKSELSTATUS_SELSDHI0_STS				(1 << 16)
#define CLKSELSTATUS_SELSDHI1_STS				(1 << 17)
#define CLKSELSTATUS_SELSDHI2_STS				(1 << 18)

#define CPG_OCTA_SSEL_SELOCTA_SET_400M			(0 << 0)
#define CPG_OCTA_SSEL_SELOCTA_SET_266M			(2 << 0)
#define CPG_OCTA_SSEL_SELOCTA_SET_250M			(3 << 0)
#define CPG_OCTA_SSEL_SELOCTA_WEN				(1 << 16)
#define CPG_SPI_SSEL_SELSPI_SET_400M			(0 << 0)
#define CPG_SPI_SSEL_SELSPI_SET_266M			(2 << 0)
#define CPG_SPI_SSEL_SELSPI_SET_250M			(3 << 0)
#define CPG_SPI_SSEL_SELSPI_WEN					(1 << 16)

#define CLKON_CLK0_ON							(1 << 0)
#define CLKON_CLK0_ONWEN						(1 << 16)
#define CLKON_CLK1_ON							(1 << 1)
#define CLKON_CLK1_ONWEN						(1 << 17)
#define CLKON_CLK2_ON							(1 << 2)
#define CLKON_CLK2_ONWEN						(1 << 18)
#define CLKON_CLK3_ON							(1 << 3)
#define CLKON_CLK3_ONWEN						(1 << 19)
#define CLKON_CLK4_ON							(1 << 4)
#define CLKON_CLK4_ONWEN						(1 << 20)
#define CLKON_CLK5_ON							(1 << 5)
#define CLKON_CLK5_ONWEN						(1 << 21)
#define CLKON_CLK6_ON							(1 << 6)
#define CLKON_CLK6_ONWEN						(1 << 22)
#define CLKON_CLK7_ON							(1 << 7)
#define CLKON_CLK7_ONWEN						(1 << 23)
#define CLKON_CLK8_ON							(1 << 8)
#define CLKON_CLK8_ONWEN						(1 << 24)
#define CLKON_CLK9_ON							(1 << 9)
#define CLKON_CLK9_ONWEN						(1 << 25)
#define CLKON_CLK10_ON							(1 << 10)
#define CLKON_CLK10_ONWEN						(1 << 26)

#define CLKMON_CLK0_MON							(1 << 0)
#define CLKMON_CLK1_MON							(1 << 1)
#define CLKMON_CLK2_MON							(1 << 2)
#define CLKMON_CLK3_MON							(1 << 3)
#define CLKMON_CLK4_MON							(1 << 4)
#define CLKMON_CLK5_MON							(1 << 5)
#define CLKMON_CLK6_MON							(1 << 6)
#define CLKMON_CLK7_MON							(1 << 7)
#define CLKMON_CLK8_MON							(1 << 8)
#define CLKMON_CLK9_MON							(1 << 9)
#define CLKMON_CLK10_MON						(1 << 10)

#define RST_UNIT0_RSTB							(1 << 0)
#define RST_UNIT0_RSTWEN						(1 << 16)
#define RST_UNIT1_RSTB							(1 << 1)
#define RST_UNIT1_RSTWEN						(1 << 17)
#define RST_UNIT2_RSTB							(1 << 2)
#define RST_UNIT2_RSTWEN						(1 << 18)
#define RST_UNIT3_RSTB							(1 << 3)
#define RST_UNIT3_RSTWEN						(1 << 19)
#define RST_UNIT4_RSTB							(1 << 4)
#define RST_UNIT4_RSTWEN						(1 << 20)
#define RST_UNIT5_RSTB							(1 << 5)
#define RST_UNIT5_RSTWEN						(1 << 21)
#define RST_UNIT6_RSTB							(1 << 6)
#define RST_UNIT6_RSTWEN						(1 << 22)
#define RST_UNIT7_RSTB							(1 << 7)
#define RST_UNIT7_RSTWEN						(1 << 23)
#define RST_UNIT8_RSTB							(1 << 8)
#define RST_UNIT8_RSTWEN						(1 << 24)
#define RST_UNIT9_RSTB							(1 << 9)
#define RST_UNIT9_RSTWEN						(1 << 25)
#define RST_UNIT10_RSTB							(1 << 10)
#define RST_UNIT10_RSTWEN						(1 << 26)
#define RST_UNIT11_RSTB							(1 << 11)
#define RST_UNIT11_RSTWEN						(1 << 27)
#define RST_UNIT12_RSTB							(1 << 12)
#define RST_UNIT12_RSTWEN						(1 << 28)

#define RSTMON_RST0_MON							(0 << 0)
#define RSTMON_RST1_MON							(1 << 0)
#define RSTMON_RST2_MON							(2 << 0)
#define RSTMON_RST3_MON							(3 << 0)
#define RSTMON_RST4_MON							(4 << 0)
#define RSTMON_RST5_MON							(5 << 0)
#define RSTMON_RST6_MON							(6 << 0)
#define RSTMON_RST7_MON							(7 << 0)
#define RSTMON_RST8_MON							(8 << 0)
#define RSTMON_RST9_MON							(9 << 0)
#define RSTMON_RST10_MON						(10 << 0)
#define RSTMON_RST11_MON						(11 << 0)
#define RSTMON_RST12_MON						(12 << 0)

#define WDTOVF_RST_WDTOVF0						(1 << 0)
#define WDTOVF_RST_WDTOVF1						(1 << 1)
#define WDTOVF_RST_WDTOVF2						(1 << 2)
#define WDTOVF_RST_WDTOVF0_WEN					(1 << 16)
#define WDTOVF_RST_WDTOVF1_WEN					(1 << 17)
#define WDTOVF_RST_WDTOVF2_WEN					(1 << 18)

#define WDTRST_SEL_WDTRSTSEL0					(1 << 0)
#define WDTRST_SEL_WDTRSTSEL1					(1 << 1)
#define WDTRST_SEL_WDTRSTSEL2					(1 << 2)
#define WDTRST_SEL_WDTRSTSEL4					(1 << 4)
#define WDTRST_SEL_WDTRSTSEL5					(1 << 5)
#define WDTRST_SEL_WDTRSTSEL6					(1 << 6)
#define WDTRST_SEL_WDTRSTSEL8					(1 << 8)
#define WDTRST_SEL_WDTRSTSEL9					(1 << 9)
#define WDTRST_SEL_WDTRSTSEL10					(1 << 10)
#define WDTRST_SEL_WDTRSTSEL0_WEN				(1 << 16)
#define WDTRST_SEL_WDTRSTSEL1_WEN				(1 << 17)
#define WDTRST_SEL_WDTRSTSEL2_WEN				(1 << 18)
#define WDTRST_SEL_WDTRSTSEL4_WEN				(1 << 20)
#define WDTRST_SEL_WDTRSTSEL5_WEN				(1 << 21)
#define WDTRST_SEL_WDTRSTSEL6_WEN				(1 << 22)
#define WDTRST_SEL_WDTRSTSEL8_WEN				(1 << 24)
#define WDTRST_SEL_WDTRSTSEL9_WEN				(1 << 25)
#define WDTRST_SEL_WDTRSTSEL10_WEN				(1 << 26)

#define CLUSTER_PCHMON_PACCEPT_MON				(1 << 0)
#define CLUSTER_PCHMON_PDENY_MON				(1 << 1)
#define CLUSTER_PCHCTL_PREQ_SET					(1 << 0)
#define CLUSTER_PCHCTL_PSTATE0_SET_ON			(0x48 << 16)
#define CLUSTER_PCHCTL_PSTATE0_SET_OFF			(0x00 << 16)
#define CORE0_PCHMON_PACCEPT0_MON				(1 << 0)
#define CORE0_PCHMON_PDENY0_MON					(1 << 1)
#define CORE0_PCHCTL_PREQ0_SET					(1 << 0)
#define CORE0_PCHCTL_PSTATE0_SET_ON				(0x08 << 16)
#define CORE0_PCHCTL_PSTATE0_SET_OFF_EMULATED	(0x01 << 16)
#define CORE0_PCHCTL_PSTATE0_SET_OFF			(0x00 << 16)

#define BUS_MSTOP_MSTOP0_ON						(1 << 0)
#define BUS_MSTOP_MSTOP0_ON_WEN					(1 << 16)
#define BUS_MSTOP_MSTOP1_ON						(1 << 1)
#define BUS_MSTOP_MSTOP1_ON_WEN					(1 << 17)
#define BUS_MSTOP_MSTOP2_ON						(1 << 2)
#define BUS_MSTOP_MSTOP2_ON_WEN					(1 << 18)
#define BUS_MSTOP_MSTOP3_ON						(1 << 3)
#define BUS_MSTOP_MSTOP3_ON_WEN					(1 << 19)
#define BUS_MSTOP_MSTOP4_ON						(1 << 4)
#define BUS_MSTOP_MSTOP4_ON_WEN					(1 << 20)
#define BUS_MSTOP_MSTOP5_ON						(1 << 5)
#define BUS_MSTOP_MSTOP5_ON_WEN					(1 << 21)
#define BUS_MSTOP_MSTOP6_ON						(1 << 6)
#define BUS_MSTOP_MSTOP6_ON_WEN					(1 << 22)
#define BUS_MSTOP_MSTOP7_ON						(1 << 7)
#define BUS_MSTOP_MSTOP7_ON_WEN					(1 << 23)
#define BUS_MSTOP_MSTOP8_ON						(1 << 8)
#define BUS_MSTOP_MSTOP8_ON_WEN					(1 << 24)
#define BUS_MSTOP_MSTOP9_ON						(1 << 9)
#define BUS_MSTOP_MSTOP9_ON_WEN					(1 << 25)
#define BUS_MSTOP_MSTOP10_ON					(1 << 10)
#define BUS_MSTOP_MSTOP10_ON_WEN				(1 << 26)
#define BUS_MSTOP_MSTOP11_ON					(1 << 11)
#define BUS_MSTOP_MSTOP11_ON_WEN				(1 << 27)
#define BUS_MSTOP_MSTOP12_ON					(1 << 12)
#define BUS_MSTOP_MSTOP12_ON_WEN				(1 << 28)
#define BUS_MSTOP_MSTOP13_ON					(1 << 13)
#define BUS_MSTOP_MSTOP13_ON_WEN				(1 << 29)
#define BUS_MSTOP_MSTOP14_ON					(1 << 14)
#define BUS_MSTOP_MSTOP14_ON_WEN				(1 << 30)
#define BUS_MSTOP_MSTOP15_ON					(1 << 15)
#define BUS_MSTOP_MSTOP15_ON_WEN				(1 << 31)

#define PWRDN_IP_PWRDN0_ON						(1 << 0)
#define PWRDN_IP_PWRDN0_ON_WEN					(1 << 16)
#define PWRDN_IP_PWRDN1_ON						(1 << 1)
#define PWRDN_IP_PWRDN1_ON_WEN					(1 << 17)
#define PWRDN_IP_PWRDN2_ON						(1 << 2)
#define PWRDN_IP_PWRDN2_ON_WEN					(1 << 18)
#define PWRDN_IP_PWRDN3_ON						(1 << 3)
#define PWRDN_IP_PWRDN3_ON_WEN					(1 << 19)
#define PWRDN_IP_PWRDN4_ON						(1 << 4)
#define PWRDN_IP_PWRDN4_ON_WEN					(1 << 20)
#define PWRDN_IP_PWRDN5_ON						(1 << 5)
#define PWRDN_IP_PWRDN5_ON_WEN					(1 << 21)
#define PWRDN_IP_PWRDN6_ON						(1 << 6)
#define PWRDN_IP_PWRDN6_ON_WEN					(1 << 22)
#define PWRDN_IP_PWRDN7_ON						(1 << 7)
#define PWRDN_IP_PWRDN7_ON_WEN					(1 << 23)
#define PWRDN_IP_PWRDN8_ON						(1 << 8)
#define PWRDN_IP_PWRDN8_ON_WEN					(1 << 24)
#define PWRDN_IP_PWRDN9_ON						(1 << 9)
#define PWRDN_IP_PWRDN9_ON_WEN					(1 << 25)
#define PWRDN_IP_PWRDN10_ON						(1 << 10)
#define PWRDN_IP_PWRDN10_ON_WEN					(1 << 26)
#define PWRDN_IP_PWRDN11_ON						(1 << 11)
#define PWRDN_IP_PWRDN11_ON_WEN					(1 << 27)
#define PWRDN_IP_PWRDN12_ON						(1 << 12)
#define PWRDN_IP_PWRDN12_ON_WEN					(1 << 28)
#define PWRDN_IP_PWRDN13_ON						(1 << 13)
#define PWRDN_IP_PWRDN13_ON_WEN					(1 << 29)
#define PWRDN_IP_PWRDN14_ON						(1 << 14)
#define PWRDN_IP_PWRDN14_ON_WEN					(1 << 30)
#define PWRDN_IP_PWRDN15_ON						(1 << 15)
#define PWRDN_IP_PWRDN15_ON_WEN					(1 << 31)

#define PWRDN_MSTOP_PWRDN_MSTOP					(1 << 0)
#define PWRDN_MSTOP_PWRDN_MSTOP_WEN				(1 << 16)
#define PWRDN_CLKON_PWRDN_CLKON					(1 << 0)
#define PWRDN_CLKON_PWRDN_CLKON_WEN				(1 << 16)
#define PWRDN_RST_PWRDN_RST						(1 << 0)
#define PWRDN_RST_PWRDN_RST_WEN					(1 << 16)

#define OTHERFUNC2_REG_RES0_SET					(1 << 0)
#define OTHERFUNC2_REG_RES0_ON_WEN				(1 << 16)

#endif	/* __CPG_REGS_H__ */
