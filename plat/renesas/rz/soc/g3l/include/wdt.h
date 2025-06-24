/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __WDT_H__
#define __WDT_H__

#define WDT0_BASE		(0x12800800)
#define WDT1_BASE		(0x12800400)
#define WDT2_BASE		(0x12800000)

#define WDT_CNT_OFFSET			(0x00)	/* Control Register */
#define WDT_SET_OFFSET			(0x04)	/* Period Setting Register */
#define WDT_TIM_OFFSET			(0x08)	/* Elapsed Time Register */
#define WDT_INT_OFFSET			(0x0C)	/* Interrupt Control Register */
#define WDT_PECR_OFFSET			(0x10)	/* Parity Error Control Register */
#define WDT_PEEN_OFFSET			(0x14)	/* Parity Error Forced Enable Register */
#define WDT_PESR_OFFSET			(0x18)	/* Parity Error Status Register */
#define WDT_PEER_OFFSET			(0x1C)	/* Parity Error Enable Register */
#define WDT_PEPO_OFFSET			(0x20)	/* Parity Error Polarity Setting Register */

void wdt_system_reset(void);

#endif /* __WDT_H__ */
