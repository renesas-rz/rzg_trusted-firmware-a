/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RIIC_REGS_H
#define RIIC_REGS_H

#define RIIC_BASE		(RZ_SOC_I2C_BASE)

#define RIIC_ICCR1		((uintptr_t)RIIC_BASE + 0x00)
#define RIIC_ICCR2		((uintptr_t)RIIC_BASE + 0x01)
#define RIIC_ICMR1		((uintptr_t)RIIC_BASE + 0x02)
#define RIIC_ICMR2		((uintptr_t)RIIC_BASE + 0x03)
#define RIIC_ICMR3		((uintptr_t)RIIC_BASE + 0x04)
#define RIIC_ICFER		((uintptr_t)RIIC_BASE + 0x05)
#define RIIC_ICSER		((uintptr_t)RIIC_BASE + 0x06)
#define RIIC_ICIER		((uintptr_t)RIIC_BASE + 0x07)
#define RIIC_ICSR1		((uintptr_t)RIIC_BASE + 0x08)
#define RIIC_ICSR2		((uintptr_t)RIIC_BASE + 0x09)
#define RIIC_ICSAR0		((uintptr_t)RIIC_BASE + 0x0A)
#define RIIC_ICBRL		((uintptr_t)RIIC_BASE + 0x10)
#define RIIC_ICBRH		((uintptr_t)RIIC_BASE + 0x11)
#define RIIC_ICDRT		((uintptr_t)RIIC_BASE + 0x12)
#define RIIC_ICDRR		((uintptr_t)RIIC_BASE + 0x13)


/* ICCR1 */
#define ICCR1_ICE		(0x80)
#define ICCR1_IICRST	(0x40)
#define ICCR1_CLO		(0x20)
#define ICCR1_SOWP		(0x10)
#define ICCR1_SCLO		(0x08)
#define ICCR1_SDAO		(0x04)
#define ICCR1_SCLI		(0x02)
#define ICCR1_SDAI		(0x01)

/* ICCR2 */
#define ICCR2_BBSY		(0x80)
#define ICCR2_MST		(0x40)
#define ICCR2_TRS		(0x20)
#define ICCR2_SP		(0x08)
#define ICCR2_RS		(0x04)
#define ICCR2_ST		(0x02)

/* ICMR1 */
#define ICMR1_MTWP		(0x80)
#define ICMR1_CKS_MASK	(0x70)
#define ICMR1_BCWP		(0x08)
#define ICMR1_BC_MASK	(0x07)

#define ICMR1_CKS(_x)	((_x << 4) & ICMR1_CKS_MASK)
#define ICMR1_BC(_x)	(_x & ICMR1_BC_MASK)

/* ICMR2 */
#define ICMR2_DLCS		(0x80)
#define ICMR2_SDDL_MASK	(0x70)
#define ICMR2_TMOH		(0x04)
#define ICMR2_TMOL		(0x02)
#define ICMR2_TMOS		(0x01)

/* ICMR3 */
#define ICMR3_SMBS		(0x80)
#define ICMR3_WAIT		(0x40)
#define ICMR3_RDRFS		(0x20)
#define ICMR3_ACKWP		(0x10)
#define ICMR3_ACKBT		(0x08)
#define ICMR3_ACKBR		(0x04)
#define ICMR3_NF_MASK	(0x03)
#define ICMR3_NF_ONE	(0x00)
#define ICMR3_NF_TWO	(0x01)
#define ICMR3_NF_THREE	(0x02)
#define ICMR3_NF_FOUR	(0x03)

/* ICFER */
#define ICFER_FMPE		(0x80)
#define ICFER_SCLE		(0x40)
#define ICFER_NFE		(0x20)
#define ICFER_NACKE		(0x10)
#define ICFER_SALE		(0x08)
#define ICFER_NALE		(0x04)
#define ICFER_MALE		(0x02)
#define ICFER_TMOE		(0x01)

/* ICSER */
#define ICSER_HOAE		(0x80)
#define ICSER_DIDE		(0x20)
#define ICSER_GCAE		(0x08)
#define ICSER_SAR2E		(0x04)
#define ICSER_SAR1E		(0x02)
#define ICSER_SAR0E		(0x01)

/* ICIER */
#define ICIER_TIE		(0x80)
#define ICIER_TEIE		(0x40)
#define ICIER_RIE		(0x20)
#define ICIER_NAKIE		(0x10)
#define ICIER_SPIE		(0x08)
#define ICIER_STIE		(0x04)
#define ICIER_ALIE		(0x02)
#define ICIER_TMOIE		(0x01)

/* ICSR1 */
#define ICSR1_HOA		(0x80)
#define ICSR1_DID		(0x20)
#define ICSR1_GCA		(0x08)
#define ICSR1_AAS2		(0x04)
#define ICSR1_AAS1		(0x02)
#define ICSR1_AAS0		(0x01)

/* ICSR2 */
#define ICSR2_TDRE		(0x80)
#define ICSR2_TEND		(0x40)
#define ICSR2_RDRF		(0x20)
#define ICSR2_NACKF		(0x10)
#define ICSR2_STOP		(0x08)
#define ICSR2_START		(0x04)
#define ICSR2_AL		(0x02)
#define ICSR2_TMOF		(0x01)

/* ICBRH */
#define ICBRH_RESERVED	(0xe0)	/* The write value should always be 1 */
#define ICBRH_BRH_MASK	(0x1f)

/* ICBRL */
#define ICBRL_RESERVED	(0xe0)	/* The write value should always be 1 */
#define ICBRL_BRL_MASK	(0x1f)

#endif /* RIIC_REGS_H */
