/*
 * Copyright (C) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : rsip_drv_private.h
 * Version      : 1.0
 * Description  : Private RSIP driver header file.
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
extern cip_drv_ret_t R_SCE_CheckJtagAuthPrivate(uint32_t * auth_mode);
extern cip_drv_ret_t R_SCE_JtagAuthPrivate(uint32_t auth_mode);
extern cip_drv_ret_t R_SCE_HukLoadPrivate(uint32_t * huk);

extern cip_drv_ret_t R_SCE_VerifyManifestPrivate(uint32_t * InData_KeyCertificate,
        uint32_t * InData_KeyCertificateLength,
        uint32_t * InData_KeyCertificateSignature, uint32_t * InData_KeyCertificatePubKey, uint32_t * InData_ImgPkHash,
        uint32_t * InData_OemRootPkHash, uint32_t * InData_CodeCertificate, uint32_t * InData_CodeCertificateLength,
        uint32_t * InData_CodeCertificateSignature, uint32_t * InData_CodeCertificatePubKey,
        uint32_t * InData_ImageHash,
        uint32_t * InData_Image, uint32_t MAX_CNT);

extern cip_drv_ret_t R_SCE_Aes128CbcDecryptInitPrivate(st_sce_aes_wrapped_key_t * InData_KeyIndex,
        uint32_t * InData_IV);
extern cip_drv_ret_t R_SCE_Aes128CbcDecryptUpdatePrivate(uint32_t * InData_Text, uint32_t * OutData_Text,
        uint32_t MAX_CNT);
extern cip_drv_ret_t R_SCE_Aes128CbcDecryptFinalPrivate(uint32_t * OutData_Text, uint32_t * OutData_length);
extern cip_drv_ret_t R_SCE_Sha256InitPrivate(st_sce_sha_md5_handle_t * handle, uint32_t MAX_CNT);
extern cip_drv_ret_t R_SCE_Sha256UpdatePrivate(st_sce_sha_md5_handle_t * handle, uint32_t * InData_Message,
        uint32_t MAX_CNT);
extern cip_drv_ret_t R_SCE_Sha256FinalPrivate(st_sce_sha_md5_handle_t * handle, uint32_t * InData_Message,
        uint32_t MAX_CNT,
        uint32_t * OutData_MsgDigest, uint32_t * OutData_Length);
extern cip_drv_ret_t R_SCE_RandomNumberGeneratePrivate(uint32_t * OutData_Text);

#endif /* RSIP_DRV_PRIVATE_H */
