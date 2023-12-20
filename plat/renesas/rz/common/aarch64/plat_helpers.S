/*
 * Copyright (c) 2016, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <asm_macros.S>
#include <platform_def.h>
#include <arch.h>
#include <common/bl_common.h>

	.globl	platform_mem_init
	.globl	plat_my_core_pos
	.globl	plat_crash_console_init
	.globl	plat_crash_console_putc
	.globl	plat_crash_console_flush
	.globl	plat_get_my_entrypoint
	.globl	plat_invalidate_icache
#if (PLAT_SOC_RZV2H)
	.globl	plat_secondary_reset
	.globl	plat_get_core_pos
	.globl	bl2_enter_bl31
	.extern	plat_set_my_stack

func plat_secondary_reset
	mrs	x0, sctlr_el3
	bic	x0, x0, #SCTLR_EE_BIT
	msr	sctlr_el3, x0
	isb

	mrs	x0, cptr_el3
	bic	w0, w0, #TCPAC_BIT
	bic	w0, w0, #TTA_BIT
	bic	w0, w0, #TFP_BIT
	msr	cptr_el3, x0

	ldr	x0, =PARAMS_BASE
	ldr	x1, [x0]
	orr	x1, x1, #1
	str	x1, [x0]
	mov_imm	x2, BL31_BASE
	br	x2
	nop
	nop
	nop
endfunc plat_secondary_reset

func plat_get_core_pos
	mrs	x0, mpidr_el1
	lsr x0, x0, #MPIDR_AFF1_SHIFT
	and x0, x0, #MPIDR_CPU_MASK
	ret
endfunc plat_get_core_pos

func bl2_enter_bl31
	mov	x20, x0
	bl	disable_mmu_el3
	mov	x0, #DCCISW
	bl	dcsw_op_all
	tlbi	alle3

	bl	disable_mmu_icache_el3
	ic	iallu
	dsb	sy
	isb
	ldp	x0, x1, [x20, #ENTRY_POINT_INFO_PC_OFFSET]
	msr	elr_el3, x0
	msr	spsr_el3, x1
	exception_return
endfunc bl2_enter_bl31
#endif /* PLAT_SOC_RZV2H */

func platform_mem_init
#if (PLAT_SOC_RZV2H)
#if !IMAGE_BL2
	ldr x0, =PLAT_TRUSTED_MAILBOX_BASE
	mov w1, #PLATFORM_CORE_COUNT
loop:
	str  xzr, [x0], #CACHE_WRITEBACK_GRANULE
	subs w1, w1, #1
	b.gt loop
#endif /* IMAGE_BL2 */
#endif /* PLAT_SOC_RZV2H */
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
	ldr x10, =PLAT_TRUSTED_MAILBOX_BASE
#if (PLAT_SOC_RZV2H)
	bl	plat_get_core_pos
	lsl	x0, x0, #CACHE_WRITEBACK_SHIFT
	ldr x0, [x10, x0]
#else
	ldr x0, [x10]
#endif /* PLAT_SOC_RZV2H */
	cbz x0, el3_panic
exit:
	ret x9
#endif /*IMAGE_BL2 */
endfunc plat_get_my_entrypoint

func plat_invalidate_icache
	ic	iallu
	ret
endfunc plat_invalidate_icache
