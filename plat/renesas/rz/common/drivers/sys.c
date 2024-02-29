/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include <lib/mmio.h>
#include <sys.h>
#include <sys_regs.h>
#include <common/debug.h>


#define BOOT_MODE_ESD		(0)
#define BOOT_MODE_EMMC_1_8	(1)
#define BOOT_MODE_EMMC_3_3	(2)
#define BOOT_MODE_SPI_1_8	(3)
#define BOOT_MODE_SPI_3_3	(4)
#define BOOT_MODE_SCIF		(5)

#define MASK_BOOTM_DEVICE	(0x0F)
#define MASK_BOOTM_SECURE	(0x10)


boot_mode_t sys_get_boot_mode(void)
{
	uint32_t stat_md_boot = mmio_read_32(SYS_LSI_MODE) & MASK_BOOTM_DEVICE;
	boot_mode_t boot_mode;

	switch (stat_md_boot) {
	case (BOOT_MODE_ESD):
		boot_mode = SYS_BOOT_MODE_ESD;
		break;
	case (BOOT_MODE_EMMC_1_8):
		boot_mode = SYS_BOOT_MODE_EMMC_1_8;
		break;
	case (BOOT_MODE_EMMC_3_3):
		boot_mode = SYS_BOOT_MODE_EMMC_3_3;
		break;
	case (BOOT_MODE_SPI_1_8):
		boot_mode = SYS_BOOT_MODE_SPI_1_8;
		break;
	case (BOOT_MODE_SPI_3_3):
		boot_mode = SYS_BOOT_MODE_SPI_3_3;
		break;

	default:
		panic();
	}

	return boot_mode;
}
