/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#include <arch.h>
#include <arch_helpers.h>
#include <common/debug.h>
#include <lib/mmio.h>
#include <lib/xlat_tables/xlat_tables_v2.h>
#include <plat/common/platform.h>

#include <cpg.h>
#include <ddr.h>
#include <pwrc.h>
#include <sys_regs.h>
#include <rz_private.h>
#include <pwrc_board.h>

extern void pwrc_func_call_with_pmustack(uintptr_t jump, void *arg);

static void __dead2 pwrc_go_suspend_to_ram(void)
{
	cpg_prepare_suspend();

	ddr_retention_entry();

	cpg_suspend_setup();

	pwrc_board_suspend_on();

	while (1)
		wfi();
}

void __dead2 pwrc_suspend_to_ram(void)
{
	/* flash all caches */
	dcsw_op_all(DCCISW);

	/* disable MMU */
	disable_mmu_el3();

	pwrc_func_call_with_pmustack((uintptr_t)pwrc_go_suspend_to_ram, NULL);

	panic();
}

void pwrc_setup(void)
{
	uintptr_t sec_entrypoint = (uintptr_t)BL2_BASE;

	uint32_t rvah0 = (uint32_t)(sec_entrypoint >> 32);
	uint32_t rval0 = (uint32_t)(sec_entrypoint & 0xFFFFFFFF);

	mmio_write_32(SYS_CA55_CFG_RVAH0, rvah0);
	mmio_write_32(SYS_CA55_CFG_RVAL0, rval0);

	pwrc_board_setup();
}
