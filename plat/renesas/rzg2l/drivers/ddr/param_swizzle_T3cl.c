/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

const uint32_t mc_odt_pins_tbl[4] = {
	2,
	1,
	2,
	1
};

const uint32_t mc_mr1_tbl[2] = {
	0xFFFFFD99,
	0x00000206
};

const uint32_t mc_mr2_tbl[2] = {
	0xFFFFF9FF,
	0x00000200
};

const uint32_t mc_mr5_tbl[2] = {
	0xFFFFFFFF,
	0x00000000
};

const uint32_t mc_mr6_tbl[2] = {
	0xFFFFFFFF,
	0x00000000
};

const uint32_t mc_phy_settings_tbl[][2] = {
	{DENALI_CTL_414,	0x4F7F06E8},
	{DENALI_CTL_415,	0x00002822},
	{DENALI_CTL_416,	0x1E1E3C22},
	{DENALI_CTL_417,	0x00000000}
};

const uint32_t swizzle_mc_tbl[][2] = {
	{DENALI_CTL_403,	0x00000A00},
	{DENALI_CTL_404,	0x00001E00},
	{DENALI_CTL_405,	0x1D131500},
	{DENALI_CTL_406,	0x00001217},
	{DENALI_CTL_407,	0x180E2000},
	{DENALI_CTL_408,	0x0D1F0B1B},
	{DENALI_CTL_409,	0x021A1400},
	{DENALI_CTL_410,	0x0C1C0616},
	{DENALI_CTL_411,	0x00000000}
};

const uint32_t swizzle_phy_tbl[][2] = {
	{PHY_LANE_SEL,			0x00000000	},
	{CA_SWIZZLE_POSITION,	0x19081815	},
	{PHY_LANE_SEL,			0x00000001	},
	{CA_SWIZZLE_POSITION,	0x0702140B	},
	{PHY_LANE_SEL,			0x00000002	},
	{CA_SWIZZLE_POSITION,	0x170D0309	},
	{PHY_LANE_SEL,			0x00000003	},
	{CA_SWIZZLE_POSITION,	0x0F00051D	},
	{PHY_LANE_SEL,			0x00000004	},
	{CA_SWIZZLE_POSITION,	0x13121110	},
	{PHY_LANE_SEL,			0x00000005	},
	{CA_SWIZZLE_POSITION,	0x0E160106	},
	{PHY_LANE_SEL,			0x00000006	},
	{CA_SWIZZLE_POSITION,	0x0A1A1B04	},
	{PHY_LANE_SEL,			0x00000007	},
	{CA_SWIZZLE_POSITION,	0x001E0C1C	}
};
