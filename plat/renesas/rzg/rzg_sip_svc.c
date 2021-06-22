/*
 * Copyright (c) 2016-2017, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/runtime_svc.h>
#include <common/debug.h>
#include <smccc_helpers.h>
#include <string.h>
#include <arch_helpers.h>
#include "rzg_sip_svc.h"

/*
 * This function handles RZG defined SiP Calls
 */
static uintptr_t rzg_sip_handler(unsigned int smc_fid,
			u_register_t x1,
			u_register_t x2,
			u_register_t x3,
			u_register_t x4,
			void *cookie,
			void *handle,
			u_register_t flags)
{
	int32_t		arg[2];

	switch (smc_fid) {
		case RZG_SIP_SVC_GET_ECC_MODE:
			arg[0] = RZG_DRAM_ECC;
			arg[1] = RZG_DRAM_ECC_FULL;
			SMC_RET2(handle, arg[0], arg[1]);

		default:
			WARN("Unimplemented RZG SiP Service Call: ");
			WARN("0x%x \n", smc_fid);
			SMC_RET1(handle, SMC_UNK);
	}
}


/* Define a runtime service descriptor for fast SMC calls */
DECLARE_RT_SVC(
	rzg_sip_svc,
	OEN_SIP_START,
	OEN_SIP_END,
	SMC_TYPE_FAST,
	NULL,
	rzg_sip_handler
);
