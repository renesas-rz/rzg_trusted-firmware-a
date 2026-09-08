/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <lib/mmio.h>
#include <common/runtime_svc.h>
#include <common/debug.h>
#include <smccc_helpers.h>
#include <arch_helpers.h>
#include <rz_soc_def.h>
#include <rz_sip_svc.h>
#include <sys.h>


static uintptr_t rz_otp_handler_chipid(void *handle, u_register_t x1, u_register_t flags)
{
	uint32_t chipid[4];

#if (PROTECTED_CHIPID == 1)
	uint32_t ns = is_caller_non_secure(flags);

	if (ns) {
		WARN("%s: Unauthorized service call from non-secure\n", __func__);
		SMC_RET1(handle, SMC_UNK);
	} else
#endif
	{
		uint64_t chipid_64bbits = sys_get_platform_chipid();

		/* Return 64bit ID in litte endian format */
		chipid[0] = (uint32_t)(chipid_64bbits & 0xFFFFFFFF);
		chipid[1] = (uint32_t)(chipid_64bbits >> 32);
		/* Fillout upper 64bits with zero */
		chipid[2] = 0UL;
		chipid[3] = 0UL;

		SMC_RET4(handle, chipid[0], chipid[1], chipid[2], chipid[3]);
	}
}

static uintptr_t rz_otp_handler_productid(void *handle)
{
	uint32_t productid = sys_get_platform_part_number();

	SMC_RET1(handle, productid);
}

static bool is_rz_otp_tsu_offset(uint32_t offset)
{
	return ((offset >= OTP_TSU_REG_ADR_TEMPHI) && (offset <= OTP_TSU_REG_ADR_TEMPLO));
}

static uintptr_t rz_otp_tsu_get_temp(void *handle, u_register_t x1)
{
	uint32_t val;

	if (is_rz_otp_tsu_offset((uint32_t) x1)) {
		mmio_write_32(RZT2H_OTPPWR, (RZT2H_OTPPWR_ACCL | RZT2H_OTPPWR_PWR));
		while ((mmio_read_32(RZT2H_OTPSTR) & 0x1) != 0x1)
			;
		mmio_write_32(RZT2H_OTPADRRD, (uintptr_t) x1);
		val = mmio_read_32(RZT2H_OTPDATARD);
		mmio_write_32(RZT2H_OTPPWR, 0x0);
		while ((mmio_read_32(RZT2H_OTPSTR) & 0x1) == 0x1)
			;
		SMC_RET1(handle, val);
	} else {
		WARN("%s: Offset address out of OTP-TSU areas\n", __func__);
		SMC_RET1(handle, SMC_ARCH_CALL_INVAL_PARAM);
	}
}

uintptr_t rz_plat_sip_handler(uint32_t smc_fid,
					u_register_t x1,
					u_register_t x2,
					u_register_t x3,
					u_register_t x4,
					void *cookie,
					void *handle,
					u_register_t flags)
{
	switch (smc_fid) {
	case RZ_SIP_SVC_GET_CHIPID:
		return rz_otp_handler_chipid(handle, x1, flags);
	case RZ_SIP_SVC_GET_PRODUCTID:
		return rz_otp_handler_productid(handle);
	case RZ_SIP_SVC_GET_SYSTSU:
		return rz_otp_tsu_get_temp(handle, x1);
	default:
		WARN("%s: Unimplemented RZ SiP Service Call: 0x%x\n", __func__, smc_fid);
		SMC_RET1(handle, SMC_UNK);
	}
}
