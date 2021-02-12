/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SPI_MULTI_H_
#define _SPI_MULTI_H_

#define SPI_MULTI_ADDR_WIDES_24				(0)
#define SPI_MULTI_ADDR_WIDES_32				(1)

#define SPI_MULTI_DQ_WIDES_1_1_1			(0)
#define SPI_MULTI_DQ_WIDES_1_1_4			(1)
#define SPI_MULTI_DQ_WIDES_1_4_4			(2)

#define SPI_MULTI_DUMMY_2CYCLE				(1 << 0)
#define SPI_MULTI_DUMMY_3CYCLE				(2 << 0)
#define SPI_MULTI_DUMMY_4CYCLE				(3 << 0)
#define SPI_MULTI_DUMMY_5CYCLE				(4 << 0)
#define SPI_MULTI_DUMMY_6CYCLE				(5 << 0)
#define SPI_MULTI_DUMMY_7CYCLE				(6 << 0)
#define SPI_MULTI_DUMMY_8CYCLE				(7 << 0)
#define SPI_MULTI_DUMMY_9CYCLE				(8 << 0)
#define SPI_MULTI_DUMMY_10CYCLE				(9 << 0)
#define SPI_MULTI_DUMMY_11CYCLE				(10 << 0)
#define SPI_MULTI_DUMMY_12CYCLE				(11 << 0)
#define SPI_MULTI_DUMMY_13CYCLE				(12 << 0)
#define SPI_MULTI_DUMMY_14CYCLE				(13 << 0)
#define SPI_MULTI_DUMMY_15CYCLE				(14 << 0)
#define SPI_MULTI_DUMMY_16CYCLE				(15 << 0)
#define SPI_MULTI_DUMMY_17CYCLE				(16 << 0)
#define SPI_MULTI_DUMMY_18CYCLE				(17 << 0)
#define SPI_MULTI_DUMMY_19CYCLE				(18 << 0)
#define SPI_MULTI_DUMMY_20CYCLE				(19 << 0)

int spi_multi_setup(uint32_t addr_width, uint32_t dq_width, uint32_t dummy_cycle);
void spi_multi_timing_set(void);

#endif	/* _SPI_MULTI_H_ */
