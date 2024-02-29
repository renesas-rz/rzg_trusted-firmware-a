/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/desc_image_load.h>
#include <arch_helpers.h>
#include <platform_def.h>
#include <rz_soc_def.h>


bl_load_info_t *plat_get_bl_image_load_info(void)
{
	return get_bl_load_info_from_mem_params_desc();
}

bl_params_t *plat_get_next_bl_params(void)
{
	return get_next_bl_params_from_mem_params_desc();
}

void plat_flush_next_bl_params(void)
{
	/* Ensure this RAM region is flushed before MMU is turned off otherwise this data will no longer be visible */
	flush_dcache_range((uintptr_t)PARAMS_BASE, (size_t)PARAMS_SIZE);
}

