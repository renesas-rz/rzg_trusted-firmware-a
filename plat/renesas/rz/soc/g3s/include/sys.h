/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SYS_H__
#define __SYS_H__

#define MASK_BOOTM_DEVICE		(0x0007)
#define MASK_BOOTM_SECURE		(0x8000)

#define BOOT_MODE_ESD			(0)
#define BOOT_MODE_EMMC_3_3		(1)
#define BOOT_MODE_SPI_3_3		(2)
#define BOOT_MODE_SCIF			(3)
/* Not Used						(4) */
#define BOOT_MODE_EMMC_1_8		(5)
#define BOOT_MODE_SPI_1_8		(6)
/* Not Used						(7) */

#endif	/* __SYS_H__ */
