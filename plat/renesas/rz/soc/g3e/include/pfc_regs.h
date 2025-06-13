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
#define PFC_P2A							(PFC_BASE + 0x002A)	/* Port register */
#define PFC_P2B							(PFC_BASE + 0x002B)	/* Port register */
#define PFC_P2C							(PFC_BASE + 0x002C)	/* Port register */
#define PFC_P2D							(PFC_BASE + 0x002D)	/* Port register */
#define PFC_P2E							(PFC_BASE + 0x002E)	/* Port register */
#define PFC_P2F							(PFC_BASE + 0x002F)	/* Port register */
#define PFC_P30							(PFC_BASE + 0x0030)	/* Port register */
#define PFC_P31							(PFC_BASE + 0x0031)	/* Port register */
#define PFC_P33							(PFC_BASE + 0x0033)	/* Port register */
#define PFC_P34							(PFC_BASE + 0x0034)	/* Port register */
#define PFC_P35							(PFC_BASE + 0x0035)	/* Port register */
#define PFC_P36							(PFC_BASE + 0x0036)	/* Port register */
#define PFC_P3C							(PFC_BASE + 0x003C)	/* Port register */

#define PFC_PM20						(PFC_BASE + 0x0140)	/* Port mode register */
#define PFC_PM21						(PFC_BASE + 0x0142)	/* Port mode register */
#define PFC_PM22						(PFC_BASE + 0x0144)	/* Port mode register */
#define PFC_PM23						(PFC_BASE + 0x0146)	/* Port mode register */
#define PFC_PM24						(PFC_BASE + 0x0148)	/* Port mode register */
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
#define PFC_PM33						(PFC_BASE + 0x0166)	/* Port mode register */
#define PFC_PM34						(PFC_BASE + 0x0168)	/* Port mode register */
#define PFC_PM35						(PFC_BASE + 0x016A)	/* Port mode register */
#define PFC_PM36						(PFC_BASE + 0x016C)	/* Port mode register */
#define PFC_PM3C						(PFC_BASE + 0x0178)	/* Port mode register */

#define PFC_PMC20						(PFC_BASE + 0x0220)	/* Port mode control register */
#define PFC_PMC21						(PFC_BASE + 0x0221)	/* Port mode control register */
#define PFC_PMC22						(PFC_BASE + 0x0222)	/* Port mode control register */
#define PFC_PMC23						(PFC_BASE + 0x0223)	/* Port mode control register */
#define PFC_PMC24						(PFC_BASE + 0x0224)	/* Port mode control register */
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
#define PFC_PMC33						(PFC_BASE + 0x0233)	/* Port mode control register */
#define PFC_PMC34						(PFC_BASE + 0x0234)	/* Port mode control register */
#define PFC_PMC35						(PFC_BASE + 0x0235)	/* Port mode control register */
#define PFC_PMC36						(PFC_BASE + 0x0236)	/* Port mode control register */
#define PFC_PMC3C						(PFC_BASE + 0x023C)	/* Port mode control register */

#define PFC_PFC20						(PFC_BASE + 0x0480)	/* Port function control register */
#define PFC_PFC21						(PFC_BASE + 0x0484)	/* Port function control register */
#define PFC_PFC22						(PFC_BASE + 0x0488)	/* Port function control register */
#define PFC_PFC23						(PFC_BASE + 0x048C)	/* Port function control register */
#define PFC_PFC24						(PFC_BASE + 0x0490)	/* Port function control register */
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
#define PFC_PFC33						(PFC_BASE + 0x04CC)	/* Port function control register */
#define PFC_PFC34						(PFC_BASE + 0x04D0)	/* Port function control register */
#define PFC_PFC35						(PFC_BASE + 0x04D4)	/* Port function control register */
#define PFC_PFC36						(PFC_BASE + 0x04D8)	/* Port function control register */
#define PFC_PFC3C						(PFC_BASE + 0x04F0)	/* Port function control register */


#define PFC_PIN20						(PFC_BASE + 0x0820)	/* Port terminal input register */
#define PFC_PIN21						(PFC_BASE + 0x0821)	/* Port terminal input register */
#define PFC_PIN22						(PFC_BASE + 0x0822)	/* Port terminal input register */
#define PFC_PIN23						(PFC_BASE + 0x0823)	/* Port terminal input register */
#define PFC_PIN24						(PFC_BASE + 0x0824)	/* Port terminal input register */
#define PFC_PIN25						(PFC_BASE + 0x0825)	/* Port terminal input register */
#define PFC_PIN26						(PFC_BASE + 0x0826)	/* Port terminal input register */
#define PFC_PIN27						(PFC_BASE + 0x0827)	/* Port terminal input register */
#define PFC_PIN28						(PFC_BASE + 0x0828)	/* Port terminal input register */
#define PFC_PIN2A						(PFC_BASE + 0x082A)	/* Port terminal input register */
#define PFC_PIN2B						(PFC_BASE + 0x082B)	/* Port terminal input register */
#define PFC_PIN2C						(PFC_BASE + 0x082C)	/* Port terminal input register */
#define PFC_PIN2D						(PFC_BASE + 0x082D)	/* Port terminal input register */
#define PFC_PIN2E						(PFC_BASE + 0x082E)	/* Port terminal input register */
#define PFC_PIN2F						(PFC_BASE + 0x082F)	/* Port terminal input register */
#define PFC_PIN30						(PFC_BASE + 0x0830)	/* Port terminal input register */
#define PFC_PIN31						(PFC_BASE + 0x0831)	/* Port terminal input register */
#define PFC_PIN33						(PFC_BASE + 0x0833)	/* Port terminal input register */
#define PFC_PIN34						(PFC_BASE + 0x0834)	/* Port terminal input register */
#define PFC_PIN35						(PFC_BASE + 0x0835)	/* Port terminal input register */
#define PFC_PIN36						(PFC_BASE + 0x0836)	/* Port terminal input register */
#define PFC_PIN3C						(PFC_BASE + 0x083C)	/* Port terminal input register */

#define PFC_IOLH05						(PFC_BASE + 0x1028)	/* IOLH switch register */
#define PFC_IOLH06						(PFC_BASE + 0x1030)	/* IOLH switch register */
#define PFC_IOLH09						(PFC_BASE + 0x1048)	/* IOLH switch register */
#define PFC_IOLH0A						(PFC_BASE + 0x1050)	/* IOLH switch register */
#define PFC_IOLH20						(PFC_BASE + 0x1100)	/* IOLH switch register */
#define PFC_IOLH21						(PFC_BASE + 0x1108)	/* IOLH switch register */
#define PFC_IOLH22						(PFC_BASE + 0x1110)	/* IOLH switch register */
#define PFC_IOLH23						(PFC_BASE + 0x1118)	/* IOLH switch register */
#define PFC_IOLH24						(PFC_BASE + 0x1120)	/* IOLH switch register */
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
#define PFC_IOLH33						(PFC_BASE + 0x1198)	/* IOLH switch register */
#define PFC_IOLH34						(PFC_BASE + 0x11A0)	/* IOLH switch register */
#define PFC_IOLH35						(PFC_BASE + 0x11A8)	/* IOLH switch register */
#define PFC_IOLH36						(PFC_BASE + 0x11B0)	/* IOLH switch register */
#define PFC_IOLH3C						(PFC_BASE + 0x11E0)	/* IOLH switch register */

#define PFC_SR05						(PFC_BASE + 0x1428)	/* Slew-Rate switch register */
#define PFC_SR06						(PFC_BASE + 0x1430)	/* Slew-Rate switch register */
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
#define PFC_SR2A						(PFC_BASE + 0x1550)	/* Slew-Rate switch register */
#define PFC_SR2B						(PFC_BASE + 0x1558)	/* Slew-Rate switch register */
#define PFC_SR2C						(PFC_BASE + 0x1560)	/* Slew-Rate switch register */
#define PFC_SR2D						(PFC_BASE + 0x1568)	/* Slew-Rate switch register */
#define PFC_SR2E						(PFC_BASE + 0x1570)	/* Slew-Rate switch register */
#define PFC_SR2F						(PFC_BASE + 0x1578)	/* Slew-Rate switch register */
#define PFC_SR30						(PFC_BASE + 0x1580)	/* Slew-Rate switch register */
#define PFC_SR31						(PFC_BASE + 0x1588)	/* Slew-Rate switch register */
#define PFC_SR33						(PFC_BASE + 0x1598)	/* Slew-Rate switch register */
#define PFC_SR34						(PFC_BASE + 0x15A0)	/* Slew-Rate switch register */
#define PFC_SR35						(PFC_BASE + 0x15A8)	/* Slew-Rate switch register */
#define PFC_SR36						(PFC_BASE + 0x15B0)	/* Slew-Rate switch register */
#define PFC_SR3C						(PFC_BASE + 0x15E0)	/* Slew-Rate switch register */

#define PFC_IEN09						(PFC_BASE + 0x1848)	/* IEN switch register */
#define PFC_IEN0A						(PFC_BASE + 0x1850)	/* IEN switch register */
#define PFC_IEN2A						(PFC_BASE + 0x1950)	/* IEN switch register */
#define PFC_IEN2D						(PFC_BASE + 0x1968)	/* IEN switch register */
#define PFC_IEN30						(PFC_BASE + 0x1980)	/* IEN switch register */
#define PFC_IEN31						(PFC_BASE + 0x1988)	/* IEN switch register */
#define PFC_IEN233						(PFC_BASE + 0x1998)	/* IEN switch register */

#define PFC_PUPD05						(PFC_BASE + 0x1C28)	/* PU/PD switch register */
#define PFC_PUPD06						(PFC_BASE + 0x1C30)	/* PU/PD switch register */
#define PFC_PUPD09						(PFC_BASE + 0x1C48)	/* PU/PD switch register */
#define PFC_PUPD0A						(PFC_BASE + 0x1C50)	/* PU/PD switch register */
#define PFC_PUPD20						(PFC_BASE + 0x1D00)	/* PU/PD switch register */
#define PFC_PUPD21						(PFC_BASE + 0x1D08)	/* PU/PD switch register */
#define PFC_PUPD22						(PFC_BASE + 0x1D10)	/* PU/PD switch register */
#define PFC_PUPD23						(PFC_BASE + 0x1D18)	/* PU/PD switch register */
#define PFC_PUPD24						(PFC_BASE + 0x1D20)	/* PU/PD switch register */
#define PFC_PUPD25						(PFC_BASE + 0x1D28)	/* PU/PD switch register */
#define PFC_PUPD26						(PFC_BASE + 0x1D30)	/* PU/PD switch register */
#define PFC_PUPD27						(PFC_BASE + 0x1D38)	/* PU/PD switch register */
#define PFC_PUPD28						(PFC_BASE + 0x1D40)	/* PU/PD switch register */
#define PFC_PUPD2A						(PFC_BASE + 0x1D50)	/* PU/PD switch register */
#define PFC_PUPD2B						(PFC_BASE + 0x1D58)	/* PU/PD switch register */
#define PFC_PUPD2C						(PFC_BASE + 0x1D60)	/* PU/PD switch register */
#define PFC_PUPD2D						(PFC_BASE + 0x1D68)	/* PU/PD switch register */
#define PFC_PUPD2E						(PFC_BASE + 0x1D70)	/* PU/PD switch register */
#define PFC_PUPD2F						(PFC_BASE + 0x1D78)	/* PU/PD switch register */
#define PFC_PUPD30						(PFC_BASE + 0x1D80)	/* PU/PD switch register */
#define PFC_PUPD31						(PFC_BASE + 0x1D88)	/* PU/PD switch register */
#define PFC_PUPD33						(PFC_BASE + 0x1D98)	/* PU/PD switch register */
#define PFC_PUPD34						(PFC_BASE + 0x1DA0)	/* PU/PD switch register */
#define PFC_PUPD35						(PFC_BASE + 0x1DA8)	/* PU/PD switch register */
#define PFC_PUPD36						(PFC_BASE + 0x1DB0)	/* PU/PD switch register */
#define PFC_PUPD3C						(PFC_BASE + 0x1DE0)	/* PU/PD switch register */

#define PFC_FILONOFF20					(PFC_BASE + 0x2100)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF21					(PFC_BASE + 0x2108)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF22					(PFC_BASE + 0x2110)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF23					(PFC_BASE + 0x2118)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF24					(PFC_BASE + 0x2120)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF25					(PFC_BASE + 0x2128)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF26					(PFC_BASE + 0x2130)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF27					(PFC_BASE + 0x2138)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF28					(PFC_BASE + 0x2140)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF2A					(PFC_BASE + 0x2150)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF2B					(PFC_BASE + 0x2158)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF2C					(PFC_BASE + 0x2160)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF2D					(PFC_BASE + 0x2168)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF2E					(PFC_BASE + 0x2170)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF2F					(PFC_BASE + 0x2178)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF30					(PFC_BASE + 0x2180)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF31					(PFC_BASE + 0x2188)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF33					(PFC_BASE + 0x2198)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF34					(PFC_BASE + 0x21A0)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF35					(PFC_BASE + 0x21A8)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF36					(PFC_BASE + 0x21B0)	/* Digital noise filter (FILONOFF) register */
#define PFC_FILONOFF3C					(PFC_BASE + 0x21E0)	/* Digital noise filter (FILONOFF) register */

#define PFC_FILNUM20					(PFC_BASE + 0x2500)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM21					(PFC_BASE + 0x2508)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM22					(PFC_BASE + 0x2510)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM23					(PFC_BASE + 0x2518)	/* Digital noise filter (FILNUM) register */
#define PFC_FILNUM24					(PFC_BASE + 0x2520)	/* Digital noise filter (FILNUM) register */
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

#define PFC_FILCLKSEL20					(PFC_BASE + 0x2900)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL21					(PFC_BASE + 0x2908)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL22					(PFC_BASE + 0x2910)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL23					(PFC_BASE + 0x2918)	/* Digital noise filter (FILCLKSEL) register */
#define PFC_FILCLKSEL24					(PFC_BASE + 0x2920)	/* Digital noise filter (FILCLKSEL) register */
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

#define PFC_ISEL20						(PFC_BASE + 0x2D00)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL21						(PFC_BASE + 0x2D08)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL22						(PFC_BASE + 0x2D10)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL23						(PFC_BASE + 0x2D18)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL24						(PFC_BASE + 0x2D20)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL25						(PFC_BASE + 0x2D28)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL26						(PFC_BASE + 0x2D30)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL27						(PFC_BASE + 0x2D38)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL28						(PFC_BASE + 0x2D40)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL2A						(PFC_BASE + 0x2D50)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL2B						(PFC_BASE + 0x2D58)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL2C						(PFC_BASE + 0x2D60)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL2D						(PFC_BASE + 0x2D68)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL2E						(PFC_BASE + 0x2D70)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL2F						(PFC_BASE + 0x2D78)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL30						(PFC_BASE + 0x2D80)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL31						(PFC_BASE + 0x2D88)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL33						(PFC_BASE + 0x2D98)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL34						(PFC_BASE + 0x2DA0)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL35						(PFC_BASE + 0x2DA8)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL36						(PFC_BASE + 0x2DB0)	/* Interrupt Select (ISEL) register */
#define PFC_ISEL3C						(PFC_BASE + 0x2DE0)	/* Interrupt Select (ISEL) register */

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
#define PFC_NOD2A						(PFC_BASE + 0x3150)	/* Nch Open Drain (NOD) register */
#define PFC_NOD2B						(PFC_BASE + 0x3158)	/* Nch Open Drain (NOD) register */
#define PFC_NOD2C						(PFC_BASE + 0x3160)	/* Nch Open Drain (NOD) register */
#define PFC_NOD2D						(PFC_BASE + 0x3168)	/* Nch Open Drain (NOD) register */
#define PFC_NOD2E						(PFC_BASE + 0x3170)	/* Nch Open Drain (NOD) register */
#define PFC_NOD2F						(PFC_BASE + 0x3178)	/* Nch Open Drain (NOD) register */
#define PFC_NOD30						(PFC_BASE + 0x3180)	/* Nch Open Drain (NOD) register */
#define PFC_NOD31						(PFC_BASE + 0x3188)	/* Nch Open Drain (NOD) register */
#define PFC_NOD33						(PFC_BASE + 0x3198)	/* Nch Open Drain (NOD) register */
#define PFC_NOD34						(PFC_BASE + 0x31A0)	/* Nch Open Drain (NOD) register */
#define PFC_NOD35						(PFC_BASE + 0x31A8)	/* Nch Open Drain (NOD) register */
#define PFC_NOD36						(PFC_BASE + 0x31B0)	/* Nch Open Drain (NOD) register */
#define PFC_NOD3C						(PFC_BASE + 0x31E0)	/* Nch Open Drain (NOD) register */

#define PFC_SMT20						(PFC_BASE + 0x3500)	/* Schmitt (SMT) register */
#define PFC_SMT21						(PFC_BASE + 0x3508)	/* Schmitt (SMT) register */
#define PFC_SMT23						(PFC_BASE + 0x3518)	/* Schmitt (SMT) register */
#define PFC_SMT24						(PFC_BASE + 0x3520)	/* Schmitt (SMT) register */
#define PFC_SMT25						(PFC_BASE + 0x3528)	/* Schmitt (SMT) register */
#define PFC_SMT26						(PFC_BASE + 0x3530)	/* Schmitt (SMT) register */
#define PFC_SMT27						(PFC_BASE + 0x3538)	/* Schmitt (SMT) register */
#define PFC_SMT28						(PFC_BASE + 0x3540)	/* Schmitt (SMT) register */
#define PFC_SMT2A						(PFC_BASE + 0x3550)	/* Schmitt (SMT) register */
#define PFC_SMT2B						(PFC_BASE + 0x3558)	/* Schmitt (SMT) register */
#define PFC_SMT2C						(PFC_BASE + 0x3560)	/* Schmitt (SMT) register */
#define PFC_SMT2D						(PFC_BASE + 0x3568)	/* Schmitt (SMT) register */
#define PFC_SMT2E						(PFC_BASE + 0x3570)	/* Schmitt (SMT) register */
#define PFC_SMT2F						(PFC_BASE + 0x3578)	/* Schmitt (SMT) register */
#define PFC_SMT30						(PFC_BASE + 0x3580)	/* Schmitt (SMT) register */
#define PFC_SMT31						(PFC_BASE + 0x3588)	/* Schmitt (SMT) register */
#define PFC_SMT33						(PFC_BASE + 0x3598)	/* Schmitt (SMT) register */
#define PFC_SMT34						(PFC_BASE + 0x35A0)	/* Schmitt (SMT) register */
#define PFC_SMT35						(PFC_BASE + 0x35A8)	/* Schmitt (SMT) register */
#define PFC_SMT36						(PFC_BASE + 0x35B0)	/* Schmitt (SMT) register */
#define PFC_SMT3C						(PFC_BASE + 0x35E0)	/* Schmitt (SMT) register */

#define PFC_PWPR						(PFC_BASE + 0x3C04)	/* Write protect */

#define PFC_OEN							(PFC_BASE + 0x3C40)	/* OEN switch register */

#define	PFC_OFF				(0)
#define PFC_ON				(1)

/* Controls writing to PFC and PMC */
#define PWPR_PFC_WE_A			(1 << 6)
/* Controls writing to OEN */
#define PWPR_PFC_WE_B			(1 << 5)

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
