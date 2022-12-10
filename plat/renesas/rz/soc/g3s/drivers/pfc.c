/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <pfc_regs.h>
#include <lib/mmio.h>

//TODO: Check Slew Rate registers exist and their values are the same as the G2L.
//TODO: SD1 and SD0 both required? ESD and EMC different ports?
//TODO: Check IOLH drive level. G2L values used.

//64 vs 32 bit registers
//1.8 vs 3.3?

static PFC_REGS pfc_mux_sd_reg_tbl[PFC_MUX_SD_TBL_NUM] = {
	/* P0(sd0) CP, WP - (sd1) CP, WP */
	{
		{ PFC_ON,	(uintptr_t)PFC_PMC20,	0x0F },					/* PMC */
		{ PFC_ON,	(uintptr_t)PFC_PFC20,	0x00001111 },			/* PFC */
		{ PFC_OFF,	(uintptr_t)PFC_IOLH20,	0x0000000001010101 },	/* IOLH */
		{ PFC_OFF,	(uintptr_t)PFC_PUPD20,	0x0000000000000000 },	/* PUPD */
		{ PFC_OFF, 	(uintptr_t)PFC_SR20,	0x0000000001010101 },	/* SR */
		{ PFC_OFF,	(uintptr_t)NULL,		0 }						/* IEN */
	}
};

static PFC_REGS pfc_mux_scif_reg_tbl[PFC_MUX_SD_TBL_NUM] = {
	/* P13(scif0) - Tx, Rx, Sck, Rts, Cts*/
	//Todo: there is a duplication in the pin assignment for SCIF0 Rx and Tx. Check which pins are correct.
	{
		{ PFC_ON,	(uintptr_t)PFC_PMC25,	0x1F },					/* PMC */
		{ PFC_ON,	(uintptr_t)PFC_PFC25,	0x00011111 },			/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH25,	0x0000000101010101 },	/* IOLH */
		{ PFC_ON,	(uintptr_t)PFC_PUPD25,	0x0000000000000000 },	/* PUPD */
		{ PFC_ON,	(uintptr_t)PFC_SR25,	0x0000000101010101 },	/* SR */
		{ PFC_OFF,	(uintptr_t)NULL,		0 }						/* IEN */
	}
};

static PFC_REGS  pfc_qspi_reg_tbl[PFC_QSPI_TBL_NUM] = {
	/* XSPI  - CS1, CS0, DS, WP, RESET */
	{
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PMC */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH04,	0x0000000000020202 },	/* IOLH */
		{ PFC_ON,	(uintptr_t)PFC_PUPD04,	0x0000000000000000 },	/* PUPD */
		{ PFC_ON,	(uintptr_t)PFC_SR04,	0x0000000000010000 },	/* SR */
		{ PFC_OFF,	(uintptr_t)NULL,		0 }						/* IEN */
	},
	/* XSPI - IO7 - IO0*/
	{
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PMC */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH05,	0x0000020202020202 },	/* IOLH */
		{ PFC_ON,	(uintptr_t)PFC_PUPD05,	0x0000000000000000 },	/* PUPD */
		{ PFC_ON,	(uintptr_t)PFC_SR05,	0x0000010101010101 },	/* SR */
		{ PFC_OFF,	(uintptr_t)NULL,		0 }						/* IEN */
	}
};

static PFC_REGS  pfc_sd_reg_tbl[PFC_SD_TBL_NUM] = {
	/* SD0 RST, CMD, CLK*/
	{
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PMC */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH10,	0x0000000000020202 },	/* IOLH */
		{ PFC_ON,	(uintptr_t)PFC_PUPD10,	0x0000000000000000 },	/* PUPD */
		{ PFC_ON,	(uintptr_t)PFC_SR10,	0x0000000000010101 },	/* SR */
		{ PFC_ON,	(uintptr_t)PFC_IEN10,	0x0000000000000100 }	/* IEN */
	},
	/* SD0 DATA7 - DATA0 */
	{
		{ PFC_OFF, (uintptr_t)NULL,			0 },					/* PMC */
		{ PFC_OFF, (uintptr_t)NULL,			0 },					/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH11,	0x0202020202020202 },	/* IOLH */
		{ PFC_ON,  (uintptr_t)PFC_PUPD11,	0x0000000000000000 },	/* PUPD */
		{ PFC_ON,  (uintptr_t)PFC_SR11,		0x0101010101010101 },	/* SR */
		{ PFC_ON,  (uintptr_t)PFC_IEN11,	0x0101010101010101 }	/* IEN */
	},
	/* SD1 CMD, CLK */
	{
		{ PFC_OFF,	(uintptr_t)NULL, 		0 },					/* PMC */
		{ PFC_OFF,	(uintptr_t)NULL, 		0 },					/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH12,	0x0000000000000202 },	/* IOLH */
		{ PFC_ON,	(uintptr_t)PFC_PUPD12,	0x0000000000000000 },	/* PUPD */
		{ PFC_ON,	(uintptr_t)PFC_SR12,	0x0000000000000101 },	/* SR */
		{ PFC_ON,	(uintptr_t)PFC_IEN12,	0x0000000000000100 }	/* IEN */
	},
	/* SD1 DATA7 - DATA0 */
	{
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PMC */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH13,	0x0202020202020202 },	/* IOLH */
		{ PFC_ON,	(uintptr_t)PFC_PUPD13,	0x0000000000000000 },	/* PUPD */
		{ PFC_ON,	(uintptr_t)PFC_SR13,	0x0101010101010101 },	/* SR */
		{ PFC_ON,	(uintptr_t)PFC_IEN13,	0x0101010101010101 }	/* IEN */
	}
};

static void pfc_write_registers(uint8_t tbl_size, PFC_REGS* pfc_reg_tbl)
{
	int cnt;

	for (cnt = 0; cnt < tbl_size; cnt++) {
		/* PMC */
		if (pfc_reg_tbl[cnt].pmc.flg == PFC_ON) {
			mmio_write_8(pfc_reg_tbl[cnt].pmc.reg, pfc_reg_tbl[cnt].pmc.val);
		}
		/* PFC */
		if (pfc_reg_tbl[cnt].pfc.flg == PFC_ON) {
			mmio_write_32(pfc_reg_tbl[cnt].pfc.reg, pfc_reg_tbl[cnt].pfc.val);
		}
		/* IOLH */
		if (pfc_reg_tbl[cnt].iolh.flg == PFC_ON) {
			mmio_write_64(pfc_reg_tbl[cnt].iolh.reg, pfc_reg_tbl[cnt].iolh.val);
		}
		/* PUPD */
		if (pfc_reg_tbl[cnt].pupd.flg == PFC_ON) {
			mmio_write_64(pfc_reg_tbl[cnt].pupd.reg, pfc_reg_tbl[cnt].pupd.val);
		}
		/* SR */
		if (pfc_reg_tbl[cnt].sr.flg == PFC_ON) {
			mmio_write_64(pfc_reg_tbl[cnt].sr.reg, pfc_reg_tbl[cnt].sr.val);
		}
		/* IEN */
		if (pfc_reg_tbl[cnt].ien.flg == PFC_ON) {
			mmio_write_64(pfc_reg_tbl[cnt].ien.reg, pfc_reg_tbl[cnt].ien.val);
		}
	}
}

static void pfc_scif_setup(void)
{
	/* multiplexer terminal switching */
	mmio_write_32(PFC_PWPR, 0x0);
	mmio_write_32(PFC_PWPR, PWPR_PFCWE);

	pfc_write_registers(PFC_MUX_SCIF_TBL_NUM, pfc_mux_scif_reg_tbl);

	mmio_write_32(PFC_PWPR, 0x0);
	mmio_write_32(PFC_PWPR, PWPR_B0Wl);
}

static void pfc_qspi_setup(void)
{
	pfc_write_registers(PFC_QSPI_TBL_NUM, pfc_qspi_reg_tbl);
}

static void pfc_sd_setup(void)
{
	/* Since SDx is 3.3V, the initial value will be set. */
	mmio_write_32(PFC_SD_ch0, 1);
	mmio_write_32(PFC_SD_ch1, 0);

	pfc_write_registers(PFC_SD_TBL_NUM, pfc_sd_reg_tbl);

	/* multiplexer terminal switching */
	mmio_write_32(PFC_PWPR, 0x0);
	mmio_write_32(PFC_PWPR, PWPR_PFCWE);

	pfc_write_registers(PFC_MUX_SD_TBL_NUM, pfc_mux_sd_reg_tbl);

	mmio_write_32(PFC_PWPR, 0x0);
	mmio_write_32(PFC_PWPR, PWPR_B0Wl);
}

void pfc_setup(void)
{
	pfc_scif_setup();
	pfc_qspi_setup();
	pfc_sd_setup();
}
