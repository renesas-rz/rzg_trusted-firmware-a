/*
 * Copyright (c) 2026, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CORE_CTRL_H__
#define __CORE_CTRL_H__

#include <stdint.h>

void core_ctrl_warm_boot(uint32_t coreid);
void core_ctrl_set_in_standby(uint32_t coreid);

#endif /* __CORE_CTRL_H__ */
