/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include <lib/mmio.h>
#include <sys.h>
#include <sys_regs.h>
#include <common/debug.h>
#include <otp_drv.h>
#include <otp_data.h>
#include <otp.h>


boot_mode_t sys_get_boot_mode(void)
{
	uint8_t boot_dev  = (mmio_read_32(MD_MON) >> MD_MON_MD0MON_BIT) & MD_MON_MDxMON_MSK;
	boot_mode_t boot_mode;

	switch (boot_dev) {
	case (MD_MON_MODE_ESD):
		boot_mode = SYS_BOOT_MODE_ESD;
		break;

	case (MD_MON_MODE_EMMC):
		boot_mode = SYS_BOOT_MODE_EMMC;
		break;

	case (MD_MON_MODE_XSPI0_x1):
		boot_mode = SYS_BOOT_MODE_XSPI0_x1;
		break;

	case (MD_MON_MODE_XSPI0_x8):
		boot_mode = SYS_BOOT_MODE_XSPI0_x8;
		break;

	case (MD_MON_MODE_XSPI1_x1):
		boot_mode = SYS_BOOT_MODE_XSPI1_x1;
		break;

	default:
		panic();
	}

	return boot_mode;
}

bool sys_get_is_3v3(uint32_t md_mon_mdv_lat)
{
	return (MD_MON_MODE_MDV_3V3 == ((mmio_read_32(MD_MON) >> md_mon_mdv_lat) & MD_MON_MDV_LAT_MSK));
}

void sys_base_unlock(uint32_t unlock_mask)
{
	uint32_t prcrn;

	/* Unlock PRCRN bit(s) in given mask */
	prcrn = mmio_read_32(PRCRN) & 0x0000000FU;
	mmio_write_32(PRCRN, (prcrn | 0x0000A500U | unlock_mask));
}

void sys_base_lock(uint32_t lock_mask)
{
	uint32_t prcrn;

	/* Lock PRCRN bit(s) in given mask */
	prcrn = mmio_read_32(PRCRN) & 0x0000000FU & (~lock_mask);
	mmio_write_32(PRCRN, (prcrn | 0x0000A500U));

}

void sys_safetybase_unlock(uint32_t unlock_mask)
{
	uint32_t prcrs;

	/* Unlock PRCRS bit(s) in given mask */
	prcrs = mmio_read_32(PRCRS) & 0x0000000FU;
	mmio_write_32(PRCRS, (prcrs  | 0x0000A500U | unlock_mask));
}

void sys_safetybase_lock(uint32_t lock_mask)
{
	uint32_t prcrs;

	/* Lock PRCRS bit(s) in given mask */
	prcrs = mmio_read_32(PRCRS) & 0x0000000FU & (~lock_mask);
	mmio_write_32(PRCRS, (prcrs | 0x0000A500U));
}

uint64_t sys_get_platform_chipid(void)
{
	uint64_t chipid = 0U;

	(void)r_otp_read_wrapper(OTP_UNIQUE_ID_ADDR, (uint16_t *)&chipid, OTP_UNIQUE_ID_ADDR_LEN);

	return chipid;
}

uint32_t sys_get_platform_part_number(void)
{
	uint32_t part_number = 0U;

	(void)r_otp_read_wrapper(OTP_PN_ADDR, (uint16_t *)&part_number, OTP_PN_ADDR_LEN);

	return part_number;
}

uint32_t sys_get_platform_product_version(void)
{
	uint32_t product_version = 0U;

	(void)r_otp_read_wrapper(OTP_PRODUCT_VER_ADDR, (uint16_t *)&product_version, OTP_PRODUCT_VER_LEN);

	return product_version;
}

uint32_t sys_get_platform_core_count(void)
{
	uint32_t core_count = 0U;	/* Zero is an impossible value for the core count */
	uint32_t part_number = sys_get_platform_part_number();

	/* TO-DO- To be updated once part numbers have been provided*/
	switch (part_number) {
	/* RZT2N 537 package*/
	case OTP_PN_RZT2N_537_DUAL_ESC_HPSW_CANFD_SEC_R9A07G076M48GBG:
	case OTP_PN_RZT2N_537_DUAL_ESC_CANFD_SEC_R9A07G076M47GBG:
	case OTP_PN_RZT2N_537_DUAL_CAN_SEC_R9A07G076M45GBG:
	case OTP_PN_RZT2N_537_DUAL_ESC_HPSW_CANFD_R9A07G076M43GBG: {
		core_count = 2U;
	}
	break;

	case OTP_PN_RZT2N_537_SINGLE_ESC_CANFD_SEC_R9A07G076M37GBG:
	case OTP_PN_RZT2N_537_SINGLE_CAN_SEC_R9A07G076M35GBG:
	case OTP_PN_RZT2N_537_SINGLE_CAN_R9A07G076M30GBG: {
		core_count = 1U;
	}
	break;

	/* RZT2N 489 package*/
	case OTP_PN_RZT2N_489_DUAL_ESC_HPSW_CANFD_SEC_R9A07G076M48GBA:
	case OTP_PN_RZT2N_489_DUAL_ESC_CANFD_SEC_R9A07G076M47GBA:
	case OTP_PN_RZT2N_489_DUAL_CAN_SEC_R9A07G076M45GBA:
	case OTP_PN_RZT2N_489_DUAL_ESC_HPSW_CANFD_R9A07G076M43GBA: {
		core_count = 2U;
	}
	break;

	case OTP_PN_RZT2N_489_SINGLE_ESC_CANFD_SEC_R9A07G076M37GBA:
	case OTP_PN_RZT2N_489_SINGLE_CAN_SEC_R9A07G076M35GBA:
	case OTP_PN_RZT2N_489_SINGLE_CAN_R9A07G076M30GBA: {
		core_count = 1U;
	}
	break;

	/* Part Number not recognised */
	default: {
		/* Zero is an impossible value for the core count */
		core_count = 0U;
	}
	break;
	};

	return core_count;
}

bool sys_check_platform_core_count(void)
{
	uint32_t core_count = sys_get_platform_core_count();
	bool res = true;

	/* Check Hardware reported core count against build core count */
	if (core_count != PLATFORM_CORE_COUNT) {
		ERROR("Reported Core count (%d) is different from PLATFORM_CORE_COUNT (%d)\n", core_count, PLATFORM_CORE_COUNT);
		res = false;
	}

	return res;
}

void sys_read_modify_32bits(uint32_t addr, uint32_t data, uint32_t mask)
{
	mmio_write_32(addr, ((mmio_read_32(addr) & (~(uint32_t)mask)) | ((uint32_t)mask & (uint32_t)data)));
}
