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

static void rz_program_trusted_mailbox(uintptr_t address)
{
	uintptr_t *mailbox = (uintptr_t *) PLAT_TRUSTED_MAILBOX_BASE;

	*mailbox = address;
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

#if PLAT_SYSTEM_SUSPEND
static void rz_pwr_domain_suspend(const psci_power_state_t *target_state)
{
	if (CORE_PWR_STATE(target_state) != PLAT_MAX_OFF_STATE)
		return;

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

static int rz_validate_ns_entrypoint(uintptr_t ns_entrypoint)
{
	if (ns_entrypoint >= RZG3E_NS_DRAM_BASE)
		return PSCI_E_SUCCESS;

	return PSCI_E_INVALID_ADDRESS;
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
	/*****PSCI_CPU_SUSPEND_AARCH64*****/
	.cpu_standby						= NULL,
	.validate_power_state				= rz_validate_power_state,

	/**********************************/
	/*****PSCI_CPU_ON_AARCH64*****/
	.pwr_domain_on						= NULL,
	.pwr_domain_on_finish				= NULL,
	.pwr_domain_off						= NULL,
	/*****************************/
	/*****PSCI_SYSTEM_OFF*****/
	.system_off							= rzg3e_system_off,
	/*************************/
	/*****PSCI_SYSTEM_SUSPEND_AARCH64*****/
	#if PLAT_SYSTEM_SUSPEND
	.pwr_domain_suspend					= rz_pwr_domain_suspend,
	.pwr_domain_suspend_finish			= rz_pwr_domain_suspend_finish,
	.pwr_domain_pwr_down_wfi			= rz_pwr_domain_pwr_down_wfi,
	.validate_ns_entrypoint				= rz_validate_ns_entrypoint,
	.get_sys_suspend_power_state		= rz_get_sys_suspend_power_state,
	#endif /* PLAT_SYSTEM_SUSPEND */
	/*************************************/
};

int plat_setup_psci_ops(uintptr_t sec_entrypoint,
			const plat_psci_ops_t **psci_ops)
{
	rz_program_trusted_mailbox(sec_entrypoint);
	*psci_ops = &rzg3e_plat_psci_ops;

	return 0;
}

void arm_gicv3_distif_pre_save(unsigned int rdist_proc_num)
{}

void arm_gicv3_distif_post_restore(unsigned int rdist_proc_num)
{}
