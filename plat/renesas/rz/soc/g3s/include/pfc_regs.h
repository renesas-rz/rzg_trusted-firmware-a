/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __PFC_REGS_H__
#define __PFC_REGS_H__

#include <rz_soc_def.h>					/* Get the PFC base address */

#define PFC_P20							(PFC_BASE + 0x0020)	/* Port register */
#define PFC_P21							(PFC_BASE + 0x0021)	/* Port register */
#define PFC_P22							(PFC_BASE + 0x0022)	/* Port register */
#define PFC_P23							(PFC_BASE + 0x0023)	/* Port register */
#define PFC_P24							(PFC_BASE + 0x0024)	/* Port register */
#define PFC_P25							(PFC_BASE + 0x0025)	/* Port register */
#define PFC_P26							(PFC_BASE + 0x0026)	/* Port register */
#define PFC_P27							(PFC_BASE + 0x0027)	/* Port register */
#define PFC_P28							(PFC_BASE + 0x0028)	/* Port register */
#define PFC_P29							(PFC_BASE + 0x0029)	/* Port register */
#define PFC_P2A							(PFC_BASE + 0x002A)	/* Port register */
#define PFC_P30							(PFC_BASE + 0x0030)	/* Port register */
#define PFC_P31							(PFC_BASE + 0x0031)	/* Port register */
#define PFC_P32							(PFC_BASE + 0x0032)	/* Port register */
#define PFC_P33							(PFC_BASE + 0x0033)	/* Port register */
#define PFC_P34							(PFC_BASE + 0x0034)	/* Port register */
#define PFC_P35							(PFC_BASE + 0x0035)	/* Port register */
#define PFC_P36							(PFC_BASE + 0x0036)	/* Port register */
#define PFC_P37							(PFC_BASE + 0x0037)	/* Port register */

#define PFC_PM20						(PFC_BASE + 0x0140)	/* Port mode register */
#define PFC_PM21						(PFC_BASE + 0x0142)	/* Port mode register */
#define PFC_PM22						(PFC_BASE + 0x0144)	/* Port mode register */
#define PFC_PM23						(PFC_BASE + 0x0146)	/* Port mode register */
#define PFC_PM24						(PFC_BASE + 0x0148)	/* Port mode register */
#define PFC_PM25						(PFC_BASE + 0x014A)	/* Port mode register */
#define PFC_PM26						(PFC_BASE + 0x014C)	/* Port mode register */
#define PFC_PM27						(PFC_BASE + 0x014E)	/* Port mode register */
#define PFC_PM28						(PFC_BASE + 0x0150)	/* Port mode register */
#define PFC_PM29						(PFC_BASE + 0x0152)	/* Port mode register */
#define PFC_PM2A						(PFC_BASE + 0x0154)	/* Port mode register */
#define PFC_PM30						(PFC_BASE + 0x0160)	/* Port mode register */
#define PFC_PM31						(PFC_BASE + 0x0162)	/* Port mode register */
#define PFC_PM32						(PFC_BASE + 0x0164)	/* Port mode register */
#define PFC_PM33						(PFC_BASE + 0x0166)	/* Port mode register */
#define PFC_PM34						(PFC_BASE + 0x0168)	/* Port mode register */
#define PFC_PM35						(PFC_BASE + 0x016A)	/* Port mode register */
#define PFC_PM36						(PFC_BASE + 0x016C)	/* Port mode register */
#define PFC_PM37						(PFC_BASE + 0x016E)	/* Port mode register */

#define PFC_PMC20						(PFC_BASE + 0x0220)	/* Port mode control register */
#define PFC_PMC21						(PFC_BASE + 0x0221)	/* Port mode control register */
#define PFC_PMC22						(PFC_BASE + 0x0222)	/* Port mode control register */
#define PFC_PMC23						(PFC_BASE + 0x0223)	/* Port mode control register */
#define PFC_PMC24						(PFC_BASE + 0x0224)	/* Port mode control register */
#define PFC_PMC25						(PFC_BASE + 0x0225)	/* Port mode control register */
#define PFC_PMC26						(PFC_BASE + 0x0226)	/* Port mode control register */
#define PFC_PMC27						(PFC_BASE + 0x0227)	/* Port mode control register */
#define PFC_PMC28						(PFC_BASE + 0x0228)	/* Port mode control register */
#define PFC_PMC29						(PFC_BASE + 0x0229)	/* Port mode control register */
#define PFC_PMC2A						(PFC_BASE + 0x022A)	/* Port mode control register */
#define PFC_PMC30						(PFC_BASE + 0x0230)	/* Port mode control register */
#define PFC_PMC31						(PFC_BASE + 0x0231)	/* Port mode control register */
#define PFC_PMC32						(PFC_BASE + 0x0232)	/* Port mode control register */
#define PFC_PMC33						(PFC_BASE + 0x0233)	/* Port mode control register */
#define PFC_PMC34						(PFC_BASE + 0x0234)	/* Port mode control register */
#define PFC_PMC35						(PFC_BASE + 0x0235)	/* Port mode control register */
#define PFC_PMC36						(PFC_BASE + 0x0236)	/* Port mode control register */
#define PFC_PMC37						(PFC_BASE + 0x0237)	/* Port mode control register */
#define PFC_PFC20						(PFC_BASE + 0x0480)	/* Port function control register */
#define PFC_PFC21						(PFC_BASE + 0x0484)	/* Port function control register */
#define PFC_PFC22						(PFC_BASE + 0x0488)	/* Port function control register */
#define PFC_PFC23						(PFC_BASE + 0x048C)	/* Port function control register */
#define PFC_PFC24						(PFC_BASE + 0x0490)	/* Port function control register */
#define PFC_PFC25						(PFC_BASE + 0x0494)	/* Port function control register */
#define PFC_PFC26						(PFC_BASE + 0x0498)	/* Port function control register */
#define PFC_PFC27						(PFC_BASE + 0x049C)	/* Port function control register */
#define PFC_PFC28						(PFC_BASE + 0x04A0)	/* Port function control register */
#define PFC_PFC29						(PFC_BASE + 0x04A4)	/* Port function control register */
#define PFC_PFC2A						(PFC_BASE + 0x04A8)	/* Port function control register */
#define PFC_PFC30						(PFC_BASE + 0x04C0)	/* Port function control register */
#define PFC_PFC31						(PFC_BASE + 0x04C4)	/* Port function control register */
#define PFC_PFC32						(PFC_BASE + 0x04C8)	/* Port function control register */
#define PFC_PFC33						(PFC_BASE + 0x04CC)	/* Port function control register */
#define PFC_PFC34						(PFC_BASE + 0x04D0)	/* Port function control register */
#define PFC_PFC35						(PFC_BASE + 0x04D4)	/* Port function control register */
#define PFC_PFC36						(PFC_BASE + 0x04D8)	/* Port function control register */
#define PFC_PFC37						(PFC_BASE + 0x04DC)	/* Port function control register */

#define PFC_PIN20						(PFC_BASE + 0x0820) /* Port Input Register */
#define PFC_PIN21						(PFC_BASE + 0x0821) /* Port Input Register */
#define PFC_PIN22						(PFC_BASE + 0x0822) /* Port Input Register */
#define PFC_PIN23						(PFC_BASE + 0x0823) /* Port Input Register */
#define PFC_PIN24						(PFC_BASE + 0x0824) /* Port Input Register */
#define PFC_PIN25						(PFC_BASE + 0x0825) /* Port Input Register */
#define PFC_PIN26						(PFC_BASE + 0x0826) /* Port Input Register */
#define PFC_PIN27						(PFC_BASE + 0x0827) /* Port Input Register */
#define PFC_PIN28						(PFC_BASE + 0x0828) /* Port Input Register */
#define PFC_PIN29						(PFC_BASE + 0x0829) /* Port Input Register */
#define PFC_PIN2A						(PFC_BASE + 0x082A) /* Port Input Register */
#define PFC_PIN30						(PFC_BASE + 0x0830) /* Port Input Register */
#define PFC_PIN31						(PFC_BASE + 0x0831) /* Port Input Register */
#define PFC_PIN32						(PFC_BASE + 0x0832) /* Port Input Register */
#define PFC_PIN33						(PFC_BASE + 0x0833) /* Port Input Register */
#define PFC_PIN34						(PFC_BASE + 0x0834) /* Port Input Register */
#define PFC_PIN35						(PFC_BASE + 0x0835) /* Port Input Register */
#define PFC_PIN36						(PFC_BASE + 0x0836) /* Port Input Register */
#define PFC_PIN37						(PFC_BASE + 0x0837) /* Port Input Register */

#define PFC_ISEL20						(PFC_BASE + 0x2D00) /* Interrupt Enable Control Register */
#define PFC_ISEL21						(PFC_BASE + 0x2D08) /* Interrupt Enable Control Register */
#define PFC_ISEL22						(PFC_BASE + 0x2D10) /* Interrupt Enable Control Register */
#define PFC_ISEL23						(PFC_BASE + 0x2D18) /* Interrupt Enable Control Register */
#define PFC_ISEL24						(PFC_BASE + 0x2D20) /* Interrupt Enable Control Register */
#define PFC_ISEL25						(PFC_BASE + 0x2D28) /* Interrupt Enable Control Register */
#define PFC_ISEL26						(PFC_BASE + 0x2D30) /* Interrupt Enable Control Register */
#define PFC_ISEL27						(PFC_BASE + 0x2D38) /* Interrupt Enable Control Register */
#define PFC_ISEL28						(PFC_BASE + 0x2D40) /* Interrupt Enable Control Register */
#define PFC_ISEL29						(PFC_BASE + 0x2D48) /* Interrupt Enable Control Register */
#define PFC_ISEL2A						(PFC_BASE + 0x2D50) /* Interrupt Enable Control Register */
#define PFC_ISEL30						(PFC_BASE + 0x2D80) /* Interrupt Enable Control Register */
#define PFC_ISEL31						(PFC_BASE + 0x2D88) /* Interrupt Enable Control Register */
#define PFC_ISEL32						(PFC_BASE + 0x2D90) /* Interrupt Enable Control Register */
#define PFC_ISEL33						(PFC_BASE + 0x2D98) /* Interrupt Enable Control Register */
#define PFC_ISEL34						(PFC_BASE + 0x2DA0) /* Interrupt Enable Control Register */
#define PFC_ISEL35						(PFC_BASE + 0x2DA8) /* Interrupt Enable Control Register */
#define PFC_ISEL36						(PFC_BASE + 0x2DB0) /* Interrupt Enable Control Register */
#define PFC_ISEL37						(PFC_BASE + 0x2DB8) /* Interrupt Enable Control Register */

#define PFC_IOLH01						(PFC_BASE + 0x1008)	/* IOLH switch register */
#define PFC_IOLH04						(PFC_BASE + 0x1020)	/* IOLH switch register */
#define PFC_IOLH05						(PFC_BASE + 0x1028)	/* IOLH switch register */
#define PFC_IOLH06						(PFC_BASE + 0x1030)	/* IOLH switch register */
#define PFC_IOLH10						(PFC_BASE + 0x1080)	/* IOLH switch register */
#define PFC_IOLH11						(PFC_BASE + 0x1088)	/* IOLH switch register */
#define PFC_IOLH12						(PFC_BASE + 0x1090)	/* IOLH switch register */
#define PFC_IOLH13						(PFC_BASE + 0x1098)	/* IOLH switch register */
#define PFC_IOLH20						(PFC_BASE + 0x1100)	/* IOLH switch register */
#define PFC_IOLH21						(PFC_BASE + 0x1108)	/* IOLH switch register */
#define PFC_IOLH22						(PFC_BASE + 0x1110)	/* IOLH switch register */
#define PFC_IOLH23						(PFC_BASE + 0x1118)	/* IOLH switch register */
#define PFC_IOLH24						(PFC_BASE + 0x1120)	/* IOLH switch register */
#define PFC_IOLH25						(PFC_BASE + 0x1128)	/* IOLH switch register */
#define PFC_IOLH26						(PFC_BASE + 0x1130)	/* IOLH switch register */
#define PFC_IOLH27						(PFC_BASE + 0x1138)	/* IOLH switch register */
#define PFC_IOLH28						(PFC_BASE + 0x1140)	/* IOLH switch register */
#define PFC_IOLH29						(PFC_BASE + 0x1148)	/* IOLH switch register */
#define PFC_IOLH2A						(PFC_BASE + 0x1150)	/* IOLH switch register */
#define PFC_IOLH30						(PFC_BASE + 0x1180)	/* IOLH switch register */
#define PFC_IOLH31						(PFC_BASE + 0x1188)	/* IOLH switch register */
#define PFC_IOLH32						(PFC_BASE + 0x1190)	/* IOLH switch register */
#define PFC_IOLH33						(PFC_BASE + 0x1198)	/* IOLH switch register */
#define PFC_IOLH34						(PFC_BASE + 0x11A0)	/* IOLH switch register */
#define PFC_IOLH35						(PFC_BASE + 0x11A8)	/* IOLH switch register */
#define PFC_IOLH36						(PFC_BASE + 0x11B0)	/* IOLH switch register */
#define PFC_IOLH37						(PFC_BASE + 0x11B8)	/* IOLH switch register */

#define PFC_PUPD20						(PFC_BASE + 0x1D00) /* PU/PD switch register */
#define PFC_PUPD21						(PFC_BASE + 0x1D08) /* PU/PD switch register */
#define PFC_PUPD22						(PFC_BASE + 0x1D10) /* PU/PD switch register */
#define PFC_PUPD23						(PFC_BASE + 0x1D18) /* PU/PD switch register */
#define PFC_PUPD24						(PFC_BASE + 0x1D20) /* PU/PD switch register */
#define PFC_PUPD25						(PFC_BASE + 0x1D28) /* PU/PD switch register */
#define PFC_PUPD26						(PFC_BASE + 0x1D30) /* PU/PD switch register */
#define PFC_PUPD27						(PFC_BASE + 0x1D38) /* PU/PD switch register */
#define PFC_PUPD28						(PFC_BASE + 0x1D40) /* PU/PD switch register */
#define PFC_PUPD29						(PFC_BASE + 0x1D48) /* PU/PD switch register */
#define PFC_PUPD2A						(PFC_BASE + 0x1D50) /* PU/PD switch register */
#define PFC_PUPD30						(PFC_BASE + 0x1D80) /* PU/PD switch register */
#define PFC_PUPD31						(PFC_BASE + 0x1D88) /* PU/PD switch register */
#define PFC_PUPD32						(PFC_BASE + 0x1D90) /* PU/PD switch register */
#define PFC_PUPD33						(PFC_BASE + 0x1D98) /* PU/PD switch register */
#define PFC_PUPD34						(PFC_BASE + 0x1DA0) /* PU/PD switch register */
#define PFC_PUPD35						(PFC_BASE + 0x1DA8) /* PU/PD switch register */
#define PFC_PUPD36						(PFC_BASE + 0x1DB0) /* PU/PD switch register */
#define PFC_PUPD37						(PFC_BASE + 0x1DB8) /* PU/PD switch register */

#define PFC_FILONOFF00					(PFC_BASE + 0x2000) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF20					(PFC_BASE + 0x2100) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF21					(PFC_BASE + 0x2108) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF22					(PFC_BASE + 0x2110) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF23					(PFC_BASE + 0x2118) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF24					(PFC_BASE + 0x2120) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF25					(PFC_BASE + 0x2128) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF26					(PFC_BASE + 0x2130) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF27					(PFC_BASE + 0x2138) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF28					(PFC_BASE + 0x2140) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF29					(PFC_BASE + 0x2148) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF2A					(PFC_BASE + 0x2150) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF30					(PFC_BASE + 0x2180) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF31					(PFC_BASE + 0x2188) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF32					(PFC_BASE + 0x2190) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF33					(PFC_BASE + 0x2198) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF34					(PFC_BASE + 0x21A0) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF35					(PFC_BASE + 0x21A8) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF36					(PFC_BASE + 0x21B0) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF37					(PFC_BASE + 0x21B8) /* Digital noise filter (FILONOFF) register */

#define PFC_FILNUM00					(PFC_BASE + 0x2400)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM20					(PFC_BASE + 0x2500)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM21					(PFC_BASE + 0x2508)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM22					(PFC_BASE + 0x2510)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM23					(PFC_BASE + 0x2518)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM24					(PFC_BASE + 0x2520)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM25					(PFC_BASE + 0x2528)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM26					(PFC_BASE + 0x2530)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM27					(PFC_BASE + 0x2538)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM28					(PFC_BASE + 0x2540)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM29					(PFC_BASE + 0x2548)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM2A					(PFC_BASE + 0x2550)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM30					(PFC_BASE + 0x2580)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM31					(PFC_BASE + 0x2588)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM32					(PFC_BASE + 0x2590)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM33					(PFC_BASE + 0x2598)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM34					(PFC_BASE + 0x25A0)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM35					(PFC_BASE + 0x25A8)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM36					(PFC_BASE + 0x25B0)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM37					(PFC_BASE + 0x25B8)	/* Digital noise filter (FILNUM) register */

#define PFC_FILCLKSEL01					(PFC_BASE + 0x2808)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL20					(PFC_BASE + 0x2900)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL21					(PFC_BASE + 0x2908)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL22					(PFC_BASE + 0x2910)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL23					(PFC_BASE + 0x2918)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL24					(PFC_BASE + 0x2920)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL25					(PFC_BASE + 0x2928)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL26					(PFC_BASE + 0x2930)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL27					(PFC_BASE + 0x2938)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL28					(PFC_BASE + 0x2940)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL29					(PFC_BASE + 0x2948)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL2A					(PFC_BASE + 0x2950)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL30					(PFC_BASE + 0x2980)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL31					(PFC_BASE + 0x2988)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL32					(PFC_BASE + 0x2990)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL33					(PFC_BASE + 0x2998)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL34					(PFC_BASE + 0x29A0)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL35					(PFC_BASE + 0x29A8)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL36					(PFC_BASE + 0x29B0)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL37					(PFC_BASE + 0x29B8)	/* Digital noise filter (FILCLKSEL) register */

#define PFC_IEN01						(PFC_BASE + 0x1808)	/* IEN switch register */
#define PFC_IEN02						(PFC_BASE + 0x1810)	/* IEN switch register */
#define PFC_IEN09						(PFC_BASE + 0x1848)	/* IEN switch register */
#define PFC_IEN10						(PFC_BASE + 0x1880)	/* IEN switch register */
#define PFC_IEN11						(PFC_BASE + 0x1888)	/* IEN switch register */
#define PFC_IEN12						(PFC_BASE + 0x1890)	/* IEN switch register */
#define PFC_IEN13						(PFC_BASE + 0x1898)	/* IEN switch register */
#define PFC_IEN23						(PFC_BASE + 0x1918)	/* IEN switch register */
#define PFC_IEN24						(PFC_BASE + 0x1920)	/* IEN switch register */
#define PFC_IEN30						(PFC_BASE + 0x1980)	/* IEN switch register */
#define PFC_IEN34						(PFC_BASE + 0x19A0)	/* IEN switch register */

#define PFC_PWPR						(PFC_BASE + 0x3000)	/* Write protect */

#define PFC_ETH_ch0						(PFC_BASE + 0x3010)	/* ETH ch0 voltage control register */
#define PFC_ETH_ch1						(PFC_BASE + 0x3014)	/* ETH ch1 voltage control register */
#define PFC_ETH_MII						(PFC_BASE + 0x3018)	/* Register for setting the mode of ETH MII / RGMII */
#define PFC_SD_ch0						(PFC_BASE + 0x3004)	/* SD ch0 IO voltage control register */
#define PFC_SD_ch1						(PFC_BASE + 0x3008)	/* SD ch1 IO voltage control register */
#define PFC_XSPI						(PFC_BASE + 0x300C)	/* XSPI IO voltage control register */
#define PFC_XSPI_OEN					(PFC_BASE + 0x3020)	/* Register for setting the OEN of XSPI Multi/OCTA */
#define PFC_I3C_SET						(PFC_BASE + 0x301C) /* I3C control register */

/* Combined terminal setting */
/* Definition for port register */
#define P_P0					(1 << 0)
#define P_P1					(1 << 1)
#define P_P2					(1 << 2)
#define P_P3					(1 << 3)
#define P_P4					(1 << 4)
#define P_P5					(1 << 5)
/* Definition for port mode register */
#define PM0_HIZ					(0 << 0)
#define PM0_IN					(1 << 0)
#define PM0_OUT_DIS				(2 << 0)
#define PM0_OUT_EN				(3 << 0)
#define PM1_HIZ					(0 << 2)
#define PM1_IN					(1 << 2)
#define PM1_OUT_DIS				(2 << 2)
#define PM1_OUT_EN				(3 << 2)
#define PM2_HIZ					(0 << 4)
#define PM2_IN					(1 << 4)
#define PM2_OUT_DIS				(2 << 4)
#define PM2_OUT_EN				(3 << 4)
#define PM3_HIZ					(0 << 6)
#define PM3_IN					(1 << 6)
#define PM3_OUT_DIS				(2 << 6)
#define PM3_OUT_EN				(3 << 6)
#define PM4_HIZ					(0 << 8)
#define PM4_IN					(1 << 8)
#define PM4_OUT_DIS				(2 << 8)
#define PM4_OUT_EN				(3 << 8)
#define PM5_HIZ					(0 << 10)
#define PM5_IN					(1 << 10)
#define PM5_OUT_DIS				(2 << 10)
#define PM5_OUT_EN				(3 << 10)
/* Definition for port mode control register */
#define PMC_PMC0				(1 << 0)
#define PMC_PMC1				(1 << 1)
#define PMC_PMC2				(1 << 2)
#define PMC_PMC3				(1 << 3)
#define PMC_PMC4				(1 << 4)
#define PMC_PMC5				(1 << 5)
/* Definition for port function control register */
#define PFC0_MODE1				(0 << 0)
#define PFC0_MODE2				(1 << 0)
#define PFC0_MODE3				(2 << 0)
#define PFC0_MODE4				(3 << 0)
#define PFC0_MODE5				(4 << 0)
#define PFC0_MODE6				(5 << 0)
#define PFC0_MODE7				(6 << 0)
#define PFC0_MODE8				(7 << 0)
#define PFC1_MODE1				(0 << 4)
#define PFC1_MODE2				(1 << 4)
#define PFC1_MODE3				(2 << 4)
#define PFC1_MODE4				(3 << 4)
#define PFC1_MODE5				(4 << 4)
#define PFC1_MODE6				(5 << 4)
#define PFC1_MODE7				(6 << 4)
#define PFC1_MODE8				(7 << 4)
#define PFC2_MODE1				(0 << 8)
#define PFC2_MODE2				(1 << 8)
#define PFC2_MODE3				(2 << 8)
#define PFC2_MODE4				(3 << 8)
#define PFC2_MODE5				(4 << 8)
#define PFC2_MODE6				(5 << 8)
#define PFC2_MODE7				(6 << 8)
#define PFC2_MODE8				(7 << 8)
#define PFC3_MODE1				(0 << 12)
#define PFC3_MODE2				(1 << 12)
#define PFC3_MODE3				(2 << 12)
#define PFC3_MODE4				(3 << 12)
#define PFC3_MODE5				(4 << 12)
#define PFC3_MODE6				(5 << 12)
#define PFC3_MODE7				(6 << 12)
#define PFC3_MODE8				(7 << 12)
#define PFC4_MODE1				(0 << 16)
#define PFC4_MODE2				(1 << 16)
#define PFC4_MODE3				(2 << 16)
#define PFC4_MODE4				(3 << 16)
#define PFC4_MODE5				(4 << 16)
#define PFC4_MODE6				(5 << 16)
#define PFC4_MODE7				(6 << 16)
#define PFC4_MODE8				(7 << 16)
#define PFC5_MODE1				(0 << 20)
#define PFC5_MODE2				(1 << 20)
#define PFC5_MODE3				(2 << 20)
#define PFC5_MODE4				(3 << 20)
#define PFC5_MODE5				(4 << 20)
#define PFC5_MODE6				(5 << 20)
#define PFC5_MODE7				(6 << 20)
#define PFC5_MODE8				(7 << 20)
/* Definition for port input register */
#define PIN_PIN0				(1 << 0)
#define PIN_PIN1				(1 << 1)
#define PIN_PIN2				(1 << 2)
#define PIN_PIN3				(1 << 3)
#define PIN_PIN4				(1 << 4)
#define PIN_PIN5				(1 << 5)
/* Definition for interrupt enable control register */
#define ISEL0_DIS				(0 << 0)
#define ISEL0_EN				(1 << 0)
#define ISEL1_DIS				(0 << 8)
#define ISEL1_EN				(1 << 8)
#define ISEL2_DIS				(0 << 16)
#define ISEL2_EN				(1 << 16)
#define ISEL3_DIS				(0 << 24)
#define ISEL3_EN				(1 << 24)
/* Definition for IOLH switch register */
#define IOLH0_PGA_33V_1_9_mA	(0 << 0)
#define IOLH0_PGA_33V_4_0_mA	(1 << 0)
#define IOLH0_PGA_33V_8_0_mA	(2 << 0)
#define IOLH0_PGA_33V_9_0_mA	(3 << 0)
#define IOLH0_PGA_18V_2_2_mA	(0 << 0)
#define IOLH0_PGA_18V_4_4_mA	(1 << 0)
#define IOLH0_PGA_18V_9_0_mA	(2 << 0)
#define IOLH0_PGA_18V_10_0_mA	(3 << 0)
#define IOLH0_PGB_33V_4_0_mA	(0 << 0)
#define IOLH0_PGB_33V_6_0_mA	(1 << 0)
#define IOLH0_PGB_33V_8_0_mA	(2 << 0)
#define IOLH0_PGB_33V_9_0_mA	(3 << 0)
#define IOLH0_PGB_18V_7_0_mA	(0 << 0)
#define IOLH0_PGB_18V_8_0_mA	(1 << 0)
#define IOLH0_PGB_18V_9_0_mA	(2 << 0)
#define IOLH0_PGB_18V_10_0_mA	(3 << 0)
#define IOLH0_PGC_33V_4_5_mA	(0 << 0)
#define IOLH0_PGC_33V_5_2_mA	(1 << 0)
#define IOLH0_PGC_33V_5_7_mA	(2 << 0)
#define IOLH0_PGC_33V_6_05_mA	(3 << 0)
#define IOLH0_PGC_25V_4_7_mA	(0 << 0)
#define IOLH0_PGC_25V_5_3_mA	(1 << 0)
#define IOLH0_PGC_25V_5_8_mA	(2 << 0)
#define IOLH0_PGC_25V_6_1_mA	(3 << 0)
#define IOLH0_PGC_18V_5_2_mA	(0 << 0)
#define IOLH0_PGC_18V_6_0_mA	(1 << 0)
#define IOLH0_PGC_18V_6_55_mA	(2 << 0)
#define IOLH0_PGC_18V_6_8_mA	(3 << 0)
#define IOLH1_PGA_33V_1_9_mA	(0 << 8)
#define IOLH1_PGA_33V_4_0_mA	(1 << 8)
#define IOLH1_PGA_33V_8_0_mA	(2 << 8)
#define IOLH1_PGA_33V_9_0_mA	(3 << 8)
#define IOLH1_PGA_18V_2_2_mA	(0 << 8)
#define IOLH1_PGA_18V_4_4_mA	(1 << 8)
#define IOLH1_PGA_18V_9_0_mA	(2 << 8)
#define IOLH1_PGA_18V_10_0_mA	(3 << 8)
#define IOLH1_PGB_33V_4_0_mA	(0 << 8)
#define IOLH1_PGB_33V_6_0_mA	(1 << 8)
#define IOLH1_PGB_33V_8_0_mA	(2 << 8)
#define IOLH1_PGB_33V_9_0_mA	(3 << 8)
#define IOLH1_PGB_18V_7_0_mA	(0 << 8)
#define IOLH1_PGB_18V_8_0_mA	(1 << 8)
#define IOLH1_PGB_18V_9_0_mA	(2 << 8)
#define IOLH1_PGB_18V_10_0_mA	(3 << 8)
#define IOLH1_PGC_33V_4_5_mA	(0 << 8)
#define IOLH1_PGC_33V_5_2_mA	(1 << 8)
#define IOLH1_PGC_33V_5_7_mA	(2 << 8)
#define IOLH1_PGC_33V_6_05_mA	(3 << 8)
#define IOLH1_PGC_25V_4_7_mA	(0 << 8)
#define IOLH1_PGC_25V_5_3_mA	(1 << 8)
#define IOLH1_PGC_25V_5_8_mA	(2 << 8)
#define IOLH1_PGC_25V_6_1_mA	(3 << 8)
#define IOLH1_PGC_18V_5_2_mA	(0 << 8)
#define IOLH1_PGC_18V_6_0_mA	(1 << 8)
#define IOLH1_PGC_18V_6_55_mA	(2 << 8)
#define IOLH1_PGC_18V_6_8_mA	(3 << 8)
#define IOLH2_PGA_33V_1_9_mA	(0 << 16)
#define IOLH2_PGA_33V_4_0_mA	(1 << 16)
#define IOLH2_PGA_33V_8_0_mA	(2 << 16)
#define IOLH2_PGA_33V_9_0_mA	(3 << 16)
#define IOLH2_PGA_18V_2_2_mA	(0 << 16)
#define IOLH2_PGA_18V_4_4_mA	(1 << 16)
#define IOLH2_PGA_18V_9_0_mA	(2 << 16)
#define IOLH2_PGA_18V_10_0_mA	(3 << 16)
#define IOLH2_PGB_33V_4_0_mA	(0 << 16)
#define IOLH2_PGB_33V_6_0_mA	(1 << 16)
#define IOLH2_PGB_33V_8_0_mA	(2 << 16)
#define IOLH2_PGB_33V_9_0_mA	(3 << 16)
#define IOLH2_PGB_18V_7_0_mA	(0 << 16)
#define IOLH2_PGB_18V_8_0_mA	(1 << 16)
#define IOLH2_PGB_18V_9_0_mA	(2 << 16)
#define IOLH2_PGB_18V_10_0_mA	(3 << 16)
#define IOLH2_PGC_33V_4_5_mA	(0 << 16)
#define IOLH2_PGC_33V_5_2_mA	(1 << 16)
#define IOLH2_PGC_33V_5_7_mA	(2 << 16)
#define IOLH2_PGC_33V_6_05_mA	(3 << 16)
#define IOLH2_PGC_25V_4_7_mA	(0 << 16)
#define IOLH2_PGC_25V_5_3_mA	(1 << 16)
#define IOLH2_PGC_25V_5_8_mA	(2 << 16)
#define IOLH2_PGC_25V_6_1_mA	(3 << 16)
#define IOLH2_PGC_18V_5_2_mA	(0 << 16)
#define IOLH2_PGC_18V_6_0_mA	(1 << 16)
#define IOLH2_PGC_18V_6_55_mA	(2 << 16)
#define IOLH2_PGC_18V_6_8_mA	(3 << 16)
#define IOLH3_PGA_33V_1_9_mA	(0 << 24)
#define IOLH3_PGA_33V_4_0_mA	(1 << 24)
#define IOLH3_PGA_33V_8_0_mA	(2 << 24)
#define IOLH3_PGA_33V_9_0_mA	(3 << 24)
#define IOLH3_PGA_18V_2_2_mA	(0 << 24)
#define IOLH3_PGA_18V_4_4_mA	(1 << 24)
#define IOLH3_PGA_18V_9_0_mA	(2 << 24)
#define IOLH3_PGA_18V_10_0_mA	(3 << 24)
#define IOLH3_PGB_33V_4_0_mA	(0 << 24)
#define IOLH3_PGB_33V_6_0_mA	(1 << 24)
#define IOLH3_PGB_33V_8_0_mA	(2 << 24)
#define IOLH3_PGB_33V_9_0_mA	(3 << 24)
#define IOLH3_PGB_18V_7_0_mA	(0 << 24)
#define IOLH3_PGB_18V_8_0_mA	(1 << 24)
#define IOLH3_PGB_18V_9_0_mA	(2 << 24)
#define IOLH3_PGB_18V_10_0_mA	(3 << 24)
#define IOLH3_PGC_33V_4_5_mA	(0 << 24)
#define IOLH3_PGC_33V_5_2_mA	(1 << 24)
#define IOLH3_PGC_33V_5_7_mA	(2 << 24)
#define IOLH3_PGC_33V_6_05_mA	(3 << 24)
#define IOLH3_PGC_25V_4_7_mA	(0 << 24)
#define IOLH3_PGC_25V_5_3_mA	(1 << 24)
#define IOLH3_PGC_25V_5_8_mA	(2 << 24)
#define IOLH3_PGC_25V_6_1_mA	(3 << 24)
#define IOLH3_PGC_18V_5_2_mA	(0 << 24)
#define IOLH3_PGC_18V_6_0_mA	(1 << 24)
#define IOLH3_PGC_18V_6_55_mA	(2 << 24)
#define IOLH3_PGC_18V_6_8_mA	(3 << 24)
#define IOLH4_PGA_33V_1_9_mA	(0 << 32)
#define IOLH4_PGA_33V_4_0_mA	(1 << 32)
#define IOLH4_PGA_33V_8_0_mA	(2 << 32)
#define IOLH4_PGA_33V_9_0_mA	(3 << 32)
#define IOLH4_PGA_18V_2_2_mA	(0 << 32)
#define IOLH4_PGA_18V_4_4_mA	(1 << 32)
#define IOLH4_PGA_18V_9_0_mA	(2 << 32)
#define IOLH4_PGA_18V_10_0_mA	(3 << 32)
#define IOLH4_PGB_33V_4_0_mA	(0 << 32)
#define IOLH4_PGB_33V_6_0_mA	(1 << 32)
#define IOLH4_PGB_33V_8_0_mA	(2 << 32)
#define IOLH4_PGB_33V_9_0_mA	(3 << 32)
#define IOLH4_PGB_18V_7_0_mA	(0 << 32)
#define IOLH4_PGB_18V_8_0_mA	(1 << 32)
#define IOLH4_PGB_18V_9_0_mA	(2 << 32)
#define IOLH4_PGB_18V_10_0_mA	(3 << 32)
#define IOLH4_PGC_33V_4_5_mA	(0 << 32)
#define IOLH4_PGC_33V_5_2_mA	(1 << 32)
#define IOLH4_PGC_33V_5_7_mA	(2 << 32)
#define IOLH4_PGC_33V_6_05_mA	(3 << 32)
#define IOLH4_PGC_25V_4_7_mA	(0 << 32)
#define IOLH4_PGC_25V_5_3_mA	(1 << 32)
#define IOLH4_PGC_25V_5_8_mA	(2 << 32)
#define IOLH4_PGC_25V_6_1_mA	(3 << 32)
#define IOLH4_PGC_18V_5_2_mA	(0 << 32)
#define IOLH4_PGC_18V_6_0_mA	(1 << 32)
#define IOLH4_PGC_18V_6_55_mA	(2 << 32)
#define IOLH4_PGC_18V_6_8_mA	(3 << 32)
#define IOLH5_PGA_33V_1_9_mA	(0 << 40)
#define IOLH5_PGA_33V_4_0_mA	(1 << 40)
#define IOLH5_PGA_33V_8_0_mA	(2 << 40)
#define IOLH5_PGA_33V_9_0_mA	(3 << 40)
#define IOLH5_PGA_18V_2_2_mA	(0 << 40)
#define IOLH5_PGA_18V_4_4_mA	(1 << 40)
#define IOLH5_PGA_18V_9_0_mA	(2 << 40)
#define IOLH5_PGA_18V_10_0_mA	(3 << 40)
#define IOLH5_PGB_33V_4_0_mA	(0 << 40)
#define IOLH5_PGB_33V_6_0_mA	(1 << 40)
#define IOLH5_PGB_33V_8_0_mA	(2 << 40)
#define IOLH5_PGB_33V_9_0_mA	(3 << 40)
#define IOLH5_PGB_18V_7_0_mA	(0 << 40)
#define IOLH5_PGB_18V_8_0_mA	(1 << 40)
#define IOLH5_PGB_18V_9_0_mA	(2 << 40)
#define IOLH5_PGB_18V_10_0_mA	(3 << 40)
#define IOLH5_PGC_33V_4_5_mA	(0 << 40)
#define IOLH5_PGC_33V_5_2_mA	(1 << 40)
#define IOLH5_PGC_33V_5_7_mA	(2 << 40)
#define IOLH5_PGC_33V_6_05_mA	(3 << 40)
#define IOLH5_PGC_25V_4_7_mA	(0 << 40)
#define IOLH5_PGC_25V_5_3_mA	(1 << 40)
#define IOLH5_PGC_25V_5_8_mA	(2 << 40)
#define IOLH5_PGC_25V_6_1_mA	(3 << 40)
#define IOLH5_PGC_18V_5_2_mA	(0 << 40)
#define IOLH5_PGC_18V_6_0_mA	(1 << 40)
#define IOLH5_PGC_18V_6_55_mA	(2 << 40)
#define IOLH5_PGC_18V_6_8_mA	(3 << 40)
#define IOLH6_PGA_33V_1_9_mA	(0 << 48)
#define IOLH6_PGA_33V_4_0_mA	(1 << 48)
#define IOLH6_PGA_33V_8_0_mA	(2 << 48)
#define IOLH6_PGA_33V_9_0_mA	(3 << 48)
#define IOLH6_PGA_18V_2_2_mA	(0 << 48)
#define IOLH6_PGA_18V_4_4_mA	(1 << 48)
#define IOLH6_PGA_18V_9_0_mA	(2 << 48)
#define IOLH6_PGA_18V_10_0_mA	(3 << 48)
#define IOLH6_PGB_33V_4_0_mA	(0 << 48)
#define IOLH6_PGB_33V_6_0_mA	(1 << 48)
#define IOLH6_PGB_33V_8_0_mA	(2 << 48)
#define IOLH6_PGB_33V_9_0_mA	(3 << 48)
#define IOLH6_PGB_18V_7_0_mA	(0 << 48)
#define IOLH6_PGB_18V_8_0_mA	(1 << 48)
#define IOLH6_PGB_18V_9_0_mA	(2 << 48)
#define IOLH6_PGB_18V_10_0_mA	(3 << 48)
#define IOLH6_PGC_33V_4_5_mA	(0 << 48)
#define IOLH6_PGC_33V_5_2_mA	(1 << 48)
#define IOLH6_PGC_33V_5_7_mA	(2 << 48)
#define IOLH6_PGC_33V_6_05_mA	(3 << 48)
#define IOLH6_PGC_25V_4_7_mA	(0 << 48)
#define IOLH6_PGC_25V_5_3_mA	(1 << 48)
#define IOLH6_PGC_25V_5_8_mA	(2 << 48)
#define IOLH6_PGC_25V_6_1_mA	(3 << 48)
#define IOLH6_PGC_18V_5_2_mA	(0 << 48)
#define IOLH6_PGC_18V_6_0_mA	(1 << 48)
#define IOLH6_PGC_18V_6_55_mA	(2 << 48)
#define IOLH6_PGC_18V_6_8_mA	(3 << 48)
#define IOLH7_PGA_33V_1_9_mA	(0 << 56)
#define IOLH7_PGA_33V_4_0_mA	(1 << 56)
#define IOLH7_PGA_33V_8_0_mA	(2 << 56)
#define IOLH7_PGA_33V_9_0_mA	(3 << 56)
#define IOLH7_PGA_18V_2_2_mA	(0 << 56)
#define IOLH7_PGA_18V_4_4_mA	(1 << 56)
#define IOLH7_PGA_18V_9_0_mA	(2 << 56)
#define IOLH7_PGA_18V_10_0_mA	(3 << 56)
#define IOLH7_PGB_33V_4_0_mA	(0 << 56)
#define IOLH7_PGB_33V_6_0_mA	(1 << 56)
#define IOLH7_PGB_33V_8_0_mA	(2 << 56)
#define IOLH7_PGB_33V_9_0_mA	(3 << 56)
#define IOLH7_PGB_18V_7_0_mA	(0 << 56)
#define IOLH7_PGB_18V_8_0_mA	(1 << 56)
#define IOLH7_PGB_18V_9_0_mA	(2 << 56)
#define IOLH7_PGB_18V_10_0_mA	(3 << 56)
#define IOLH7_PGC_33V_4_5_mA	(0 << 56)
#define IOLH7_PGC_33V_5_2_mA	(1 << 56)
#define IOLH7_PGC_33V_5_7_mA	(2 << 56)
#define IOLH7_PGC_33V_6_05_mA	(3 << 56)
#define IOLH7_PGC_25V_4_7_mA	(0 << 56)
#define IOLH7_PGC_25V_5_3_mA	(1 << 56)
#define IOLH7_PGC_25V_5_8_mA	(2 << 56)
#define IOLH7_PGC_25V_6_1_mA	(3 << 56)
#define IOLH7_PGC_18V_5_2_mA	(0 << 56)
#define IOLH7_PGC_18V_6_0_mA	(1 << 56)
#define IOLH7_PGC_18V_6_55_mA	(2 << 56)
#define IOLH7_PGC_18V_6_8_mA	(3 << 56)
/* Definition for PUPD switching register */
#define PUPD0_NO				(0 << 0)
#define PUPD0_UP				(1 << 0)
#define PUPD0_DOWN				(2 << 0)
#define PUPD1_NO				(0 << 8)
#define PUPD1_UP				(1 << 8)
#define PUPD1_DOWN				(2 << 8)
#define PUPD2_NO				(0 << 16)
#define PUPD2_UP				(1 << 16)
#define PUPD2_DOWN				(2 << 16)
#define PUPD3_NO				(0 << 24)
#define PUPD3_UP				(1 << 24)
#define PUPD3_DOWN				(2 << 24)
#define PUPD4_NO				(0 << 32)
#define PUPD4_UP				(1 << 32)
#define PUPD4_DOWN				(2 << 32)
#define PUPD5_NO				(0 << 40)
#define PUPD5_UP				(1 << 40)
#define PUPD5_DOWN				(2 << 40)
#define PUPD6_NO				(0 << 48)
#define PUPD6_UP				(1 << 48)
#define PUPD6_DOWN				(2 << 48)
#define PUPD7_NO				(0 << 56)
#define PUPD7_UP				(1 << 56)
#define PUPD7_DOWN				(2 << 56)
/* Digital noise filter (FILONOFF) register */
#define FILONOFF_FILON0			(1 << 0)
#define FILONOFF_FILON1			(1 << 8)
#define FILONOFF_FILON2			(1 << 16)
#define FILONOFF_FILON3			(1 << 24)
#define FILONOFF_FILON4			(1 << 32)
#define FILONOFF_FILON5			(1 << 40)
#define FILONOFF_FILON6			(1 << 48)
#define FILONOFF_FILON7			(1 << 56)
/* Digital noise filter (FILNUM) register */
#define FILNUM_FILNUM0_4		(0 << 0)
#define FILNUM_FILNUM0_8		(1 << 0)
#define FILNUM_FILNUM0_12		(2 << 0)
#define FILNUM_FILNUM0_16		(3 << 0)
#define FILNUM_FILNUM1_4		(0 << 8)
#define FILNUM_FILNUM1_8		(1 << 8)
#define FILNUM_FILNUM1_12		(2 << 8)
#define FILNUM_FILNUM1_16		(3 << 8)
#define FILNUM_FILNUM2_4		(0 << 16)
#define FILNUM_FILNUM2_8		(1 << 16)
#define FILNUM_FILNUM2_12		(2 << 16)
#define FILNUM_FILNUM2_16		(3 << 16)
#define FILNUM_FILNUM3_4		(0 << 24)
#define FILNUM_FILNUM3_8		(1 << 24)
#define FILNUM_FILNUM3_12		(2 << 24)
#define FILNUM_FILNUM3_16		(3 << 24)
#define FILNUM_FILNUM4_4		(0 << 32)
#define FILNUM_FILNUM4_8		(1 << 32)
#define FILNUM_FILNUM4_12		(2 << 32)
#define FILNUM_FILNUM4_16		(3 << 32)
#define FILNUM_FILNUM5_4		(0 << 40)
#define FILNUM_FILNUM5_8		(1 << 40)
#define FILNUM_FILNUM5_12		(2 << 40)
#define FILNUM_FILNUM5_16		(3 << 40)
#define FILNUM_FILNUM6_4		(0 << 48)
#define FILNUM_FILNUM6_8		(1 << 48)
#define FILNUM_FILNUM6_12		(2 << 48)
#define FILNUM_FILNUM6_16		(3 << 48)
#define FILNUM_FILNUM7_4		(0 << 56)
#define FILNUM_FILNUM7_8		(1 << 56)
#define FILNUM_FILNUM7_12		(2 << 56)
#define FILNUM_FILNUM7_16		(3 << 56)
/* Digital noise filter (FILCLKSEL) register */
#define FILCLKSEL_FILCLK0_0		(0 << 0)
#define FILCLKSEL_FILCLK0_1		(1 << 0)
#define FILCLKSEL_FILCLK0_2		(2 << 0)
#define FILCLKSEL_FILCLK0_3		(3 << 0)
#define FILCLKSEL_FILCLK1_0		(0 << 8)
#define FILCLKSEL_FILCLK1_1		(1 << 8)
#define FILCLKSEL_FILCLK1_2		(2 << 8)
#define FILCLKSEL_FILCLK1_3		(3 << 8)
#define FILCLKSEL_FILCLK2_0		(0 << 16)
#define FILCLKSEL_FILCLK2_1		(1 << 16)
#define FILCLKSEL_FILCLK2_2		(2 << 16)
#define FILCLKSEL_FILCLK2_3		(3 << 16)
#define FILCLKSEL_FILCLK3_0		(0 << 24)
#define FILCLKSEL_FILCLK3_1		(1 << 24)
#define FILCLKSEL_FILCLK3_2		(2 << 24)
#define FILCLKSEL_FILCLK3_3		(3 << 24)
#define FILCLKSEL_FILCLK4_0		(0 << 32)
#define FILCLKSEL_FILCLK4_1		(1 << 32)
#define FILCLKSEL_FILCLK4_2		(2 << 32)
#define FILCLKSEL_FILCLK4_3		(3 << 32)
#define FILCLKSEL_FILCLK5_0		(0 << 40)
#define FILCLKSEL_FILCLK5_1		(1 << 40)
#define FILCLKSEL_FILCLK5_2		(2 << 40)
#define FILCLKSEL_FILCLK5_3		(3 << 40)
#define FILCLKSEL_FILCLK6_0		(0 << 48)
#define FILCLKSEL_FILCLK6_1		(1 << 48)
#define FILCLKSEL_FILCLK6_2		(2 << 48)
#define FILCLKSEL_FILCLK6_3		(3 << 48)
#define FILCLKSEL_FILCLK7_0		(0 << 56)
#define FILCLKSEL_FILCLK7_1		(1 << 56)
#define FILCLKSEL_FILCLK7_2		(2 << 56)
#define FILCLKSEL_FILCLK7_3		(3 << 56)
/* Definition for IEN switching register */
#define IEN0_ENABLE				(1 << 0)
#define IEN1_ENABLE				(1 << 8)
#define IEN2_ENABLE				(1 << 16)
#define IEN3_ENABLE				(1 << 24)
#define IEN4_ENABLE				(1 << 32)
#define IEN5_ENABLE				(1 << 40)
#define IEN6_ENABLE				(1 << 48)
#define IEN7_ENABLE				(1 << 56)
/* SD ch0 IO Definition for voltage control register */
#define SD0_PVDD				(1 << 0)
/* SD ch1 IO Definition for voltage control register */
#define SD1_PVDD				(1 << 0)
/* Definition for QSPI IO voltage control register */
#define QSPI_PVDD				(1 << 0)
/* ETH ch0 voltage control register */
#define ETH_ch0_3_3				(0 << 0)
#define ETH_ch0_1_8				(1 << 0)
#define ETH_ch0_2_5				(2 << 0)
/* ETH ch1 voltage control register */
#define ETH_ch1_3_3				(0 << 0)
#define ETH_ch1_1_8				(1 << 0)
#define ETH_ch1_2_5				(2 << 0)
/* Write protection definition */
#define PWPR_B0WI				(1 << 7)
#define PWPR_PFCWE				(1 << 6)
/* Register for setting the mode of ETH MII / RGMII */
#define ETH_MII_0_MII			(1 << 0)
#define ETH_MII_1_MII			(1 << 1)
#define XSPI_OEN_SORST_N		(1 << 0)

#define	PFC_SET_TBL_NUM			(11)
#define	PFC_OFF					(0)
#define PFC_ON					(1)

#define PFC_MUX_SD_TBL_NUM		(1)
#define PFC_MUX_SCIF_TBL_NUM	(1)
#define PFC_XSPI_TBL_NUM		(2)
#define PFC_SD_TBL_NUM_DEV		(4)
#define PFC_SD_TBL_NUM_SMARC	(6)

typedef struct {
	int			flg;
	uintptr_t	reg;
	uint8_t		val;
} PFC_REG_UINT8;

typedef struct {
	int			flg;
	uintptr_t	reg;
	uint32_t	val;
} PFC_REG_UINT32;

typedef struct {
	int			flg;
	uintptr_t	reg;
	uint64_t	val;
} PFC_REG_UINT64;

typedef struct {
	PFC_REG_UINT8	pmc;
	PFC_REG_UINT32	pfc;
	PFC_REG_UINT64	iolh;
	PFC_REG_UINT64	pupd;
	PFC_REG_UINT64	sr;
	PFC_REG_UINT64	ien;
} PFC_REGS;

#endif	/* __PFC_REGS_H__ */
