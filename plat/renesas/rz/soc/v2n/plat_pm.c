/*
 * Copyright (c) 2024, Renesas Electronics Corporation. All rights reserved.
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
#include <cpg.h>
#include <wdt.h>
#include <core_ctrl.h>
#include <cpg_regs.h>
#include <sys_regs.h>
#include <rz_private.h>
#include <rz_soc_def.h>
#include <common/bl_common.h>
#include <pwrc.h>
#include <ddr.h>


#define LO_REG							(0U)
#define HI_REG							(1U)

#define SYSTEM_PWR_STATE(s)				((s)->pwr_domain_state[PLAT_MAX_PWR_LVL])
#define CLUSTER_PWR_STATE(s)			((s)->pwr_domain_state[MPIDR_AFFLVL1])
#define CORE_PWR_STATE(s)				((s)->pwr_domain_state[MPIDR_AFFLVL0])

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

const uint32_t cores_reset_vector[PLATFORM_CORE_COUNT][2] = {
	{ SYS_ACPU_CFG_RVAL0, SYS_ACPU_CFG_RVAH0 },
	{ SYS_ACPU_CFG_RVAL1, SYS_ACPU_CFG_RVAH1 },
	{ SYS_ACPU_CFG_RVAL2, SYS_ACPU_CFG_RVAH2 },
	{ SYS_ACPU_CFG_RVAL3, SYS_ACPU_CFG_RVAH3 }
};

static void rz_program_trusted_mailbox(u_register_t mpidr, uintptr_t address)
{
	mailbox_t *mailbox = (mailbox_t *) PLAT_TRUSTED_MAILBOX_BASE;
	uint64_t linear_id = plat_core_pos_by_mpidr(mpidr);
	unsigned long range;

	mailbox[linear_id].value = address;
	range = (unsigned long)&mailbox[linear_id];

	flush_dcache_range(range, sizeof(range));
}

static int rzv2n_pwr_domain_on(u_register_t mpidr)
{
	uint8_t coreid = MPIDR_AFFLVL1_VAL(mpidr);

	if (coreid >= PLATFORM_CORE_COUNT)
		return PSCI_E_INVALID_PARAMS;

	rz_program_trusted_mailbox(mpidr, gp_warm_ep);

	/*  Set Reset Vector */
	mmio_write_32(cores_reset_vector[coreid][LO_REG], (uint32_t)(gp_warm_ep & 0xFFFFFFFC));
	mmio_write_32(cores_reset_vector[coreid][HI_REG], (uint32_t)((gp_warm_ep >> 32) & 0xFF));

	/* Reset applied to set */
	core_ctrl_warm_boot(coreid);

	return PSCI_E_SUCCESS;
}

static void rzv2n_pwr_domain_on_finish(const psci_power_state_t *target_state)
{
	plat_gic_pcpu_init();
	plat_gic_cpuif_enable();
}

static void rzv2n_pwr_domain_off(const psci_power_state_t *state)
{
	unsigned long mpidr = read_mpidr_el1();

	uint8_t coreid = MPIDR_AFFLVL1_VAL(mpidr);

	if (coreid >= PLATFORM_CORE_COUNT) {
		return;
	}

	/* Prevent interrupts from spuriously waking up this cpu */
	plat_gic_cpuif_disable();

	/* Request transition to Cortex-A55 CoreX Sleep Mode */
	core_ctrl_set_in_standby(coreid);
}

#if PLAT_SYSTEM_SUSPEND

static void rzv2n_pwr_domain_suspend(const psci_power_state_t *target_state)
{
	unsigned long mpidr = read_mpidr_el1();

	if (CORE_PWR_STATE(target_state) != PLAT_MAX_OFF_STATE)
		return;

	mmio_write_8(RESUME_MAILBOX_BASE, 1);
	flush_dcache_range((uintptr_t)RESUME_MAILBOX_BASE, (size_t)1);
	rz_program_trusted_mailbox(mpidr, gp_warm_ep);

	pwrc_setup();
	/* Prevent interrupts from spuriously waking up this cpu */
	plat_gic_cpuif_disable();
	plat_gic_save();
}

static void rzv2n_pwr_domain_suspend_finish(const psci_power_state_t *target_state)
{
	plat_gic_driver_init();
	plat_gic_resume();
	plat_gic_cpuif_enable();

	pwrc_setup();
	plat_copy_code_to_system_ram();
}

static void __dead2 rzv2n_pwr_domain_pwr_down_wfi(const psci_power_state_t *target_state)
{
	if (SYSTEM_PWR_STATE(target_state) == PLAT_MAX_OFF_STATE) {
		pwrc_suspend_to_ram();
	}

	wfi();
	ERROR("RZ/V2N Power Down: operation not handled.\n");
	panic();
}

static void rzv2n_get_sys_suspend_power_state(psci_power_state_t *req_state)
{
	int i;

	for (i = MPIDR_AFFLVL0; i <= PLAT_MAX_PWR_LVL; i++)
		req_state->pwr_domain_state[i] = PLAT_MAX_OFF_STATE;
}
#endif /* PLAT_SYSTEM_SUSPEND */

static void __dead2 rzv2n_system_off(void)
{
	/* Set the CPG_LP_PWC_CTL1.ALL_OFF_TRG bit to allow desired power-off sequencing */
	mmio_write_32(CPG_LP_PWC_CTL1, CPG_LP_PWC_CTL1_ALL_OFF_TRG);

	wfi();
	ERROR("RZ/V2N System Off: operation not handled.\n");
	panic();
}

static void __dead2 rzv2n_system_reset(void)
{
	INFO("RZ/V2N System Reset\n");

	cpg_reset_wdt1();

	for (int i = 0; i < PLATFORM_CORE_COUNT; i++) {
		mmio_write_32(cores_reset_vector[i][LO_REG], 0x00000000);
		mmio_write_32(cores_reset_vector[i][HI_REG], 0x00000000);
	}

	cpg_setup_wdt1();

	console_flush();

	/* Issue Barrier instruction */
	isb();
	dsb();

	wdt_system_reset();

	while (1) {

	}
	panic();
}


const plat_psci_ops_t rzv2n_plat_psci_ops = {
	/******PSCI_CPU_ON_AARCH64*****/
	.pwr_domain_on						= rzv2n_pwr_domain_on,
	.pwr_domain_on_finish				= rzv2n_pwr_domain_on_finish,
	/*********PSCI_CPU_OFF*********/
	.pwr_domain_off						= rzv2n_pwr_domain_off,
	/********PSCI_SYSTEM_OFF*******/
	.system_off							= rzv2n_system_off,
	/*******PSCI_SYSTEM_RESET******/
	.system_reset						= rzv2n_system_reset,
	/**PSCI_SYSTEM_SUSPEND_AARCH64*/
#if PLAT_SYSTEM_SUSPEND
	.pwr_domain_suspend					= rzv2n_pwr_domain_suspend,
	.pwr_domain_suspend_finish			= rzv2n_pwr_domain_suspend_finish,
	.pwr_domain_pwr_down_wfi			= rzv2n_pwr_domain_pwr_down_wfi,
	.get_sys_suspend_power_state		= rzv2n_get_sys_suspend_power_state,
#endif /* PLAT_SYSTEM_SUSPEND */
};

int plat_setup_psci_ops(uintptr_t sec_entrypoint,
			const plat_psci_ops_t **psci_ops)
{
	gp_warm_ep = sec_entrypoint;
	*psci_ops = &rzv2n_plat_psci_ops;

	return 0;
}

void arm_gicv3_distif_pre_save(unsigned int rdist_proc_num)
{}

void arm_gicv3_distif_post_restore(unsigned int rdist_proc_num)
{}
