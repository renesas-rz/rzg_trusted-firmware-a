/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
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
#define PFC_P2B							(PFC_BASE + 0x002B)	/* Port register */

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
#define PFC_PM2B						(PFC_BASE + 0x0156)	/* Port mode register */

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
#define PFC_PMC2B						(PFC_BASE + 0x022B)	/* Port mode control register */

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
#define PFC_PFC2B						(PFC_BASE + 0x04AC)	/* Port function control register */

#define PFC_PIN20						(PFC_BASE + 0x0820)	/* Port terminal input register */
#define PFC_PIN21						(PFC_BASE + 0x0821)	/* Port terminal input register */
#define PFC_PIN22						(PFC_BASE + 0x0822)	/* Port terminal input register */
#define PFC_PIN23						(PFC_BASE + 0x0823)	/* Port terminal input register */
#define PFC_PIN24						(PFC_BASE + 0x0824)	/* Port terminal input register */
#define PFC_PIN25						(PFC_BASE + 0x0825)	/* Port terminal input register */
#define PFC_PIN26						(PFC_BASE + 0x0826)	/* Port terminal input register */
#define PFC_PIN27						(PFC_BASE + 0x0827)	/* Port terminal input register */
#define PFC_PIN28						(PFC_BASE + 0x0828)	/* Port terminal input register */
#define PFC_PIN29						(PFC_BASE + 0x0829)	/* Port terminal input register */
#define PFC_PIN2A						(PFC_BASE + 0x082A)	/* Port terminal input register */
#define PFC_PIN2B						(PFC_BASE + 0x082B)	/* Port terminal input register */

#define PFC_IOLH03						(PFC_BASE + 0x1018)	/* IOLH switch register */
#define PFC_IOLH05						(PFC_BASE + 0x1028)	/* IOLH switch register */
#define PFC_IOLH06						(PFC_BASE + 0x1030)	/* IOLH switch register */
#define PFC_IOLH07						(PFC_BASE + 0x1038)	/* IOLH switch register */
#define PFC_IOLH08						(PFC_BASE + 0x1040)	/* IOLH switch register */
#define PFC_IOLH09						(PFC_BASE + 0x1048)	/* IOLH switch register */
#define PFC_IOLH0A						(PFC_BASE + 0x1050)	/* IOLH switch register */
#define PFC_IOLH0B						(PFC_BASE + 0x1058)	/* IOLH switch register */
#define PFC_IOLH0C						(PFC_BASE + 0x1060)	/* IOLH switch register */
#define PFC_IOLH0E						(PFC_BASE + 0x1070)	/* IOLH switch register */
#define PFC_IOLH0F						(PFC_BASE + 0x1078)	/* IOLH switch register */
#define PFC_IOLH10						(PFC_BASE + 0x1080)	/* IOLH switch register */
#define PFC_IOLH11						(PFC_BASE + 0x1088)	/* IOLH switch register */
#define PFC_IOLH12						(PFC_BASE + 0x1090)	/* IOLH switch register */
#define PFC_IOLH13						(PFC_BASE + 0x1098)	/* IOLH switch register */
#define PFC_IOLH14						(PFC_BASE + 0x10A0)	/* IOLH switch register */
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
#define PFC_IOLH2B						(PFC_BASE + 0x1158)	/* IOLH switch register */

#define PFC_SR03						(PFC_BASE + 0x1418)	/* Slew-Rate switch register */
#define PFC_SR05						(PFC_BASE + 0x1428)	/* Slew-Rate switch register */
#define PFC_SR06						(PFC_BASE + 0x1430)	/* Slew-Rate switch register */
#define PFC_SR07						(PFC_BASE + 0x1438)	/* Slew-Rate switch register */
#define PFC_SR08						(PFC_BASE + 0x1440)	/* Slew-Rate switch register */
#define PFC_SR09						(PFC_BASE + 0x1448)	/* Slew-Rate switch register */
#define PFC_SR0A						(PFC_BASE + 0x1450)	/* Slew-Rate switch register */
#define PFC_SR0B						(PFC_BASE + 0x1458)	/* Slew-Rate switch register */
#define PFC_SR0C						(PFC_BASE + 0x1460)	/* Slew-Rate switch register */
#define PFC_SR0E						(PFC_BASE + 0x1470)	/* Slew-Rate switch register */
#define PFC_SR0F						(PFC_BASE + 0x1478)	/* Slew-Rate switch register */
#define PFC_SR10						(PFC_BASE + 0x1480)	/* Slew-Rate switch register */
#define PFC_SR11						(PFC_BASE + 0x1488)	/* Slew-Rate switch register */
#define PFC_SR12						(PFC_BASE + 0x1490)	/* Slew-Rate switch register */
#define PFC_SR13						(PFC_BASE + 0x1498)	/* Slew-Rate switch register */
#define PFC_SR14						(PFC_BASE + 0x14A0)	/* Slew-Rate switch register */
#define PFC_SR20						(PFC_BASE + 0x1500)	/* Slew-Rate switch register */
#define PFC_SR21						(PFC_BASE + 0x1508)	/* Slew-Rate switch register */
#define PFC_SR23						(PFC_BASE + 0x1518)	/* Slew-Rate switch register */
#define PFC_SR24						(PFC_BASE + 0x1520)	/* Slew-Rate switch register */
#define PFC_SR25						(PFC_BASE + 0x1528)	/* Slew-Rate switch register */
#define PFC_SR26						(PFC_BASE + 0x1530)	/* Slew-Rate switch register */
#define PFC_SR27						(PFC_BASE + 0x1538)	/* Slew-Rate switch register */
#define PFC_SR28						(PFC_BASE + 0x1540)	/* Slew-Rate switch register */
#define PFC_SR29						(PFC_BASE + 0x1548)	/* Slew-Rate switch register */
#define PFC_SR2A						(PFC_BASE + 0x1550)	/* Slew-Rate switch register */
#define PFC_SR2B						(PFC_BASE + 0x1558)	/* Slew-Rate switch register */

#define PFC_IEN03						(PFC_BASE + 0x1818)	/* IEN switch register */
#define PFC_IEN09						(PFC_BASE + 0x1848)	/* IEN switch register */
#define PFC_IEN0A						(PFC_BASE + 0x1850)	/* IEN switch register */
#define PFC_IEN0B						(PFC_BASE + 0x1858)	/* IEN switch register */
#define PFC_IEN0C						(PFC_BASE + 0x1860)	/* IEN switch register */
#define PFC_IEN0F						(PFC_BASE + 0x1878)	/* IEN switch register */
#define PFC_IEN12						(PFC_BASE + 0x1890)	/* IEN switch register */
#define PFC_IEN2B						(PFC_BASE + 0x1958)	/* IEN switch register */

#define PFC_PUPD05						(PFC_BASE + 0x1C28)	/* PU/PD switch register */
#define PFC_PUPD06						(PFC_BASE + 0x1C30)	/* PU/PD switch register */
#define PFC_PUPD07						(PFC_BASE + 0x1C38)	/* PU/PD switch register */
#define PFC_PUPD08						(PFC_BASE + 0x1C40)	/* PU/PD switch register */
#define PFC_PUPD09						(PFC_BASE + 0x1C48)	/* PU/PD switch register */
#define PFC_PUPD0A						(PFC_BASE + 0x1C50)	/* PU/PD switch register */
#define PFC_PUPD0B						(PFC_BASE + 0x1C58)	/* PU/PD switch register */
#define PFC_PUPD0C						(PFC_BASE + 0x1C60)	/* PU/PD switch register */
#define PFC_PUPD0F						(PFC_BASE + 0x1C78)	/* PU/PD switch register */
#define PFC_PUPD10						(PFC_BASE + 0x1C80)	/* PU/PD switch register */
#define PFC_PUPD11						(PFC_BASE + 0x1C88)	/* PU/PD switch register */
#define PFC_PUPD12						(PFC_BASE + 0x1C90)	/* PU/PD switch register */
#define PFC_PUPD13						(PFC_BASE + 0x1C98)	/* PU/PD switch register */
#define PFC_PUPD14						(PFC_BASE + 0x1CA0)	/* PU/PD switch register */
#define PFC_PUPD20						(PFC_BASE + 0x1D00)	/* PU/PD switch register */
#define PFC_PUPD21						(PFC_BASE + 0x1D08)	/* PU/PD switch register */
#define PFC_PUPD22						(PFC_BASE + 0x1D10)	/* PU/PD switch register */
#define PFC_PUPD23						(PFC_BASE + 0x1D18)	/* PU/PD switch register */
#define PFC_PUPD24						(PFC_BASE + 0x1D20)	/* PU/PD switch register */
#define PFC_PUPD25						(PFC_BASE + 0x1D28)	/* PU/PD switch register */
#define PFC_PUPD26						(PFC_BASE + 0x1D30)	/* PU/PD switch register */
#define PFC_PUPD27						(PFC_BASE + 0x1D38)	/* PU/PD switch register */
#define PFC_PUPD28						(PFC_BASE + 0x1D40)	/* PU/PD switch register */
#define PFC_PUPD29						(PFC_BASE + 0x1D48)	/* PU/PD switch register */
#define PFC_PUPD2A						(PFC_BASE + 0x1D50)	/* PU/PD switch register */
#define PFC_PUPD2B						(PFC_BASE + 0x1D58)	/* PU/PD switch register */

#define PFC_FILONOFF01					(PFC_BASE + 0x2008)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF20					(PFC_BASE + 0x2100)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF21					(PFC_BASE + 0x2108)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF22					(PFC_BASE + 0x2110)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF23					(PFC_BASE + 0x2118)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF24					(PFC_BASE + 0x2120)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF25					(PFC_BASE + 0x2128)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF26					(PFC_BASE + 0x2130)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF27					(PFC_BASE + 0x2138)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF28					(PFC_BASE + 0x2140)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF29					(PFC_BASE + 0x2148)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF2A					(PFC_BASE + 0x2150)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF2B					(PFC_BASE + 0x2158)	/* Digital noise filter (FILONOFF) register */

#define PFC_FILNUM01					(PFC_BASE + 0x2408)	/* Digital noise filter (FILNUM) register */
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
#define PFC_FILNUM2B					(PFC_BASE + 0x2558)	/* Digital noise filter (FILNUM) register */

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
#define PFC_FILCLKSEL2B					(PFC_BASE + 0x2958)	/* Digital noise filter (FILCLKSEL) register */

#define PFC_ISEL20						(PFC_BASE + 0x2D00)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL21						(PFC_BASE + 0x2D08)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL22						(PFC_BASE + 0x2D10)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL23						(PFC_BASE + 0x2D18)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL24						(PFC_BASE + 0x2D20)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL25						(PFC_BASE + 0x2D28)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL26						(PFC_BASE + 0x2D30)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL27						(PFC_BASE + 0x2D38)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL28						(PFC_BASE + 0x2D40)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL29						(PFC_BASE + 0x2D48)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL2A						(PFC_BASE + 0x2D50)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL2B						(PFC_BASE + 0x2D58)	/* Interrupt Select (ISEL) register */

#define PFC_NOD05						(PFC_BASE + 0x3028)	/* Nch Open Drain (NOD) register */
#define PFC_NOD20						(PFC_BASE + 0x3100)	/* Nch Open Drain (NOD) register */
#define PFC_NOD21						(PFC_BASE + 0x3108)	/* Nch Open Drain (NOD) register */
#define PFC_NOD22						(PFC_BASE + 0x3110)	/* Nch Open Drain (NOD) register */
#define PFC_NOD23						(PFC_BASE + 0x3118)	/* Nch Open Drain (NOD) register */
#define PFC_NOD24						(PFC_BASE + 0x3120)	/* Nch Open Drain (NOD) register */
#define PFC_NOD25						(PFC_BASE + 0x3128)	/* Nch Open Drain (NOD) register */
#define PFC_NOD26						(PFC_BASE + 0x3130)	/* Nch Open Drain (NOD) register */
#define PFC_NOD27						(PFC_BASE + 0x3138)	/* Nch Open Drain (NOD) register */
#define PFC_NOD28						(PFC_BASE + 0x3140)	/* Nch Open Drain (NOD) register */
#define PFC_NOD29						(PFC_BASE + 0x3148)	/* Nch Open Drain (NOD) register */
#define PFC_NOD2A						(PFC_BASE + 0x3150)	/* Nch Open Drain (NOD) register */
#define PFC_NOD2B						(PFC_BASE + 0x3158)	/* Nch Open Drain (NOD) register */

#define PFC_SMT20						(PFC_BASE + 0x3500)	/* Schmitt (SMT) register */
#define PFC_SMT21						(PFC_BASE + 0x3508)	/* Schmitt (SMT) register */
#define PFC_SMT23						(PFC_BASE + 0x3518)	/* Schmitt (SMT) register */
#define PFC_SMT24						(PFC_BASE + 0x3520)	/* Schmitt (SMT) register */
#define PFC_SMT25						(PFC_BASE + 0x3528)	/* Schmitt (SMT) register */
#define PFC_SMT26						(PFC_BASE + 0x3530)	/* Schmitt (SMT) register */
#define PFC_SMT27						(PFC_BASE + 0x3538)	/* Schmitt (SMT) register */
#define PFC_SMT28						(PFC_BASE + 0x3540)	/* Schmitt (SMT) register */
#define PFC_SMT29						(PFC_BASE + 0x3548)	/* Schmitt (SMT) register */
#define PFC_SMT2A						(PFC_BASE + 0x3550)	/* Schmitt (SMT) register */
#define PFC_SMT2B						(PFC_BASE + 0x3558)	/* Schmitt (SMT) register */

#define PFC_PWPR						(PFC_BASE + 0x3C04)	/* Write protect */

#define PFC_OEN							(PFC_BASE + 0x3C40)	/* OEN switch register */

/* Definition for port register */
#define P_P0				(1 << 0)
#define P_P1				(1 << 1)
#define P_P2				(1 << 2)
#define P_P3				(1 << 3)
#define P_P4				(1 << 4)
#define P_P5				(1 << 5)
#define P_P6				(1 << 6)
#define P_P7				(1 << 7)

/* Definition for port mode register */
#define PM0_HIZ				(0 << 0)
#define PM0_IN				(1 << 0)
#define PM0_OUT_DIS			(2 << 0)
#define PM0_OUT_EN			(3 << 0)
#define PM1_HIZ				(0 << 2)
#define PM1_IN				(1 << 2)
#define PM1_OUT_DIS			(2 << 2)
#define PM1_OUT_EN			(3 << 2)
#define PM2_HIZ				(0 << 4)
#define PM2_IN				(1 << 4)
#define PM2_OUT_DIS			(2 << 4)
#define PM2_OUT_EN			(3 << 4)
#define PM3_HIZ				(0 << 6)
#define PM3_IN				(1 << 6)
#define PM3_OUT_DIS			(2 << 6)
#define PM3_OUT_EN			(3 << 6)
#define PM4_HIZ				(0 << 8)
#define PM4_IN				(1 << 8)
#define PM4_OUT_DIS			(2 << 8)
#define PM4_OUT_EN			(3 << 8)
#define PM5_HIZ				(0 << 10)
#define PM5_IN				(1 << 10)
#define PM5_OUT_DIS			(2 << 10)
#define PM5_OUT_EN			(3 << 10)
#define PM6_HIZ				(0 << 12)
#define PM6_IN				(1 << 12)
#define PM6_OUT_DIS			(2 << 12)
#define PM6_OUT_EN			(3 << 12)
#define PM7_HIZ				(0 << 14)
#define PM7_IN				(1 << 14)
#define PM7_OUT_DIS			(2 << 14)
#define PM7_OUT_EN			(3 << 14)

/* Definition for port mode control register */
#define PMC_PMC0			(1 << 0)
#define PMC_PMC1			(1 << 1)
#define PMC_PMC2			(1 << 2)
#define PMC_PMC3			(1 << 3)
#define PMC_PMC4			(1 << 4)
#define PMC_PMC5			(1 << 5)
#define PMC_PMC6			(1 << 6)
#define PMC_PMC7			(1 << 7)

/* Definition for terminal input register */
#define PMC_PIN0			(1 << 0)
#define PMC_PIN1			(1 << 1)
#define PMC_PIN2			(1 << 2)
#define PMC_PIN3			(1 << 3)
#define PMC_PIN4			(1 << 4)
#define PMC_PIN5			(1 << 5)
#define PMC_PIN6			(1 << 6)
#define PMC_PIN7			(1 << 7)

/* Definition for IOLH switch register */
#define IOLH0_X1			(0 << 0)
#define IOLH0_X2			(1 << 0)
#define IOLH0_X4			(2 << 0)
#define IOLH0_X6			(3 << 0)
#define IOLH1_X1			(0 << 8)
#define IOLH1_X2			(1 << 8)
#define IOLH1_X4			(2 << 8)
#define IOLH1_X6			(3 << 8)
#define IOLH2_X1			(0 << 16)
#define IOLH2_X2			(1 << 16)
#define IOLH2_X4			(2 << 16)
#define IOLH2_X6			(3 << 16)
#define IOLH3_X1			(0 << 24)
#define IOLH3_X2			(1 << 24)
#define IOLH3_X4			(2 << 24)
#define IOLH3_X6			(3 << 24)

/* Definition for Slew-Rate switch register */
#define SR0_FAST			(1 << 0)
#define SR1_FAST			(1 << 8)
#define SR2_FAST			(1 << 16)
#define SR3_FAST			(1 << 24)

/* Definition for IEN switching register */
#define IEN0_ENABLE			(1 << 0)
#define IEN1_ENABLE			(1 << 8)
#define IEN2_ENABLE			(1 << 16)
#define IEN3_ENABLE			(1 << 24)

/* Definition for PUPD switching register */
#define PUPD0_NO			(1 << 0)
#define PUPD0_DOWN			(2 << 0)
#define PUPD0_UP			(3 << 0)
#define PUPD1_NO			(1 << 8)
#define PUPD1_DOWN			(2 << 8)
#define PUPD1_UP			(3 << 8)
#define PUPD2_NO			(1 << 16)
#define PUPD2_DOWN			(2 << 16)
#define PUPD2_UP			(3 << 16)
#define PUPD3_NO			(1 << 24)
#define PUPD3_DOWN			(2 << 24)
#define PUPD3_UP			(3 << 24)

/* Digital noise filter (FILONOFF) register */
#define FILONOFF_FILON0		(1 << 0)
#define FILONOFF_FILON1		(1 << 8)
#define FILONOFF_FILON2		(1 << 16)
#define FILONOFF_FILON3		(1 << 24)

/* Digital noise filter (FILNUM) register */
#define FILNUM_FILNUM0_0	(0 << 0)
#define FILNUM_FILNUM0_1	(1 << 0)
#define FILNUM_FILNUM0_2	(2 << 0)
#define FILNUM_FILNUM0_3	(3 << 0)
#define FILNUM_FILNUM1_0	(0 << 8)
#define FILNUM_FILNUM1_1	(1 << 8)
#define FILNUM_FILNUM1_2	(2 << 8)
#define FILNUM_FILNUM1_3	(3 << 8)
#define FILNUM_FILNUM2_0	(0 << 16)
#define FILNUM_FILNUM2_1	(1 << 16)
#define FILNUM_FILNUM2_2	(2 << 16)
#define FILNUM_FILNUM2_3	(3 << 16)
#define FILNUM_FILNUM3_0	(0 << 24)
#define FILNUM_FILNUM3_1	(1 << 24)
#define FILNUM_FILNUM3_2	(2 << 24)
#define FILNUM_FILNUM3_3	(3 << 24)

/* Digital noise filter (FILCLKSEL) register definition */
#define FILCLKSEL_FILCLK0_0	(0 << 0)
#define FILCLKSEL_FILCLK0_1	(1 << 0)
#define FILCLKSEL_FILCLK0_2	(2 << 0)
#define FILCLKSEL_FILCLK0_3	(3 << 0)
#define FILCLKSEL_FILCLK1_0	(0 << 8)
#define FILCLKSEL_FILCLK1_1	(1 << 8)
#define FILCLKSEL_FILCLK1_2	(2 << 8)
#define FILCLKSEL_FILCLK1_3	(3 << 8)
#define FILCLKSEL_FILCLK2_0	(0 << 16)
#define FILCLKSEL_FILCLK2_1	(1 << 16)
#define FILCLKSEL_FILCLK2_2	(2 << 16)
#define FILCLKSEL_FILCLK2_3	(3 << 16)
#define FILCLKSEL_FILCLK3_0	(0 << 24)
#define FILCLKSEL_FILCLK3_1	(1 << 24)
#define FILCLKSEL_FILCLK3_2	(2 << 24)
#define FILCLKSEL_FILCLK3_3	(3 << 24)

/*ISEL register definition */
#define ISEL0				(1 << 0)
#define ISEL1				(1 << 8)
#define ISEL2				(1 << 16)
#define ISEL3				(1 << 24)

/* Write protection definition */
#define PWPR_REGWE_A		(1 << 6)				/* PFC & PMC Write Enable */
#define PWPR_REGWE_B		(1 << 5)				/* OEN Write Enable */

/* OEN bits definition */
#define OEN_ET0				(1 << 0)				/* OEN: ET0_TXC_TX_CLK */
#define OEN_ET1				(1 << 1)				/* OEN: ET1_TXC_TX_CLK */
#define OEN_XSPI_RESET0_N	(1 << 2)				/* OEN: 1=OE of IO Block is OFF */
#define OEN_XSPI_CS0N		(1 << 3)				/* OEN: 1=OE of IO Block is OFF */
#define OEN_XSPI_CLKN		(1 << 4)				/* OEN: 1=OE of IO Block is OFF */
#define OEN_XSPI_CLKP		(1 << 5)				/* OEN: 1=OE of IO Block is OFF */

#define	PFC_OFF				(0)
#define PFC_ON				(1)


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

typedef struct {
	uint32_t	enable_mask;
	uint32_t	drive_mask;
	uint32_t	drive_offset;
} PFC_IO_DRIVE;

#endif	/* __PFC_REGS_H__ */
