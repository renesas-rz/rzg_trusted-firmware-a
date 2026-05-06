/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <lib/mmio.h>

#include <riic.h>
#include <vbatt_regs.h>
#include <pfc.h>

#define PMIC_ADDRESS				(uint8_t)(0x12)		/* PMIC I2C address of IC */
#define GP_ADDRESS					(uint8_t)(0x38)		/* GreenPak I2C address of IC */

/*
 * Sleep History Status Clear Register
 */
#define GP_SHSC_ADDR				(uint8_t)(0x97)
#define SLP_HIST_STS_CLEAR			(uint8_t)(0x50)
#define SLP_HIST_STS_KEEP			(uint8_t)(0x5E)

/*
 * Status Register
 */
#define GP_SR_ADDR					(uint8_t)(0xF1)
/* Sleep History Status */
#define SLP_HST_STS_MASK			(uint8_t)(1 << 4)
#define SLP_HST_STS_EVENT_OCCURRED	(uint8_t)(1 << 4)
#define SLP_HST_STS_NO_EVENT		(uint8_t)(0 << 4)
/* Power Button Interrupt Status */
#define PWRBTN_INT_STS_MASK			(uint8_t)(1 << 5)
/* Sleep Button Interrupt Status */
#define SLPBTN_INT_STS_MASK			(uint8_t)(1 << 6)

/*
 * Enable Control Register
 */
#define GP_ECR_ADDR					(uint8_t)(0xF4)
/* Sleep Enable Request */
#define SLP_EN_REQ_MASK				(uint8_t)(1 << 0)
/* Sleep Button Wake Enable Request */
#define SLPBTN_WK_EN_REQ_MASK		(uint8_t)(1 << 4)

/*
 * Enable Status Register
 */
#define GP_ESR_ADDR					(uint8_t)(0xF5)
/* Sleep Enable Status */
#define SLP_EN_STS_MASK				(uint8_t)(1 << 0)
#define SLP_EN_STS_ENABLED			(uint8_t)(1 << 0)
#define SLP_EN_STS_DISABLED			(uint8_t)(0 << 0)
/* Sleep Button Wake Enable Status */
#define SLPBTN_WK_EN_STS_MASK		(uint8_t)(1 << 4)
#define SLPBTN_WK_EN_STS_ENABLED	(uint8_t)(1 << 4)
#define SLPBTN_WK_EN_STS_DISABLED	(uint8_t)(0 << 4)

#if IMAGE_BL2
#if defined(PLAT_SYSTEM_SUSPEND_awo)
bool pwrc_board_is_resume(void)
{
	uint8_t gpak_data = 0;
	pfc_riic_pmic_setup();
	riic_setup();
	riic_read(GP_ADDRESS, GP_ESR_ADDR, &gpak_data);

	if ((gpak_data & SLP_EN_STS_MASK) == SLP_EN_STS_ENABLED) {
		return true;
	}

	return false;
}
#endif

#if defined(PLAT_SYSTEM_SUSPEND_vbat)
bool pwrc_board_is_resume(void)
{
	uint8_t gpak_data = 0;
	pfc_riic_pmic_setup();
	riic_setup();
	riic_read(GP_ADDRESS, GP_SR_ADDR, &gpak_data);

	if ((gpak_data & SLP_HST_STS_MASK) == SLP_HST_STS_EVENT_OCCURRED) {
		return true;
	}

	return false;
}
#endif

void pwrc_clear_resume_flag(void)
{
	pfc_riic_pmic_setup();
	riic_setup();

	riic_write(GP_ADDRESS, GP_SHSC_ADDR, SLP_HIST_STS_CLEAR);
	riic_write(GP_ADDRESS, GP_SHSC_ADDR, SLP_HIST_STS_KEEP);
}
#endif /* IMAGE_BL2 */

#if IMAGE_BL31
/*
 * Below the function pwrc_board_suspend_on() for vbat mode will be performed by the CA55
 * The pwrc_board_suspend_on() function for AWO mode should be performed by the CM33 and
 * is thus not included in this code. This function is solely executed in SRAM, beware of
 * including any code that branches to code that is not in SRAM, as it may cause a crash.
 */
#if defined(PLAT_SYSTEM_SUSPEND_vbat)
void pwrc_board_suspend_on(void)
{
	uint8_t gpak_data = 0;

	riic_setup();
	riic_read(GP_ADDRESS, GP_ESR_ADDR, &gpak_data);
	if ((gpak_data & SLPBTN_WK_EN_STS_MASK) == SLPBTN_WK_EN_STS_DISABLED) {
		riic_read(GP_ADDRESS, GP_ECR_ADDR, &gpak_data);
		riic_write(GP_ADDRESS, GP_ECR_ADDR, gpak_data & (~SLPBTN_WK_EN_REQ_MASK));
		riic_write(GP_ADDRESS, GP_ECR_ADDR, gpak_data | SLPBTN_WK_EN_REQ_MASK);
	}

	riic_read(GP_ADDRESS, GP_ECR_ADDR, &gpak_data);
	riic_write(GP_ADDRESS, GP_ECR_ADDR, gpak_data & (~SLP_EN_REQ_MASK));
	riic_write(GP_ADDRESS, GP_ECR_ADDR, gpak_data | SLP_EN_REQ_MASK);
}
#endif
#endif
