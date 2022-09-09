/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**********************************************************************************************************************
 * File Name    : cpg_iodefine.h
 * Version      : 1.0
 * Description  : iodefine header file.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.09.2020 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef CPG_IODEFINE_H
#define CPG_IODEFINE_H

#define CPG_BASE_ADDR   (0x11010000uL)                          /* Base address of CPG */
#define CPG_PLL1_STBY       (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x000uL))/* PLL standby control register for CA55 */
#define CPG_PLL1_CLK1       (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x004uL))   /* PLL output set register1 for CA55 */
#define CPG_PLL1_CLK2       (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x008uL))   /* PLL output set register2 for CA55 */
#define CPG_PLL1_SETTING    (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x040uL))   /* PLL set register for CA55  */
#define CPG_PL1_DDIV        (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x200uL))   /* Divider register for CA55's clock */

#define CPG_PL3A_DDIV       (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x208uL))   /* Division rate setting register
                                                                                   for PLL3*/

#define CPG_PL2SDHI_DSEL    (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x218uL))   /* Source clock set register for SDHI*/

#define CPG_CLKSTATUS       (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x280uL))   /* Clock status monitor register */

#define CPG_PL3_SSEL        (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x408uL))   /* Source clock selecting register
                                                                                   for PLL3*/

#define CPG_CLKON_OSTM      (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x534uL))   /* Clock on/off register for OSTM0 */
#define CPG_CLKON_SPI_MULTI (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x550uL))   /* Clock on/off register for SPI */
#define CPG_CLKON_SDHI      (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x554uL))   /* Clock on/off register for SDHI0 */
#define CPG_CLKON_SCIF      (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x584uL))   /* Clock on/off register for SCIF  */
#define CPG_CLKON_GPIO      (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x598uL))   /* Clock on/off register for GPIO  */

#define CPG_CLKMON_OSTM     (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x6B4uL))   /* Clock monitor register for OSTM 0*/
#define CPG_CLKMON_SPI_MULTI    (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x6D0uL))   /* Clock monitor reg. for SPI */
#define CPG_CLKMON_SDHI     (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x6D4uL))   /* Clock monitor register for SDHI0 */
#define CPG_CLKMON_SCIF     (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x704uL))   /* Clock monitor register for SCIF  */
#define CPG_CLKMON_GPIO     (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x718uL))   /* Clock monitor register for GPIO  */

#define CPG_RST_OSTM        (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x834uL))   /* Reset on/off register for OSTM */

#define CPG_RST_SPI         (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x850uL))   /* Reset on/off register for SPI */
#define CPG_RST_SDHI        (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x854uL))   /* Reset on/off register for SDHI */
#define CPG_RST_SCIF        (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x884uL))   /* Reset on/off register for SCIF */
#define CPG_RST_GPIO        (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x898uL))   /* Reset on/off register for GPIO */

#define CPG_RSTMON_OSTM     (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x9B4uL))   /* Reset monitor register for OSTM */

#define CPG_RSTMON_SPI      (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x9D0uL))   /* Reset monitor register for SPI */
#define CPG_RSTMON_SDHI     (*(volatile uint32_t *)(CPG_BASE_ADDR + 0x9D4uL))   /* Reset monitor register for SDHI */
#define CPG_RSTMON_SCIF     (*(volatile uint32_t *)(CPG_BASE_ADDR + 0xA04uL))   /* Reset monitor register for SCIF */
#define CPG_RSTMON_GPIO     (*(volatile uint32_t *)(CPG_BASE_ADDR + 0xA18uL))   /* Reset monitor register for GPIO */

/*** CPG_PL1_DDIV ***/
#define CPG_PL1_WEN_MSK     (0x00010000uL)                      /* Mask bit pattern of write-enable */
#define CPG_PL1_WEN         (0x00010000uL)                      /* Write-enable  to this register */
#define CPG_PL1_WDIS        (0x00000000uL)                      /* Write-disable to this register */

#define CPG_PL1_DIV_MSK     (0x00000003uL)                      /* Mask bit pattern of division rate */
#define CPG_PL1_DIV1        (0x00000000uL)                      /* rate, 1/1 */
#define CPG_PL1_DIV2        (0x00000001uL)                      /* rate, 1/2 */
#define CPG_PL1_DIV4        (0x00000002uL)                      /* rate, 1/4 */
#define CPG_PL1_DIV8        (0x00000003uL)                      /* rate, 1/8 */

/*** Bit define of CPG_PL3A_DDIV register ***/
#define CPG_PL3A_DDIV_DIVPL3C_WEN           (0x01000000uL)      /* Write-enable DIVPL3C_WEN bit */
#define CPG_PL3A_DDIV_DIVPL3C_MASK          (0x00000700uL)      /* DIV_PLL3_C: mask */
#define CPG_PL3A_DDIV_DIVPL3C_SET_NODIV     (0x00000000uL)      /* DIV_PLL3_C: no divided */
#define CPG_PL3A_DDIV_DIVPL3C_SET_HLF       (0x00000100uL)      /* DIV_PLL3_C: half */
#define CPG_PL3A_DDIV_DIVPL3C_SET_DIV4      (0x00000200uL)      /* DIV_PLL3_C: divide by  4 */
#define CPG_PL3A_DDIV_DIVPL3C_SET_DIV8      (0x00000300uL)      /* DIV_PLL3_C: divide by  8 */
#define CPG_PL3A_DDIV_DIVPL3C_SET_DIV16     (0x00000400uL)      /* DIV_PLL3_C: divide by 16 */

/*** Bit define of CPG_PL3_SSEL register ***/
#define CPG_PL3_SSEL_WEN                    (0x01110000uL)      /* Write-enable Bit[8], Bit[4] and Bit[0] */
#define CPG_PL3_SSEL_FOUT3_2                (0x00000011uL)      /* FOUT3/2 system */

/*** Bit define of CPG_PL2SDHI_DSEL register ***/
#define CPG_PL2SDHI_DSEL_SDHI0_WEN          (0x00010000uL)      /* Write-enable  SEL_SDHI0_SET bit */
#define CPG_PL2SDHI_DSEL_SDHI0_WDIS         (0x00000000uL)      /* Write-disable SEL_SDHI0_SET bit */

#define CPG_PL2SDHI_DSEL_SDHI0_SET_MSK      (0x00000003uL)      /* Mask bit pattern of SEL_SDHI0_SET_MSK bit */
#define CPG_PL2SDHI_DSEL_SDHI0_CLK800       (0x00000000uL)      /* 800MHz */
#define CPG_PL2SDHI_DSEL_SDHI0_CLK533       (0x00000001uL)      /* 533MHz */
#define CPG_PL2SDHI_DSEL_SDHI0_PLL2_DIV8    (0x00000002uL)      /* 1/8  * PLL2 */
#define CPG_PL2SDHI_DSEL_SDHI0_PLL2_DIV12   (0x00000003uL)      /* 1/12 * PLL2 */

#define CPG_CLKSTATUS_SDHI0_MSK (0x10000000uL)                  /* Mask bit pattern of clock status for SDHI0 */
#define CPG_CLKSTATUS_SDHI0_BSY (0x10000000uL)                  /* Busy to change clock for SDHI0 */

/*** CPG_CLKON ***/
#define CPG_CLKON_WEN_CH0       (0x00010000uL)                  /* Write-enable  to this register */
#define CPG_CLKON_WDIS_CH0      (0x00000000uL)                  /* Write-disable to this register */


/*** Bit define of CPG_CLKON_SPIMULTI/CPG_CLKMON_SPIMULTI register ***/
#define CPG_CLKON_OSTM0_ON      (0x00000001uL)                  /* OSTM0_PCLK on */
#define CPG_CLKON_OSTM0_OFF     (0x00000000uL)

#define CPG_RST_OSTM0_OFF       (0x00000001uL)                  /* Reset off for OSTM0 */
#define CPG_RST_OSTM0_ON        (0x00000000uL)                  /* Reset on  for OSTM0 */
#define CPG_RST_OSTM0_WEN       (0x00010000uL)                  /* Write-enable UNIT0_RST bit for OSTM0 */
#define CPG_RSTMON_OSTM0_MSK    (0x00000001uL)
#define CPG_RSTMON_OSTM0_ON     (0x00000001uL)                  /* Monitor reset on for OSTM0 */
#define CPG_RSTMON_OSTM0_OFF    (0x00000000uL)                  /* Monitor reset off for OSTM0 */

/*** Bit define of CPG_CLKON_SPIMULTI/CPG_CLKMON_SPIMULTI register ***/
#define CPG_CLKON_SPIMULTI_ON   (0x00000003uL)                  /* SPI_CLK0 and SPI_CLK1 on */
#define CPG_CLKON_SPIMULTI_OFF  (0x00000000uL)
#define CPG_CLKON_SPIMULTI_WEN  (0x00030000uL)
#define CPG_CLKON_SPIMULTI_WDIS (0x00000000uL)
#define CPG_CLKMON_SPIMULTI_MSK (0x00000003uL)
#define CPG_CLKMON_SPIMULTI_ON  (0x00000003uL)
#define CPG_CLKMON_SPIMULTI_OFF (0x00000000uL)

#define CPG_RST_SPI0_OFF        (0x00000001uL)                  /* Reset off for SPI0 */
#define CPG_RST_SPI0_ON         (0x00000000uL)                  /* Reset on  for SPI0 */
#define CPG_RST_SPI0_WEN        (0x00010000uL)                  /* Write-enable UNIT0_RST bit for SPI0 */
#define CPG_RSTMON_SPI0_MSK     (0x00000001uL)
#define CPG_RSTMON_SPI0_ON      (0x00000001uL)                  /* Monitor reset on for SPI0 */
#define CPG_RSTMON_SPI0_OFF     (0x00000000uL)                  /* Monitor reset off for SPI0 */

/*** Bit define of CPG_CLKON_SDHI/CPG_CLKMON_SDHI register ***/
#define CPG_CLKON_SDHI0_ON      (0x0000000FuL)                  /* SDHI0_IMCLK/SDHI0_IMCLK2/SDHI0_CLK_HS/SDHI0_ACLK on*/
#define CPG_CLKON_SDHI0_OFF     (0x00000000uL)
#define CPG_CLKON_WEN_SDHI0     (0x000F0000uL)                  /* Write-enable  to this register */
#define CPG_CLKON_WDIS_SDHI0    (0x00000000uL)                  /* Write-disable to this register */
#define CPG_CLKMON_SDHI0_ON     (0x0000000FuL)                  /* Monitor clock on for SDHI0 */
#define CPG_CLKMON_SDHI0_OFF    (0x00000000uL)                  /* Monitor clock off for SDHI0 */

#define CPG_RST_SDHI0_ON        (0x00000000uL)                  /* Reset on for SDHI0 */
#define CPG_RST_SDHI0_OFF       (0x00000001uL)                  /* Reset off for SDHI0 */
#define CPG_RST_SDHI0_WEN       (0x00010000uL)                  /* Write-enable  UNIT0_RSTB bit for SDHI0 */
#define CPG_RST_SDHI0_WDIS      (0x00000000uL)                  /* Write-disable UNIT0_RSTB bit for SDHI0 */
#define CPG_RSTMON_SDHI0_ON     (0x00000001uL)                  /* Monitor reset on for SDHI0 */

/*** Bit define of CPG_CLKON_SCIF/CPG_CLKMON_SCIF register ***/
#define CPG_CLKON_SCIF0_ON      (0x00000001uL)                  /* CLK0_ON */
#define CPG_CLKON_SCIF0_OFF     (0x00000000uL)

#define CPG_RST_SCIF0_OFF       (0x00000001uL)                  /* Reset off for SCIF0 */
#define CPG_RST_SCIF0_WEN       (0x00010000uL)                  /* Write-enable  UNIT0_RSTB bit for SCIF0 */
#define CPG_RST_SCIF0_WDIS      (0x00000000uL)                  /* Write-disable UNIT0_RSTB bit for SCIF0 */
#define CPG_RSTMON_SCIF0_MSK    (0x00000001uL)
#define CPG_RSTMON_SCIF0_ON     (0x00000001uL)                  /* Monitor reset on for SCIF0 */
#define CPG_RSTMON_SCIF0_OFF    (0x00000000uL)                  /* Monitor reset off for SCIF0 */

/*** Bit define of CPG_CLKON_GPIO/CPG_CLKMON_GPIO register ***/
#define CPG_CLKON_GPIO_ON       (0x00000001uL)                  /* CLK0_ON */
#define CPG_CLKON_GPIO_OFF      (0x00000000uL)

#define CPG_RST_GPIO_OFF        (0x00000007uL)                  /* Reset off for GPIO */
#define CPG_RST_GPIO_WEN        (0x00070000uL)                  /* Write-enable  UNIT0_RSTB bit for GPIO */
#define CPG_RST_GPIO_WDIS       (0x00000000uL)                  /* Write-disable UNIT0_RSTB bit for GPIO */
#define CPG_RSTMON_GPIO_OFF     (0x00000000uL)                  /* Monitor reset off for GPIO */
#define CPG_RSTMON_GPIO_MSK     (0x00000007uL)                  /* Bit mask monitor reset off for GPIO */

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

#endif /* CPG_IODEFINE_H */
