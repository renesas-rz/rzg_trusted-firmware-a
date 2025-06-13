/*
 * Copyright (c) 2024, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <otp.h>
#include <otp_regs.h>
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

	ret_val = r_otp_read(OTP_ANTI_RB_SETTING_ADDR, setting, 1);
	if (true == ret_val)
	{
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

	ret_val = r_otp_read(OTP_ANTI_RB_VER_ADDR, version, 1);
	if (true == ret_val)
	{
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

	if (OTP_ROOT_PUBLIC_KEY_HASH_SIZE != hash_size)
	{
		return 1;
	}

	ret_val = r_otp_read(OTP_ROOT_PUBLIC_KEY_HASH_ADDR, hash, hash_size);
	if (true == ret_val)
	{
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
	uint32_t result = 1;
	bool ret_val = false;

	uint32_t offset = OTP_SECURE_BOOT_COMMON_KEY1_ADDR + (key_size * key_id);

	if (OTP_SECURE_BOOT_COMMON_KEY_SIZE != key_size)
	{
		return 1;
	}

	ret_val = r_otp_read(offset, key, key_size);
	if (true == ret_val)
	{
		result = 0;
	}

	return result;
}
#endif