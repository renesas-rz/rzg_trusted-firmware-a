/*
 * Copyright (C) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************************************************************
 * File Name    : rsip_drv.h
 * Version      : 1.0
 * Description  : RSIP driver header file.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_cip_drv_api.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef RSIP_DRV_H
#define RSIP_DRV_H

/* return value (RSIP driver included in BootROM) */
#define CIP_DRV_RET_CLK_SUPPLY_FAIL                 ((cip_drv_ret_t)0xAAAA0101UL) /* Failed to supply clk to RSIP */
#define CIP_DRV_RET_JTAG_AUTH_FAIL                  ((cip_drv_ret_t)0xAAAA0102UL) /* Failed to authentication JTAG with RSIP */
#define CIP_DRV_RET_INIT_FAIL                       ((cip_drv_ret_t)0xAAAA0103UL) /* Failed to Initialize RSIP */
#define CIP_DRV_RET_KEY_FAIL                        ((cip_drv_ret_t)0xAAAA0104UL) /* Wrong key data */

#define HW_SCE_HARDWARE_UNIQUE_KEY_WORD_SIZE        (12u)
#define HW_SCE_ROTPK_HASH_WORD_SIZE                 (8u)

#define HW_SCE_AES128_KEY_INDEX_WORD_SIZE           (12U)
#define HW_SCE_AES256_KEY_INDEX_WORD_SIZE           (16U)
#define HW_SCE_AES_BLOCK_BYTE_SIZE                  (16U)
#define HW_SCE_AES_CBC_IV_BYTE_SIZE                 (16U)
#define HW_SCE_SHA256_HASH_LENGTH_BYTE_SIZE         (32U)

#define HW_SCE_ECC_P256_KEY_SIZE                    (64U)                           /* ECC P256 Key Size Qx + Qy */
#define HW_SCE_ECC_P256_KEY_PARAM_SIZE              (HW_SCE_ECC_P256_KEY_SIZE / 2)  /* ECC P256 Key Size Qx, Qy */
#define HW_SCE_ECDSA_DATA_BYTE_SIZE                 (64U)                           /* ECDSA P256 Sign Size */

#define HW_SCE_HASH_STATE_WORD_SIZE                 (20U)

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
/* key index type */
typedef enum {
	SCE_KEY_INDEX_TYPE_INVALID = 0U,
	SCE_KEY_INDEX_TYPE_AES128
} e_sce_key_index_type_t;

/** AES wrapped key data structure. DO NOT MODIFY. */
typedef struct sce_aes_wrapped_key {
	uint32_t type; ///< key type
	/* AES128, AES256 are supported */
	uint32_t value[HW_SCE_AES256_KEY_INDEX_WORD_SIZE]; ///< wrapped key value
} st_sce_aes_wrapped_key_t;

typedef struct sce_aes_handle {
	/* serial number of this handle */
	uint32_t id;
	/* wrapped key */
	st_sce_aes_wrapped_key_t wrapped_key;
	/* text size under encryption / decryption */
	uint32_t current_input_data_size;
	/* text array less than the block long */
	uint32_t last_1_block_as_fraction[HW_SCE_AES_BLOCK_BYTE_SIZE / sizeof(uint32_t)];
	/* reserved */
	uint32_t last_2_block_as_fraction[(HW_SCE_AES_BLOCK_BYTE_SIZE / sizeof(uint32_t)) * 2];
	/* current initialization vector used in CBC/CTR mode */
	uint32_t current_initial_vector[HW_SCE_AES_CBC_IV_BYTE_SIZE / sizeof(uint32_t)];
	/* control flag of calling function */
	uint8_t flag_call_init;
} st_sce_aes_handle_t;

typedef struct sce_sha_md5_handle {
	uint32_t id;                                              ///< serial number of this handle
	uint32_t sha_buffer[HW_SCE_SHA256_HASH_LENGTH_BYTE_SIZE]; ///< message array length less than the block long
	uint32_t all_received_length;                             ///< entire length of message
	uint32_t buffering_length;                                ///< message array length less than the block long
	uint32_t hash_state[HW_SCE_HASH_STATE_WORD_SIZE];         ///< current hash state
	uint8_t flag_call_init;                                   ///< control flag of calling function
} st_sce_sha_md5_handle_t;

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
extern cip_drv_ret_t R_SCE_Open(void);
extern cip_drv_ret_t R_SCE_Close(void);

extern cip_drv_ret_t R_SCE_VerifyManifestImageSign(uint32_t *image, uint32_t image_len, uint32_t *rotpk_hash,
												   uint32_t *kcert, uint32_t kcert_len, uint32_t *kcert_sign, uint32_t kcert_pk_offset,
												   uint32_t imgpk_hash_offset,
												   uint32_t imgpk_hash_len, uint32_t *ccert, uint32_t ccert_len, uint32_t *ccert_sign, uint32_t ccert_pk_offset);
extern cip_drv_ret_t R_SCE_VerifyManifestImageHash(uint32_t *image, uint32_t image_len, uint32_t *rotpk_hash,
												   uint32_t *kcert, uint32_t kcert_len, uint32_t *kcert_sign, uint32_t kcert_pk_offset,
												   uint32_t imgpk_hash_offset,
												   uint32_t imgpk_hash_len, uint32_t *ccert, uint32_t ccert_len, uint32_t *ccert_sign, uint32_t ccert_pk_offset,
												   uint32_t image_hash_offset, uint32_t image_hash_len);

extern cip_drv_ret_t R_SCE_AES128CBC_DecryptInit(st_sce_aes_handle_t *handle, st_sce_aes_wrapped_key_t *wrapped_key,
												 uint32_t *initial_vector);
extern cip_drv_ret_t R_SCE_AES128CBC_DecryptUpdate(st_sce_aes_handle_t *handle, uint32_t *cipher, uint32_t *plain,
												   uint32_t cipher_length);
extern cip_drv_ret_t R_SCE_AES128CBC_DecryptFinal(st_sce_aes_handle_t *handle, uint32_t *plain,
												  uint32_t *plain_length);

extern cip_drv_ret_t R_SCE_SHA256_Init(st_sce_sha_md5_handle_t *handle, uint32_t message_length);
extern cip_drv_ret_t R_SCE_SHA256_Update(st_sce_sha_md5_handle_t *handle, uint32_t *message, uint32_t message_length);
extern cip_drv_ret_t R_SCE_SHA256_Final(st_sce_sha_md5_handle_t *handle, uint32_t *digest, uint32_t *digest_length);
extern cip_drv_ret_t R_SCE_RandomNumberGenerate(uint32_t *random);

#endif /* RSIP_DRV_H */
