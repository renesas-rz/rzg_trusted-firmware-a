/*
 * Copyright (c) 2026, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <drivers/console.h>
#include <scifa.h>
#include <rz_soc_def.h>
#include <common/debug.h>

static console_t rzg3l_console;

void rz_console_init(void)
{
	/* initialize console driver */
	int ret = console_rz_register(
							RZG3L_SCIF_0_BASE,
							RZG3L_UART_INCK_HZ,
							RZG3L_UART_BAUDRATE,
							&rzg3l_console);
	if (!ret)
		panic();

	console_set_scope(&rzg3l_console,
			CONSOLE_FLAG_BOOT | CONSOLE_FLAG_RUNTIME | CONSOLE_FLAG_CRASH);
}

void rz_console_deinit(void)
{
	console_unregister(&rzg3l_console);
}
