/*
 * Copyright (c) 2024, Renesas Electronics Corporation. All rights reserved.
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
		{ CPG_LP_CA55_CTL6, 0x1,		0x0,		RST_ACTION_READ},
		{ CPG_LP_CA55_CTL2, 0x7F,		0x1,		RST_ACTION_WRITE},
		{ CPG_LP_CA55_CTL2, 0x1FF,		0x101,		RST_ACTION_READ},
		{ CPG_LP_CA55_CTL2, 0x7F,		0x0,		RST_ACTION_WRITE},
		{ CPG_LP_CA55_CTL2, 0x1FF,		0x0,		RST_ACTION_READ},
		{ CPG_RST_0,		0x100010,	0x100000,	RST_ACTION_WRITE},
		{ CPG_RSTMON_0,		0x10,		0x10,		RST_ACTION_READ}
	},
	/* Core 1 */
	{
		{ CPG_LP_CA55_CTL6, 0x2,		0x0,		RST_ACTION_READ},
		{ CPG_LP_CA55_CTL2, 0x7F0000,	0x10000,	RST_ACTION_WRITE},
		{ CPG_LP_CA55_CTL2, 0x1FF0000,	0x1010000,	RST_ACTION_READ},
		{ CPG_LP_CA55_CTL2, 0x7F0000,	0x0,		RST_ACTION_WRITE},
		{ CPG_LP_CA55_CTL2, 0x1FF0000,	0x0,		RST_ACTION_READ},
		{ CPG_RST_0,		0x200020,	0x200000,	RST_ACTION_WRITE},
		{ CPG_RSTMON_0,		0x20,		0x20,		RST_ACTION_READ}
	},
	/* Core 2 */
	{
		{ CPG_LP_CA55_CTL6, 0x4,		0x0,		RST_ACTION_READ},
		{ CPG_LP_CA55_CTL3, 0x7F,		0x1,		RST_ACTION_WRITE},
		{ CPG_LP_CA55_CTL3, 0x1FF,		0x101,		RST_ACTION_READ},
		{ CPG_LP_CA55_CTL3, 0x7F,		0x0,		RST_ACTION_WRITE},
		{ CPG_LP_CA55_CTL3, 0x1FF,		0x0,		RST_ACTION_READ},
		{ CPG_RST_0,		0x400040,	0x400000,	RST_ACTION_WRITE},
		{ CPG_RSTMON_0,		0x40,		0x40,		RST_ACTION_READ}
	},

	/* Core 3 */
	{
		{ CPG_LP_CA55_CTL6, 0x8,		0x0,		RST_ACTION_READ},
		{ CPG_LP_CA55_CTL3, 0x7F0000,	0x10000,	RST_ACTION_WRITE},
		{ CPG_LP_CA55_CTL3, 0x1FF0000,	0x1010000,	RST_ACTION_READ},
		{ CPG_LP_CA55_CTL3, 0x7F0000,	0x0,		RST_ACTION_WRITE},
		{ CPG_LP_CA55_CTL3, 0x1FF0000,	0x0,		RST_ACTION_READ},
		{ CPG_RST_0,		0x800080,	0x800000,	RST_ACTION_WRITE},
		{ CPG_RSTMON_0,		0x80,		0x80,		RST_ACTION_READ}
	}
};

/* Warm external reset release steps matrix */
static  const reset_steps_t warm_ext_reset_release[PLATFORM_CORE_COUNT][NUM_STEPS_RELEASE_EXT_WARM_RST] = {
	/* Core 0 */
	{
		{ CPG_LP_CA55_CTL2,	0x7F,		0x11,		RST_ACTION_WRITE},
		{ CPG_RST_0,		0x3FFF3FFF,	0x3FFF3FFF,	RST_ACTION_WRITE},
		{ CPG_RSTMON_0,		0x1FFFF,	0x0,		RST_ACTION_READ},
		{ CPG_LP_CA55_CTL2,	0x1FF,		0x111,		RST_ACTION_READ},
		{ CPG_LP_CA55_CTL2,	0x7F,		0x10,		RST_ACTION_WRITE},
		{ CPG_LP_CA55_CTL2,	0x1FF,		0x10,		RST_ACTION_READ},

	},
	/* Core 1 */
	{
		{ CPG_LP_CA55_CTL2, 0x7F0000,	0x110000,	RST_ACTION_WRITE},
		{ CPG_RST_0,		0x3FFF3FFF,	0x3FFF3FFF,	RST_ACTION_WRITE},
		{ CPG_RSTMON_0,		0x1FFFF,	0x0,		RST_ACTION_READ},
		{ CPG_LP_CA55_CTL2, 0x1FF0000,	0x1110000,	RST_ACTION_READ},
		{ CPG_LP_CA55_CTL2, 0x7F0000,	0x100000,	RST_ACTION_WRITE},
		{ CPG_LP_CA55_CTL2, 0x1FF0000,	0x100000,	RST_ACTION_READ},
	},
	/* Core 2 */
	{
		{ CPG_LP_CA55_CTL3, 0x7F,		0x11,		RST_ACTION_WRITE},
		{ CPG_RST_0,		0x3FFF3FFF,	0x3FFF3FFF, RST_ACTION_WRITE},
		{ CPG_RSTMON_0,		0x1FFFF,	0x0,		RST_ACTION_READ},
		{ CPG_LP_CA55_CTL3, 0x1FF,		0x111,		RST_ACTION_READ},
		{ CPG_LP_CA55_CTL3, 0x7F,		0x10,		RST_ACTION_WRITE},
		{ CPG_LP_CA55_CTL3, 0x1FF,		0x10,		RST_ACTION_READ},
	},

	/* Core 3 */
	{
		{ CPG_LP_CA55_CTL3, 0x7F0000,	0x110000,	RST_ACTION_WRITE},
		{ CPG_RST_0,		0x3FFF3FFF,	0x3FFF3FFF,	RST_ACTION_WRITE},
		{ CPG_RSTMON_0,		0x1FFFF,	0x0,		RST_ACTION_READ},
		{ CPG_LP_CA55_CTL3, 0x1FF0000,	0x1110000,	RST_ACTION_READ},
		{ CPG_LP_CA55_CTL3, 0x7F0000,	0x100000,	RST_ACTION_WRITE},
		{ CPG_LP_CA55_CTL3, 0x1FF0000,	0x100000,	RST_ACTION_READ},
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
	mmio_write_32(CPG_LP_CTL1, mmio_read_32(CPG_LP_CTL1) | (CPG_LP_CTL1_CA55SLEEP_REQ << coreid));

	/* Issue Barrier instruction */
	isb();
	dsb();
	dcsw_op_all(DCCISW);
}

void core_ctrl_warm_boot(uint32_t coreid)
{
	/* If in standby, then clear standby mode. */
	if ((mmio_read_32(CPG_LP_CTL1) & (CPG_LP_CTL1_CA55SLEEP_ACK << coreid)) != 0) {
		/* Clear CA55SLEEP_REQ and CA55SLEEP_ACK */
		mmio_write_32(CPG_LP_CTL1, mmio_read_32(CPG_LP_CTL1) & ~((CPG_LP_CTL1_CA55SLEEP_ACK | CPG_LP_CTL1_CA55SLEEP_REQ) << coreid));
	}

	/*
	 * Warm external reset apply.
	 * Necessary to apply RVBARADDR0/1/2/3 change.
	 */
	perform_control_sequence(&warm_ext_reset_apply[coreid][0], NUM_STEPS_APPLY_EXT_WARM_RST);

	/* Warm external reset release */
	perform_control_sequence(&warm_ext_reset_release[coreid][0], NUM_STEPS_RELEASE_EXT_WARM_RST);
}
