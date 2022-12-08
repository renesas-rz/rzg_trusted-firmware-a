/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <pfc_regs.h>
#include <lib/mmio.h>

//Todo: check if registers exist
//Todo: check all values of registers
//Todo: check Slew Rate registers exist and their values.
//Todo: Check PFC on or off

static PFC_REGS pfc_mux_reg_tbl[PFC_MUX_TBL_NUM] = {
	/* P0(sd0) - CP, WP &
	 *   (sd1) - CP, WP */
	{
		{ PFC_ON,  (uintptr_t)PFC_PMC20,  0x0F },					/* PMC */
		{ PFC_ON,  (uintptr_t)PFC_PFC20,  0x00001111 },				/* PFC */
		{ PFC_OFF, (uintptr_t)PFC_IOLH20, 0x0000000001010101 },		/* IOLH */
		{ PFC_OFF, (uintptr_t)PFC_PUPD20, 0x0000000000000000 },		/* PUPD */
		{ PFC_OFF, (uintptr_t)PFC_SR20,   0x0000000001010101 },		/* SR */
		{ PFC_OFF, (uintptr_t)NULL,       0 }						/* IEN */
	},
	/* P13(scif0) - Tx, Rx, Sck, Rts, Cts*/
	//Todo: there is a duplication in the pin assignment for SCIF0 Rx and Tx. Check which pins are correct.
	{
		{ PFC_ON,  (uintptr_t)PFC_PMC25,  0x1F },					/* PMC */
		{ PFC_ON,  (uintptr_t)PFC_PFC25,  0x00011111 },				/* PFC */
		{ PFC_OFF, (uintptr_t)PFC_IOLH25, 0x0000000101010101 },		/* IOLH */
		{ PFC_OFF, (uintptr_t)PFC_PUPD25, 0x0000000000000000 },		/* PUPD */
		{ PFC_OFF, (uintptr_t)PFC_SR25,   0x0000000101010101 },		/* SR */
		{ PFC_OFF, (uintptr_t)NULL,       0 }						/* IEN */
	}
};

static PFC_REGS  pfc_qspi_reg_tbl[PFC_QSPI_TBL_NUM] = {
	/* XSPI  - CS1, CS0, DS, WP, RESET */
	{
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PMC */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH04, 0x0000000000020202 },		/* IOLH */
		{ PFC_ON,  (uintptr_t)PFC_PUPD04, 0x0000000000000000 },		/* PUPD */
		{ PFC_ON,  (uintptr_t)PFC_SR04,   0x0000000000010000 },		/* SR */
		{ PFC_OFF, (uintptr_t)NULL,       0 }						/* IEN */
	},
	/* XSPI - IO7 - IO0*/
	{
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PMC */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH05, 0x0000020202020202 },		/* IOLH */
		{ PFC_ON,  (uintptr_t)PFC_PUPD05, 0x0000000000000000 },		/* PUPD */
		{ PFC_ON,  (uintptr_t)PFC_SR05,   0x0000010101010101 },		/* SR */
		{ PFC_OFF, (uintptr_t)NULL,       0 }						/* IEN */
	}

};

static PFC_REGS  pfc_sd_reg_tbl[PFC_SD_TBL_NUM] = {
	/* SD0_CMD*/
	{
		{ PFC_OFF,	(uintptr_t)PFC_PMC10,  0x02 },					/* PMC */
		{ PFC_OFF,	(uintptr_t)PFC_PFC10,  0x00000002 },			/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH10, 0x0000000000000200 },	/* IOLH */
		{ PFC_ON,  	(uintptr_t)PFC_PUPD10, 0x0000000000000000 },	/* PUPD */
		{ PFC_ON,	(uintptr_t)PFC_SR10,   0x0000000000000100 },	/* SR */
		{ PFC_ON,	(uintptr_t)PFC_IEN10,  0x0000000000000100 }		/* IEN */
	},
	/* SD0 DATA7 - DATA0 */
	{
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PMC */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH11, 0x0202020202020202 },		/* IOLH */
		{ PFC_ON,  (uintptr_t)PFC_PUPD11, 0x0000000000000000 },		/* PUPD */
		{ PFC_ON,  (uintptr_t)PFC_SR11,   0x0101010101010101 },		/* SR */
		{ PFC_ON,  (uintptr_t)PFC_IEN11,  0x0101010101010101 }		/* IEN */
	},
	/* SD1_CMD */
	{
		{ PFC_OFF,  (uintptr_t)PFC_PMC12,  0x02 },					/* PMC */
		{ PFC_OFF,  (uintptr_t)PFC_PFC12,  0x00000002 },				/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH12, 0x0000000000000200 },		/* IOLH */
		{ PFC_ON,  (uintptr_t)PFC_PUPD12, 0x0000000000000000 },		/* PUPD */
		{ PFC_ON,  (uintptr_t)PFC_SR12,   0x0000000000000100 },		/* SR */
		{ PFC_ON,  (uintptr_t)PFC_IEN12,  0x0000000000000100 }		/* IEN */
	},
	/* SD1 DATA7 - DATA0 */
	{
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PMC */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH13, 0x0202020202020202 },		/* IOLH */
		{ PFC_ON,  (uintptr_t)PFC_PUPD13, 0x0000000000000000 },		/* PUPD */
		{ PFC_ON,  (uintptr_t)PFC_SR13,   0x0101010101010101 },		/* SR */
		{ PFC_ON,  (uintptr_t)PFC_IEN13,  0x0101010101010101 }		/* IEN */
	}
};

static void pfc_mux_setup(void)
{
	int      cnt;

	/* multiplexer terminal switching */
	mmio_write_32(PFC_PWPR, 0x0);
	mmio_write_32(PFC_PWPR, PWPR_PFCWE);

	for (cnt = 0; cnt < PFC_MUX_TBL_NUM; cnt++) {
		/* PMC */
		if (pfc_mux_reg_tbl[cnt].pmc.flg == PFC_ON) {
			mmio_write_8(pfc_mux_reg_tbl[cnt].pmc.reg, pfc_mux_reg_tbl[cnt].pmc.val);
		}
		/* PFC */
		if (pfc_mux_reg_tbl[cnt].pfc.flg == PFC_ON) {
			mmio_write_32(pfc_mux_reg_tbl[cnt].pfc.reg, pfc_mux_reg_tbl[cnt].pfc.val);
		}
		/* IOLH */
		if (pfc_mux_reg_tbl[cnt].iolh.flg == PFC_ON) {
			mmio_write_64(pfc_mux_reg_tbl[cnt].iolh.reg, pfc_mux_reg_tbl[cnt].iolh.val);
		}
		/* PUPD */
		if (pfc_mux_reg_tbl[cnt].pupd.flg == PFC_ON) {
			mmio_write_64(pfc_mux_reg_tbl[cnt].pupd.reg, pfc_mux_reg_tbl[cnt].pupd.val);
		}
		/* SR */
		if (pfc_mux_reg_tbl[cnt].sr.flg == PFC_ON) {
			mmio_write_64(pfc_mux_reg_tbl[cnt].sr.reg, pfc_mux_reg_tbl[cnt].sr.val);
		}
	}

	mmio_write_32(PFC_PWPR, 0x0);
	mmio_write_32(PFC_PWPR, PWPR_B0Wl);
}

static void pfc_qspi_setup(void)
{
	int      cnt;

	for (cnt = 0; cnt < PFC_QSPI_TBL_NUM; cnt++) {
		/* IOLH */
		if (pfc_qspi_reg_tbl[cnt].iolh.flg == PFC_ON) {
			mmio_write_64(pfc_qspi_reg_tbl[cnt].iolh.reg, pfc_qspi_reg_tbl[cnt].iolh.val);
		}
		/* PUPD */
		if (pfc_qspi_reg_tbl[cnt].pupd.flg == PFC_ON) {
			mmio_write_64(pfc_qspi_reg_tbl[cnt].pupd.reg, pfc_qspi_reg_tbl[cnt].pupd.val);
		}
		/* SR */
		if (pfc_qspi_reg_tbl[cnt].sr.flg == PFC_ON) {
			mmio_write_64(pfc_qspi_reg_tbl[cnt].sr.reg, pfc_qspi_reg_tbl[cnt].sr.val);
		}
	}
}

static void pfc_sd_setup(void)
{
	int      cnt;

	/* Since SDx is 3.3V, the initial value will be set. */
	mmio_write_32(PFC_SD_ch0, 1);
	mmio_write_32(PFC_SD_ch1, 0);

	for (cnt = 0; cnt < PFC_SD_TBL_NUM; cnt++) {
		/* PMC */
		if (pfc_sd_reg_tbl[cnt].pmc.flg == PFC_ON) {
			mmio_write_8(pfc_sd_reg_tbl[cnt].pmc.reg, pfc_sd_reg_tbl[cnt].pmc.val);
		}
		/* PFC */
		if (pfc_sd_reg_tbl[cnt].pfc.flg == PFC_ON) {
			mmio_write_32(pfc_sd_reg_tbl[cnt].pfc.reg, pfc_sd_reg_tbl[cnt].pfc.val);
		}
		/* IOLH */
		if (pfc_sd_reg_tbl[cnt].iolh.flg == PFC_ON) {
			mmio_write_64(pfc_sd_reg_tbl[cnt].iolh.reg, pfc_sd_reg_tbl[cnt].iolh.val);
		}
		/* PUPD */
		if (pfc_sd_reg_tbl[cnt].pupd.flg == PFC_ON) {
			mmio_write_64(pfc_sd_reg_tbl[cnt].pupd.reg, pfc_sd_reg_tbl[cnt].pupd.val);
		}
		/* SR */
		if (pfc_sd_reg_tbl[cnt].sr.flg == PFC_ON) {
			mmio_write_64(pfc_sd_reg_tbl[cnt].sr.reg, pfc_sd_reg_tbl[cnt].sr.val);
		}
		/* IEN */
		if (pfc_sd_reg_tbl[cnt].ien.flg == PFC_ON) {
			mmio_write_64(pfc_sd_reg_tbl[cnt].ien.reg, pfc_sd_reg_tbl[cnt].ien.val);
		}
	}
}

void pfc_setup(void)
{
	pfc_mux_setup();
	pfc_qspi_setup();
	pfc_sd_setup();
}
