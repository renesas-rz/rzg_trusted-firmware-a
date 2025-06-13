/**********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* www.renesas.com/disclaimer
*
* Copyright (C) 2019-2021 Renesas Electronics Corporation. All rights reserved.
**********************************************************************************************************************/
/*!********************************************************************************************************************
* \file             : r_sb_cmn.c
* \par version      : 1.00
* \par Product Name : Renesas-SB-Lib FSBL Custom
* \par Device(s)    : Does not depend on specific hardware
* \par Description  : This file implements common APIs for the SB-Lib.
**********************************************************************************************************************/
/**********************************************************************************************************************
* History : Version           DD.MM.YYYY Description
*         : 1.00              24.08.2020 First Release.
*         : FSBL Custom 1.00  27.10.2021 First Release.
**********************************************************************************************************************/

/* PRQA S 5002 ++ # D018 Exception according to file nomenclature */

/*=====================================================================================================================
 Includes <System Includes> , "Project Includes"
=====================================================================================================================*/
#include <stddef.h>
#include <stdint.h>
#include "r_sb_cmn.h"

/*=====================================================================================================================
 Private macro definitions
=====================================================================================================================*/

/*=====================================================================================================================
 Private type definitions
=====================================================================================================================*/

/*=====================================================================================================================
 Public global variables
=====================================================================================================================*/
/** Global variables used for flow verification */
/*!********************************************************************
 * \addtogroup SBLIBCommonVariablesInternal
 * \{******************************************************************/
volatile uint32_t g_sb_flow_counter;    /**< A variable that stores the count value for flow verification */
/*! \}*/

/*=====================================================================================================================
 Private global variables
=====================================================================================================================*/

/*=====================================================================================================================
 Private function prototypes
=====================================================================================================================*/

/*=====================================================================================================================
 Public function definitions
=====================================================================================================================*/

/*=====================================================================================================================
 Private function definitions
=====================================================================================================================*/

/*=====================================================================================================================
 End of file
=====================================================================================================================*/
