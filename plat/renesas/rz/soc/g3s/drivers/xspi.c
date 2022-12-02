/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <lib/utils_def.h>
#include <lib/mmio.h>
#include <arch_helpers.h>
#include <xspi_regs.h>
#include <xspi.h>


#define XSPI_BMCFG_SET_VALUE				(0x00010001UL)
#define XSPI_CMCFG0CS0_SET_VALUE			(0x00080008UL)
#define XSPI_CMCFG1CS0_SET_VALUE			(0x00000300UL)
#define XSPI_LIOCFGCS0_SET_VALUE			(0x00000000UL)
#define XSPI_BMCTL0_SET_VALUE				(0x00000001UL)
#define XSPI_CDTBUF0_SET_VALUE				(0x00000000UL)
#define XSPI_INTC_SET_VALUE					(0x00000000UL)

#define XSPI_IN					(0u)
#define XSPI_OUT				(1u)

#define MX25_RDID				(0x00u)
#define MX25_RSTEN				(0x01u)
#define MX25_RST				(0x02u)

#define DEVID_ID_MASK			(0x00FFFFFFu)
#define DEVICE_ID_BAD			(0u)
#define DEVICE_ID_ERROR			(0x00FFFFFFu)

#define XSPI_COMMAND_TIMEOUT	(100000u)


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

static st_xspi_cmd_info_t m_cmd_info;

static const st_xspi_cmd_t mx25_cmds[] = {
/* {instruction,   direction,	latency,	 data_size,	 addr_size,	 cmd_size}*/
	{0x9F00u,		XSPI_IN,	 0u,		   3u,		   0u,		  1u},	/* MX25_RDID */
	{0x6600u,		XSPI_OUT,	 0u,		   0u,		   0u,		  1u},	/* MX25_RSTEN */
	{0x9900u,		XSPI_OUT,	 0u,		   0u,		   0u,		  1u},	/* MX25_RST */
};

static int xspi_single_command(const st_xspi_cmd_info_t * const p_cmd_info)
{
	uint32_t timeout;

	mmio_write_32(XSPI_CDCTL0, mmio_read_32(XSPI_CDCTL0) & (~XSPI_CDCTL0_TRREQ_MSK));

	mmio_write_32(XSPI_CDTBUF0,	  (((uint32_t)mx25_cmds[p_cmd_info->cmd_idx].instruction) << XSPI_CDTBUF_CMD_OFFSET)
								| (((uint32_t)mx25_cmds[p_cmd_info->cmd_idx].direction) << XSPI_CDTBUF_TRTYPE_OFFSET)
								| (((uint32_t)mx25_cmds[p_cmd_info->cmd_idx].latency)	<< XSPI_CDTBUF_LATE_OFFSET)
								| (((uint32_t)mx25_cmds[p_cmd_info->cmd_idx].data_size) << XSPI_CDTBUF_DATASIZE_OFFSET)
								| (((uint32_t)mx25_cmds[p_cmd_info->cmd_idx].addr_size) << XSPI_CDTBUF_ADDSIZE_OFFSET)
								| (((uint32_t)mx25_cmds[p_cmd_info->cmd_idx].inst_size) << XSPI_CDTBUF_CMDSIZE_OFFSET));

	mmio_write_32(XSPI_CDABUF0, p_cmd_info->addr);
	mmio_write_32(XSPI_CDD0BUF0, p_cmd_info->data);

	mmio_write_32(XSPI_CDCTL0, mmio_read_32(XSPI_CDCTL0) | XSPI_CDCTL0_TRREQ_MSK);

	timeout = XSPI_COMMAND_TIMEOUT;
	/* Wait for command to complete */
	while ((0u == (mmio_read_32(XSPI_INTS) & XSPI_INTS_CMDCMP_MSK)) && (timeout > 0u)) {
		timeout--;
		__asm__ ("nop");
		dsb();
	}

	mmio_write_32(XSPI_INTC, mmio_read_32(XSPI_INTC) | XSPI_INTC_CMDCMPC_MSK);

	return (timeout == 0u) ? XSPI_ERROR : XSPI_SUCCESS;
}

static int xspi_reset(void)
{
	int ret;

	/* Issue the reset command */
	m_cmd_info.cmd_idx = MX25_RSTEN;
	m_cmd_info.data = 0;
	m_cmd_info.addr = 0;

	ret = xspi_single_command(&m_cmd_info);
	if (ret == XSPI_SUCCESS) {
		m_cmd_info.cmd_idx = MX25_RST;
		ret = xspi_single_command(&m_cmd_info);
	}

	return ret;
}

static int xspi_read_identification(void)
{
	uint32_t id = DEVICE_ID_BAD;
	uint32_t prev_id = DEVICE_ID_BAD;
	uint32_t count = 1000;

	m_cmd_info.cmd_idx = MX25_RDID;
	m_cmd_info.data = 0;
	m_cmd_info.addr = 0;

	while (count > 0U) {
		if (xspi_single_command(&m_cmd_info) == XSPI_SUCCESS) {
			/* Command success */
			id = mmio_read_32(XSPI_CDD0BUF0) & DEVID_ID_MASK;
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

int xspi_setup(void)
{
	int ret;

	mmio_write_32(XSPI_BMCFG,		XSPI_BMCFG_SET_VALUE);
	mmio_write_32(XSPI_CMCFG0CS0,	XSPI_CMCFG0CS0_SET_VALUE);
	mmio_write_32(XSPI_CMCFG1CS0,	XSPI_CMCFG1CS0_SET_VALUE);
	mmio_write_32(XSPI_LIOCFGCS0,	XSPI_LIOCFGCS0_SET_VALUE);
	mmio_write_32(XSPI_BMCTL0,		XSPI_BMCTL0_SET_VALUE);
	mmio_write_32(XSPI_CDTBUF0,		XSPI_CDTBUF0_SET_VALUE);
	mmio_write_32(XSPI_INTC,		XSPI_BMCFG_SET_VALUE);

	ret = xspi_reset();
	if (ret == XSPI_SUCCESS)
		ret = xspi_read_identification();

	return ret;
}
