/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
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

#include <syc.h>
#include <pwrc.h>
#include <cpg.h>
#include <sys_regs.h>
#include <rz_private.h>
#include <rz_soc_def.h>
#include <common/bl_common.h>
#include <cpg_regs.h>
#include <wdt.h>
#include <pfc.h>
#include <rz_console.h>
#include <core_ctrl.h>

#define LO_REG							(0U)
#define HI_REG							(1U)

#define SYSTEM_PWR_STATE(s)		((s)->pwr_domain_state[PLAT_MAX_PWR_LVL])
#define CLUSTER_PWR_STATE(s)	((s)->pwr_domain_state[MPIDR_AFFLVL1])
#define CORE_PWR_STATE(s)		((s)->pwr_domain_state[MPIDR_AFFLVL0])

typedef struct {
	uintptr_t reg_pchctl;
	uintptr_t reg_pchmon;
} CPG_CORE_PWR;

typedef unsigned long aligned_mailbox_value_t __aligned(CACHE_WRITEBACK_GRANULE);

typedef struct {
	aligned_mailbox_value_t value; /* Mailbox must be cache aligned */
} mailbox_t;

uintptr_t	gp_warm_ep;

const uint32_t cores_reset_vector[PLATFORM_CORE_COUNT][2] = {
	{ SYS_CA55_CFG_RVAL0, SYS_CA55_CFG_RVAH0 },
	{ SYS_CA55_CFG_RVAL1, SYS_CA55_CFG_RVAH1 },
	{ SYS_CA55_CFG_RVAL2, SYS_CA55_CFG_RVAH2 },
	{ SYS_CA55_CFG_RVAL3, SYS_CA55_CFG_RVAH3 }
};

static void rz_cpu_standby(plat_local_state_t cpu_state)
{
	/** Application CPU Sleep Mode **/

	u_register_t scr_el3 = read_scr_el3();

	mmio_write_32(SYS_LP_CTL2, 0x00000001);

	write_scr_el3(scr_el3 | SCR_IRQ_BIT | SCR_FIQ_BIT);
	dsb();
	wfi();
	write_scr_el3(scr_el3);

	mmio_write_32(SYS_LP_CTL2, 0x00000000);
}


static int rz_validate_ns_entrypoint(uintptr_t ns_entrypoint)
{
	if (ns_entrypoint >= RZG3L_NS_DRAM_BASE)
		return PSCI_E_SUCCESS;

	return PSCI_E_INVALID_ADDRESS;
}

#if PLAT_SYSTEM_SUSPEND
static void rz_program_trusted_mailbox(u_register_t mpidr, uintptr_t address)
{
	mailbox_t *mailbox = (mailbox_t *) PLAT_TRUSTED_MAILBOX_BASE;
	uint64_t linear_id = plat_core_pos_by_mpidr(mpidr);
	unsigned long range;

	mailbox[linear_id].value = address;
	range = (unsigned long)&mailbox[linear_id];

	flush_dcache_range(range, sizeof(range));
}

static void rz_pwr_domain_suspend(const psci_power_state_t *target_state)
{
	cpg_prepare_suspend();
	rz_console_deinit();

	unsigned long mpidr = read_mpidr_el1();

	pfc_riic_pmic_setup();

	if (CORE_PWR_STATE(target_state) != PLAT_MAX_OFF_STATE)
		return;

	rz_program_trusted_mailbox(mpidr, gp_warm_ep);

	/* Prevent interrupts from spuriously waking up this cpu */
	plat_gic_cpuif_disable();
	plat_gic_save();

	/* Enable the transition request interrupt to the Cortex-A55 Sleep Mode */
	mmio_write_32(SYS_LP_CTL6, 0x00000100);
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

	if (SYSTEM_PWR_STATE(target_state) == PLAT_MAX_OFF_STATE) {
		pwrc_suspend_to_ram();
	}

	wfi();
	ERROR("RZ/G3L Power Down: operation not handled.\n");
	panic();
}

static void rz_get_sys_suspend_power_state(psci_power_state_t *req_state)
{
	int i;

	for (i = MPIDR_AFFLVL0; i <= PLAT_MAX_PWR_LVL; i++)
		req_state->pwr_domain_state[i] = PLAT_MAX_OFF_STATE;
}
#endif /* PLAT_SYSTEM_SUSPEND */


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

static void __dead2 rz_system_off(void)
{
	wfi();
	ERROR("RZ/G3L System Off: operation not handled.\n");
	panic();
}

static int rzg3l_pwr_domain_on(u_register_t mpidr)
{
	uint8_t coreid = MPIDR_AFFLVL1_VAL(mpidr);

	if (coreid >= PLATFORM_CORE_COUNT)
		return PSCI_E_INVALID_PARAMS;

	/*  Set Reset Vector */
	mmio_write_32(cores_reset_vector[coreid][LO_REG], (uint32_t)(gp_warm_ep & 0xFFFFFFFC));
	mmio_write_32(cores_reset_vector[coreid][HI_REG], (uint32_t)((gp_warm_ep >> 32) & 0xFF));

	core_ctrl_warm_boot(coreid);

	return PSCI_E_SUCCESS;
}

static void rzg3l_pwr_domain_on_finish(const psci_power_state_t *target_state)
{
	plat_gic_pcpu_init();
	plat_gic_cpuif_enable();
}

static void rzg3l_pwr_domain_off(const psci_power_state_t *state)
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

static void __dead2 rzg3l_system_reset(void)
{
	INFO("RZ/G3L System Reset\n");

	cpg_reset_wdt0();

	for (int i = 0; i < PLATFORM_CORE_COUNT; i++) {
		mmio_write_32(cores_reset_vector[i][LO_REG], 0x00000000);
		mmio_write_32(cores_reset_vector[i][HI_REG], 0x00000000);
	}

	cpg_setup_wdt0();

	console_flush();

	/* Issue Barrier instruction */
	isb();
	dsb();

	wdt_system_reset();

	for (;;) {

	}

	panic();
}

const plat_psci_ops_t rz_plat_psci_ops = {
	/*****PSCI Common function*****/
	.validate_ns_entrypoint				= rz_validate_ns_entrypoint,
	/*****PSCI_CPU_SUSPEND_AARCH64*****/
	.cpu_standby						= rz_cpu_standby,
	/*****PSCI_CPU_ON_AARCH64*****/
	.pwr_domain_on						= rzg3l_pwr_domain_on,
	.pwr_domain_on_finish				= rzg3l_pwr_domain_on_finish,
	/*****PSCI_CPU_OFF*****/
	.pwr_domain_off						= rzg3l_pwr_domain_off,
	/*****PSCI_CPU_SUSPEND_AARCH64*****/
	.validate_power_state				= rz_validate_power_state,
	/*****PSCI_SYSTEM_OFF*****/
	.system_off							= rz_system_off,
	/*****PSCI_SYSTEM_RESET*****/
	.system_reset						= rzg3l_system_reset,
	/*****PSCI_SYSTEM_SUSPEND_AARCH64*****/
#if PLAT_SYSTEM_SUSPEND
	.pwr_domain_suspend					= rz_pwr_domain_suspend,
	.pwr_domain_suspend_finish			= rz_pwr_domain_suspend_finish,
	.pwr_domain_pwr_down_wfi			= rz_pwr_domain_pwr_down_wfi,
	.get_sys_suspend_power_state		= rz_get_sys_suspend_power_state,
#endif /* PLAT_SYSTEM_SUSPEND */
};

int plat_setup_psci_ops(uintptr_t sec_entrypoint, const plat_psci_ops_t **psci_ops)
{
	gp_warm_ep = sec_entrypoint;
	*psci_ops = &rz_plat_psci_ops;
	return 0;
}

void arm_gicv3_distif_pre_save(unsigned int rdist_proc_num)
{}

void arm_gicv3_distif_post_restore(unsigned int rdist_proc_num)
{}
