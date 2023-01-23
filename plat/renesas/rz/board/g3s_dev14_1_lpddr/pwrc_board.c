/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#include <riic.h>


#if IMAGE_BL2
bool pwrc_board_is_resume(void)
{
	static bool is_retention = false;
#if PLAT_SYSTEM_SUSPEND
	static bool first_call = true;

	if (first_call) {

		uint8_t ctrl_reg;

	if (0 > riic_read(0x38, 0xF4, &ctrl_reg))
		panic();

		if (0 == (0x01 & ctrl_reg))
			is_retention = true;
		else
			is_retention = false;

		first_call = false;
	}
#endif
	return is_retention;
}
#endif

void pwrc_board_sleep_on(void)
{
	riic_write(0x38, 0xB0, 0xFF);
	riic_write(0x38, 0xF4, 0x31);
}

void pwrc_board_sleep_off(void)
{
	uint32_t timeout = 100;

	while (0 < timeout--)
	{
		uint8_t ctrl_reg;
		uint8_t ists_reg;

		if ((0 > riic_read(0x38, 0xF4, &ctrl_reg)) || (0 > riic_read(0x38, 0xF1, &ists_reg))) {
			panic();
		}

		if (0 == (0x01 & ctrl_reg))
			panic();
			
		if ((0 == (0x20 & ists_reg)) || (0 == (0x40 & ists_reg)))
			continue;
		
		if (0 > riic_write(0x38, 0xF4, 0x00))
			panic();

		return;
	}
	panic();
}

void pwrc_board_setup(void)
{
	riic_setup();

#if IMAGE_BL2
	(void)pwrc_board_is_resume();
#endif
}
