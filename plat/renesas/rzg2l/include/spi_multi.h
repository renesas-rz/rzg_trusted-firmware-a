#ifndef _SPI_MULTI_H_
#define _SPI_MULTI_H_

//#define DATA_READ_COMMAND					(0x03 << 16)
//#define QUAD_FAST_READ_COMMAND				(0x6B << 16)

#define SPI_MULTI_BIT_WIDE_1_1_4			(0)
#define SPI_MULTI_BIT_WIDE_1_4_4			(1)

extern int spi_multi_setup(uint32_t command);

#endif	/* _SPI_MULTI_H_ */
