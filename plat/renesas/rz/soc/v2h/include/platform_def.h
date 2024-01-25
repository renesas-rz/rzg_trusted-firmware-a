/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PLATFORM_DEF_H
#define PLATFORM_DEF_H

#ifndef __ASSEMBLER__
#include <stdlib.h>
#endif

#include <arch.h>
#include <rz_soc_def.h>

/*******************************************************************************
 * Platform binary types for linking
 ******************************************************************************/
#define PLATFORM_LINKER_FORMAT	"elf64-littleaarch64"
#define PLATFORM_LINKER_ARCH	aarch64

/*******************************************************************************
 * Generic platform constants
 ******************************************************************************/
#define PLATFORM_STACK_SIZE		U(0x1000)

#define PLATFORM_SYSTEM_COUNT	U(1)
#define PLATFORM_CLUSTER_COUNT	U(1)
#define PLATFORM_CORE_COUNT		U(4)

#define PLAT_MAX_PWR_LVL		MPIDR_AFFLVL2
#define PLAT_NUM_PWR_DOMAINS	(PLATFORM_CORE_COUNT + \
								 PLATFORM_CLUSTER_COUNT + \
								 PLATFORM_SYSTEM_COUNT)

#define PLAT_MAX_RET_STATE		U(1)
#define PLAT_MAX_OFF_STATE		U(2)
#define PLAT_MAX_PWR_LVL_STATES	U(2)

#define MAX_IO_DEVICES			U(3)
#define MAX_IO_HANDLES			U(2)
#define MAX_IO_BLOCK_DEVICES	U(1)

/*******************************************************************************
 * BL2 specific defines.
 ******************************************************************************/
#define BL2_BASE				UL(0x08103000)
#define BL2_LIMIT				UL(0x08163000)

/*******************************************************************************
 * BL31 specific defines.
 ******************************************************************************/
#define BL31_BASE				UL(0x44000000)
#define BL31_LIMIT				UL(0x44040000)

#define PLAT_TRUSTED_MAILBOX_BASE		BL31_LIMIT

#define BL31_SRAM_BASE			UL(0x08078000)
#define BL31_SRAM_LIMIT			UL(0x0807F000)

/*******************************************************************************
 * BL32 specific defines.
 ******************************************************************************/
#ifndef SPD_none
#define BL32_BASE				UL(0x44100000)
#define BL32_LIMIT				(BL32_BASE + 0x100000)
#endif

/*******************************************************************************
 * BL33
 ******************************************************************************/
#define BL33_BASE				UL(0x50000000)
#define BL33_LIMIT				(BL33_BASE + 0x08000000)

/*******************************************************************************
 * Platform specific page table and MMU setup constants
 ******************************************************************************/
#if IMAGE_BL2
#define MAX_XLAT_TABLES			U(8)
#define MAX_MMAP_REGIONS		U(11)
#elif IMAGE_BL31
#define MAX_XLAT_TABLES			U(6)
#define MAX_MMAP_REGIONS		U(9)
#endif

#define PLAT_VIRT_ADDR_SPACE_SIZE	(1ULL << 36)
#define PLAT_PHY_ADDR_SPACE_SIZE	(1ULL << 36)		/* Max Physical Address is 0xF_FFFF_FFFF */

/*******************************************************************************
 * Declarations and constants to access the mailboxes safely. Each mailbox is
 * aligned on the biggest cache line size in the platform. This is known only
 * to the platform as it might have a combination of integrated and external
 * caches. Such alignment ensures that two maiboxes do not sit on the same cache
 * line at any cache level. They could belong to different cpus/clusters &
 * get written while being protected by different locks causing corruption of
 * a valid mailbox address.
 ******************************************************************************/
#define CACHE_WRITEBACK_SHIFT		U(6)
#define CACHE_WRITEBACK_GRANULE		(U(1) << CACHE_WRITEBACK_SHIFT)

#endif /* PLATFORM_DEF_H */
