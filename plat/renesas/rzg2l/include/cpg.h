/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _CPG_H_
#define _CPG_H_

extern void cpg_setup(void);
void cpg_active_ddr(void (*disable_phy)(void));

#endif /* _CPG_H_ */
