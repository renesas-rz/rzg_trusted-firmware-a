/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <drivers/arm/tzc400.h>

#define TZC400_FILTER_MAX		(4)
#define TZC400_ID_ACC_REG0		(0x000F000F)

void plat_tzc400_setup(uintptr_t tzc_base)
{
	tzc400_init(tzc_base);
//	tzc400_disable_filters();
	tzc400_configure_region0(TZC_REGION_S_NONE, TZC400_ID_ACC_REG0);
	tzc400_enable_filters();

	/* TODO */
	/* Disables read/write access speculation. */
	/* tzc400 driver does not support */
}
