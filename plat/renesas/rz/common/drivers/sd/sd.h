/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**********************************************************************************************************************
 * File Name    : sd.h
 * Version      : 1.0
 * Description  : SD Driver header file.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.09.2020 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "sys_sel.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef SD_H
#define SD_H

/* ==== Option ==== */
#ifndef SD_IO_FORCE_BYTE    /* When SDIO is published, describe the following macro in sdif.h */
#define SD_IO_FORCE_BYTE    ( 0x0010u )  /* Byte access only  */
#endif

#define _SDHI0_BASE_    ( 0x11C00000U )


/* ==== SDHI register address ==== */
#define SD_CMD              ( 0x0000 )  /* SD Command */
#define SD_PORTSEL          ( 0x0008 )  /* SD Port Selection */
#define SD_ARG0             ( 0x0010 )  /* SD Command Argument (low  16bits) */
#define SD_ARG1             ( 0x0018 )  /* SD Command Argument (high 16bits) */
#define SD_STOP             ( 0x0020 )  /* Data Stop */
#define SD_SECCNT           ( 0x0028 )  /* Block Count */
#define SD_RESP0            ( 0x0030 )  /* Response R23-8 */
#define SD_RESP1            ( 0x0038 )  /* Response R39-24 */
#define SD_RESP2            ( 0x0040 )  /* Response R55-40 */
#define SD_RESP3            ( 0x0048 )  /* Response R71-56 */
#define SD_RESP4            ( 0x0050 )  /* Response R87-72 */
#define SD_RESP5            ( 0x0058 )  /* Response R103-88 */
#define SD_RESP6            ( 0x0060 )  /* Response R119-104 */
#define SD_RESP7            ( 0x0068 )  /* Response R127-120 */
#define SD_INFO1            ( 0x0070 )  /* SD Interrupt Flag(1) */
#define SD_INFO2            ( 0x0078 )  /* SD Interrupt Flag(2) */
#define SD_INFO1_MASK       ( 0x0080 )  /* SD Interrupt Flag(1) Mask */
#define SD_INFO2_MASK       ( 0x0088 )  /* SD Interrupt Flag(2) Mask */
#define SD_CLK_CTRL         ( 0x0090 )  /* SD Clock Control */
#define SD_SIZE             ( 0x0098 )  /* Block Size */
#define SD_OPTION           ( 0x00A0 )  /* Access Option */
#define SD_ERR_STS1         ( 0x00B0 )  /* CMD,CRC,END Error Status */
#define SD_ERR_STS2         ( 0x00B8 )  /* Timeout Error Status  */
#define SD_BUF0             ( 0x00C0 )  /* SD Buffer */
#define SDIO_MODE           ( 0x00D0 )  /* SDIO Mode */
#define SDIO_INFO1          ( 0x00D8 )  /* SDIO Interrupt Flag */
#define SDIO_INFO1_MASK     ( 0x00E0 )  /* SDIO Interrupt Flag Mask */
#define SOFT_RST            ( 0x0380 )  /* Soft Reset */
#define VERSION             ( 0x0388 )  /* Version */
#define HOST_MODE           ( 0x0390 )  /* SWAP Control */
/* Unused */
/*#define POWER_CTRL      ((0xf2u<<SD_REG_SHIFT)+SD_BYTE_OFFSET)*/  /* Power Control */
#define EXT_SDIO            ( 0x03D0 )  /* Extended SDIO Interrupt Flag */
#define EXT_WP              ( 0x03D8 )  /* Extended Write Protect */
#define EXT_CD              ( 0x03E0 )  /* Extended Card Detect */
#define EXT_CD_DAT3         ( 0x03E8 )  /* Extended Card Detect(DAT3) */
#define EXT_CD_MASK         ( 0x03F0 )  /* Extended Card Detect Mask */
#define EXT_CD_DAT3_MASK    ( 0x03F8 )  /* Extended Card Detect(DAT3) Mask */
#define CC_EXT_MODE         ( 0x0360 )  /* DMA Mode Enable */

/* ==== Command type ==== */
/* ---- SD commands ---- */
#define CMD0                (0u)        /* GO_IDLE_STATE */
#define CMD1                (1u)        /* SD_SEND_OP_COND for MMC */
#define CMD2                (2u)        /* ALL_SEND_CID */
#define CMD3                (3u)        /* SEND_RELATIVE_ADDR */
#define CMD4                (4u)        /* SET_DSR */
#define CMD7                (7u)        /* SELECT/DESELECT_CARD */
#define CMD9                (9u)        /* SEND_CSD */
#define CMD10               (10u)       /* SEND_CID */
#define CMD12               (12u)       /* STOP_TRANSMISSION */
#define CMD13               (13u)       /* SEND_STATUS */
#define CMD15               (15u)       /* GO_INACTIVE_STATE */
#define CMD16               (16u)       /* SET_BLOCK_LEN */
#define CMD17               (17u)       /* READ_SINGLE_BLOCK */
#define CMD18               (18u)       /* READ_MULTIPLE_BLOCK */
#define CMD24               (24u)       /* WRITE_SINGLE_BLOCK */
#define CMD25               (25u)       /* WRITE_MULTIPLE_BLOCK */
#define CMD27               (27u)       /* PROGRAM_CSD */
#define CMD28               (28u)       /* SET_WRITE_PROT */
#define CMD29               (29u)       /* CLR_WRITE_PROT */
#define CMD30               (30u)       /* SEND_WRITE_PROT */
#define CMD32               (32u)       /* ERASE_WR_BLK_START */
#define CMD33               (33u)       /* ERASE_WR_BLK_END */
#define CMD35               (35u)       /* ERASE_GROUP_START */
#define CMD36               (36u)       /* ERASE_GROUP_END */
#define CMD38               (38u)       /* ERASE */
#define CMD42               (42u)       /* LOCK_UNLOCK */
#define CMD55               (55u)       /* APP_CMD */

/* ---- IO commnds ---- */    /* add for IO */
#define CMD5                (0x4705u)   /* IO_SEND_OP_COND */
#define CMD52_W             (0x4434u)   /* IO_WRITE_DIRECT */
#define CMD52_R             (0x5434u)   /* IO_READ_DIRECT */
#define CMD53_W_BLOCK       (0x6c35u)   /* IO_WRITE_EXTENDED_BLOCK */
#define CMD53_W_BYTE        (0x4c35u)   /* IO_WRITE_EXTENDED_BYTE */
#define CMD53_R_BLOCK       (0x7c35u)   /* IO_READ_EXTENDED_BLOCK */
#define CMD53_R_BYTE        (0x5c35u)   /* IO_READ_EXTENDED_BYTE */

/* ---- Switch function command (phys spec ver1.10) ---- */
#define CMD6                (0x1C06u)   /* SWITCH_FUNC */

/* ---- Dual voltage inquiry command (phys spec ver2.0) ---- */
#define CMD8                (0x0408u)   /* SEND_IF_COND */

/* ---- Application specific commands ---- */
#define ACMD6               (0x40u|6u)  /* SET_BUS_WIDTH */
#define ACMD13              (0x40u|13u) /* SD_STATUS */
#define ACMD22              (0x40u|22u) /* SEND_NUM_WR_BLOCKS */
#define ACMD23              (0x40u|23u) /* SET_WR_BLK_ERASE_COUNT */
#define ACMD41              (0x40u|41u) /* SD_SEND_OP_COND */
#define ACMD42              (0x40u|42u) /* SET_CLR_CARD_DETECT */
#define ACMD51              (0x40u|51u) /* SEND_SCR */

/* ---- Security commands (security spec ver1.01) ---- */
#define ACMD18              (0x40u|18u) /* SECURE_READ_MULTIPLE_BLOCK */
#define ACMD25              (0x40u|25u) /* SECURE_WRITE_MULTIPLE_BLOCK */
#define ACMD26              (0x40u|26u) /* SECURE_WRITE_MKB */
#define ACMD38              (0x40u|38u) /* SECURE_ERASE */
#define ACMD43              (0x40u|43u) /* GET_MKB */
#define ACMD44              (0x40u|44u) /* GET_MID */
#define ACMD45              (0x40u|45u) /* SET_CER_RN1 */
#define ACMD46              (0x40u|46u) /* GET_CER_RN2 */
#define ACMD47              (0x40u|47u) /* SET_CER_RES2 */
#define ACMD48              (0x40u|48u) /* GET_CER_RES1 */
#define ACMD49              (0x40u|49u) /* CHANGE_SECURE_AREA */

/* ==== Constants ==== */
/* --- Command arg --- */
#define ARG_ACMD6_1bit      (0)
#define ARG_ACMD6_4bit      (2)

/* ---- Response type  ---- */
#define SD_RESP_NON         (0)         /* No response */
#define SD_RESP_R1          (1)         /* Nomal response */
#define SD_RESP_R1b         (2)         /* Nomal response with an optional busy signal */
#define SD_RESP_R1_SCR      (3)         /* Nomal response with an optional busy signal */
#define SD_RESP_R2_CID      (4)         /* CID register */
#define SD_RESP_R2_CSD      (5)         /* CSD register */
#define SD_RESP_R3          (6)         /* OCR register */
#define SD_RESP_R6          (7)         /* Published RCA response */
#define SD_RESP_R4          (8)         /* IO OCR register */
#define SD_RESP_R5          (9)         /* IO RW response */
#define SD_RESP_R7          (10)        /* Card Interface Condition response */

/* --- R1 response error bit ---- */
#define RES_SW_INTERNAL             (0xe8400000ul)  /* Driver illegal process */
                                                    /* OUT_OF_RANGE */
                                                    /* ADDRESS_ERROR */
                                                    /* BLOCK_LEN_ERROR */
                                                    /* ERASE_PARAM */
                                                    /* RES_ILLEGAL_COMMAND */
#define RES_ERASE_SEQ_ERROR         (0x10008000ul)  /* ERASE_SEQ_ERROR + WP_ERASE_SKIP */
#define RES_WP_VIOLATION            (0x04000000ul)
#define RES_CARD_IS_LOCKED          (0x02000000ul)
#define RES_CARD_UNLOCKED_FAILED    (0x01000000ul)
#define RES_COM_CRC_ERROR           (0x00800000ul)
#define RES_CARD_ECC_FAILED         (0x00200000ul)
#define RES_CC_ERROR                (0x00100000ul)
#define RES_ERROR                   (0x00080000ul)
#define RES_AKE_SEQ_ERROR           (0x00000008ul)
#define RES_STATE                   (0x00001e00ul)

/* --- Current_state --- */
#define STATE_IDEL                  (0)
#define STATE_READY                 (1u<<9u)
#define STATE_IDENT                 (2u<<9u)
#define STATE_STBY                  (3u<<9u)
#define STATE_TRAN                  (4u<<9u)
#define STATE_DATA                  (5u<<9u)
#define STATE_RCV                   (6u<<9u)
#define STATE_PRG                   (7u<<9u)
#define STATE_DIS                   (8u<<9u)

/* ---- Maximum block count per multiple command ---- */
#define TRANS_SECTORS   (p_hndl->trans_sectors)   /* Max 65535 blocks */

/* ---- Set block address, if HC card ---- */   /* @050805 for ver2.0 */
#define SET_ACC_ADDR    ((p_hndl->csd_structure == 0x01) ? (psn) : (psn*512))

/* ---- SD clock control ---- */
#define SD_CLOCK_ENABLE             (1)         /* Supply clock */
#define SD_CLOCK_DISABLE            (0)         /* Halt clock */

/* ---- Info1 interrupt mask ---- */
#define SD_INFO1_MASK_DET_DAT3      (0x0300u)   /* Card Insert and Remove (DAT3) */
#define SD_INFO1_MASK_DET_CD        (0x0018u)   /* Card Insert and Remove (CD) */
#define SD_INFO1_MASK_INS_DAT3      (0x0200u)   /* Card Insert (DAT3) */
#define SD_INFO1_MASK_INS_CD        (0x0010u)   /* Card Insert (CD) */
#define SD_INFO1_MASK_REM_DAT3      (0x0100u)   /* Card Remove (DAT3) */
#define SD_INFO1_MASK_REM_CD        (0x0008u)   /* Card Remove (CD) */
#define SD_INFO1_MASK_DATA_TRNS     (0x0004u)   /* Command sequence end */
#define SD_INFO1_MASK_TRNS_RESP     (0x0005u)   /* Command sequence end and Response end */
#define SD_INFO1_MASK_RESP          (0x0001u)   /* Response end */
#define SD_INFO1_MASK_DET_DAT3_CD   (SD_INFO1_MASK_DET_DAT3|SD_INFO1_MASK_DET_CD)

/* ---- Info2 interrupt mask ---- */
#define SD_INFO2_MASK_BWE           (0x827fu)   /* Write enable and All errors */
#define SD_INFO2_MASK_BRE           (0x817fu)   /* Read enable and All errors */
#define SD_INFO2_MASK_ERR           (0x807fu)   /* All errors */
#define SD_INFO2_MASK_ILA           (0x8000u)
#define SD_INFO2_MASK_CBSY          (0x4000u)   /* Command type register busy */
#define SD_INFO2_MASK_ERR6          (0x0040u)
#define SD_INFO2_MASK_ERR5          (0x0020u)
#define SD_INFO2_MASK_ERR4          (0x0010u)
#define SD_INFO2_MASK_ERR3          (0x0008u)
#define SD_INFO2_MASK_ERR2          (0x0004u)
#define SD_INFO2_MASK_ERR1          (0x0002u)
#define SD_INFO2_MASK_ERR0          (0x0001u)
#define SD_INFO2_MASK_WE            (0x0200u)   /* Write enable */
#define SD_INFO2_MASK_RE            (0x0100u)   /* Read enable */

/* ---- Sdio_info interrupt mask ---- */
#define SDIO_INFO1_MASK_EXWT        (0x8000u)
#define SDIO_INFO1_MASK_EXPUB52     (0x4000u)
#define SDIO_INFO1_MASK_IOIRQ       (0x0001u)   /* Interrupt from IO Card */

/* ---- Ext_cd interrupt mask ---- */
#define SD_EXT_CD_MASK_DET_P1       (0x0003u)
#define SD_EXT_CD_MASK_DET_P2       (0x0018u)
#define SD_EXT_CD_MASK_DET_P3       (0x00c0u)

#define SD_EXT_CD_MASK_CD_P1        (0x0004u)
#define SD_EXT_CD_MASK_CD_P2        (0x0020u)
#define SD_EXT_CD_MASK_CD_P3        (0x0100u)


/* ---- Sdio mode ---- */
#define SDIO_MODE_C52PUB            (0x0200u)
#define SDIO_MODE_IOABT             (0x0100u)
#define SDIO_MODE_RWREQ             (0x0004u)
#define SDIO_MODE_IOMOD             (0x0001u)   /* Interrupt from IO Card */

/* ---- Cc extmode register ---- */
#define CC_EXT_MODE_DMASDRW         (0x0002u)

/* ---- Time out count ---- */
#define SD_TIMEOUT_CMD              (100)       /* Commnad timeout */
#define SD_TIMEOUT_MULTIPLE         (1000)      /* Block transfer timeout */
#define SD_TIMEOUT_RESP             (1000)      /* Command sequence timeout */
#define SD_TIMEOUT_DMA_END          (1000)      /* DMA transfer timeout */
#define SD_TIMEOUT_ERASE_CMD        (10000)     /* Erase timeout */
#define SD_TIMEOUT_PROG_CMD         (10000)     /* Programing timeout */

/* ---- Data transafer direction ---- */
#define SD_TRANS_READ               (0)         /* Host <- SD */
#define SD_TRANS_WRITE              (1)         /* SD -> Host */

/* ---- Card register size ---- */
#define STATUS_DATA_BYTE            (64)        /* STATUS_DATA size */
#define SD_STATUS_BYTE              (64)        /* SD STATUS size */
#define SD_SCR_REGISTER_BYTE        (8)         /* SCR register size */

/* ---- Area distinction ---- */
#define SD_USER_AREA                (1u)
#define SD_PROT_AREA                (2u)

/* --- SD specification version ---- */
#define SD_SPEC_10                  (0) /* SD physical spec 1.01 (phys spec ver1.01) */
#define SD_SPEC_11                  (1) /* SD physical spec 1.10 (phys spec ver1.10) */
#define SD_SPEC_20                  (2) /* SD physical spec 2.00 (phys spec ver2.00) */  /* @050805 for Ver2.0 */

/* --- SD Card Speed ---- */
#define SD_CUR_SPEED                (0x01u)     /* Current speed mode */
#define SD_SUP_SPEED                (0x10u)     /* Supported speed mode */

/* ---- Cc extmode register ---- */
#define CC_EXT_MODE_DMASDRW         (0x0002u)

/* ==== Format parameter ==== */
#define SIZE_CARD_256KB             (256*1024/512)
#define SIZE_CARD_1MB               (1024*1024/512)
#define SIZE_CARD_2MB               (2*1024*1024/512)
#define SIZE_CARD_4MB               (4*1024*1024/512)
#define SIZE_CARD_8MB               (8*1024*1024/512)
#define SIZE_CARD_16MB              (16*1024*1024/512)
#define SIZE_CARD_32MB              (32*1024*1024/512)
#define SIZE_CARD_64MB              (64*1024*1024/512)
#define SIZE_CARD_128MB             (128*1024*1024/512)
#define SIZE_CARD_256MB             (256*1024*1024/512)
#define SIZE_CARD_504MB             (504*1024*1024/512)
#define SIZE_CARD_1008MB            (1008*1024*1024/512)
#define SIZE_CARD_1024MB            (1024*1024*1024/512)
#define SIZE_CARD_2016MB            (2016*1024*1024/512)
#define SIZE_CARD_2048MB            (2048ul*1024ul*1024ul/512ul)

/* For FS ver2.0 */
#define SIZE_CARD_4032MB            (4032ul*1024ul*2ul)
#define SIZE_CARD_4096MB            (4096ul*1024ul*2ul)
#define SIZE_CARD_8192MB            (8192ul*1024ul*2ul)
#define SIZE_CARD_16384MB           (16384ul*1024ul*2ul)
#define SIZE_CARD_32768MB           (32768ul*1024ul*2ul)

#define NUM_HEAD_2                  (2)
#define NUM_HEAD_4                  (4)
#define NUM_HEAD_8                  (8)
#define NUM_HEAD_16                 (16)
#define NUM_HEAD_32                 (32)
#define NUM_HEAD_64                 (64)
#define NUM_HEAD_128                (128)

/* For FS ver2.0 */
#define NUM_HEAD_255                (255)

#define SEC_PER_TRACK_16            (16)
#define SEC_PER_TRACK_32            (32)
#define SEC_PER_TRACK_63            (63)

#define SEC_PER_CLUSTER_1           (1)
#define SEC_PER_CLUSTER_2           (2)
#define SEC_PER_CLUSTER_8           (8)
#define SEC_PER_CLUSTER_16          (16)
#define SEC_PER_CLUSTER_32          (32)
#define SEC_PER_CLUSTER_64          (64)

#define SIZE_OF_BU_1                (1)
#define SIZE_OF_BU_2                (2)
#define SIZE_OF_BU_8                (8)
#define SIZE_OF_BU_16               (16)
#define SIZE_OF_BU_32               (32)
#define SIZE_OF_BU_64               (64)
#define SIZE_OF_BU_128              (128)
/* For FS ver2.0 */
#define SIZE_OF_BU_8192             (8192)

#define SD_SECTOR_SIZE              (512)

/* For Phy Ver2.0 */
/* Maximum AU size */
#define SD_ERASE_SECTOR             ((4096*1024)/512)


/* ==== Macro functions ==== */
#define _sd_set_hndl(hndl)          (SDHandle[SDSelectPort] = (hndl))
#define _sd_get_hndl()              (SDHandle[SDSelectPort])
#define _sd_get_hndls(a)            (SDHandle[a])


/* Fix function macro */
#define sd_outp(h,offset,data)      (*(volatile uint64_t *)(_SDHI0_BASE_+(offset)) = (data))
#define sd_inp(h,offset)            (*(volatile uint64_t *)(_SDHI0_BASE_+(offset)))

#define sd_outp32(h,offset,data)    (*(volatile uint32_t *)(_SDHI0_BASE_+(offset)) = (uint32_t )(data))
#define sd_inp32(h,offset)          (*(volatile uint32_t *)(_SDHI0_BASE_+(offset)))

/* ---- System unique ---- */
#define sd_set_fifo(h,data)

#define _sd_card_send_cmd(hndl,cmd,resp)    (_sd_card_send_cmd_arg((hndl),(cmd),(resp),0,0))


/* ==== Command type ==== */
/* ---- eSD commands ---- */
#define CMD43                       (0x052B)    /* SELECT_PARTITIONS */
#define CMD44                       (0x0C2C)    /* MANAGE_PARTITIONS */
#define CMD45                       (0x1C2D)    /* QUERY_PARTITIONS */

/* ==== Constants ==== */
#define SD_QUERY_PARTITION_LIST_BYTE    (512)
#define SD_SPLIT_PARTITION_BYTE         (512)

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
/* ==== Encrypted device key structure ==== */
/* ==== Format parameter structure ==== */
typedef struct _chs_recom{
    uint32_t    capa;
    uint8_t     heads;
    uint8_t     spt;
}CHS_RECOM;

typedef struct _scbu_recom{
    uint32_t    capa;
    uint8_t     sc;

    /* For overflow */
    uint16_t    bu;
}SCBU_RECOM;

/* ==== SD Driver work buffer (allocated by File system) ==== */
/* Hide from users */
/* ---- ""VAL DEF"" ---- */
typedef struct __sdhndl{            /* SD handle */
    uintptr_t   reg_base;               /* SDHI base address */
    uint32_t    card_sector_size;       /* Sector size (user area) */
    uint32_t    prot_sector_size;       /* Sector size (protect area) */
    uint32_t    erase_sect;             /* Erase block size */
    uint8_t     fat_type;               /* FAT type (FAT12:1 FAT16:2 FAT32:3 unknown:0)  */
    uint8_t     csd_structure;          /* CSD structure (Standard capacity:0 High capacity:1) @050805 for ver2.0 */
    uint8_t     csd_tran_speed;         /* CSD transfer speed */
    uint16_t    csd_ccc;                /* CSD command class */
    uint8_t     csd_copy;               /* CSD copy flag (not used) */
    uint8_t     csd_file_format;        /* CSD file format group */
    uint8_t     sd_spec;                /* SCR spec version (Ver1.0-1.01:0 Ver1.10:1) */
    uint8_t     if_mode;                /* Bus width (1bit:0 4bits:1) */
    uint8_t     speed_mode;             /* Card speed mode;
                                           Current speed  : 0
                                           Supported speed:0x10 */
    /* For SD phy ver2.0 */
    uint8_t     speed_class;            /* Card speed class */
    uint8_t     perform_move;           /* Card move performance */

    uint8_t     media_type;             /* Card type */
    uint8_t     write_protect;          /* Write protect:     OFF : 0
                                                           H/W WP : 1
                                           CSD  TMP_WRITE_PROTECT : 2
                                           CSD PERM_WRITE_PROTECT : 4
                                           SD ROM                 : 0x10 */
    uint8_t     io_flag;                /* Io initialize flag */
                                        /* Interrupt enable       : bit4
                                           Power on initialized   : bit2
                                           Memory initialized     : bit1
                                           Io func initialized    : bit0 */
    uint8_t     io_info;                /* Io function's information */
                                        /* Io ready               : bit7
                                           Number of io func      : bit6-bit4
                                           memory present         : bit3
                                           Reserved               : bit2-bit0 */
    uint16_t    int_info1;          /* SD_INFO1 status */
    uint16_t    int_info2;          /* SD_INFO2 status */
    uint16_t    int_info1_mask;     /* SD_INFO1_MASK status */
    uint16_t    int_info2_mask;     /* SD_INFO2_MASK status */
    uint16_t    int_io_info;        /* SDIO_INFO1 status */
    uint16_t    int_io_info_mask;   /* SDIO_INFO1_MASK status */
    uint32_t    voltage;            /* System supplied voltage */
    int32_t     error;              /* Error detail information */
    uint16_t    stop;               /* Compulsory stop flag */
    uint8_t     mount;              /* Mount flag (unmount:0 mount:1) */
    uint8_t     int_mode;           /* Interrupt flag detect method (polling:0 H/W interrupt:1) */
    uint8_t     trans_mode;         /* Data transfer method  PIO : 0
                                                      SD_BUF DMA : 0x2 */
    uint8_t     sup_card;           /* Support card;
                                                Memory (include MMC) : 0
                                                                  IO : 1 */
    uint8_t     sup_speed;          /* Support speed (Default:0 High-speed:1) */
    uint8_t     sup_ver;            /* Support version (ver1.1:0 ver2.x:1) */
    uint8_t     cd_port;            /* Card detect method (CD pin:0 DAT3:1) */
    uint8_t     port;               /* Card port number */
    int16_t     trans_sectors;      /* Maximum block counts per multiple command */
    int32_t     (*int_cd_callback)(int32_t arg);     /* Callback function for card detection */
    int32_t     (*int_format_callback)(int32_t arg); /* Callback function for card format */
    int32_t     (*int_callback)(int32_t arg);        /* Callback function for interrupt flags */
    int32_t     (*int_io_callback)(int32_t arg);     /* Callback function for interrupt flags */
    int32_t     resp_status;        /* R1/R1b response status */
    uint16_t    ocr[4/sizeof(uint16_t)];        /* OCR value */
    uint16_t    io_ocr[4/sizeof(uint16_t)];     /* IO OCR value */
    uint16_t    if_cond[4/sizeof(uint16_t)];    /* IF_COND value */    /* @050805 for ver2.0 */
    uint16_t    cid[16/sizeof(uint16_t)];       /* CID value */
    uint16_t    csd[16/sizeof(uint16_t)];       /* CSD value */
    uint16_t    dsr[2/sizeof(uint16_t)];        /* DSR value */

    /* @120807 Modified so that the head of array member is located on four byte boundary.  */
    uint16_t    dummy1;

    uint16_t    rca[4/sizeof(uint16_t)];           /* RCA value */
    uint16_t    scr[8/sizeof(uint16_t)];           /* SCR value */
    uint16_t    sdstatus[14/sizeof(uint16_t)];     /* SD STATUS value */

    /* @120807  Modified so that the head of array member is located on four byte boundary.  */
    uint16_t    dummy2;

    uint16_t    status_data[18/sizeof(uint16_t)];  /* STATUS DATA value (phys spec ver1.10) */

    /* @120807  Modified so that the head of array member is located on four byte boundary.  */
    uint16_t    dummy3;

    uint16_t    io_len[8];                         /* Io block length common:0 func:more than 1 */
    uint8_t     io_reg[8][0x14/sizeof(uint8_t)];   /* CCCR(=0) and FBR(1 to 7) value */
    uint8_t     cis[8][0x14/sizeof(uint8_t)];      /* CIS value (to be fixed) */
    uint8_t     *rw_buff;                   /* Work buffer pointer */
    uint32_t    buff_size;                  /* Work buffer size */

    int32_t     partition_id;               /* Partition ID for eSD */
}SDHNDL;
/* ""VAL DEF END"" */

/* ---- Variables used for format ---- */
typedef struct _sd_format_param{
    /* Format work buffer */
    uint8_t  *pbuff;                /* Work buffer address */
    uint32_t buff_sec_size;         /* Work buffer size (sector) */

    /* Format parameter */
    uint32_t    area_size;          /* Sectors per area(user/protect) */
    uint32_t    format_size;        /* Number of format sectors */

    uint16_t    fmt_spt;            /* Number of tracks */
    uint16_t    fmt_hn;             /* Number of heads */
    uint16_t    fmt_sc;             /* Sectors per cluster */
    uint16_t    fmt_bu;             /* Boundary unit (sector) */

    uint16_t    fmt_max;            /* Max cluster number */
    uint16_t    fmt_sf;             /* Sectors per FAT
                                          FAT12 and FAT16: BP22-BP23
                                          FAT 32 : BP36-BP39
                                    */

    uint32_t    fmt_nom;            /* Sectors per MBR */
    uint32_t    fmt_ssa;            /* Sectors per sysytem area */

    uint8_t     fmt_fatbit;         /* FAT12, FAT16 or FAT32 */
    /* For FS ver2.0 */
    uint16_t    fmt_rsc;            /* Reserved sector count */

    CHS_RECOM   *chs;
    SCBU_RECOM  *scbu;
    int32_t area;                       /* Format area */
    /* Format write function */
    int32_t (*write)(SDHNDL *hndl, struct _sd_format_param *sdfmt, uint8_t fill, uint32_t secno, int32_t seccnt,
            int32_t (*callback)(uint32_t arg1, uint32_t arg2));
    /* Format erase function  */
    int32_t (*erase)(SDHNDL *hndl, struct _sd_format_param *sdfmt, uint32_t secno, int32_t seccnt,
            int32_t (*callback)(uint32_t arg1, uint32_t arg2));
}SD_FMT_WORK;

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/
extern int32_t SDSelectPort;

extern SDHNDL *SDHandle[NUM_PORT];

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
/* ---- sd_init.c ---- */
extern int32_t _sd_init_hndl( SDHNDL *hndl, uint32_t mode, uint32_t voltage );
extern int32_t _sd_select_port( SDHNDL *hndl, int32_t portno );

/* ---- sd_mount.c ---- */
extern int32_t _sd_card_init(SDHNDL *hndl);
extern int32_t _sd_io_mount( SDHNDL *hndl);
extern int32_t _sd_mem_mount(SDHNDL *hndl);
extern int32_t _sd_set_io_speed(SDHNDL *hndl);
extern int32_t _sd_set_mem_speed(SDHNDL *hndl);
extern int32_t _sd_card_switch_func(SDHNDL *hndl, uint16_t h_arg,
                                    uint16_t l_arg);
extern int32_t _sd_card_get_status(SDHNDL *hndl);
extern int32_t _sd_card_get_scr(SDHNDL *hndl);
extern int32_t _sd_read_byte(SDHNDL *hndl, uint16_t cmd, uint16_t h_arg,
                            uint16_t l_arg, uint8_t *readbuff, uint16_t byte);
extern int32_t _sd_write_byte(SDHNDL *hndl, uint16_t cmd, uint16_t h_arg,
                            uint16_t l_arg, uint8_t *writebuff, uint16_t byte);
extern int32_t _sd_card_select_partition(SDHNDL *hndl, int32_t id);
extern int32_t _esd_get_partition_id(SDHNDL *hndl, int32_t *id);

/* ---- sd_trns.c ---- */
extern int32_t _sd_software_trans(SDHNDL *hndl, uint8_t *buff, uint32_t cnt, int32_t dir);
extern int32_t _sd_dma_trans(SDHNDL *hndl, uint32_t cnt);

/* ---- sd_read.c ---- */
/* No function */

/* ---- sd_write.c ---- */
extern int32_t _sd_write_sect(SDHNDL *hndl, uint8_t *buff, uint32_t psn,
        int32_t cnt, int32_t writemode);

/* ---- sd_io_read.c ---- */
extern int32_t _sdio_read(SDHNDL *hndl, uint8_t *buff, uint32_t func,
                            uintptr_t adr, int32_t cnt, uint32_t op_code);

/* ---- sd_io_write.c ---- */
extern int32_t _sdio_write(SDHNDL *hndl, uint8_t *buff, uint32_t func,
                            uintptr_t adr, int32_t cnt, uint32_t op_code);

/* ---- sd_io_direct.c ---- */
extern int32_t _sdio_direct(SDHNDL *hndl, uint8_t *buff, uint32_t func,
                            uintptr_t adr, uint32_t rw_flag, uint32_t raw_flag);

/* ---- sd_cd.c ---- */
extern int32_t _sd_check_media(SDHNDL *hndl);
extern int32_t _sd_get_ext_cd_int(SDHNDL *hndl);

/* ---- sd_cmd.c ---- */
extern int32_t _sd_send_cmd( SDHNDL *hndl, uint16_t cmd);
extern int32_t _sd_send_acmd(SDHNDL *hndl, uint16_t cmd, uint16_t h_arg, uint16_t l_arg);
extern int32_t _sd_send_mcmd(SDHNDL *hndl, uint16_t cmd, uint32_t startaddr);
extern int32_t _sd_card_send_cmd_arg(SDHNDL *hndl, uint16_t cmd, int32_t resp, uint16_t h_arg, uint16_t l_arg);
extern void _sd_set_arg(SDHNDL *hndl, uint16_t h_arg, uint16_t l_arg);
extern int32_t _sd_card_send_ocr(SDHNDL *hndl, int32_t type);
extern int32_t _sd_check_resp_error(SDHNDL *hndl);
extern int32_t _sd_get_resp( SDHNDL *hndl, int32_t resp);
extern int32_t _sd_check_csd(SDHNDL *hndl);
extern int32_t _sd_check_info2_err(SDHNDL *hndl);
extern int32_t _sd_send_iocmd(SDHNDL *hndl, uint16_t cmd, uint32_t arg);

/* ---- sd_int.c ---- */
extern int32_t    _sd_set_int_mask(  SDHNDL *hndl, uint16_t mask1, uint16_t mask2);
extern int32_t    _sd_clear_int_mask(SDHNDL *hndl, uint16_t mask1, uint16_t mask2);
extern int32_t _sd_get_int(SDHNDL *hndl);
extern int32_t _sd_clear_info(SDHNDL *hndl, uint16_t clear_info1,  uint16_t clear_info2);

/* ---- sdio_int.c ---- */
extern int32_t _sdio_set_int_mask(  SDHNDL *hndl, uint16_t mask);
extern int32_t _sdio_clear_int_mask(SDHNDL *hndl, uint16_t mask);
extern int32_t _sdio_clear_info(SDHNDL *hndl, uint16_t clear);
extern int32_t _sdio_get_int(SDHNDL *hndl);

/* ---- sd_util.c ---- */
extern int32_t _sd_set_clock(SDHNDL *hndl, int32_t clock, int32_t enable);
extern int32_t _sd_set_port( SDHNDL *hndl, int32_t port);
extern int32_t _sd_iswp(SDHNDL *hndl);
extern int32_t _sd_set_err(SDHNDL *hndl,   int32_t error);
extern int32_t _sd_bit_search(uint16_t data);
extern int32_t _sd_get_size(SDHNDL *hndl, uint32_t area);
extern int32_t _sd_standby( SDHNDL *hndl);
extern int32_t _sd_active(  SDHNDL *hndl);
extern int32_t _sd_inactive(SDHNDL *hndl);
extern int32_t _sdio_set_blocklen(SDHNDL *hndl, uint16_t len, uint8_t func);
extern int32_t _sd_memset(uint8_t *p,   uint8_t data, uint32_t cnt);
extern int32_t _sd_memcpy(uint8_t *dst, uint8_t *src, uint32_t cnt);
extern uint16_t _sd_rand(void);
extern void _sd_srand(uint32_t seed);

/* ---- sd_format.c ---- */
extern int32_t _sd_format(SDHNDL *hndl,SD_FMT_WORK *sdfmt, int32_t format_mode,
        int32_t (*callback)(uint32_t arg1, uint32_t arg2));
extern int32_t _sd_wait_rbusy(SDHNDL *hndl, int32_t time);

/* ---- sd_mount.c ---- */
extern int32_t _sd_calc_erase_sector(SDHNDL *hndl);

#endif    /* SD_H */

/* End of File */
