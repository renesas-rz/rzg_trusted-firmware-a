/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __OTP_REGS_H__
#define __OTP_REGS_H__

#include "../drivers/auth/rsip/inc/rsip_drv.h"

#define OTP_ANTI_RB_VER_ADDR						(0x203U)
#define OTP_ANTI_RB_SETTING_ADDR					(0x201U)
#define OTP_ROOT_PUBLIC_KEY_HASH_ADDR				(0x1A8U)
#define OTP_SECURE_BOOT_COMMON_KEY4_ADDR			(0x19CU)
#define OTP_SECURE_BOOT_COMMON_KEY3_ADDR			(0x190U)
#define OTP_SECURE_BOOT_COMMON_KEY2_ADDR			(0x184U)
#define OTP_SECURE_BOOT_COMMON_KEY1_ADDR			(0x178U)

#define OTP_ROOT_PUBLIC_KEY_HASH_SIZE				HW_SCE_ROTPK_HASH_WORD_SIZE
#define OTP_SECURE_BOOT_COMMON_KEY_SIZE				HW_SCE_AES128_KEY_INDEX_WORD_SIZE

#endif /* __OTP_REG_H__ */
