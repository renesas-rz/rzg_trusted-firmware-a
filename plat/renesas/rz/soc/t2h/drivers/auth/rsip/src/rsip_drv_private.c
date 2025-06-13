/*
 * Copyright (C) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : rsip_drv_private.c
 * Version      : 1.0
 * Description  : Private RSIP driver functions.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <lib/mmio.h>
#include <r_cip_drv_api.h>
#include "rsip_drv.h"
#include "rsip_drv_private.h"

#include "rsip_p/r_rsip_primitive.h"
#include "rsip_p/r_rsip_addr.h"
#include "rsip_p/r_rsip_err.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#define RSIP_SELFCHECK2_RETRY_COUNT     (3)

/* procedure 06 flag */
#define RSIP_JAM_RSIP_EN                (0x00000001uL)
#define RSIP_JAM_RSIP_DI                (0x00000000uL)

/* Authenticate id buffer size */
#define RSIP_AUTH_ID_WORD_SIZE          (4)

/* Input endian */
#define RSIP_1424H_ALL_LE               (0x00CF00CFuL)
#define RSIP_1424H_ALL_BE               (0x00CF0000uL)
#define RSIP_1424H_ENDIAN               (RSIP_1424H_ALL_LE)

/* Output endian */
#define RSIP_1428H_ALL_LE               (0x00CF00CFuL)
#define RSIP_1428H_ALL_BE               (0x00CF0000uL)
#define RSIP_1428H_ENDIAN               (RSIP_1428H_ALL_LE)

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCE_VerifyManifestPrivate
 * Description  : .
 * Arguments    : InData_KeyCertificate
 *              : InData_KeyCertificateLength
 *              : InData_KeyCertificateSignature
 *              : InData_KeyCertificatePubKey
 *              : InData_ImgPkHash
 *              : InData_OemRootPkHash
 *              : InData_CodeCertificate
 *              : InData_CodeCertificateLength
 *              : InData_CodeCertificateSignature
 *              : InData_CodeCertificatePubKey
 *              : InData_ImageHash
 *              : InData_Image
 *              : MAX_CNT
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t R_SCE_VerifyManifestPrivate(uint32_t * InData_KeyCertificate, uint32_t * InData_KeyCertificateLength,
        uint32_t * InData_KeyCertificateSignature, uint32_t * InData_KeyCertificatePubKey, uint32_t * InData_ImgPkHash,
        uint32_t * InData_OemRootPkHash, uint32_t * InData_CodeCertificate, uint32_t * InData_CodeCertificateLength,
        uint32_t * InData_CodeCertificateSignature, uint32_t * InData_CodeCertificatePubKey,
        uint32_t * InData_ImageHash,
        uint32_t * InData_Image, uint32_t MAX_CNT)
{
    cip_drv_ret_t ret_val = CIP_DRV_RET_FAIL;
    rsip_ret_t err_val;
    uint32_t indata_cmd[1];

    if (NULL == InData_ImageHash)
    {
        indata_cmd[0] = bswap32(0x00000000);
    }
    else
    {
        indata_cmd[0] = bswap32(0x00000001);
    }

    err_val = r_rsip_p24(indata_cmd, InData_KeyCertificate, InData_KeyCertificateLength,
            InData_KeyCertificateSignature, InData_KeyCertificatePubKey, InData_ImgPkHash,
            InData_OemRootPkHash, InData_CodeCertificate, InData_CodeCertificateLength,
            InData_CodeCertificateSignature, InData_CodeCertificatePubKey, InData_ImageHash,
            InData_Image, MAX_CNT);
    switch (err_val)
    {
        case RSIP_RET_PASS:
        {
            ret_val = CIP_DRV_RET_PASS;
            break;
        }
        case RSIP_RET_VERIFICATION_FAIL:
        {
            ret_val = CIP_DRV_RET_AUTH_FAIL;
            break;
        }
        case RSIP_RET_PARAM_FAIL:
        {
            ret_val = CIP_DRV_RET_FAIL;
            break;
        }
        case RSIP_RET_RESOURCE_CONFLICT:
        {
            ret_val = CIP_DRV_RET_RESOURCE_CONFLICT;
            break;
        }
        default:
        {
            ret_val = CIP_DRV_RET_FAIL;
            break;
        }
    }
    return ret_val;
}
/**********************************************************************************************************************
 End of function R_SCE_VerifyManifestPrivate
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCE_Aes128CbcDecryptInitPrivate
 * Description  : .
 * Arguments    : InData_KeyIndex
 *              : InData_IV
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t R_SCE_Aes128CbcDecryptInitPrivate(st_sce_aes_wrapped_key_t * InData_KeyIndex, uint32_t * InData_IV)
{
    cip_drv_ret_t ret_val = CIP_DRV_RET_FAIL;
    rsip_ret_t err_val;
    uint32_t indata_cmd[1];

    if (SCE_KEY_INDEX_TYPE_AES128 != InData_KeyIndex->type)
    {
        return CIP_DRV_RET_KEY_FAIL;
    }
    indata_cmd[0] = bswap32(0x00000003);

    err_val = r_rsip_p47i(indata_cmd, InData_KeyIndex->value, InData_IV);
    switch (err_val)
    {
        case RSIP_RET_PASS:
        {
            ret_val = CIP_DRV_RET_PASS;
            break;
        }
        case RSIP_RET_KEY_FAIL:
        {
            ret_val = CIP_DRV_RET_KEY_FAIL;
            break;
        }
        case RSIP_RET_RESOURCE_CONFLICT:
        {
            ret_val = CIP_DRV_RET_RESOURCE_CONFLICT;
            break;
        }
        default:
        {
            ret_val = CIP_DRV_RET_FAIL;
            break;
        }
    }

    return ret_val;
}
/**********************************************************************************************************************
 End of function R_SCE_Aes128CbcDecryptInitPrivate
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCE_Aes128CbcDecryptUpdatePrivate
 * Description  : .
 * Arguments    : InData_Text
 *              : OutData_Text
 *              : MAX_CNT
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t R_SCE_Aes128CbcDecryptUpdatePrivate(uint32_t * InData_Text, uint32_t * OutData_Text, uint32_t MAX_CNT)
{
    r_rsip_p47u(InData_Text, OutData_Text, MAX_CNT);
    return CIP_DRV_RET_PASS;
}
/**********************************************************************************************************************
 End of function R_SCE_Aes128CbcDecryptUpdatePrivate
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCE_Aes128CbcDecryptFinalPrivate
 * Description  : .
 * Arguments    : OutData_Text
 *              : OutData_length
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t R_SCE_Aes128CbcDecryptFinalPrivate(uint32_t * OutData_Text, uint32_t * OutData_length)
{
    cip_drv_ret_t ret_val = CIP_DRV_RET_FAIL;
    rsip_ret_t err_val;

    err_val = r_rsip_p47f();
    switch (err_val)
    {
        case RSIP_RET_PASS:
        {
            ret_val = CIP_DRV_RET_PASS;
            break;
        }
        default:
        {
            ret_val = CIP_DRV_RET_FAIL;
            break;
        }
    }

    return ret_val;
}
/**********************************************************************************************************************
 End of function R_SCE_Aes128CbcDecryptFinalPrivate
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCE_Sha256InitPrivate
 * Description  : .
 * Arguments    : handle
 *              : MAX_CNT
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t R_SCE_Sha256InitPrivate(st_sce_sha_md5_handle_t * handle, uint32_t MAX_CNT)
{
    return CIP_DRV_RET_PASS;
}
/**********************************************************************************************************************
 End of function R_SCE_Sha256InitPrivate
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCE_Sha256UpdatePrivate
 * Description  : .
 * Arguments    : handle
 *              : InData_Message
 *              : MAX_CNT
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t R_SCE_Sha256UpdatePrivate(st_sce_sha_md5_handle_t * handle, uint32_t * InData_Message, uint32_t MAX_CNT)
{
    cip_drv_ret_t ret_val = CIP_DRV_RET_FAIL;
    rsip_ret_t err_val;
    uint32_t indata_hash_type[1];
    uint32_t indata_cmd[1];
    uint32_t indata_msg_len[2];

    indata_hash_type[0] = bswap32(0x00000002); /* SHA-256 */
    indata_cmd[0] = bswap32(0x00000001); /* "Init" to "Suspend" */
    indata_msg_len[0] = bswap32((uint32_t)(((uint64_t )handle->all_received_length * 8) >> 32));
    indata_msg_len[1] = bswap32((uint32_t)((uint64_t )handle->all_received_length * 8));

    err_val = r_rsip_p72(indata_hash_type, indata_cmd, InData_Message, indata_msg_len,
            (uint32_t*) NULL, MAX_CNT, (uint32_t*) NULL, handle->hash_state);
    switch (err_val)
    {
        case RSIP_RET_PASS:
        {
            ret_val = CIP_DRV_RET_PASS;
            break;
        }
        case RSIP_RET_RESOURCE_CONFLICT:
        {
            ret_val = CIP_DRV_RET_RESOURCE_CONFLICT;
            break;
        }
        default:
        {
            ret_val = CIP_DRV_RET_FAIL;
            break;
        }
    }

    return ret_val;
}
/**********************************************************************************************************************
 End of function R_SCE_Sha256UpdatePrivate
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCE_Sha256FinalPrivate
 * Description  : .
 * Arguments    : handle
 *              : InData_Message
 *              : MAX_CNT
 *              : OutData_MsgDigest
 *              : OutData_Length
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t R_SCE_Sha256FinalPrivate(st_sce_sha_md5_handle_t * handle, uint32_t * InData_Message, uint32_t MAX_CNT,
        uint32_t * OutData_MsgDigest, uint32_t * OutData_Length)
{
    cip_drv_ret_t ret_val = CIP_DRV_RET_FAIL;
    rsip_ret_t err_val;
    uint32_t indata_hash_type[1];
    uint32_t indata_cmd[1];
    uint32_t indata_msg_len[2];

    indata_hash_type[0] = bswap32(0x00000002); /* SHA-256 */

    if (handle->all_received_length == handle->buffering_length)
    {
        indata_cmd[0] = bswap32(0x00000000); /* "Init" to "Final" */
    }
    else
    {
        indata_cmd[0] = bswap32(0x00000003); /* "Resume" to "Final" */
    }

    indata_msg_len[0] = bswap32((uint32_t)(((uint64_t )handle->all_received_length * 8) >> 32));
    indata_msg_len[1] = bswap32((uint32_t)((uint64_t )handle->all_received_length * 8));

    err_val = r_rsip_p72(indata_hash_type, indata_cmd, InData_Message, indata_msg_len,
            handle->hash_state, MAX_CNT, (uint32_t*) OutData_MsgDigest, (uint32_t*) NULL);
    switch (err_val)
    {
        case RSIP_RET_PASS:
        {
            ret_val = CIP_DRV_RET_PASS;
            break;
        }
        case RSIP_RET_RESOURCE_CONFLICT:
        {
            ret_val = CIP_DRV_RET_RESOURCE_CONFLICT;
            break;
        }
        default:
        {
            ret_val = CIP_DRV_RET_FAIL;
            break;
        }
    }
    *OutData_Length = HW_SCE_SHA256_HASH_LENGTH_BYTE_SIZE;

    return ret_val;
}
/**********************************************************************************************************************
 End of function R_SCE_Sha256FinalPrivate
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCE_RandomNumberGeneratePrivate
 * Description  : .
 * Argument     : OutData_Text
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t R_SCE_RandomNumberGeneratePrivate(uint32_t * OutData_Text)
{
    cip_drv_ret_t ret_val = CIP_DRV_RET_FAIL;
    rsip_ret_t err_val;

    err_val = r_rsip_p20(OutData_Text);
    switch (err_val)
    {
        case RSIP_RET_PASS:
        {
            ret_val = CIP_DRV_RET_PASS;
            break;
        }
        case RSIP_RET_RESOURCE_CONFLICT:
        {
            ret_val = CIP_DRV_RET_RESOURCE_CONFLICT;
            break;
        }
        default:
        {
            ret_val = CIP_DRV_RET_FAIL;
            break;
        }
    }

    return ret_val;
}
/**********************************************************************************************************************
 End of function R_SCE_RandomNumberGeneratePrivate
 *********************************************************************************************************************/