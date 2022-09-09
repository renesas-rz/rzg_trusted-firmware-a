/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RZ_SOC_DEF_H__
#define __RZ_SOC_DEF_H__

#define RZV2H_BOOT_ROM_BASE         (0x00000000)
#define RZV2H_MSRAM_BASE            (0x00010000)	//TODO: KTG: Update
#define RZV2H_ASRAM_BASE            (0x00020000)	//TODO: KTG: Update
#define RZV2H_DEVICE_BASE           (0x10000000)
#define RZV2H_SCIF0_BASE            (0x11C01400)
#define RZV2H_SPIMULT_BASE          (0x12800000)
#define RZV2H_SPIMULT_WBUF_BASE     (0x10070000)	//TODO: KTG: Update
#define RZV2H_SYC_BASE              (0x14010000)
#define RZV2H_CPG_BASE              (0x10420000)
#define RZV2H_SYSC_BASE             (0x10430000)	//TODO: KTG: Confirm
#define RZV2H_GPIO_BASE             (0x10410000)
#define RZV2H_TZC_ASRAM_BASE        (0x13CB0000)	//TODO: KTG: Confirm
#define RZV2H_TZC_MSRAM_BASE        (0x10460000)	//TODO: KTG: Confirm
#define RZV2H_TZC_SPI_BASE          (0x10470000)	//TODO: KTG: Confirm
#define RZV2H_TZC_DDR00_BASE        (0x13C60000)
#define RZV2H_TZC_DDR01_BASE        (0x13C70000)
#define RZV2H_TZC_DDR10_BASE        (0x13C80000)
#define RZV2H_TZC_DDR11_BASE        (0x13C90000)
#define RZV2H_DDR0_PHY_BASE         (0x1A000000)
#define RZV2H_DDR1_PHY_BASE         (0x1C000000)
#define RZV2H_DDR0_MEMC_BASE        (0x1E000000)
#define RZV2H_DDR1_MEMC_BASE        (0x1E010000)

#define RZV2H_DDR_MEMC_BASE 		(0x1E000000)	//TODO: KTG: Remove, temporary scaffold code
#define RZV2H_DDR_PHY_BASE			(0x1A000000)	//TODO: KTG: Remove, temporary scaffold code

#define RZV2H_OTP_BASE              (0x10450000)
#define RZV2H_GIC_BASE              (0x14900000)
#define RZV2H_SD0_BASE              (0x15C00000)	//TODO: KTG: Confirm
#define RZV2H_SPIROM_BASE           (0x20000000)	//TODO: KTG: Update
#define RZV2H_DDR1_BASE             (0x40000000)	//TODO: KTG: Confirm
#define RZV2H_DDR2_BASE             (0x140000000)	//TODO: KTG: Confirm
#define RZV2H_DDR3_BASE             (0x240000000)	//TODO: KTG: Confirm
#define RZV2H_DDR4_BASE             (0x340000000)	//TODO: KTG: Confirm

#define RZV2H_GICD_BASE             (RZV2H_GIC_BASE)				//TODO: KTG: Update
#define RZV2H_GICR_BASE             (RZV2H_GIC_BASE + 0x00040000)	//TODO: KTG: Update

#define RZV2H_SRAM_BASE             (RZV2H_MSRAM_BASE)

#define RZV2H_BOOT_ROM_SIZE         (0x00010000 - RZV2H_BOOT_ROM_BASE)		//TODO: KTG: Update
#define RZV2H_MSRAM_SIZE            (0x00020000 - RZV2H_MSRAM_BASE)			//TODO: KTG: Update
#define RZV2H_ASRAM_SIZE            (0x00030000 - RZV2H_ASRAM_BASE)			//TODO: KTG: Update
#define RZV2H_SRAM_SIZE             (RZV2H_MSRAM_SIZE + RZV2H_ASRAM_SIZE)	//TODO: KTG: Update
#define RZV2H_DEVICE_SIZE           (0x15000000 - RZV2H_DEVICE_BASE)		//TODO: KTG: Update
#define RZV2H_SPIROM_SIZE           (0x30000000 - RZV2H_SPIROM_BASE)		//TODO: KTG: Update
#define RZV2H_DDR1_SIZE             (RZV2H_DDR2_BASE - RZV2H_DDR1_BASE)		
#define RZV2H_DDR2_SIZE             (RZV2H_DDR3_BASE - RZV2H_DDR2_BASE)
#define RZV2H_DDR3_SIZE             (RZV2H_DDR4_BASE - RZV2H_DDR3_BASE)
//#define RZV2H_DDR4_SIZE             (xxxxxx - RZV2H_DDR4_BASE)			//TODO: KTG: Update

#define RZV2H_SPIROM_FIP_BASE       (RZV2H_SPIROM_BASE + 0x0001D200)		//TODO: KTG: Update
#define RZV2H_SPIROM_FIP_SIZE       (0x30000000 - RZV2H_SPIROM_FIP_BASE)	//TODO: KTG: Update

#define RZV2H_EMMC_FIP_BASE         (0x00020000)							//TODO: KTG: Update
#define RZV2H_EMMC_FIP_SIZE         (0x001DFFFF)							//TODO: KTG: Update

#define RZV2H_SYC_INCK_HZ           (24000000)								//TODO: KTG: Update
#define RZV2H_UART_INCK_HZ          (100000000)								//TODO: KTG: Update	
#define RZV2H_UART_BARDRATE         (115200)								//TODO: KTG: Update

/* Boot Info base address */
#define RZV2H_BOOTINFO_BASE         (RZV2H_SRAM_BASE)						//TODO: KTG: Update

/* Definitions used in common code */

/* Base address where parameters to BL31 are stored */
#define PARAMS_BASE                 (RZV2H_SRAM_BASE + 0x0001F000)			//TODO: KTG: Update
#define PARAMS_SIZE                 (0x1000)								//TODO: KTG: Update

#define RZ_SOC_BOOTINFO_BASE		RZV2H_BOOTINFO_BASE

#define RZ_SOC_SYC_BASE				RZV2H_SYC_BASE

#define RZ_SOC_GICD_BASE			RZV2H_GICD_BASE
#define RZ_SOC_GICR_BASE			RZV2H_GICR_BASE

#define RZ_SOC_TZC_ASRAM_BASE		RZV2H_TZC_ASRAM_BASE
#define RZ_SOC_TZC_MSRAM_BASE		RZV2H_TZC_MSRAM_BASE

#define RZ_SOC_SPIROM_FIP_BASE		RZV2H_SPIROM_FIP_BASE
#define RZ_SOC_SPIROM_FIP_SIZE		RZV2H_SPIROM_FIP_SIZE

#define RZ_SOC_EMMC_FIP_BASE		RZV2H_EMMC_FIP_BASE
#define RZ_SOC_EMMC_FIP_SIZE		RZV2H_EMMC_FIP_SIZE

#define RZ_SOC_SYSC_BASE_DEVID 		(RZV2H_SYSC_BASE + 0xA04)
#define RZ_SOC_OTP_BASE_DEVID		(RZV2H_OTP_BASE + 0x1178)
#define RZ_SOC_OTP_BASE_CHIPID		(RZV2H_OTP_BASE + 0x1140)

#endif /* __RZ_SOC_DEF_H__ */
