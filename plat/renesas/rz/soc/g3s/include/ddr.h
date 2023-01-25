/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __DDR_H__
#define __DDR_H__
#include <stdint.h>
#include <stddef.h>

#define	START_LIST_1D	0x000
#define	START_LIST_2D	0x200
#define	START_LIST_CTL	0x300
#define	SCR_SIZE		0x400

extern uint16_t csr_table[SCR_SIZE];

void ddr_setup(void);
void ddr_retention_entry(void);
void ddr_retention_exit(void);

#endif	/* __DDR_H__ */
