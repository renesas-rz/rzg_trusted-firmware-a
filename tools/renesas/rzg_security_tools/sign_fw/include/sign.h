/*
 * Copyright (c) 2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SIGN_H
#define SIGN_H

/* Supported key algorithms */
enum {
	KEY_ALG_RSA,		/* RSASSA-PKCS1-v1_5 (default) */
	KEY_ALG_MAX_NUM
};

/* Supported hash algorithms */
enum{
	HASH_ALG_SHA256,
};

int sign_file(int key_alg, char *key_name, int md_alg, 
		const char *ip_name, const char *op_name, unsigned int align);

#endif /* SIGN_H */
