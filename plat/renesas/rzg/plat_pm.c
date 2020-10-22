/*
 * Copyright (c) 2015-2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <errno.h>

#include <platform_def.h>

#include <arch_helpers.h>
#include <common/bl_common.h>
#include <common/debug.h>
#include <drivers/arm/cci.h>
#include <drivers/arm/gicv2.h>
#include <lib/bakery_lock.h>
#include <lib/mmio.h>
#include <lib/psci/psci.h>
#include <plat/common/platform.h>

#include "iic_dvfs.h"
#include "pwrc.h"
#include "rzg_def.h"
#include "rzg_private.h"

#define DVFS_SET_VID_0V		(0x00)
#define P_ALL_OFF		(0x80)
#define KEEPON_DDR1C		(0x08)
#define KEEPON_DDR0C		(0x04)
#define KEEPON_DDR1		(0x02)
#define KEEPON_DDR0		(0x01)

#define SYSTEM_PWR_STATE(s)	((s)->pwr_domain_state[PLAT_MAX_PWR_LVL])
#define CLUSTER_PWR_STATE(s)	((s)->pwr_domain_state[MPIDR_AFFLVL1])
#define CORE_PWR_STATE(s)	((s)->pwr_domain_state[MPIDR_AFFLVL0])

extern void rzg_pwrc_restore_generic_timer(uint64_t *stack);
extern void plat_rzg_gic_driver_init(void);
extern void plat_rzg_gic_init(void);
extern u_register_t rzg_boot_mpidr;

static uintptr_t rzg_sec_entrypoint;

static void rzg_program_mailbox(uint64_t mpidr, uint64_t address)
{
	mailbox_t *rzg_mboxes = (mailbox_t *) MBOX_BASE;
	uint64_t linear_id = plat_core_pos_by_mpidr(mpidr);
	unsigned long range;

	rzg_mboxes[linear_id].value = address;
	range = (unsigned long)&rzg_mboxes[linear_id];

	flush_dcache_range(range, sizeof(range));
}

static void rzg_cpu_standby(plat_local_state_t cpu_state)
{
	u_register_t scr_el3 = read_scr_el3();

	write_scr_el3(scr_el3 | SCR_IRQ_BIT);
	dsb();
	wfi();
	write_scr_el3(scr_el3);
}

static int rzg_pwr_domain_on(u_register_t mpidr)
{
	rzg_program_mailbox(mpidr, rzg_sec_entrypoint);
	rzg_pwrc_cpuon(mpidr);

	return PSCI_E_SUCCESS;
}

static void rzg_pwr_domain_on_finish(const psci_power_state_t *target_state)
{
	uint32_t cluster_type = rzg_pwrc_get_cluster();
	unsigned long mpidr = read_mpidr_el1();

	if (CLUSTER_PWR_STATE(target_state) == PLAT_MAX_OFF_STATE)
		if (cluster_type == RZG_CLUSTER_A53A57)
			plat_cci_enable();

	rzg_pwrc_disable_interrupt_wakeup(mpidr);
	rzg_program_mailbox(mpidr, 0);

	gicv2_cpuif_enable();
	gicv2_pcpu_distif_init();
}

static void rzg_pwr_domain_off(const psci_power_state_t *target_state)
{
	uint32_t cluster_type = rzg_pwrc_get_cluster();
	unsigned long mpidr = read_mpidr_el1();

	gicv2_cpuif_disable();
	rzg_pwrc_cpuoff(mpidr);

	if (CLUSTER_PWR_STATE(target_state) == PLAT_MAX_OFF_STATE) {
		if (cluster_type == RZG_CLUSTER_A53A57)
			plat_cci_disable();

		rzg_pwrc_clusteroff(mpidr);
	}
}

static void rzg_pwr_domain_suspend(const psci_power_state_t *target_state)
{
	uint32_t cluster_type = rzg_pwrc_get_cluster();
	unsigned long mpidr = read_mpidr_el1();

	if (CORE_PWR_STATE(target_state) != PLAT_MAX_OFF_STATE)
		return;

	rzg_program_mailbox(mpidr, rzg_sec_entrypoint);
	rzg_pwrc_enable_interrupt_wakeup(mpidr);
	gicv2_cpuif_disable();
	rzg_pwrc_cpuoff(mpidr);

	if (CLUSTER_PWR_STATE(target_state) == PLAT_MAX_OFF_STATE) {
		if (cluster_type == RZG_CLUSTER_A53A57)
			plat_cci_disable();

		rzg_pwrc_clusteroff(mpidr);
	}
#if RZG_SYSTEM_SUSPEND
	if (SYSTEM_PWR_STATE(target_state) == PLAT_MAX_OFF_STATE)
		rzg_pwrc_suspend_to_ram();
#endif
}

static void rzg_pwr_domain_suspend_finish(const psci_power_state_t
					   *target_state)
{
	uint32_t cluster_type = rzg_pwrc_get_cluster();

	if (SYSTEM_PWR_STATE(target_state) != PLAT_MAX_OFF_STATE)
		goto finish;

	plat_rzg_gic_driver_init();
	plat_rzg_gic_init();

	if (cluster_type == RZG_CLUSTER_A53A57)
		plat_cci_init();

	rzg_pwrc_restore_timer_state();
	rzg_pwrc_setup();
	rzg_pwrc_code_copy_to_system_ram();

#if RZG_SYSTEM_SUSPEND
	rzg_pwrc_init_suspend_to_ram();
#endif
finish:
	rzg_pwr_domain_on_finish(target_state);
}

static void __dead2 rzg_system_off(void)
{
#if PMIC_ROHM_BD9571
#if PMIC_LEVEL_MODE
	if (rzg_iic_dvfs_send(PMIC, DVFS_SET_VID, DVFS_SET_VID_0V))
		ERROR("BL3-1:Failed the SYSTEM-OFF.\n");
#else
	if (rzg_iic_dvfs_send(PMIC, BKUP_MODE_CNT, P_ALL_OFF))
		ERROR("BL3-1:Failed the SYSTEM-RESET.\n");
#endif
#else
	uint64_t cpu = read_mpidr_el1() & 0x0000ffff;
	int32_t rtn_on;

	rtn_on = rzg_pwrc_cpu_on_check(cpu);

	if (cpu == rzg_boot_mpidr)
		panic();

	if (rtn_on)
		panic();

	rzg_pwrc_cpuoff(cpu);
	rzg_pwrc_clusteroff(cpu);

#endif /* PMIC_ROHM_BD9571 */
	wfi();
	ERROR("RZG System Off: operation not handled.\n");
	panic();
}

static void __dead2 rzg_system_reset(void)
{
#if PMIC_ROHM_BD9571
#if PMIC_LEVEL_MODE
#if RZG_SYSTEM_RESET_KEEPON_DDR
	uint8_t mode;
	int32_t error;

	error = rzg_iic_dvfs_send(PMIC, REG_KEEP10, KEEP10_MAGIC);
	if (error) {
		ERROR("Failed send KEEP10 magic ret=%d \n", error);
		goto done;
	}

	error = rzg_iic_dvfs_receive(PMIC, BKUP_MODE_CNT, &mode);
	if (error) {
		ERROR("Failed recieve BKUP_Mode_Cnt ret=%d \n", error);
		goto done;
	}

	mode |= KEEPON_DDR1C | KEEPON_DDR0C | KEEPON_DDR1 | KEEPON_DDR0;
	error = rzg_iic_dvfs_send(PMIC, BKUP_MODE_CNT, mode);
	if (error) {
		ERROR("Failed send KEEPON_DDRx ret=%d \n", error);
		goto done;
	}

	rzg_pwrc_set_suspend_to_ram();
done:
#else
	if (rzg_iic_dvfs_send(PMIC, BKUP_MODE_CNT, P_ALL_OFF))
		ERROR("BL3-1:Failed the SYSTEM-RESET.\n");
#endif
#endif
#else
	rzg_pwrc_system_reset();
#endif
	wfi();

	ERROR("RZG System Reset: operation not handled.\n");
	panic();
}

static int rzg_validate_power_state(unsigned int power_state,
				    psci_power_state_t *req_state)
{
	unsigned int pwr_lvl = psci_get_pstate_pwrlvl(power_state);
	unsigned int pstate = psci_get_pstate_type(power_state);
	uint32_t i;

	if (pstate == PSTATE_TYPE_STANDBY) {
		if (pwr_lvl != MPIDR_AFFLVL0)
			return PSCI_E_INVALID_PARAMS;

		req_state->pwr_domain_state[MPIDR_AFFLVL0] = PLAT_MAX_RET_STATE;
	} else {
		for (i = MPIDR_AFFLVL0; i <= pwr_lvl; i++)
			req_state->pwr_domain_state[i] = PLAT_MAX_OFF_STATE;
	}

	if (psci_get_pstate_id(power_state))
		return PSCI_E_INVALID_PARAMS;

	return PSCI_E_SUCCESS;
}

#if RZG_SYSTEM_SUSPEND
static void rzg_get_sys_suspend_power_state(psci_power_state_t *req_state)
{
	unsigned long mpidr = read_mpidr_el1() & 0x0000ffffU;
	int i;

	if (mpidr != rzg_boot_mpidr)
		goto deny;

	for (i = MPIDR_AFFLVL0; i <= PLAT_MAX_PWR_LVL; i++)
		req_state->pwr_domain_state[i] = PLAT_MAX_OFF_STATE;

	return;
deny:
	/* deny system suspend entry */
	req_state->pwr_domain_state[PLAT_MAX_PWR_LVL] = PSCI_LOCAL_STATE_RUN;
	for (i = MPIDR_AFFLVL0; i < PLAT_MAX_PWR_LVL; i++)
		req_state->pwr_domain_state[i] = PLAT_MAX_RET_STATE;
}
#endif

static const plat_psci_ops_t rzg_plat_psci_ops = {
	.cpu_standby			= rzg_cpu_standby,
	.pwr_domain_on			= rzg_pwr_domain_on,
	.pwr_domain_off			= rzg_pwr_domain_off,
	.pwr_domain_suspend		= rzg_pwr_domain_suspend,
	.pwr_domain_on_finish		= rzg_pwr_domain_on_finish,
	.pwr_domain_suspend_finish	= rzg_pwr_domain_suspend_finish,
	.system_off			= rzg_system_off,
	.system_reset			= rzg_system_reset,
	.validate_power_state		= rzg_validate_power_state,
#if RZG_SYSTEM_SUSPEND
	.get_sys_suspend_power_state	= rzg_get_sys_suspend_power_state,
#endif
};

int plat_setup_psci_ops(uintptr_t sec_entrypoint, const plat_psci_ops_t **psci_ops)
{
	*psci_ops = &rzg_plat_psci_ops;
	rzg_sec_entrypoint = sec_entrypoint;

#if RZG_SYSTEM_SUSPEND
	rzg_pwrc_init_suspend_to_ram();
#endif

	return 0;
}
