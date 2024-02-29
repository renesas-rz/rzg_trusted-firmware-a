/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RZ_PLAT_LD_S
#define RZ_PLAT_LD_S

#include <common/bl_common.ld.h>
#include <lib/xlat_tables/xlat_tables_defs.h>

OUTPUT_FORMAT(PLATFORM_LINKER_FORMAT)
OUTPUT_ARCH(PLATFORM_LINKER_ARCH)

#define BL31_SRAM_SIZE	(BL31_SRAM_LIMIT - BL31_SRAM_BASE)

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
		*/ddr*.o (.text* .rodata* .data* .bss*)
		*/pwrc*.o (.text* .rodata* .data* .bss*)
		*/riic*.o (.text* .rodata* .data* .bss*)
		*/cpg*.o (.text* .rodata* .data* .bss*)
		. = ALIGN(PAGE_SIZE);
		__BL31_PMUSRAM_END__ = .;
	} >SRAM AT>DRAM

	.stack (NOLOAD) : {
		__BL31_PMUSTACK_START__ = .;
		*(bl31_pmustack)
		__BL31_PMUSTACK_END__ = .;
	} >SRAM

	ASSERT(__BL31_END__ <= BL31_LIMIT - BL31_SRAM_SIZE,
	"BL31 image too large - writing on top of SRAM!")
}

#endif /* RZ_PLAT_LD_S */
