/*
 * Copyright (c) 2024, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CPG_H__
#define __CPG_H__

void cpg_early_setup(void);
void cpg_setup(void);
void cpg_ddr_part1(void);
void cpg_ddr_part2(void);
void cpg_ddr_set_pwrokin_off(void);
void cpg_reset_wdt1(void);
void cpg_setup_wdt1(void);
#if PLAT_M33_BOOT_SUPPORT
void cpg_cm33_setup(void);
bool cpg_is_m33_core_booted(void);
#endif /* PLAT_M33_BOOT_SUPPORT */

#if PLAT_SYSTEM_SUSPEND
void cpg_resume_setup(void);
void cpg_suspend_setup(void);
#endif /* PLAT_SYSTEM_SUSPEND */

#endif /* __CPG_H__ */
