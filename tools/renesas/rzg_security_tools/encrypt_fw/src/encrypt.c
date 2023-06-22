/*
 * Copyright (c) 2019, Linaro Limited. All rights reserved.
 * Author: Sumit Garg <sumit.garg@linaro.org>
 *
 * Copyright (c) 2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <firmware_encrypted.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <string.h>
#include "debug.h"
#include "encrypt.h"

#define BUFFER_SIZE		256
#define IV_SIZE			16
#define IV_STRING_SIZE		32
#define KEY_SIZE		16
#define KEY_STRING_SIZE		32

static int cbc_encrypt(char *key_string,
		       char *nonce_string, const char *ip_name,
		       const char *op_name)
{
	FILE *ip_file;
	FILE *op_file;
	EVP_CIPHER_CTX *ctx;
	unsigned char data[BUFFER_SIZE], enc_data[BUFFER_SIZE];
	unsigned char key[KEY_SIZE], iv[IV_SIZE];
	int bytes, enc_len = 0, i, j, ret = 0;

	if (strlen(key_string) != KEY_STRING_SIZE) {
		ERROR("Unsupported key size: %lu\n", strlen(key_string));
		return -1;
	}

	for (i = 0, j = 0; i < KEY_SIZE; i++, j += 2) {
		if (sscanf(&key_string[j], "%02hhx", &key[i]) != 1) {
			ERROR("Incorrect key format\n");
			return -1;
		}
	}

	if (strlen(nonce_string) != IV_STRING_SIZE) {
		ERROR("Unsupported IV size: %lu\n", strlen(nonce_string));
		return -1;
	}

	for (i = 0, j = 0; i < IV_SIZE; i++, j += 2) {
		if (sscanf(&nonce_string[j], "%02hhx", &iv[i]) != 1) {
			ERROR("Incorrect IV format\n");
			return -1;
		}
	}

	ip_file = fopen(ip_name, "rb");
	if (ip_file == NULL) {
		ERROR("Cannot read %s\n", ip_name);
		return -1;
	}

	op_file = fopen(op_name, "wb");
	if (op_file == NULL) {
		ERROR("Cannot write %s\n", op_name);
		fclose(ip_file);
		return -1;
	}

	ctx = EVP_CIPHER_CTX_new();
	if (ctx == NULL) {
		ERROR("EVP_CIPHER_CTX_new failed\n");
		ret = -1;
		goto out_file;
	}

	ret = EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, NULL, NULL);
	if (ret != 1) {
		ERROR("EVP_EncryptInit_ex failed\n");
		ret = -1;
		goto out;
	}
	
	ret = EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv);
	if (ret != 1) {
		ERROR("EVP_EncryptInit_ex failed\n");
		goto out;
	}

	EVP_CIPHER_CTX_set_padding(ctx, 0);

	while ((bytes = fread(data, 1, BUFFER_SIZE, ip_file)) != 0) {
		ret = EVP_EncryptUpdate(ctx, enc_data, &enc_len, data, bytes);
		if (ret != 1) {
			ERROR("EVP_EncryptUpdate failed\n");
			ret = -1;
			goto out;
		}

		fwrite(enc_data, 1, enc_len, op_file);
	}

	ret = EVP_EncryptFinal_ex(ctx, enc_data, &enc_len);
	if (ret != 1) {
		ERROR("EVP_EncryptFinal_ex failed\n");
		ret = -1;
		goto out;
	}

	fwrite(enc_data, 1, enc_len, op_file);
	
	EVP_CIPHER_CTX_cleanup(ctx);

out:
	EVP_CIPHER_CTX_free(ctx);

out_file:
	fclose(ip_file);
	fclose(op_file);

	/*
	 * EVP_* APIs returns 1 as success but enctool considers
	 * 0 as success.
	 */
	if (ret == 1)
		ret = 0;

	return ret;
}

int encrypt_file(int enc_alg, char *key_string,
		 char *nonce_string, const char *ip_name, const char *op_name)
{
	switch (enc_alg) {
	case KEY_ALG_CBC:
		return cbc_encrypt(key_string, nonce_string,
				   ip_name, op_name);
	default:
		return -1;
	}
}
