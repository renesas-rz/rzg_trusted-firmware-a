/*
 * Copyright (c) 2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <stdio.h>
#include <string.h>
#include "debug.h"
#include "sign.h"

#define BUFFER_SIZE		256
#define KEY_SIZE		16
#define KEY_STRING_SIZE	32

const EVP_MD *get_digest(int alg)
{
	switch (alg) {
	case HASH_ALG_SHA256:
		return EVP_sha256();
	default:
		return NULL;
	}
}

static int rsa_sign(char *key_name, int md_alg, const char *ip_name,
			const char *op_name, unsigned int align)
{
	FILE *ip_file = NULL;
	FILE *op_file = NULL;
	FILE *kp_file = NULL;
	EVP_MD_CTX *ctx = NULL;
	EVP_PKEY *pkey = NULL;
	unsigned char data[BUFFER_SIZE];
	unsigned char sign[BUFFER_SIZE];
	size_t sig_size = BUFFER_SIZE;
	int bytes, ret = 0;
	unsigned long in_fsz, pad_sz;
	
	ip_file = fopen(ip_name, "rb");
	if (ip_file == NULL) {
		ERROR("Cannot read %s\n", ip_name);
		ret = -1;
		goto out;
	}

	op_file = fopen(op_name, "wb");
	if (op_file == NULL) {
		ERROR("Cannot write %s\n", op_name);
		ret = -1;
		goto out;
	}

	kp_file = fopen(key_name, "rb");
	if (op_file == NULL) {
		ERROR("Cannot read %s\n", ip_name);
		ret = -1;
		goto out;
	}
	
	pkey = PEM_read_PrivateKey(kp_file, NULL, NULL, NULL);
	if (pkey == NULL) {
		ERROR("PEM_read_PrivateKey failed\n");
		ret = -1;
		goto out;
	}
	
	if (BUFFER_SIZE < EVP_PKEY_size(pkey)) {
		ERROR("EVP_PKEY_size failed\n");
		ret = -1;
		goto out;
	}

	ctx = EVP_MD_CTX_create();
	if (ctx == NULL) {
		ERROR("EVP_MD_CTX_create failed\n");
		ret = -1;
		goto out;
	}

	ret = EVP_DigestSignInit(ctx, NULL, get_digest(md_alg), NULL, pkey);
	if (ret != 1) {
		ERROR("EVP_DigestSignInit failed\n");
		ret = -1;
		goto out;
	}
	
	in_fsz = 0;
	while ((bytes = fread(data, 1, BUFFER_SIZE, ip_file)) != 0) {
		ret = EVP_DigestSignUpdate(ctx, data, bytes);
		if (ret != 1) {
			ERROR("EVP_DigestSignUpdate failed\n");
			ret = -1;
			goto out;
		}
		in_fsz += fwrite(data, 1, bytes, op_file);
	}
	
	memset(data, 0, BUFFER_SIZE);
	pad_sz = ((in_fsz + (align - 1)) & (~(align - 1))) - in_fsz;
	while (pad_sz > 0) { 
		bytes = (pad_sz < BUFFER_SIZE) ? pad_sz : BUFFER_SIZE;
		pad_sz -= bytes;
		ret = EVP_DigestSignUpdate(ctx, data, bytes);
		if (ret != 1) {
			ERROR("EVP_DigestSignUpdate failed\n");
			ret = -1;
			goto out;
		}
		fwrite(data, 1, bytes, op_file);
	}
	
	ret = EVP_DigestSignFinal(ctx, sign, &sig_size);
	if (ret != 1) {
		ERROR("EVP_DigestSignFinal failed\n");
		ret = -1;
		goto out;
	}

	fwrite(sign, 1, sig_size, op_file);
	
	EVP_MD_CTX_reset(ctx);

out:
	if (NULL != ctx)
		EVP_MD_CTX_destroy(ctx);
	if (NULL != pkey)
		EVP_PKEY_free(pkey);
	if (NULL != ip_file)
		fclose(ip_file);
	if (NULL != op_file)
		fclose(op_file);
	if (NULL != kp_file)
		fclose(kp_file);

	/*
	 * EVP_* APIs returns 1 as success but signtool considers
	 * 0 as success.
	 */
	if (ret == 1)
		ret = 0;

	return ret;
}

int sign_file(int key_alg, char *key_name, int md_alg, 
		const char *ip_name, const char *op_name, unsigned int align)
{
	switch (key_alg) {
	case KEY_ALG_RSA:
		return rsa_sign(key_name, md_alg, ip_name, op_name, align);
	default:
		return -1;
	}
}
