/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RZ_SOC_DEF_H__
#define __RZ_SOC_DEF_H__

#include <common/tbbr/tbbr_img_def.h>

#define RZG3S_BOOT_ROM_BASE				UL(0x00000000)
#define RZG3S_AUTH_API_BASE				UL(0x00014300)
#define RZG3S_MSRAM_0_BASE				UL(0x00020000)
#define RZG3S_MSRAM_1_BASE				UL(0x00060000)
#define RZG3S_ASRAM_0_BASE				UL(0x000A0000)
#define RZG3S_ASRAM_1_BASE				UL(0x000E0000)
#define RZG3S_DEVICE_BASE				UL(0x10000000)
#define RZG3S_SCIF_0_BASE				UL(0x1004B800)
#define RZG3S_SCIF_1_BASE				UL(0x1004BC00)
#define RZG3S_SCIF_2_BASE				UL(0x1004C000)
#define RZG3S_SCIF_3_BASE				UL(0x1004C400)
#define RZG3S_SCIF_4_BASE				UL(0x1004C800)
#define RZG3S_VBATT_BASE				UL(0x1005C000)
#define RZG3S_XSPI_BASE					UL(0x10060000)
#define RZG3S_OCTA_BASE					UL(0x10080000)
#define RZG3S_I2C_0_BASE				UL(0x10090000)
#define RZG3S_I2C_1_BASE				UL(0x10090400)
#define RZG3S_MHU_BASE					UL(0x10400000)
#define RZG3S_SYC_BASE					UL(0x11000000)
#define RZG3S_CPG_BASE					UL(0x11010000)
#define RZG3S_SYSC_BASE					UL(0x11020000)
#define RZG3S_GPIO_BASE					UL(0x11030000)
#define RZG3S_TZC400_ASRAM_0_BASE		UL(0x11200000)
#define RZG3S_TZC400_ASRAM_1_BASE		UL(0x11210000)
#define RZG3S_TZC400_MSRAM_0_BASE		UL(0x11230000)
#define RZG3S_TZC400_MSRAM_1_BASE		UL(0x11240000)
#define RZG3S_TZC400_xSPI_BASE			UL(0x11250000)
#define RZG3S_TZC400_DDR_BASE			UL(0x11260000)
#define RZG3S_DDR0_PHY_BASE				UL(0x11400000)
#define RZG3S_DDR0_MEMC_BASE			UL(0x11600000)
#define RZG3S_OTP_BASE					UL(0x11860000)
#define RZG3S_SD0_BASE					UL(0x11C00000)
#define RZG3S_SD1_BASE					UL(0x11C10000)
#define RZG3S_SD2_BASE					UL(0x11C20000)
#define RZG3S_GIC_600_BASE				UL(0x12400000)
#define RZG3S_GPV_REG0_BASE				UL(0x12A00000)
#define RZG3S_SPIROM_BASE				UL(0x20000000)
#define RZG3S_PCIe_BASE					ULL(0x30000000)
#define RZG3S_DDR0_BASE					ULL(0x40000000)
#define RZG3S_DDR1_BASE					ULL(0xC0000000)

#define RZG3S_BOOT_ROM_SIZE				(RZG3S_MSRAM_0_BASE - RZG3S_BOOT_ROM_BASE)
#define RZG3S_SPIROM_SIZE				(RZG3S_PCIe_BASE - RZG3S_SPIROM_BASE)

#define RZG3S_DEVICE_SIZE				(0x20000000 - RZG3S_DEVICE_BASE)
#define RZG3S_GICD_BASE					(RZG3S_GIC_600_BASE)
#define RZG3S_GICR_BASE					(RZG3S_GIC_600_BASE + 0x00040000)

#define RZG3S_MSRAM_SIZE				UL(0x00080000)
#define RZG3S_ASRAM_SIZE				UL(0x00080000)
#define RZG3S_ASRAM_LIMIT				(RZG3S_ASRAM_0_BASE + RZG3S_ASRAM_SIZE)
#define RZG3S_SRAM_BASE					(RZG3S_MSRAM_0_BASE)
#define RZG3S_SRAM_SIZE					(RZG3S_MSRAM_SIZE + RZG3S_ASRAM_SIZE)
#define RZG3S_DDR0_SIZE					ULL(0x40000000)

#define RZG3S_NS_DRAM_BASE				ULL(0x48000000)

/*
 * 0x64000 has been chosen as FIP base as that leaves room at start of xSPI Flash
 * to store a 128K BL2 image.
 */
#define RZG3S_BL2_SIZE_MAX				UL(0x00064000)
#define RZG3S_FIP_SIZE_MAX				UL(0x00140000)

#define RZG3S_SPIROM_FIP_BASE			(RZG3S_SPIROM_BASE + RZG3S_BL2_SIZE_MAX)
#define RZG3S_EMMC_FIP_BASE				(RZG3S_BL2_SIZE_MAX)
#define RZG3S_SD_FIP_BASE				(RZG3S_BL2_SIZE_MAX)

#define RZG3S_DDR_CONFIG_MAX			UL(0x00001000)
#define RZG3S_SPIROM_DDR_CFG_BASE		(RZG3S_SPIROM_FIP_BASE + RZG3S_FIP_SIZE_MAX)
#define RZG3S_EMMC_DDR_CFG_BASE			(RZG3S_EMMC_FIP_BASE + RZG3S_FIP_SIZE_MAX)
#define RZG3S_SD_DDR_CFG_BASE			(RZG3S_SD_FIP_BASE + RZG3S_FIP_SIZE_MAX)

#define RZG3S_M33_FW_OFFSET				UL(0x00200000)
#define RZG3S_SPIROM_M33_FW_BASE		(RZG3S_SPIROM_BASE + RZG3S_M33_FW_OFFSET)
#define RZG3S_EMMC_M33_FW_BASE			(RZG3S_M33_FW_OFFSET)
#define RZG3S_SD_M33_FW_BASE			(RZG3S_M33_FW_OFFSET)
#define RZG3S_M33_FW_SIZE				UL(0x00030000)

#define RZG3S_SYC_INCK_HZ				U(24000000)
#define RZG3S_UART_INCK_HZ				U(100000000)
#define RZG3S_UART_BARDRATE				U(115200)

/* Definitions used in common code */

/* Base address where parameters to BL31 are stored */
#define PARAMS_SIZE						UL(0x00001000)
#define PARAMS_BASE						(RZG3S_ASRAM_LIMIT - PARAMS_SIZE)

#define RZ_TBB_CERT_SIZE				UL(0x00000400)

#define RZ_SOC_SYC_BASE					RZG3S_SYC_BASE

#define RZ_SOC_GICD_BASE				RZG3S_GICD_BASE
#define RZ_SOC_GICR_BASE				RZG3S_GICR_BASE

#define RZ_SOC_SPIROM_FIP_BASE			RZG3S_SPIROM_FIP_BASE
#define RZ_SOC_SPIROM_FIP_SIZE			RZG3S_FIP_SIZE_MAX

#define RZ_SOC_EMMC_FIP_BASE			RZG3S_EMMC_FIP_BASE
#define RZ_SOC_EMMC_FIP_SIZE			RZG3S_FIP_SIZE_MAX

#define RZ_SOC_SD_FIP_BASE				RZG3S_SD_FIP_BASE
#define RZ_SOC_SD_FIP_SIZE				RZG3S_FIP_SIZE_MAX

#define RZ_SOC_SPIROM_DDR_CFG_BASE		RZG3S_SPIROM_DDR_CFG_BASE
#define RZ_SOC_SPIROM_DDR_CFG_SIZE		RZG3S_DDR_CONFIG_MAX
#define RZ_SOC_EMMC_DDR_CFG_BASE		RZG3S_EMMC_DDR_CFG_BASE
#define RZ_SOC_EMMC_DDR_CFG_SIZE		RZG3S_DDR_CONFIG_MAX
#define RZ_SOC_SD_DDR_CFG_BASE			RZG3S_SD_DDR_CFG_BASE
#define RZ_SOC_SD_DDR_CFG_SIZE			RZG3S_DDR_CONFIG_MAX

#define RZ_SOC_SYSC_BASE_DEVID			SYS_LSI_DEVID
#define RZ_SOC_OTP_BASE_CHIPID			(RZG3S_OTP_BASE + 0x1140)
#define RZG3S_SYSC_USB_PWRRDY_OFFSET			UL(0x0D70)
#define RZG3S_SYSC_PCIE_RST_RSM_B_OFFSET		UL(0x0D74)

#define RZ_SOC_I2C_BASE					RZG3S_I2C_1_BASE

#if TRUSTED_BOARD_BOOT
#define RZ_SOC_AUTH_API_BASE			(RZG3S_AUTH_API_BASE + 0x0000)

#define RZ_SOC_CIP_INIT_BASE			(RZG3S_AUTH_API_BASE + 0x1208)
#define RZ_SOC_CIP_HUKL_BASE			(RZG3S_AUTH_API_BASE + 0x14C4)
#define RZ_SOC_CIP_RAND_BASE			(RZG3S_AUTH_API_BASE + 0x1DC4)
#endif

#define	RZ_SOC_DDR_PHY_BASE				RZG3S_DDR0_PHY_BASE
#define	RZ_SOC_DDR_MC_BASE				RZG3S_DDR0_MEMC_BASE

#define SYS_BASE						RZG3S_SYSC_BASE
#define PFC_BASE						RZG3S_GPIO_BASE
#define CPG_BASE						RZG3S_CPG_BASE
#define XSPI_BASE						RZG3S_XSPI_BASE
#define VBATT_BASE						RZG3S_VBATT_BASE

/* eMMC registers */
#define MMC0_SD_BASE					RZG3S_SD0_BASE
#define MMC1_SD_BASE					RZG3S_SD1_BASE
#define MMC2_SD_BASE					RZG3S_SD2_BASE

#endif /* __RZ_SOC_DEF_H__ */
