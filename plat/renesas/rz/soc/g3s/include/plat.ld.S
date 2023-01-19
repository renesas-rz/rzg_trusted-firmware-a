/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RZ_PLAT_LD_S
#define RZ_PLAT_LD_S

#include <lib/xlat_tables/xlat_tables_defs.h>

MEMORY {
	SRAM (rwx): ORIGIN = BL31_SRAM_BASE, LENGTH = BL31_SRAM_SIZE
	DRAM (r): ORIGIN = BL31_LIMIT - BL31_SRAM_SIZE, LENGTH = BL31_SRAM_SIZE
}

SECTIONS
{
	. = BL31_LIMIT - BL31_SRAM_SIZE;

	__BL31_PMUSRAM_BASE__ = .;

	/* system ram start is in SRAM */
	.sram : ALIGN(PAGE_SIZE) {
		__BL31_PMUSRAM_START__ = .;
		*pwrc.o
		*riic.o
		. = ALIGN(PAGE_SIZE);
		__BL31_PMUSRAM_END__ = .;
	} >SRAM AT>DRAM

	ASSERT(__BL31_END__ <= BL31_LIMIT - BL31_SRAM_SIZE,
	"BL31 image too large - writing on top of SRAM!")
}

#endif /* RZ_PLAT_LD_S */
