/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __PFC_REGS_H__
#define __PFC_REGS_H__

#define PORT_P(b, n)						((b) + 0x0000UL  + (n))
#define PORT_PM(b, n)						((b) + 0x0200UL  + (2UL  * ((uint16_t)n)))
#define PORT_PMC(b, n)						((b) + 0x0400UL  + (n))
#define PORT_PFC(b, n)						((b) + 0x0600ULL + (8ULL * ((uint64_t)n)))
#define PORT_DRCTL(b, n)					((b) + 0x0A00ULL + (8ULL * ((uint64_t)n)))

/* Pin current drive control */
#define DRCTL_Em0							(0U)
#define DRCTL_Em0_MSK						(0x3U << DRCTL_Em0)
#define DRCTL_Em0_DRIVE_LO_MSK				(0x0U << DRCTL_Em0)
#define DRCTL_Em0_DRIVE_MI_MSK				(0x1U << DRCTL_Em0)
#define DRCTL_Em0_DRIVE_HI_MSK				(0x2U << DRCTL_Em0)
#define DRCTL_Em0_DRIVE_UHI_MSK				(0x3U << DRCTL_Em0)
/* Pin Schmitt control */
#define DRCTL_SMTm0							(4U)
#define DRCTL_SMTm0_MSK						(0x1U << DRCTL_SMTm0)
/* Pin Slew Rate control */
#define DRCTL_SRm0							(5U)
#define DRCTL_SRm0_MSK						(0x1U << DRCTL_SRm0)


typedef struct {
	uint8_t port;
	uint8_t bit;
	uint8_t function;
	uint8_t drctl;
} PORT_SETTINGS;

#endif	/* __PFC_REGS_H__ */
