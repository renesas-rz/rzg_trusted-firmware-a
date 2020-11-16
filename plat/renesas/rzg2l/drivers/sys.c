/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <lib/mmio.h>
#include "sys.h"
#include "sys_regs.h"

uint32_t sys_get_mode_mr(void)
{
	uint32_t	reg;

	reg = mmio_read_32(SYS_LSI_MODE);
	return (reg & MODEMR_BOOT_DEV_MASK);
}
