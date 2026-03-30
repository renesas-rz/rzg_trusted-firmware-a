/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <pfc_regs.h>
#include <sys_regs.h>
#include <sys.h>
#include <lib/mmio.h>

static const pfc_regs_t pfc_sd_reg_tbl[PFC_SD_TBL_NUM] = {
	/* SD0_CLK (P09.0), SD0_CMD (P09.1), SD0_RSTN (P09.2), SD0_DS (P09.5) */
	{
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PMC */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH09,	0x0000030000030303 },	/* IOLH */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PUPD */
		{ PFC_ON,	(uintptr_t)PFC_IEN09,	0x0000010000000100 }	/* IEN */
	},
	/* SD0_D0 (P0A.0), SD0_D1 (P0A.1), SD0_D2 (P0A.2), SD0_D3 (P0A.3), SD0_D4 (P0A.4), SD0_D5 (P0A.5), SD0_D6 (P0A.6), SD0_D7 (P0A.7) */
	{
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PMC */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH0A,	0x0303030303030303 },	/* IOLH */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PUPD */
		{ PFC_ON,	(uintptr_t)PFC_IEN0A,	0x0101010101010101 }	/* IEN */
	}
};

static const pfc_regs_t pfc_scif_reg_tbl[PFC_SCIF_TBL_NUM] = {
	/* SCIF0_RX (P06.0), SCIF0_TX (P06.1) */
	{
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PMC */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH06,	0x0000000000000303 },	/* IOLH */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PUPD */
		{ PFC_OFF,	(uintptr_t)NULL,		0 }						/* IEN */
	}
};

static const pfc_regs_t  pfc_xspi_reg_tbl[PFC_XSPI_TBL_NUM] = {
	/* XSPI_CKP (P35.0), XSPI_CS0 (P35.1), XSPI_CS1 (P35.2), XSPI_DS (P35.3), XSPI_RESET (P35.4),*/
	{
		{ PFC_ON,	(uintptr_t)PFC_PMC35,	0x1F },					/* PMC */
		{ PFC_ON,	(uintptr_t)PFC_PFC35,	0x0 },					/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH35,	0x0000000303030303 },	/* IOLH */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PUPD */
		{ PFC_OFF,	(uintptr_t)NULL,		0 }						/* IEN */
	},
	/* XSPI_IO0 (P36.0),  XSPI_IO1 (P36.1),  XSPI_IO2 (P36.2), XSPI_IO3 (P36.3),  XSPI_IO4 (P36.4),  XSPI_IO5 (P36.5),  XSPI_IO6 (P36.6),  XSPI_IO7 (P36.7) */
	{
		{ PFC_ON,	(uintptr_t)PFC_PMC36,	0xFF },					/* PMC */
		{ PFC_ON,	(uintptr_t)PFC_PFC36,	0x0 },					/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH36,	0x0303030303030303 },	/* IOLH */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PUPD */
		{ PFC_OFF,	(uintptr_t)NULL,		0 }						/* IEN */
	}
};


#if PLAT_SYSTEM_SUSPEND
/* RIIC0 */
static const pfc_regs_t pfc_i2c0_reg_tbl[PFC_RIIC_TBL_NUM] = {
	{
		{ PFC_ON,   (uintptr_t)PFC_PMC35, 0x0C },					/* PMC */
		{ PFC_ON,   (uintptr_t)PFC_PFC35, 0x00004400 },				/* PFC */
		{ PFC_OFF,  (uintptr_t)NULL,      0 },						/* IOLH */
		{ PFC_OFF,  (uintptr_t)NULL,      0 },						/* PUPD */
		{ PFC_OFF,  (uintptr_t)NULL,      0 }						/* IEN */
	}
};
#endif /* PLAT_SYSTEM_SUSPEND */

static void pfc_write_registers(uint8_t tbl_size, const pfc_regs_t *pfc_reg_tbl)
{
	for (uint8_t cnt = 0; cnt < tbl_size; cnt++) {
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

		/* IEN */
		if (pfc_reg_tbl[cnt].ien.flg == PFC_ON) {
			mmio_write_64(pfc_reg_tbl[cnt].ien.reg, pfc_reg_tbl[cnt].ien.val);
		}
	}
}

static void pfc_scif_setup(void)
{
	pfc_write_registers(PFC_SCIF_TBL_NUM, pfc_scif_reg_tbl);
}

void pfc_xspi_setup(void)
{
	/* Set OEN of XSPI Multi/OctaRAM IO block. */
	mmio_write_32(PFC_XSPI_OEN, XSPI_OEN_SORST_N);

	mmio_write_32(PFC_PWPR, PWPR_B0WI_ENABLE_PFCWE | PWPR_PFCWE_DISABLE);
	mmio_write_32(PFC_PWPR, PWPR_PFCWE_ENABLE);

	pfc_write_registers(PFC_XSPI_TBL_NUM, pfc_xspi_reg_tbl);

	mmio_write_32(PFC_PWPR, PWPR_B0WI_ENABLE_PFCWE | PWPR_PFCWE_DISABLE);
	mmio_write_32(PFC_PWPR, PWPR_B0WI_DISABLE_PFCWE);

	mmio_write_32(PFC_XSPI_VOLT_CTL, QSPI_PVDD_1V8);
}

static void pfc_sd_setup(void)
{
	/* Since SDx is 3.3V, the initial value will be set. */
	mmio_write_32(PFC_SD_ch0, 1);

	pfc_write_registers(PFC_SD_TBL_NUM, pfc_sd_reg_tbl);
}

#if PLAT_SYSTEM_SUSPEND
void pfc_riic_pmic_setup(void)
{
	mmio_write_32(PFC_PWPR, PWPR_B0WI_ENABLE_PFCWE | PWPR_PFCWE_DISABLE);
	mmio_write_32(PFC_PWPR, PWPR_PFCWE_ENABLE);

	for (uint8_t cnt = 0; cnt < PFC_RIIC_TBL_NUM; cnt++) {
		/* PFC */
		if (pfc_i2c0_reg_tbl[cnt].pfc.flg == PFC_ON) {
			mmio_write_32(pfc_i2c0_reg_tbl[cnt].pfc.reg, pfc_i2c0_reg_tbl[cnt].pfc.val);
		}
		/* PMC */
		if (pfc_i2c0_reg_tbl[cnt].pmc.flg == PFC_ON) {
			mmio_write_8(pfc_i2c0_reg_tbl[cnt].pmc.reg, pfc_i2c0_reg_tbl[cnt].pmc.val);
		}
	}

	mmio_write_32(PFC_PWPR, PWPR_B0WI_ENABLE_PFCWE | PWPR_PFCWE_DISABLE);
	mmio_write_32(PFC_PWPR, PWPR_B0WI_DISABLE_PFCWE);

}
#endif /* PLAT_SYSTEM_SUSPEND */

static const pfc_regs_t *pfc_boot_mode_tbls[SYS_BOOT_MODE_MAX] = {
	pfc_sd_reg_tbl,
	pfc_sd_reg_tbl,
	pfc_sd_reg_tbl,
	pfc_xspi_reg_tbl,
	pfc_xspi_reg_tbl,
	pfc_scif_reg_tbl
};

/* different order here. */
static const uint8_t pfc_boot_mode_tbl_len[SYS_BOOT_MODE_MAX] = {
	PFC_SD_TBL_NUM,
	PFC_SD_TBL_NUM,
	PFC_SD_TBL_NUM,
	PFC_XSPI_TBL_NUM,
	PFC_XSPI_TBL_NUM,
	PFC_SCIF_TBL_NUM
};

static void pfc_drive_setup(void)
{
	/* Get the boot mode */
	int16_t boot_mode = sys_get_boot_mode();

	if (boot_mode < SYS_BOOT_MODE_MAX) {
		const pfc_regs_t *p_pins_tbl = pfc_boot_mode_tbls[boot_mode];
		uint8_t tbl_len = pfc_boot_mode_tbl_len[boot_mode];

		for (uint8_t cnt = 0; cnt < tbl_len; cnt++) {
			if (p_pins_tbl[cnt].iolh.flg == PFC_ON) {
				/* Write IOLH value from pfc_sd_reg_tbl[] masked with value in pin table */
				mmio_write_64(p_pins_tbl[cnt].iolh.reg, p_pins_tbl[cnt].iolh.val);
			}
		}
	}
}

void pfc_setup(void)
{
	pfc_scif_setup();
	pfc_xspi_setup();
	pfc_sd_setup();
	pfc_drive_setup();
}
