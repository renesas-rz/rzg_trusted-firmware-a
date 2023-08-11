/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RZ_SOC_PRIVATE_H__
#define __RZ_SOC_PRIVATE_H__

/* bl31_plat_setup.c */
void plat_copy_code_to_system_ram(void);

/* plat_ddr_setup.c */
void plat_ddr_setup(void);

#endif /* __RZ_SOC_PRIVATE_H__ */
