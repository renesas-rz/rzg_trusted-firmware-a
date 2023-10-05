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
#include <sys_regs.h>


static bool is_rz_sys_pcie_offset(uint32_t offset)
{
	if ((offset >= SYS_PCIE_REG_OFFSET_START) && (offset <= SYS_PCIE_REG_OFFSET_END))
		return true;
	else
		return false;
}

static uintptr_t rz_sys_pcie_get_val(void *handle, u_register_t x1)
{
	uint32_t val;

	if (is_rz_sys_pcie_offset((uint32_t) x1)) {
		val = mmio_read_32(RZV2H_SYSC_BASE + (uintptr_t) x1);
		SMC_RET1(handle, val);
	} else {
		WARN("%s: Offset address out of SYS-PCIe areas\n", __func__);
		SMC_RET1(handle, SMC_ARCH_CALL_INVAL_PARAM);
	}
}

static uintptr_t rz_sys_pcie_set_val(void *handle, u_register_t x1, u_register_t x2)
{
	if (is_rz_sys_pcie_offset((uint32_t) x1)) {
		mmio_write_32(RZV2H_SYSC_BASE + (uintptr_t) x1, (uint32_t) x2);
		SMC_RET0(handle);
	} else {
		WARN("%s: Offset address out of SYS-PCIe areas\n", __func__);
		SMC_RET1(handle, SMC_ARCH_CALL_INVAL_PARAM);
	}
}

static uintptr_t rz_otp_handler_chipid(void *handle, u_register_t x1, u_register_t flags)
{
	uint32_t chipid[4];

#if (PROTECTED_CHIPID == 1)
	uint32_t ns = is_caller_non_secure(flags);

	if (ns) {
		WARN("%s: Unauthorized service call from non-secure\n", __func__);
		SMC_RET1(handle, SMC_UNK);
	}
#endif

	chipid[0] = mmio_read_32(RZ_SOC_OTP_BASE_CHIPID + 0x0);
	chipid[1] = mmio_read_32(RZ_SOC_OTP_BASE_CHIPID + 0x4);
	chipid[2] = mmio_read_32(RZ_SOC_OTP_BASE_CHIPID + 0x8);
	chipid[3] = mmio_read_32(RZ_SOC_OTP_BASE_CHIPID + 0xC);

	SMC_RET4(handle, chipid[0], chipid[1], chipid[2], chipid[3]);
}

static uintptr_t rz_otp_handler_productid(void *handle)
{
	uint32_t productid = mmio_read_32(RZ_SOC_OTP_BASE_PRODUCTID);

	SMC_RET1(handle, productid);
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
	case RZ_SIP_SVC_GET_SYSPCIE:
		return rz_sys_pcie_get_val(handle, x1);
	case RZ_SIP_SVC_SET_SYSPCIE:
		return rz_sys_pcie_set_val(handle, x1, x2);
	default:
		WARN("%s: Unimplemented RZ SiP Service Call: 0x%x\n", __func__, smc_fid);
		SMC_RET1(handle, SMC_UNK);
	}
}
