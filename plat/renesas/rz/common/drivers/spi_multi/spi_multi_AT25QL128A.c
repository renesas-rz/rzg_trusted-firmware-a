/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdint.h>
#include <lib/utils_def.h>
#include <lib/mmio.h>
#include <arch_helpers.h>
#include <spi_multi_regs.h>
#include <spi_multi.h>
#include <spi_multi_reg_values.h>

#include <drivers/delay_timer.h>

/* spi_multi_status_read input parameter */
typedef enum {
        STATUS_REGISTER_1,
        STATUS_REGISTER_2,
}status_register_type;

static uint8_t spi_multi_status_read(status_register_type r_type)
{
	uint32_t val;
	uint8_t r_status;
	uint8_t command;

	/* SDR mode serial flash settings */
	mmio_write_32(SPIM_PHYCNT, SPIM_PHYCNT_SET_VALUE);
	mmio_write_32(SPIM_PHYCNT, SPIM_PHYCNT_SDR_TIM_ADJ_SET_VALUE);

	/* Set the QSPIn_SSL setting value & Manual Mode */
	mmio_write_32(SPIM_CMNCR, SPIM_CMNCR_MANUAL_SET_VALUE);

	/* Set the Manual Mode Command */
	if (r_type == STATUS_REGISTER_1) {
		command = SMCMR_CMD_READ_STATUS_REGISTER_1;
	} else {
		command = SMCMR_CMD_READ_STATUS_REGISTER_2;
	}
	val = command << SMCMR_CMD_BIT_SHIFT;
	mmio_write_32(SPIM_SMCMR, val);

	/* Set the Transfer Data size setting value &  command output enable */
	val = SMENR_CDE | SPI_MANUAL_COMMAND_SIZE_16_BIT;
	mmio_write_32(SPIM_SMENR, val);

	val = SMCR_SPIE | SMCR_SPIRE;
	mmio_write_32(SPIM_SMCR, val);

	/* Wait until the transfer is complete */
	do {
		val = mmio_read_32(SPIM_CMNSR);
	} while ((val & CMNSR_TEND) == 0);

	val=mmio_read_32(SPIM_SMRDR0);

	r_status = (uint8_t)val;
	return(r_status);
}

static void spi_multi_cmd_write(uint8_t command,uint8_t size,uint32_t data)
{
	uint32_t val;

	/* SDR mode serial flash settings */
	mmio_write_32(SPIM_PHYCNT, SPIM_PHYCNT_SET_VALUE);
	mmio_write_32(SPIM_PHYCNT, SPIM_PHYCNT_SDR_TIM_ADJ_SET_VALUE);

	/* Set the QSPIn_SSL setting value & Manual Mode */
	mmio_write_32(SPIM_CMNCR, SPIM_CMNCR_MANUAL_SET_VALUE);

	/* Set the Manual Mode Command */
	val = command << SMCMR_CMD_BIT_SHIFT;
	mmio_write_32(SPIM_SMCMR, val);

	/* Set the Transfer Data size setting value &  command output enable */
	val = SMENR_CDE | size;
	mmio_write_32(SPIM_SMENR, val);

	/* Set the write data in Manual mode */
	mmio_write_32(SPIM_SMWDR0, data);

	/* Set the SDR transfer & SPI flash mode setting value */
	mmio_write_32(SPIM_SMDRENR, SPIM_SMDRENR_SET_VALUE);

	/* Set the data transfer enable & data write enable  */
	if (size == SPI_MANUAL_COMMAND_SIZE_0)
	{
		val = SMCR_SPIE;
	} else {
		val = SMCR_SPIE | SMCR_SPIWE;
	}
	mmio_write_32(SPIM_SMCR, val);

	/* Wait until the transfer is complete */
	do {
		val = mmio_read_32(SPIM_CMNSR);
	} while ((val & CMNSR_TEND) == 0);
}

void spi_multi_setup_device( void )
{

	uint32_t val;
	uint8_t  read_status;

	mmio_write_32(SPIM_PHYOFFSET1, SPIM_PHYOFFSET1_SET_VALUE);
	mmio_write_32(SPIM_PHYOFFSET2, SPIM_PHYOFFSET2_SET_VALUE);
	spi_multi_timing_set();

	/*  Set Data read option */
	/* Required when command 0xEB is specified.
	 * Not required when a command other than is specified,
	 * but there is no problem in operation.
	 */
	val = SPIM_DROPR_SET_VALUE;
	mmio_write_32(SPIM_DROPR, val);

	read_status = spi_multi_status_read(STATUS_REGISTER_2);
	if ((read_status & STATUS_2_QE) == STATUS_2_QE) {
		return;
	}
	/* Write Enable Command */
	spi_multi_cmd_write(SMCMR_CMD_WRITE_ENABLE,SPI_MANUAL_COMMAND_SIZE_0,0);
	/* Write Status Register-2 Command Quad Enable */
	val = ((STATUS_2_QE | read_status) << SMWDR0_1BYTE_DATA_BIT_SHIFT);
	spi_multi_cmd_write(SMCMR_CMD_WRITE_STATUS_REGISTER_2,SPI_MANUAL_COMMAND_SIZE_8_BIT,val);
	/* status 1 BUSY check */
	while(1) {
		read_status = spi_multi_status_read(STATUS_REGISTER_1);
		if (( read_status & STATUS_1_BUSY_BIT) == STATUS_1_BUSY) {
			udelay(STATUS_BUSY_READ_DELAY_TIME);
			continue;
		} else {
			break;
		}
	}
	return;
}
