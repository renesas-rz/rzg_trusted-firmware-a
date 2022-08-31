/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <arch_helpers.h>
#include <plat/common/platform.h>

#define RANDOM_CANARY_VALUE    ((u_register_t) 3673632101744508211ULL)

u_register_t plat_get_stack_protector_canary(void)
{
    return RANDOM_CANARY_VALUE;
}
