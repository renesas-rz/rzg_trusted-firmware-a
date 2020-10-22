/*
 * Copyright (c) 2015-2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RZG_PRIVATE_H
#define RZG_PRIVATE_H

#include <platform_def.h>

#include <common/bl_common.h>
#include <lib/bakery_lock.h>
#include <lib/el3_runtime/cpu_data.h>

typedef volatile struct mailbox {
	unsigned long value __aligned(CACHE_WRITEBACK_GRANULE);
} mailbox_t;

/*
 * This structure represents the superset of information that is passed to
 * BL31 e.g. while passing control to it from BL2 which is bl31_params
 * and bl31_plat_params and its elements
 */
typedef struct bl2_to_bl31_params_mem {
	image_info_t bl32_image_info;
	image_info_t bl33_image_info;
	entry_point_info_t bl33_ep_info;
	entry_point_info_t bl32_ep_info;
} bl2_to_bl31_params_mem_t;

#if USE_COHERENT_MEM
#define RZG_INSTANTIATE_LOCK	DEFINE_BAKERY_LOCK(rzg_lock);
#define rzg_lock_init()	bakery_lock_init(&rzg_lock)
#define rzg_lock_get()		bakery_lock_get(&rzg_lock)
#define rzg_lock_release()	bakery_lock_release(&rzg_lock)
#else
/*
 * Constants to specify how many bakery locks this platform implements. These
 * are used if the platform chooses not to use coherent memory for bakery lock
 * data structures.
 */
#define RZG_MAX_BAKERIES	2
#define RZG_PWRC_BAKERY_ID	0

/*
 * Definition of structure which holds platform specific per-cpu data. Currently
 * it holds only the bakery lock information for each cpu. Constants to
 * specify how many bakeries this platform implements and bakery ids are
 * specified in rzg_def.h
 */
typedef struct rzg_cpu_data {
	bakery_info_t pcpu_bakery_info[RZG_MAX_BAKERIES];
} rzg_cpu_data_t;

#define RZG_CPU_DATA_LOCK_OFFSET	\
	__builtin_offsetof(rzg_cpu_data_t, pcpu_bakery_info)
/*
 * Helper macros for bakery lock api when using the above rzg_cpu_data_t for
 * bakery lock data structures. It assumes that the bakery_info is at the
 * beginning of the platform specific per-cpu data.
 */
#define rzg_lock_init(_lock_arg)

#define rzg_lock_get(_lock_arg) 					\
	bakery_lock_get(_lock_arg, 					\
		CPU_DATA_PLAT_PCPU_OFFSET + RZG_CPU_DATA_LOCK_OFFSET)

#define rzg_lock_release(_lock_arg)					\
	bakery_lock_release(_lock_arg,	    				\
		CPU_DATA_PLAT_PCPU_OFFSET + RZG_CPU_DATA_LOCK_OFFSET)
/* Ensure that the size of the RZG specific per-cpu data structure and the size
 * of the memory allocated in generic per-cpu data for the platform are the same
 */
CASSERT(PLAT_PCPU_DATA_SIZE == sizeof(rzg_cpu_data_t),
	rzg_pcpu_data_size_mismatch);
#endif
/*
 * Function and variable prototypes
 */
void rzg_configure_mmu_el3(unsigned long total_base,
			    unsigned long total_size,
			    unsigned long ro_start, unsigned long ro_limit
#if USE_COHERENT_MEM
			    , unsigned long coh_start, unsigned long coh_limit
#endif
    );

void rzg_setup_topology(void);
void rzg_cci_disable(void);
void rzg_cci_enable(void);
void rzg_cci_init(void);

void plat_invalidate_icache(void);
void plat_cci_disable(void);
void plat_cci_enable(void);
void plat_cci_init(void);

void mstpcr_write(uint32_t mstpcr, uint32_t mstpsr, uint32_t target_bit);
void cpg_write(uintptr_t regadr, uint32_t regval);

void rzg_console_boot_init(void);
void rzg_console_boot_end(void);
void rzg_console_runtime_init(void);
void rzg_console_runtime_end(void);

#endif /* RZG_PRIVATE_H */
