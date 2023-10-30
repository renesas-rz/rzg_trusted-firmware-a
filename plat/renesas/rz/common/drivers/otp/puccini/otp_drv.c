/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch_helpers.h>
#include <lib/mmio.h>
#include "otp_drv.h"
#include "otp_regs.h"


static bool r_otp_open(void);
static void r_otp_close(void);
static void r_otp_dummy_read(void);


static bool r_otp_open(void)
{
	bool ready = false;
	bool ret = false;

	/* On power up OTP is powered on but on returning to this */
	/* function it is probably powered off. */
	if (0U == (mmio_read_32(OTP_OTPPWR) & (1U << OTP_OTPPWR_PWR))) {
		if ((0U == (mmio_read_32(OTP_OTPSTR) & (1U << OTP_OTPSTR_CMD_RDY)))) {
			if (0U != (mmio_read_32(OTP_OTPFLAG) & (1U << OTP_OTPFLAG_FLAG))) {
				if (0U != (mmio_read_32(OTP_OTPFLAG) & (1U << OTP_OTPFLAG_RREND))) {
					/* RDY bit should be clear */
					ready = true;
				}
			}
		}
	} else if (0U != (mmio_read_32(OTP_OTPSTR) & (1U << OTP_OTPSTR_CMD_RDY))) { /* Not powered off so RDY bit should be set */
		ready = true;
	} else {
		ready = false;
	}

	if (true == ready) {
		/* Enable power and switch to APB interface */
		mmio_write_32(OTP_OTPPWR, (1U << OTP_OTPPWR_PWR) | (1U << OTP_OTPPWR_ACCL));
		ret = true;
	}

	return ret;
}

static void r_otp_close(void)
{
	/* Dummy read */
	r_otp_dummy_read();

	/* Switch off OTP PWR and ACCL bits */
	mmio_write_32(OTP_OTPPWR, 0U);

	/* Wait for ready bit to clear */
	while (0U != (mmio_read_32(OTP_OTPSTR) & (1U << OTP_OTPSTR_CMD_RDY))) {
		isb();	  /* Not sure if this is necessary */
	}
}

static void r_otp_dummy_read(void)
{
	volatile uint32_t value;

	while (1 != (mmio_read_32(OTP_OTPSTR) & (1U << OTP_OTPSTR_CMD_RDY))) {
		; /* Polling */
	}

	mmio_write_32(OTP_OTPADRRD, OTP_IP_DUMMY_READ_ADDR);

	value = value;

	value = mmio_read_32(OTP_OTPDATARD);
}


bool r_otp_read(uint32_t addr, uint32_t *p_value, uint32_t count)
{
	bool ret = false;

	if (true == r_otp_open()) {
		if ((addr >= OTP_IP_ADDR_MIN) && (addr <= OTP_IP_ADDR_MAX) && ((addr + count - 1U) <= OTP_IP_ADDR_MAX)) {
			/* Wait for OTP access enable */
			while (1U != (mmio_read_32(OTP_OTPSTR) & (1U << OTP_OTPSTR_CMD_RDY))) {
				isb();	  /* Not sure if this is necessary */
			}

			mmio_write_32(OTP_OTPADRRD, addr);

			while (count > 0U) {
				*p_value = mmio_read_32(OTP_OTPDATARD);
				if (0U != (mmio_read_32(OTP_OTPSTR) & (1U << OTP_OTPSTR_ERR_RP))) {
					/* Error - reading protected area */
					break;
				}
				/* The address in OTP_OTPDATARD auto increments */
				p_value++;
				count -= 1U;
			}

			if (count == 0U) {
				ret = true;
			}
		}

		r_otp_close();
	}

	return ret;
}
