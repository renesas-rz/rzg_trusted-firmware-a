#ifndef _SPI_MULTI_H_
#define _SPI_MULTI_H_

#define DATA_READ_COMMAND					(0x03 << 16)
#define QUAD_FAST_READ_COMMAND				(0x6B << 16)

extern int spi_multi_setup(uint32_t command);

#endif	/* _SPI_MULTI_H_ */
