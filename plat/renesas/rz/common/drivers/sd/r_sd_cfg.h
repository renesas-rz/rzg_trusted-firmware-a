/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/*******************************************************************************
* System Name  : SDHI Driver
* File Name    : r_sd_cfg.h
* Version      : 1.30
******************************************************************************/
#ifndef R_SD_CFG_H
#define R_SD_CFG_H

#include <rz_soc_def.h>						/* Get the SDHI Base addresses and SD_CFG_BASE (if default value is overridden)*/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
/* ------------------------------------------------------
	Set SDHI Base Address
--------------------------------------------------------*/
#ifndef SD_CFG_BASE
#define SD_CFG_BASE			MMC0_SD_BASE
#endif

/* ------------------------------------------------------
	Set the method of check SD Status
--------------------------------------------------------*/
#define SD_CFG_POLL

/* ------------------------------------------------------
	Set the method of data transfer
--------------------------------------------------------*/
#define SD_CFG_TRNS_SW

/* ------------------------------------------------------
	Set the card type to support
--------------------------------------------------------*/
#define SD_CFG_MEM

/* ------------------------------------------------------
	Set the version to support
--------------------------------------------------------*/
/* #define SD_CFG_VER1X */            /* Version 1.1 */
#define SD_CFG_VER2X            /* Version 2.x */

/* ------------------------------------------------------
	Set the method to detect card
--------------------------------------------------------*/

		/* the SD bus width */
		#define SD_CFG_DRIVER_MODE_BUS_WIDTH    (SD_MODE_4BIT)

		/* the method of check SD Status */
		#define SD_CFG_DRIVER_MODE_METHOD       (SD_MODE_POLL)

		/* the method of data transfer */
		#define SD_CFG_DRIVER_MODE_TRANSFER     (SD_MODE_SW)

		/* the card type to support */
		#define SD_CFG_DRIVER_MODE_CARD_TYPE    (SD_MODE_MEM)

		/* the speed to support */
		#define SD_CFG_DRIVER_MODE_SPEED        (SD_MODE_DS)

#if    defined(SD_CFG_VER2X)
		/* the version to support */
		#define SD_CFG_DRIVER_MODE_VER          (SD_MODE_VER2X)
#else
		/* the version to support */
		#define SD_CFG_DRIVER_MODE_VER          (SD_MODE_VER1X)
#endif

/* Set driver mode configration */
#define SD_CFG_DRIVER_MODE                  (SD_CFG_DRIVER_MODE_BUS_WIDTH | \
																						SD_CFG_DRIVER_MODE_METHOD     | \
																						SD_CFG_DRIVER_MODE_TRANSFER   | \
																						SD_CFG_DRIVER_MODE_CARD_TYPE  | \
																						SD_CFG_DRIVER_MODE_SPEED      | \
																						SD_CFG_DRIVER_MODE_VER)

/* ==== end of the setting ==== */



/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

#endif /* R_SD_CFG_H    */

/* End of File */
