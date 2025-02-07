/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CPG_H__
#define __CPG_H__

void cpg_early_setup(void);
void cpg_setup(void);
void cpg_active_ddr1(void);
void cpg_active_ddr2(void);
void cpg_prepare_suspend(void);
void cpg_suspend_setup(void);
void cpg_resume_setup(void);
void cpg_m33_setup(void);

#endif /* __CPG_H__ */
