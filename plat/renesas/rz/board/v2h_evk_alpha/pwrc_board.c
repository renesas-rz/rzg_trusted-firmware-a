/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stddef.h>
#include <assert.h>
#include <riic.h>

#define PMIC	(0x12)

bool pwrc_board_is_resume(void)
{
	static bool is_resume;
	static bool first_call = true;

	if (first_call) {
		uint32_t time = 10;
		uint8_t mpio_conf;

		riic_setup();

		while (time--) {
			if ((riic_read(PMIC, 0x8D, &mpio_conf) < 0))
				continue;

			if (!(mpio_conf & 0x01))
				break;

			is_resume = true;

			/* Reset default value */
			if (riic_write(PMIC, 0x8D, 0x22) < 0) {
				ERROR("RZ/V2H: Fail to resume system.\n");
				panic();
			}
		}
	}

	first_call = false;
	return is_resume;
}

void __attribute__ ((section(".sram")))
pwrc_board_suspend_on(void)
{
	riic_setup();

	riic_write(PMIC, 0x8D, 0x1);
	riic_write(PMIC, 0x8A, 0xD);
}
