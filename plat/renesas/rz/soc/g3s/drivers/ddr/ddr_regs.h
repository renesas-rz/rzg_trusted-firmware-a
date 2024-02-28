/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __DDR_REGS_H__
#define __DDR_REGS_H__

#include <platform_def.h>

#define	DDR_PHY_BASE				((uintptr_t)RZ_SOC_DDR_PHY_BASE)
#define	DDR_MC_BASE					((uintptr_t)RZ_SOC_DDR_MC_BASE)

#define INT_STATUS_INIT_ADDR		(370)
#define INT_STATUS_INIT_OFFSET		(24)
#define INT_STATUS_INIT_WIDTH		(8)
#define LPI_WAKEUP_EN_ADDR			(192)
#define LPI_WAKEUP_EN_OFFSET		(8)
#define LPI_WAKEUP_EN_WIDTH			(6)
#define PHY_INDEP_INIT_MODE_ADDR	(22)
#define PHY_INDEP_INIT_MODE_OFFSET	(8)
#define PHY_INDEP_INIT_MODE_WIDTH	(1)
#define DFIBUS_FREQ_F0_ADDR			(22)
#define DFIBUS_FREQ_F0_OFFSET		(16)
#define DFIBUS_FREQ_F0_WIDTH		(5)
#define PWRUP_SREFRESH_EXIT_ADDR	(133)
#define PWRUP_SREFRESH_EXIT_OFFSET	(16)
#define PWRUP_SREFRESH_EXIT_WIDTH	(1)
#define LP_CMD_ADDR					(185)
#define LP_CMD_OFFSET				(8)
#define LP_CMD_WIDTH				(7)
#define LP_STATE_ADDR				(194)
#define LP_STATE_OFFSET				(8)
#define LP_STATE_WIDTH				(7)
#define CONTROLLER_BUSY_ADDR		(356)
#define CONTROLLER_BUSY_OFFSET		(8)
#define CONTROLLER_BUSY_WIDTH		(1)
#define START_ADDR					(0)
#define START_OFFSET				(0)
#define START_WIDTH					(1)
#define DRAM_CLASS_ADDR				(0)
#define DRAM_CLASS_OFFSET			(8)
#define DRAM_CLASS_WIDTH			(4)
#define TDFI_WRDATA_DELAY_ADDR		(579)
#define TDFI_WRDATA_DELAY_OFFSET	(16)
#define TDFI_WRDATA_DELAY_WIDTH		(8)
#define TDFI_PHY_RDLAT_F0_ADDR		(550)
#define TDFI_PHY_RDLAT_F0_OFFSET	(0)
#define TDFI_PHY_RDLAT_F0_WIDTH		(8)
#define MEM_DP_REDUCTION_ADDR		(350)
#define MEM_DP_REDUCTION_OFFSET		(8)
#define MEM_DP_REDUCTION_WIDTH		(1)
#define BSTLEN_ADDR					(67)
#define BSTLEN_OFFSET				(16)
#define BSTLEN_WIDTH				(5)

#endif /* __DDR_REGS_H__ */
