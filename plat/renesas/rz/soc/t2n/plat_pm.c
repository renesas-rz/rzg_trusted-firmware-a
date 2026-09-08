/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch_helpers.h>
#include <assert.h>
#include <lib/psci/psci.h>
#include <lib/mmio.h>
#include <common/debug.h>
#include <lib/bakery_lock.h>
#include <plat/common/platform.h>
#include <sys_regs.h>
#include <sys.h>
#include <rz_private.h>
#include <rz_soc_def.h>
#include <common/bl_common.h>

#define LO_REG							(0U)
#define HI_REG							(1U)
#define MAX_PLATFORM_CORE_COUNT			(2U)

#define CORE_ON_STATE					(1U)
#define CORE_OFF_STATE					(0U)

uintptr_t gp_warm_ep;

static volatile uint8_t cores_state[PLATFORM_CORE_COUNT];

const uint32_t rval[MAX_PLATFORM_CORE_COUNT][2] = {
	{ CA55_RVBAL0, CA55_RVBAH0 },
	{ CA55_RVBAL1, CA55_RVBAH1 }
};

static int rzt2n_pwr_domain_on(u_register_t mpidr)
{
	const uint32_t swrcpu[MAX_PLATFORM_CORE_COUNT][1] = {
		{ SWR550 },
		{ SWR551 }
	};

	uint8_t coreid = MPIDR_AFFLVL1_VAL(mpidr);

	if (coreid >= PLATFORM_CORE_COUNT)
		return PSCI_E_INVALID_PARAMS;

	if (cores_state[coreid] == CORE_ON_STATE) {
		ERROR("BL31: Core %u is already ON.\n", coreid);
		return PSCI_E_ALREADY_ON;
	}

	/* Un-lock writing to CA55_RVBAxy registers */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);

	/* Write rebase address */
	mmio_write_32(rval[coreid][LO_REG], (uint32_t)(gp_warm_ep & 0xFFFFFFFC));
	mmio_write_32(rval[coreid][HI_REG], (uint32_t)((gp_warm_ep >> 32) & 0xFF));

	/* Disable Write Protection */
	sys_safetybase_unlock(PRCRx_LOW_POWER);

#if FSP_A551_SUPPORT
	if (coreid < 1) {
#endif
		/* Release the reset state of CA55 cluster */
		mmio_write_32(SWR55C, 0x00000000);

		/* Release the reset state of each CA55 core */
		mmio_write_32(swrcpu[coreid][0], 0x00000000);
#if FSP_A551_SUPPORT
	}
#endif

	/* Deliberately not Re-locking Registers */

	/* Disable Write Protection of PRCRN register*/
	sys_base_unlock(PRCRx_CLOCK_GEN);
	sys_base_unlock(PRCRx_LOW_POWER);
	sys_base_unlock(PRCRx_GPIO);
	sys_base_unlock(PRCRx_SYS_CTRL);

	/* Disable Write Protection of PRCRS register (LOW_POWER & SYS_CTRL already disabled above)*/
	sys_safetybase_unlock(PRCRx_CLOCK_GEN);
	sys_safetybase_unlock(PRCRx_GPIO);

#if FSP_A551_SUPPORT
	if (coreid < 1) {
#endif
		/* Set the core state to ON */
		cores_state[coreid] = CORE_ON_STATE;
		flush_dcache_range((uintptr_t)&cores_state, sizeof(cores_state));
		dsb();
		isb();
#if FSP_A551_SUPPORT
	}
#endif

	sev();

	return PSCI_E_SUCCESS;
}

static void rzt2n_pwr_domain_on_finish(const psci_power_state_t *target_state)
{
	plat_gic_pcpu_init();
	plat_gic_cpuif_enable();
}

static void rzt2n_pwr_domain_off(const psci_power_state_t *state)
{
	unsigned long mpidr = read_mpidr_el1();
	uint8_t coreid = MPIDR_AFFLVL1_VAL(mpidr);

	if (coreid >= PLATFORM_CORE_COUNT)
		return;

	/* Prevent interrupts from spuriously waking up this cpu */
	plat_gic_cpuif_disable();

	/* Set the core state to OFF */
	cores_state[coreid] = CORE_OFF_STATE;

	/* Issue Barrier instruction */
	isb();
	dsb();

	/* A WFI instruction will be executed via lib/psci/psci_off.c->psci_power_down_wfi() */
}

static void __dead2 rzt2n_pwr_domain_pwr_down_wfi(const psci_power_state_t *target_state)
{
	unsigned long mpidr = read_mpidr_el1();
	uint8_t coreid = MPIDR_AFFLVL1_VAL(mpidr);

	if (coreid >= PLATFORM_CORE_COUNT) {
		ERROR("Exceeds platform core count.\n");
		panic();
	}

	/*
	 * Perform cache maintenance - hardware control not used.
	 * Cache is flushed and disabled to ensure that no dirtly
	 * cache lines remain.
	 */
	dcsw_op_all(DCCISW);
	disable_mmu_el3();

	while ((cores_state[coreid] == CORE_OFF_STATE))
		wfe();

	write_rmr_el3(RMR_EL3_RR_BIT | RMR_EL3_AA64_BIT);

	dsbsy();

	for (;;)
		wfi();
}

static void __dead2 rzt2n_system_off(void)
{
	wfi();
	ERROR("RZ/T2N System Off: operation not handled.\n");
	panic();
}

static void __dead2 rzt2n_system_reset(void)
{
	sys_safetybase_unlock(PRCRx_LOW_POWER);
	mmio_write_32(SWRSYS, 0x4321A501);
	sys_safetybase_lock(PRCRx_LOW_POWER);
	wfi();
	panic();
}

const plat_psci_ops_t rzt2n_plat_psci_ops = {
	.pwr_domain_on				= rzt2n_pwr_domain_on,
	.pwr_domain_on_finish		= rzt2n_pwr_domain_on_finish,
	.pwr_domain_off				= rzt2n_pwr_domain_off,
	.pwr_domain_pwr_down_wfi	= rzt2n_pwr_domain_pwr_down_wfi,
	.system_off					= rzt2n_system_off,
	.system_reset				= rzt2n_system_reset,
};

int plat_setup_psci_ops(uintptr_t sec_entrypoint,
			const plat_psci_ops_t **psci_ops)
{
	gp_warm_ep = sec_entrypoint;
	*psci_ops = &rzt2n_plat_psci_ops;

	unsigned long mpidr = read_mpidr_el1();
	uint8_t coreid = MPIDR_AFFLVL1_VAL(mpidr);

	if (coreid >= PLATFORM_CORE_COUNT) {
		ERROR("Exceeds platform core count.\n");
		panic();
	}

	cores_state[coreid] = CORE_ON_STATE;

	/* Un-lock writing to CA55_RVBAxy registers */
	sys_safetybase_unlock(PRCRx_SYS_CTRL);

	/* Write rebase address */
	mmio_write_32(rval[coreid][LO_REG], (uint32_t)(gp_warm_ep & 0xFFFFFFFC));
	mmio_write_32(rval[coreid][HI_REG], (uint32_t)((gp_warm_ep >> 32) & 0xFF));

	sys_safetybase_lock(PRCRx_SYS_CTRL);

	return 0;
}
