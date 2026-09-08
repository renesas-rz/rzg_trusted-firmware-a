/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <assert.h>
#include <stddef.h>
#include <string.h>

#include <common/debug.h>
#include <cip.h>
#include <r_sb_api.h>
#include <drivers/auth/crypto_mod.h>

#include "crypto_sblib.h"

int32_t notify_dec_image_info_callback(uintptr_t dec_base, size_t dec_size)
{
	return 0;
}

int crypto_sblib_auth(void *data_ptr, size_t len,
				const void *key_cert, unsigned int key_cert_len,
				const void *content_cert, unsigned int content_cert_len)
{
	sb_ret_t rc;

	rc = R_SB_SecureBoot(key_cert, key_cert_len, content_cert,
						 content_cert_len, NULL);
	if (rc != SB_RET_SUCCESS)
		return CRYPTO_ERR_UNKNOWN;

	return CRYPTO_SUCCESS;
}

void crypto_sblib_init(void)
{
	cip_init();
}
