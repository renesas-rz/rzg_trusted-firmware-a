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


#define PFC_TBL_LEN						(2)


/* SDHI 0 */
static PFC_REGS pfc_sd_reg_tbl[PFC_TBL_LEN] = {
	/* SD0_CLK (P9.0), SD0_CMD (P9.1), SD0_RSTN (P9.2) */
	{
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PMC */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH09, 0x0000000000030303 },		/* IOLH */
		{ PFC_ON,  (uintptr_t)PFC_PUPD09, 0x0000000000000000 },		/* PUPD */
		{ PFC_ON,  (uintptr_t)PFC_SR09,   0x0000000000010101 },		/* SR */
		{ PFC_ON,  (uintptr_t)PFC_IEN09,  0x0000000000000100 }		/* IEN */
	},

	/* SD0_DATA (PA.0 - PA.7 */
	{
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PMC */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH0A, 0x0303030303030303 },		/* IOLH */
		{ PFC_ON,  (uintptr_t)PFC_PUPD0A, 0x0000000000000000 },		/* PUPD */
		{ PFC_ON,  (uintptr_t)PFC_SR0A,   0x0101010101010101 },		/* SR */
		{ PFC_ON,  (uintptr_t)PFC_IEN0A,  0x0101010101010101 }		/* IEN */
	},
};

static PFC_REGS pfc_qspi_reg_tbl[PFC_TBL_LEN] = {
	/* QSPI0 CLK (P7.0), CS0 (P7.2) */
	{
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PMC */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH07, 0x0000000000030003 },		/* IOLH */
		{ PFC_ON,  (uintptr_t)PFC_PUPD07, 0x0000000000000000 },		/* PUPD */
		{ PFC_ON,  (uintptr_t)PFC_SR07,   0x0000000000010001 },		/* SR */
		{ PFC_OFF, (uintptr_t)NULL,       0 }						/* IEN */
	},

	/* QSPI0 IO0-IO3 (P8.0 - P8.3) */
	{
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PMC */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH08, 0x0000000003030303 },		/* IOLH */
		{ PFC_ON,  (uintptr_t)PFC_PUPD08, 0x0000000000000000 },		/* PUPD */
		{ PFC_ON,  (uintptr_t)PFC_SR08,   0x0000000001010101 },		/* SR */
		{ PFC_OFF, (uintptr_t)NULL,       0 }						/* IEN */
	},
};

/* SCIF */
static PFC_REGS pfc_scif_reg_tbl[PFC_TBL_LEN] = {
	/* SCIF_RXD (P6.0), SCIF_TXD (P6.1) */
	{
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PMC */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PFC */
		{ PFC_ON,  (uintptr_t)PFC_IOLH06, 0x0000000000000003 },		/* IOLH */
		{ PFC_ON,  (uintptr_t)PFC_PUPD06, 0x0000000000000000 },		/* PUPD */
		{ PFC_ON,  (uintptr_t)PFC_SR06,   0x0000000000000003 },		/* SR */
		{ PFC_ON,  (uintptr_t)NULL,       0x0000000000000000 }		/* IEN */
	},

	/* Padding to make same length as other pin tables */
	{
		{0}
	},
};

#if PLAT_SYSTEM_SUSPEND
/* I2C8 */
static PFC_REGS pfc_i2c_bus8_reg_tbl[PFC_TBL_LEN] = {
	/* I2C8_SDA (P20.6), I2C8_SCL (P20.7) */
	{
		{ PFC_ON,  (uintptr_t)PFC_PMC20,  0xC0 },					/* PMC */
		{ PFC_ON,  (uintptr_t)PFC_PFC20,  0x11000000 },				/* PFC */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* IOLH */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* PUPD */
		{ PFC_OFF, (uintptr_t)NULL,       0 },						/* SR */
		{ PFC_OFF, (uintptr_t)NULL,       0 }						/* IEN */
	},

	/* Padding to make same length as other pin tables */
	{
		{0}
	},
};
#endif /* PLAT_SYSTEM_SUSPEND */

static PFC_IO_DRIVE pfc_io_drive[SYS_BOOT_MODE_MAX] = {
	{SYS_LSI_OTPPOC_EN_SD_DS_MASK,		SYS_LSI_OTPPOC_SD_E_MASK,		SYS_LSI_OTPPOC_SD_E_OFFSET},
	{SYS_LSI_OTPPOC_EN_EMMC18_DS_MASK,	SYS_LSI_OTPPOC_EMMC18_E_MASK,	SYS_LSI_OTPPOC_EMMC18_E_OFFSET},
	{SYS_LSI_OTPPOC_EN_EMMC33_DS_MASK,	SYS_LSI_OTPPOC_EMMC33_E_MASK,	SYS_LSI_OTPPOC_EMMC33_E_OFFSET},
	{SYS_LSI_OTPPOC_EN_SPI18_DS_MASK,	SYS_LSI_OTPPOC_SPI18_E_MASK,	SYS_LSI_OTPPOC_SPI18_E_OFFSET},
	{SYS_LSI_OTPPOC_EN_SPI33_DS_MASK,	SYS_LSI_OTPPOC_SPI33_E_MASK,	SYS_LSI_OTPPOC_SPI33_E_OFFSET},
	{SYS_LSI_OTPPOC_EN_SCIF_DS_MASK,	SYS_LSI_OTPPOC_SCIF_E_MASK,		SYS_LSI_OTPPOC_SCIF_E_OFFSET},
};

static const PFC_REGS *pfc_boot_mode_tbls[SYS_BOOT_MODE_MAX] = {
	pfc_sd_reg_tbl,
	pfc_sd_reg_tbl,
	pfc_sd_reg_tbl,
	pfc_qspi_reg_tbl,
	pfc_qspi_reg_tbl,
	pfc_scif_reg_tbl
};

static void pfc_sd_setup(void)
{
	int cnt;

	for (cnt = 0; cnt < PFC_TBL_LEN; cnt++) {
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

static void pfc_qspi_setup(void)
{
	int cnt;

	for (cnt = 0; cnt < PFC_TBL_LEN; cnt++) {
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

static void pfc_scif_setup(void)
{
	int cnt;

	for (cnt = 0; cnt < PFC_TBL_LEN; cnt++) {
		/* PUPD */
		if (pfc_scif_reg_tbl[cnt].pupd.flg == PFC_ON) {
			mmio_write_64(pfc_scif_reg_tbl[cnt].pupd.reg, pfc_scif_reg_tbl[cnt].pupd.val);
		}
		/* SR */
		if (pfc_scif_reg_tbl[cnt].sr.flg == PFC_ON) {
			mmio_write_64(pfc_scif_reg_tbl[cnt].sr.reg, pfc_scif_reg_tbl[cnt].sr.val);
		}
	}
}

static void pfc_drive_setup(void)
{
	static const uint64_t pfc_iolh_drive_tbl[4] = {0x0000000000000000, 0x0101010101010101, 0x0202020202020202, 0x0303030303030303};
	/* Get the boot mode */
	boot_mode_t boot_mode = sys_get_boot_mode();

	if (boot_mode < SYS_BOOT_MODE_MAX) {
		const PFC_REGS *p_pins_tbl = pfc_boot_mode_tbls[boot_mode];
		uint32_t sys_lsi_otppoc = mmio_read_32(SYS_LSI_OTPPOC);
		uint64_t pfc_iolh_drive = 0;
		int cnt;

		if (0 != (sys_lsi_otppoc & pfc_io_drive[boot_mode].enable_mask)) {
			uint32_t index = ((sys_lsi_otppoc & pfc_io_drive[boot_mode].drive_mask) >> pfc_io_drive[boot_mode].drive_offset);

			pfc_iolh_drive = pfc_iolh_drive_tbl[index];
		}

		for (cnt = 0; cnt < PFC_TBL_LEN; cnt++) {
			if (p_pins_tbl[cnt].iolh.flg == PFC_ON) {
				/* Write IOLH value from pfc_sd_reg_tbl[] masked with value in pin table */
				mmio_write_64(p_pins_tbl[cnt].iolh.reg, (pfc_iolh_drive & p_pins_tbl[cnt].iolh.val));
			}
		}
	}
}

static void pfc_riic_pmic_setup(void)
{
#if PLAT_SYSTEM_SUSPEND
	int cnt;

	mmio_write_32(PFC_PWPR, mmio_read_32(PFC_PWPR) | PWPR_REGWE_A);

	for (cnt = 0; cnt < PFC_TBL_LEN; cnt++) {
		/* PFC */
		if (pfc_i2c_bus8_reg_tbl[cnt].pfc.flg == PFC_ON) {
			mmio_write_32(pfc_i2c_bus8_reg_tbl[cnt].pfc.reg, pfc_i2c_bus8_reg_tbl[cnt].pfc.val);
		}
		/* PMC */
		if (pfc_i2c_bus8_reg_tbl[cnt].pmc.flg == PFC_ON) {
			mmio_write_8(pfc_i2c_bus8_reg_tbl[cnt].pmc.reg, pfc_i2c_bus8_reg_tbl[cnt].pmc.val);
		}
	}

	mmio_write_32(PFC_PWPR, mmio_read_32(PFC_PWPR) & ~PWPR_REGWE_A);
#endif /* PLAT_SYSTEM_SUSPEND */
}

void pfc_setup(void)
{
	pfc_sd_setup();
	pfc_qspi_setup();
	pfc_scif_setup();
	pfc_drive_setup();
	pfc_riic_pmic_setup();
}
