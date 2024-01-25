/*
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
* Rev: 3.01
* Description : IO define header
*******************************************************************************/

#ifndef SDMMC_IODEFINE_H
#define SDMMC_IODEFINE_H

struct st_sdmmc0 {
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t CF:6;
			uint64_t C0:1;
			uint64_t C1:1;
			uint64_t MD0:1;
			uint64_t MD1:1;
			uint64_t MD2:1;
			uint64_t MD3:1;
			uint64_t MD4:1;
			uint64_t MD5:1;
			uint64_t MD6:1;
			uint64_t MD7:1;
			uint64_t:48;
		} BIT;
	} SD_CMD;
	char           wk1[8];
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t CF:32;
			uint64_t:32;
		} BIT;
	} SD_ARG;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t CF:16;
			uint64_t:48;
		} BIT;
	} SD_ARG1;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t STP:1;
			uint64_t:7;
			uint64_t SEC:1;
			uint64_t:7;
			uint64_t HPICMD:1;
			uint64_t HPIMODE:1;
			uint64_t:46;
		} BIT;
	} SD_STOP;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t CNT:32;
			uint64_t:32;
		} BIT;
	} SD_SECCNT;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t R:64;
		} BIT;
	} SD_RSP10;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t R:16;
			uint64_t:48;
		} BIT;
	} SD_RSP1;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t R:32;
			uint64_t:32;
		} BIT;
	} SD_RSP32;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t R:16;
			uint64_t:48;
		} BIT;
	} SD_RSP3;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t R:56;
			uint64_t:8;
		} BIT;
	} SD_RSP54;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t R:16;
			uint64_t:48;
		} BIT;
	} SD_RSP5;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t R:24;
			uint64_t:40;
		} BIT;
	} SD_RSP76;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t R:8;
			uint64_t:56;
		} BIT;
	} SD_RSP7;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t INFO0:1;
			uint64_t:1;
			uint64_t INFO2:1;
			uint64_t INFO3:1;
			uint64_t INFO4:1;
			uint64_t INFO5:1;
			uint64_t:1;
			uint64_t INFO7:1;
			uint64_t INFO8:1;
			uint64_t INFO9:1;
			uint64_t INFO10:1;
			uint64_t:5;
			uint64_t HPIRES:1;
			uint64_t:47;
		} BIT;
	} SD_INFO1;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t ERR0:1;
			uint64_t ERR1:1;
			uint64_t ERR2:1;
			uint64_t ERR3:1;
			uint64_t ERR4:1;
			uint64_t ERR5:1;
			uint64_t ERR6:1;
			uint64_t DAT0:1;
			uint64_t BRE:1;
			uint64_t BWE:1;
			uint64_t:3;
			uint64_t SCLKDIVEN:1;
			uint64_t CBSY:1;
			uint64_t ILA:1;
			uint64_t:48;
		} BIT;
	} SD_INFO2;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t IMASK0:1;
			uint64_t:1;
			uint64_t IMASK2:1;
			uint64_t IMASK3:1;
			uint64_t IMASK4:1;
			uint64_t:3;
			uint64_t IMASK8:1;
			uint64_t IMASK9:1;
			uint64_t:6;
			uint64_t IMASK16:1;
			uint64_t:47;
		} BIT;
	} SD_INFO1_MASK;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t EMASK0:1;
			uint64_t EMASK1:1;
			uint64_t EMASK2:1;
			uint64_t EMASK3:1;
			uint64_t EMASK4:1;
			uint64_t EMASK5:1;
			uint64_t EMASK6:1;
			uint64_t:1;
			uint64_t BMASK0:1;
			uint64_t BMASK1:1;
			uint64_t:5;
			uint64_t IMASK:1;
			uint64_t:48;
		} BIT;
	} SD_INFO2_MASK;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t DIV:8;
			uint64_t SCLKEN:1;
			uint64_t SDCLKOFFEN:1;
			uint64_t:54;
		} BIT;
	} SD_CLK_CTRL;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t LEN:10;
			uint64_t:54;
		} BIT;
	} SD_SIZE;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t CTOP21:1;
			uint64_t CTOP22:1;
			uint64_t CTOP23:1;
			uint64_t CTOP24:1;
			uint64_t TOP24:1;
			uint64_t TOP25:1;
			uint64_t TOP26:1;
			uint64_t TOP27:1;
			uint64_t TOUTMASK:1;
			uint64_t EXTOP:1;
			uint64_t:3;
			uint64_t WIDTH8:1;
			uint64_t:1;
			uint64_t WIDTH:1;
			uint64_t:48;
		} BIT;
	} SD_OPTION;
	char           wk2[8];
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t E0:1;
			uint64_t E1:1;
			uint64_t E2:1;
			uint64_t E3:1;
			uint64_t E4:1;
			uint64_t E5:1;
			uint64_t:2;
			uint64_t E8:1;
			uint64_t E9:1;
			uint64_t E10:1;
			uint64_t E11:1;
			uint64_t E12:1;
			uint64_t E13:1;
			uint64_t E14:1;
			uint64_t:49;
		} BIT;
	} SD_ERR_STS1;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t E0:1;
			uint64_t E1:1;
			uint64_t E2:1;
			uint64_t E3:1;
			uint64_t E4:1;
			uint64_t E5:1;
			uint64_t E6:1;
			uint64_t:57;
		} BIT;
	} SD_ERR_STS2;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t BUF:64;
		} BIT;
	} SD_BUF0;
	char           wk3[8];
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t IOMOD:1;
			uint64_t:1;
			uint64_t RWREQ:1;
			uint64_t:5;
			uint64_t IOABT:1;
			uint64_t C52PUB:1;
			uint64_t:54;
		} BIT;
	} SDIO_MODE;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t IOIRQ:1;
			uint64_t:13;
			uint64_t EXPUB52:1;
			uint64_t EXWT:1;
			uint64_t:48;
		} BIT;
	} SDIO_INFO1;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t IOMSK:1;
			uint64_t:13;
			uint64_t MEXPUB52:1;
			uint64_t MEXWT:1;
			uint64_t:48;
		} BIT;
	} SDIO_INFO1_MASK;
	char           wk4[632];
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t:1;
			uint64_t DMASDRW:1;
			uint64_t:62;
		} BIT;
	} CC_EXT_MODE;
	char           wk5[24];
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t SDRST:1;
			uint64_t:63;
		} BIT;
	} SOFT_RST;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t IP:8;
			uint64_t UR:8;
			uint64_t:48;
		} BIT;
	} VERSION;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t WMODE:1;
			uint64_t ENDIAN:1;
			uint64_t:6;
			uint64_t BUSWIDTH:1;
			uint64_t:55;
		} BIT;
	} HOST_MODE;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t DDR:1;
			uint64_t:7;
			uint64_t NOCHKCR:1;
			uint64_t:55;
		} BIT;
	} SDIF_MODE;
	char           wk6[40];
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t:1;
			uint64_t SD_RST:1;
			uint64_t:62;
		} BIT;
	} SD_STATUS;
	char           wk7[1080];
	char           wk8[8];
	char           wk9[16];
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t:4;
			uint64_t BUS_WIDTH:2;
			uint64_t:10;
			uint64_t CH_NUM:2;
			uint64_t:46;
		} BIT;
	} DM_CM_DTRAN_MODE;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t DM_START:1;
			uint64_t:63;
		} BIT;
	} DM_CM_DTRAN_CTRL;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t SEQRST:1;
			uint64_t:7;
			uint64_t DTRANRST0:1;
			uint64_t DTRANRST1:1;
			uint64_t:54;
		} BIT;
	} DM_CM_RST;
	char           wk10[8];
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t SEQEND:1;
			uint64_t:15;
			uint64_t DTRANEND0:1;
			uint64_t:3;
			uint64_t DTRANEND1:1;
			uint64_t:43;
		} BIT;
	} DM_CM_INFO1;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t SEQEND_MASK:1;
			uint64_t:15;
			uint64_t DTRANEND0_MASK:1;
			uint64_t:3;
			uint64_t DTRANEND1_MASK:1;
			uint64_t:43;
		} BIT;
	} DM_CM_INFO1_MASK;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t SEQERR:1;
			uint64_t:15;
			uint64_t DTRANERR0:1;
			uint64_t DTRANERR1:1;
			uint64_t:46;
		} BIT;
	} DM_CM_INFO2;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t SEQERR_MASK:1;
			uint64_t:15;
			uint64_t DTRANERR0_MASK:1;
			uint64_t DTRANERR1_MASK:1;
			uint64_t:46;
		} BIT;
	} DM_CM_INFO2_MASK;
	char           wk11[32];
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t:3;
			uint64_t DADDR:29;
			uint64_t:32;
		} BIT;
	} DM_DTRAN_ADDR;
	char           wk12[1912];
	union {
		uint32_t LONG;
		struct {
			uint32_t TAPEN:1;
			uint32_t:15;
			uint32_t TAPNUM:8;
			uint32_t:8;
		} BIT;
	} SCC_DTCNTL;
	char           wk13[4];
	union {
		uint32_t LONG;
		struct {
			uint32_t TAPSET:8;
			uint32_t:24;
		} BIT;
	} SCC_TAPSET;
	char           wk14[4];
	union {
		uint32_t LONG;
		struct {
			uint32_t DT2NSSET:8;
			uint32_t DT2NESET:8;
			uint32_t:16;
		} BIT;
	} SCC_DT2FF;
	char           wk15[4];
	union {
		uint32_t LONG;
		struct {
			uint32_t DTSEL:1;
			uint32_t:31;
		} BIT;
	} SCC_CKSEL;
	char           wk16[4];
	union {
		uint32_t LONG;
		struct {
			uint32_t RVSEN:1;
			uint32_t RVSW:1;
			uint32_t:6;
			uint32_t TAPSEL:8;
			uint32_t:16;
		} BIT;
	} SCC_RVSCNTL;
	char           wk17[4];
	union {
		uint32_t LONG;
		struct {
			uint32_t REQTAPDWN:1;
			uint32_t REQTAPUP:1;
			uint32_t RVSERR:1;
			uint32_t:29;
		} BIT;
	} SCC_RVSREQ;
	char           wk18[4];
	union {
		uint32_t LONG;
		struct {
			uint32_t CMPNGD:9;
			uint32_t:7;
			uint32_t CMPNGU:9;
			uint32_t:7;
		} BIT;
	} SCC_SMPCMP;
	char           wk19[4];
	union {
		uint32_t LONG;
		struct {
			uint32_t TMPOUT:16;
			uint32_t:16;
		} BIT;
	} SCC_TMPPORT;
};

struct st_sdmmc1 {
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t CF:6;
			uint64_t C0:1;
			uint64_t C1:1;
			uint64_t MD0:1;
			uint64_t MD1:1;
			uint64_t MD2:1;
			uint64_t MD3:1;
			uint64_t MD4:1;
			uint64_t MD5:1;
			uint64_t MD6:1;
			uint64_t MD7:1;
			uint64_t:48;
		} BIT;
	} SD_CMD;
	char           wk1[8];
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t CF:32;
			uint64_t:32;
		} BIT;
	} SD_ARG;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t CF:16;
			uint64_t:48;
		} BIT;
	} SD_ARG1;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t STP:1;
			uint64_t:7;
			uint64_t SEC:1;
			uint64_t:7;
			uint64_t HPICMD:1;
			uint64_t HPIMODE:1;
			uint64_t:46;
		} BIT;
	} SD_STOP;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t CNT:32;
			uint64_t:32;
		} BIT;
	} SD_SECCNT;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t R:64;
		} BIT;
	} SD_RSP10;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t R:16;
			uint64_t:48;
		} BIT;
	} SD_RSP1;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t R:32;
			uint64_t:32;
		} BIT;
	} SD_RSP32;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t R:16;
			uint64_t:48;
		} BIT;
	} SD_RSP3;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t R:56;
			uint64_t:8;
		} BIT;
	} SD_RSP54;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t R:16;
			uint64_t:48;
		} BIT;
	} SD_RSP5;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t R:24;
			uint64_t:40;
		} BIT;
	} SD_RSP76;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t R:8;
			uint64_t:56;
		} BIT;
	} SD_RSP7;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t INFO0:1;
			uint64_t:1;
			uint64_t INFO2:1;
			uint64_t INFO3:1;
			uint64_t INFO4:1;
			uint64_t INFO5:1;
			uint64_t:1;
			uint64_t INFO7:1;
			uint64_t INFO8:1;
			uint64_t INFO9:1;
			uint64_t INFO10:1;
			uint64_t:5;
			uint64_t HPIRES:1;
			uint64_t:47;
		} BIT;
	} SD_INFO1;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t ERR0:1;
			uint64_t ERR1:1;
			uint64_t ERR2:1;
			uint64_t ERR3:1;
			uint64_t ERR4:1;
			uint64_t ERR5:1;
			uint64_t ERR6:1;
			uint64_t DAT0:1;
			uint64_t BRE:1;
			uint64_t BWE:1;
			uint64_t:3;
			uint64_t SCLKDIVEN:1;
			uint64_t CBSY:1;
			uint64_t ILA:1;
			uint64_t:48;
		} BIT;
	} SD_INFO2;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t IMASK0:1;
			uint64_t:1;
			uint64_t IMASK2:1;
			uint64_t IMASK3:1;
			uint64_t IMASK4:1;
			uint64_t:3;
			uint64_t IMASK8:1;
			uint64_t IMASK9:1;
			uint64_t:6;
			uint64_t IMASK16:1;
			uint64_t:47;
		} BIT;
	} SD_INFO1_MASK;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t EMASK0:1;
			uint64_t EMASK1:1;
			uint64_t EMASK2:1;
			uint64_t EMASK3:1;
			uint64_t EMASK4:1;
			uint64_t EMASK5:1;
			uint64_t EMASK6:1;
			uint64_t:1;
			uint64_t BMASK0:1;
			uint64_t BMASK1:1;
			uint64_t:5;
			uint64_t IMASK:1;
			uint64_t:48;
		} BIT;
	} SD_INFO2_MASK;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t DIV:8;
			uint64_t SCLKEN:1;
			uint64_t SDCLKOFFEN:1;
			uint64_t:54;
		} BIT;
	} SD_CLK_CTRL;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t LEN:10;
			uint64_t:54;
		} BIT;
	} SD_SIZE;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t CTOP21:1;
			uint64_t CTOP22:1;
			uint64_t CTOP23:1;
			uint64_t CTOP24:1;
			uint64_t TOP24:1;
			uint64_t TOP25:1;
			uint64_t TOP26:1;
			uint64_t TOP27:1;
			uint64_t TOUTMASK:1;
			uint64_t EXTOP:1;
			uint64_t:3;
			uint64_t WIDTH8:1;
			uint64_t:1;
			uint64_t WIDTH:1;
			uint64_t:48;
		} BIT;
	} SD_OPTION;
	char           wk2[8];
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t E0:1;
			uint64_t E1:1;
			uint64_t E2:1;
			uint64_t E3:1;
			uint64_t E4:1;
			uint64_t E5:1;
			uint64_t:2;
			uint64_t E8:1;
			uint64_t E9:1;
			uint64_t E10:1;
			uint64_t E11:1;
			uint64_t E12:1;
			uint64_t E13:1;
			uint64_t E14:1;
			uint64_t:49;
		} BIT;
	} SD_ERR_STS1;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t E0:1;
			uint64_t E1:1;
			uint64_t E2:1;
			uint64_t E3:1;
			uint64_t E4:1;
			uint64_t E5:1;
			uint64_t E6:1;
			uint64_t:57;
		} BIT;
	} SD_ERR_STS2;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t BUF:64;
		} BIT;
	} SD_BUF0;
	char           wk3[8];
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t IOMOD:1;
			uint64_t:1;
			uint64_t RWREQ:1;
			uint64_t:5;
			uint64_t IOABT:1;
			uint64_t C52PUB:1;
			uint64_t:54;
		} BIT;
	} SDIO_MODE;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t IOIRQ:1;
			uint64_t:13;
			uint64_t EXPUB52:1;
			uint64_t EXWT:1;
			uint64_t:48;
		} BIT;
	} SDIO_INFO1;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t IOMSK:1;
			uint64_t:13;
			uint64_t MEXPUB52:1;
			uint64_t MEXWT:1;
			uint64_t:48;
		} BIT;
	} SDIO_INFO1_MASK;
	char           wk4[632];
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t:1;
			uint64_t DMASDRW:1;
			uint64_t:62;
		} BIT;
	} CC_EXT_MODE;
	char           wk5[24];
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t SDRST:1;
			uint64_t:63;
		} BIT;
	} SOFT_RST;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t IP:8;
			uint64_t UR:8;
			uint64_t:48;
		} BIT;
	} VERSION;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t WMODE:1;
			uint64_t ENDIAN:1;
			uint64_t:6;
			uint64_t BUSWIDTH:1;
			uint64_t:55;
		} BIT;
	} HOST_MODE;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t DDR:1;
			uint64_t:7;
			uint64_t NOCHKCR:1;
			uint64_t:55;
		} BIT;
	} SDIF_MODE;
	char           wk6[40];
	char           wk7[1088];
	char           wk8[8];
	char           wk9[16];
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t:4;
			uint64_t BUS_WIDTH:2;
			uint64_t:10;
			uint64_t CH_NUM:2;
			uint64_t:46;
		} BIT;
	} DM_CM_DTRAN_MODE;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t DM_START:1;
			uint64_t:63;
		} BIT;
	} DM_CM_DTRAN_CTRL;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t SEQRST:1;
			uint64_t:7;
			uint64_t DTRANRST0:1;
			uint64_t DTRANRST1:1;
			uint64_t:54;
		} BIT;
	} DM_CM_RST;
	char           wk10[8];
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t SEQEND:1;
			uint64_t:15;
			uint64_t DTRANEND0:1;
			uint64_t:3;
			uint64_t DTRANEND1:1;
			uint64_t:43;
		} BIT;
	} DM_CM_INFO1;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t SEQEND_MASK:1;
			uint64_t:15;
			uint64_t DTRANEND0_MASK:1;
			uint64_t:3;
			uint64_t DTRANEND1_MASK:1;
			uint64_t:43;
		} BIT;
	} DM_CM_INFO1_MASK;
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t SEQERR:1;
			uint64_t:15;
			uint64_t DTRANERR0:1;
			uint64_t DTRANERR1:1;
			uint64_t:46;
		} BIT;
	} DM_CM_INFO2;
	union{
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t SEQERR_MASK:1;
			uint64_t:15;
			uint64_t DTRANERR0_MASK:1;
			uint64_t DTRANERR1_MASK:1;
			uint64_t:46;
		} BIT;
	} DM_CM_INFO2_MASK;
	char           wk11[32];
	union {
		uint64_t LONGLONG;
		struct {
			uint32_t L;
			uint32_t H;
		} LONG;
		struct {
			uint16_t LL;
			uint16_t LH;
			uint16_t HL;
			uint16_t HH;
		} WORD;
		struct {
			uint64_t:3;
			uint64_t DADDR:29;
			uint64_t:32;
		} BIT;
	} DM_DTRAN_ADDR;
	char           wk12[1912];
	union {
		uint32_t LONG;
		struct {
			uint32_t TAPEN:1;
			uint32_t:15;
			uint32_t TAPNUM:8;
			uint32_t:8;
		} BIT;
	} SCC_DTCNTL;
	char           wk13[4];
	union {
		uint32_t LONG;
		struct {
			uint32_t TAPSET:8;
			uint32_t:24;
		} BIT;
	} SCC_TAPSET;
	char           wk14[4];
	union {
		uint32_t LONG;
		struct {
			uint32_t DT2NSSET:8;
			uint32_t DT2NESET:8;
			uint32_t:16;
		} BIT;
	} SCC_DT2FF;
	char           wk15[4];
	union {
		uint32_t LONG;
		struct {
			uint32_t DTSEL:1;
			uint32_t:31;
		} BIT;
	} SCC_CKSEL;
	char           wk16[4];
	union {
		uint32_t LONG;
		struct {
			uint32_t RVSEN:1;
			uint32_t RVSW:1;
			uint32_t:6;
			uint32_t TAPSEL:8;
			uint32_t:16;
		} BIT;
	} SCC_RVSCNTL;
	char           wk17[4];
	union {
		uint32_t LONG;
		struct {
			uint32_t REQTAPDWN:1;
			uint32_t REQTAPUP:1;
			uint32_t RVSERR:1;
			uint32_t:29;
		} BIT;
	} SCC_RVSREQ;
	char           wk18[4];
	union {
		uint32_t LONG;
		struct {
			uint32_t CMPNGD:9;
			uint32_t:7;
			uint32_t CMPNGU:9;
			uint32_t:7;
		} BIT;
	} SCC_SMPCMP;
	char           wk19[4];
	union {
		uint32_t LONG;
		struct {
			uint32_t TMPOUT:16;
			uint32_t:16;
		} BIT;
	} SCC_TMPPORT;
};

#endif
