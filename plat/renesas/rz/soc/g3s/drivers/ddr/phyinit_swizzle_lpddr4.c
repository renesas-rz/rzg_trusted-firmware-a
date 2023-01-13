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

// const uint32_t ddrphy_ini_swizzle[][2] =
// {
// 	{0x058100, 0x0005},
// 	{0x058101, 0x0004},
// 	{0x058102, 0x0003},
// 	{0x058103, 0x0001},
// 	{0x058104, 0x0003},
// 	{0x058105, 0x0001},
// };

const uint32_t ddrphy_ini_swizzle[][2] =
{
	{0x058100, 0x0005},
	{0x058101, 0x0004},
	{0x058102, 0x0002},
	{0x058103, 0x0001},
	{0x058104, 0x0003},
	{0x058105, 0x0000},
	{0x0300A0, 0x0003},
	{0x0300A1, 0x0001},
	{0x0300A2, 0x0005},
	{0x0300A3, 0x0002},
	{0x0300A4, 0x0004},
	{0x0300A5, 0x0007},
	{0x0300A6, 0x0006},
	{0x0300A7, 0x0000},
	{0x0310A0, 0x0004},
	{0x0310A1, 0x0007},
	{0x0310A2, 0x0006},
	{0x0310A3, 0x0005},
	{0x0310A4, 0x0001},
	{0x0310A5, 0x0000},
	{0x0310A6, 0x0003},
	{0x0310A7, 0x0002},
};

const uint32_t ddrphy_ini_swizzle_size = ARRAY_SIZE(ddrphy_ini_swizzle);
