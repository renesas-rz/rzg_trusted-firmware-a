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
 * File Name    : rsip_drv.c
 * Version      : 1.0
 * Description  : RSIP driver functions.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 27.04.2023 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <r_cip_drv_api.h>
#include "rsip_drv.h"
#include "rsip_drv_private.h"
#include "rz_soc_def.h"
#include "rsip_p/r_rsip_addr.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/* Block length (in bytes) of SHA */
#define SHA_BLOCK8_LEN              (64U)

/* Initialization function call state */
#define CALL_ONLY_INIT              (0)
#define CALL_ONLY_UPDATE_FINAL      (1)

#define PUB_KEY_POS_NUM             (4)
#define IMG_HASH_POS_NUM            (2)

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
STATIC uint32_t s_private_id_counter;
STATIC uint32_t s_aes128cbcdec_private_id;
STATIC uint32_t s_sha256_private_id;

/**********************************************************************************************************************
 * Function Name: R_SCE_Open
 * Description  : .
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t R_SCE_Open(void)
{
    s_private_id_counter = 0;
    s_aes128cbcdec_private_id = 0;
    s_sha256_private_id = 0;

    gp_sce = (uint32_t *) RZT2H_RSIP_BASE;

    return CIP_DRV_RET_PASS;
}
/**********************************************************************************************************************
 End of function R_SCE_Open
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCE_Close
 * Description  : .
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t R_SCE_Close(void)
{
    return CIP_DRV_RET_PASS;
}
/**********************************************************************************************************************
 End of function R_SCE_Close
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCE_VerifyManifestImageSign
 * Description  : .
 * Arguments    : image
 *              : image_len
 *              : rotpk_hash
 *              : kcert
 *              : kcert_len
 *              : kcert_sign
 *              : kcert_pk_offset
 *              : imgpk_hash_offset
 *              : imgpk_hash_len
 *              : ccert
 *              : ccert_len
 *              : ccert_sign
 *              : ccert_pk_offset
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t R_SCE_VerifyManifestImageSign(uint32_t * image, uint32_t image_len, uint32_t * rotpk_hash,
        uint32_t * kcert,
        uint32_t kcert_len, uint32_t * kcert_sign, uint32_t kcert_pk_offset, uint32_t imgpk_hash_offset,
        uint32_t imgpk_hash_len, uint32_t * ccert, uint32_t ccert_len, uint32_t * ccert_sign, uint32_t ccert_pk_offset)
{
    uint32_t kcert_length[1];
    uint32_t ccert_length[1];
    uint32_t kcert_pk_ofs[PUB_KEY_POS_NUM];
    uint32_t ccert_pk_ofs[PUB_KEY_POS_NUM];
    uint32_t imgpk_hash_ofs[IMG_HASH_POS_NUM];

    if (0 == imgpk_hash_len)
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }

    kcert_length[0] = bswap32(kcert_len);
    ccert_length[0] = bswap32(ccert_len);

    kcert_pk_ofs[0] = bswap32(kcert_pk_offset);
    kcert_pk_ofs[1] = bswap32(kcert_pk_offset + HW_SCE_ECC_P256_KEY_PARAM_SIZE - 1);
    kcert_pk_ofs[2] = bswap32(kcert_pk_offset + HW_SCE_ECC_P256_KEY_PARAM_SIZE);
    kcert_pk_ofs[3] = bswap32(kcert_pk_offset + (HW_SCE_ECC_P256_KEY_PARAM_SIZE * 2) - 1);

    ccert_pk_ofs[0] = bswap32(ccert_pk_offset);
    ccert_pk_ofs[1] = bswap32(ccert_pk_offset + HW_SCE_ECC_P256_KEY_PARAM_SIZE - 1);
    ccert_pk_ofs[2] = bswap32(ccert_pk_offset + HW_SCE_ECC_P256_KEY_PARAM_SIZE);
    ccert_pk_ofs[3] = bswap32(ccert_pk_offset + (HW_SCE_ECC_P256_KEY_PARAM_SIZE * 2) - 1);

    imgpk_hash_ofs[0] = bswap32(imgpk_hash_offset);
    imgpk_hash_ofs[1] = bswap32(imgpk_hash_offset + imgpk_hash_len - 1);

    return R_SCE_VerifyManifestPrivate(kcert, kcert_length, kcert_sign, kcert_pk_ofs, imgpk_hash_ofs, rotpk_hash,
            ccert, ccert_length, ccert_sign, ccert_pk_ofs, NULL, image, image_len >> 2);
}
/**********************************************************************************************************************
 End of function R_SCE_VerifyManifestImageSign
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCE_VerifyManifestImageHash
 * Description  : .
 * Arguments    : image
 *              : image_len
 *              : rotpk_hash
 *              : kcert
 *              : kcert_len
 *              : kcert_sign
 *              : kcert_pk_offset
 *              : imgpk_hash_offset
 *              : imgpk_hash_len
 *              : ccert
 *              : ccert_len
 *              : ccert_sign
 *              : ccert_pk_offset
 *              : image_hash_offset
 *              : image_hash_len
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t R_SCE_VerifyManifestImageHash(uint32_t * image, uint32_t image_len, uint32_t * rotpk_hash,
        uint32_t * kcert,
        uint32_t kcert_len, uint32_t * kcert_sign, uint32_t kcert_pk_offset, uint32_t imgpk_hash_offset,
        uint32_t imgpk_hash_len, uint32_t * ccert, uint32_t ccert_len, uint32_t * ccert_sign, uint32_t ccert_pk_offset,
        uint32_t image_hash_offset, uint32_t image_hash_len)
{
    uint32_t kcert_length[1];
    uint32_t ccert_length[1];
    uint32_t kcert_pk_ofs[PUB_KEY_POS_NUM];
    uint32_t ccert_pk_ofs[PUB_KEY_POS_NUM];
    uint32_t imgpk_hash_ofs[IMG_HASH_POS_NUM];
    uint32_t image_hash_ofs[IMG_HASH_POS_NUM];

    if ((0 == imgpk_hash_len) || (0 == image_hash_offset))
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }

    kcert_length[0] = bswap32(kcert_len);
    ccert_length[0] = bswap32(ccert_len);

    kcert_pk_ofs[0] = bswap32(kcert_pk_offset);
    kcert_pk_ofs[1] = bswap32(kcert_pk_offset + HW_SCE_ECC_P256_KEY_PARAM_SIZE - 1);
    kcert_pk_ofs[2] = bswap32(kcert_pk_offset + HW_SCE_ECC_P256_KEY_PARAM_SIZE);
    kcert_pk_ofs[3] = bswap32(kcert_pk_offset + (HW_SCE_ECC_P256_KEY_PARAM_SIZE * 2) - 1);

    ccert_pk_ofs[0] = bswap32(ccert_pk_offset);
    ccert_pk_ofs[1] = bswap32(ccert_pk_offset + HW_SCE_ECC_P256_KEY_PARAM_SIZE - 1);
    ccert_pk_ofs[2] = bswap32(ccert_pk_offset + HW_SCE_ECC_P256_KEY_PARAM_SIZE);
    ccert_pk_ofs[3] = bswap32(ccert_pk_offset + (HW_SCE_ECC_P256_KEY_PARAM_SIZE * 2) - 1);

    imgpk_hash_ofs[0] = bswap32(imgpk_hash_offset);
    imgpk_hash_ofs[1] = bswap32(imgpk_hash_offset + imgpk_hash_len - 1);

    image_hash_ofs[0] = bswap32(image_hash_offset);
    image_hash_ofs[1] = bswap32(image_hash_offset + image_hash_len - 1);

    return R_SCE_VerifyManifestPrivate(kcert, kcert_length, kcert_sign, kcert_pk_ofs, imgpk_hash_ofs, rotpk_hash,
            ccert, ccert_length, ccert_sign, ccert_pk_ofs, image_hash_ofs, image,
            image_len >> 2);
}
/**********************************************************************************************************************
 End of function R_SCE_VerifyManifestImageHash
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCE_AES128CBC_DecryptInit
 * Description  : .
 * Arguments    : handle
 *              : wrapped_key
 *              : initial_vector
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t R_SCE_AES128CBC_DecryptInit(st_sce_aes_handle_t * handle, st_sce_aes_wrapped_key_t * wrapped_key,
        uint32_t * initial_vector)
{
    memset(handle, 0, sizeof(st_sce_aes_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    s_private_id_counter++;
    s_aes128cbcdec_private_id = s_private_id_counter;
    handle->id = s_aes128cbcdec_private_id;
    memcpy(handle->current_initial_vector, initial_vector, sizeof(handle->current_initial_vector));

    return R_SCE_Aes128CbcDecryptInitPrivate(wrapped_key, handle->current_initial_vector);
}
/**********************************************************************************************************************
 End of function R_SCE_AES128CBC_DecryptInit
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCE_AES128CBC_DecryptUpdate
 * Description  : .
 * Arguments    : handle
 *              : cipher
 *              : plain
 *              : cipher_length
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t R_SCE_AES128CBC_DecryptUpdate(st_sce_aes_handle_t * handle, uint32_t * cipher, uint32_t * plain,
        uint32_t cipher_length)
{
    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }

    if (handle->id != s_aes128cbcdec_private_id)
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }

    handle->current_input_data_size = cipher_length;
    memcpy(handle->last_1_block_as_fraction,
            (((void*) cipher + ((cipher_length / HW_SCE_AES_BLOCK_BYTE_SIZE) * HW_SCE_AES_BLOCK_BYTE_SIZE))),
            (cipher_length % HW_SCE_AES_BLOCK_BYTE_SIZE));

    R_SCE_Aes128CbcDecryptUpdatePrivate(cipher, plain, cipher_length >> 2);
    memcpy(handle->current_initial_vector, (((void*) plain + (cipher_length - HW_SCE_AES_BLOCK_BYTE_SIZE))),
    HW_SCE_AES_CBC_IV_BYTE_SIZE);

    return CIP_DRV_RET_PASS;
}
/**********************************************************************************************************************
 End of function R_SCE_AES128CBC_DecryptUpdate
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCE_AES128CBC_DecryptFinal
 * Description  : .
 * Arguments    : handle
 *              : plain
 *              : plain_length
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t R_SCE_AES128CBC_DecryptFinal(st_sce_aes_handle_t * handle, uint32_t * plain, uint32_t * plain_length)
{
    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }

    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != s_aes128cbcdec_private_id)
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }

    memset(handle, 0, sizeof(st_sce_aes_handle_t));

    return R_SCE_Aes128CbcDecryptFinalPrivate(plain, plain_length);
}
/**********************************************************************************************************************
 End of function R_SCE_AES128CBC_DecryptFinal
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCE_SHA256_Init
 * Description  : .
 * Arguments    : handle
 *              : message_length
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t R_SCE_SHA256_Init(st_sce_sha_md5_handle_t * handle, uint32_t message_length)
{
    memset(handle, 0, sizeof(st_sce_sha_md5_handle_t));
    handle->flag_call_init = CALL_ONLY_UPDATE_FINAL;
    s_private_id_counter++;
    s_sha256_private_id = s_private_id_counter;
    handle->id = s_sha256_private_id;
    handle->all_received_length = message_length;
    memset(handle->sha_buffer, 0, sizeof(handle->sha_buffer));

    return R_SCE_Sha256InitPrivate(handle, message_length);
}
/**********************************************************************************************************************
 End of function R_SCE_SHA256_Init
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCE_SHA256_Update
 * Description  : .
 * Arguments    : handle
 *              : message
 *              : message_length
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t R_SCE_SHA256_Update(st_sce_sha_md5_handle_t * handle, uint32_t * message, uint32_t message_length)
{
    cip_drv_ret_t ret_val = CIP_DRV_RET_PASS;

    if (NULL == handle)
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }
    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }
    if (handle->id != s_sha256_private_id)
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }
    if (handle->all_received_length != message_length)
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }
    if (NULL == message)
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }

    if (message_length > SHA_BLOCK8_LEN)
    {
        uint32_t input_length = ((message_length - SHA_BLOCK8_LEN) / SHA_BLOCK8_LEN) * SHA_BLOCK8_LEN;
        ret_val = R_SCE_Sha256UpdatePrivate(handle, message, input_length >> 2);
        if (CIP_DRV_RET_PASS == ret_val)
        {
            handle->buffering_length = message_length - input_length;
            memcpy(handle->sha_buffer, (((void*) message) + input_length), handle->buffering_length);
        }
    }
    else
    {
        handle->buffering_length = message_length;
        memcpy(handle->sha_buffer, message, handle->buffering_length);
    }

    return ret_val;
}
/**********************************************************************************************************************
 End of function R_SCE_SHA256_Update
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCE_SHA256_Final
 * Description  : .
 * Arguments    : handle
 *              : digest
 *              : digest_length
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t R_SCE_SHA256_Final(st_sce_sha_md5_handle_t * handle, uint32_t * digest, uint32_t * digest_length)
{
    cip_drv_ret_t ret_val = CIP_DRV_RET_PASS;

    if (NULL == handle)
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }
    if (CALL_ONLY_INIT == handle->flag_call_init)
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }

    handle->flag_call_init = CALL_ONLY_INIT;
    if (handle->id != s_sha256_private_id)
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }
    if ((NULL == digest) || (NULL == digest_length))
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }

    ret_val = R_SCE_Sha256FinalPrivate(handle,
            handle->sha_buffer,
            handle->buffering_length >> 2,
            digest,
            digest_length);
    handle->all_received_length = 0;
    handle->buffering_length = 0;
    memset(handle->sha_buffer, 0, sizeof(handle->sha_buffer));

    return ret_val;
}
/**********************************************************************************************************************
 End of function R_SCE_SHA256_Final
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_SCE_RandomNumberGenerate
 * Description  : .
 * Argument     : random
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t R_SCE_RandomNumberGenerate(uint32_t * random)
{
    if (NULL == random)
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }

    return R_SCE_RandomNumberGeneratePrivate(random);
}
/**********************************************************************************************************************
 End of function R_SCE_RandomNumberGenerate
 *********************************************************************************************************************/
