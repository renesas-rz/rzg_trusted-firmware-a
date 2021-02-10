/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SPI_MULTI_H_
#define _SPI_MULTI_H_

#define SPI_MULTI_ADDR_WIDES_24				(0)
#define SPI_MULTI_ADDR_WIDES_32				(1)

#define SPI_MULTI_BIT_WIDES_1_1_1			(0)
#define SPI_MULTI_BIT_WIDES_1_1_4			(1)
#define SPI_MULTI_BIT_WIDES_1_4_4			(2)

int spi_multi_setup(uint32_t addr_width, uint32_t bit_width, uint32_t dummy_cycle);
void spi_multi_timing_set(void);

#endif	/* _SPI_MULTI_H_ */
