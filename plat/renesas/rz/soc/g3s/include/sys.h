/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SYS_H__
#define __SYS_H__

#include <stdbool.h>

typedef enum {
	SYS_BOOT_MODE_ESD = 0,
	SYS_BOOT_MODE_EMMC_1_8,
	SYS_BOOT_MODE_EMMC_3_3,
	SYS_BOOT_MODE_SPI_1_8,
	SYS_BOOT_MODE_SPI_3_3,
	SYS_BOOT_MODE_SCIF,
	SYS_BOOT_MODE_MAX,
} boot_mode_t;

int16_t sys_get_boot_mode(void);

bool sys_is_m33_core_booted(void);
void sys_m33_core_boot_op(uintptr_t vector);

bool sys_is_resume_reboot(void);
bool sys_is_resume_peripheral(void);

#endif	/* __SYS_H__ */
