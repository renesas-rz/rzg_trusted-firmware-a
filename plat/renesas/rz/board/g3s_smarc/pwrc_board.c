/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <lib/mmio.h>

#include <riic.h>
#include <vbatt_regs.h>

#define PMIC		(0x12)
#define GPAK		(0x38)

#if IMAGE_BL2
bool pwrc_board_is_resume(void)
{
	static bool is_resume;
#if defined(PLAT_SYSTEM_SUSPEND_vbat)
	static bool first_call = true;

	if (first_call) {
		int32_t timeout = 100;

		riic_setup();

		while (timeout-- > 0) {
			uint8_t ctrl_reg;
			uint8_t ists_reg;

			is_resume = false;

			if ((riic_read(GPAK, 0xF4, &ctrl_reg) < 0) || (riic_read(GPAK, 0xF1, &ists_reg) < 0)) {
				panic();
			}

			if ((0x01 & ctrl_reg) == 0) {
				break;
			}

			/* Resume from sleep state */
			if (((0x20 & ists_reg) != 0) && ((0x40 & ists_reg) != 0)) {

				is_resume = true;

				if (riic_write(GPAK, 0xF4, 0x00) < 0) {
					panic();
				}

				break;
			}
		}
	}
	first_call = false;
#elif defined(PLAT_SYSTEM_SUSPEND_awo)
	/* Receive transition to resume from CM33 core */
	if (mmio_read_32(VBATT_BKR0) == 0x000000F0) {
		is_resume = true;
	}

	/* Flag clear */
	mmio_write_32(VBATT_BKR0, 0x00000000);
#else
	is_resume = false;
#endif
	return is_resume;
}
#endif /* IMAGE_BL2 */

void pwrc_board_suspend_on(void)
{
#if defined(PLAT_SYSTEM_SUSPEND_vbat)
	/* VBATT area shut-off control */
	mmio_write_32(VBATT_ISOENPROT, 0x15AFFA51);
	mmio_write_32(VBATT_ISOEN, 0x00000001);

	riic_setup();
	riic_write(GPAK, 0xB0, 0xFF);
	riic_write(GPAK, 0xF4, 0x31);
#elif defined(PLAT_SYSTEM_SUSPEND_awo)
	/* Notifies the CM33 core of a transition to suspend */
	mmio_write_32(VBATT_BKR0, 0x000000A5);
#endif
}
