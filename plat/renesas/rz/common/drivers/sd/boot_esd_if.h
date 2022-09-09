/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**********************************************************************************************************************
 * File Name    : boot_esd_if.h
 * Version      : 1.0
 * Description  : eSD Boot Interface Definition.
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
#ifndef BOOT_ESD_IF_H
#define BOOT_ESD_IF_H

/* Access to the SD host controller FIFO */
#define        USE_DMAC        (0)                             /* DMA access */
#define        USE_CPU         (1)                             /* CPU access */
#define        SDC_ACC_TYPE    (USE_CPU)

/* Return value constant definition */
#define        ESD_BOOT_OK     (0)                          /* normal return */
#define        ESD_BOOT_NG     (-1)                         /* error return  */

#define        ESD_PARAM_ERROR (-2)

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
/* Output Structure */
typedef struct stesd_boot_output
{
    int32_t             ipartition_number;        /* output  used partition number                           */
    int32_t             ierror_code;              /* output  error code                                      */

    int32_t             iloadersize_blkno;        /* output  block number of read loader size; sector 1 to 7 */
    int32_t             iloadersize_blknum;       /* output  sector count of read loader size                */
    int32_t             iloaderprog_blkno;        /* output  block number of read loader program             */
    int32_t             iloaderprog_blknum;       /* output  sector count of read loader program             */
} stESD_BOOT_OUTPUT;

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

#endif    /* BOOT_ESD_IF_H */
/*---------------------------------------------------------------------------*/
/*      End Of File                                                          */
/*---------------------------------------------------------------------------*/
