/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RZ_PRIVATE_H__
#define __RZ_PRIVATE_H__

#include <common/bl_common.h>
#include <platform_def.h>

/* plat_helper.S */
void plat_invalidate_icache(void);

/* plat_gic.c */
void plat_gic_driver_init(void);
void plat_gic_init(void);
void plat_gic_cpuif_enable(void);
void plat_gic_cpuif_disable(void);
void plat_gic_pcpu_init(void);
void plat_gic_save(void);
void plat_gic_resume(void);

/* plat_security.c */
void plat_tzc400_setup(uintptr_t tzc_base);

/* plat_storage.c */
void rz_io_setup(void);

/* plat_ddr_setup.c  */
void plat_ddr_setup(void);

/* bl31_plat_setup.c */
void plat_copy_code_to_system_ram(void);

typedef enum boot_kind {
	RZ_COLD_BOOT,
	RZ_WARM_BOOT
} boot_kind_t;

typedef struct bl2_to_bl31_params_mem {
	boot_kind_t boot_kind;
	entry_point_info_t bl32_ep_info;
	entry_point_info_t bl33_ep_info;
} bl2_to_bl31_params_mem_t;

#endif	/* __RZ_PRIVATE_H__ */
