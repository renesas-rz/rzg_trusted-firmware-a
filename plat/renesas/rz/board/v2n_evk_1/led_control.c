/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

#include <lib/mmio.h>

#include "pfc_regs.h"
#include "led_control.h"


#define LED_IO_PM_PM_OUTPUT          (3U)
#define LED_IO_PRV_8BIT_MASK         (0xFF)

void led_control(led_io_port_pin_t pin, led_io_level_t level)
{
	volatile uint16_t pm_val = mmio_read_16(PFC_PM20);
	volatile uint8_t p_val  = mmio_read_8(PFC_P20);

	uint16_t mask_pm = (uint16_t) (~(LED_IO_PM_PM_OUTPUT << ((pin & LED_IO_PRV_8BIT_MASK) * 2)));
	uint8_t mask_p  = (uint8_t) (~(1 << (pin & LED_IO_PRV_8BIT_MASK)));

	uint16_t write_value_pm = (uint16_t) (LED_IO_PM_PM_OUTPUT << ((pin & LED_IO_PRV_8BIT_MASK) * 2));
	uint8_t write_value_p  = (uint8_t) (level << (pin & LED_IO_PRV_8BIT_MASK));

	mmio_write_16(PFC_PM20, (pm_val & mask_pm) | write_value_pm);
	mmio_write_8(PFC_P20, (p_val & mask_p) | write_value_p);
}
