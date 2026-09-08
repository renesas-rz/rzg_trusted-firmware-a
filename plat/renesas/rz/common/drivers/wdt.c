/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdint.h>
#include <lib/mmio.h>
#include <common/debug.h>
#include <wdt.h>

/* Use the WDT to perform a system reset */
void wdt_system_reset(void)
{
	mmio_write_32(WDT0_BASE + WDT_PEEN_OFFSET, 0x00000001);
}
