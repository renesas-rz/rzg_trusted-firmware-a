/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <lib/utils_def.h>
#include <lib/mmio.h>
#include <arch_helpers.h>
#include <rz_soc_def.h>
#include <xspi_puccini_regs.h>
#include <xspi.h>
#include <sys.h>
#include <common/debug.h>


#if !USE_BOOTROM_XSPI_PARAMS
#define XSPI_IN						U(0)
#define XSPI_OUT					U(1)

#define RDID						(0)
#define RSTEN						(1)
#define RESET						(2)

#define DEVID_ID_MASK				UL(0x00FFFFFF)
#define DEVICE_ID_BAD				UL(0)
#define DEVICE_ID_ERROR				UL(0x00FFFFFF)

#define XSPI_COMMAND_TIMEOUT		U(100000)

#define XSPI_ADDR(x)				(xspi_base + (x))


typedef struct {
	uint16_t instruction;
	uint8_t	 direction : 3;		/* Direction */
	uint8_t	 latency   : 5;		/* Latency (cycle) */
	uint8_t	 data_size : 4;		/* Data size (byte) */
	uint8_t	 addr_size : 4;		/* Address size (byte) */
	uint8_t	 inst_size : 4;		/* Instruction size (byte) */
} st_xspi_cmd_t;

typedef struct {
	uint8_t	 cmd_idx;
	uint32_t addr;
	uint32_t data;
} st_xspi_cmd_info_t;

static const st_xspi_cmd_t cmds[] = {
/* {instruction,   direction,	latency,	data_size,	addr_size,	cmd_size}*/
	{0x9F00u,		XSPI_IN,	0u,			3u,			0u,			1u},	/* RDID */
	{0x6600u,		XSPI_OUT,	0u,			0u,			0u,			1u},	/* RSTEN */
	{0x9900u,		XSPI_OUT,	0u,			0u,			0u,			1u},	/* RESET */
};


static int32_t xspi_single_command(const uint32_t xspi_base, const st_xspi_cmd_info_t * const p_cmd_info)
{
	uint32_t timeout;

	mmio_write_32(XSPI_ADDR(XSPI_CDCTL0_OFFSET), mmio_read_32(XSPI_ADDR(XSPI_CDCTL0_OFFSET)) & (~XSPI_CDCTL0_TRREQ_MSK));

	mmio_write_32(XSPI_ADDR(XSPI_CDTBUF0_OFFSET), (((uint32_t)cmds[p_cmd_info->cmd_idx].instruction) << XSPI_CDTBUF_CMD_OFFSET)
												| (((uint32_t)cmds[p_cmd_info->cmd_idx].direction)	<< XSPI_CDTBUF_TRTYPE_OFFSET)
												| (((uint32_t)cmds[p_cmd_info->cmd_idx].latency)	<< XSPI_CDTBUF_LATE_OFFSET)
												| (((uint32_t)cmds[p_cmd_info->cmd_idx].data_size)	<< XSPI_CDTBUF_DATASIZE_OFFSET)
												| (((uint32_t)cmds[p_cmd_info->cmd_idx].addr_size)	<< XSPI_CDTBUF_ADDSIZE_OFFSET)
												| (((uint32_t)cmds[p_cmd_info->cmd_idx].inst_size)	<< XSPI_CDTBUF_CMDSIZE_OFFSET));

	mmio_write_32(XSPI_ADDR(XSPI_CDABUF0_OFFSET), p_cmd_info->addr);

	if (cmds[p_cmd_info->cmd_idx].direction == XSPI_OUT) {
		mmio_write_32(XSPI_ADDR(XSPI_CDD0BUF0_OFFSET), p_cmd_info->data);
	}

	mmio_write_32(XSPI_ADDR(XSPI_CDCTL0_OFFSET), mmio_read_32(XSPI_ADDR(XSPI_CDCTL0_OFFSET)) | XSPI_CDCTL0_TRREQ_MSK);

	timeout = XSPI_COMMAND_TIMEOUT;
	/* Wait for command to complete */
	while ((0u == (mmio_read_32(XSPI_ADDR(XSPI_INTS_OFFSET)) & XSPI_INTS_CMDCMP_MSK)) && (timeout > 0u)) {
		timeout--;
		__asm__ ("nop");
		dsb();
	}

	mmio_write_32(XSPI_ADDR(XSPI_INTC_OFFSET), mmio_read_32(XSPI_ADDR(XSPI_INTC_OFFSET)) | XSPI_INTC_CMDCMPC_MSK);

	return (timeout == 0u) ? XSPI_ERROR : XSPI_SUCCESS;
}

static int32_t xspi_reset(const uint32_t xspi_base, const boot_mode_t boot_mode)
{
	int ret;

	switch (boot_mode) {
	case SYS_BOOT_MODE_XSPI0_x1:
	case SYS_BOOT_MODE_XSPI1_x1: {
		/* Issue the reset command */
		st_xspi_cmd_info_t cmd_rsten = {RSTEN, 0, 0};
		st_xspi_cmd_info_t cmd_reset = {RESET, 0, 0};

		ret = xspi_single_command(xspi_base, &cmd_rsten);
		if (ret == XSPI_SUCCESS) {
			ret = xspi_single_command(xspi_base, &cmd_reset);
		}
	}
	break;

	case SYS_BOOT_MODE_XSPI0_x8: {
		uint32_t i = 0;

		/* Toggle the RESETCS0# pin. Start with clearing pin - Reset active */
		mmio_write_32(XSPI_ADDR(XSPI_LIOCTL_OFFSET), mmio_read_32(XSPI_ADDR(XSPI_LIOCTL_OFFSET)) & (~XSPI_LIOCTL_RSTCS0_MSK));
		/* Make a delay before releasing the reset - reduced from 1000u to 100u as per recommendation from RTL */
		for (i = 0; i < 100u; i++) {
			__asm__ ("nop");
		}
		/* Release the reset */
		mmio_write_32(XSPI_ADDR(XSPI_LIOCTL_OFFSET), mmio_read_32(XSPI_ADDR(XSPI_LIOCTL_OFFSET)) | XSPI_LIOCTL_RSTCS0_MSK);
		ret = XSPI_SUCCESS;
	}
	break;

	default: {
		/* Error */
		panic();
	}
	break;
	}

	return ret;
}

static uint32_t xspi_read_identification(const uint32_t xspi_base)
{
	uint32_t id = DEVICE_ID_BAD;
	uint32_t prev_id = DEVICE_ID_BAD;
	int32_t count = 1000;

	st_xspi_cmd_info_t cmd_rdid = {RDID, 0, 0};

	while (count > 0U) {
		if (xspi_single_command(xspi_base, &cmd_rdid) == XSPI_SUCCESS) {
			/* Command success */
			id = mmio_read_32(XSPI_ADDR(XSPI_CDD0BUF0_OFFSET)) & DEVID_ID_MASK;
			if ((id != DEVICE_ID_BAD) && (id != DEVICE_ID_ERROR) && (prev_id == id)) {
				/* Hardware ID is valid and has been repeated on two consecutive reads so exit the while loop and then function */
				break;
			}

			prev_id = id;
		}
		/* Allow retry of Id read in case Flash device has not had enough time to start-up*/
		count--;
	}

	return id;
}

static void xspi_init_registers(const uint32_t xspi_base, const boot_mode_t boot_mode)
{
	switch (boot_mode) {
	case SYS_BOOT_MODE_XSPI0_x1:
	case SYS_BOOT_MODE_XSPI1_x1: {
		/* Set SPI mode - sample data at falling edge, drive clock at falling edge, 1S-1S-1S */
		mmio_write_32(XSPI_ADDR(XSPI_LIOCFGCS0_OFFSET), mmio_read_32(XSPI_ADDR(XSPI_LIOCFGCS0_OFFSET)) | XSPI_LIOCFGCS_PRTMD_1S_1S_1S);
		/* Set memory mapping mode */
		mmio_write_32(XSPI_ADDR(XSPI_CMCFG0CS0_OFFSET), (0x02u << XSPI_CMCFG0CS_ADDSIZE));
		/* 0 latency, 0x0300 read command */
		mmio_write_32(XSPI_ADDR(XSPI_CMCFG1CS0_OFFSET), (0x00u << XSPI_CMCFG1CS_RDLATE) | (0x0300u << XSPI_CMCFG1CS_RDCMD));
		/* Enable read access to CS0 for selected channel memory areas */
		mmio_write_32(XSPI_ADDR(XSPI_BMCTL0_OFFSET), (0x01u << XSPI_BMCTL0_CS0ACC));

		/* WRAPCFG: use default values */
		/* COMCFG:	use default values */
		/* BMCFG:	use default values */
		/* CSSCTL:	use default values */
	}
	break;

	case SYS_BOOT_MODE_XSPI0_x8: {
		mmio_write_32(XSPI_ADDR(XSPI_WRAPCFG_OFFSET), (0x1Fu << XSPI_WRAPCFG_DSSFTCS0));

		/* Set SPI mode - 8D-8D-8D */
		mmio_write_32(XSPI_ADDR(XSPI_LIOCFGCS0_OFFSET), mmio_read_32(XSPI_ADDR(XSPI_LIOCFGCS0_OFFSET))
														| (XSPI_LIOCFGCS_PRTMD_8D_8D_8D << XSPI_LIOCFGCS_PRTMD)
														| (0x01u << XSPI_LIOCFGCS_LATEMD));

		/* Set memory mapping mode */
		mmio_write_32(XSPI_ADDR(XSPI_CMCFG0CS0_OFFSET), (0x03u << XSPI_CMCFG0CS_ADDSIZE) | (0x02u << XSPI_CMCFG0CS_FFMT));
		/* 10 cycle latency, 0xA000 read command */
		mmio_write_32(XSPI_ADDR(XSPI_CMCFG1CS0_OFFSET), (0x0Au << XSPI_CMCFG1CS_RDLATE) | (0xA000u << XSPI_CMCFG1CS_RDCMD));
		/* Enable read access to CS0 for selected channel memory areas */
		mmio_write_32(XSPI_ADDR(XSPI_BMCTL0_OFFSET), (0x01u << XSPI_BMCTL0_CS0ACC));

		/* COMCFG:	use default values */
		/* BMCFG:	use default values */
		/* CSSCTL:	use default values */
	}
	break;

	default: {
		/* Error */
		panic();
	}
	break;
	}
}

static int32_t xspi_prepare(const uint32_t xspi_base)
{
	/* Check if transaction has already been requested. If so return error */
	if (0u != (mmio_read_32(XSPI_ADDR(XSPI_CDCTL0_OFFSET)) & XSPI_CDCTL0_TRREQ_MSK)) {
		return XSPI_ERROR;
	}

	mmio_write_32(XSPI_ADDR(XSPI_INTC_OFFSET), mmio_read_32(XSPI_ADDR(XSPI_INTC_OFFSET)) | XSPI_INTC_CMDCMPC_MSK);
	mmio_write_32(XSPI_ADDR(XSPI_CDTBUF0_OFFSET), mmio_read_32(XSPI_ADDR(XSPI_CDTBUF0_OFFSET)) & (~(XSPI_CDTBUF_ADDSIZE_MSK | XSPI_CDTBUF_CMDSIZE_MSK)));
	dsb();

	return XSPI_SUCCESS;
}


int32_t xspi_setup(void)
{
	boot_mode_t boot_mode = sys_get_boot_mode();
	uint32_t xspi_base;
	int32_t ret;

	/* Validate boot mode and get base address of active XSPI block */
	switch (boot_mode) {
	case SYS_BOOT_MODE_XSPI0_x1:
	case SYS_BOOT_MODE_XSPI0_x8: {
		xspi_base = XSPI0_BASE;
	}
	break;

	case SYS_BOOT_MODE_XSPI1_x1: {
		xspi_base = XSPI1_BASE;
	}
	break;

	default: {
		/* Error */
		panic();
	}
	break;
	}

	/* xSPI Init */
	xspi_init_registers(xspi_base, boot_mode);
	/* Initial flash prepare/check */
	ret = xspi_prepare(xspi_base);
	if (ret == XSPI_SUCCESS) {
		/* Reset the flash memory */
		ret = xspi_reset(xspi_base, boot_mode);
		/* Perform ID read only if no errors and operational xSPI mode is x1 */
		if (ret == XSPI_SUCCESS) {
			switch (boot_mode) {
			case SYS_BOOT_MODE_XSPI0_x1:
			case SYS_BOOT_MODE_XSPI1_x1: {
				/* Read FLASH memory signature to check we can communicate */
				/* successfully (but don't check the ID) */
				uint32_t id = xspi_read_identification(xspi_base);

				if ((id == DEVICE_ID_BAD) || (id == DEVICE_ID_ERROR)) {
					/* Failed to read id */
					ret = XSPI_ERROR;
				}
			}
			break;

			case SYS_BOOT_MODE_XSPI0_x8: {
				uint32_t i = 0;

				/* As not spending time reading and confirming device ID make a delay to allow the device to fully come out of reset */
				/* Reduce from 100000u to 10000u as per recommendation from RTL testing */
				/* 10000u subsequently measured in RTL at 226us so x 3 to 30000U = ~680us */
				for (i = 0; i < 30000u; i++) {
					__asm__ ("nop");
				}
			}
			break;

			default: {
				/* Error */
				panic();
			}
			break;
			}
		}
	}

	return ret;
}
#else /* USE_BOOTROM_XSPI_PARAMS */
int32_t xspi_setup(void)
{
	/* Do nothing - XSPI already configured via BootROM */
	return XSPI_SUCCESS;
}
#endif	/* USE_BOOTROM_XSPI_PARAMS */
