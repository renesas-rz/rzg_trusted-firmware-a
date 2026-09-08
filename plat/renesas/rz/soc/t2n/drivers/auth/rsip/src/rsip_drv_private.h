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
/**********************************************************************************************************************
 * File Name    : rsip_drv_private.h
 * Version      : 1.0
 * Description  : Private RSIP driver header file.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 27.04.2023 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <endian.h>
#include "r_cip_drv_api.h"
#include "rsip_drv.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef RSIP_DRV_PRIVATE_H
#define RSIP_DRV_PRIVATE_H

#ifndef STATIC
#define STATIC static
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
extern void R_SCE_SoftwareResetPrivate(void);
extern cip_drv_ret_t R_SCE_SelfCheck1Private(void);

extern cip_drv_ret_t R_SCE_SelfCheck2Private(void);
extern cip_drv_ret_t R_SCE_CheckJtagAuthPrivate(uint32_t *auth_mode);
extern cip_drv_ret_t R_SCE_JtagAuthPrivate(uint32_t auth_mode);
extern cip_drv_ret_t R_SCE_HukLoadPrivate(uint32_t *huk);

extern cip_drv_ret_t R_SCE_VerifyManifestPrivate(uint32_t *InData_KeyCertificate,
		uint32_t *InData_KeyCertificateLength,
		uint32_t *InData_KeyCertificateSignature, uint32_t *InData_KeyCertificatePubKey, uint32_t *InData_ImgPkHash,
		uint32_t *InData_OemRootPkHash, uint32_t *InData_CodeCertificate, uint32_t *InData_CodeCertificateLength,
		uint32_t *InData_CodeCertificateSignature, uint32_t *InData_CodeCertificatePubKey,
		uint32_t *InData_ImageHash,
		uint32_t *InData_Image, uint32_t MAX_CNT);

extern cip_drv_ret_t R_SCE_Aes128CbcDecryptInitPrivate(st_sce_aes_wrapped_key_t *InData_KeyIndex,
		uint32_t *InData_IV);
extern cip_drv_ret_t R_SCE_Aes128CbcDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text,
		uint32_t MAX_CNT);
extern cip_drv_ret_t R_SCE_Aes128CbcDecryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);
extern cip_drv_ret_t R_SCE_Sha256InitPrivate(st_sce_sha_md5_handle_t *handle, uint32_t MAX_CNT);
extern cip_drv_ret_t R_SCE_Sha256UpdatePrivate(st_sce_sha_md5_handle_t *handle, uint32_t *InData_Message,
		uint32_t MAX_CNT);
extern cip_drv_ret_t R_SCE_Sha256FinalPrivate(st_sce_sha_md5_handle_t *handle, uint32_t *InData_Message,
		uint32_t MAX_CNT,
		uint32_t *OutData_MsgDigest, uint32_t *OutData_Length);
extern cip_drv_ret_t R_SCE_RandomNumberGeneratePrivate(uint32_t *OutData_Text);

#endif /* RSIP_DRV_PRIVATE_H */
