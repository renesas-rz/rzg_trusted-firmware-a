/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RZT2N_DDRTOP_IECC_H__
#define __RZT2N_DDRTOP_IECC_H__

#include <stdint.h>
#include <inttypes.h>

#include "RZT2N_DDRTOP.h"

#define DDRTOP_ECC_start_addr 0x00000040ULL

/* Prototype */
void DDRTOP_proc_prog0(uint64_t start_addr, uint8_t addr_space, uint8_t run_mode);
void DDRTOP_proc_ecc_init0(const uint64_t range[][2], int num_of_ranges);

#endif /* __RZT2N_DDRTOP_IECC_H__ */
