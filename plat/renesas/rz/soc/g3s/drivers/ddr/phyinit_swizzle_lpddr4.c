/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <lib/utils_def.h>

/////////////////////////////////////////////////////////////////////////////////
// 32_phyinit_swizzle_lpddr4

const uint32_t ddrphy_ini_swizzle[][2] =
{
	{0x020100, 0x0005},
	{0x020101, 0x0004},
	{0x020102, 0x0003},
	{0x020103, 0x0001},
	{0x020104, 0x0003},
	{0x020105, 0x0000},
};

const uint32_t ddrphy_ini_swizzle_size = ARRAY_SIZE(ddrphy_ini_swizzle);
