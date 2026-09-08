/*
 * Copyright (c) 2016, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <asm_macros.S>
#include <platform_def.h>

	.globl	platform_mem_init
	.globl	plat_my_core_pos
	.globl	plat_crash_console_init
	.globl	plat_crash_console_putc
	.globl	plat_crash_console_flush
	.globl	plat_get_my_entrypoint
	.globl	plat_invalidate_icache

func platform_mem_init
	ret
endfunc platform_mem_init

func plat_my_core_pos
	mrs x0, mpidr_el1
	lsr x0, x0, #MPIDR_AFF1_SHIFT
	and x0, x0, #MPIDR_CPU_MASK
	ret
endfunc plat_my_core_pos

func plat_crash_console_init
	mov x0, #1
	ret
endfunc plat_crash_console_init

func plat_crash_console_putc
	ret
endfunc plat_crash_console_putc

func plat_crash_console_flush
	mov x0, #0
	ret
endfunc plat_crash_console_flush

func plat_get_my_entrypoint
	mov	x9, x30 /* lr */

#if defined(IMAGE_BL2)
	/* always cold boot on bl2 */
	mov x0, #0
	ret x9
#else
	ldr x1, =PARAMS_BASE
	ldr x21, [x1]

	/* Check the reset info */
	and x1, x21, #0x000c
	cmp x1, #0x0008
	beq el3_panic
	cmp x1, #0x000c
	beq el3_panic

	/* Check the boot kind */
	and x1, x21, #0x0003
	cmp x1, #0x0002
	beq el3_panic
	cmp x1, #0x0003
	beq el3_panic

	/* warm boot or cold boot */
	and x1, x21, #1
	cmp x1, #0
	bne warm_reset

	/* Cold boot */
	mov x0, #0
	b   exit

warm_reset:
	ldr x1, =PLAT_TRUSTED_MAILBOX_BASE
	ldr x0, [x1]
	cbz x0, el3_panic
exit:
	ret x9
#endif
endfunc plat_get_my_entrypoint

func plat_invalidate_icache
	ic	iallu
	ret
endfunc plat_invalidate_icache
