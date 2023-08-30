/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**********************************************************************************************************************
 * File Name    : esdif.h
 * Version      : 1.0
 * Description  : SD Driver interface header file.
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
#ifndef SDDRV_H
#define SDDRV_H

/* ==== Define  ==== */
/* ---- SD Driver work buffer ---- */
#define SD_SIZE_OF_INIT         (640)

/* ---- Error code ---- */
#define SD_OK                   (0)             /* OK */
#define SD_ERR                  (-1)            /* General error */
#define SD_ERR_WP               (-2)            /* Write protect error */

#define SD_ERR_RES_TOE          (-4)            /* Response time out error */
#define SD_ERR_CARD_TOE         (-5)            /* Card time out error */
#define SD_ERR_END_BIT          (-6)            /* End bit error */
#define SD_ERR_CRC              (-7)            /* CRC error */

#define SD_ERR_HOST_TOE         (-9)            /* Host time out error */
#define SD_ERR_CARD_ERASE       (-10)           /* Card erase error */
#define SD_ERR_CARD_LOCK        (-11)           /* Card lock error */
#define SD_ERR_CARD_UNLOCK      (-12)           /* Card unlock error */
#define SD_ERR_HOST_CRC         (-13)           /* Host CRC error */
#define SD_ERR_CARD_ECC         (-14)           /* Card internal ECC error */
#define SD_ERR_CARD_CC          (-15)           /* Card internal error */
#define SD_ERR_CARD_ERROR       (-16)           /* Unknown card error */
#define SD_ERR_CARD_TYPE        (-17)           /* Non support card type */
#define SD_ERR_NO_CARD          (-18)           /* No card */
#define SD_ERR_ILL_READ         (-19)           /* Illegal buffer read */
#define SD_ERR_ILL_WRITE        (-20)           /* Illegal buffer write */
#define SD_ERR_AKE_SEQ          (-21)           /* The sequence of authentication process */
#define SD_ERR_OVERWRITE        (-22)           /* CID/CSD overwrite error */
#define SD_ERR_CPU_IF           (-30)           /* Target CPU interface function error  */
#define SD_ERR_STOP             (-31)           /* User stop */

#define SD_ERR_FILE_FORMAT      (-52)           /* CSD register file format error  */

#define SD_ERR_ILL_FUNC         (-60)           /* Invalid function request error */

#define SD_ERR_IFCOND_VER       (-70)           /* Interface condition version error */

#define SD_ERR_IFCOND_ECHO      (-72)           /* Interface condition echo back pattern error */
#define SD_ERR_INTERNAL         (-99)           /* Driver software internal error */
#define SD_ERR_OUT_OF_RANGE     (-80)           /* The argument was out of range */
#define SD_ERR_ADDRESS_ERROR    (-81)           /* Misassigned address */
#define SD_ERR_BLOCK_LEN_ERROR  (-82)           /* Transfered block length is not allowed */
#define SD_ERR_ILLEGAL_COMMAND  (-83)           /* Command not legal  */
#define SD_ERR_RESERVED_ERROR18 (-84)           /* Reserved bit 18 Error */
#define SD_ERR_RESERVED_ERROR17 (-85)           /* Reserved bit 17 Error */
#define SD_ERR_CMD_ERROR        (-86)           /* SD_INFO2 bit  0 CMD error */
#define SD_ERR_CBSY_ERROR       (-87)           /* SD_INFO2 bit 14 CMD Type Reg Busy error */
#define SD_ERR_NO_RESP_ERROR    (-88)           /* SD_INFO1 bit  0 No Response error */

/* ---- Driver mode ---- */
#define SD_MODE_POLL            (0x0000ul)      /* Status check mode is software polling */

#define SD_MODE_SW              (0x0000ul)      /* Data transfer mode is software */
#define SD_MODE_DMA             (0x0002ul)      /* Data transfer mode is DMA */

/* ---- Support mode ---- */

#define SD_MODE_IO              (0x0010ul)      /* Memory and io cards are supported */

#define SD_MODE_DS              (0x0000ul)      /* Only default speed mode is supported */
#define SD_MODE_HS              (0x0040ul)      /* High speed mode is also supported */

#define SD_MODE_VER2X           (0x0080ul)      /* Ver2.x host (high capacity and dual voltage) */

/* ---- Media voltage ---- */

#define SD_VOLT_3_3             (0x00100000ul)

/* ---- Media type ---- */
#define SD_MEDIA_UNKNOWN        (0x0000u)       /* Unknown media */
#define SD_MEDIA_MMC            (0x0010u)       /* MMC card */
#define SD_MEDIA_SD             (0x0020u)       /* SD Memory card */
#define SD_MEDIA_IO             (0x0001u)       /* SD IO card */
#define SD_MEDIA_MEM            (0x0030u)       /* Memory card */
#define SD_MEDIA_COMBO          (0x0021u)       /* SD COMBO card */

/* ---- write protect info --- */

#define SD_WP_ROM               (0x0010u)       /* Card is SD-ROM */

/* ---- SD clock div ---- */                    /* IMCLK is host controller clock */
#define SD_DIV_512              (0x0080u)       /* SDCLOCK = IMCLK/512 */
#define SD_DIV_256              (0x0040u)       /* SDCLOCK = IMCLK/256 */
#define SD_DIV_32               (0x0008u)       /* SDCLOCK = IMCLK/32 */
#define SD_DIV_16               (0x0004u)       /* SDCLOCK = IMCLK/16 */
#define SD_DIV_8                (0x0002u)       /* SDCLOCK = IMCLK/8 */

/* ---- SD clock define ---- */                 /* Max frequency */
#define SD_CLK_400kHz           (0x0000u)       /* 400kHz */
#define SD_CLK_1MHz             (0x0001u)       /* 1MHz */
#define SD_CLK_5MHz             (0x0002u)       /* 5MHz */
#define SD_CLK_10MHz            (0x0003u)       /* 10MHz */
#define SD_CLK_20MHz            (0x0004u)       /* 20MHz */
#define SD_CLK_25MHz            (0x0005u)       /* 25MHz */
#define SD_CLK_50MHz            (0x0006u)       /* 50MHz (phys spec ver1.10) */

/* @060526 for SD phy ver2.0 */

/* ---- IO initialize flags define ---- */      /* Add for IO */

#define SD_IO_POWER_INIT        (0x04u)         /* Power on initialized */

/* ---- SD port mode ---- */
#define SD_PORT_SERIAL          (0x0000u)       /* 1bit mode */
#define SD_PORT_PARALLEL        (0x0001u)       /* 4bits mode */

/* ---- SD Card detect port ---- */
#define SD_CD_SOCKET            (0x0000u)       /* CD pin */
#define SD_CD_DAT3              (0x0001u)       /* DAT3 pin */

/* ---- SD Card detect interrupt ---- */
#define SD_CD_INT_DISABLE       (0x0000u)       /* Card detect interrupt disable */
#define SD_CD_INT_ENABLE        (0x0001u)       /* Card detect interrupt enable */

/* ---- Format mode ---- */
#define SD_FORMAT_QUICK         (0x0000u)       /* Quick format */
#define SD_FORMAT_FULL          (0x0001u)       /* Full format */

/* ---- SD Driver work buffer ---- */
#define ESD_SIZE_OF_INIT        (SD_SIZE_OF_INIT+4)
/* ---- Media type ---- */
#define SD_MEDIA_EMBEDDED       (0x8000u)       /* Embedded media */

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
/* ==== API prototype ===== */
/* ---- Access library I/F ---- */
extern int32_t esd_init(uintptr_t base, void *workarea, int32_t cd_port);
extern int32_t esd_check_media(void);
extern int32_t esd_mount(uint32_t mode, uint32_t voltage);
extern int32_t esd_read_sect(uint8_t *buff, uint32_t psn, int32_t cnt);
extern int32_t esd_get_type(uint16_t *type, uint8_t *speed, uint8_t *capa);
extern int32_t esd_check_int(void);
extern int32_t esd_set_buffer(void *buff, uint32_t size);
extern int32_t esd_get_partition_id(int32_t *id);
extern int32_t esd_main(void);

/* ---- Target CPU I/F ---- */
extern int32_t esddev_init(void);
extern int32_t esddev_power_on(void);
extern int32_t esddev_power_off(void);
extern int32_t esddev_read_data( uint8_t *buff, uintptr_t reg_addr, uint32_t num);
extern int32_t esddev_write_data(uint8_t *buff, uintptr_t reg_addr, uint32_t num);
extern uint32_t esddev_get_clockdiv ( int32_t clock );
extern int32_t esddev_set_port(int32_t mode);
extern int32_t esddev_int_wait(int32_t msec);
extern int32_t esddev_init_dma(uintptr_t buffadr, uintptr_t regadr, uint32_t cnt, int32_t dir);
extern int32_t esddev_wait_dma_end(uint32_t cnt);
extern int32_t esddev_disable_dma(void);
extern int32_t esddev_loc_cpu(void);
extern int32_t esddev_unl_cpu(void);

#endif    /* SDDRV_H */
/* End of File */
