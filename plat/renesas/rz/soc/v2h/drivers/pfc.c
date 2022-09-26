/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <pfc_regs.h>
#include <lib/mmio.h>


//#define PFC_MUX_TBL_NUM		(4)
#define PFC_QSPI_TBL_NUM	(2)
#define PFC_SD_TBL_NUM		(2)

//KTG: TODO: It looks like the main boot modes I/O pins are all hard coded so don't need enabling as pin functions. Confirm if some exotic peripheral pins need to be present like QSDx_CD, XSPI0_WP etc, in which case pin functions could be required.
#if 0
static PFC_REGS pfc_mux_reg_tbl[PFC_MUX_TBL_NUM] = {
	/* SDHI 0 */
	{
		{ PFC_ON,  (uintptr_t)PFC_PMC22,  0x03 },			/* PMC */
		{ PFC_ON,  (uintptr_t)PFC_PFC22,  0x00000011 },		/* PFC */
		{ PFC_OFF, (uintptr_t)NULL,       0 }				/* IOLH */
		{ PFC_OFF, (uintptr_t)NULL,       0 }				/* PUPD */
		{ PFC_OFF, (uintptr_t)NULL,       0 }				/* SR */
		{ PFC_OFF, (uintptr_t)NULL,       0 }				/* IEN */
	},
	/* SDHI 1) */
	{
		{ PFC_ON,  (uintptr_t)PFC_PMC23,  0x03 },			/* PMC */
		{ PFC_ON,  (uintptr_t)PFC_PFC23,  0x00000011 },		/* PFC */
		{ PFC_OFF, (uintptr_t)NULL,       0 }				/* IOLH */
		{ PFC_OFF, (uintptr_t)NULL,       0 }				/* PUPD */
		{ PFC_OFF, (uintptr_t)NULL,       0 }				/* SR */
		{ PFC_OFF, (uintptr_t)NULL,       0 }				/* IEN */
	},

	/* P38(scif0) */
	{
		{ PFC_ON,  (uintptr_t)PFC_PMC22,  0x03 },			/* PMC */
		{ PFC_ON,  (uintptr_t)PFC_PFC22,  0x00000011 },		/* PFC */
		{ PFC_OFF, (uintptr_t)NULL,       0 }				/* IOLH */
		{ PFC_OFF, (uintptr_t)NULL,       0 }				/* PUPD */
		{ PFC_OFF, (uintptr_t)NULL,       0 }				/* SR */
		{ PFC_OFF, (uintptr_t)NULL,       0 }				/* IEN */
	},
	/* P39(scif0) */
	{
		{ PFC_ON,  (uintptr_t)PFC_PMC23,  0x07 },			/* PMC */
		{ PFC_ON,  (uintptr_t)PFC_PFC23,  0x00000111 },		/* PFC */
		{ PFC_OFF, (uintptr_t)NULL,       0 }				/* IOLH */
		{ PFC_OFF, (uintptr_t)NULL,       0 }				/* PUPD */
		{ PFC_OFF, (uintptr_t)NULL,       0 }				/* SR */
		{ PFC_OFF, (uintptr_t)NULL,       0 }				/* IEN */
	}
};
#endif

static PFC_REGS  pfc_qspi_reg_tbl[PFC_QSPI_TBL_NUM] = {
	/* QSPI0 CLK (P7.0), CS0 (P7.2) */
	{
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PMC */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH07, 0x0000000000020002 },		/* IOLH */
		{ PFC_ON,  (uintptr_t)PFC_PUPD07, 0x0000000000000000 },		/* PUPD */
		{ PFC_ON,  (uintptr_t)PFC_SR07,   0x0000000000010001 },		/* SR */
		{ PFC_OFF, (uintptr_t)NULL,       0 }						/* IEN */
	},

	/* QSPI0 IO0-IO3 (P8.0 - P8.3) */
	{
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PMC */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH08, 0x0000000002020202 },		/* IOLH */
		{ PFC_ON,  (uintptr_t)PFC_PUPD08, 0x0000000000000000 },		/* PUPD */
		{ PFC_ON,  (uintptr_t)PFC_SR08,   0x0000000001010101 },		/* SR */
		{ PFC_OFF, (uintptr_t)NULL,       0 }						/* IEN */
	},
};

static PFC_REGS  pfc_sd_reg_tbl[PFC_SD_TBL_NUM] = {
	/* SD0_CLK (P9.0), SD0_CMD (P9.1), SD0_RSTN (P9.2) */
	{
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PMC */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH09, 0x0000000000020202 },		/* IOLH */
		{ PFC_ON,  (uintptr_t)PFC_PUPD09, 0x0000000000000000 },		/* PUPD */
		{ PFC_ON,  (uintptr_t)PFC_SR09,   0x0000000000010101 },		/* SR */
		{ PFC_ON,  (uintptr_t)PFC_IEN09,  0x0000000000000100 }		/* IEN */
	},

	/* SD0_DATA  (PA.0 - PA.7*/
	{
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PMC */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH0A, 0x0202020202020202 },		/* IOLH */
		{ PFC_ON,  (uintptr_t)PFC_PUPD0A, 0x0000000000000000 },		/* PUPD */
		{ PFC_ON,  (uintptr_t)PFC_SR0A,   0x0101010101010101 },		/* SR */
		{ PFC_ON,  (uintptr_t)PFC_IEN0A,  0x0101010101010101 }		/* IEN */
	},
};

#if 0
static void pfc_mux_setup(void)
{
	int      cnt;

	/* multiplexer terminal switching */
	mmio_write_32(PFC_PWPR, PWPR_REGWE_A);

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
}
#endif
static void pfc_qspi_setup(void)
{
	int      cnt;

	for (cnt = 0; cnt < PFC_QSPI_TBL_NUM; cnt++) {
		//if (SYS_LSI_OTPPOC & ??) //TODO: KTG: Do some required action to decide what IO drive to use dependat on 1.8V or 3V3 level
		{
			/* IOLH */
			if (pfc_qspi_reg_tbl[cnt].iolh.flg == PFC_ON) {
				mmio_write_64(pfc_qspi_reg_tbl[cnt].iolh.reg, pfc_qspi_reg_tbl[cnt].iolh.val);
			}
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
	// TOD0: KTG: Is there a SD Voltage option- or is it automated in V2H?
	//mmio_write_32(PFC_SD_ch0, 1);
	//mmio_write_32(PFC_SD_ch1, 0);

	//TODO: KTG: Looks like we may need to configure SD0 to be either eSD or eMMC - consider details of this later

	for (cnt = 0; cnt < PFC_SD_TBL_NUM; cnt++) {
		/* PMC */
		if (pfc_sd_reg_tbl[cnt].pmc.flg == PFC_ON) {
			mmio_write_8(pfc_sd_reg_tbl[cnt].pmc.reg, pfc_sd_reg_tbl[cnt].pmc.val);
		}
		/* PFC */
		if (pfc_sd_reg_tbl[cnt].pfc.flg == PFC_ON) {
			mmio_write_32(pfc_sd_reg_tbl[cnt].pfc.reg, pfc_sd_reg_tbl[cnt].pfc.val);
		}
		//if (SYS_LSI_OTPPOC & ??) //TODO: KTG: Do some required action to decide what IO drive to use dependat on 1.8V or 3V3 level
		{
			/* IOLH */
			if (pfc_sd_reg_tbl[cnt].iolh.flg == PFC_ON) {
				mmio_write_64(pfc_sd_reg_tbl[cnt].iolh.reg, pfc_sd_reg_tbl[cnt].iolh.val);
			}
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
	//pfc_mux_setup();	//TODO: KTG: Confirm if this is required
	pfc_qspi_setup();
	pfc_sd_setup();
}
