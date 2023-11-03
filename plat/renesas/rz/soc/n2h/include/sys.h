/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SYS_H__
#define __SYS_H__

#include <stdbool.h>


typedef enum {
	SYS_BOOT_MODE_XSPI0_x1,
	SYS_BOOT_MODE_XSPI0_x8,
	SYS_BOOT_MODE_XSPI1_x1,
	SYS_BOOT_MODE_ESD,
	SYS_BOOT_MODE_EMMC,
	SYS_BOOT_MODE_MAX,
} boot_mode_t;


extern boot_mode_t sys_get_boot_mode(void);
extern bool sys_get_is_3v3(void);
extern void sys_base_unlock(uint32_t unlock_mask);
extern void sys_base_lock(uint32_t lock_mask);
extern void sys_safetybase_unlock(uint32_t unlock_mask);
extern void sys_safetybase_lock(uint32_t lock_mask);
extern uint64_t sys_get_platform_chipid(void);
extern uint32_t sys_get_platform_part_number(void);
extern uint32_t sys_get_platform_core_count(void);
extern bool sys_check_platform_core_count(void);

#endif	/* __SYS_H__ */
