/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**********************************************************************************************************************
 * File Name    : sd_trans.c
 * Version      : 1.0
 * Description  : Data transfer.
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

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_software_trans
 * Description  : transfer data to/from card by software
 *              : this operations are used multiple command data phase
 *              : if dir is SD_TRANS_READ, data is from card to host
 *              : if dir is SD_TRANS_WRITE, data is from host to card.
 * Arguments    : SDHNDL *hndl : SD handle
 *              : uint8_t *buff : destination/source data buffer
 *              : uint32_t cnt : number of transfer bytes
 *              : int32_t dir : transfer direction.
 * Return Value : hndl->error : SD handle error value
 *              : SD_OK : end of succeed
 *              : SD_ERR: end of error.
 * Remark       : transfer finished, check CMD12 sequence refer to All end
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_software_trans(SDHNDL *hndl, uint8_t *buff, uint32_t cnt, int32_t dir)
{
    int32_t j;
    int32_t (*func)(uint8_t * arg1, uintptr_t arg2, uint32_t arg3);

    if (dir == SD_TRANS_READ)
    {
        func = esddev_read_data;
    }
    else
    {
        func = esddev_write_data;
    }

    for (j = cnt; j > 0; j--)
    {
        /* ---- Wait BWE/BRE interrupt ---- */
        if (esddev_int_wait(SD_TIMEOUT_MULTIPLE) != SD_OK)
        {
            _sd_set_err(hndl, SD_ERR_HOST_TOE);
            break;
        }

        if (hndl->int_info2 & SD_INFO2_MASK_ERR)
        {
            _sd_check_info2_err(hndl);
            break;
        }
        if (dir == SD_TRANS_READ)
        {
            _sd_clear_info(hndl, 0x0000, SD_INFO2_MASK_RE); /* Clear BRE and errors bit */
        }
        else
        {
            _sd_clear_info(hndl, 0x0000, SD_INFO2_MASK_WE); /* Clear BWE and errors bit */
        }

        /* Write/read to/from SD_BUF by 1 sector */
        if (( *func)(buff, (uintptr_t) hndl->reg_base + SD_BUF0, 512) != SD_OK)
        {
            _sd_set_err(hndl, SD_ERR_CPU_IF);
            break;
        }

        /* Update buffer */
        buff += 512;

    }

    return hndl->error;
}
/**********************************************************************************************************************
 * End of function _sd_software_trans
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_dma_trans
 * Description  : transfer data to/from card by DMA
 *              : this operations are multiple command data phas.
 * Arguments    : SDHNDL *hndl : SD handle
 *              : uint32_t cnt : number of transfer bytes.
 * Return Value : hndl->error : SD handle error value
 *              : SD_OK : end of succeed
 *              : SD_ERR: end of error.
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_dma_trans(SDHNDL *hndl, uint32_t cnt)
{
    /* ---- Check DMA transfer end  --- */
    /* Timeout value is depend on transfer size */
    if (esddev_wait_dma_end(cnt * 512) != SD_OK)
    {
        esddev_disable_dma(); /* Disable DMAC */
        _sd_set_err(hndl, SD_ERR_CPU_IF);
        goto ErrExit;
    }

    /* ---- Disable DMAC ---- */
    if (esddev_disable_dma() != SD_OK)
    {
        _sd_set_err(hndl, SD_ERR_CPU_IF);
        goto ErrExit;
    }

ErrExit :
    return hndl->error;
}
/**********************************************************************************************************************
 * End of function _sd_dma_trans
 *********************************************************************************************************************/

/* End of File */
