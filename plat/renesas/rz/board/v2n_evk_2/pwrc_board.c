/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stddef.h>
#include <assert.h>
#include <riic.h>
#include <cdefs.h>

#define PMIC	(0x12)

void __section(".sram")
pwrc_board_suspend_on(void)
{
	riic_setup();

	riic_write(PMIC, 0x6C, 0x80);
}
