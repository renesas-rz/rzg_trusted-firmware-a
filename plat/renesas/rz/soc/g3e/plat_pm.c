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

#include <cpg_regs.h>
#include <sys_regs.h>
#include <rz_private.h>
#include <rz_soc_def.h>
#include <common/bl_common.h>


#define LO_REG							(0U)
#define HI_REG							(1U)


typedef struct {
	uintptr_t reg;
	uint32_t  preq_mask;
	uint32_t  paccept_mask;
	uint32_t  pstate_on_mask;
} CPG_CORE_PWR;


uintptr_t	gp_warm_ep;


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

	/* Confirm that the processing on the Cortex-M33 side is completed */
	while ((mmio_read_32(CPG_LP_CTL1) & (CPG_LP_CTL1_CA55SLEEP_ACK << coreid)) != (CPG_LP_CTL1_CA55SLEEP_ACK << coreid))
		;

	/* Issue Barrier instruction */
	isb();
	dsb();

	/* A WFI instruction will be executed via lib/psci/psci_off.c->psci_power_down_wfi() */
}

static void __dead2 rzg3e_system_off(void)
{
	wfi();
	ERROR("RZ/G3E System Off: operation not handled.\n");
	panic();
}

const plat_psci_ops_t rzg3e_plat_psci_ops = {
	.pwr_domain_on						= rzg3e_pwr_domain_on,
	.pwr_domain_on_finish				= rzg3e_pwr_domain_on_finish,
	.pwr_domain_off						= rzg3e_pwr_domain_off,
	.system_off							= rzg3e_system_off,
};

int plat_setup_psci_ops(uintptr_t sec_entrypoint,
			const plat_psci_ops_t **psci_ops)
{
	gp_warm_ep = sec_entrypoint;
	*psci_ops = &rzg3e_plat_psci_ops;

	return 0;
}
