/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include <lib/mmio.h>
#include <sys.h>
#include <sys_regs.h>
#include <common/debug.h>
#include <pwrc_board.h>

bool sys_is_resume_reboot(void)
{
#if PLAT_SYSTEM_SUSPEND
	return pwrc_board_is_resume();
#else
	return false;
#endif
}

boot_mode_t sys_get_boot_mode(void)
{
	uint8_t boot_dev = mmio_read_32(SYS_LSI_MODE) & SYS_LSI_MODE_MASK;
	boot_mode_t boot_mode;

	switch (boot_dev) {
	case (SYS_LSI_MODE_ESD):
	case (SYS_LSI_MODE_ESD_2):
		boot_mode = SYS_BOOT_MODE_ESD;
		break;
	case (SYS_LSI_MODE_EMMC18):
		boot_mode = SYS_BOOT_MODE_EMMC_1_8;
		break;
	case (SYS_LSI_MODE_EMMC33):
		boot_mode = SYS_BOOT_MODE_EMMC_3_3;
		break;
	case (SYS_LSI_MODE_SFLASH18):
		boot_mode = SYS_BOOT_MODE_SPI_1_8;
		break;
	case (SYS_LSI_MODE_SFLASH33):
		boot_mode = SYS_BOOT_MODE_SPI_3_3;
		break;

	default:
		panic();
	}

	return boot_mode;
}
