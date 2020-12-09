#include <stdint.h>
#include <lib/utils_def.h>
#include "spi_multi_regs.h"
#include "spi_multi.h"
#include <lib/mmio.h>


#if DEBUG_SPI_MULTI_SLOW
#define SPIM_CMNCR_DATA			(0x01aaa200)
#define SPIM_DRCR_DATA			(0x001f0000)
#define SPIM_DRCMR_DATA			(0x00000000)
#define SPIM_DRENR_DATA			(0x00004700)
#define SPIM_DRDMCR_DATA		(0x00000000)
#define SPIM_DRDRENR_DATA		(0x00000000)
#define SPIM_PHYCNT_DATA		(0x00030260)
#define SPIM_PHYOFFSET1_DATA	(0x31511144)
#else
typedef struct {
	uint32_t  phycnt_data;
	uint32_t  phyadj2_data;;
} SPI_MULTI_ADJ;

SPI_MULTI_ADJ spi_multi_adj_tbl[] = {
	{ (PHYCNT_CKSEL_FAST | PHYCNT_DEF_DATA), PHYADJ2_ADJ_DATA1 },
	{ (PHYCNT_CKSEL_FAST | PHYCNT_DEF_DATA), PHYADJ2_ADJ_DATA2 },
	{ (PHYCNT_CKSEL_MID2 | PHYCNT_DEF_DATA), PHYADJ2_ADJ_DATA1 },
	{ (PHYCNT_CKSEL_MID2 | PHYCNT_DEF_DATA), PHYADJ2_ADJ_DATA2 },
	{ (PHYCNT_CKSEL_MID1 | PHYCNT_DEF_DATA), PHYADJ2_ADJ_DATA1 },
	{ (PHYCNT_CKSEL_MID1 | PHYCNT_DEF_DATA), PHYADJ2_ADJ_DATA2 },
	{ (PHYCNT_CKSEL_SLOW | PHYCNT_DEF_DATA), PHYADJ2_ADJ_DATA1 },
	{ (PHYCNT_CKSEL_SLOW | PHYCNT_DEF_DATA), PHYADJ2_ADJ_DATA2 },
};
	
#endif

void spi_multi_setup(uint32_t command)
{
#if !DEBUG_SPI_MULTI_SLOW
	uint32_t val = 0;
	uint32_t dat;
	uint32_t cnt;
	uint32_t adj_ok1;
	uint32_t adj_ok2;

	/* Wait until the transfer is complete */
	do {
		val = mmio_read_32(SPIM_CMNSR);
	}while((val & CMNSR_TEND) == 0);
#endif
	
#if DEBUG_SPI_MULTI_SLOW
	mmio_write_32(SPIM_CMNCR, SPIM_CMNCR_DATA);
#else
	/* Set the QSPIn_SSL setting value */
	val = CMNCR_IO0FV_OUT_PREV | CMNCR_IO2FV_OUT_PREV |
		  CMNCR_IO3FV_OUT_PREV | CMNCR_MOIIO0_OUT_PREV |
		  CMNCR_MOIIO1_OUT_PREV | CMNCR_MOIIO2_OUT_PREV |
		  CMNCR_MOIIO3_OUT_PREV | CMNCR_DEF_DATA;
	mmio_write_32(SPIM_CMNCR, val);
#endif
	
#if DEBUG_SPI_MULTI_SLOW
	mmio_write_32(SPIM_DRCR, SPIM_DRCR_DATA);
#else
	/* Clear the RBE bit */
	val = DRCR_RBE;
	mmio_clrbits_32(SPIM_DRCR, val);
	val = DRCR_RBURST_32_DATALEN;
	mmio_write_32(SPIM_DRCR, val);
#endif
	
	/* Set the data read command */
	mmio_write_32(SPIM_DRCMR, command);
	
#if DEBUG_SPI_MULTI_SLOW
	mmio_write_32(SPIM_DRENR, SPIM_DRENR_DATA);
#else
	/* Set the bit width of command and address output to 1 bit and the address size to 4 bytes */
	val = DRENR_CDB_4BIT | DRENR_OCDB_1BIT | DRENR_ADB_4BIT |
		  DRENR_OPDB_1BIT | DRENR_DRDB_4BIT | DRENR_CDE |  
		  DRENR_ADE_ADD31_OUT | DRENR_OPDE_NO_OUT;
	mmio_write_32(SPIM_DRENR, val);
#endif
	
#if !DEBUG_SPI_MULTI_SLOW
	/* Extended external address setting */
	val = DREAR_EAC_EXADDR27;
	mmio_write_32(SPIM_DREAR, val);
#endif
	
#if DEBUG_SPI_MULTI_SLOW
	mmio_write_32(SPIM_DRDMCR, SPIM_DRDMCR_DATA);
#else
	/* Dummy cycle setting */
	mmio_write_32(SPIM_DRDMCR, DRDMCR_DMCYC_4CYCLE);
#endif
	
#if DEBUG_SPI_MULTI_SLOW
	mmio_write_32(SPIM_DRDRENR, SPIM_DRDRENR_DATA);
#else
	/* Change to SPI flash mode */
	mmio_write_32(SPIM_DRDRENR, 0x00000000);
#endif
	
#if DEBUG_SPI_MULTI_SLOW
	mmio_write_32(SPIM_PHYCNT, SPIM_PHYCNT_DATA);
#else
	/* SDR mode serial flash settings */
	val = PHYCNT_CKSEL_FAST | PHYCNT_DEF_DATA;
	mmio_write_32(SPIM_PHYCNT, val);
#endif
	
#if DEBUG_SPI_MULTI_SLOW
	mmio_write_32(SPIM_PHYOFFSET1, SPIM_PHYOFFSET1_DATA);
#else
	/* Read timing setting */
	val = PHYOFFSET1_DEF_DATA | PHYOFFSET1_DDRTMG_SPIDRE_0;
	mmio_write_32(SPIM_PHYOFFSET1, val);
	
	/* Timing adjustment */
	mmio_write_32(SPIM_PHYADJ1, PHYADJ1_DEF_DATA);
	mmio_write_32(SPIM_PHYADJ2, PHYADJ2_DEF_DATA);
	
	for(cnt = 0; cnt < ARRAY_SIZE(spi_multi_adj_tbl); cnt++) {
		mmio_write_32(SPIM_PHYADJ1, PHYADJ1_ADJ_DATA);
		mmio_write_32(SPIM_PHYADJ2, spi_multi_adj_tbl[cnt].phyadj2_data);
		mmio_write_32(SPIM_PHYCNT, spi_multi_adj_tbl[cnt].phycnt_data);
		
		dat = *(volatile uint32_t *)(0x2001D210);
		if(dat == 0x6d08d447) {
			adj_ok1 = cnt;
			break;
		}
	}
	
	for(cnt = 0; cnt < ARRAY_SIZE(spi_multi_adj_tbl); cnt++) {
		mmio_write_32(SPIM_PHYADJ1, PHYADJ1_ADJ_DATA);
		mmio_write_32(SPIM_PHYADJ2, spi_multi_adj_tbl[cnt].phyadj2_data);
		mmio_write_32(SPIM_PHYCNT, spi_multi_adj_tbl[cnt].phycnt_data);
		
		dat = *(volatile uint32_t *)(0x2001D210);
		if(dat == 0x6d08d447) {
			adj_ok2 = cnt;
			break;
		}
	}
	
	val = (adj_ok1 + adj_ok2) / 2;
	mmio_write_32(SPIM_PHYADJ1, PHYADJ1_ADJ_DATA);
	mmio_write_32(SPIM_PHYADJ2, spi_multi_adj_tbl[val].phyadj2_data);
	mmio_write_32(SPIM_PHYCNT, spi_multi_adj_tbl[val].phycnt_data);
	
#endif
}
