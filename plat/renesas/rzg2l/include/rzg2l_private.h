/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RZG2L_PRIVATE_H__
#define __RZG2L_PRIVATE_H__

#include <common/bl_common.h>

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

/* plat_security.c */
void plat_tzc400_setup(uintptr_t tzc_base);

/*
 * This structure represents the superset of information that is passed to
 * BL31 e.g. while passing control to it from BL2 which is bl31_params
 * and bl31_plat_params and its elements
 */
typedef struct bl2_to_bl31_params_mem {
	entry_point_info_t bl33_ep_info;
} bl2_to_bl31_params_mem_t;

#endif	/* __RZG2L_PRIVATE_H__ */
