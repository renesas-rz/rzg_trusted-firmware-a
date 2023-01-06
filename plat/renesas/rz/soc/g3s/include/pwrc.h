/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PWRC_H
#define PWRC_H

bool pwrc_is_ddr_retention_mode(void);
void pwrc_suspend_to_ram(void);
void pwrc_setup(void);

#endif /* PWRC_H */
