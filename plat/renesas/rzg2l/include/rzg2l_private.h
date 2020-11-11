/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RZG2L_PRIVATE_H__
#define __RZG2L_PRIVATE_H__

#include <platform_def.h>

#define RZG2L_DEVICE_BASE			(0x10000000)
#define RZG2L_SCIF0_BASE			(0x1004B800)
#define RZG2L_SPIMULT_BASE			(0x10060000)
#define RZG2L_SPIMULT_WBUF_BASE		(0x10070000)
#define RZG2L_SYC_BASE				(0x11020000)
#define RZG2L_CPG_BASE				(0x11010000)
#define RZG2L_SYSC_BASE				(0x11020000)
#define RZG2L_GPIO_BASE				(0x11030000)
#define RZG2L_TZC_SPI_BASE			(0x11060000)
#define RZG2L_TZC_DDR_BASE			(0x11070000)
#define RZG2L_DDR_PHY_BASE			(0x11400000)
#define RZG2L_DDR_MEMC_BASE			(0x11410000)
#define RZG2L_GIC_BASE				(0x11900000)
#define RZG2L_SD0_BASE				(0x11C00000)
#define RZG2L_SPIROM_BASE			(0x20000000)
#define RZG2L_DDR1_BASE				(0x40000000)
#define RZG2L_DDR2_BASE				(0x80000000)
#define RZG2L_DDR3_BASE				(0x100000000)

#define RZG2L_DEVICE_SIZE			(0x15000000 - RZG2L_DEVICE_BASE)
#define RZG2L_SPIROM_SIZE			(0x30000000 - RZG2L_SPIROM_BASE)
#define RZG2L_DDR1_SIZE				(RZG2L_DDR2_BASE - RZG2L_DDR1_BASE)
#define RZG2L_DDR2_SIZE				(RZG2L_DDR3_BASE - RZG2L_DDR2_BASE)

#define RZG2L_GICD_BASE				(RZG2L_GIC_BASE)
#define RZG2L_GICR_BASE				(RZG2L_GIC_BASE + 0x00040000)

#define RZG2L_SYC_INCK_HZ			(24000000)
#define RZG2L_UART_INCK_HZ			(100000000)
#define RZG2L_UART_BARDRATE			(115200)


/* plat_gicv3.c */
void plat_gic_driver_init(void);
void plat_gic_init(void);

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
	image_info_t bl33_image_info;
	entry_point_info_t bl33_ep_info;
} bl2_to_bl31_params_mem_t;

#if USE_COHERENT_MEM
#define RZG2L_INSTANTIATE_LOCK	DEFINE_BAKERY_LOCK(rzg2l_lock);
#define rzg2l_lock_init()	bakery_lock_init(&rzg2l_lock)
#define rzg2l_lock_get()		bakery_lock_get(&rzg2l_lock)
#define rzg2l_lock_release()	bakery_lock_release(&rzg2l_lock)
#else
/*
 * Constants to specify how many bakery locks this platform implements. These
 * are used if the platform chooses not to use coherent memory for bakery lock
 * data structures.
 */
#define RZG2L_MAX_BAKERIES	2
#define RZG2L_PWRC_BAKERY_ID	0

/*
 * Definition of structure which holds platform specific per-cpu data. Currently
 * it holds only the bakery lock information for each cpu. Constants to
 * specify how many bakeries this platform implements and bakery ids are
 * specified in rzg2l_def.h
 */
typedef struct rzg2l_cpu_data {
	bakery_info_t pcpu_bakery_info[RZG2L_MAX_BAKERIES];
} rzg2l_cpu_data_t;

#define RZG2L_CPU_DATA_LOCK_OFFSET	\
	__builtin_offsetof(rzg2l_cpu_data_t, pcpu_bakery_info)
/*
 * Helper macros for bakery lock api when using the above rzg2l_cpu_data_t for
 * bakery lock data structures. It assumes that the bakery_info is at the
 * beginning of the platform specific per-cpu data.
 */
#define rzg2l_lock_init(_lock_arg)

#define rzg2l_lock_get(_lock_arg) 					\
	bakery_lock_get(_lock_arg, 					\
		CPU_DATA_PLAT_PCPU_OFFSET + RZG2L_CPU_DATA_LOCK_OFFSET)

#define rzg2l_lock_release(_lock_arg)					\
	bakery_lock_release(_lock_arg,	    				\
		CPU_DATA_PLAT_PCPU_OFFSET + RZG2L_CPU_DATA_LOCK_OFFSET)
/* Ensure that the size of the RZG2L specific per-cpu data structure and the size
 * of the memory allocated in generic per-cpu data for the platform are the same
 */
/* CASSERT(PLAT_PCPU_DATA_SIZE == sizeof(rzg2l_cpu_data_t),
	rzg2l_pcpu_data_size_mismatch); */
#endif
/*
 * Function and variable prototypes
 */
void rzg2l_configure_mmu_el3(unsigned long total_base,
			    unsigned long total_size,
			    unsigned long ro_start, unsigned long ro_limit
#if USE_COHERENT_MEM
			    , unsigned long coh_start, unsigned long coh_limit
#endif
    );

void rzg2l_setup_topology(void);
void rzg2l_cci_disable(void);
void rzg2l_cci_enable(void);
void rzg2l_cci_init(void);

void plat_invalidate_icache(void);
void plat_cci_disable(void);
void plat_cci_enable(void);
void plat_cci_init(void);

void mstpcr_write(uint32_t mstpcr, uint32_t mstpsr, uint32_t target_bit);
void cpg_write(uintptr_t regadr, uint32_t regval);

void rzg2l_console_boot_init(void);
void rzg2l_console_boot_end(void);
void rzg2l_console_runtime_init(void);
void rzg2l_console_runtime_end(void);


#endif	/* __RZG2L_PRIVATE_H__ */
