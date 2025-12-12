/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __LED_CONTROL_H__
#define __LED_CONTROL_H__

typedef enum e_led_io_port_pin_t {
	LED_IO_PORT_00_PIN_00 = 0x0000,
	LED_IO_PORT_00_PIN_01 = 0x0001,
} led_io_port_pin_t;

typedef enum e_led_io_level {
	LED_OFF = 0,
	LED_ON
} led_io_level_t;

void led_control(led_io_port_pin_t pin, led_io_level_t level);

#endif /* __LED_CONTROL_H__ */
