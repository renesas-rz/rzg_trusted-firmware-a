/*
 * Copyright (c) 2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ENCRYPT_H
#define ENCRYPT_H

/* Supported key algorithms */
enum {
	KEY_ALG_CBC		/* AES-CBC (default) */
};

int encrypt_file(int enc_alg, char *key_string,
		 char *nonce_string, const char *ip_name, const char *op_name);

#endif /* ENCRYPT_H */
