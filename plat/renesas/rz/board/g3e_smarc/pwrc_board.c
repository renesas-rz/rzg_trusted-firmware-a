/*
 * Copyright (c) 2024, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <lib/mmio.h>
#include <common/debug.h>
#include <riic.h>

#define GPAK				(0x38)
#define GPAK_RET_STAT_REG	(0xF1)
#define GPAK_RET_CTRL_REG	(0x49)

#if IMAGE_BL2
bool pwrc_board_is_resume(void)
{
	static bool is_resume;
#if defined(PLAT_SYSTEM_SUSPEND)
	int32_t timeout = 100;
	static bool is_first_call = true;

	if (is_first_call) {
		is_first_call = false;
	} else {
		return is_resume;
	}

	riic_setup();

	while (timeout-- > 0) {

		uint8_t retention_status;

		if ((riic_read(GPAK, GPAK_RET_STAT_REG, &retention_status) < 0)) {
			panic();
		}

		if (0x3F == retention_status) {
			is_resume = true;
		} else if (0x1F == retention_status && !is_resume) {
			INFO("Not resuming from reset\n");
			is_resume = false;
			break;
		} else if (0x1F == retention_status && is_resume) {
			/* retention status register has been reset */
			INFO("Resuming from reset\n");
			INFO("Retention status successfully reset\n");
			break;
		}

		/* reset the retention status */
		if (is_resume) {
			if (riic_write(GPAK, GPAK_RET_CTRL_REG, 0x00) < 0) {
				panic();
			}
			if (riic_write(GPAK, GPAK_RET_CTRL_REG, 0x18) < 0) {
				panic();
			}
		}
	}

	if (timeout <= 0) {
		WARN("Retention status not found!");
	}

#else
	is_resume = false;
#endif
	return is_resume;
}
#endif /* IMAGE_BL2 */

void pwrc_board_suspend_on(void)
{
#if defined(PLAT_SYSTEM_SUSPEND)
	riic_setup();
	riic_write(GPAK, 0xF4, 0x0E);
	riic_write(GPAK, 0xF4, 0x0F);
#endif
}

