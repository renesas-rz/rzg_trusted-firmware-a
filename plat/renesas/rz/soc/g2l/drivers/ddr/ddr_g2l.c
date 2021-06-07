/*
 * Copyright (c) 2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <ddr_internal.h>

void ddr_ctrl_reten_en_n(uint8_t val)
{
	val &= 1;
	rmw_phy_reg(DDRPHY_R78, 0xFFFEFFFF, (val << 16));
}
