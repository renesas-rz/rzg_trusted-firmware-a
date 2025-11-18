/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <asm_macros.S>
#include <console_macros.S>

#define RDR				(0x0000)
#define TDR				(0x0004)
#define CCR0			(0x0008)
#define CCR1			(0x000C)
#define CCR2			(0x0010)
#define CCR3			(0x0014)
#define CCR4			(0x0018)
#define FCR				(0x0024)
#define CSR				(0x0048)
#define FRSR			(0x0050)
#define FTSR			(0x0054)
#define CFCLR			(0x0068)


/*
 * RDR register bit mask definitions
 */
#define RDR_RDAT				(0)									/* RDAT offset */
#define RDR_RDAT_MSK_8BIT		(0xFF << RDR_RDAT)

/*
 * TDR register bit mask definitions
 */
#define TDR_TDAT				(0)									/* TDAT offset */
#define TDR_TDAT_MSK_8BIT		(0xFF << TDR_TDAT)

/*
 * CCR0 register bit mask definitions
 */
#define CCR0_RE					(0)									/* RE offset */
#define CCR0_RE_MSK				(1 << CCR0_RE)
#define CCR0_TE					(4)									/* TE offset */
#define CCR0_TE_MSK				(1 << CCR0_TE)

/*
 * CCR2 register bit mask definitions
 */
#define CCR2_BRR				(8)									/* BRR offset */
#define CCR2_BRR_MSK			(0xFF << CCR2_BRR)
#define CCR2_CKS				(20)								/* CKS offset */
#define CCR2_CKS_MSK			(3 << CCR2_CKS)
#define CCR2_BRME				(16)								/* BRME offset */
#define CRR2_BRME_MSK			(1 << CCR2_BRME)
#define CCR2_MDDR				(24)								/* MDDR offset */
#define CCR2_MDDR_MSK			(0xFF << CCR2_MDDR)

/*
 * CCR3 register bit mask definitions
 */
#define CCR3_BPEN				(7)									/* BPEN offset */
#define CCR3_BPEN_MSK			(1 << CCR3_BPEN)
#define CCR3_CHCR				(8)									/* CHCR offset */
#define CCR3_CHCR_MSK			(3 << CCR3_CHCR)
#define CCR3_STP				(14)								/* STP offset */
#define CCR3_STP_MSK			(1 << CCR3_STP)
#define CCR3_MOD				(16)								/* MOD offset */
#define CCR3_MOD_MSK			(7 << CCR3_MOD)
#define CCR3_MP					(19)								/* MP offset */
#define CCR3_MP_MSK				(1 << CCR3_MP)
#define CCR3_FM					(20)								/* FM offset */
#define CCR3_FM_MSK				(1 << CCR3_FM)
#define CCR3_DEN				(21)								/* DEN offset */
#define CCR3_DEN_MSK			(1 << CCR3_DEN)
#define CCR3_CKE				(24)								/* CKE offset */
#define CCR3_CKE_MSK			(3 << CCR3_CKE)
#define CCR3_ACS0				(26)								/* ACS0 offset */
#define CCR3_ACS0_MSK			(1 << CCR3_ACS0)

/*
 * CCR3 register values
 */
#define CCR3_CHCR_8BIT			(2 << CCR3_CHCR)
#define CCR3_STP_ONE			(0 << CCR3_STP)
#define CCR3_MOD_ASYNC			(0 << CCR3_MOD)
#define CCR3_MP_DISABLED		(0 << CCR3_MP)
#define CCR3_DEN_DISABLED		(0 << CCR3_DEN)

/*
 * CSR register bit mask definitions
 */
#define CSR_ORER				(24)								/* ORER offset */
#define CSR_ORER_MSK			(1 << CSR_ORER)
#define CSR_PER					(27)								/* PER offset */
#define CSR_PER_MSK				(1 << CSR_PER)
#define CSR_FER					(28)								/* FER offset */
#define CSR_FER_MSK				(1 << CSR_FER)
#define CSR_TDRE				(29)								/* TDRE offset */
#define CSR_TDRE_MSK			(1 << CSR_TDRE)
#define CSR_TEND				(30)								/* TEND offset */
#define CSR_TEND_MSK			(1 << CSR_TEND)
#define CSR_RDRF				(31)								/* RDRF offset */
#define CSR_RDRF_MSK			(1 << CSR_RDRF)

/*
 * FRSR register bit mask definitions
 */
#define FRSR_R					(8)									/* R[5:0] offset */
#define FRSR_R_MSK				(0x3F << FRSR_R)

/*
 * FTSR register bit mask definitions
 */
#define FTSR_T					(0)									/* T[5:0] offset */
#define FTSR_T_WIDTH			(6)									/* T Field width in bits */
#define FTSR_T_MSK				(0x3F << FTSR_T)

/*
 * CFCLR register bit mask definitions
 */
#define CFCLR_ERSC				(4)									/* ERSC offset */
#define CFCLR_ERSC_MSK			(1 << CFCLR_ERSC)
#define CFCLR_DCMFC				(16)								/* DCMFC offset */
#define CFCLR_DCMFC_MSK			(1 << CFCLR_DCMFC)
#define CFCLR_DPERC				(17)								/* DPERC offset */
#define CFCLR_DPERC_MSK			(1 << CFCLR_DPERC)
#define CFCLR_DFERC				(18)								/* DFERC offset */
#define CFCLR_DFERC_MSK			(1 << CFCLR_DFERC)
#define CFCLR_ORERC				(24)								/* ORERC offset */
#define CFCLR_ORERC_MSK			(1 << CFCLR_ORERC)
#define CFCLR_MFFC				(26)								/* ORERC offset */
#define CFCLR_MFFC_MSK			(1 << CFCLR_MFFC)
#define CFCLR_PERC				(27)								/* PERC offset */
#define CFCLR_PERC_MSK			(1 << CFCLR_PERC)
#define CFCLR_FERC				(28)								/* FERC offset */
#define CFCLR_FERC_MSK			(1 << CFCLR_FERC)
#define CFCLR_TDREC				(29)								/* TDREC offset */
#define CFCLR_TDREC_MSK			(1 << CFCLR_TDREC)
#define CFCLR_RDRFC				(31)								/* RDRFC offset */
#define CFCLR_RDRFC_MSK			(1 << CFCLR_RDRFC)

/*
 * FCR register bit mask definitions
 */
#define FCR_TTRG				(8)									/* TTRG offset */
#define FCR_TTRG_MSK			(0xF << FCR_TTRG)
#define FCR_RTRG				(16)								/* RTRG offset */
#define FCR_RTRG_MSK			(0xF << FCR_RTRG)

#define FIFO_FULL				(16)


	.globl	console_rz_register
	.globl	console_rz_init
	.globl	console_rz_putc
	.globl	console_rz_flush

	/* -----------------------------------------------
	 * int console_rz_register(
	 *		uintptr_t base, uint32_t clk, uint32_t baud,
	 *		console_t *console)
	 * Function to initialize and register a new console.
	 * Storage passed in for the console struct
	 * *must* be persistent (i.e. not from the stack).
	 * In:	x0 - UART register base address
	 *		w1 - UART clock in Hz
	 *		w2 - Baud rate
	 *		x3 - pointer to empty console_t struct
	 * Out: return 1 on success, 0 on error
	 * Clobber list : x0, x1, x2, x6, x7, x14
	 * -----------------------------------------------
	 */
func console_rz_register
	mov	x7, x30
	mov	x6, x3
	cbz	x6, register_fail
	str	x0, [x6, #CONSOLE_T_BASE]

	bl	console_rz_init

	mov	x0, x6
	mov	x30, x7
	/* TODO: KTG: according to docs/change-log.md 'finish_console_register' is deprecated. Research usage of non-deprecated alternative */
	finish_console_register rz, putc=1, getc=0, flush=1
	b l_ret

register_fail:
	mov x0, #0

l_ret:
	ret	x7
endfunc console_rz_register

	/* -----------------------------------------------
	 * int console_rz_init(unsigned long base_addr,
	 * unsigned int uart_clk, unsigned int baud_rate)
	 * Function to initialize the console without a
	 * C Runtime to print debug information. This
	 * function will be accessed by console_rz_register
	 * and crash reporting.
	 * In: x0 - console base address
	 *	w1 - Uart clock in Hz
	 *	w2 - Baud rate
	 * Out: return 1 on success
	 * Clobber list : x3, x4
	 * -----------------------------------------------
	 */
func console_rz_init
/* Setup the CPG according to the fixed baud rate and peripheral clock */
	/* SCKCR3 default b[7:6] = b'00 :
	*
	*	ASYNC_SERIALCLK_SEL[00]=75MHz
	*/

	/* Set default values for CCR0 - Disable Rx and Tx, all Int enable functions */
	str wzr, [x0, #CCR0]
	/* Setup for TCLK = 75MHz, */
	ldr w3, [x0, #CCR3]
	and w3, w3, #~CCR3_CHCR_MSK
	orr w3, w3, #CCR3_CHCR_8BIT				/* 8-bit data length for Rx and Tx */
	and w3, w3, #~CCR3_STP_MSK				/* STP field cleared - 1 Stop Bit */
	and w3, w3, #~CCR3_MOD_MSK				/* ASYNC MODE field cleared - ASYNC selected */
	and w3, w3, #~CCR3_MP_MSK				/* Multi-processor comms field cleared - mode disabled */
	orr w3, w3, #CCR3_FM_MSK				/* FIFO mode, set to 1 */
	and w3, w3, #~CCR3_DEN_MSK				/* DEN field closed - RS-485 disabled */
	and w3, w3, #~CCR3_CKE_MSK				/* leave as '0', On-chip Baud rate generator */
	and w3, w3, #~CCR3_ACS0_MSK				/* leave as '0', External Clock Input */

	/* NOTE:
	* TCLK can be selected from PCLKM (100MHz/75MHz) or PCLKSCI0 (96MHz, initial value)
	* dependent on CCR3.BPEN of SCI0.
	* SCKCR3:DIV_SUB_SEL default is b'0 --> PCLKM=75MHz
	* (In case of BPEN=1, synchronization circuit is bypassed.
	* This requires that bus clock, PCLKM and TCLK are same clock frequency and clock source.
	* In this case the same clock is input to the bus clock, PCLK and the operation clock, TCLK))
	* TCLK
	* CCR3.BPEN=0 : PCLKSCI0 (96MHz) (initial value)
	* CCR3.BPEN=1 : PCLKM (100MHz/75MHz)
	*/
	/* Set BPEN = 1 , to bypass synchronizer circuit between the bus clock and operation clock	*/
	and w3, w3, #~CCR3_BPEN_MSK
	orr w3, w3, #CCR3_BPEN_MSK

	/* Write the modified value */
	str w3, [x0, #CCR3]

	/* Setup CCR2 for baud rate see Table 5-4 in SCI IP spec */
	udiv w4, w1, w2					/* Divide PCLK by Baudrate */
	add w4, w4, #16					/* About to divide by 32 so add a pre-rounding value of 16 (32/2) to allow integer rounding to nearest number */
	mov w3, #32
	udiv w4, w4, w3
	sub w4, w4, #1
	lsl w4, w4, #CCR2_BRR
	and w4, w4, #CCR2_BRR_MSK
	/* Set BRR value */
	ldr w3, [x0, #CCR2]
	and w3, w3, #~CCR2_BRR_MSK
	orr w3, w3, w4					/* set BRR */
	/* Clear CKS bit */
	and w3, w3, #~CCR2_CKS_MSK

	/* Write the modified value */
	str w3, [x0, #CCR2]

	/* Set default values for CCR1 - No Parity, Tx Data/ Rx Data not inverted, full duplex */
	str wzr, [x0, #CCR1]
	/* Set default values for CCR4 - No Compare Data pattern, Adjust Sampling timing etc. */
	str wzr, [x0, #CCR4]

	/* To enable FIFO for Rx setup FCR (and set FM bit in SCIO_CCR3=1) */
	ldr w3, [x0, #FCR]
	and w3, w3, #~FCR_TTRG_MSK
	orr w3, w3, #(15 << FCR_TTRG)
	and w3, w3, #~FCR_RTRG_MSK
	orr w3, w3, #(1 << FCR_RTRG)
	/* Write the modified value */
	str w3, [x0, #FCR]

	/* Clear all error flags by writing '1' to error bits and write '0' to reserved bits */
	mov w3, wzr
	orr w3, w3, #CFCLR_ORERC_MSK
	orr w3, w3, #CFCLR_PERC_MSK
	orr w3, w3, #CFCLR_FERC_MSK
	str w3, [x0, #CFCLR]

	/* Enable Rx & TX on SCI0 */
	ldr w3, [x0, #CCR0]
	/* Enable Rx */
	and w3, w3, #~CCR0_RE_MSK
	orr w3, w3, CCR0_RE_MSK				/* Enable RE */
	/* Enable Tx */
	and w3, w3, #~CCR0_TE_MSK
	orr w3, w3, CCR0_TE_MSK				/* Enable TE */
	str w3, [x0, #CCR0]

	mov	x0, #1
	ret
endfunc console_rz_init

	/* --------------------------------------------------------
	 * int console_rz_putc(int c, console_t *console)
	 * Function to output a character over the console. It
	 * returns the character printed on success or -1 on error.
	 * In : w0 - character to be printed
	 *		x1 - pointer to console_t structure
	 * Out : return -1 on error else return character.
	 * Clobber list : x2
	 * --------------------------------------------------------
	 */
func console_rz_putc
	ldr x1, [x1, #CONSOLE_T_BASE]
	cmp w0, #0xA
	/* Prepend '\r' to '\n' */
	bne 2f
1:
	/* Check if the transmit FIFO is full */
	ldrb w2, [x1, #FTSR]
	ubfx w2, w2, #FTSR_T, #FTSR_T_WIDTH
	cmp w2, #FIFO_FULL
	bge 1b
	mov w2, #0x0D
	strb w2, [x1, #TDR]
2:
	/* Check if the transmit FIFO is full */
	ldrb w2, [x1, #FTSR]
	ubfx w2, w2, #FTSR_T, #FTSR_T_WIDTH
	cmp w2, #FIFO_FULL
	bge 2b
	strb w0, [x1, #TDR]

	ret
endfunc console_rz_putc

	/* ---------------------------------------------
	 * int console_rz_flush(console_t *console)
	 * Function to force a write of all buffered
	 * data that hasn't been output. It returns 0
	 * upon successful completion, otherwise it
	 * returns -1.
	 * In : x0 - pointer to console_t structure
	 * Out : return 0 upon successful completion, otherwise -1
	 * Clobber list : x1, x2
	 * ---------------------------------------------
	 */
func console_rz_flush
	ldr x0, [x0, #CONSOLE_T_BASE]
1:
	/* Check TEND flag */
	ldr w1, [x0, #CSR]
	and w1, w1, #CSR_TEND_MSK
	mov w2, #CSR_TEND_MSK		/* Value does not fit as a literal into 'CMP' instruction so pass it via a register instead */
	cmp w1, w2
	bne 1b

	mov w0, #0
	ret
endfunc console_rz_flush
