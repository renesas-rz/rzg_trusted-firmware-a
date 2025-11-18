/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights
 * reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <platform_def.h>
#include <R_TSIP_Boot_Lib.h>
#include "tsip_lib.h"

static struct {
	uint32_t inst[0x0510 / sizeof(uint32_t)];
	uint32_t head[0x0030 / sizeof(uint32_t)];
	uint32_t sram[0x0200 / sizeof(uint32_t)];
} tsip_data;

static void *s_shrm = NULL;
static void *s_heap = NULL;

int tsip_auth_init(void *shrm, void *heap)
{
	s_shrm = shrm;
	s_heap = heap;
	return 0;
}

int tsip_auth_keys(void *key, unsigned long len)
{
	unsigned long ret;

	memcpy(tsip_data.inst, key, sizeof(tsip_data.inst));

	ret = R_TSIP_Init((uint8_t *)tsip_data.sram, (uint8_t *)key);
	if (ret != R_PASS) {
		memset(&tsip_data, 0, sizeof(tsip_data));
		return 1;
	}

	memcpy(s_shrm, &tsip_data, sizeof(tsip_data));

	return 0;
}

int tsip_auth_image(void *src, unsigned long len, void *dst)
{
	unsigned long ret;

	static bool s_flag = true;

	if (s_flag) {
		/* Backup header data for firmware update */
		uintptr_t head = ((uintptr_t)src + len) - sizeof(tsip_data.head);
		memcpy(tsip_data.head, (void *)head, sizeof(tsip_data.head));
		s_flag = false;
	}

	ret = R_TSIP_VerifyBootData(src, len, s_heap, dst);
	if (ret != R_PASS) {
		memset(&tsip_data, 0, sizeof(tsip_data));
		return 1;
	}

	memcpy(s_shrm, &tsip_data, sizeof(tsip_data));

	return 0;
}
