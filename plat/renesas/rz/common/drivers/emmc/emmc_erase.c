/*
 * Copyright (c) 2015-2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch_helpers.h>

#include "emmc_config.h"
#include "emmc_hal.h"
#include "emmc_std.h"
#include "emmc_registers.h"
#include "emmc_def.h"

EMMC_ERROR_CODE emmc_erase_sector(uint32_t start_address, uint32_t end_address)
{
	EMMC_ERROR_CODE result;

	/* parameter check */
	if (start_address > end_address) {
		emmc_write_error_info(EMMC_FUNCNO_ERASE_SECTOR, EMMC_ERR_PARAM);
		return EMMC_ERR_PARAM;
	}

	/* state check */
	if (mmc_drv_obj.mount != TRUE) {
		emmc_write_error_info(EMMC_FUNCNO_ERASE_SECTOR, EMMC_ERR_STATE);
		return EMMC_ERR_STATE;
	}

	/* CMD35 */
	emmc_make_nontrans_cmd(CMD35_ERASE_GROUP_START, start_address);
	result = emmc_exec_cmd(EMMC_R1_ERROR_MASK, mmc_drv_obj.response);
	if (result != EMMC_SUCCESS) {
		return result;
	}

	/* CMD36 */
	emmc_make_nontrans_cmd(CMD36_ERASE_GROUP_END, end_address);
	result = emmc_exec_cmd(EMMC_R1_ERROR_MASK, mmc_drv_obj.response);
	if (result != EMMC_SUCCESS) {
		return result;
	}

	/* CMD38 */
	emmc_make_nontrans_cmd(CMD38_ERASE, 0);
	result = emmc_exec_cmd(EMMC_R1_ERROR_MASK, mmc_drv_obj.response);
	if (result != EMMC_SUCCESS) {
		return result;
	}

	//Figure A.11 - CIM_ERASE_GROUP   CMD13
	/* CMD13 */
	emmc_make_nontrans_cmd(CMD13_SEND_STATUS, EMMC_RCA<<16);
	result = emmc_exec_cmd(EMMC_R1_ERROR_MASK, mmc_drv_obj.response);
	if (result != EMMC_SUCCESS) {
		return result;
	}

	/* ready status check */
	if ((mmc_drv_obj.r1_card_status & EMMC_R1_READY) == 0) {
		emmc_write_error_info(EMMC_FUNCNO_ERASE_SECTOR, EMMC_ERR_CARD_BUSY);
		return EMMC_ERR_CARD_BUSY;
	}

	return EMMC_SUCCESS;
}
