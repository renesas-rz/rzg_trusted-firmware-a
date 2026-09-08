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
#include <common/debug.h>
#include <lib/mmio.h>
#include <otp_data.h>

#define NUM_PORTS						(26u)
#define NUM_BITS						(8u)
#define NUM_FUNCTIONS					(64u)

#define PORT_SAFETY_MAX					(7U)

#define PIN_COUNT(x)					(sizeof(x)/sizeof((x)[0]))


static void set_port_pins(const PORT_SETTINGS *p_setting, const uint32_t count)
{
	uint32_t i;
	uint32_t port_num;
	uintptr_t base_adrs;

	/* Validate parameters */
	if ((p_setting != NULL) && (count > 0u)) {
		/* Enable writing to GPIO registers in Safety Region and Non-Safety Region */
		sys_safetybase_unlock(PRCRx_GPIO);
		sys_base_unlock(PRCRx_GPIO);

		for (i = 0; i < count; i++) {
			port_num = p_setting->port;
			if (p_setting->port <= PORT_SAFETY_MAX) {
				/* Space of Safety area register */
				base_adrs = PORT_SAFETY_BASE;
			} else {
				/* Space of Non-Safety area register */
				base_adrs = PORT_BASE;
			}

			/* Check port bit and function are valid */
			if ((port_num < NUM_PORTS) && (p_setting->bit < NUM_BITS) && (p_setting->function < NUM_FUNCTIONS)) {
				mmio_write_64(PORT_DRCTL(base_adrs, port_num), mmio_read_64(PORT_DRCTL(base_adrs, port_num)) & (~(((uint32_t)0xffu) << (8u*p_setting->bit))));
				mmio_write_64(PORT_DRCTL(base_adrs, port_num), mmio_read_64(PORT_DRCTL(base_adrs, port_num)) | (((uint32_t)p_setting->drctl) << (8u*p_setting->bit)));

				/* Set the corresponding PMCmn to 0 before changing the PFCmn register */
				mmio_write_8(PORT_PMC(base_adrs, port_num), mmio_read_8(PORT_PMC(base_adrs, port_num)) & (~(1u << p_setting->bit)));

				/* Select the pin function */
				mmio_write_64(PORT_PFC(base_adrs, port_num), mmio_read_64(PORT_PFC(base_adrs, port_num)) & (~(((uint64_t)0x3fu) << (8u*p_setting->bit))));
				mmio_write_64(PORT_PFC(base_adrs, port_num), mmio_read_64(PORT_PFC(base_adrs, port_num)) | (((uint64_t)p_setting->function) << (8u*p_setting->bit)));

				/* Select peripheral function (not GPIO) for pin */
				mmio_write_8(PORT_PMC(base_adrs, port_num), mmio_read_8(PORT_PMC(base_adrs, port_num)) | (1u << p_setting->bit));
			}

			p_setting++;
		}

		/* Disable writing to GPIO registers in Safety Region and Non-Safety Region */
		sys_safetybase_lock(PRCRx_GPIO);
		sys_base_lock(PRCRx_GPIO);
	}
}

static void pfc_emmc_setup(void)
{
	/* Pin settings for eMMC */
	static const PORT_SETTINGS sdhi0_emmc_18V_pins[] = {
		/* Port, pin, func#, drive control */
		{4, 4, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_CLK */
		{4, 5, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_CMD */
		{4, 6, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_DATA0 */
		{4, 7, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_DATA1 */
		{5, 0, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_DATA2 */
		{5, 1, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_DATA3 */
		{5, 2, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_DATA4 */
		{5, 3, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_DATA5 */
		{5, 4, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_DATA6 */
		{5, 5, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_DATA7 */
		{5, 6, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_RST# */
	};

	static const PORT_SETTINGS sdhi0_emmc_33V_pins[] = {
		/* Port, pin, func#, drive control */
		{4, 4, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_CLK */
		{4, 5, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_CMD */
		{4, 6, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_DATA0 */
		{4, 7, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_DATA1 */
		{5, 0, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_DATA2 */
		{5, 1, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_DATA3 */
		{5, 2, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_DATA4 */
		{5, 3, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_DATA5 */
		{5, 4, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_DATA6 */
		{5, 5, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_DATA7 */
		{5, 6, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD0_RST# */
	};

	if (true == sys_get_is_3v3(MD_MON_MDV_LAT_SDHI0)) {
		set_port_pins(sdhi0_emmc_33V_pins, PIN_COUNT(sdhi0_emmc_33V_pins));
	} else {
		set_port_pins(sdhi0_emmc_18V_pins, PIN_COUNT(sdhi0_emmc_18V_pins));
	}
}

static void pfc_sd_setup(void)
{
	/* sd vcc pin is platform specific*/
	uint32_t part_number = sys_get_platform_part_number();

	/* TO-DO- To be updated once part numbers have been provided*/
	switch (part_number) {
	/* RZT2N 537 package*/
	case OTP_PN_RZT2N_537_DUAL_ESC_HPSW_CANFD_SEC_R9A07G076M48GBG: {
		INFO("BL2: 537 Pin RZT2N platform detected");
		/* Pin settings for eSD */
		static const PORT_SETTINGS sdhi1_pins[] = {
			/* Port, pin, func#, drive control for SDHI1 4bit boot */
			{3, 6, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD1_CLK */
			{3, 7, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_CMD */
			{4, 0, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_DATA0 */
			{4, 1, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_DATA1 */
			{4, 2, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_DATA2 */
			{4, 3, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_DATA3 */
			{13, 1, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_PWEN */
		};
		set_port_pins(sdhi1_pins, PIN_COUNT(sdhi1_pins));
	}
	break;

	/* RZT2N 489 package*/
	case OTP_PN_RZT2N_489_DUAL_ESC_HPSW_CANFD_SEC_R9A07G076M48GBA: {
		INFO("BL2: 489 Pin RZT2N platform detected");
		/* Pin settings for eSD */
		static const PORT_SETTINGS sdhi1_pins[] = {
			/* Port, pin, func#, drive control for SDHI1 4bit boot */
			{3, 6, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD1_CLK */
			{3, 7, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_CMD */
			{4, 0, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_DATA0 */
			{4, 1, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_DATA1 */
			{4, 2, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_DATA2 */
			{4, 3, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_DATA3 */
			{3, 3, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_PWEN */
		};
		set_port_pins(sdhi1_pins, PIN_COUNT(sdhi1_pins));
	}
	break;

	/* Part Number not recognised */
	default: {
		INFO("BL2: No known RZT2N platform detected, sd_vcc pin not enabled");
		/* Pin settings for eSD */
		static const PORT_SETTINGS sdhi1_pins[] = {
			/* Port, pin, func#, drive control for SDHI1 4bit boot */
			{3, 6, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD1_CLK */
			{3, 7, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_CMD */
			{4, 0, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_DATA0 */
			{4, 1, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_DATA1 */
			{4, 2, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_DATA2 */
			{4, 3, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_DATA3 */
			//{13, 1, 44, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_PWEN */
		};
		set_port_pins(sdhi1_pins, PIN_COUNT(sdhi1_pins));
	}
	break;
	};
}

static void pfc_xspi0_setup(void)
{
	static PORT_SETTINGS xspi0_x1_18V_pins[] = {
		/* Port, pin, func#, drive control */
		{17, 2, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_CKP */
		{17, 7, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO0 */
		{18, 0, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO1 */
		{17, 4, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_CS0# */
	};

	static PORT_SETTINGS xspi0_x1_33V_pins[] = {
		/* Port, pin, func#, drive control */
		{17, 2, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_CKP */
		{17, 7, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO0 */
		{18, 0, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO1 */
		{17, 4, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_CS0# */
	};

	static PORT_SETTINGS xspi0_x8_18V_pins[] = {
		/* Port, pin, func#, drive control */
		{17, 2, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_CKP */
		{17, 3, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_CKN */
		{17, 7, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO0 */
		{18, 0, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO1 */
		{18, 1, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO2 */
		{18, 2, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO3 */
		{18, 3, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO4 */
		{18, 4, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO5 */
		{18, 5, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO6 */
		{18, 6, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO7 */
		{17, 4, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_CS0# */
		{17, 6, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_DS */
		{18, 7, 32, DRCTL_Em0_DRIVE_LO_MSK},					/* XSPI0_RESET0# */
																/* NOTE: KTG: Keep #RESET definition at end of pin list */
	};

	static PORT_SETTINGS xspi0_x8_33V_pins[] = {
		/* Port, pin, func#, drive control */
		{17, 2, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_CKP */
		{17, 3, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_CKN */
		{17, 7, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO0 */
		{18, 0, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO1 */
		{18, 1, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO2 */
		{18, 2, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO3 */
		{18, 3, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO4 */
		{18, 4, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO5 */
		{18, 5, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO6 */
		{18, 6, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO7 */
		{17, 4, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_CS0# */
		{17, 6, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_DS */
		{18, 7, 32, DRCTL_Em0_DRIVE_LO_MSK},									/* XSPI0_RESET0# */
																				/* NOTE: KTG: Keep #RESET definition at end of pin list */
	};

	boot_mode_t boot_mode = sys_get_boot_mode();

	switch (boot_mode) {
	case (SYS_BOOT_MODE_XSPI0_x1):
		/* Check status of MDV3MON pin at startup.*/
		if (true == sys_get_is_3v3(MD_MON_MDV_LAT_XSPI0)) {
			set_port_pins(xspi0_x1_33V_pins, PIN_COUNT(xspi0_x1_33V_pins));
			/* TO-DO: Confirm with device team if voltage for the diff domains would be set in hardware */
		} else {
			set_port_pins(xspi0_x1_18V_pins, PIN_COUNT(xspi0_x1_18V_pins));
		}
		break;

	/* Default XSPI0 mode to be configured is x8 */
	default:
		/* Check status of MDV3MON pin at startup. */
		if (true == sys_get_is_3v3(MD_MON_MDV_LAT_XSPI0)) {
			set_port_pins(xspi0_x8_33V_pins, PIN_COUNT(xspi0_x8_33V_pins));
			/* TO-DO: Confirm with device team if voltage for the diff domains would be set in hardware */
		} else {
			set_port_pins(xspi0_x8_18V_pins, PIN_COUNT(xspi0_x8_18V_pins));
		}
		break;
	}
}

static void pfc_xspi1_setup(void)
{
	static PORT_SETTINGS xspi1_x1_18V_pins[] = {
		/* Port, pin, func#, drive control */
		{15, 6, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI1_CKP */
		{16, 2, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI1_IO0 */
		{16, 3, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI1_IO1 */
		{16, 4, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI1_IO2 */
		{16, 5, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI1_IO3 */
		{15, 7, 32, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI1_CS0# */
	};

	static PORT_SETTINGS xspi1_x1_33V_pins[] = {
		/* Port, pin, func#, drive control */
		{15, 6, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI1_CKP */
		{16, 2, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI1_IO0 */
		{16, 3, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI1_IO1 */
		{16, 4, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI1_IO2 */
		{16, 5, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI1_IO3 */
		{15, 7, 32, DRCTL_SMTm0_MSK | DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI1_CS0# */
	};

	/* Check status of MDV3MON pin at startup.*/
	if (true == sys_get_is_3v3(MD_MON_MDV_LAT_XSPI1)) {
		set_port_pins(xspi1_x1_33V_pins, PIN_COUNT(xspi1_x1_33V_pins));
		/* TO-DO: Confirm with device team if voltage for the diff domains would be set in hardware */
	} else {
		set_port_pins(xspi1_x1_18V_pins, PIN_COUNT(xspi1_x1_18V_pins));
	}
}

static void pfc_scif_setup(void)
{
	static const PORT_SETTINGS sci0_pins[] = {
		/* Port, pin, func#, drive control */
		{11, 1, 24, DRCTL_Em0_DRIVE_LO_MSK},	/* RXD1 */
		{11, 2, 24, DRCTL_Em0_DRIVE_LO_MSK},	/* TXD1 */
	};

	set_port_pins(sci0_pins, PIN_COUNT(sci0_pins));
}


void pfc_setup(void)
{
	pfc_emmc_setup();
	pfc_sd_setup();
	pfc_xspi0_setup();
	pfc_xspi1_setup();
	pfc_scif_setup();
}
