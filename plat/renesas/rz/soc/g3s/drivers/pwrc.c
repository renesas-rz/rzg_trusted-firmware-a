/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <string.h>

#include <arch.h>
#include <arch_helpers.h>
#include <common/debug.h>
#include <lib/mmio.h>
#include <lib/xlat_tables/xlat_tables_v2.h>
#include <plat/common/platform.h>

#include <cpg.h>
#include <pwrc.h>
#include <sys_regs.h>
#include <rz_private.h>

static void __dead2 pwrc_go_suspend_to_ram(void)
{
	// TODO
	while (1)
		wfi();
}

bool pwrc_is_ddr_retention_mode(void)
{
	return false;
}

void pwrc_suspend_to_ram(void)
{
	/* disable MMU */
	disable_mmu_el3();

	cpg_suspend_setup();

	pwrc_go_suspend_to_ram();
}

void pwrc_setup(void)
{
	uintptr_t sec_entrypoint = (uintptr_t)BL2_BASE;

	uint32_t rvah0 = (uint32_t)(sec_entrypoint >> 32);
	uint32_t rval0 = (uint32_t)(sec_entrypoint & 0xFFFFFFFF);

	mmio_write_32(SYS_CA55_CFG_RVAH0, rvah0);
	mmio_write_32(SYS_CA55_CFG_RVAL0, rval0);
}
