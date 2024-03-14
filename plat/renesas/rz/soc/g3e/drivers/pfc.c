/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <pfc_regs.h>
#include <sys_regs.h>
#include <sys.h>
#include <lib/mmio.h>

#define PFC_SCIF_TBL_NUM	(1)
#define PFC_XSPI_TBL_NUM	(3)
#define PFC_SD_TBL_NUM		(2)
#define PFC_USB_TBL_NUM		(1)


static void pfc_write_registers(uint8_t tbl_size, PFC_REGS *pfc_reg_tbl)
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

/* SDHI 0 */
static PFC_REGS pfc_sd_reg_tbl[PFC_SD_TBL_NUM] = {
	/* SD0_CLK (P9.0), SD0_CMD (P9.1), SD0_RSTN (P9.2), SD0_PWEN (P9.3), SD0_IOVS (P9.4) */
	{
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PMC */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH09, 0x0000000000030303 },		/* IOLH */
		{ PFC_ON,  (uintptr_t)PFC_PUPD09, 0x0000000000000000 },		/* PUPD */
		{ PFC_ON,  (uintptr_t)PFC_SR09,   0x0000000000010101 },		/* SR */
		{ PFC_ON,  (uintptr_t)PFC_IEN09,  0x0000000000000100 }		/* IEN */
	},

	/* SD0_DATA  (PA.0 - PA.7) */
	{
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PMC */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH0A, 0x0303030303030303 },		/* IOLH */
		{ PFC_ON,  (uintptr_t)PFC_PUPD0A, 0x0000000000000000 },		/* PUPD */
		{ PFC_ON,  (uintptr_t)PFC_SR0A,   0x0101010101010101 },		/* SR */
		{ PFC_ON,  (uintptr_t)PFC_IEN0A,  0x0101010101010101 }		/* IEN */
	}
};

/* TODO: Setup USB 2.0 pins when manual is updated with these values.*/
/* USB 2.0 Ch0 */
static PFC_REGS pfc_usb_reg_tbl[PFC_USB_TBL_NUM] = {
	{
		{ PFC_OFF, (uintptr_t)NULL,       0 },		/* PMC */
		{ PFC_OFF, (uintptr_t)NULL,       0 },		/* PFC */
		{ PFC_OFF, (uintptr_t)NULL,       0 },		/* IOLH */
		{ PFC_OFF, (uintptr_t)NULL,       0 },		/* PUPD */
		{ PFC_OFF, (uintptr_t)NULL,       0 },		/* SR */
		{ PFC_OFF, (uintptr_t)NULL,       0 }		/* IEN */
	}
};

/*
 * TODO: The values here are untested, they should be verified.
 * The config for XSPI0's data lines was copied from the BootROM.
 * The config for Chip0's control lines (35) was copied from the BootROM.
 * The config for Chip1's control lines (27) was deduced from whats done for chip 0.
 */
static PFC_REGS  pfc_xspi_reg_tbl[PFC_XSPI_TBL_NUM] = {
	/*
	 * Chip 1 control lines
	 * -, XSPI0_INT1N (P76), -, XSPI0_RESET1N (P74), XSPI0_CS1N (P73), -, -, -
	 */
	{
		{ PFC_OFF, (uintptr_t)PFC_PMC27,  0x58 },					/* PMC */
		{ PFC_OFF, (uintptr_t)PFC_PFC27,  0x0A0AA000 },				/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH27, 0x0003000303000000 },		/* IOLH */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PUPD */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* SR */
		{ PFC_OFF, (uintptr_t)NULL,       0 }						/* IEN */
	},
	/*
	 * Chip 0 control lines
	 * -, -, -, XSPI0_RESET0N (PL4), XSPI0_DS (PL3), - , XSPI0_CS0N (PL1), XSPI0_CKP (PL0)
	 */
	{
		{ PFC_ON,  (uintptr_t)PFC_PMC35,  0xFF },					/* PMC */
		{ PFC_ON,  (uintptr_t)PFC_PFC35,  0x00000000 },				/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH35, 0x0303030303030303 },		/* IOLH */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PUPD */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* SR */
		{ PFC_OFF, (uintptr_t)NULL,       0 }						/* IEN */
	},

	/*
	 * XSPI0 data lines
	 * XSPI0 IO7 - IO0 (PM.7 - PM.0)
	 */
	{
		{ PFC_ON,  (uintptr_t)PFC_PMC36,  0xFF },					/* PMC */
		{ PFC_ON,  (uintptr_t)PFC_PFC36,  0x00000000 },				/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH36, 0x0303030303030303 },		/* IOLH */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PUPD */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* SR */
		{ PFC_OFF, (uintptr_t)NULL,       0 }						/* IEN */
	},
};

/* SCIF */
static PFC_REGS pfc_scif_reg_tbl[PFC_SCIF_TBL_NUM] = {
	/* SCIF_RXD (P6.0), SCIF_TXD (P6.1) */
	{
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PMC */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH06, 0x0000000000000003 },		/* IOLH */
		{ PFC_ON,  (uintptr_t)PFC_PUPD06, 0x0000000000000000 },		/* PUPD */
		{ PFC_ON,  (uintptr_t)PFC_SR06,   0x0000000000000003 },		/* SR */
		{ PFC_ON,  (uintptr_t)NULL,       0 }						/* IEN */
	}
};

static void pfc_sd_setup(void)
{
	pfc_write_registers(PFC_SD_TBL_NUM, pfc_sd_reg_tbl);
}

static void pfc_xspi_setup(void)
{
	mmio_write_32(PFC_PWPR, PWPR_PFC_WE_A);
	pfc_write_registers(PFC_XSPI_TBL_NUM, pfc_xspi_reg_tbl);
	mmio_write_32(PFC_PWPR, 0x0);
}

static void pfc_usb_setup(void)
{
	pfc_write_registers(PFC_USB_TBL_NUM, pfc_usb_reg_tbl);
}

static void pfc_scif_setup(void)
{
	pfc_write_registers(PFC_SCIF_TBL_NUM, pfc_scif_reg_tbl);
}

static const uint8_t pfc_boot_mode_tbl_len[SYS_BOOT_MODE_MAX] = {
	PFC_SD_TBL_NUM,
	PFC_SD_TBL_NUM,
	PFC_SD_TBL_NUM,
	PFC_XSPI_TBL_NUM,
	PFC_XSPI_TBL_NUM,
	PFC_SCIF_TBL_NUM,
	PFC_USB_TBL_NUM
};

static const PFC_REGS *pfc_boot_mode_tbls[SYS_BOOT_MODE_MAX] = {
	pfc_sd_reg_tbl,
	pfc_sd_reg_tbl,
	pfc_sd_reg_tbl,
	pfc_xspi_reg_tbl,
	pfc_xspi_reg_tbl,
	pfc_scif_reg_tbl,
	pfc_usb_reg_tbl
};

static void pfc_drive_setup(void)
{
	/* Get the boot mode */
	int16_t boot_mode = sys_get_boot_mode();

	if (boot_mode < SYS_BOOT_MODE_MAX) {
		const PFC_REGS *p_pins_tbl = pfc_boot_mode_tbls[boot_mode];
		uint8_t tbl_len = pfc_boot_mode_tbl_len[boot_mode];

		int cnt;

		for (cnt = 0; cnt < tbl_len; cnt++) {
			if (p_pins_tbl[cnt].iolh.flg == PFC_ON) {
				/* Write IOLH value from pfc_sd_reg_tbl[] masked with value in pin table */
				mmio_write_64(p_pins_tbl[cnt].iolh.reg, p_pins_tbl[cnt].iolh.val);
			}
		}
	}
}

void pfc_setup(void)
{
	pfc_sd_setup();
	pfc_xspi_setup();
	pfc_usb_setup();
	pfc_scif_setup();
	pfc_drive_setup();
}
