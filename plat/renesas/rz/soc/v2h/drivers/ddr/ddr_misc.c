/*
 * Copyright (c) 2024, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <arch_helpers.h>
#include <common/debug.h>
#include <lib/mmio.h>
#include <drivers/delay_timer.h>

#include "ddr_regs.h"
#include "ddr_private.h"


static uint32_t m_mcbase;
static uint32_t m_phybase;


static void decode_streaming_message(void);


void ddrtop_mc_apb_rmw(uint32_t addr, uint32_t data, uint32_t mask)
{
	uint32_t tmp_data;

	tmp_data = ddrtop_mc_apb_rd(addr);
	data = (data & mask) | (tmp_data & (~mask));

	ddrtop_mc_apb_wr(addr, data);
}

void ddrtop_mc_apb_poll(uint32_t addr, uint32_t data, uint32_t mask)
{
	uint32_t tmp_data;

	tmp_data = ddrtop_mc_apb_rd(addr);
	tmp_data &= mask;

	while (tmp_data != data) {
		udelay(1);
		tmp_data = ddrtop_mc_apb_rd(addr);
		tmp_data &= mask;
	}
}

void ddrtop_mc_param_wr(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data)
{
	uint32_t tmp_data;
	uint32_t tmp_mask;

	tmp_data = data << offset;
	tmp_mask = ((1 << width) - 1) << offset;

	ddrtop_mc_apb_rmw(addr, tmp_data, tmp_mask);
}

uint32_t ddrtop_mc_param_rd(uint32_t addr, uint32_t offset, uint32_t width)
{
	uint32_t tmp_data;
	uint32_t tmp_mask;

	tmp_data = ddrtop_mc_apb_rd(addr);
	tmp_mask = ((1 << width) - 1) << offset;

	return (tmp_data & tmp_mask) >> offset;
}

void ddrtop_mc_param_poll(uint32_t addr, uint32_t offset, uint32_t width, uint32_t data)
{
	uint32_t tmp_data;
	uint32_t tmp_mask;

	tmp_data = data << offset;
	tmp_mask = ((1 << width) - 1) << offset;

	ddrtop_mc_apb_poll(addr, tmp_data, tmp_mask);
}

void dwc_ddrphy_phyinit_userCustom_G_waitDone(uint8_t sel_train)
{
	uint32_t train_done = 0;
	uint32_t mail;

	do {
		/* Wait at least 500 cycles */
		uint32_t data = dwc_ddrphy_apb_rd(0x0d0004);

		if ((data & 0x1) == 0) {
			mail = get_mail(0);
			if (mail == 0xff || mail == 0x07) {
				train_done = 1;
			} else if (mail == 0x08) {
				decode_streaming_message();
			}
		}
	} while (train_done == 0);

	if (mail == 0xff) {
		ERROR("Training failed.\n");
		panic();
	}
}

uint32_t get_mail(uint8_t mode_32bits)
{
	uint32_t mail = 0;
	uint32_t wd_timer = 0;

	while ((dwc_ddrphy_apb_rd(0x0d0004) & 0x1) != 0)
		;

	mail = dwc_ddrphy_apb_rd(0x0d0032);

	if (mode_32bits != 0) {
		mail = (dwc_ddrphy_apb_rd(0x0d0034) << 16) | mail;
	}

	dwc_ddrphy_apb_wr(0x0d0031, 0x0000);

	while ((dwc_ddrphy_apb_rd(0x0d0004) & 0x1) == 0) {
		if (wd_timer++ > 1000) {
			ERROR("Watchdog timer overflow\n");
			panic();
		}
	}
	dwc_ddrphy_apb_wr(0x0d0031, 0x0001);

	VERBOSE("mail = %x\n", mail);
	return mail;
}

void set_ddrtop_mc_base_addr(uint32_t base_addr)
{
	m_mcbase = base_addr;
}

uint32_t get_ddrtop_mc_base_addr(void)
{
	return m_mcbase;
}

void set_ddrphy_base_addr(uint32_t base_addr)
{
	m_phybase = base_addr;
}

uint32_t get_ddrphy_base_addr(void)
{
	return m_phybase;
}

static void decode_streaming_message(void)
{
	uint32_t coded_message_hex;
	uint16_t num_args;
	int i;

	coded_message_hex = get_mail(1);
	/* Get the number of argument need to be read from mailbox */
	num_args = (uint16_t)(0xffff & coded_message_hex);
	for (i = 0; i < num_args; i++) {
		(void)get_mail(1);
	}
}
