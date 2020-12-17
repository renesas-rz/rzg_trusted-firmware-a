/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <lib/utils_def.h>
#include <lib/mmio.h>
#include "spi_multi_regs.h"
#include "spi_multi.h"

typedef struct {
	uint32_t  phycnt_data;
	uint32_t  phyadj2_data;
} SPI_MULTI_ADJ;

static SPI_MULTI_ADJ spi_multi_adj_tbl[] = {
	{ (PHYCNT_CKSEL_FAST | PHYCNT_DEF_DATA), PHYADJ2_ADJ_DATA1 },
	{ (PHYCNT_CKSEL_FAST | PHYCNT_DEF_DATA), PHYADJ2_ADJ_DATA2 },
	{ (PHYCNT_CKSEL_MID2 | PHYCNT_DEF_DATA), PHYADJ2_ADJ_DATA1 },
	{ (PHYCNT_CKSEL_MID2 | PHYCNT_DEF_DATA), PHYADJ2_ADJ_DATA2 },
	{ (PHYCNT_CKSEL_MID1 | PHYCNT_DEF_DATA), PHYADJ2_ADJ_DATA1 },
	{ (PHYCNT_CKSEL_MID1 | PHYCNT_DEF_DATA), PHYADJ2_ADJ_DATA2 },
	{ (PHYCNT_CKSEL_SLOW | PHYCNT_DEF_DATA), PHYADJ2_ADJ_DATA1 },
	{ (PHYCNT_CKSEL_SLOW | PHYCNT_DEF_DATA), PHYADJ2_ADJ_DATA2 },
};

typedef struct {
	uint32_t  command;
	uint32_t  adr_wide;
	uint32_t  data_wide;
	uint32_t  cycle;
} SPI_MULTI_BIT_WIDE_PTN;

static SPI_MULTI_BIT_WIDE_PTN spi_multi_bit_ptn[2] = {
	{ 0x6B << 16, DRENR_ADB_1BIT, DRENR_DRDB_4BIT, DRDMCR_DMCYC_8CYCLE },
	{ 0xEB << 16, DRENR_ADB_4BIT, DRENR_DRDB_4BIT, DRDMCR_DMCYC_10CYCLE }
};

int spi_multi_setup(uint32_t ptn)
{
	int ret = 0;
	uint32_t val;
	uint32_t dat;
	uint32_t cnt;
	uint32_t adj_ok1;
	uint32_t adj_ok2;
	int      flg = 0;

	/* SDR mode serial flash settings */
	mmio_write_32(SPIM_PHYCNT, PHYCNT_DEF_DATA);

	/* Read timing setting */
	val = PHYOFFSET1_DEF_DATA | PHYOFFSET1_DDRTMG_SPIDRE_0;
	mmio_write_32(SPIM_PHYOFFSET1, val);

	/* Set the QSPIn_SSL setting value */
	val = CMNCR_IO0FV_OUT_PREV | CMNCR_IO2FV_OUT_PREV |
		  CMNCR_IO3FV_OUT_PREV | CMNCR_MOIIO0_OUT1 |
		  CMNCR_MOIIO1_OUT1    | CMNCR_MOIIO2_OUT1 |
		  CMNCR_MOIIO3_OUT1    | CMNCR_DEF_DATA;
	mmio_write_32(SPIM_CMNCR, val);

	val = SSLDR_SCKDL_4_5 | SSLDR_SLNDL_4QSPIn | SSLDR_SPNDL_4QSPIn;
	mmio_write_32(SPIM_SSLDR, val);

	/* Clear the RBE bit */
	mmio_clrbits_32(SPIM_DRCR, DRCR_RBE);
	val = DRCR_RBE | DRCR_RCF | DRCR_RBURST_32_DATALEN;
	mmio_write_32(SPIM_DRCR, val);
	mmio_read_32(SPIM_DRCR);

	/* Set the data read command */
	mmio_write_32(SPIM_DRCMR, spi_multi_bit_ptn[ptn].command);

	/* Set the bit width of command and address output to 1 bit and	*/
	/* the address size to 4 byte									*/
	val = DRENR_CDB_1BIT | DRENR_OCDB_1BIT | spi_multi_bit_ptn[ptn].adr_wide |
		  DRENR_OPDB_1BIT | spi_multi_bit_ptn[ptn].data_wide | DRENR_CDE | DRENR_DME |
		  DRENR_ADE_ADD23_OUT | DRENR_OPDE_NO_OUT;
	mmio_write_32(SPIM_DRENR, val);

#if !DEBUG_RZG2L_FPGA
	/* Extended external address setting */
	mmio_write_32(SPIM_DREAR, DREAR_EAC_EXADDR27);
#endif

	/* Dummy cycle setting */
	mmio_write_32(SPIM_DRDMCR, spi_multi_bit_ptn[ptn].cycle);

	/* Change to SPI flash mode */
	mmio_write_32(SPIM_DRDRENR, 0x00000000);

	/* Wait until the transfer is complete */
	do {
		val = mmio_read_32(SPIM_CMNSR);
	} while ((val & CMNSR_TEND) == 0);

	/* Timing adjustment */
	mmio_write_32(SPIM_PHYADJ1, PHYADJ1_DEF_DATA);
	mmio_write_32(SPIM_PHYADJ2, PHYADJ2_DEF_DATA);

	for (cnt = 0; cnt < ARRAY_SIZE(spi_multi_adj_tbl); cnt++) {
		/* Wait until the transfer is complete */
		do {
			val = mmio_read_32(SPIM_CMNSR);
		} while ((val & CMNSR_TEND) == 0);

		mmio_write_32(SPIM_PHYADJ1, PHYADJ1_ADJ_DATA);
		mmio_write_32(SPIM_PHYADJ2, spi_multi_adj_tbl[cnt].phyadj2_data);
		mmio_write_32(SPIM_PHYCNT, spi_multi_adj_tbl[cnt].phycnt_data);

		dat = *(volatile uint32_t *)(0x2001D210);
		if (dat == 0x6d08d447) {
			adj_ok1 = cnt;
			flg++;
			break;
		}
	}

	for (cnt = 0; cnt < ARRAY_SIZE(spi_multi_adj_tbl); cnt++) {
		/* Wait until the transfer is complete */
		do {
			val = mmio_read_32(SPIM_CMNSR);
		} while ((val & CMNSR_TEND) == 0);

		mmio_write_32(SPIM_PHYADJ1, PHYADJ1_ADJ_DATA);
		mmio_write_32(SPIM_PHYADJ2, spi_multi_adj_tbl[cnt].phyadj2_data);
		mmio_write_32(SPIM_PHYCNT, spi_multi_adj_tbl[cnt].phycnt_data);

		dat = *(volatile uint32_t *)(0x2001D210);
		if (dat == 0x6d08d447) {
			adj_ok2 = cnt;
			flg++;
			break;
		}
	}

	if (flg == 2) {
		val = (adj_ok1 + adj_ok2) / 2;
		mmio_write_32(SPIM_PHYADJ1, PHYADJ1_ADJ_DATA);
		mmio_write_32(SPIM_PHYADJ2, spi_multi_adj_tbl[val].phyadj2_data);
		mmio_write_32(SPIM_PHYCNT, spi_multi_adj_tbl[val].phycnt_data);
	} else {
		ret = -1;
	}

	return ret;
}
