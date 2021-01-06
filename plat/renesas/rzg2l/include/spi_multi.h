/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SPI_MULTI_H_
#define _SPI_MULTI_H_

#define SPI_MULTI_BIT_WIDE_1_1_4			(0)
#define SPI_MULTI_BIT_WIDE_1_4_4			(1)
#define SPI_MULTI_BIT_WIDE_1_1_1			(2)

int spi_multi_setup(uint32_t command);

#endif	/* _SPI_MULTI_H_ */
