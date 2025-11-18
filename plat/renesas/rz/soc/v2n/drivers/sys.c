/*
 * Copyright (c) 2024, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include <lib/mmio.h>
#include <stdbool.h>
#include <common/debug.h>
#include <cpg.h>
#include <sys.h>
#include <sys_regs.h>
#include <platform_def.h>

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

#if PLAT_M33_BOOT_SUPPORT
void sys_m33_core_boot_op(void)
{
	mmio_write_32(SYS_MCPU_CFG2, BL22_S_VECTOR);
	mmio_write_32(SYS_MCPU_CFG3, BL22_NS_VECTOR);
	cpg_cm33_setup();
}
#endif /* PLAT_M33_BOOT_SUPPORT */

bool sys_is_resume(void)
{
#if PLAT_SYSTEM_SUSPEND
	return mmio_read_8(RESUME_MAILBOX_BASE) != 0;
#else
	return false;
#endif /* PLAT_SYSTEM_SUSPEND */
}

