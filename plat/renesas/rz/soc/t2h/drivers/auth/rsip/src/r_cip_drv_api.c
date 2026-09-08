/*
 * Copyright (C) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : r_cip_drv_api.c
 * Version      : 1.0
 * Description  : This file define API functions for Boot ROM.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <r_cip_drv_api.h>
#include <otp.h>
#include "r_cip_drv_param.h"
#include "rsip_drv.h"
#include "rsip_drv_private.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/* Anti Rollback setting */
#define ANTI_RB_VER_BIT_LENGTH      (sizeof(uint32_t) * 8)
#define ANTI_RB_ENABLE_BIT          (0x00070000uL)      /* anti roll back enable */

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 *********************************************************************************************************************/
extern int32_t notify_dec_image_info_callback(uintptr_t dec_base, size_t dec_size);

/**********************************************************************************************************************
 Private global variables and functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_CIP_DRV_PrcDecryptImage
 * Description  : Secure boot
 * Arguments    : [IN] dec_param -
 *                    Decryption Parameter for Secure Boot.
 * Return Value : CIP_DRV_RET_PASS -
 *                    normal end
 *                CIP_DRV_RET_FAIL -
 *                    failed to decrypt.
 *                CIP_DRV_RET_UNSUPPORTER_ALGORITHM -
 *                    unsupported algorithm.
 *                CIP_DRV_RET_RESOURCE_CONFLICT -
 *                    resource conflict.
 *********************************************************************************************************************/
cip_drv_ret_t R_CIP_DRV_PrcDecryptImage(const st_cip_drv_cipher_img_param_t *const p_dec_img_param)
{
	cip_drv_ret_t ret_val = CIP_DRV_RET_PASS;
	st_sce_aes_handle_t aes_handle;
	st_sce_aes_wrapped_key_t wrapped_key;
	uint32_t key_id;

	ret_val = cip_drv_check_dec_param(p_dec_img_param);
	if (CIP_DRV_RET_PASS != ret_val) {
		return ret_val;
	}

	/* key_select data included in dec parameter is stored as little endian data */
	key_id = ((p_dec_img_param->key_select) & CIP_DRV_KEY_INDEX_MASK);

	if ((0 <= key_id) && (key_id < 4)) {
		uint32_t result = otp_read_decrypt_key(key_id, (uint32_t *)wrapped_key.value,
											   HW_SCE_AES128_KEY_INDEX_WORD_SIZE);
		if (0 != result) {
			ret_val = CIP_DRV_RET_FAIL;
		}
	} else {
		/* if key_id = 4, key is included in image. get key from end of IMG */
		uintptr_t key_pos = (uintptr_t)p_dec_img_param->p_img_src + p_dec_img_param->img_len;
		memcpy(wrapped_key.value, (void *)key_pos, HW_SCE_AES128_KEY_INDEX_WORD_SIZE * sizeof(uint32_t));
	}

	if (CIP_DRV_RET_PASS == ret_val) {
		if (0 != notify_dec_image_info_callback((uintptr_t)p_dec_img_param->p_img_dst, p_dec_img_param->img_len)) {
			ret_val = CIP_DRV_RET_PARAM_ERROR;
		}
	}

	/* execute RSIP procedure */
	if (CIP_DRV_RET_PASS == ret_val) {
		wrapped_key.type = SCE_KEY_INDEX_TYPE_AES128;
		/* RSIP procedure 47i */
		ret_val = R_SCE_AES128CBC_DecryptInit(&aes_handle, &wrapped_key, (uint32_t *)p_dec_img_param->p_iv);

		if (CIP_DRV_RET_PASS == ret_val) {
			/* RSIP procedure 47u */
			(void)R_SCE_AES128CBC_DecryptUpdate(&aes_handle, (uint32_t *)p_dec_img_param->p_img_src,
												p_dec_img_param->p_img_dst, p_dec_img_param->img_len);
			/* RSIP procedure 47f */
			ret_val = R_SCE_AES128CBC_DecryptFinal(&aes_handle, NULL, NULL);
		}
	}

	return ret_val;
} /* End of function R_CIP_DRV_PrcDecryptImage() */

/**********************************************************************************************************************
 * Function Name: R_CIP_DRV_PrcVerifyCertChain
 * Description  : Verify Key cert and Code cert for Secure boot
 * Arguments    : [IN] p_cc_key_cert_param -
 *                    pointer to Key Certification.
 *                     p_cc_code_cert_param -
 *                    pointer to Code Certification.
 *                     p_dec_img_param -
 *                    Timing specification for decryption and verification. Not Use.
 * Return Value : CIP_DRV_RET_PASS -
 *                    normal end
 *                CIP_DRV_RET_FAIL -
 *                    failed (includes Parameter error)
 *                CIP_DRV_RET_UNSUPPORTED_ALGORITHM
 *                    not supported algorithm
 *                CIP_DRV_RET_RESOURCE_CONFLICT
 *                    resource conflict
 *                CIP_DRV_RET_AUTH_FAIL -
 *                    failed to authentication
 *********************************************************************************************************************/
cip_drv_ret_t R_CIP_DRV_PrcVerifyCertChain(const st_cip_drv_cc_key_cert_param_t *const p_cc_key_cert_param,
										   const st_cip_drv_cc_code_cert_param_t *const p_cc_code_cert_param,
										   const st_cip_drv_cipher_img_param_t *const p_dec_img_param)
{
	cip_drv_ret_t ret_val = CIP_DRV_RET_PASS;
	uint32_t rotpk_hash[HW_SCE_ROTPK_HASH_WORD_SIZE];
	uint32_t img_hash_pos = 0;
	uint32_t key_sign_pk_pos = 0;
	uint32_t img_pk_hash_pos = 0;
	uint32_t code_sign_pk_pos = 0;

	ret_val = cip_drv_check_verify_param(p_cc_key_cert_param, p_cc_code_cert_param, p_dec_img_param);
	if (CIP_DRV_RET_PASS != ret_val) {
		return ret_val;
	}

	ret_val = cip_drv_calc_hash_and_pk_pos(p_cc_key_cert_param, p_cc_code_cert_param, &key_sign_pk_pos,
										   &img_pk_hash_pos, &code_sign_pk_pos, &img_hash_pos);
	if (CIP_DRV_RET_PASS != ret_val) {
		return ret_val;
	}

	/* get hash value of ROT PK in OTP memory */
	if (0 != otp_read_rotpk_hash(rotpk_hash, HW_SCE_ROTPK_HASH_WORD_SIZE)) {
		return CIP_DRV_RET_FAIL;
	}

	if (NULL == p_cc_code_cert_param->p_img_hash) {
		ret_val = R_SCE_VerifyManifestImageSign((uint32_t *)p_cc_code_cert_param->p_img, p_cc_code_cert_param->img_len,
												rotpk_hash, (uint32_t *)p_cc_key_cert_param->p_key_cert,
												p_cc_key_cert_param->key_cert_sign_len,
												(uint32_t *)p_cc_key_cert_param->p_sign, key_sign_pk_pos,
												img_pk_hash_pos, p_cc_key_cert_param->img_pk_hash_len,
												(uint32_t *)p_cc_code_cert_param->p_code_cert,
												p_cc_code_cert_param->code_cert_sign_len,
												(uint32_t *)p_cc_code_cert_param->p_sign, code_sign_pk_pos);
	} else {
		ret_val = R_SCE_VerifyManifestImageHash((uint32_t *)p_cc_code_cert_param->p_img, p_cc_code_cert_param->img_len,
												rotpk_hash, (uint32_t *)p_cc_key_cert_param->p_key_cert,
												p_cc_key_cert_param->key_cert_sign_len,
												(uint32_t *)p_cc_key_cert_param->p_sign, key_sign_pk_pos,
												img_pk_hash_pos, p_cc_key_cert_param->img_pk_hash_len,
												(uint32_t *)p_cc_code_cert_param->p_code_cert,
												p_cc_code_cert_param->code_cert_sign_len,
												(uint32_t *)p_cc_code_cert_param->p_sign, code_sign_pk_pos,
												img_hash_pos, p_cc_code_cert_param->img_hash_len);
	}

	return ret_val;
}
/**********************************************************************************************************************
 End of function R_CIP_DRV_PrcVerifyCertChain
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_CIP_DRV_CheckImageVersion
 * Description  : Image version check
 * Arguments    : [IN] image_version -
 *
 *                     build_num -
 *
 * Return Value : CIP_DRV_RET_PASS -
 *                    input version is new version
 *                CIP_DRV_RET_SAME_IMAGE_VERSION -
 *                    input version is same as stored in hardware resource.
 *                CIP_DRV_RET_LOWER_IMAGE_VERSION -
 *                    input version is old version or failed to get version
 *********************************************************************************************************************/
cip_drv_ret_t R_CIP_DRV_CheckImageVersion(const uint32_t image_version, const uint32_t build_num)
{
	cip_drv_ret_t ret_val = CIP_DRV_RET_SAME_IMAGE_VERSION;
	uint32_t arb_setting = 0x00000000uL;
	uint32_t arb_version = 0x00000000uL;

	if (0 != otp_read_anti_rb_setting(&arb_setting)) {
		return CIP_DRV_RET_LOWER_IMAGE_VERSION;
	}

	if (ANTI_RB_ENABLE_BIT != (arb_setting & ANTI_RB_ENABLE_BIT)) {
		return CIP_DRV_RET_SAME_IMAGE_VERSION;
	}

	if (0 != otp_read_anti_rb_version(&arb_version)) {
		return CIP_DRV_RET_LOWER_IMAGE_VERSION;
	}

	int32_t exit_loop = 0;
	for (int32_t cnt = 0; ((0 == exit_loop) && (cnt < ANTI_RB_VER_BIT_LENGTH)); cnt++) {
		uint32_t cur_ver = ((0x80000000uL) >> cnt) & arb_version;
		uint32_t new_ver = ((0x80000000uL) >> cnt) & image_version;

		if ((0 != cur_ver) && (0 != new_ver)) {
			ret_val = CIP_DRV_RET_SAME_IMAGE_VERSION;
			exit_loop = 1;
		} else if ((0 != cur_ver) && (0 == new_ver)) {
			ret_val = CIP_DRV_RET_LOWER_IMAGE_VERSION;
			exit_loop = 1;
		} else if ((0 == cur_ver) && (0 != new_ver)) {
			ret_val = CIP_DRV_RET_PASS;
			exit_loop = 1;
			/* Version counter updates are not performed here. */
			/* Write processing is performed by MaskRom.       */
		} else {
			; /* Do Nothing, check next bit */
		}
	}

	return ret_val;
}
/**********************************************************************************************************************
 End of function R_CIP_DRV_CheckImageVersion
*********************************************************************************************************************/

/* End of File */
