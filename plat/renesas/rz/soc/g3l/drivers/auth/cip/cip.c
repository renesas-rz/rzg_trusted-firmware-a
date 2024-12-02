/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <assert.h>
#include <stddef.h>
#include <string.h>

#include <common/debug.h>
#include <platform_def.h>
#include <cip.h>

#if !TRUSTED_BOARD_BOOT
#error "TRUSTED_BOARD_BOOT must be enabled"
#endif

typedef uint32_t (*cip_init_api_t)(void);
typedef uint32_t (*cip_hukl_api_t)(void);
typedef uint32_t (*cip_rand_api_t)(uint32_t *random);

static cip_rand_api_t cip_rand_api;

void cip_init(void)
{
	static bool first_call = true;

	if (first_call) {
#if IMAGE_BL2
		cip_init_api_t cip_init_api;
		cip_hukl_api_t cip_hukl_api;

		cip_init_api = (cip_init_api_t)RZ_SOC_CIP_INIT_BASE;
		cip_hukl_api = (cip_hukl_api_t)RZ_SOC_CIP_HUKL_BASE;

		cip_init_api();
		cip_hukl_api();
#endif
		cip_rand_api = (cip_rand_api_t)RZ_SOC_CIP_RAND_BASE;

		first_call = false;
	}
}

void cip_rng(uintptr_t buffer, size_t len)
{
	static uint32_t random[4];

	assert((void *)buffer != NULL);

	cip_init();

	for (size_t i = 0; i < len; i += sizeof(random)) {
		cip_rand_api(random);
		memcpy((void *)(buffer + i), random, MIN(len - i, sizeof(random)));
	}
}
