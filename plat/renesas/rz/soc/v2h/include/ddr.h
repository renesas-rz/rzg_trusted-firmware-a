/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __PLAT_DDR_H__
#define __PLAT_DDR_H__

#define RET_CSR_SIZE		(0x400)
extern uint32_t ddr_csr_table[RET_CSR_SIZE];

void ddr_setup(void);
void ddr_retention_entry(void);
void ddr_retention_exit(uint8_t base);

#endif	/* __PLAT_DDR_H__ */
