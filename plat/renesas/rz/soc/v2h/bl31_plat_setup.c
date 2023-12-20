/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <arch_helpers.h>
#include <assert.h>
#include <common/bl_common.h>
#include <lib/xlat_tables/xlat_tables_compat.h>
#include <plat/common/common_def.h>

#include <scifa.h>
#include <plat_tzc_def.h>
#include <rz_private.h>
#include <rz_soc_def.h>
#include <pwrc.h>

static console_t rzv2h_bl31_console;
static bl2_to_bl31_params_mem_t from_bl2;

#ifdef PLAT_EXTRA_LD_SCRIPT
IMPORT_SYM(uintptr_t, __BL31_PMUSRAM_START__, BL31_PMUSRAM_START);
IMPORT_SYM(uintptr_t, __BL31_PMUSRAM_END__, BL31_PMUSRAM_END);
IMPORT_SYM(uintptr_t, __BL31_PMUSRAM_BASE__, BL31_PMUSRAM_BASE);
#endif

void plat_copy_code_to_system_ram(void)
{
#if (PLAT_EXTRA_LD_SCRIPT && PLAT_SYSTEM_SUSPEND)
	int ret __attribute__ ((unused));
	uint32_t attr;
	const uintptr_t pmu_code_load = BL31_PMUSRAM_BASE;
	const uintptr_t pmu_code_image = BL31_PMUSRAM_START;
	size_t pmu_code_size = BL31_PMUSRAM_END - BL31_PMUSRAM_START;

	attr = MT_MEMORY | MT_RW | MT_SECURE | MT_EXECUTE_NEVER;
	ret = xlat_change_mem_attributes(pmu_code_image, pmu_code_size, attr);
	assert(ret == 0);

	memcpy((void *)pmu_code_image, (void *)pmu_code_load, pmu_code_size);
	flush_dcache_range(pmu_code_image, pmu_code_size);

	attr = MT_MEMORY | MT_RO | MT_SECURE | MT_EXECUTE;
	ret = xlat_change_mem_attributes(pmu_code_image, pmu_code_size, attr);
	assert(ret == 0);

	/* Invalidate instruction cache */
	plat_invalidate_icache();
	dsb();
	isb();
#endif /* PLAT_EXTRA_LD_SCRIPT && PLAT_SYSTEM_SUSPEND */
}

void bl31_early_platform_setup2(u_register_t arg0,
								u_register_t arg1,
								u_register_t arg2,
								u_register_t arg3)
{
	int ret;

	/* initialize console driver */
	ret = console_rz_register(
							RZV2H_SCIF_BASE,
							RZV2H_UART_INCK_HZ,
							RZV2H_UART_BARDRATE,
							&rzv2h_bl31_console);
	if (!ret)
		panic();

	console_set_scope(&rzv2h_bl31_console,
			CONSOLE_FLAG_BOOT | CONSOLE_FLAG_RUNTIME | CONSOLE_FLAG_CRASH);

	/* copy bl2_to_bl31_params_mem_t*/
	memcpy(&from_bl2, (void *)PARAMS_BASE, sizeof(from_bl2));
}

void bl31_plat_arch_setup(void)
{
	const mmap_region_t bl31_regions[] = {
		MAP_REGION_FLAT(BL31_START, BL31_END - BL31_START,
						MT_MEMORY | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(BL_CODE_BASE, BL_CODE_END - BL_CODE_BASE,
						MT_CODE | MT_SECURE),
		MAP_REGION_FLAT(BL_RO_DATA_BASE, BL_RO_DATA_END - BL_RO_DATA_BASE,
						MT_RO_DATA | MT_SECURE),
		{0}
	};

	const mmap_region_t rzv2h_mmap[] = {
		MAP_REGION_FLAT(RZV2H_SRAM_BASE, RZV2H_SRAM_SIZE,
				MT_MEMORY | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZV2H_DEVICE_BASE, RZV2H_DEVICE_SIZE,
				MT_DEVICE | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZV2H_DDR0_BASE, RZV2H_DDR0_SIZE,
				MT_MEMORY | MT_RW | MT_SECURE),
		{0}
	};

	setup_page_tables(bl31_regions, rzv2h_mmap);
	enable_mmu_el3(0);
	plat_copy_code_to_system_ram();
}

void bl31_platform_setup(void)
{
	/* initialize GIC-600 */
	plat_gic_driver_init();
	plat_gic_init();

	pwrc_setup();
}

entry_point_info_t *bl31_plat_get_next_image_ep_info(uint32_t type)
{
	entry_point_info_t *next_image_info = NULL;

	next_image_info = (type == NON_SECURE)
			? &from_bl2.bl33_ep_info : &from_bl2.bl32_ep_info;

	if (next_image_info->pc)
		return next_image_info;
	else
		return NULL;
}
