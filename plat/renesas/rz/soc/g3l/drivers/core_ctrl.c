/*
 * Copyright (c) 2026, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <arch_helpers.h>
#include <core_ctrl.h>
#include <cpg_regs.h>
#include <lib/mmio.h>
#include <rz_soc_def.h>
#include <platform_def.h>
#include <common/debug.h>
#include <lib/utils_def.h>
#include <sys_regs.h>
#include <common/debug.h>

/* External Reset Application */
#define NUM_STEPS_APPLY_EXT_WARM_RST		(7)
#define NUM_STEPS_RELEASE_EXT_WARM_RST		(6)

typedef enum {
	RST_ACTION_WRITE	= 0,
	RST_ACTION_READ
} rst_action_t;

typedef struct {
	uintptr_t reg;
	uint32_t  step_mask;
	uint32_t  step_value;
	rst_action_t action;
} reset_steps_t;

/* Warm external reset apply steps matrix */
static const reset_steps_t warm_ext_reset_apply[PLATFORM_CORE_COUNT][NUM_STEPS_APPLY_EXT_WARM_RST] = {
	/* Core 0 */
	{
		{ SYS_LP_CA55CK_CTL4,	0x00000001,	0x00000000,	RST_ACTION_READ},
		{ CPG_CORE0_PCHCTL,		0x007F0001,	0x00000001,	RST_ACTION_WRITE},
		{ CPG_CORE0_PCHMON,		0x00000003,	0x00000001,	RST_ACTION_READ},
		{ CPG_CORE0_PCHCTL,		0x007F0001,	0x00000000,	RST_ACTION_WRITE},
		{ CPG_CORE0_PCHMON,		0x00000003,	0x00000000,	RST_ACTION_READ},
		{ CPG_RST_CA55_1,		0x3FF3FFF,	0x00100000,	RST_ACTION_WRITE},
		{ CPG_RSTMON_CA55_1,	0x00000010,	0x00000010,	RST_ACTION_READ}
	},
	/* Core 1 */
	{
		{ SYS_LP_CA55CK_CTL4,	0x00000002,	0x00000000,	RST_ACTION_READ},
		{ CPG_CORE1_PCHCTL,		0x007F0001,	0x00000001,	RST_ACTION_WRITE},
		{ CPG_CORE1_PCHMON,		0x00000003,	0x00000001,	RST_ACTION_READ},
		{ CPG_CORE1_PCHCTL,		0x007F0001,	0x00000000,	RST_ACTION_WRITE},
		{ CPG_CORE1_PCHMON,		0x00000003,	0x00000000,	RST_ACTION_READ},
		{ CPG_RST_CA55_1,		0x3FFF3FFF,	0x00200000,	RST_ACTION_WRITE},
		{ CPG_RSTMON_CA55_1,	0x00000020,	0x00000020,	RST_ACTION_READ}
	},
	/* Core 2 */
	{
		{ SYS_LP_CA55CK_CTL4,	0x00000004,	0x00000000,	RST_ACTION_READ},
		{ CPG_CORE2_PCHCTL,		0x007F0001,	0x00000001,	RST_ACTION_WRITE},
		{ CPG_CORE2_PCHMON,		0x00000003,	0x00000001,	RST_ACTION_READ},
		{ CPG_CORE2_PCHCTL,		0x007F0001,	0x00000000,	RST_ACTION_WRITE},
		{ CPG_CORE2_PCHMON,		0x00000003,	0x00000000,	RST_ACTION_READ},
		{ CPG_RST_CA55_1,		0x3FFF3FFF,	0x00400000,	RST_ACTION_WRITE},
		{ CPG_RSTMON_CA55_1,	0x00000040,	0x00000040,	RST_ACTION_READ}
	},

	/* Core 3 */
	{
		{ SYS_LP_CA55CK_CTL4,	0x00000008,	0x00000000,	RST_ACTION_READ},
		{ CPG_CORE3_PCHCTL,		0x007F0001,	0x00000001,	RST_ACTION_WRITE},
		{ CPG_CORE3_PCHMON,		0x00000003,	0x00000001,	RST_ACTION_READ},
		{ CPG_CORE3_PCHCTL,		0x007F0001,	0x00000000,	RST_ACTION_WRITE},
		{ CPG_CORE3_PCHMON,		0x00000003,	0x00000000,	RST_ACTION_READ},
		{ CPG_RST_CA55_1,		0x3FFF3FFF,	0x00800000,	RST_ACTION_WRITE},
		{ CPG_RSTMON_CA55_1,	0x00000080,	0x00000080,	RST_ACTION_READ}
	}
};

/* Warm external reset release steps matrix */
static  const reset_steps_t warm_ext_reset_release[PLATFORM_CORE_COUNT][NUM_STEPS_RELEASE_EXT_WARM_RST] = {
	/* Core 0 */
	{
		{ CPG_CORE0_PCHCTL,		0x007F0001,	0x00080001,	RST_ACTION_WRITE},
		{ CPG_RST_CA55_1,		0x3FFF3FFF,	0x03FF03FF,	RST_ACTION_WRITE},
		{ CPG_RSTMON_CA55_1,	0x00003FFF,	0x00000000,	RST_ACTION_READ},
		{ CPG_CORE0_PCHMON,		0x00000003,	0x00000001,	RST_ACTION_READ},
		{ CPG_CORE0_PCHCTL,		0x007F0001,	0x00080000,	RST_ACTION_WRITE},
		{ CPG_CORE0_PCHMON,		0x00000003,	0x00000000,	RST_ACTION_READ},

	},
	/* Core 1 */
	{
		{ CPG_CORE1_PCHCTL,		0x007F0001,	0x00080001,	RST_ACTION_WRITE},
		{ CPG_RST_CA55_1,		0x3FFF3FFF,	0x03FF03FF,	RST_ACTION_WRITE},
		{ CPG_RSTMON_CA55_1,	0x00003FFF,	0x00000000,	RST_ACTION_READ},
		{ CPG_CORE1_PCHMON,		0x00000003,	0x00000001,	RST_ACTION_READ},
		{ CPG_CORE1_PCHCTL,		0x007F0001,	0x00080000,	RST_ACTION_WRITE},
		{ CPG_CORE1_PCHMON,		0x00000003,	0x00000000,	RST_ACTION_READ},
	},
	/* Core 2 */
	{
		{ CPG_CORE2_PCHCTL,		0x007F0001,	0x00080001,	RST_ACTION_WRITE},
		{ CPG_RST_CA55_1,		0x3FFF3FFF,	0x03FF03FF,	RST_ACTION_WRITE},
		{ CPG_RSTMON_CA55_1,	0x00003FFF,	0x00000000,	RST_ACTION_READ},
		{ CPG_CORE2_PCHMON,		0x00000003,	0x00000001,	RST_ACTION_READ},
		{ CPG_CORE2_PCHCTL,		0x007F0001,	0x00080000,	RST_ACTION_WRITE},
		{ CPG_CORE2_PCHMON,		0x00000003,	0x00000000,	RST_ACTION_READ},
	},

	/* Core 3 */
	{
		{ CPG_CORE3_PCHCTL,		0x007F0001,	0x00080001,	RST_ACTION_WRITE},
		{ CPG_RST_CA55_1,		0x3FFF3FFF,	0x03FF03FF,	RST_ACTION_WRITE},
		{ CPG_RSTMON_CA55_1,	0x00003FFF,	0x00000000,	RST_ACTION_READ},
		{ CPG_CORE3_PCHMON,		0x00000003,	0x00000001,	RST_ACTION_READ},
		{ CPG_CORE3_PCHCTL,		0x007F0001,	0x00080000,	RST_ACTION_WRITE},
		{ CPG_CORE3_PCHMON,		0x00000003,	0x00000000,	RST_ACTION_READ},
	}
};

static void perform_control_sequence(const reset_steps_t reset_seq[], uint8_t num_steps)
{
	uint8_t i;

	for (i = 0; i < num_steps; i++) {
		reset_steps_t step = reset_seq[i];

		if (step.action == RST_ACTION_WRITE) {
			uint32_t reg_val = mmio_read_32(step.reg);

			reg_val &= (~step.step_mask);
			reg_val |= step.step_value;
			mmio_write_32(step.reg, reg_val);
		} else {
			while ((mmio_read_32(step.reg) & step.step_mask) != step.step_value)
				;
		}
	}
}

void core_ctrl_set_in_standby(uint32_t coreid)
{
	/* Request transition to Cortex-A55 CoreX Sleep Mode */
	mmio_write_32(SYS_LP_CTL1, mmio_read_32(SYS_LP_CTL1) | (SYS_LP_CTL1_CA55SLEEP_REQ << coreid));

	/* Enter the Cortex-A55 Sleep Mode */
	mmio_write_32(SYS_LP_CTL2, mmio_read_32(SYS_LP_CTL2) | SYS_LP_CTL2_CA55_STBYCTL);

	/* Issue Barrier instruction */
	isb();
	dsb();
	dcsw_op_all(DCCISW);
}

void core_ctrl_warm_boot(uint32_t coreid)
{
	/* If in standby, then clear standby mode. */
	if ((mmio_read_32(SYS_LP_CTL1) & (SYS_LP_CTL1_CA55SLEEP_ACK << coreid)) != 0) {
		/* Clear CA55SLEEP_REQ and CA55SLEEP_ACK */
		mmio_write_32(SYS_LP_CTL1, mmio_read_32(SYS_LP_CTL1) & ~((SYS_LP_CTL1_CA55SLEEP_ACK | SYS_LP_CTL1_CA55SLEEP_REQ) << coreid));
	}

	/* if no cores are asleep, then clear the stby bit */
	if ((mmio_read_32(SYS_LP_CTL1) & (SYS_LP_CTL1_CA55SLEEP_ACK_MSK | SYS_LP_CTL1_CA55SLEEP_REQ_MSK)) == 0) {
		mmio_write_32(SYS_LP_CTL2, mmio_read_32(SYS_LP_CTL2) & (~SYS_LP_CTL2_CA55_STBYCTL));
	}

	/*
	 * Warm external reset apply.
	 * Necessary to apply RVBARADDR0/1/2/3 change.
	 */
	perform_control_sequence(&warm_ext_reset_apply[coreid][0], NUM_STEPS_APPLY_EXT_WARM_RST);

	/* Warm external reset release */
	perform_control_sequence(&warm_ext_reset_release[coreid][0], NUM_STEPS_RELEASE_EXT_WARM_RST);
}
