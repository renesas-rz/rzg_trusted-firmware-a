/*
 * Copyright (c) 2024, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/debug.h>
#include <lib/mmio.h>
#include <lib/xlat_tables/xlat_tables_v2.h>
#include <cdefs.h>

#include <plat/common/platform.h>
#include <cpg.h>
#include <ddr.h>
#include <pwrc.h>
#include <sys_regs.h>
#include <cpg_regs.h>
#include <rz_private.h>

#define V2N_ICU_SWINT		UL(RZV2N_ICU + 0x0130)
#define INT_NUM_TO_USE		0
#define SHIFT_FOR_IM33		16

/*
 * Power Controller Setup
 *  - Set the reset vector of secondary cores
 *  - Setup I2C for PMIC access
 */
void pwrc_setup(void)
{
	const uint32_t rval[PLATFORM_CORE_COUNT][2] = {
		{ SYS_ACPU_CFG_RVAL0, SYS_ACPU_CFG_RVAH0 },
		{ SYS_ACPU_CFG_RVAL1, SYS_ACPU_CFG_RVAH1 },
		{ SYS_ACPU_CFG_RVAL2, SYS_ACPU_CFG_RVAH2 },
		{ SYS_ACPU_CFG_RVAL3, SYS_ACPU_CFG_RVAH3 }
	};

	unsigned int i;
	uint32_t rvah0 = (uint32_t)(((uintptr_t)&plat_secondary_reset >> 32) & 0xFF);
	uint32_t rval0 = (uint32_t)((uintptr_t)&plat_secondary_reset & 0xFFFFFFFC);

	for (i = 0; i < PLATFORM_CORE_COUNT; i++) {
		mmio_write_32(rval[i][1], rvah0);
		mmio_write_32(rval[i][0], rval0);
	}
}
#if PLAT_SYSTEM_SUSPEND
extern void pwrc_func_call_with_pmustack(uintptr_t jump, void *arg);

void __section(".sram")
pwrc_go_suspend_to_ram(void)
{
	ddr_retention_entry();

	cpg_suspend_setup();

	/* Notify CM33 */
	mmio_write_32(V2N_ICU_SWINT, 1 << (INT_NUM_TO_USE + SHIFT_FOR_IM33));

	mmio_write_32(CPG_LP_CTL1, CPG_LP_CTL1_CA55SLEEP_REQ_MSK);

	while (1)
		wfi();

	/*
	 * This function never returns from here.
	 * The core is powered off and re-enters through another point in the code (BL2).
	 */
}

void __dead2 pwrc_suspend_to_ram(void)
{
	console_flush();

	/* flush all caches */
	dcsw_op_all(DCCISW);

	/* disable MMU */
	disable_mmu_el3();

	dsb();
	isb();

	/* switch to stack */
	pwrc_func_call_with_pmustack((uintptr_t)pwrc_go_suspend_to_ram, NULL);

	panic();
}
#endif /* PLAT_SYSTEM_SUSPEND */
