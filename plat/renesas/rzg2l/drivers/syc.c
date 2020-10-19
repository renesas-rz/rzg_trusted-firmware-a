/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <lib/mmio.h>

#define CNTCR		0x000
#define CNTFID0		0x020

#define CNTCR_EN	(1 << 0)

static uintptr_t reg_base;

static void syc_reg_write(uint32_t offset, uint32_t val)
{
	mmio_write_32(reg_base + offset, val);
}

static uint32_t syc_reg_read(uint32_t offset)
{
	return mmio_read_32(reg_base + offset);
}

static void enable_counter(unsigned int enable)
{
	syc_reg_write(CNTCR, enable & CNTCR_EN);
}

void syc_init(uintptr_t base_addr, unsigned int freq)
{
	reg_base = base_addr;
	syc_reg_write(CNTFID0, freq);
	enable_counter(CNTCR_EN);
}

unsigned int syc_get_freq(void)
{
	return syc_reg_read(CNTFID0);
}
