/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <arch_helpers.h>
#include <plat/common/platform.h>
#include <cip.h>

#define RANDOM_CANARY_VALUE		((u_register_t) 3673632101744508211ULL)

u_register_t plat_get_stack_protector_canary(void)
{
	u_register_t random = RANDOM_CANARY_VALUE;
#if TRUSTED_BOARD_BOOT
	cip_rng((uintptr_t)&random, sizeof(random));
#endif
	return random;
}
