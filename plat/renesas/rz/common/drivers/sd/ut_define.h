/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**********************************************************************************************************************
 * File Name    : ut_define.h
 * Version      : 1.0
 * Description  : header file for ut.
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
#ifndef INC_UT_DEFINE_H_
#define INC_UT_DEFINE_H_

#ifdef _UT_
#define EXTERN static

#define SD_DRV_CODE_SEC
#define SD_DRV_RODATA_SEC
#define SD_DRV_RWDATA_SEC
#define SD_DRV_ZIDATA_SEC

#define SECURE_CODE_SEC
#define SECURE_RODATA_SEC
#define SECURE_RWDATA_SEC
#define SECURE_ZIDATA_SEC
#else
#define EXTERN extern

#define SD_DRV_CODE_SEC
#define SD_DRV_RODATA_SEC
#define SD_DRV_RWDATA_SEC
#define SD_DRV_ZIDATA_SEC

/* define of section for secure */
#define SECURE_CODE_SEC     __attribute__((section("CODE_SECURE")))
#define SECURE_RODATA_SEC   __attribute__((section("CONST_SECURE")))
#define SECURE_RWDATA_SEC   __attribute__((section("DATA_SECURE")))
#define SECURE_ZIDATA_SEC   __attribute__((section("BSS_SECURE")))
#endif

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

#endif /* INC_UT_DEFINE_H_ */
