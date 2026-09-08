/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// RZ/T2N

#ifndef __PARAM_DDRINIT
#define __PARAM_DDRINIT

#include <stdint.h>

extern const char ddr_version_str[];

extern const uint32_t param_setup_mc[][2];

extern const uint32_t param_dram_boot_clk_period;
extern const uint32_t param_dram_clk_period;

extern const uint8_t param_phyinit_00;
extern const uint8_t param_phyinit_01;
extern const uint8_t param_phyinit_02;
extern const uint8_t param_phyinit_03;
extern const uint8_t param_phyinit_10;
extern const uint32_t param_phyinit_11;
extern const uint32_t param_phyinit_12;
extern const uint32_t param_phyinit_13;

extern const uint8_t param_ddr_swizzle_ca[];
extern const uint8_t param_ddr_swizzle_cs[];
extern const uint8_t param_ddr_swizzle_bit[];
extern const uint8_t param_ddr_swizzle_byte[];

extern const uint32_t param_setup_mc_size;

#endif // __PARAM_DDRINIT
