/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <sys.h>
#include <sys_regs.h>
#include <cpg.h>
#include <cpg_regs.h>
#include <lib/mmio.h>

int16_t sys_get_boot_mode(void)
{
	int16_t boot_mode = 0;
	uint8_t boot_dev = (mmio_read_32(SYS_LSI_MODE) >> SYS_LSI_MODE_SHIFT) & SYS_LSI_MODE_MASK;

	switch(boot_dev)
	{
		case (SYS_LSI_MODE_VOL_1_8 | SYS_LSI_MODE_ESD):
		case (SYS_LSI_MODE_VOL_3_3 | SYS_LSI_MODE_ESD):
			boot_mode = SYS_BOOT_MODE_ESD;
			break;
		case (SYS_LSI_MODE_VOL_1_8 | SYS_LSI_MODE_EMMC):
			boot_mode = SYS_BOOT_MODE_EMMC_1_8;
			break;
		case (SYS_LSI_MODE_VOL_3_3 | SYS_LSI_MODE_EMMC):
			boot_mode = SYS_BOOT_MODE_EMMC_3_3;
			break;
		case (SYS_LSI_MODE_VOL_1_8 | SYS_LSI_MODE_SFLASH):
			boot_mode = SYS_BOOT_MODE_SPI_1_8;
			break;
		case (SYS_LSI_MODE_VOL_3_3 | SYS_LSI_MODE_SFLASH):
			boot_mode = SYS_BOOT_MODE_SPI_3_3;
			break;
		case (SYS_LSI_MODE_VOL_1_8 | SYS_LSI_MODE_SCIF):
		case (SYS_LSI_MODE_VOL_3_3 | SYS_LSI_MODE_SCIF):
			boot_mode = SYS_BOOT_MODE_SCIF;
			break;
		default:
			panic();
	}

	return boot_mode;
}

void sys_boot_subcore(uintptr_t vector)
{
	if (0 != (mmio_read_32(CPG_PLL6_STBY) & PLL6_STBY_SSC_EN))
	{
		mmio_write_32(SYS_CM33_CFG0, 0x01012E1E);
		mmio_write_32(SYS_CM33_CFG1, 0x01012E1E);
	}
	else
	{
		mmio_write_32(SYS_CM33_CFG0, 0x0001312C);
		mmio_write_32(SYS_CM33_CFG1, 0x0001312C);
	}

	mmio_write_32(SYS_CM33_CFG2, vector);

	cpg_subcore_setup();
}

bool sys_is_subcore_booted(void)
{
	bool is_booted = false;
	// TODO
	return is_booted;
}

bool sys_is_peri_suspended(void)
{
	return sys_is_subcore_booted();
}
