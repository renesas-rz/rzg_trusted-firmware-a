/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SYS_H__
#define __SYS_H__

#define MODEMR_BOOT_DEV_MASK    (7)
#define BOOT_MODE_ESD           (0)
#define BOOT_MODE_EMMC_1_8      (1)
#define BOOT_MODE_EMMC_3_3      (2)
#define BOOT_MODE_SPI_1_8       (3)
#define BOOT_MODE_SPI_3_3       (4)
#define BOOT_MODE_SCIF          (5)

uint32_t sys_get_mode_mr(void);

#endif	/* __SYS_H__ */
