/*
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
* System Name  : SDHI Driver
* File Name    : r_sd_cfg.h
* Version      : 1.30
******************************************************************************/
#ifndef R_SD_CFG_H
#define R_SD_CFG_H

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
#define SD_CFG_IP0_BASE      (MMC0_SD_BASE)      /* Set the base address of SDHI ch0. */
#define SD_CFG_IP1_BASE      (MMC1_SD_BASE)      /* Set the base address of SDHI ch1. */

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
