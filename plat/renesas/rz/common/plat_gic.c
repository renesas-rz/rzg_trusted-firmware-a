/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <platform_def.h>

#include <common/interrupt_props.h>
#include <drivers/arm/gicv3.h>
#include <plat/common/platform.h>

#include <rz_soc_def.h>
#include <rz_private.h>

static uintptr_t plat_rdistif_base_addrs[PLATFORM_CORE_COUNT];

static gicv3_redist_ctx_t rdist_ctx;
static gicv3_dist_ctx_t dist_ctx;

static unsigned int plat_mpidr_to_core_pos(u_register_t mpidr)
{
	mpidr |= (read_mpidr_el1() & MPIDR_MT_MASK);
	return (unsigned int)plat_core_pos_by_mpidr(mpidr);
}

const gicv3_driver_data_t rzg2l_gic_data = {
	.gicd_base = RZ_SOC_GICD_BASE,
	.gicr_base = RZ_SOC_GICR_BASE,
	.rdistif_num = PLATFORM_CORE_COUNT,
	.rdistif_base_addrs = plat_rdistif_base_addrs,
	.mpidr_to_core_pos = plat_mpidr_to_core_pos,
};

void plat_gic_driver_init(void)
{
	gicv3_driver_init(&rzg2l_gic_data);
}

void plat_gic_init(void)
{
	gicv3_distif_init();
	gicv3_rdistif_init(plat_my_core_pos());
	gicv3_cpuif_enable(plat_my_core_pos());
}

void plat_gic_cpuif_enable(void)
{
	gicv3_cpuif_enable(plat_my_core_pos());
}

void plat_gic_cpuif_disable(void)
{
	gicv3_cpuif_disable(plat_my_core_pos());
}

void plat_gic_pcpu_init(void)
{
	gicv3_rdistif_init(plat_my_core_pos());
}

void plat_gic_save(void)
{
	gicv3_rdistif_save(plat_my_core_pos(), &rdist_ctx);

	gicv3_distif_save(&dist_ctx);
}


void plat_gic_resume(void)
{
	gicv3_distif_init_restore(&dist_ctx);

	gicv3_rdistif_init_restore(plat_my_core_pos(), &rdist_ctx);
}
