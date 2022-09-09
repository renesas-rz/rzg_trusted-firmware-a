/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**********************************************************************************************************************
 * File Name    : boot_common.h
 * Version      : 1.0
 * Description  : RZ/G2L Boot Program common header file.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.09.2020 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef BOOTCOM_H
#define BOOTCOM_H

#define SHRINK_ROM      /* Shrink ROM size */

#ifdef  SHRINK_ROM
#define NOUSE_VARIABLE_ARGUMENTS
#endif  /* SHRINK_ROM */

/*------------ RAM load address -------------------------------*/
#define BOOT_INFO_ADR               (0x000010000ul)     /* Boot information area */
#define LOAD_ADDR                   (0x000012000ul)     /* Load address in internal RAM */
#define BOOT_INFO_SIZE              (0x000000300ul)     /* Boot information area size */
#define LOAD_ADDR_LIMIT             (0x00002F000ul)

/*------------ Load size block in internal RAM ----------------*/
#define LOAD_SIZE_BLOCK_SIZE        (0x00000200ul)      /* "Loader size block's" size */

#define RAM_LSBLOCK_ADDR            ((uintptr_t) (LOAD_ADDR - LOAD_SIZE_BLOCK_SIZE))/* Address of load size block */
#define RAM_LSBLOCK_DATA_SIZE_PTR   ((uint32_t *)(RAM_LSBLOCK_ADDR))            /* Pointer to loader size */
#define RAM_LSBLOCK_SIGN_PTR        ((uint16_t *)(RAM_LSBLOCK_ADDR + 0x01FEul)) /* Pointer to signature */

/*------------ Definitions of Return Value---------------------*/
#define OK                          ( 0)                /* Successful operation */
#define ERR                         (-1)                /* Fail                 */

/* Add from eMMC boot*/
/*------------ Loader program size -------------------------------*/
#define D_BOOT_LOAD_SIZE_MAX        (LOAD_ADDR_LIMIT - LOAD_ADDR)   /* 0x00012000 ... 0x0002EFFF */

/*------------ Definitions of Return Value ---------------------*/
#define D_BOOT_OK                           (0l)                /* Successful operation */
#define D_BOOT_ERR                          (-1l)               /* Fail                 */
#define D_BOOT_SECU_ERR                     (-20l)              /* Fail w/verify        */

#define D_BOOT_SECU_ERR_CHKSUM              (-21l)              /* Checksum error w/verify         */
#define D_BOOT_SECU_ERR_SIZE                (-22l)              /* Program size error w/verify     */
#define D_BOOT_SECU_ERR_MDR                 (-23l)              /* MDR register error w/verify     */

/*------------ Definitions of boot type ---------------------*/
#define D_BOOT_MODE_SECEN                   (0x00010000u)       /* Secure enable (standard/secure product)         */
#define D_BOOT_MODE_MDCLKS                  (0x00001000u)       /* MD clock                                        */
#define D_BOOT_MODE_DBGEN                   (0x00000200u)       /* Debug enable                                    */
#define D_BOOT_MODE_COUHLD                  (0x00000100u)       /* CPU hold                                        */
#define D_BOOT_MODE_ESD                     (0x00000000u)       /* eSD boot mode                                   */
#define D_BOOT_MODE_EMMC18                  (0x00000001u)       /* eMMC boot 1.8V mode                             */
#define D_BOOT_MODE_EMMC33                  (0x00000002u)       /* eMMC boot 3.3V mode                             */
#define D_BOOT_MODE_SFLASH18                (0x00000003u)       /* Serial flash 1.8V boot mode                     */
#define D_BOOT_MODE_SFLASH33                (0x00000004u)       /* Serial flash 3.3V boot mode                     */
#define D_BOOT_MODE_SCIF                    (0x00000005u)       /* SCIF download boot mode                         */
#define D_BOOT_MODE_BTMD_MSK                (0x00000007u)       /* Boot mode mask bit pattern                      */

#define D_BOOT_MODE_SECBEN_MSK              (D_BOOT_MODE_SECEN | D_BOOT_MODE_BTMD_MSK)


#define D_BOOT_MODE_TYPE_INFO               (0x00010007ul)      /* bit[16]:Secure enable, bit[2:0]:BTMD[2:0]  */
#define D_BOOT_MODE_INIT_PTRN               (0xFFFFFFFFul)      /* Initial pattern */


/*------------ Definitions of device ID ---------------------*/
#define D_BOOT_DEVIDR_DEV_REV_MSK           (0xF0000000u)       /* Device revision mask bit pattern */
#define D_BOOT_DEVIDR_DEV_ID_MSK            (0x0FFFFFFFu)       /* Device ID mask bit pattern */
#define D_BOOT_DEVIDR_DEV_ID_RZG2L          (0x0841C447u)       /* Device ID specified RZ/G2L */

/*------------ Definitions of CA55 core information ---------------------*/
#define D_BOOT_PRR_CA55_1CPU_MSK            (0x00000001u)       /* CA55 core count mask bit pattern */
#define D_BOOT_PRR_CA55_SINGLE              (0x00000001u)       /* CA55 single core */
#define D_BOOT_PRR_CA55_DUAL                (0x00000000u)       /* CA55 dual   core */
/*------------ Definitions of CM33 core information ---------------------*/
#define D_BOOT_PRR_CM33_DIS_MSK             (0x00000010u)       /* CM33 core implement mask bit pattern */
#define D_BOOT_PRR_CA33_DIS                 (0x00000010u)       /* CM33 core not implement */
#define D_BOOT_PRR_CA33_ENA                 (0x00000000u)       /* CM33 core implement */
/*------------ Definitions of GPU information ---------------------*/
#define D_BOOT_PRR_GPU_DIS_MSK              (0x00000010u)       /* GPU implement mask bit pattern */
#define D_BOOT_PRR_GPU_DIS                  (0x00000010u)       /* Mali-G31 not implement */
#define D_BOOT_PRR_GPU_ENA                  (0x00000000u)       /* Mali-G31 implement */

/*------------ Definitions of secure boot mode en/dis ---------------------*/
#define D_BOOT_OTPSECENR_SECEN_MSK          (0x00000001u)       /* Secure boot mask bit pattern      */
#define D_BOOT_OTPSECENR_SECEN_ENA          (0x00000001u)       /* Secure boot mode (secure enable)  */
#define D_BOOT_OTPSECENR_SECEN_DIS          (0x00000000u)       /* Normal boot mode (secure disable) */

/*------------ Definitions of secure boot device en/dis ---------------------*/
/*------------ eSD information ---------------------*/
#define D_BOOT_OTPSECMDR_SD_MSK             (0x00000001u)       /* eSD secure boot mask bit pattern */
#define D_BOOT_OTPSECMDR_SD_ENA             (0x00000001u)       /* eSD secure boot enable  */
#define D_BOOT_OTPSECMDR_SD_DIS             (0x00000000u)       /* eSD secure boot disable */
/*------------ eSD information ---------------------*/
#define D_BOOT_OTPSECMDR_EMMC18_MSK         (0x00000002u)       /* eMMC 1.8V secure boot mask bit pattern */
#define D_BOOT_OTPSECMDR_EMMC18_ENA         (0x00000002u)       /* eMMC 1.8V secure boot enable  */
#define D_BOOT_OTPSECMDR_EMMC18_DIS         (0x00000000u)       /* eMMC 1.8V secure boot disable */
#define D_BOOT_OTPSECMDR_EMMC33_MSK         (0x00000004u)       /* eMMC 3.3V secure boot mask bit pattern */
#define D_BOOT_OTPSECMDR_EMMC33_ENA         (0x00000004u)       /* eMMC 3.3V secure boot enable  */
#define D_BOOT_OTPSECMDR_EMMC33_DIS         (0x00000000u)       /* eMMC 3.3V secure boot disable */
/*------------ SPI multi information ---------------------*/
#define D_BOOT_OTPSECMDR_SPI18_MSK          (0x00000008u)       /* sFlash 1.8V secure boot mask bit pattern */
#define D_BOOT_OTPSECMDR_SPI18_ENA          (0x00000008u)       /* sFlash 1.8V secure boot enable  */
#define D_BOOT_OTPSECMDR_SPI18_DIS          (0x00000000u)       /* sFlash 1.8V secure boot disable */
#define D_BOOT_OTPSECMDR_SPI33_MSK          (0x00000010u)       /* sFlash 3.3V secure boot mask bit pattern */
#define D_BOOT_OTPSECMDR_SPI33_ENA          (0x00000010u)       /* sFlash 3.3V secure boot enable  */
#define D_BOOT_OTPSECMDR_SPI33_DIS          (0x00000000u)       /* sFlash 3.3V secure boot disable */


/*------------ Definitions of JTAG certification mode ---------------------*/
#define D_BOOT_OTPJAMR_JAM_MSK              (0x00000003u)       /* JTAG certification mode mask bit pattern */

/*------------ Definitions of boot frequency ---------------------*/
#define D_BOOT_OTPJCORER_FREQ_MSK           (0x00000003u)       /* Boot frequency mask bit pattern on CA55 core0 */

/*------------ Definitions of driving power ---------------------*/
#define D_BOOT_OTPPOC_DRV_ESD_MSK           (0x00000003u)       /* Driving power mask bit pattern for eSD */
#define D_BOOT_OTPPOC_DRV_EMMC18_MSK        (0x0000000Cu)       /* Driving power mask bit pattern for eMMC 1.8V */
#define D_BOOT_OTPPOC_DRV_EMMC33_MSK        (0x00000030u)       /* Driving power mask bit pattern for eMMC 3.3V */
#define D_BOOT_OTPPOC_DRV_SPI18_MSK         (0x000000C0u)       /* Driving power mask bit pattern for SPI Multi 1.8V */
#define D_BOOT_OTPPOC_DRV_SPI33_MSK         (0x00000300u)       /* Driving power mask bit pattern for SPI Multi 3.3V */
#define D_BOOT_OTPPOC_DRV_SCIF_MSK          (0x00000C00u)       /* Driving power mask bit pattern for SCIF */

#define D_BOOT_OTPPOC_DRV_MSK               (0x00000003u)       /* Driving power mask bit pattern */


#define VOLTAGE18                           (0U)
#define VOLTAGE33                           (1U)

/*------------ RAM load address -------------------------------*/
#define D_BOOT_LOAD_ADDR        ((volatile uint32_t *)(LOAD_ADDR))          /* Loader Program (eSD/eMMC) Top address  */
#define D_BOOT_BOOT_RESULT      (BOOT_INFO_ADR + 0x0002ul)
#define D_BOOT_LOG_INFO_BASE    (BOOT_INFO_ADR + 0x0004ul)                  /* Boot Log start address */

#define D_BOOT_SB_FUNC_RESULT   (BOOT_INFO_ADR + 0x0018ul)
#define D_BOOT_MB_FUNC_RESULT   (BOOT_INFO_ADR + 0x001Cul)
#define D_BOOT_MEASURED_BOOT    (BOOT_INFO_ADR + 0x0020ul)

/*------------ Secure boot load definition -------------------------------*/
#define D_BOOT_KEY_CERT_ADDR        ((volatile uint32_t *)((uint32_t)D_BOOT_LOAD_ADDR))
#define D_BOOT_KEY_CERT_LEN_MAX     (1024)
#define D_BOOT_CODE_CERT_ADDR       ((volatile uint32_t *)((uint32_t)D_BOOT_LOAD_ADDR + 1024))
#define D_BOOT_CODE_CERT_LEN_MAX    (1024)

/*------------ Definitions of boot mode information ---------------------*/
#define D_BOOT_MODE_INFO_NRM_ESD            (0x0000u)           /* eSD                boot mode, normal mode    */
#define D_BOOT_MODE_INFO_NRM_EMMC18         (0x0001u)           /* eMMC(1.8V)         boot mode, normal mode    */
#define D_BOOT_MODE_INFO_NRM_EMMC33         (0x0002u)           /* eMMC(3.3V)         boot mode, normal mode    */
#define D_BOOT_MODE_INFO_NRM_SFLSH18        (0x0003u)           /* Serial Flash(1.8V) boot mode, normal mode    */
#define D_BOOT_MODE_INFO_NRM_SFLSH33        (0x0004u)           /* Serial Flash(1.8V) boot mode, normal mode    */
#define D_BOOT_MODE_INFO_NRM_SCIF           (0x0005u)           /* SCIF download      boot mode, normal mode    */
#define D_BOOT_MODE_INFO_SECURE             (0x0010u)           /*             Secure boot mode, normal mode    */

#define D_BOOT_MODE_INFO_SEC_ESD            (D_BOOT_MODE_INFO_SECURE | D_BOOT_MODE_INFO_NRM_ESD)
#define D_BOOT_MODE_INFO_SEC_EMMC18         (D_BOOT_MODE_INFO_SECURE | D_BOOT_MODE_INFO_NRM_EMMC18)
#define D_BOOT_MODE_INFO_SEC_EMMC33         (D_BOOT_MODE_INFO_SECURE | D_BOOT_MODE_INFO_NRM_EMMC33)
#define D_BOOT_MODE_INFO_SEC_SFLSH18        (D_BOOT_MODE_INFO_SECURE | D_BOOT_MODE_INFO_NRM_SFLSH18)
#define D_BOOT_MODE_INFO_SEC_SFLSH33        (D_BOOT_MODE_INFO_SECURE | D_BOOT_MODE_INFO_NRM_SFLSH33)
#define D_BOOT_MODE_INFO_SEC_SCIF           (D_BOOT_MODE_INFO_SECURE | D_BOOT_MODE_INFO_NRM_SCIF)

#define D_BOOT_TRANS_RESULT_INIT            (0xFFFFu)           /* Initialized value for result to transmit image   */

#define D_BOOT_SIGNATURE                    (0xAA55u)           /* Signature on program size block */

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
extern int32_t rom_main(void);

#endif /* BOOTCOM_H */
