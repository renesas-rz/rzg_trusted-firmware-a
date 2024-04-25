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

#define NUM_PORTS						(36u)
#define NUM_BITS						(8u)
#define NUM_FUNCTIONS					(64u)

#define PORT_SAFETY_MAX					(12U)
#define PORT_DRCTL_PIN_REG_HI			(4U)

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

		/* Dsable writing to GPIO registers in Safety Region and Non-Safety Region */
		sys_safetybase_lock(PRCRx_GPIO);
		sys_base_lock(PRCRx_GPIO);
	}
}

static void pfc_emmc_setup(void)
{
	/* Pin settings for eMMC */
	static const PORT_SETTINGS sdhi0_emmc_18V_pins[] = {
		/* Port, pin, func#, drive control */
		{12, 0, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_UHI_MSK},	/* SD0_CLK */
		{12, 1, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_CMD */
		{12, 2, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_DATA0 */
		{12, 3, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_DATA1 */
		{12, 4, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_DATA2 */
		{12, 5, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_DATA3 */
		{12, 6, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_DATA4 */
		{12, 7, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_DATA5 */
		{13, 0, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_DATA6 */
		{13, 1, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_DATA7 */
		{13, 2, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_RST# */
	};

	static const PORT_SETTINGS sdhi0_emmc_33V_pins[] = {
		/* Port, pin, func#, drive control */
		{12, 0, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_CLK */
		{12, 1, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_CMD */
		{12, 2, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_DATA0 */
		{12, 3, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_DATA1 */
		{12, 4, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_DATA2 */
		{12, 5, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_DATA3 */
		{12, 6, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_DATA4 */
		{12, 7, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_DATA5 */
		{13, 0, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_DATA6 */
		{13, 1, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_DATA7 */
		{13, 2, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD0_RST# */
	};

	if (true == sys_get_is_3v3()) {
		set_port_pins(sdhi0_emmc_33V_pins, PIN_COUNT(sdhi0_emmc_33V_pins));
	} else {
		set_port_pins(sdhi0_emmc_18V_pins, PIN_COUNT(sdhi0_emmc_18V_pins));
	}
}

static void pfc_sd_setup(void)
{
	/* Pin settings for eSD */
	static const PORT_SETTINGS sdhi1_pins[] = {
		/* Port, pin, func#, drive control for SDHI1 4bit boot */
		{16, 5, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* SD1_CLK */
		{16, 6, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_CMD */
		{16, 7, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_DATA0 */
		{17, 0, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_DATA1 */
		{17, 1, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_DATA2 */
		{17, 2, 41, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_MI_MSK},	/* SD1_DATA3 */
	};

	set_port_pins(sdhi1_pins, PIN_COUNT(sdhi1_pins));
}

static void pfc_xspi0_setup(void)
{
	static PORT_SETTINGS xspi0_x1_pins[] = {
		/* Port, pin, func#, drive control */
		{5, 1, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_CKP */
		{5, 6, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO0 */
		{5, 7, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO1 */
		{5, 3, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_CS0# */
	};

	static PORT_SETTINGS xspi0_x8_pins[] = {
		/* Port, pin, func#, drive control */
		{5, 5, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_DS */
		{5, 2, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_CKN */
		{5, 1, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_CKP */
		{5, 6, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO0 */
		{5, 7, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO1 */
		{6, 0, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO2 */
		{6, 1, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO3 */
		{6, 2, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO4 */
		{6, 3, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO5 */
		{6, 4, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO6 */
		{6, 5, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_IO7 */
		{5, 3, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI0_CS0# */
		{6, 6, 28, 0},											/* XSPI0_RESET0# */
																/* NOTE: KTG: Keep #RESET definition at end of pin list */
	};

	boot_mode_t boot_mode = sys_get_boot_mode();

	/* Unlock IOVOLCTL0 register */
	sys_base_unlock(PRCRx_SYS_CTRL);

	switch (boot_mode) {
	case (SYS_BOOT_MODE_XSPI0_x1):
		/* Check status of MDV3MON pin at startup. If high then enable Slewrate */
		if (true == sys_get_is_3v3()) {
			uint32_t i;

			/* MDV3MON_MSK is set so configure pins for enabling Schmitt operation */
			for (i = 0; i < PIN_COUNT(xspi0_x1_pins); i++) {
				xspi0_x1_pins[i].drctl |= DRCTL_SMTm0_MSK;
			}

			/* setup 3.3V voltage */
			mmio_write_32(IOVOLCTL0, IOVOLCTL_33V);
		} else {
			/* setup 1.8V voltage */
			mmio_write_32(IOVOLCTL0, IOVOLCTL_18V);
		}

		set_port_pins(xspi0_x1_pins, PIN_COUNT(xspi0_x1_pins));
		break;

	/* Default XSPI0 mode to be configured is x8 */
	default:
		/* Check status of MDV3MON pin at startup. If high then enable Slewrate */
		if (true == sys_get_is_3v3()) {
			uint32_t i;

			/* MDV3MON_MSK is set so configure pins for enabling Schmitt operation */
			for (i = 0; i < PIN_COUNT(xspi0_x8_pins); i++) {
				xspi0_x8_pins[i].drctl |= DRCTL_SMTm0_MSK;
			}

			/* setup 3.3V voltage */
			mmio_write_32(IOVOLCTL0, IOVOLCTL_33V);
		} else {
			/* setup 1.8V voltage */
			mmio_write_32(IOVOLCTL0, IOVOLCTL_18V);
		}

		set_port_pins(xspi0_x8_pins, PIN_COUNT(xspi0_x8_pins));
		break;
	}

	/* Lock IOVOLCTL0 register */
	sys_base_lock(PRCRx_SYS_CTRL);
}

static void pfc_xspi1_setup(void)
{
	static PORT_SETTINGS xspi1_x1_pins[] = {
		/* Port, pin, func#, drive control */
		{1, 4, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI1_IO0 */
		{1, 5, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI1_IO1 */
		{1, 0, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI1_CKP */
		{1, 1, 28, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* XSPI1_CS0# */
	};

	/* Unlock IOVOLCTL1 register */
	sys_base_unlock(PRCRx_SYS_CTRL);

	/* Check status of MDV3MON pin at startup. If high then enable Slewrate */
	if (true == sys_get_is_3v3()) {
		uint32_t i;

		/* MDV3MON_MSK is set so configure pins for enabling Schmitt operation */
		for (i = 0; i < PIN_COUNT(xspi1_x1_pins); i++) {
			xspi1_x1_pins[i].drctl |= DRCTL_SMTm0_MSK;
		}

		/* setup 3.3V voltage */
		mmio_write_32(IOVOLCTL1, IOVOLCTL_33V);
	} else {
		/* setup 1.8V voltage */
		mmio_write_32(IOVOLCTL1, IOVOLCTL_18V);
	}

	set_port_pins(xspi1_x1_pins, PIN_COUNT(xspi1_x1_pins));

	/* Lock IOVOLCTL1 register */
	sys_base_lock(PRCRx_SYS_CTRL);
}

static void pfc_scif_setup(void)
{
	static const PORT_SETTINGS sci0_pins[] = {
		/* Port, pin, func#, drive control */
		{27, 5, 20, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* TXD0 */
		{27, 4, 20, DRCTL_SRm0_MSK | DRCTL_Em0_DRIVE_HI_MSK},	/* RXD0 */
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
