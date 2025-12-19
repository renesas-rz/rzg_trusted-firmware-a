/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PWRC_BOARD_H
#define PWRC_BOARD_H

bool pwrc_board_is_resume(void);
void pwrc_clear_resume_flag(void);

#if PLAT_SYSTEM_SUSPEND_vbat
void pwrc_board_suspend_on(void);
#endif

#endif /* PWRC_BOARD_H */
