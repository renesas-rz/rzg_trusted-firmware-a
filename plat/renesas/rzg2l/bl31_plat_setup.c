/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <arch_helpers.h>
#include <assert.h>
#include <common/bl_common.h>
#include <lib/xlat_tables/xlat_tables_compat.h>

#include "syc.h"

static const mmap_region_t rzg2l_mmap[] = {
	{0}
};

void bl31_early_platform_setup2(u_register_t arg0,
								u_register_t arg1,
								u_register_t arg2,
								u_register_t arg3)
{
}

void bl31_plat_arch_setup(void)
{
	const mmap_region_t bl31_regions[] = {
		MAP_REGION_FLAT(BL31_START, BL31_END - BL31_START,
						MT_MEMORY | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(BL_CODE_BASE, BL_CODE_END - BL_CODE_BASE,
						MT_CODE | MT_SECURE),
#if USE_COHERENT_MEM
		MAP_REGION_FLAT(BL_COHERENT_RAM_BASE,
						BL_COHERENT_RAM_END - BL_COHERENT_RAM_BASE,
						MT_DEVICE | MT_RW | MT_SECURE),
#endif
		{0}
	};

	setup_page_tables(bl31_regions, rzg2l_mmap);
	enable_mmu_el3(0);
}

void bl31_platform_setup(void)
{
}

entry_point_info_t *bl31_plat_get_next_image_ep_info(uint32_t type)
{
	entry_point_info_t *ep = NULL;

	return ep;
}

unsigned int plat_get_syscnt_freq2(void)
{
	return syc_get_freq();
}
