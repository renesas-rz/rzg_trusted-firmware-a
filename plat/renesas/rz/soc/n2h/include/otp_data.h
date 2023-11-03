/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __OTP_DATA_H__
#define __OTP_DATA_H__

#define OTP_UNIQUE_ID_ADDR								U(0x00E5)
#define OTP_PN_ADDR										U(0x01D9)

#define OTP_UNIQUE_ID_ADDR_LEN							U(2)
#define OTP_PN_ADDR_LEN									U(1)

#define OTP_PN_RZN2H_QUAD_SEC_R9A09G077M48GBG			UL(0x00003000)
#define OTP_PN_RZN2H_DUAL_SEC_R9A09G077M28GBG			UL(0x00003001)
#define OTP_PN_RZN2H_SINGLE_SEC_R9A09G077M08GBG			UL(0x00003002)
#define OTP_PN_RZN2H_QUAD_R9A09G077M44GBG				UL(0x00003003)
#define OTP_PN_RZN2H_DUAL_R9A09G077M24GBG				UL(0x00003004)
#define OTP_PN_RZN2H_SINGLE_R9A09G077M04GBG				UL(0x00003005)

#endif /* __OTP_DATA_H__ */
