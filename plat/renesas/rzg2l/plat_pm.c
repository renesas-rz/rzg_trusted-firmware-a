/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch_helpers.h>
#include <assert.h>
#include <lib/psci/psci.h>
#include <lib/mmio.h>

uintptr_t	gp_warm_ep;

static int rzg2l_pwr_domain_on(u_register_t mpidr)
{
	return PSCI_E_SUCCESS;
}

static void rzg2l_pwr_domain_on_finish(const psci_power_state_t *target_state)
{
}

const plat_psci_ops_t rzg2l_plat_psci_ops = {
	.pwr_domain_on						= rzg2l_pwr_domain_on,
	.pwr_domain_on_finish				= rzg2l_pwr_domain_on_finish,
};

int plat_setup_psci_ops(uintptr_t sec_entrypoint,
			const plat_psci_ops_t **psci_ops)
{
	gp_warm_ep = sec_entrypoint;
	*psci_ops = &rzg2l_plat_psci_ops;

	return 0;
}
