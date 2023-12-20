/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CPG_H__
#define __CPG_H__

void cpg_early_setup(void);
void cpg_setup(void);
void cpg_ddr0_part1(void);
void cpg_ddr0_part2(void);
void cpg_ddr1_part1(void);
void cpg_ddr1_part2(void);
void cpg_prepare_suspend(void);
void cpg_ddr_pwrokin_off(uint8_t base);

#endif /* __CPG_H__ */
