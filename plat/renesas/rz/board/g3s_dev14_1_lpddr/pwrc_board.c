/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#include <riic.h>

#define PMIC		(0x12)
#define GPAK		(0x38)

#if IMAGE_BL2
bool pwrc_board_is_resume(void)
{
	static bool is_retention = false;
#if PLAT_SYSTEM_SUSPEND
	static bool first_call = true;

	if (first_call) {

		volatile uint32_t timeout = 100;

		while (0 < timeout--)
		{
			uint8_t ctrl_reg;
			uint8_t ists_reg;

			if ((0 > riic_read(GPAK, 0xF4, &ctrl_reg)) || (0 > riic_read(GPAK, 0xF1, &ists_reg))) {
				panic();
			}

			if (0 == (0x01 & ctrl_reg)) {
				is_retention = false;
				break;
			}

			/* Resume from sleep state */
			if ((0 != (0x20 & ists_reg)) && (0 != (0x40 & ists_reg))) {

				is_retention = true;

				if (0 > riic_write(GPAK, 0xF4, 0x00))
					panic();

				break;
			}
		}

		first_call = false;
	}
#endif
	return is_retention;
}
#endif

void pwrc_board_suspend_on(void)
{
#if defined(PLAT_SYSTEM_SUSPEND_awo)
	/* Switch the sleep mode from VBAT to AWO */
	riic_write(PMIC, 0x35, 0x07);
	riic_write(PMIC, 0x4B, 0x0C);
	riic_write(PMIC, 0x50, 0x0F);
	riic_write(PMIC, 0x80, 0x3E);

	riic_write(GPAK, 0xF4, 0x31);
#elif defined(PLAT_SYSTEM_SUSPEND_vbat)
	riic_write(GPAK, 0xB0, 0xFF);
	riic_write(GPAK, 0xF4, 0x31);
#endif
}

void pwrc_board_setup(void)
{
	riic_setup();
}
