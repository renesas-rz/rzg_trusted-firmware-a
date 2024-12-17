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
#include <common/bl_common.h>

#include "cpg_regs.h"
#include "pwrc.h"
#include "rz_private.h"
#include "rz_soc_def.h"
#include "sys_regs.h"
#include "syc.h"


#define LO_REG							(0U)
#define HI_REG							(1U)

#define SYSTEM_PWR_STATE(s)		((s)->pwr_domain_state[PLAT_MAX_PWR_LVL])
#define CLUSTER_PWR_STATE(s)	((s)->pwr_domain_state[MPIDR_AFFLVL1])
#define CORE_PWR_STATE(s)		((s)->pwr_domain_state[MPIDR_AFFLVL0])

typedef struct {
	uintptr_t reg;
	uint32_t  preq_mask;
	uint32_t  paccept_mask;
	uint32_t  pstate_on_mask;
} CPG_CORE_PWR;

typedef struct {
	unsigned long value __aligned(CACHE_WRITEBACK_GRANULE);
} mailbox_t;

uintptr_t	gp_warm_ep;

static void rz_program_trusted_mailbox(u_register_t mpidr, uintptr_t address)
{
	mailbox_t *mailbox = (mailbox_t *) PLAT_TRUSTED_MAILBOX_BASE;
	uint64_t linear_id = plat_core_pos_by_mpidr(mpidr);
	unsigned long range;

	mailbox[linear_id].value = address;
	range = (unsigned long)&mailbox[linear_id];

	flush_dcache_range(range, sizeof(range));
}

static int rz_validate_ns_entrypoint(uintptr_t ns_entrypoint)
{
	if (ns_entrypoint >= RZG3E_NS_DRAM_BASE)
		return PSCI_E_SUCCESS;

	return PSCI_E_INVALID_ADDRESS;
}

static int rz_validate_power_state(unsigned int power_state, psci_power_state_t *req_state)
{
	int pstate = psci_get_pstate_type(power_state);
	int pwrlvl = psci_get_pstate_pwrlvl(power_state);
	int i;

	if (pstate == PSTATE_TYPE_STANDBY) {
		if (pwrlvl != MPIDR_AFFLVL0)
			return PSCI_E_INVALID_PARAMS;

		req_state->pwr_domain_state[MPIDR_AFFLVL0] = PLAT_MAX_RET_STATE;
	} else {
		for (i = MPIDR_AFFLVL0; i <= pwrlvl; i++)
			req_state->pwr_domain_state[i] = PLAT_MAX_OFF_STATE;
	}

	if (psci_get_pstate_id(power_state))
		return PSCI_E_INVALID_PARAMS;

	return PSCI_E_SUCCESS;
}

static int rzg3e_pwr_domain_on(u_register_t mpidr)
{
	const uint32_t rval[PLATFORM_CORE_COUNT][2] = {
		{ SYS_ACPU_CFG_RVAL0, SYS_ACPU_CFG_RVAH0 },
		{ SYS_ACPU_CFG_RVAL1, SYS_ACPU_CFG_RVAH1 },
		{ SYS_ACPU_CFG_RVAL2, SYS_ACPU_CFG_RVAH2 },
		{ SYS_ACPU_CFG_RVAL3, SYS_ACPU_CFG_RVAH3 }
	};

	const CPG_CORE_PWR pch[PLATFORM_CORE_COUNT] = {
		{ CPG_LP_CA55_CTL2, CPG_LP_CA55_CTL2_COREPREQ0, CPG_LP_CA55_CTL2_COREACCEPT0, CPG_LP_CA55_CTL2_CORESTATE0_ON_MASK },
		{ CPG_LP_CA55_CTL2, CPG_LP_CA55_CTL2_COREPREQ1, CPG_LP_CA55_CTL2_COREACCEPT1, CPG_LP_CA55_CTL2_CORESTATE1_ON_MASK },
		{ CPG_LP_CA55_CTL3, CPG_LP_CA55_CTL3_COREPREQ2, CPG_LP_CA55_CTL3_COREACCEPT2, CPG_LP_CA55_CTL3_CORESTATE2_ON_MASK },
		{ CPG_LP_CA55_CTL3, CPG_LP_CA55_CTL3_COREPREQ3, CPG_LP_CA55_CTL3_COREACCEPT3, CPG_LP_CA55_CTL3_CORESTATE3_ON_MASK }
	};

	uint8_t coreid = MPIDR_AFFLVL1_VAL(mpidr);

	if (coreid >= PLATFORM_CORE_COUNT)
		return PSCI_E_INVALID_PARAMS;

	/* Check if in standby */
	if ((mmio_read_32(CPG_LP_CTL1) & 0x1) == 0x1) {
		mmio_write_32(pch[coreid].reg, pch[coreid].preq_mask);
		while ((mmio_read_32(pch[coreid].reg) & pch[coreid].paccept_mask) != pch[coreid].paccept_mask)
			;
		mmio_write_32(pch[coreid].reg, 0x00000000);
		while ((mmio_read_32(pch[coreid].reg) & pch[coreid].paccept_mask) != 0x0)
			;
	}

	rz_program_trusted_mailbox(mpidr, gp_warm_ep);

	/*  Start the core */
	mmio_write_32(rval[coreid][LO_REG], (uint32_t)(gp_warm_ep & 0xFFFFFFFC));
	mmio_write_32(rval[coreid][HI_REG], (uint32_t)((gp_warm_ep >> 32) & 0xFF));

	/* Assert PORESET */
	mmio_write_32(CPG_RST_0, (0x00010000 << coreid));
	while ((mmio_read_32(CPG_RSTMON_0) & (0x1 << coreid)) == 0x0)
		;

	/* Deassert PORESET and RERESET */
	mmio_write_32(CPG_RST_0, (0x00110011 << coreid));
	while ((mmio_read_32(CPG_RSTMON_0) & (0x1 << coreid)) != 0x0)
		;

	mmio_write_32(pch[coreid].reg, (pch[coreid].pstate_on_mask | pch[coreid].preq_mask));
	while ((mmio_read_32(pch[coreid].reg) & pch[coreid].paccept_mask) != pch[coreid].paccept_mask)
		;

	mmio_write_32(pch[coreid].reg, pch[coreid].pstate_on_mask);
	while ((mmio_read_32(pch[coreid].reg) & pch[coreid].paccept_mask) != 0x0)
		;

	return PSCI_E_SUCCESS;
}

static void rzg3e_pwr_domain_on_finish(const psci_power_state_t *target_state)
{
	plat_gic_pcpu_init();
	plat_gic_cpuif_enable();
}

static void rzg3e_pwr_domain_off(const psci_power_state_t *state)
{
	unsigned long mpidr = read_mpidr_el1();
	uint8_t coreid = MPIDR_AFFLVL1_VAL(mpidr);

	if (coreid >= PLATFORM_CORE_COUNT)
		return;

	/* Prevent interrupts from spuriously waking up this cpu */
	plat_gic_cpuif_disable();

	/* Request transition to Cortex-A55 CoreX Sleep Mode */
	mmio_write_32(CPG_LP_CTL1, (CPG_LP_CTL1_CA55SLEEP_REQ << coreid));


	/* Enter the Cortex-A55 Sleep Mode */
	mmio_write_32(CPG_LP_CTL1, mmio_read_32(CPG_LP_CTL1) | 0x00000001);

	/* Issue Barrier instruction */
	isb();
	dsb();

	/* A WFI instruction will be executed via lib/psci/psci_off.c->psci_power_down_wfi() */
}

#if PLAT_SYSTEM_SUSPEND
static void rz_pwr_domain_suspend(const psci_power_state_t *target_state)
{
	unsigned long mpidr = read_mpidr_el1();

	if (CORE_PWR_STATE(target_state) != PLAT_MAX_OFF_STATE)
		return;

	rz_program_trusted_mailbox(mpidr, gp_warm_ep);

	/* Prevent interrupts from spuriously waking up this cpu */
	plat_gic_cpuif_disable();
	plat_gic_save();
}

static void rz_pwr_domain_suspend_finish(const psci_power_state_t *target_state)
{
	plat_gic_driver_init();
	plat_gic_resume();
	plat_gic_cpuif_enable();

	plat_copy_code_to_system_ram();
	pwrc_setup();
}

static void __dead2 rz_pwr_domain_pwr_down_wfi(const psci_power_state_t *target_state)
{
#if PLAT_SYSTEM_SUSPEND
	if (SYSTEM_PWR_STATE(target_state) == PLAT_MAX_OFF_STATE)
		pwrc_suspend_to_ram();
#endif /* PLAT_SYSTEM_SUSPEND */

	wfi();
	ERROR("RZ/G3E Power Down: operation not handled.\n");
	panic();
}

static void rz_get_sys_suspend_power_state(psci_power_state_t *req_state)
{
	int i;

	for (i = MPIDR_AFFLVL0; i <= PLAT_MAX_PWR_LVL; i++)
		req_state->pwr_domain_state[i] = PLAT_MAX_OFF_STATE;
}
#endif /* PLAT_SYSTEM_SUSPEND */

static void __dead2 rzg3e_system_off(void)
{
	wfi();
	ERROR("RZ/G3E System Off: operation not handled.\n");
	panic();
}


const plat_psci_ops_t rzg3e_plat_psci_ops = {
	/*****PSCI Common function*****/
	.validate_ns_entrypoint				= rz_validate_ns_entrypoint,
	/*****PSCI_CPU_SUSPEND_AARCH64*****/
	.validate_power_state				= rz_validate_power_state,
	/*****PSCI_CPU_ON_AARCH64*****/
	.pwr_domain_on						= rzg3e_pwr_domain_on,
	.pwr_domain_on_finish				= rzg3e_pwr_domain_on_finish,
	/*****PSCI_CPU_OFF*****/
	.pwr_domain_off						= rzg3e_pwr_domain_off,
	/*****PSCI_SYSTEM_OFF*****/
	.system_off							= rzg3e_system_off,
	/*****PSCI_SYSTEM_SUSPEND_AARCH64*****/
	#if PLAT_SYSTEM_SUSPEND
	.pwr_domain_suspend					= rz_pwr_domain_suspend,
	.pwr_domain_suspend_finish			= rz_pwr_domain_suspend_finish,
	.pwr_domain_pwr_down_wfi			= rz_pwr_domain_pwr_down_wfi,
	.get_sys_suspend_power_state		= rz_get_sys_suspend_power_state,
	#endif /* PLAT_SYSTEM_SUSPEND */
};

int plat_setup_psci_ops(uintptr_t sec_entrypoint,
			const plat_psci_ops_t **psci_ops)
{
	gp_warm_ep = sec_entrypoint;
	*psci_ops = &rzg3e_plat_psci_ops;

	return 0;
}

void arm_gicv3_distif_pre_save(unsigned int rdist_proc_num)
{}

void arm_gicv3_distif_post_restore(unsigned int rdist_proc_num)
{}
