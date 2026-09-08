/*
 * Copyright (c) 2024-2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <otp.h>
#include <otp_data.h>
#include <otp_drv.h>

/***********************************************************************************************************************
* Function Name: otp_read_anti_rb_setting
* Description  : Get anti-rollback version configuration register.
* Arguments    : setting - Anti-rollback version configuration register value.
* Return Value : 1:ERR or 0:OK
***********************************************************************************************************************/
uint32_t otp_read_anti_rb_setting(uint32_t *setting)
{
	uint32_t result = 1;
	bool ret_val = false;

	ret_val = r_otp_read_wrapper(OTP_ANTI_RB_SETTING_ADDR, (uint16_t *)setting, OTP_ANTI_RB_SETTING_ADDR_LEN);
	if (true == ret_val) {
		result = 0;
	}

	return result;
}

/***********************************************************************************************************************
* Function Name: otp_read_anti_rb_version
* Description  : Get anti-rollback version register.
* Arguments    : version  - Anti-rollback version register value.
* Return Value : 1:ERR or 0:OK
***********************************************************************************************************************/
uint32_t otp_read_anti_rb_version(uint32_t *version)
{
	uint32_t result = 1;
	bool ret_val = false;

	ret_val = r_otp_read_wrapper(OTP_ANTI_RB_VER_ADDR, (uint16_t *)version, OTP_ANTI_RB_VER_ADDR_LEN);
	if (true == ret_val) {
		result = 0;
	}

	return result;
}

#if TRUSTED_BOARD_BOOT
/***********************************************************************************************************************
* Function Name: otp_read_rotpk_hash
* Description  : Get hash.
* Arguments    : hash      - Hash register value.
*              : hash_size - Hash register size.
* Return Value : 1:ERR or 0:OK
***********************************************************************************************************************/
uint32_t otp_read_rotpk_hash(uint32_t *hash, uint32_t hash_size)
{
	uint32_t result = 1;
	bool ret_val = false;

	if (OTP_ROOT_PUBLIC_KEY_HASH_LEN != (hash_size << 1)) {
		return 1;
	}

	ret_val = r_otp_read_wrapper(OTP_ROOT_PUBLIC_KEY_HASH_ADDR, (uint16_t *)hash, OTP_ROOT_PUBLIC_KEY_HASH_LEN);
	if (true == ret_val) {
		result = 0;
	}

	return result;
}

/***********************************************************************************************************************
* Function Name: otp_read_decrypt_key
* Description  : Get Decrtypt key.
* Arguments    : key_id   - Decrypt key id.
*              : key      - Decrypt key value.
*              : key_size - Decrypt key size.
* Return Value : 1:ERR or 0:OK
***********************************************************************************************************************/
uint32_t otp_read_decrypt_key(uint32_t key_id, uint32_t *key, uint32_t key_size)
{
	const uint32_t decrypt_key[4] = {
		OTP_SECURE_BOOT_COMMON_KEY1_ADDR,
		OTP_SECURE_BOOT_COMMON_KEY2_ADDR,
		OTP_SECURE_BOOT_COMMON_KEY3_ADDR,
		OTP_SECURE_BOOT_COMMON_KEY4_ADDR
	};

	uint32_t result = 1;
	bool ret_val = false;

	uint32_t offset = decrypt_key[key_id];

	if (OTP_SECURE_BOOT_COMMON_KEY_LEN != (key_size << 1)) {
		return 1;
	}

	ret_val = r_otp_read_wrapper(offset, (uint16_t *)key, OTP_SECURE_BOOT_COMMON_KEY_LEN);
	if (true == ret_val) {
		result = 0;
	}

	return result;
}
#endif

/***********************************************************************************************************************
* Function Name: r_otp_read_wrapper
* Description  : read otp halfward.
* Arguments    : addr    - OTP address
*              : p_value - OTP value
*              : count   - OTP size.
* Return Value : false or true
***********************************************************************************************************************/
bool r_otp_read_wrapper(uint32_t addr, uint16_t *p_value, uint32_t count)
{
	bool ret_val = false;
	uint32_t read_data;

	for (int i = 0; i < count; i++) {
		ret_val = r_otp_read(addr + i, &read_data, 1);
		if (true == ret_val) {
			p_value[i] = (uint16_t)read_data;
		} else {
			break;
		}
	}
	return ret_val;
}
