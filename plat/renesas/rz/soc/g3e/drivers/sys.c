/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include <lib/mmio.h>
#include <sys.h>
#include <sys_regs.h>
#include <common/debug.h>

/* MD_BOOT [2:0] */
#define SYS_LSI_MODE_ESD					(0)
#define SYS_LSI_MODE_EMMC_3V3				(1)
#define SYS_LSI_MODE_SFLASH_3V3				(2)
#define SYS_LSI_MODE_SCIF					(3)
#define SYS_LSI_MODE_USB					(4)		/* Same as SYS_LSI_MODE_ESD */
#define SYS_LSI_MODE_EMMC_1V8				(5)
#define SYS_LSI_MODE_SFLASH_1V8				(6)
#define SYS_LSI_MODE_MASK					(0x7)

boot_mode_t sys_get_boot_mode(void)
{
	uint8_t boot_dev = mmio_read_32(SYS_LSI_MODE) & SYS_LSI_MODE_MASK;
	boot_mode_t boot_mode;

	switch (boot_dev) {
	case (SYS_LSI_MODE_ESD):
		boot_mode = SYS_BOOT_MODE_ESD;
		break;
	case (SYS_LSI_MODE_EMMC_3V3):
		boot_mode = SYS_BOOT_MODE_EMMC_3_3;
		break;
	case (SYS_LSI_MODE_SFLASH_3V3):
		boot_mode = SYS_BOOT_MODE_SPI_3_3;
		break;
	case (SYS_LSI_MODE_SCIF):
		boot_mode = SYS_BOOT_MODE_SCIF;
		break;
	case (SYS_LSI_MODE_USB):
		boot_mode = SYS_BOOT_MODE_USB;
		break;
	case (SYS_LSI_MODE_EMMC_1V8):
		boot_mode = SYS_BOOT_MODE_EMMC_1_8;
		break;
	case (SYS_LSI_MODE_SFLASH_1V8):
		boot_mode = SYS_BOOT_MODE_SPI_1_8;
		break;
	default:
		panic();
	}

	return boot_mode;
}
