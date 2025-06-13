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
#include <rsip_drv.h>
#include <cip.h>

#if !TRUSTED_BOARD_BOOT
#error "TRUSTED_BOARD_BOOT must be enabled"
#endif

volatile uint32_t * gp_sce;

void cip_init(void)
{
	static bool first_call = true;

	if (first_call) {

		R_SCE_Open();

		first_call = false;
	}
}

void cip_rng(uintptr_t buffer, size_t len)
{
	static uint32_t random[4];

	assert((void *)buffer != NULL);

	cip_init();

	for (size_t i = 0; i < len; i+= sizeof(random)) {
		R_SCE_RandomNumberGenerate(random);
		memcpy((void *)(buffer + i), random, MIN(len - i, sizeof(random)));
	}
}
