/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __PFC_REGS_H__
#define __PFC_REGS_H__

#include <rz_soc_def.h>					/* Get the PFC base address */

#define PFC_P22							(PFC_BASE + 0x0022)	/* Port register */
#define PFC_P23							(PFC_BASE + 0x0023)	/* Port register */
#define PFC_P25							(PFC_BASE + 0x0025)	/* Port register */
#define PFC_P26							(PFC_BASE + 0x0026)	/* Port register */
#define PFC_P27							(PFC_BASE + 0x0027)	/* Port register */
#define PFC_P28							(PFC_BASE + 0x0028)	/* Port register */
#define PFC_P2A							(PFC_BASE + 0x002A)	/* Port register */
#define PFC_P2B							(PFC_BASE + 0x002B)	/* Port register */
#define PFC_P2C							(PFC_BASE + 0x002C)	/* Port register */
#define PFC_P2D							(PFC_BASE + 0x002D)	/* Port register */
#define PFC_P2E							(PFC_BASE + 0x002E)	/* Port register */
#define PFC_P2F							(PFC_BASE + 0x002F)	/* Port register */
#define PFC_P30							(PFC_BASE + 0x0030)	/* Port register */
#define PFC_P31							(PFC_BASE + 0x0031)	/* Port register */
#define PFC_P32							(PFC_BASE + 0x0032)	/* Port register */
#define PFC_P33							(PFC_BASE + 0x0033)	/* Port register */
#define PFC_P34							(PFC_BASE + 0x0034)	/* Port register */
#define PFC_P35							(PFC_BASE + 0x0035)	/* Port register */
#define PFC_P36							(PFC_BASE + 0x0036)	/* Port register */
#define PFC_P3C							(PFC_BASE + 0x003C)	/* Port register */

#define PFC_PM22						(PFC_BASE + 0x0144)	/* Port mode register */
#define PFC_PM23						(PFC_BASE + 0x0146)	/* Port mode register */
#define PFC_PM25						(PFC_BASE + 0x014A)	/* Port mode register */
#define PFC_PM26						(PFC_BASE + 0x014C)	/* Port mode register */
#define PFC_PM27						(PFC_BASE + 0x014E)	/* Port mode register */
#define PFC_PM28						(PFC_BASE + 0x0150)	/* Port mode register */
#define PFC_PM2A						(PFC_BASE + 0x0154)	/* Port mode register */
#define PFC_PM2B						(PFC_BASE + 0x0156)	/* Port mode register */
#define PFC_PM2C						(PFC_BASE + 0x0158)	/* Port mode register */
#define PFC_PM2D						(PFC_BASE + 0x015A)	/* Port mode register */
#define PFC_PM2E						(PFC_BASE + 0x015C)	/* Port mode register */
#define PFC_PM2F						(PFC_BASE + 0x015E)	/* Port mode register */
#define PFC_PM30						(PFC_BASE + 0x0160)	/* Port mode register */
#define PFC_PM31						(PFC_BASE + 0x0162)	/* Port mode register */
#define PFC_PM32						(PFC_BASE + 0x0164)	/* Port mode register */
#define PFC_PM33						(PFC_BASE + 0x0166)	/* Port mode register */
#define PFC_PM34						(PFC_BASE + 0x0168)	/* Port mode register */
#define PFC_PM35						(PFC_BASE + 0x016A)	/* Port mode register */
#define PFC_PM36						(PFC_BASE + 0x016C)	/* Port mode register */
#define PFC_PM3C						(PFC_BASE + 0x0178)	/* Port mode register */

#define PFC_PMC22						(PFC_BASE + 0x0222)	/* Port mode control register */
#define PFC_PMC23						(PFC_BASE + 0x0223)	/* Port mode control register */
#define PFC_PMC25						(PFC_BASE + 0x0225)	/* Port mode control register */
#define PFC_PMC26						(PFC_BASE + 0x0226)	/* Port mode control register */
#define PFC_PMC27						(PFC_BASE + 0x0227)	/* Port mode control register */
#define PFC_PMC28						(PFC_BASE + 0x0228)	/* Port mode control register */
#define PFC_PMC2A						(PFC_BASE + 0x022A)	/* Port mode control register */
#define PFC_PMC2B						(PFC_BASE + 0x022B)	/* Port mode control register */
#define PFC_PMC2C						(PFC_BASE + 0x022C)	/* Port mode control register */
#define PFC_PMC2D						(PFC_BASE + 0x022D)	/* Port mode control register */
#define PFC_PMC2E						(PFC_BASE + 0x022E)	/* Port mode control register */
#define PFC_PMC2F						(PFC_BASE + 0x022F)	/* Port mode control register */
#define PFC_PMC30						(PFC_BASE + 0x0230)	/* Port mode control register */
#define PFC_PMC31						(PFC_BASE + 0x0231)	/* Port mode control register */
#define PFC_PMC32						(PFC_BASE + 0x0232)	/* Port mode control register */
#define PFC_PMC33						(PFC_BASE + 0x0233)	/* Port mode control register */
#define PFC_PMC34						(PFC_BASE + 0x0234)	/* Port mode control register */
#define PFC_PMC35						(PFC_BASE + 0x0235)	/* Port mode control register */
#define PFC_PMC36						(PFC_BASE + 0x0236)	/* Port mode control register */
#define PFC_PMC3C						(PFC_BASE + 0x023C)	/* Port mode control register */

#define PFC_PFC22						(PFC_BASE + 0x0488)	/* Port function control register */
#define PFC_PFC23						(PFC_BASE + 0x048C)	/* Port function control register */
#define PFC_PFC25						(PFC_BASE + 0x0494)	/* Port function control register */
#define PFC_PFC26						(PFC_BASE + 0x0498)	/* Port function control register */
#define PFC_PFC27						(PFC_BASE + 0x049C)	/* Port function control register */
#define PFC_PFC28						(PFC_BASE + 0x04A0)	/* Port function control register */
#define PFC_PFC2A						(PFC_BASE + 0x04A8)	/* Port function control register */
#define PFC_PFC2B						(PFC_BASE + 0x04AC)	/* Port function control register */
#define PFC_PFC2C						(PFC_BASE + 0x04B0)	/* Port function control register */
#define PFC_PFC2D						(PFC_BASE + 0x04B4)	/* Port function control register */
#define PFC_PFC2E						(PFC_BASE + 0x04B8)	/* Port function control register */
#define PFC_PFC2F						(PFC_BASE + 0x04BC)	/* Port function control register */
#define PFC_PFC30						(PFC_BASE + 0x04C0)	/* Port function control register */
#define PFC_PFC31						(PFC_BASE + 0x04C4)	/* Port function control register */
#define PFC_PFC32						(PFC_BASE + 0x04C8)	/* Port function control register */
#define PFC_PFC33						(PFC_BASE + 0x04CC)	/* Port function control register */
#define PFC_PFC34						(PFC_BASE + 0x04D0)	/* Port function control register */
#define PFC_PFC35						(PFC_BASE + 0x04D4)	/* Port function control register */
#define PFC_PFC36						(PFC_BASE + 0x04D8)	/* Port function control register */
#define PFC_PFC3C						(PFC_BASE + 0x04F0)	/* Port function control register */

#define PFC_PIN22						(PFC_BASE + 0x0822) /* Port Input Register */
#define PFC_PIN23						(PFC_BASE + 0x0823) /* Port Input Register */
#define PFC_PIN25						(PFC_BASE + 0x0825) /* Port Input Register */
#define PFC_PIN26						(PFC_BASE + 0x0826) /* Port Input Register */
#define PFC_PIN27						(PFC_BASE + 0x0827) /* Port Input Register */
#define PFC_PIN28						(PFC_BASE + 0x0828) /* Port Input Register */
#define PFC_PIN2A						(PFC_BASE + 0x082A) /* Port Input Register */
#define PFC_PIN2B						(PFC_BASE + 0x082B) /* Port Input Register */
#define PFC_PIN2C						(PFC_BASE + 0x082C) /* Port Input Register */
#define PFC_PIN2D						(PFC_BASE + 0x082D) /* Port Input Register */
#define PFC_PIN2E						(PFC_BASE + 0x082E) /* Port Input Register */
#define PFC_PIN2F						(PFC_BASE + 0x082F) /* Port Input Register */
#define PFC_PIN30						(PFC_BASE + 0x0830) /* Port Input Register */
#define PFC_PIN31						(PFC_BASE + 0x0831) /* Port Input Register */
#define PFC_PIN32						(PFC_BASE + 0x0832) /* Port Input Register */
#define PFC_PIN33						(PFC_BASE + 0x0833) /* Port Input Register */
#define PFC_PIN34						(PFC_BASE + 0x0834) /* Port Input Register */
#define PFC_PIN35						(PFC_BASE + 0x0835) /* Port Input Register */
#define PFC_PIN36						(PFC_BASE + 0x0836) /* Port Input Register */
#define PFC_PIN3C						(PFC_BASE + 0x083C) /* Port Input Register */

#define PFC_ISEL22						(PFC_BASE + 0x2D10) /* Interrupt Enable Control Register */
#define PFC_ISEL23						(PFC_BASE + 0x2D18) /* Interrupt Enable Control Register */
#define PFC_ISEL25						(PFC_BASE + 0x2D28) /* Interrupt Enable Control Register */
#define PFC_ISEL26						(PFC_BASE + 0x2D30) /* Interrupt Enable Control Register */
#define PFC_ISEL27						(PFC_BASE + 0x2D38) /* Interrupt Enable Control Register */
#define PFC_ISEL28						(PFC_BASE + 0x2D40) /* Interrupt Enable Control Register */
#define PFC_ISEL2A						(PFC_BASE + 0x2D50) /* Interrupt Enable Control Register */
#define PFC_ISEL2B						(PFC_BASE + 0x2D58) /* Interrupt Enable Control Register */
#define PFC_ISEL2C						(PFC_BASE + 0x2D60) /* Interrupt Enable Control Register */
#define PFC_ISEL2D						(PFC_BASE + 0x2D68) /* Interrupt Enable Control Register */
#define PFC_ISEL2E						(PFC_BASE + 0x2D70) /* Interrupt Enable Control Register */
#define PFC_ISEL2F						(PFC_BASE + 0x2D78) /* Interrupt Enable Control Register */
#define PFC_ISEL30						(PFC_BASE + 0x2D80) /* Interrupt Enable Control Register */
#define PFC_ISEL31						(PFC_BASE + 0x2D88) /* Interrupt Enable Control Register */
#define PFC_ISEL32						(PFC_BASE + 0x2D90) /* Interrupt Enable Control Register */
#define PFC_ISEL33						(PFC_BASE + 0x2D98) /* Interrupt Enable Control Register */
#define PFC_ISEL34						(PFC_BASE + 0x2DA0) /* Interrupt Enable Control Register */
#define PFC_ISEL35						(PFC_BASE + 0x2DA8) /* Interrupt Enable Control Register */
#define PFC_ISEL36						(PFC_BASE + 0x2DB0) /* Interrupt Enable Control Register */
#define PFC_ISEL3C						(PFC_BASE + 0x2DE0) /* Interrupt Enable Control Register */

#define PFC_IOLH05						(PFC_BASE + 0x1028)	/* IOLH switch register */
#define PFC_IOLH06						(PFC_BASE + 0x1030)	/* IOLH switch register */
#define PFC_IOLH09						(PFC_BASE + 0x1048)	/* IOLH switch register */
#define PFC_IOLH0A						(PFC_BASE + 0x1050)	/* IOLH switch register */
#define PFC_IOLH23						(PFC_BASE + 0x1118)	/* IOLH switch register */
#define PFC_IOLH25						(PFC_BASE + 0x1128)	/* IOLH switch register */
#define PFC_IOLH26						(PFC_BASE + 0x1130)	/* IOLH switch register */
#define PFC_IOLH27						(PFC_BASE + 0x1138)	/* IOLH switch register */
#define PFC_IOLH28						(PFC_BASE + 0x1140)	/* IOLH switch register */
#define PFC_IOLH2A						(PFC_BASE + 0x1150)	/* IOLH switch register */
#define PFC_IOLH2B						(PFC_BASE + 0x1158)	/* IOLH switch register */
#define PFC_IOLH2C						(PFC_BASE + 0x1160)	/* IOLH switch register */
#define PFC_IOLH2D						(PFC_BASE + 0x1168)	/* IOLH switch register */
#define PFC_IOLH2E						(PFC_BASE + 0x1170)	/* IOLH switch register */
#define PFC_IOLH2F						(PFC_BASE + 0x1178)	/* IOLH switch register */
#define PFC_IOLH30						(PFC_BASE + 0x1180)	/* IOLH switch register */
#define PFC_IOLH31						(PFC_BASE + 0x1188)	/* IOLH switch register */
#define PFC_IOLH32						(PFC_BASE + 0x1190)	/* IOLH switch register */
#define PFC_IOLH33						(PFC_BASE + 0x1198)	/* IOLH switch register */
#define PFC_IOLH34						(PFC_BASE + 0x11A0)	/* IOLH switch register */
#define PFC_IOLH35						(PFC_BASE + 0x11A8)	/* IOLH switch register */
#define PFC_IOLH36						(PFC_BASE + 0x11B0)	/* IOLH switch register */
#define PFC_IOLH3C						(PFC_BASE + 0x11E0)	/* IOLH switch register */

#define PFC_PUPD06						(PFC_BASE + 0x1C30) /* PU/PD switch register */
#define PFC_PUPD09						(PFC_BASE + 0x1C48) /* PU/PD switch register */
#define PFC_PUPD0A						(PFC_BASE + 0x1C50) /* PU/PD switch register */
#define PFC_PUPD23						(PFC_BASE + 0x1D18) /* PU/PD switch register */
#define PFC_PUPD25						(PFC_BASE + 0x1D28) /* PU/PD switch register */
#define PFC_PUPD26						(PFC_BASE + 0x1D30) /* PU/PD switch register */
#define PFC_PUPD27						(PFC_BASE + 0x1D38) /* PU/PD switch register */
#define PFC_PUPD28						(PFC_BASE + 0x1D40) /* PU/PD switch register */
#define PFC_PUPD2A						(PFC_BASE + 0x1D50) /* PU/PD switch register */
#define PFC_PUPD2B						(PFC_BASE + 0x1D58) /* PU/PD switch register */
#define PFC_PUPD2C						(PFC_BASE + 0x1D60) /* PU/PD switch register */
#define PFC_PUPD2D						(PFC_BASE + 0x1D68) /* PU/PD switch register */
#define PFC_PUPD2E						(PFC_BASE + 0x1D70) /* PU/PD switch register */
#define PFC_PUPD2F						(PFC_BASE + 0x1D78) /* PU/PD switch register */
#define PFC_PUPD30						(PFC_BASE + 0x1D80) /* PU/PD switch register */
#define PFC_PUPD31						(PFC_BASE + 0x1D88) /* PU/PD switch register */
#define PFC_PUPD33						(PFC_BASE + 0x1D98) /* PU/PD switch register */
#define PFC_PUPD34						(PFC_BASE + 0x1DA0) /* PU/PD switch register */
#define PFC_PUPD35						(PFC_BASE + 0x1DA8) /* PU/PD switch register */
#define PFC_PUPD36						(PFC_BASE + 0x1DB0) /* PU/PD switch register */
#define PFC_PUPD3C						(PFC_BASE + 0x1DE0) /* PU/PD switch register */

#define PFC_FILONOFF22					(PFC_BASE + 0x2110) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF23					(PFC_BASE + 0x2118) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF25					(PFC_BASE + 0x2128) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF26					(PFC_BASE + 0x2130) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF27					(PFC_BASE + 0x2138) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF28					(PFC_BASE + 0x2140) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF2A					(PFC_BASE + 0x2150) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF2B					(PFC_BASE + 0x2158) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF2C					(PFC_BASE + 0x2160) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF2D					(PFC_BASE + 0x2168) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF2E					(PFC_BASE + 0x2170) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF2F					(PFC_BASE + 0x2178) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF30					(PFC_BASE + 0x2180) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF31					(PFC_BASE + 0x2188) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF33					(PFC_BASE + 0x2198) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF34					(PFC_BASE + 0x21A0) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF35					(PFC_BASE + 0x21A8) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF36					(PFC_BASE + 0x21B0) /* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF3C					(PFC_BASE + 0x21E0) /* Digital noise filter (FILONOFF) register */

#define PFC_FILNUM22					(PFC_BASE + 0x2510)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM23					(PFC_BASE + 0x2518)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM25					(PFC_BASE + 0x2528)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM26					(PFC_BASE + 0x2530)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM27					(PFC_BASE + 0x2538)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM28					(PFC_BASE + 0x2540)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM2A					(PFC_BASE + 0x2550)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM2B					(PFC_BASE + 0x2558)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM2C					(PFC_BASE + 0x2560)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM2D					(PFC_BASE + 0x2568)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM2E					(PFC_BASE + 0x2570)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM2F					(PFC_BASE + 0x2578)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM30					(PFC_BASE + 0x2580)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM31					(PFC_BASE + 0x2588)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM33					(PFC_BASE + 0x2598)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM34					(PFC_BASE + 0x25A0)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM35					(PFC_BASE + 0x25A8)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM36					(PFC_BASE + 0x25B0)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM3C					(PFC_BASE + 0x25E0)	/* Digital noise filter (FILNUM) register */

#define PFC_FILCLKSEL22					(PFC_BASE + 0x2910)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL23					(PFC_BASE + 0x2918)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL25					(PFC_BASE + 0x2928)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL26					(PFC_BASE + 0x2930)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL27					(PFC_BASE + 0x2938)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL28					(PFC_BASE + 0x2940)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL2A					(PFC_BASE + 0x2950)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL2B					(PFC_BASE + 0x2958)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL2C					(PFC_BASE + 0x2960)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL2D					(PFC_BASE + 0x2968)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL2E					(PFC_BASE + 0x2970)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL2F					(PFC_BASE + 0x2978)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL30					(PFC_BASE + 0x2980)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL31					(PFC_BASE + 0x2988)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL33					(PFC_BASE + 0x2998)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL34					(PFC_BASE + 0x29A0)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL35					(PFC_BASE + 0x29A8)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL36					(PFC_BASE + 0x29B0)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL3C					(PFC_BASE + 0x29E0)	/* Digital noise filter (FILCLKSEL) register */

#define PFC_IEN09						(PFC_BASE + 0x1848)	/* IEN switch register */
#define PFC_IEN0A						(PFC_BASE + 0x1850)	/* IEN switch register */
#define PFC_IEN22						(PFC_BASE + 0x1910)	/* IEN switch register */
#define PFC_IEN2A						(PFC_BASE + 0x1950)	/* IEN switch register */
#define PFC_IEN2D						(PFC_BASE + 0x1968)	/* IEN switch register */
#define PFC_IEN30						(PFC_BASE + 0x1980)	/* IEN switch register */
#define PFC_IEN31						(PFC_BASE + 0x1988)	/* IEN switch register */
#define PFC_IEN33						(PFC_BASE + 0x1998)	/* IEN switch register */

#define PFC_PWPR						(PFC_BASE + 0x3000)	/* Write protect */

#define PFC_ETH_ch0						(PFC_BASE + 0x3010)	/* ETH ch0 voltage control register */
#define PFC_ETH_ch1						(PFC_BASE + 0x3014)	/* ETH ch1 voltage control register */
#define PFC_ETH_MII						(PFC_BASE + 0x3018)	/* Register for setting the mode of ETH MII / RGMII */
#define PFC_SD_ch0						(PFC_BASE + 0x3004)	/* SD ch0 IO voltage control register */
#define PFC_SD_ch1						(PFC_BASE + 0x3008)	/* SD ch1 IO voltage control register */
#define PFC_XSPI_VOLT_CTL				(PFC_BASE + 0x300C)	/* XSPI IO voltage control register */
#define PFC_I3C_SET						(PFC_BASE + 0x301C) /* I3C control register */
#define PFC_XSPI_OEN					(PFC_BASE + 0x3020)	/* Register for setting the OEN of XSPI Multi */
#define PFC_SD_ch2						(PFC_BASE + 0x3024)	/* SD ch2 IO voltage control register */
#define PFC_OTHER_POC					(PFC_BASE + 0x3028)	/* Other IO Voltage Mode Control Register  */


/* Combined terminal setting */
/* Definition for port register */
#define P_P0					(1 << 0)
#define P_P1					(1 << 1)
#define P_P2					(1 << 2)
#define P_P3					(1 << 3)
#define P_P4					(1 << 4)
#define P_P5					(1 << 5)
#define P_P6					(1 << 6)
#define P_P7					(1 << 7)
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
#define PM6_HIZ					(0 << 12)
#define PM6_IN					(1 << 12)
#define PM6_OUT_DIS				(2 << 12)
#define PM6_OUT_EN				(3 << 12)
#define PM7_HIZ					(0 << 14)
#define PM7_IN					(1 << 14)
#define PM7_OUT_DIS				(2 << 14)
#define PM7_OUT_EN				(3 << 14)

/* Definition for port mode control register */
#define PMC_PMC0				(1 << 0)
#define PMC_PMC1				(1 << 1)
#define PMC_PMC2				(1 << 2)
#define PMC_PMC3				(1 << 3)
#define PMC_PMC4				(1 << 4)
#define PMC_PMC5				(1 << 5)
#define PMC_PMC6				(1 << 6)
#define PMC_PMC7				(1 << 7)

/* Definition for port function control register */
#define PFC0_MODE1				(0 << 0)
#define PFC0_MODE2				(1 << 0)
#define PFC0_MODE3				(2 << 0)
#define PFC0_MODE4				(3 << 0)
#define PFC0_MODE5				(4 << 0)
#define PFC0_MODE6				(5 << 0)
#define PFC0_MODE7				(6 << 0)
#define PFC0_MODE8				(7 << 0)
#define PFC0_MODE9				(8 << 0)
#define PFC0_MODE10				(9 << 0)
#define PFC0_MODE11				(10 << 0)
#define PFC0_MODE12				(11 << 0)
#define PFC0_MODE13				(12 << 0)
#define PFC0_MODE14				(13 << 0)
#define PFC0_MODE15				(14 << 0)
#define PFC0_MODE16				(15 << 0)
#define PFC1_MODE1				(0 << 4)
#define PFC1_MODE2				(1 << 4)
#define PFC1_MODE3				(2 << 4)
#define PFC1_MODE4				(3 << 4)
#define PFC1_MODE5				(4 << 4)
#define PFC1_MODE6				(5 << 4)
#define PFC1_MODE7				(6 << 4)
#define PFC1_MODE8				(7 << 4)
#define PFC1_MODE9				(8 << 4)
#define PFC1_MODE10				(9 << 4)
#define PFC1_MODE11				(10 << 4)
#define PFC1_MODE12				(11 << 4)
#define PFC1_MODE13				(12 << 4)
#define PFC1_MODE14				(13 << 4)
#define PFC1_MODE15				(14 << 4)
#define PFC1_MODE16				(15 << 4)
#define PFC2_MODE1				(0 << 8)
#define PFC2_MODE2				(1 << 8)
#define PFC2_MODE3				(2 << 8)
#define PFC2_MODE4				(3 << 8)
#define PFC2_MODE5				(4 << 8)
#define PFC2_MODE6				(5 << 8)
#define PFC2_MODE7				(6 << 8)
#define PFC2_MODE8				(7 << 8)
#define PFC2_MODE9				(8 << 8)
#define PFC2_MODE10				(9 << 8)
#define PFC2_MODE11				(10 << 8)
#define PFC2_MODE12				(11 << 8)
#define PFC2_MODE13				(12 << 8)
#define PFC2_MODE14				(13 << 8)
#define PFC2_MODE15				(14 << 8)
#define PFC2_MODE16				(15 << 8)
#define PFC3_MODE1				(0 << 12)
#define PFC3_MODE2				(1 << 12)
#define PFC3_MODE3				(2 << 12)
#define PFC3_MODE4				(3 << 12)
#define PFC3_MODE5				(4 << 12)
#define PFC3_MODE6				(5 << 12)
#define PFC3_MODE7				(6 << 12)
#define PFC3_MODE8				(7 << 12)
#define PFC3_MODE9				(8 << 12)
#define PFC3_MODE10				(9 << 12)
#define PFC3_MODE11				(10 << 12)
#define PFC3_MODE12				(11 << 12)
#define PFC3_MODE13				(12 << 12)
#define PFC3_MODE14				(13 << 12)
#define PFC3_MODE15				(14 << 12)
#define PFC3_MODE16				(15 << 12)
#define PFC4_MODE1				(0 << 16)
#define PFC4_MODE2				(1 << 16)
#define PFC4_MODE3				(2 << 16)
#define PFC4_MODE4				(3 << 16)
#define PFC4_MODE5				(4 << 16)
#define PFC4_MODE6				(5 << 16)
#define PFC4_MODE7				(6 << 16)
#define PFC4_MODE8				(7 << 16)
#define PFC4_MODE9				(8 << 16)
#define PFC4_MODE10				(9 << 16)
#define PFC4_MODE11				(10 << 16)
#define PFC4_MODE12				(11 << 16)
#define PFC4_MODE13				(12 << 16)
#define PFC4_MODE14				(13 << 16)
#define PFC4_MODE15				(14 << 16)
#define PFC4_MODE16				(15 << 16)
#define PFC5_MODE1				(0 << 20)
#define PFC5_MODE2				(1 << 20)
#define PFC5_MODE3				(2 << 20)
#define PFC5_MODE4				(3 << 20)
#define PFC5_MODE5				(4 << 20)
#define PFC5_MODE6				(5 << 20)
#define PFC5_MODE7				(6 << 20)
#define PFC5_MODE8				(7 << 20)
#define PFC5_MODE9				(8 << 20)
#define PFC5_MODE10				(9 << 20)
#define PFC5_MODE11				(10 << 20)
#define PFC5_MODE12				(11 << 20)
#define PFC5_MODE13				(12 << 20)
#define PFC5_MODE14				(13 << 20)
#define PFC5_MODE15				(14 << 20)
#define PFC5_MODE16				(15 << 20)
#define PFC6_MODE1				(0 << 24)
#define PFC6_MODE2				(1 << 24)
#define PFC6_MODE3				(2 << 24)
#define PFC6_MODE4				(3 << 24)
#define PFC6_MODE5				(4 << 24)
#define PFC6_MODE6				(5 << 24)
#define PFC6_MODE7				(6 << 24)
#define PFC6_MODE8				(7 << 24)
#define PFC6_MODE9				(8 << 24)
#define PFC6_MODE10				(9 << 24)
#define PFC6_MODE11				(10 << 24)
#define PFC6_MODE12				(11 << 24)
#define PFC6_MODE13				(12 << 24)
#define PFC6_MODE14				(13 << 24)
#define PFC6_MODE15				(14 << 24)
#define PFC6_MODE16				(15 << 24)
#define PFC7_MODE1				(0 << 28)
#define PFC7_MODE2				(1 << 28)
#define PFC7_MODE3				(2 << 28)
#define PFC7_MODE4				(3 << 28)
#define PFC7_MODE5				(4 << 28)
#define PFC7_MODE6				(5 << 28)
#define PFC7_MODE7				(6 << 28)
#define PFC7_MODE8				(7 << 28)
#define PFC7_MODE9				(8 << 28)
#define PFC7_MODE10				(9 << 28)
#define PFC7_MODE11				(10 << 28)
#define PFC7_MODE12				(11 << 28)
#define PFC7_MODE13				(12 << 28)
#define PFC7_MODE14				(13 << 28)
#define PFC7_MODE15				(14 << 28)
#define PFC7_MODE16				(15 << 28)

/* Definition for port input register */
#define PIN_PIN0				(1 << 0)
#define PIN_PIN1				(1 << 1)
#define PIN_PIN2				(1 << 2)
#define PIN_PIN3				(1 << 3)
#define PIN_PIN4				(1 << 4)
#define PIN_PIN5				(1 << 5)
#define PIN_PIN6				(1 << 6)
#define PIN_PIN7				(1 << 7)

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
#define QSPI_PVDD_3V3			(0)				/* 3.3V voltage for XSPI */
#define QSPI_PVDD_1V8			(1 << 0)		/* 1.8V voltage for XSPI */
#define QSPI_PVDD_2V5			(1 << 1)		/* 2.5V voltage for XSPI */

/* ETH ch0 voltage control register */
#define ETH_ch0_3_3				(0 << 0)
#define ETH_ch0_1_8				(1 << 0)
#define ETH_ch0_2_5				(2 << 0)

/* ETH ch1 voltage control register */
#define ETH_ch1_3_3				(0 << 0)
#define ETH_ch1_1_8				(1 << 0)
#define ETH_ch1_2_5				(2 << 0)

/* Write protection definition */
#define PWPR_B0WI_DISABLE_PFCWE	(1 << 7)
#define PWPR_B0WI_ENABLE_PFCWE	(0 << 7)
#define PWPR_PFCWE_ENABLE		(1 << 6)
#define PWPR_PFCWE_DISABLE		(0 << 6)

/* Register for setting the mode of ETH MII / RGMII */
#define ETH_MII_0_MII			(1 << 0)
#define ETH_MII_1_MII			(1 << 1)
#define XSPI_OEN_SORST_N		(1 << 0)

#define	PFC_SET_TBL_NUM			(11)
#define	PFC_OFF					(0)
#define PFC_ON					(1)

#define PFC_SCIF_TBL_NUM		(1)
#define PFC_XSPI_TBL_NUM		(2)
#define PFC_SD_TBL_NUM			(2)
#define PFC_RIIC_TBL_NUM		(1)

typedef struct {
	uint8_t		flg;
	uintptr_t	reg;
	uint8_t		val;
} pfc_reg_uint8_t;

typedef struct {
	uint8_t		flg;
	uintptr_t	reg;
	uint32_t	val;
} pfc_reg_uint32_t;

typedef struct {
	uint8_t		flg;
	uintptr_t	reg;
	uint64_t	val;
} pfc_reg_uint64_t;

typedef struct {
	pfc_reg_uint8_t		pmc;
	pfc_reg_uint32_t	pfc;
	pfc_reg_uint64_t	iolh;
	pfc_reg_uint64_t	pupd;
	pfc_reg_uint64_t	ien;
} pfc_regs_t;

#endif	/* __PFC_REGS_H__ */
