/*
 * Copyright (c) 2024, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PWRC_H
#define PWRC_H

void pwrc_setup(void);

void plat_secondary_reset(void);

#if PLAT_SYSTEM_SUSPEND
void pwrc_suspend_to_ram(void);
#endif /* PLAT_SYSTEM_SUSPEND */

#endif /* PWRC_H */
