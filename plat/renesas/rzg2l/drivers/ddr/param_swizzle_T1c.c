/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

const uint32_t mc_odt_pins_tbl[4] = {
	1,
	2,
	0,
	0
};

const uint32_t mc_mr1_tbl[2] = {
	0xFFFFF8F9,
	0x00000200
};

const uint32_t mc_mr2_tbl[2] = {
	0xFFFFF1FF,
	0x00000000
};

const uint32_t mc_mr5_tbl[2] = {
	0xFFFFFE3F,
	0x000001C0
};

const uint32_t mc_mr6_tbl[2] = {
	0xFFFFFF80,
	0x0000001C
};

const uint32_t mc_phy_settings_tbl[][2] = {
	{DENALI_CTL_414,	0x576E08E8},
	{DENALI_CTL_415,	0x00003C22},
	{DENALI_CTL_416,	0x22227822},
	{DENALI_CTL_417,	0x00123312}
};

const uint32_t swizzle_mc_tbl[][2] = {
	{DENALI_CTL_403,	0x00001502},
	{DENALI_CTL_404,	0x00000A06},
	{DENALI_CTL_405,	0x170D1112},
	{DENALI_CTL_406,	0x1900130B},
	{DENALI_CTL_407,	0x1F09180C},
	{DENALI_CTL_408,	0x161C0E14},
	{DENALI_CTL_409,	0x01101B1A},
	{DENALI_CTL_410,	0x001D051E},
	{DENALI_CTL_411,	0x00000000}
};

const uint32_t swizzle_phy_tbl[][2] = {
	{DDRPHY_R29,	0x00000000},
	{DDRPHY_R11,	0x1B03150E},
	{DDRPHY_R29,	0x00000001},
	{DDRPHY_R11,	0x16001706},
	{DDRPHY_R29,	0x00000002},
	{DDRPHY_R11,	0x0D050807},
	{DDRPHY_R29,	0x00000003},
	{DDRPHY_R11,	0x1901020B},
	{DDRPHY_R29,	0x00000004},
	{DDRPHY_R11,	0x13121110},
	{DDRPHY_R29,	0x00000005},
	{DDRPHY_R11,	0x0F090418},
	{DDRPHY_R29,	0x00000006},
	{DDRPHY_R11,	0x1A0A1D14},
	{DDRPHY_R29,	0x00000007},
	{DDRPHY_R11,	0x001E1C0C}
};
