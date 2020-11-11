/*
 * Copyright (c) 2015-2019, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RZG2L_DEF_H
#define RZG2L_DEF_H

#define RZG2L_PRIMARY_CPU			(0x0)
#define RZG2L_TRUSTED_SRAM_BASE		(0x44000000)
#define RZG2L_TRUSTED_SRAM_SIZE		(0x0003E000)
#define RZG2L_SHARED_MEM_BASE		(RZG2L_TRUSTED_SRAM_BASE + RZG2L_TRUSTED_SRAM_SIZE)
#define RZG2L_SHARED_MEM_SIZE		U(0x00001000)
#define FLASH0_BASE					U(0x2001D200)
#define FLASH0_SIZE					((0x20000000 + 0x10000000) - FLASH0_BASE)
/* Entrypoint mailboxes */
#define MBOX_BASE					(RZG2L_SHARED_MEM_BASE)
#define MBOX_SIZE					(0x200)

/* Base address where parameters to BL31 are stored */
#define PARAMS_BASE					(MBOX_BASE + MBOX_SIZE)


#endif /* RZG2L_DEF_H */
