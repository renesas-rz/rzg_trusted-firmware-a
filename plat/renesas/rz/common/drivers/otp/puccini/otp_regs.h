/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __OTP_REGS_H__
#define __OTP_REGS_H__

#include <rz_soc_def.h>						/* Get the OTP base address */


#define OTP_OTPPWR_OFFSET					UL(0x0000)
#define OTP_OTPSTR_OFFSET					UL(0x0004)
#define OTP_OTPADRRD_OFFSET					UL(0x0014)
#define OTP_OTPDATARD_OFFSET				UL(0x0018)
#define OTP_OTPFLAG_OFFSET					UL(0x001C)

#define OTP_OTPPWR							(OTP_BASE + OTP_OTPPWR_OFFSET)
#define OTP_OTPSTR							(OTP_BASE + OTP_OTPSTR_OFFSET)
#define OTP_OTPADRRD						(OTP_BASE + OTP_OTPADRRD_OFFSET)
#define OTP_OTPDATARD						(OTP_BASE + OTP_OTPDATARD_OFFSET)
#define OTP_OTPFLAG							(OTP_BASE + OTP_OTPFLAG_OFFSET)

#define OTP_IP_ADDR_MIN						U(0x0E5)
#define OTP_IP_ADDR_MAX						U(0x3FF)
#define OTP_IP_DUMMY_READ_ADDR				U(0x0200)

/* OTPPWR Register */
#define OTP_OTPPWR_PWR						U(0)
#define OTP_OTPPWR_ACCL						U(4)

/* OTPSTR Register */
#define OTP_OTPSTR_CMD_RDY					U(0)
#define OTP_OTPSTR_ERR_RP					U(4)

/* OTPFLAG Register */
#define OTP_OTPFLAG_FLAG					U(0)
#define OTP_OTPFLAG_RREND					U(1)

#endif /* __OTP_REGS_H__ */
