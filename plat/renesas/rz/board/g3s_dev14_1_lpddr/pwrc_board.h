/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PWRC_BOARD_H
#define PWRC_BOARD_H

bool pwrc_board_is_resume(void);
void pwrc_board_sleep_on(void);
void pwrc_board_sleep_off(void);
void pwrc_board_setup(void);

#endif /* PWRC_BOARD_H */
