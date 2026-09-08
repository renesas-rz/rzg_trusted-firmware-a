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
* \file             : r_sb_build_config.h
* \par version      : 1.00
* \par Product Name : Renesas-SB-Lib FSBL Custom
* \par Device(s)    : Does not depend on specific hardware
* \par Description  : This file defines macros for build configuration.
**********************************************************************************************************************/
/**********************************************************************************************************************
* History : Version           DD.MM.YYYY Description
*         : 1.00              24.08.2020 First Release.
*         : FSBL Custom 1.00  27.10.2021 First Release.
**********************************************************************************************************************/
#ifndef R_SB_BUILD_CONFIG_H
/* Multiple inclusion protection macro */
#define R_SB_BUILD_CONFIG_H

/*!***********************************************************
 * \addtogroup  SBLIBBC
 * \{
 * \addtogroup  SBLIBBCDefines   BuildConfiguration Definitions
 * \}********************************************************/

/*=====================================================================================================================
 Public macro definitions
=====================================================================================================================*/
/*!*********************************************************
 * \addtogroup  SBLIBBCDefines
 * \{*******************************************************/
/** Select whether to check the setting value of the build configuration \n
	- 1: This config is valid
	- 0: This config is invalid */
#define SB_CFG_PARAM_CHECKING_ENABLE           (1U)

/** Select whether to enable or disable Secure Boot function by Mac verification \n
	- 1: This config is valid
	- 0: This config is invalid */
#define SB_CFG_SB_MAC_VERIFICATION             (0U)

/** Select whether to enable or disable Secure Boot function by Certificate chain verification \n
	- 1: This config is valid
	- 0: This config is invalid */
#define SB_CFG_SB_CERT_CHAIN_VERIFICATION      (1U)

/** Select whether to enable or disable Certificate chain verification using the IMG PK of the previous stage \n
	- 1: This config is valid
	- 0: This config is invalid */
#define SB_CFG_SB_CERT_CHAIN_USE_IMG_PK        (1U)

/** Select whether to enable or disable Image Encryption / Decryption function
	and Tenporary Encrypted Image Decryption function \n
	- 1: This config is valid
	- 0: This config is invalid */
#define SB_CFG_IMAGE_ENC_DEC                   (1U)

/** Select whether to enable or disable CRC check function \n
	- 1: This config is valid
	- 0: This config is invalid */
#define SB_CFG_CHECK_CRC                       (0U)

/** Select whether to enable or disable Integrity check function that use in Flash programmer stage \n
	- 1: This config is valid
	- 0: This config is invalid */
#define SB_CFG_CHECK_INTEGRITY                 (0U)

/*! \}*/

/*=====================================================================================================================
 Public type definitions
=====================================================================================================================*/

/*=====================================================================================================================
 Public global variables
=====================================================================================================================*/

/*=====================================================================================================================
 Public global functions
=====================================================================================================================*/

#endif  /* R_SB_BUILD_CONFIG_H */
/*=====================================================================================================================
 End of File
=====================================================================================================================*/
