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

#include <cpg_regs.h>
#include <sys_regs.h>
#include <rz_private.h>
#include <rz_soc_def.h>
#include <common/bl_common.h>

uintptr_t	gp_warm_ep;

static int rzv2h_pwr_domain_on(u_register_t mpidr)
{

	const uint32_t rval[PLATFORM_CORE_COUNT][2] = {
		{ SYS_ACPU_CFG_RVAL0, SYS_ACPU_CFG_RVAH0 },
		{ SYS_ACPU_CFG_RVAL1, SYS_ACPU_CFG_RVAH1 },
		{ SYS_ACPU_CFG_RVAL2, SYS_ACPU_CFG_RVAH2 },
		{ SYS_ACPU_CFG_RVAL3, SYS_ACPU_CFG_RVAH3 }
	};
#if 0
	const uint32_t pch[2][2] = {
		{ CPG_CORE0_PCHCTL, CPG_CORE0_PCHMON },
		{ CPG_CORE1_PCHCTL, CPG_CORE1_PCHMON }
	};
#endif
	uint8_t coreid = MPIDR_AFFLVL1_VAL(mpidr);

	if (coreid >= PLATFORM_CORE_COUNT)
		return PSCI_E_INVALID_PARAMS;
#if 0


//TODO: KTG: Confirm sequence

	/*  Apply an external reset */
	if ((mmio_read_32(SYS_LP_CTL2) & 0x1) == 0x1) {
		mmio_write_32(pch[coreid][0], 0x00000001);
		while ((mmio_read_32(pch[coreid][1]) & 0x1) != 0x1)
			;
		mmio_write_32(pch[coreid][0], 0x00000000);
		while ((mmio_read_32(pch[coreid][1]) & 0x1) != 0x0)
			;
	}
#endif
	/*  Start the core */
	mmio_write_32(rval[coreid][0], (uint32_t)(gp_warm_ep & 0xFFFFFFFC));
	mmio_write_32(rval[coreid][1], (uint32_t)((gp_warm_ep >> 32) & 0xFF));
#if 0
	/* Assert PORESET */
	mmio_write_32(CPG_RST_CA55, (0x00010000 << coreid));
	while ((mmio_read_32(CPG_RSTMON_CA55) & (0x1 << coreid)) == 0x0)
		;

	/* Deassert PORESET */
	mmio_write_32(CPG_RST_CA55, (0x00050005 << coreid));
	while ((mmio_read_32(CPG_RSTMON_CA55) & (0x1 << coreid)) != 0x0)
		;

	mmio_write_32(pch[coreid][0], 0x00080001);
	while ((mmio_read_32(pch[coreid][1]) & 0x1) != 0x1)
		;
	mmio_write_32(pch[coreid][0], 0x00080000);
	while ((mmio_read_32(pch[coreid][1]) & 0x1) != 0x0)
		;
#endif
	return PSCI_E_SUCCESS;
}

static void rzv2h_pwr_domain_on_finish(const psci_power_state_t *target_state)
{
#if !DEBUG_RZV2H_FPGA
	plat_gic_pcpu_init();
	plat_gic_cpuif_enable();
#endif
}

static void rzv2h_pwr_domain_off(const psci_power_state_t *state)
{
	unsigned long mpidr = read_mpidr_el1();
	uint8_t coreid = MPIDR_AFFLVL1_VAL(mpidr);

	/* Request transition to Cortex-A55 CoreX Sleep Mode */
	mmio_write_32(CPG_LP_CTL1, (CPG_LP_CTL1_CA55SLEEP_REQ << coreid));	
	/* Prevent interrupts from spuriously waking up this cpu */
	plat_gic_cpuif_disable();
	/* A WFI instruction will be executed via lib/psci/psci_off.c->psci_power_down_wfi() */
}

static void __dead2 rzv2h_system_off(void)
{
	wfi();
	ERROR("RZG System Off: operation not handled.\n");
	panic();
}

const plat_psci_ops_t rzv2h_plat_psci_ops = {
	.pwr_domain_on						= rzv2h_pwr_domain_on,
	.pwr_domain_on_finish				= rzv2h_pwr_domain_on_finish,
	.pwr_domain_off						= rzv2h_pwr_domain_off,
	.system_off							= rzv2h_system_off,
};

int plat_setup_psci_ops(uintptr_t sec_entrypoint,
			const plat_psci_ops_t **psci_ops)
{
	gp_warm_ep = sec_entrypoint;
	*psci_ops = &rzv2h_plat_psci_ops;

	return 0;
}
