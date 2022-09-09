/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**********************************************************************************************************************
 * File Name    : sd_init.c
 * Version      : 1.0
 * Description  : SD Driver initialize.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.09.2020 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdint.h>
#include "esdif.h"
#include "sys_sel.h"
#include "sd.h"
#include "ut_define.h"

#include <assert.h>

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
int32_t SDSelectPort;
SDHNDL *SDHandle[NUM_PORT];

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: esd_init
 * Description  : initialize SD Driver work memory started from SDHI register base
 *              : address specified by argument (base)
 *              : initialize port specified by argument (cd_port)
 *              : work memory is allocated quadlet boundary
 * Arguments    : uintptr_t base : SDHI register base address
 *              : void *workarea : SD Driver work memory
 *              : int32_t cd_port : card detect port
 *              :   SD_CD_SOCKET : card detect by CD pin
 *              :   SD_CD_DAT3 : card detect by DAT3 pin
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 *              : SD_ERR_CPU_IF : CPU-IF function error
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t esd_init(uintptr_t base, void *workarea, int32_t cd_port)
{
    int32_t  i;
    int32_t  j;
    uint16_t info1;
    uint8_t  *p_ptr;
    SDHNDL   *p_hndl;
    int32_t  ret;


    /* ==== Initialize work memory  ==== */
    if((uintptr_t)workarea == 0)
    {
        ret = SD_ERR;
        goto ErrExit;
    }

    /* ==== Work memory boundary check (quadlet unit) ==== */
    if((uintptr_t)workarea & 0x3u)
    {
        ret = SD_ERR;
        goto ErrExit;
    }

    /* ==== Check card detect port ==== */
    if((cd_port != SD_CD_SOCKET) && (cd_port != SD_CD_DAT3))
    {
        ret = SD_ERR;
        goto ErrExit;
    }

    /* Card detect port is fixed at CD pin */
/*    cd_port = SD_CD_SOCKET;*/

    /* ==== Initialize peripheral module ==== */
    if(esddev_init() != SD_OK)
    {
        ret = SD_ERR_CPU_IF;
        goto ErrExit;
    }

    /* Disable all interrupts */
    esddev_loc_cpu();

    p_hndl = (SDHNDL *)workarea;

    for(j = 0;j < NUM_PORT; j++)
    {
        SDHandle[j] = p_hndl;

        /* ---- Clear work memory zero value --- */
        p_ptr = (uint8_t *)p_hndl;
        for(i= sizeof(SDHNDL); i > 0 ;i--)
        {
            *p_ptr++ = 0;
        }

        /* ---- Set SDHI register address ---- */
        p_hndl->reg_base = base;
        p_hndl->cd_port = (uint8_t)cd_port;
        /* ---- Initialize maximum block count ---- */
        p_hndl->trans_sectors = 256;

        p_hndl->port = (uint8_t)j;        /* Set select port number */

        p_hndl++;    /* Point to next handle */
    }

    /* Return to select port0 */
    SDSelectPort = 0;
    p_hndl = _sd_get_hndl();

    /* ==== Initialize SDHI ==== */
    sd_outp(p_hndl,SD_INFO1_MASK,0x031d);
    sd_outp(p_hndl,SD_INFO2_MASK,0x837f);
    sd_outp(p_hndl,SDIO_INFO1_MASK,0xc007);
    sd_outp(p_hndl,SDIO_MODE,0x0000);
    info1 = sd_inp(p_hndl,SD_INFO1);
    sd_outp(p_hndl,SD_INFO1,(uint16_t)(info1&~0x0005u));
    sd_outp(p_hndl,SD_INFO2,0x0000);
    sd_outp(p_hndl,SDIO_INFO1,0x0000);
    if(NUM_PORT > 1)
    {
        sd_outp(p_hndl,EXT_SDIO,0x0777);
        sd_outp(p_hndl,EXT_CD_MASK,0x00ff);
        sd_outp(p_hndl,EXT_CD_DAT3_MASK,0x00ff);
    }
    sd_outp(p_hndl,SOFT_RST,0);
    sd_outp(p_hndl,SOFT_RST,1);

    /* BUS SIZE setting */
    sd_outp(p_hndl,HOST_MODE,0x00000101);

    /* 0x00a*; If SDCLK is 16.6MHz, set the count value for timeout 503msec */
    sd_outp(p_hndl,SD_OPTION,0x00ae);

    /* Enable all interrupts */
    esddev_unl_cpu();

    return SD_OK;
ErrExit:
    for(i = 0;i < NUM_PORT;i++)
    {
        SDHandle[i] = 0;    /* Relese SD handle */
    }
    return ret;
}
/**********************************************************************************************************************
 * End of function esd_init
 *********************************************************************************************************************/

#if 0

#endif

/**********************************************************************************************************************
 * Function Name: _sd_init_hndl
 * Description  : initialize following SD handle members
 *              : media_type : card type
 *              : write_protect : write protect
 *              : resp_status : R1/R1b response status
 *              : error : error detail information
 *              : stop : compulsory stop flag
 *              : prot_sector_size : sector size (protect area)
 *              : card registers : ocr, cid, csd, dsr, rca, scr, sdstatus and status_data
 * Arguments    : none.SDHNDL *hndl : SD handle
 *              : uint32_t mode : driver mode
 *              : uint32_t voltage : working voltage
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_init_hndl(SDHNDL *hndl, uint32_t mode, uint32_t voltage)
{
    int32_t i;

    hndl->media_type = SD_MEDIA_UNKNOWN;
    hndl->write_protect = 0;
    hndl->resp_status = STATE_IDEL;
    hndl->error = SD_OK;
    hndl->stop = 0;
    hndl->prot_sector_size = 0;
    hndl->voltage = voltage;
    hndl->speed_mode = 0;
    hndl->int_mode =   (uint8_t)(mode & 0x01u);
    hndl->trans_mode = (uint8_t)(mode & 0x02u);
    hndl->sup_card =   (uint8_t)(mode & 0x30u);
    hndl->sup_speed =  (uint8_t)(mode & 0x40u);
    hndl->sup_ver =    (uint8_t)(mode & 0x80u);

    /* Initialize card registers */
    for(i = 0; i < (4/sizeof(uint16_t)); ++i)
    {
        hndl->ocr[i] = 0;
    }
    for(i = 0; i < (16/sizeof(uint16_t)); ++i)
    {
        hndl->cid[i] = 0;
    }
    for(i = 0; i < (16/sizeof(uint16_t)); ++i)
    {
        hndl->csd[i] = 0;
    }
    for(i = 0; i < (2/sizeof(uint16_t)); ++i)
    {
        hndl->dsr[i] = 0;
    }
    for(i = 0; i < (4/sizeof(uint16_t)); ++i)
    {
        hndl->rca[i] = 0;
    }
    for(i = 0; i < (8/sizeof(uint16_t)); ++i)
    {
        hndl->scr[i] = 0;
    }
    /* For expand SD STATUS relevant fields */
    for(i = 0; i < (14/sizeof(uint16_t)); ++i)
    {
        hndl->sdstatus[i] = 0;
    }
    for(i = 0; i < (18/sizeof(uint16_t)); ++i)
    {
        hndl->status_data[i] = 0;
    }
    for(i = 0; i < (4/sizeof(uint16_t)); ++i)
    {
        hndl->if_cond[i] = 0;
    }

    if(hndl->sup_card & SD_MODE_IO)
    {
        int32_t j;

        hndl->io_flag = 0;
        hndl->io_info = 0;

        for(i = 0; i < (4/sizeof(uint16_t)); ++i)
        {
            hndl->io_ocr[i] = 0;
        }

        for(i = 0; i < 6; ++i)
        {
            for(j = 0; j < (0x14/sizeof(uint8_t)); ++j)
            {
                hndl->io_reg[i][j] = 0;
            }
        }
    }

    if(hndl->sup_ver == SD_MODE_VER2X)
    {
        hndl->if_cond[0] = 0;
        hndl->if_cond[1] = 0x00aa;
        if(hndl->voltage & 0x00FF8000)
        {
            hndl->if_cond[1] |= 0x0100;    /* High volatege : 2.7V-3.6V */
        }
        if(hndl->voltage & 0x00000F00)
        {
            hndl->if_cond[1] |= 0x0200;    /* Low volatege : 1.65V-1.95V */
        }
    }

    return SD_OK;
}
/**********************************************************************************************************************
 * End of function _sd_init_hndl
 *********************************************************************************************************************/

/* End of File */
