/*
 * Copyright (C) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : r_cip_drv_param.h
 * Version      : 1.0
 * Description  : This file declare functions to check arguments.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_cip_drv_api.h"
#include "rsip_drv.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef R_CIP_DRV_PARAM_H
#define R_CIP_DRV_PARAM_H

#define CIP_DRV_IV_SELECT               (CIP_DRV_CIPHER_INFO_IV_SEL_STORED_CODE_CERT)
#define CIP_DRV_IV_LENGTH               (HW_SCE_AES_CBC_IV_BYTE_SIZE)

#define CIP_DRV_KEY_SELECT_MASK         (0xFFFFFF00uL)
#define CIP_DRV_KEY_INDEX_MASK          (0x000000FFuL)

#define CIP_DRV_MIN_KEY_INDEX           (0)  /* min key_select num */
#define CIP_DRV_MAX_KEY_INDEX           (4)  /* max key_select num */

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
extern cip_drv_ret_t cip_drv_check_dec_param(const st_cip_drv_cipher_img_param_t *const dec_param);
extern cip_drv_ret_t cip_drv_check_verify_param(const st_cip_drv_cc_key_cert_param_t *const p_cc_key_cert_param,
												const st_cip_drv_cc_code_cert_param_t *const p_cc_code_cert_param,
												const st_cip_drv_cipher_img_param_t *const p_dec_img_param);
extern cip_drv_ret_t cip_drv_calc_hash_and_pk_pos(const st_cip_drv_cc_key_cert_param_t *const p_cc_key_cert_param,
												  const st_cip_drv_cc_code_cert_param_t *const p_cc_code_cert_param, uint32_t *kcert_pk_offset,
												  uint32_t *imgpk_hash_offset, uint32_t *ccert_pk_offset, uint32_t *image_hash_offset);

#endif /* R_CIP_DRV_PARAM_H */
