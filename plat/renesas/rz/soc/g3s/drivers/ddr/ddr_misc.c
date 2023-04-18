/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <arch_helpers.h>
#include <common/debug.h>
#include <lib/mmio.h>

#include "ddr_regs.h"
#include "ddr_private.h"

static void soft_delay(uint64_t usec);

#pragma weak decode_major_message
void decode_major_message(uint32_t mail, uint8_t sel_train)
{
	;
}

void wait_pclk(uint32_t cycles)
{
	const uint32_t pclk_freq = 100000000; /* PCLK = 100MHz */

	soft_delay((((uint64_t)cycles * 1000000) / pclk_freq) + 1);
}

void wait_dficlk(uint32_t cycles)
{
	const uint32_t dficlk_freq = 400000000; /* dfiCLK = 400MHz */

	soft_delay((((uint64_t)cycles * 1000000) / dficlk_freq) + 1);
}

void DDRTOP_mc_apb_rmw(uint32_t addr, uint32_t data, uint32_t mask)
{
	uint32_t tmp_data;

	tmp_data = DDRTOP_mc_apb_rd(addr);
	data = (data & mask) | (tmp_data & (~mask));

	DDRTOP_mc_apb_wr(addr, data);
}

void DDRTOP_mc_apb_poll(uint32_t addr, uint32_t data, uint32_t mask)
{
	uint32_t tmp_data;

	tmp_data = DDRTOP_mc_apb_rd(addr);
	tmp_data &= mask;

	while (tmp_data != data) {
		wait_pclk(10);
		tmp_data = DDRTOP_mc_apb_rd(addr);
		tmp_data &= mask;
	}
}

void DDRTOP_mc_param_wr(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data)
{
	uint32_t tmp_data;
	uint32_t tmp_mask;

	tmp_data = data << offset;
	tmp_mask = ((1 << width) - 1) << offset;

	DDRTOP_mc_apb_rmw(addr, tmp_data, tmp_mask);
}

uint32_t DDRTOP_mc_param_rd(uint32_t addr, uint32_t offset, uint32_t width)
{
	uint32_t tmp_data;
	uint32_t tmp_mask;

	tmp_data = DDRTOP_mc_apb_rd(addr);
	tmp_mask = ((1 << width) - 1) << offset;

	return (tmp_data & tmp_mask) >> offset;
}

void DDRTOP_mc_param_poll(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data)
{
	uint32_t tmp_data;
	uint32_t tmp_mask;

	tmp_data = data << offset;
	tmp_mask = ((1 << width) - 1) << offset;

	DDRTOP_mc_apb_poll(addr, tmp_data, tmp_mask);
}

void dwc_ddrphy_apb_poll(uint32_t addr, uint32_t data, uint32_t mask)
{
	uint32_t tmp_data;

	tmp_data = dwc_ddrphy_apb_rd(addr);
	tmp_data &= mask;

	while (tmp_data != data) {
		wait_pclk(10);
		tmp_data = dwc_ddrphy_apb_rd(addr);
		tmp_data &= mask;
	}
}

void dwc_ddrphy_phyinit_userCustom_G_waitDone(uint8_t sel_train)
{
	uint32_t mail;

	wait_dficlk(10);

	do {
		/* Wait at least 500 cycles */
		wait_pclk(500);

		mail = get_mail(0);

		decode_major_message(mail, sel_train);

	} while ((mail != 0xff) && (mail != 0x07));

	if (mail == 0xff) {
		ERROR("Training failed.\n");
		panic();
	}
}

uint32_t get_mail(uint8_t mode_32bits)
{
	uint32_t mail = 0;
	uint32_t wd_timer = 0;

	while (0 != (dwc_ddrphy_apb_rd(0x0006E004) & 0x1))
		;

	mail = dwc_ddrphy_apb_rd(0x06E032);

	if (mode_32bits != 0) {
		mail = (dwc_ddrphy_apb_rd(0x0006E034) << 16) | mail;
	}

	dwc_ddrphy_apb_wr(0x06E031, 0x00000000);

	while (0 == (dwc_ddrphy_apb_rd(0x0006E004) & 0x1)) {
		if (wd_timer++ > 1000) {
			ERROR("Watchdog timer overflow.\n");
			panic();
		}
	}

	dwc_ddrphy_apb_wr(0x0006E031, 0x00000001);

	return mail;
}

static void soft_delay(uint64_t usec)
{
	/* RZ/G3S: CPU Clock = 1.2G Hz*/
	const uint32_t cpuclk_freq = 1200000000;
	/* If the number of nop clock cycles is 4 */
	const uint32_t nop_clk_cycles = 4;
	/* Number of NOP instructions required for 1us */
	const uint32_t num_of_nop_needed = cpuclk_freq / (nop_clk_cycles * 1000000);

	volatile uint64_t timeout = num_of_nop_needed * usec;

	while (timeout--) {
		__asm__ ("nop");
		dsb();
	}
}
