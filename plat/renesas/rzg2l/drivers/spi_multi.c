#include <stdint.h>
#include "spi_multi_regs.h"
#include "spi_multi.h"
#include <lib/mmio.h>

void spi_multi_setup(void)
{
	uint32_t val = 0;

	/* Wait until the transfer is complete */
	do {
		val = mmio_read_32(SPIM_CMNSR);
	}while((val & CMNSR_TEND) == 0);
	
	/* Set the QSPIn_SSL setting value */
	val = CMNCR_IO0FV_OUT_PREV | CMNCR_IO2FV_OUT_PREV | CMNCR_IO3FV_OUT_PREV | CMNCR_MOIIO0_OUT_PREV | CMNCR_MOIIO1_OUT_PREV | CMNCR_MOIIO2_OUT_PREV | CMNCR_MOIIO3_OUT_PREV;
	mmio_write_32(SPIM_CMNCR, val);
	
	/* Clear the RBE bit */
	val = DRCR_RBE;
	mmio_clrbits_32(SPIM_DRCR, val);
	
	/* Set the data read command */
	val = DATA_READ_COMMAND;
	mmio_write_32(SPIM_DRCMR, val);
	
	/* Set the bit width of command and address output to 1 bit and the address size to 4 bytes */
	val = DRENR_CDB_4BIT | DRENR_OCDB_4BIT | DRENR_ADB_4BIT | DRENR_OPDB_4BIT | DRENR_DRDB_4BIT | DRENR_CDE | DRENR_ADE_ADD31_OUT | DRENR_OPDE_NO_OUT;
	mmio_write_32(SPIM_DRENR, val);
	
	/* Extended external address setting */
	val = DREAR_EAV_ADDRE_DATA | DREAR_EAC_EXADDR27;
	mmio_write_32(SPIM_DREAR, val);
	
	/* Dummy cycle setting */
	mmio_write_32(SPIM_DRDMCR, 0);
	
	/* Change to SPI flash mode */
	mmio_write_32(SPIM_DRDRENR, 0);
	
	/* SDR mode serial flash settings */
	mmio_write_32(SPIM_PHYCNT, 0);
	
	/* Read timing setting */
	mmio_write_32(SPIM_PHYOFFSET1, PHYOFFSET1_DDRTMG_SPIDRE_0);
}
