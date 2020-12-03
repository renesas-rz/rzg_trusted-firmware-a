/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <drivers/arm/tzc400.h>

#define TZC400_FILTER_MAX		(4)
#define TZC400_PORT_MAX			(4)
#define TZC400_NSAID_MASK		((1 << TZC400_PORT_MAX) - 1)

void plat_tzc400_setup(uintptr_t tzc_base)
{
	tzc400_init(tzc_base);
	tzc400_disable_filters();
	tzc400_configure_region0(TZC_REGION_S_RDWR, 
		(TZC400_NSAID_MASK << TZC_REGION_ACCESS_WR_EN_SHIFT) | TZC400_NSAID_MASK);
	tzc400_set_action(TZC_ACTION_ERR);
	tzc400_enable_filters();
}
