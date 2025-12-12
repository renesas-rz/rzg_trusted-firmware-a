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
#include <vbatt_regs.h>

extern void pwrc_func_call_with_pmustack(uintptr_t jump, void *arg);

static void __dead2 pwrc_go_suspend_to_ram(void)
{
	/*
	 * The console output will become unavailable after the DDR goes into retention mode.
	 * Be sure to avoid printing anything after this point.
	 */
	ddr_retention_entry();

#if PLAT_SYSTEM_SUSPEND_vbat
	mmio_write_32(SYS_PWRRDY_N, PWRRDY_N_USB_OFF | PWRRDY_N_DSI_OFF | PWRRDY_N_CSI_OFF);
	cpg_setup_vbat_suspend();
	/* VBATT area shut-off control */
	mmio_write_32(VBATT_ISOENPROT, WPROT_WRITE_ENABLE);
	mmio_write_32(VBATT_ISOEN, ISOEN_ON);

	pwrc_board_suspend_on();
#endif

	mmio_write_32(SYS_LP_CTL2, CA55_STBYCTL_SLEEP_START);

	while (1)
		wfi();
	/*
	 * This function never returns from here.
	 * The core is powered off and re-enters through another point in the code (BL2).
	 */
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
	const uint32_t rval[PLATFORM_CORE_COUNT][2] = {
		{ SYS_CA55_CFG_RVAL0, SYS_CA55_CFG_RVAH0 },
		{ SYS_CA55_CFG_RVAL1, SYS_CA55_CFG_RVAH1 },
		{ SYS_CA55_CFG_RVAL2, SYS_CA55_CFG_RVAH2 },
		{ SYS_CA55_CFG_RVAL3, SYS_CA55_CFG_RVAH3 }
	};

	uint8_t i;
	uint32_t rvah0 = (uint32_t)(((uintptr_t)&plat_secondary_reset >> 32) & CA55_CFG_RVAH_MASK);
	uint32_t rval0 = (uint32_t)((uintptr_t)&plat_secondary_reset & CA55_CFG_RVAL_MASK);

	for (i = 0; i < PLATFORM_CORE_COUNT; i++) {
		mmio_write_32(rval[i][1], rvah0);
		mmio_write_32(rval[i][0], rval0);
	}
}
