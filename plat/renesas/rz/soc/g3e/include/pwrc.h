/*
 * Copyright (c) 2024, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PWRC_H
#define PWRC_H

void pwrc_suspend_to_ram(void);
void plat_secondary_reset(void);
void pwrc_setup(void);

#endif /* PWRC_H */
