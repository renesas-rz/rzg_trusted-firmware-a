/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __VBATT_REGS_H__
#define __VBATT_REGS_H__

#include <rz_soc_def.h>				/* Get the VBATT base address */

#define VBATT_BKPSR					(VBATT_BASE + 0x0000)
#define VBATT_TAMPSR				(VBATT_BASE + 0x0004)
#define VBATT_TAMPCR				(VBATT_BASE + 0x0008)
#define VBATT_TCECR					(VBATT_BASE + 0x000C)
#define VBATT_TAMPICR1				(VBATT_BASE + 0x0010)
#define VBATT_TAMPICR2				(VBATT_BASE + 0x0014)
#define VBATT_TAMPIMR				(VBATT_BASE + 0x0018)
#define VBATT_BKSCCR				(VBATT_BASE + 0x001C)
#define VBATT_SOSCCR				(VBATT_BASE + 0x0020)
#define VBATT_SOSCCR2				(VBATT_BASE + 0x0024)
#define VBATT_ISOEN					(VBATT_BASE + 0x0028)
#define VBATT_ISOENPROT				(VBATT_BASE + 0x002C)
#define VBATT_XOSCCR				(VBATT_BASE + 0x0030)
#define VBATT_XOSCCR2				(VBATT_BASE + 0x0034)
#define VBATT_BKR0					(VBATT_BASE + 0x0080)
#define VBATT_BKR1					(VBATT_BASE + 0x0084)
#define VBATT_BKR2					(VBATT_BASE + 0x0088)
#define VBATT_BKR3					(VBATT_BASE + 0x008C)
#define VBATT_BKR4					(VBATT_BASE + 0x0090)
#define VBATT_BKR5					(VBATT_BASE + 0x0094)
#define VBATT_BKR6					(VBATT_BASE + 0x0098)
#define VBATT_BKR7					(VBATT_BASE + 0x009C)
#define VBATT_BKR8					(VBATT_BASE + 0x00A0)
#define VBATT_BKR9					(VBATT_BASE + 0x00A4)
#define VBATT_BKR10					(VBATT_BASE + 0x00A8)
#define VBATT_BKR11					(VBATT_BASE + 0x00AC)
#define VBATT_BKR12					(VBATT_BASE + 0x00B0)
#define VBATT_BKR13					(VBATT_BASE + 0x00B4)
#define VBATT_BKR14					(VBATT_BASE + 0x00B8)
#define VBATT_BKR15					(VBATT_BASE + 0x00BC)
#define VBATT_BKR16					(VBATT_BASE + 0x00C0)
#define VBATT_BKR17					(VBATT_BASE + 0x00C4)
#define VBATT_BKR18					(VBATT_BASE + 0x00C8)
#define VBATT_BKR19					(VBATT_BASE + 0x00CC)
#define VBATT_BKR20					(VBATT_BASE + 0x00D0)
#define VBATT_BKR21					(VBATT_BASE + 0x00D4)
#define VBATT_BKR22					(VBATT_BASE + 0x00D8)
#define VBATT_BKR23					(VBATT_BASE + 0x00DC)
#define VBATT_BKR24					(VBATT_BASE + 0x00E0)
#define VBATT_BKR25					(VBATT_BASE + 0x00E4)
#define VBATT_BKR26					(VBATT_BASE + 0x00E8)
#define VBATT_BKR27					(VBATT_BASE + 0x00EC)
#define VBATT_BKR28					(VBATT_BASE + 0x00F0)
#define VBATT_BKR29					(VBATT_BASE + 0x00F4)
#define VBATT_BKR30					(VBATT_BASE + 0x00F8)
#define VBATT_BKR31					(VBATT_BASE + 0x00FC)

#endif /* __VBATT_REGS_H__ */
