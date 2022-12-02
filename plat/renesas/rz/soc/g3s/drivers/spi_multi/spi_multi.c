/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <spi_multi.h>
#include <xspi.h>


int spi_multi_setup(void)
{
	return xspi_setup();
}
