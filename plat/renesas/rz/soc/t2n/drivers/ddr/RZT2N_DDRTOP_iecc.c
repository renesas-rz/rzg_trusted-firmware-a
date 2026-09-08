/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdint.h>
#include <drivers/delay_timer.h>
#include <common/debug.h>

#include "include/RZx/DDR/RZT2N_DDRTOP.h"
#include "include/RZx/DDR/RZT2N_DDRTOP_iecc.h"

/**
 * @brief Programming BIST logic to initialize memory area for Inline-ECC
 * @param start_addr 2^addr_space
 * @param addr_space bit[0]:1=first run, bit[1]:1=independent mode (with setting ecc_disable_w_uc_err)
 * @param run_mode
 */
void DDRTOP_proc_prog0(
	uint64_t start_addr,
	uint8_t addr_space,
	uint8_t run_mode)
{
	uint8_t lp_auto_entry_en = 0;

	INFO("Memory initialization program: 0x%08" PRIx64 " - 0x%08" PRIx64 "\n", start_addr, start_addr + (1 << addr_space) - 1);

	if (((run_mode >> 1) & 1) == 1) {
		/* This step should only be executed if the Inline-ECC feature is used. */
		DDRTOP_mc_param_wr(ECC_DISABLE_W_UC_ERR_ADDR, ECC_DISABLE_W_UC_ERR_OFFSET, ECC_DISABLE_W_UC_ERR_WIDTH, 1);

		/* Before running a BIST function, the automatic low power modes should be disabled (lp_auto_entry_en=0)
		 * and the memories should be removed from any low power state.
		 */
		lp_auto_entry_en = DDRTOP_mc_param_rd(LP_AUTO_ENTRY_EN_ADDR, LP_AUTO_ENTRY_EN_OFFSET, LP_AUTO_ENTRY_EN_WIDTH);
		DDRTOP_mc_param_wr(LP_AUTO_ENTRY_EN_ADDR, LP_AUTO_ENTRY_EN_OFFSET, LP_AUTO_ENTRY_EN_WIDTH, 0);
	}

	if (run_mode != 0) {
		/* Write all-0 to Memory. */
		DDRTOP_mc_param_wr(BIST_DATA_CHECK_ADDR, BIST_DATA_CHECK_OFFSET, BIST_DATA_CHECK_WIDTH, 1);
		DDRTOP_mc_param_wr(BIST_TEST_MODE_ADDR, BIST_TEST_MODE_OFFSET, BIST_TEST_MODE_WIDTH, 0b100);
		DDRTOP_mc_param_wr(BIST_DATA_PATTERN_ADDR + 0, 0, 32, 0);
		DDRTOP_mc_param_wr(BIST_DATA_PATTERN_ADDR + 1, 0, 32, 0);
	}

	/* Run BIST */
	DDRTOP_mc_param_wr(BIST_START_ADDRESS_ADDR, 0, 32, (start_addr & 0xffffffff));
	DDRTOP_mc_param_wr(ADDR_SPACE_ADDR, ADDR_SPACE_OFFSET, ADDR_SPACE_WIDTH, addr_space);
	udelay(1);
	DDRTOP_mc_param_wr(BIST_GO_ADDR, BIST_GO_OFFSET, BIST_GO_WIDTH, 1);
	DDRTOP_mc_param_poll(INT_STATUS_BIST_ADDR, INT_STATUS_BIST_OFFSET, 1, 1);
	DDRTOP_mc_param_wr(BIST_GO_ADDR, BIST_GO_OFFSET, BIST_GO_WIDTH, 0);

	/* Clear int_status_bist */
	DDRTOP_mc_param_wr(INT_ACK_BIST_ADDR, INT_ACK_BIST_OFFSET, 1, 1);
	DDRTOP_mc_param_poll(INT_STATUS_BIST_ADDR, INT_STATUS_BIST_OFFSET, 1, 0);

	if (((run_mode >> 1) & 1) == 1) {
		/* Clear int_status_ecc */
		DDRTOP_mc_param_wr(INT_ACK_ECC_ADDR, INT_ACK_ECC_OFFSET, INT_ACK_ECC_WIDTH, 0x000001CF);
		DDRTOP_mc_param_poll(INT_STATUS_ECC_ADDR, INT_STATUS_ECC_OFFSET, INT_STATUS_ECC_WIDTH, 0x00000000);

		/* Revert lp_auto_entry_en */
		DDRTOP_mc_param_wr(LP_AUTO_ENTRY_EN_ADDR, LP_AUTO_ENTRY_EN_OFFSET, LP_AUTO_ENTRY_EN_WIDTH, lp_auto_entry_en);

		/* Revert ecc_disable_w_uc_err */
		DDRTOP_mc_param_wr(ECC_DISABLE_W_UC_ERR_ADDR, ECC_DISABLE_W_UC_ERR_OFFSET, ECC_DISABLE_W_UC_ERR_WIDTH, 0);

		udelay(1);
	}
}

/**
 * @brief Memory initialization programming for Inline-ECC
 * @param range
 * @param num_of_ranges
 */
void DDRTOP_proc_ecc_init0(const uint64_t range[][2], int num_of_ranges)
{
	const int AW = 34;
	const uint64_t ADDR1 = 1;
	uint8_t lp_auto_entry_en = 0;
	uint64_t addr;
	uint64_t prog_size;

	/* This step should only be executed if the Inline-ECC feature is used. */
	DDRTOP_mc_param_wr(ECC_DISABLE_W_UC_ERR_ADDR, ECC_DISABLE_W_UC_ERR_OFFSET, ECC_DISABLE_W_UC_ERR_WIDTH, 1);

	/* Before running a BIST function, the automatic low power modes should be disabled (lp_auto_entry_en=0)
	 * and the memories should be removed from any low power state.
	 */
	lp_auto_entry_en = DDRTOP_mc_param_rd(LP_AUTO_ENTRY_EN_ADDR, LP_AUTO_ENTRY_EN_OFFSET, LP_AUTO_ENTRY_EN_WIDTH);
	DDRTOP_mc_param_wr(LP_AUTO_ENTRY_EN_ADDR, LP_AUTO_ENTRY_EN_OFFSET, LP_AUTO_ENTRY_EN_WIDTH, 0);

	/* Write all-0 to Memory */
	uint8_t prog0_run_mode = 0b01;
	for (int i = 0; i < num_of_ranges; i++) {
		prog_size = range[i][1] - range[i][0] + 1;
		addr = range[i][0];
		for (int j = 0; j < AW; j++) {
			if (((prog_size >> j) & ADDR1) == ADDR1) {
				DDRTOP_proc_prog0(addr, j, prog0_run_mode);
				addr = addr + (1 << j);
				prog0_run_mode = 0b00;
			}
		}
	}

	/* Clear int_status_ecc */
	DDRTOP_mc_param_wr(INT_ACK_ECC_ADDR, INT_ACK_ECC_OFFSET, INT_ACK_ECC_WIDTH, 0x000001CF);
	DDRTOP_mc_param_poll(INT_STATUS_ECC_ADDR, INT_STATUS_ECC_OFFSET, INT_STATUS_ECC_WIDTH, 0x00000000);

	/* Revert lp_auto_entry_en */
	DDRTOP_mc_param_wr(LP_AUTO_ENTRY_EN_ADDR, LP_AUTO_ENTRY_EN_OFFSET, LP_AUTO_ENTRY_EN_WIDTH, lp_auto_entry_en);

	/* Revert ecc_disable_w_uc_err */
	DDRTOP_mc_param_wr(ECC_DISABLE_W_UC_ERR_ADDR, ECC_DISABLE_W_UC_ERR_OFFSET, ECC_DISABLE_W_UC_ERR_WIDTH, 0);

	udelay(1);
}
