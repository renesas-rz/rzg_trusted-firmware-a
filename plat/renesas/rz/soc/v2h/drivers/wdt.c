/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdint.h>
#include <lib/mmio.h>
#include <common/debug.h>
#include <wdt.h>

/* Initialises the WDT for the */
void wdt_system_reset(void)
{
	mmio_write_16(WDT1_BASE + WDT_CR_OFFSET, 0x3300); /* WDT setup for short period */
	mmio_write_8(WDT1_BASE + WDT_RCR_OFFSET, 0x00); /* Enable WDT1 interrupt output to ICU */
	mmio_write_16(WDT1_BASE + WDT_SR_OFFSET, 0x0000); /* Clear an underflow flag and refresh error flag */
	mmio_write_8(WDT1_BASE + WDT_RR_OFFSET, 0x00); /* Start WDT Part 1 of 2 */
	mmio_write_8(WDT1_BASE + WDT_RR_OFFSET, 0xFF); /* Start WDT Part 2 of 2 */
	console_flush();
}
