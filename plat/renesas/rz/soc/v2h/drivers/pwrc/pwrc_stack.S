/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <asm_macros.S>
#include <assert_macros.S>
#include <platform_def.h>

	.global pwrc_func_call_with_pmustack
	.local platform_bl31_pmustack

/*
 * x0 : jump address,
 * x1 : arg,
 */
func pwrc_func_call_with_pmustack

	/* lr to stack */
	stp	x29, x30, [sp, #-16]

	/* get pmustack */
	mov	x3, x0
	get_up_stack platform_bl31_pmustack, PLATFORM_STACK_SIZE

	/* switch to pmustack */
	mov	x2, sp
	mov	sp, x0

	/* save jump addr & stack pointer */
	sub	sp, sp, #16
	stp	x3, x2, [sp]

	/* data synchronization barrier */
	dsb	sy

	/* jump to code */
	mov	x0, x1
	blr	x3

	/* load stack pointer */
	ldp	x0, x1, [sp, #0]

	/* switch to stack pointer */
	mov	sp, x1

	/* return */
	ldp	x29, x30, [sp, #-16]
	ret
endfunc pwrc_func_call_with_pmustack

declare_stack platform_bl31_pmustack, bl31_pmustack, \
		PLATFORM_STACK_SIZE, 1, CACHE_WRITEBACK_GRANULE
