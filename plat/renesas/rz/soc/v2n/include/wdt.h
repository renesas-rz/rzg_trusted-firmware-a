/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __WDT_H__
#define __WDT_H__

#define WDT0_BASE		(0x11C00400)
#define WDT1_BASE		(0x14400000)
#define WDT2_BASE		(0x13000000)
#define WDT3_BASE		(0x13000400)

#define WDT_RR_OFFSET			(0x00)	/* Refresh Register */
#define WDT_CR_OFFSET			(0x02)	/* Control Register */
#define WDT_SR_OFFSET			(0x04)	/* Status Register */
#define WDT_RCR_OFFSET			(0x06)	/* Reset Control Register */

void wdt_system_reset(void);

#endif /* __WDT_H__ */