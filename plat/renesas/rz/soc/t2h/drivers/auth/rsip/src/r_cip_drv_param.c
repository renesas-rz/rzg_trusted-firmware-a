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
 * File Name    : rsip_check_param.c
 * Version      : 1.0
 * Description  : This file define functions to check arguments.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 21.04.2021 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <r_cip_drv_api.h>
#include "r_cip_drv_param.h"
#include "rsip_drv_private.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Validate address for RSIP
 * Function Name: rsip_validate_addr
 * Description  : Validate address
 * Arguments    : [IN] addr -
 *                    address
 *                     alignment -
 *                    alignment
 * Return Value : CIP_DRV_RET_PASS -
 *                    valid address
 *                CIP_DRV_RET_PARAM_ERROR -
 *                    invalid address
 *********************************************************************************************************************/
STATIC cip_drv_ret_t rsip_validate_addr(const uint32_t *const addr, const uint32_t alignment)
{
    cip_drv_ret_t ret_val = CIP_DRV_RET_PASS;

    if (false == is_aligned((uintptr_t) addr, alignment))
    {
        ret_val = CIP_DRV_RET_PARAM_ERROR;
    }
    return ret_val;
} /* End of function rsip_validate_addr() */

/**********************************************************************************************************************
 Validate size for RSIP
 * Function Name: rsip_validate_size
 * Description  : Validate size
 * Arguments    : [IN] size -
 *                    size value
 * Return Value : CIP_DRV_RET_PASS -
 *                    valid size
 *                CIP_DRV_RET_PARAM_ERROR -
 *                    invalid size
 *********************************************************************************************************************/
STATIC cip_drv_ret_t rsip_validate_size(const uint32_t size, const uint32_t alignment)
{
    cip_drv_ret_t ret_val = CIP_DRV_RET_PASS;

    if ((0 == size) || (false == is_aligned(size, alignment)))
    {
        ret_val = CIP_DRV_RET_PARAM_ERROR;
    }
    return ret_val;
} /* End of function rsip_validate_size() */

/**********************************************************************************************************************
 Validate key-ID for RSIP
 * Function Name: rsip_validate_keyselect
 * Description  : Validate key_select
 * Arguments    : [IN] key_id -
 *                    key id
 * Return Value : CIP_DRV_RET_PASS -
 *                    valid key_select
 *                CIP_DRV_RET_PARAM_ERROR -
 *                    invalid key_select
 *********************************************************************************************************************/
STATIC cip_drv_ret_t rsip_validate_keyselect(const uint32_t key_select)
{
    /* Supports external registered key only */
    if (CIP_DRV_CIPHER_INFO_KEY_SEL_INSTALLED != (key_select & CIP_DRV_KEY_SELECT_MASK))
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }
    /* Support key id 0 to 4 */
    if (CIP_DRV_MAX_KEY_INDEX < (key_select & CIP_DRV_KEY_INDEX_MASK))
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }
    return CIP_DRV_RET_PASS;
} /* End of function rsip_validate_keyselect() */

/**********************************************************************************************************************
 Validate iV for RSIP
 * Function Name: rsip_validate_iv
 * Description  : Validate iv
 * Arguments    : [IN]  iv_select -
 *                    iv_select
 *                      iv_len -
 *                    iv length
 *
 * Return Value : CIP_DRV_RET_PASS -
 *                    valid iv length and iv_select
 *                CIP_DRV_RET_PARAM_ERROR -
 *                    invalid iv
 *********************************************************************************************************************/
STATIC cip_drv_ret_t rsip_validate_iv(const uint32_t iv_select, const uint32_t *const iv, const uint32_t iv_len)
{
    /* Supports specified iv length and iv_select only */
    if (CIP_DRV_CIPHER_INFO_IV_SEL_STORED_CODE_CERT != iv_select)
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }
    if ((NULL == iv) || (HW_SCE_AES_CBC_IV_BYTE_SIZE != iv_len))
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }
    return rsip_validate_addr(iv, 4);
} /* End of function rsip_validate_iv() */

/**********************************************************************************************************************
 Validate cipher mode for RSIP
 * Function Name: rsip_validate_cipher
 * Description  : Validate cipher mode
 * Arguments    : [IN]  cipher_algo -
 *                    cipher algorithm
 *                      cipher_mode -
 *                    cipher mode
 * Return Value : CIP_DRV_RET_PASS -
 *                    supported algorithm and mode
 *                CIP_DRV_RET_UNSUPPORTED_ALGORITHM -
 *                    not supported algorithm
 *********************************************************************************************************************/
STATIC cip_drv_ret_t rsip_validate_cipher(const uint32_t cipher_algo, const uint32_t cipher_mode)
{
    cip_drv_ret_t ret_val = CIP_DRV_RET_PASS;

    /* Supports AES128-CBC mode only */
    if ((CIP_DRV_CIPHER_ALGO_AES128 != cipher_algo) || (CIP_DRV_CIPHER_MODE_CBC != cipher_mode))
    {
        ret_val = CIP_DRV_RET_UNSUPPORTED_ALGORITHM;
    }
    return ret_val;
} /* End of function rsip_validate_cipher() */

/**********************************************************************************************************************
 Validate verify mode for RSIP
 * Function Name: rsip_validate_verify_keycert
 * Description  : Validate algorithm of verification for key ceritification
 * Arguments    : [IN]  img_pk_hash_algo -
 *                    Image PK hash algorithm
 *                      sign_algo -
 *                    Sign algorithm
 *                      sign_hash_algo -
 *                    Sign hash algorithm
 *                      sign_scheme -
 *                    Sign scheme
 * Return Value : CIP_DRV_RET_PASS -
 *                    supported algorithm
 *                CIP_DRV_RET_UNSUPPORTED_ALGORITHM -
 *                    not supported algorithm
 *********************************************************************************************************************/
STATIC cip_drv_ret_t rsip_validate_verify_keycert(const uint32_t img_pk_hash_algo, const uint32_t sign_algo,
        const uint32_t sign_hash_algo, const uint32_t sign_scheme)
{
    cip_drv_ret_t ret_val = CIP_DRV_RET_PASS;

    /* Supports specified algorithm only */
    if ((CIP_DRV_HASH_ALGO_SHA2_256 != img_pk_hash_algo) || (CIP_DRV_SIGN_ALGO_ECDSA_P256 != sign_algo)
            || (CIP_DRV_HASH_ALGO_SHA2_256 != sign_hash_algo) || (CIP_DRV_SCHEME_NONE != sign_scheme))
    {
        ret_val = CIP_DRV_RET_UNSUPPORTED_ALGORITHM;
    }
    return ret_val;
} /* End of function rsip_validate_verify_keycert() */

/**********************************************************************************************************************
 Validate verify mode for RSIP
 * Function Name: rsip_validate_verify_codecert
 * Description  : Validate algorithm of verification for code ceritification
 * Arguments    : [IN]  img_hash_algo -
 *                    Image hash algorithm
 *                      sign_algo -
 *                    Sign algorithm
 *                      sign_hash_algo -
 *                    Sign hash algorithm
 *                      sign_scheme -
 *                    Sigh scheme
 * Return Value : CIP_DRV_RET_PASS -
 *                    supported algorithm
 *                CIP_DRV_RET_UNSUPPORTED_ALGORITHM
 *                    not supported algorithm
 *********************************************************************************************************************/
STATIC cip_drv_ret_t rsip_validate_verify_codecert(const uint32_t img_hash_algo, const uint32_t sign_algo,
        const uint32_t sign_hash_algo, const uint32_t sign_scheme)
{
    cip_drv_ret_t ret_val = CIP_DRV_RET_PASS;

    /* Supports specified algorithm only */
    if (((CIP_DRV_HASH_ALGO_SHA2_256 != img_hash_algo) && (CIP_DRV_HASH_ALGO_NONE != img_hash_algo))
            || (CIP_DRV_SIGN_ALGO_ECDSA_P256 != sign_algo) || (CIP_DRV_HASH_ALGO_SHA2_256 != sign_hash_algo)
            || (CIP_DRV_SCHEME_NONE != sign_scheme))
    {
        ret_val = CIP_DRV_RET_UNSUPPORTED_ALGORITHM;
    }
    return ret_val;
} /* End of function rsip_validate_verify_codecert() */

/**********************************************************************************************************************
 Check decryption parameter for RSIP
 * Function Name: cip_drv_check_dec_param
 * Description  : Check parameter for decryption
 * Arguments    : [IN] dec_param -
 *                    decryption parameter
 * Return Value : CIP_DRV_RET_PASS -
 *                    valid parameter
 *                CIP_DRV_RET_PARAM_ERROR -
 *                    invalid parameter
 *                CIP_DRV_RET_UNSUPPORTED_ALGORITHM -
 *                    not supported algorithm
 *********************************************************************************************************************/
cip_drv_ret_t cip_drv_check_dec_param(const st_cip_drv_cipher_img_param_t *const dec_param)
{
    cip_drv_ret_t ret_val = CIP_DRV_RET_PASS;

    if (NULL == dec_param)
    {
        ret_val = CIP_DRV_RET_PARAM_ERROR;
    }
    if (CIP_DRV_RET_PASS == ret_val)
    {
        ret_val = rsip_validate_keyselect(dec_param->key_select);
    }
    if (CIP_DRV_RET_PASS == ret_val)
    {
        ret_val = rsip_validate_addr(dec_param->p_img_src, 4);
    }
    if (CIP_DRV_RET_PASS == ret_val)
    {
        ret_val = rsip_validate_addr(dec_param->p_img_dst, 4);
    }
    if (CIP_DRV_RET_PASS == ret_val)
    {
        ret_val = rsip_validate_size(dec_param->img_len, 4);
    }
    if (CIP_DRV_RET_PASS == ret_val)
    {
        ret_val = rsip_validate_cipher(dec_param->cipher_algo, dec_param->cipher_mode);
    }
    if (CIP_DRV_RET_PASS == ret_val)
    {
        ret_val = rsip_validate_iv(dec_param->iv_select, dec_param->p_iv, dec_param->iv_len);
    }

    return ret_val;
} /* End of function cip_drv_check_dec_param() */

/**********************************************************************************************************************
 Check verification parameter for RSIP
 * Function Name: cip_drv_check_verify_param
 * Description  : Check parameter for verification
 * Arguments    : [IN] dec_param -
 *                    verification parameter
 * Return Value : CIP_DRV_RET_PASS -
 *                    valid parameter
 *                CIP_DRV_RET_PARAM_ERROR -
 *                    invalid parameter
 *                CIP_DRV_RET_UNSUPPORTED_ALGORITHM -
 *                    not supported algorithm
 *********************************************************************************************************************/
cip_drv_ret_t cip_drv_check_verify_param(const st_cip_drv_cc_key_cert_param_t *const p_cc_key_cert_param,
        const st_cip_drv_cc_code_cert_param_t *const p_cc_code_cert_param,
        const st_cip_drv_cipher_img_param_t *const p_dec_img_param)
{
    cip_drv_ret_t ret_val = CIP_DRV_RET_PASS;

    if ((NULL == p_cc_key_cert_param) || (NULL == p_cc_code_cert_param))
    {
        ret_val = CIP_DRV_RET_PARAM_ERROR;
    }

    /* key cert check */

    if (CIP_DRV_RET_PASS == ret_val)
    {
        /* Support CIP_DRV_KEY_CERT_PK_CMP_SRC_ROT only */
        if (CIP_DRV_KEY_CERT_PK_CMP_SRC_ROT != p_cc_key_cert_param->key_cert_pk_cmp_src)
        {
            ret_val = CIP_DRV_RET_PARAM_ERROR;
        }
    }

    /* Check algo */
    if (CIP_DRV_RET_PASS == ret_val)
    {
        ret_val = rsip_validate_verify_keycert(p_cc_key_cert_param->img_pk_hash_algo, p_cc_key_cert_param->sign_algo,
                p_cc_key_cert_param->sign_hash_algo, p_cc_key_cert_param->sign_scheme);
    }
    /* Check alignment */
    if (CIP_DRV_RET_PASS == ret_val)
    {
        ret_val = rsip_validate_addr(p_cc_key_cert_param->p_img_pk_hash, 4);
    }
    if (CIP_DRV_RET_PASS == ret_val)
    {
        ret_val = rsip_validate_addr(p_cc_key_cert_param->p_sign_pk, 4);
    }
    if (CIP_DRV_RET_PASS == ret_val)
    {
        ret_val = rsip_validate_addr(p_cc_key_cert_param->p_sign, 4);
    }
    if (CIP_DRV_RET_PASS == ret_val)
    {
        ret_val = rsip_validate_addr(p_cc_key_cert_param->p_key_cert, 4);
    }
    /* Check size */
    if (CIP_DRV_RET_PASS == ret_val)
    {
        ret_val = rsip_validate_size(p_cc_key_cert_param->img_pk_hash_len, 4);
    }
    if (CIP_DRV_RET_PASS == ret_val)
    {
        if (HW_SCE_ECC_P256_KEY_SIZE != p_cc_key_cert_param->sign_pk_len)
        {
            ret_val = CIP_DRV_RET_PARAM_ERROR;
        }
        if (HW_SCE_ECDSA_DATA_BYTE_SIZE != p_cc_key_cert_param->sign_len)
        {
            ret_val = CIP_DRV_RET_PARAM_ERROR;
        }
    }

    /* code cert check */

    /* Check Algo */
    if (CIP_DRV_RET_PASS == ret_val)
    {
        ret_val = rsip_validate_verify_codecert(p_cc_code_cert_param->img_hash_algo, p_cc_code_cert_param->sign_algo,
                p_cc_code_cert_param->sign_hash_algo,
                p_cc_code_cert_param->sign_scheme);
    }

    if (CIP_DRV_RET_PASS == ret_val)
    {
        /* Support no save img pk only */
        if (0 != p_cc_code_cert_param->is_save_img_pk)
        {
            ret_val = CIP_DRV_RET_PARAM_ERROR;
        }

        /* check size */
        if (HW_SCE_ECC_P256_KEY_SIZE != p_cc_code_cert_param->sign_pk_len)
        {
            ret_val = CIP_DRV_RET_PARAM_ERROR;
        }
        if (HW_SCE_ECDSA_DATA_BYTE_SIZE != p_cc_code_cert_param->sign_len)
        {
            ret_val = CIP_DRV_RET_PARAM_ERROR;
        }
    }
    return ret_val;
} /* End of function cip_drv_check_verify_param() */

/**********************************************************************************************************************
 * Function Name: cip_drv_calc_hash_and_pk_pos
 * Description  : .
 * Arguments    : p_cc_key_cert_param
 *              : p_cc_code_cert_param
 *              : kcert_pk_offset
 *              : imgpk_hash_offset
 *              : ccert_pk_offset
 *              : image_hash_offset
 * Return Value : .
 *********************************************************************************************************************/
cip_drv_ret_t cip_drv_calc_hash_and_pk_pos(const st_cip_drv_cc_key_cert_param_t *const p_cc_key_cert_param,
        const st_cip_drv_cc_code_cert_param_t *const p_cc_code_cert_param, uint32_t * kcert_pk_offset,
        uint32_t * imgpk_hash_offset, uint32_t * ccert_pk_offset, uint32_t * image_hash_offset)
{
    if ((uint64_t) p_cc_key_cert_param->p_sign_pk <= (uint64_t) p_cc_key_cert_param->p_key_cert)
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }
    *kcert_pk_offset = (uint32_t)(
            (uint64_t) p_cc_key_cert_param->p_sign_pk - (uint64_t) p_cc_key_cert_param->p_key_cert);

    if ((uint64_t) p_cc_key_cert_param->p_img_pk_hash <= (uint64_t) p_cc_key_cert_param->p_key_cert)
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }
    *imgpk_hash_offset = (uint32_t)(
            (uint64_t) p_cc_key_cert_param->p_img_pk_hash - (uint64_t) p_cc_key_cert_param->p_key_cert);

    if ((uint64_t) p_cc_code_cert_param->p_sign_pk <= (uint64_t) p_cc_code_cert_param->p_code_cert)
    {
        return CIP_DRV_RET_PARAM_ERROR;
    }
    *ccert_pk_offset = (uint32_t)(
            (uint64_t) p_cc_code_cert_param->p_sign_pk - (uint64_t) p_cc_code_cert_param->p_code_cert);

    if (NULL != p_cc_code_cert_param->p_img_hash)
    {
        if ((uint64_t)(p_cc_code_cert_param->p_img_hash) <= (uint64_t)(p_cc_code_cert_param->p_code_cert))
        {
            return CIP_DRV_RET_PARAM_ERROR;
        }
        *image_hash_offset = (uint32_t)(
                (uint64_t) p_cc_code_cert_param->p_img_hash - (uint64_t) p_cc_code_cert_param->p_code_cert);
    }

    return CIP_DRV_RET_PASS;
}
/**********************************************************************************************************************
 End of function cip_drv_calc_hash_and_pk_pos
 *********************************************************************************************************************/

/* End of File */
