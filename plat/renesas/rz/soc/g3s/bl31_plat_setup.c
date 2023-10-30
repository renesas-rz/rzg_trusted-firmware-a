/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <arch_helpers.h>
#include <assert.h>
#include <common/bl_common.h>
#include <lib/xlat_tables/xlat_tables_compat.h>
#include <plat/common/common_def.h>

#include <pwrc.h>
#include <riic.h>
#include <scifa.h>
#include <rz_private.h>
#include <rz_soc_def.h>

#ifdef PLAT_EXTRA_LD_SCRIPT
IMPORT_SYM(uintptr_t, __BL31_PMUSRAM_START__, BL31_PMUSRAM_START);
IMPORT_SYM(uintptr_t, __BL31_PMUSRAM_END__, BL31_PMUSRAM_END);
IMPORT_SYM(uintptr_t, __BL31_PMUSRAM_BASE__, BL31_PMUSRAM_BASE);
#endif

static console_t rzg3s_bl31_console;
static bl2_to_bl31_params_mem_t from_bl2;

void plat_copy_code_to_system_ram(void)
{
#ifdef PLAT_EXTRA_LD_SCRIPT
	uint32_t attr;
	const uintptr_t pmu_code_load = BL31_PMUSRAM_BASE;
	const uintptr_t pmu_code_image = BL31_PMUSRAM_START;
	size_t pmu_code_size = BL31_PMUSRAM_END - BL31_PMUSRAM_START;

	attr = MT_MEMORY | MT_RW | MT_SECURE | MT_EXECUTE_NEVER;
	xlat_change_mem_attributes(pmu_code_image, pmu_code_size, attr);

	memcpy((void *)pmu_code_image, (void *)pmu_code_load, pmu_code_size);
	flush_dcache_range(pmu_code_image, pmu_code_size);

	attr = MT_MEMORY | MT_RO | MT_SECURE | MT_EXECUTE;
	xlat_change_mem_attributes(pmu_code_image, pmu_code_size, attr);

	/* Invalidate instruction cache */
	plat_invalidate_icache();
	dsb();
	isb();
#endif
}

void bl31_early_platform_setup2(u_register_t arg0,
								u_register_t arg1,
								u_register_t arg2,
								u_register_t arg3)
{
	int ret;

	/* initialize console driver */
	ret = console_rz_register(
							RZG3S_SCIF_0_BASE,
							RZG3S_UART_INCK_HZ,
							RZG3S_UART_BARDRATE,
							&rzg3s_bl31_console);
	if (!ret)
		panic();

	console_set_scope(&rzg3s_bl31_console,
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

	const mmap_region_t rzg3s_mmap[] = {
		MAP_REGION_FLAT(RZG3S_SRAM_BASE, RZG3S_SRAM_SIZE,
				MT_MEMORY | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZG3S_DEVICE_BASE, RZG3S_DEVICE_SIZE,
				MT_DEVICE | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZG3S_DDR0_BASE, RZG3S_DDR0_SIZE,
				MT_MEMORY | MT_RW | MT_SECURE),
		{0}
	};

	setup_page_tables(bl31_regions, rzg3s_mmap);
	enable_mmu_el3(0);
}

void bl31_platform_setup(void)
{
	/* initialize GIC-600 */
	plat_gic_driver_init();
	plat_gic_init();

	plat_copy_code_to_system_ram();
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
