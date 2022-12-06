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

static int rzg3s_pwr_domain_on(u_register_t mpidr)
{
	uint8_t coreid = MPIDR_AFFLVL1_VAL(mpidr);

	if (coreid >= PLATFORM_CORE_COUNT)
		return PSCI_E_INVALID_PARAMS;

	/*  Apply an external reset */
	if ((mmio_read_32(SYS_LP_CTL2) & 0x1) == 0x1) {
		mmio_write_32(CPG_CORE0_PCHCTL, 0x00000001);
		while ((mmio_read_32(CPG_CORE0_PCHMON) & 0x1) != 0x1)
			;
		mmio_write_32(CPG_CORE0_PCHCTL, 0x00000000);
		while ((mmio_read_32(CPG_CORE0_PCHMON) & 0x1) != 0x0)
			;
	}

	/*  Start the core */
	mmio_write_32(SYS_CA55_CFG_RVAL0, (uint32_t)(gp_warm_ep & 0xFFFFFFFC));
	mmio_write_32(SYS_CA55_CFG_RVAH0, (uint32_t)((gp_warm_ep >> 32) & 0xFF));

	/* Assert PORESET */
	mmio_write_32(CPG_RST_CA55, 0x00010000);
	while ((mmio_read_32(CPG_RSTMON_CA55) & 0x1) == 0x0)
		;

	/* Deassert PORESET */
	mmio_write_32(CPG_RST_CA55, 0x00050005);
	while ((mmio_read_32(CPG_RSTMON_CA55) & 0x1) != 0x0)
		;

	mmio_write_32(CPG_CORE0_PCHCTL, 0x00080001);
	while ((mmio_read_32(CPG_CORE0_PCHMON) & 0x1) != 0x1)
		;
	mmio_write_32(CPG_CORE0_PCHCTL, 0x00080000);
	while ((mmio_read_32(CPG_CORE0_PCHMON) & 0x1) != 0x0)
		;

	return PSCI_E_SUCCESS;
}

static void rzg3s_pwr_domain_on_finish(const psci_power_state_t *target_state)
{
#if !DEBUG_FPGA
	plat_gic_pcpu_init();
	plat_gic_cpuif_enable();
#endif /*DEBUG_FPGA */
}

static void rzg3s_pwr_domain_off(const psci_power_state_t *state)
{
	unsigned long mpidr = read_mpidr_el1();
	uint8_t coreid = MPIDR_AFFLVL1_VAL(mpidr);

	if (coreid >= PLATFORM_CORE_COUNT)
		return;

	/* Prevent interrupts from spuriously waking up this cpu */
	plat_gic_cpuif_disable();

	/*  Enable the transition request interrupt to the Cortex-A55 Sleep Mode */
	mmio_write_32(SYS_LP_CTL6, 0x00000100);

	/* Transition request to Cortex-A55 CoreX Sleep Mode */
	mmio_write_32(SYS_LP_CTL1, 0x00000100);

	/* Confirm that the processing on the Cortex-M33 side is completed */
	while ((mmio_read_32(SYS_LP_CTL5) & 0x00000100) != 0x00000100)
		;
	/* Enter the Cortex-A55 Sleep Mode */
	/* Start the Cortex-A55 Sleep Mode */
	mmio_write_32(SYS_LP_CTL2, 0x00000001);

	/* Issue Barrier instruction */
	isb();
	dsb();

	/* A WFI instruction will be executed via lib/psci/psci_off.c->psci_power_down_wfi() */
}

static void __dead2 rzg3s_system_off(void)
{
	wfi();
	ERROR("RZ/G3S System Off: operation not handled.\n");
	panic();
}

const plat_psci_ops_t rzg3s_plat_psci_ops = {
	.pwr_domain_on						= rzg3s_pwr_domain_on,
	.pwr_domain_on_finish				= rzg3s_pwr_domain_on_finish,
	.pwr_domain_off						= rzg3s_pwr_domain_off,
	.system_off							= rzg3s_system_off,
};

int plat_setup_psci_ops(uintptr_t sec_entrypoint,
			const plat_psci_ops_t **psci_ops)
{
	gp_warm_ep = sec_entrypoint;
	*psci_ops = &rzg3s_plat_psci_ops;

	return 0;
}
