/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

#include "rom_api.h"

typedef uint32_t(*rom_get_lcs_api_f) (uint32_t *lcs);

uint32_t rzg_rom_get_lcs(uint32_t *lcs)
{
	static const uintptr_t rom_get_lcs = 0xEB10018CU;
	rom_get_lcs_api_f get_lcs;

	get_lcs = (rom_get_lcs_api_f) rom_get_lcs;

	return get_lcs(lcs);
}
