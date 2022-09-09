/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**********************************************************************************************************************
 * File Name    : sd_read.c
 * Version      : 1.0
 * Description  : Card read.
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
SD_DRV_CODE_SEC static int32_t _sd_single_read(SDHNDL *hndl, uint8_t *buff, uint32_t psn, int32_t mode);

/**********************************************************************************************************************
 * Function Name: esd_read_sect
 * Description  : read sector data from physical sector number (=psn) by the
 *              : number of sectors (=cnt)
 *              : if SD Driver mode is SD_MODE_SW, data transfer by
 *              : esddev_read_data function
 *              : if SD Driver mode is SD_MODE_DMA, data transfer by DMAC.
 * Arguments    : uint8_t *buff : read data buffer
 *              : uint32_t psn : read physical sector number
 *              : uint32_t cnt : number of read sectors.
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error.
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t esd_read_sect(uint8_t *buff, uint32_t psn, int32_t cnt)
{

    SDHNDL *p_hndl;
    int32_t i;
    int32_t j;
    int32_t ret;
    int32_t mode = 0;
    int32_t mmc_lastsect = 0;
    uint16_t info1_back;
    uint16_t ext_cd_back;
    uint16_t ext_dat3_back;
    uint16_t opt_back;

    p_hndl = _sd_get_hndl();
    if (p_hndl == 0)
    {
        return SD_ERR; /* Not initialized */
    }

    /* ---- Check card is mounted ---- */
    if ( !p_hndl->mount)
    {
        _sd_set_err(p_hndl, SD_ERR);
        return p_hndl->error; /* Not mounted yet */
    }

    p_hndl->error = SD_OK;

    /* ---- Is stop compulsory? ---- */
    if (p_hndl->stop)
    {
        p_hndl->stop = 0;
        _sd_set_err(p_hndl, SD_ERR_STOP);
        return SD_ERR_STOP;
    }

    /* ---- Is card existed? ---- */
    if (_sd_check_media(p_hndl) != SD_OK)
    {
        _sd_set_err(p_hndl, SD_ERR_NO_CARD); /* No card */
        return SD_ERR_NO_CARD;
    }

    /* Access area check */
    if ((psn >= p_hndl->card_sector_size) || ((psn + cnt) > p_hndl->card_sector_size))
    {
        _sd_set_err(p_hndl, SD_ERR);
        return p_hndl->error; /* Out of area */
    }

    /* If DMA transfer, buffer boundary is quadlet unit */
    if ((p_hndl->trans_mode & SD_MODE_DMA) && (((uintptr_t) buff & 0x03u) == 0))
    {
        mode = SD_MODE_DMA; /* Set DMA mode */
    }

    /* Transfer size is fixed (512 bytes) */
    sd_outp(p_hndl, SD_SIZE, 512);

    /* ==== Check status precede read operation ==== */
    if (_sd_card_send_cmd_arg(p_hndl, CMD13, SD_RESP_R1, p_hndl->rca[0], 0x0000)
            == SD_OK)
    {
        if ((p_hndl->resp_status & RES_STATE) != STATE_TRAN)
        { /* Not transfer state */
#if    1
            p_hndl->error = SD_ERR;
#endif
            goto ErrExit;
        }
    }
    else
    { /* SDHI error */
        goto ErrExit;
    }

    /* ==== Execute multiple transfer by 256 sectors ==== */
    for (i = cnt; i > 0;
            i -= TRANS_SECTORS, psn += TRANS_SECTORS, buff += (TRANS_SECTORS * 512))
    {

        /* ---- Is card existed? ---- */
        if (_sd_check_media(p_hndl) != SD_OK)
        {
            _sd_set_err(p_hndl, SD_ERR_NO_CARD); /* No card */
            goto ErrExit;
        }

        /* Set transfer sector numbers to SD_SECCNT */
        cnt = i - TRANS_SECTORS;
        if (cnt < 0)
        { /* Remaining sectors are less than TRANS_SECTORS */
            cnt = i;
        }
        else
        {
            cnt = TRANS_SECTORS;
        }

        if (cnt <= 2)
        {
            /* Disable SD_SECCNT */
            sd_outp(p_hndl, SD_STOP, 0x0000);
            for (j = cnt; j > 0; j--, psn++, buff += 512)
            {
                ret = _sd_single_read(p_hndl, buff, psn, mode);
                if (ret != SD_OK)
                {
                    opt_back = sd_inp(p_hndl, SD_OPTION);
                    sd_outp(p_hndl, SOFT_RST, 0);
                    sd_outp(p_hndl, SOFT_RST, 1);
                    sd_outp(p_hndl, SD_OPTION, opt_back);

                    /* 2010.03.19 Countermeasures for the problem that the clock is initialized after reset */
                    /* ---- Supply clock (data-transfer ratio) ---- */
                    if (_sd_set_clock(p_hndl, (int) p_hndl->csd_tran_speed, SD_CLOCK_ENABLE) != SD_OK)
                    {
                        return p_hndl->error;
                    }

                    break;
                }
            }
            return p_hndl->error;
        }

        /* Enable SD_SECCNT */
        sd_outp(p_hndl, SD_STOP, 0x0100);

        /* Issue CMD12 not automatically, if MMC last sector access */
        mmc_lastsect = 0;
        if ((p_hndl->media_type == SD_MEDIA_MMC) && (p_hndl->card_sector_size == (psn + cnt)))
        {
            mmc_lastsect = 1;
        }

        sd_outp(p_hndl, SD_SECCNT, (uint16_t )cnt);

        /* ---- Enable RespEnd and ILA ---- */
        _sd_set_int_mask(p_hndl, SD_INFO1_MASK_RESP, 0);

        if (mode == SD_MODE_DMA)
        { /* ==== DMA ==== */
            sd_set_fifo(p_hndl,0x0000); /* Disable FIFO */

            /* ---- Initialize DMAC ---- */
            if (esddev_init_dma((uintptr_t) buff, p_hndl->reg_base + SD_BUF0,
                    cnt * 512, SD_TRANS_READ) != SD_OK)
            {
                _sd_set_err(p_hndl, SD_ERR_CPU_IF);
                goto ErrExit;
            }
            sd_outp(p_hndl, CC_EXT_MODE, (uint16_t)(sd_inp(p_hndl,CC_EXT_MODE) | CC_EXT_MODE_DMASDRW)); /* enable DMA */
        }

        /* Issue CMD18 (READ_MULTIPLE_BLOCK) */
        if (mmc_lastsect)
        { /* MMC last sector access */
            if (_sd_send_mcmd(p_hndl, CMD18 | 0x7c00u, SET_ACC_ADDR) != SD_OK)
            {
                goto ErrExit_DMA;
            }
        }
        else
        {
            if (_sd_send_mcmd(p_hndl, CMD18, SET_ACC_ADDR) != SD_OK)
            {
                goto ErrExit_DMA;
            }
        }

        /* ---- Disable RespEnd and ILA ---- */
        _sd_clear_int_mask(p_hndl, SD_INFO1_MASK_RESP, SD_INFO2_MASK_ILA);

        if (mode == SD_MODE_SW)
        { /* ==== PIO ==== */
            /* Enable All end, BRE and errors */
            _sd_set_int_mask(p_hndl, SD_INFO1_MASK_DATA_TRNS, SD_INFO2_MASK_BRE);
            /* Software data transfer */
            ret = _sd_software_trans(p_hndl, buff, cnt, SD_TRANS_READ);
        }
        else
        { /* ==== DMA ==== */
            /* Disable card ins&rem interrupt for FIFO */
            info1_back = (uint16_t) (_sd_get_hndls(0)->int_info1_mask
                    & SD_INFO1_MASK_DET_CD);
            _sd_clear_int_mask(_sd_get_hndls(0), SD_INFO1_MASK_DET_CD, 0);
            ext_cd_back = sd_inp(p_hndl, EXT_CD_MASK);
            ext_dat3_back = sd_inp(p_hndl, EXT_CD_DAT3_MASK);
            sd_outp(p_hndl, EXT_CD_MASK, 0x00ff); /* Disable all interrupt */
            sd_outp(p_hndl, EXT_CD_DAT3_MASK, 0x00ff); /* Disable all interrupt */

            /* Enable All end and errors */
            _sd_set_int_mask(p_hndl, SD_INFO1_MASK_DATA_TRNS, SD_INFO2_MASK_ERR);
            sd_set_fifo(p_hndl,0x0002); /* Disable FIFO */

            /* DMA data transfer */
            ret = _sd_dma_trans(p_hndl, cnt);

            sd_outp(p_hndl, CC_EXT_MODE, (uint16_t)(sd_inp(p_hndl,CC_EXT_MODE) & ~CC_EXT_MODE_DMASDRW));
            sd_set_fifo(p_hndl,0x0000); /* Disable FIFO */
            _sd_set_int_mask(p_hndl, info1_back, 0);
            sd_outp(p_hndl, EXT_CD_MASK, ext_cd_back);
            sd_outp(p_hndl, EXT_CD_DAT3_MASK, ext_dat3_back);

        }

        if (ret != SD_OK)
        {
            goto ErrExit;
        }
        /* ---- Wait All end interrupt ---- */
        if (esddev_int_wait(SD_TIMEOUT_RESP) != SD_OK)
        {
            _sd_set_err(p_hndl, SD_ERR_HOST_TOE);
            goto ErrExit;
        }
        /* ---- Check errors ---- */
        if (p_hndl->int_info2 & SD_INFO2_MASK_ERR)
        {
            _sd_check_info2_err(p_hndl);
            goto ErrExit;
        }

        /* Clear All end bit */
        _sd_clear_info(p_hndl, SD_INFO1_MASK_DATA_TRNS, 0x0000);

        /* Disable All end, BRE and errors */
        _sd_clear_int_mask(p_hndl, SD_INFO1_MASK_DATA_TRNS, SD_INFO2_MASK_BRE);

        if (mmc_lastsect)
        {
            if (_sd_card_send_cmd(p_hndl,12,SD_RESP_R1b) != SD_OK)
            {
                /* Check OUT_OF_RANGE error */
                /* Ignore errors during last block access */
                if (p_hndl->resp_status & 0xffffe008ul)
                {
                    if ((psn + cnt) != p_hndl->card_sector_size)
                    {
                        goto ErrExit;
                        /* But for last block */
                    }
                    if (p_hndl->resp_status & 0x7fffe008ul)
                    {
                        goto ErrExit;
                        /* Not OUT_OF_RANGE error */
                    }
                    /* Clear OUT_OF_RANGE error */
                    p_hndl->resp_status &= 0x1f00u;
                    p_hndl->error = SD_OK;
                }
                else
                { /* SDHI error, ex)Timeout error so on */
                    goto ErrExit;
                }
            }
        }

        /* ==== Check status after read operation ==== */
        if (_sd_card_send_cmd_arg(p_hndl, CMD13, SD_RESP_R1, p_hndl->rca[0], 0x0000)
                != SD_OK)
        {
            /* Check OUT_OF_RANGE error */
            /* Ignore errors during last block access */
            if (p_hndl->resp_status & 0xffffe008ul)
            {
                if ((psn + cnt) != p_hndl->card_sector_size)
                {
                    goto ErrExit;
                    /* But for last block */
                }
                if (p_hndl->resp_status & 0x7fffe008ul)
                {
                    goto ErrExit;
                    /* Not OUT_OF_RANGE error */
                }
                /* Clear OUT_OF_RANGE error */
                p_hndl->resp_status &= 0x1f00u;
                p_hndl->error = SD_OK;
            }
            else
            { /* SDHI error, ex)Timeout error so on */
                goto ErrExit;
            }
        }

        if ((p_hndl->resp_status & RES_STATE) != STATE_TRAN)
        {
#if    1
            p_hndl->error = SD_ERR;
#endif
            goto ErrExit;
        }

        /* ---- Is stop compulsory? ---- */
        if (p_hndl->stop)
        {
            p_hndl->stop = 0;
            /* Data transfer stop (issue CMD12) */
            sd_outp(p_hndl, SD_STOP, 0x0001);
            i = 0; /* Set zero to break loop */
            _sd_set_err(p_hndl, SD_ERR_STOP);
        }
    }

    return p_hndl->error;

ErrExit_DMA :
    if (mode == SD_MODE_DMA)
    {
        esddev_disable_dma(); /* Disable DMA */
    }

ErrExit :
    sd_outp(p_hndl, CC_EXT_MODE, (uint16_t)(sd_inp(p_hndl,CC_EXT_MODE)
            & ~CC_EXT_MODE_DMASDRW)); /* Disable DMA */

    sd_set_fifo(p_hndl,0x0000); /* Disable FIFO */

    mode = p_hndl->error;

    /* ---- Clear error bits ---- */
    _sd_clear_info(p_hndl, SD_INFO1_MASK_TRNS_RESP, 0x837f);
    /* ---- Disable all interrupts ---- */
    _sd_clear_int_mask(p_hndl, SD_INFO1_MASK_TRNS_RESP, 0x837f);

    /* If the command sequence is being executed, stop the data transfer */
    if ((sd_inp(p_hndl,SD_INFO2) & SD_INFO2_MASK_CBSY) == SD_INFO2_MASK_CBSY)
    {
        uint16_t sd_option;
        uint16_t sd_clk_ctrl;

        /* ---- Enable All end ---- */
        _sd_set_int_mask(p_hndl, SD_INFO1_MASK_DATA_TRNS, 0);
        /* ---- Data transfer stop (issue CMD12) ---- */
        sd_outp(p_hndl, SD_STOP, 0x0001);
        /* ---- Wait All end ---- */
        esddev_int_wait(SD_TIMEOUT_RESP);
        _sd_clear_info(p_hndl, SD_INFO1_MASK_TRNS_RESP, 0x837f);
        _sd_clear_int_mask(p_hndl, SD_INFO1_MASK_DATA_TRNS, 0);

        esddev_loc_cpu();
        sd_option = sd_inp(p_hndl, SD_OPTION);
        sd_clk_ctrl = sd_inp(p_hndl, SD_CLK_CTRL);
        sd_outp(p_hndl, SOFT_RST, 0);
        sd_outp(p_hndl, SOFT_RST, 1);
        sd_outp(p_hndl, SD_STOP, 0x0000);
        sd_outp(p_hndl, SD_OPTION, sd_option);
        sd_outp(p_hndl, SD_CLK_CTRL, sd_clk_ctrl);
        esddev_unl_cpu();
    }

    /* Check CURRENT_STATE */
    if (_sd_card_send_cmd_arg(p_hndl, CMD13, SD_RESP_R1, p_hndl->rca[0], 0x0000) == SD_OK)
    {
        /* Not transfer state? */
        if ((p_hndl->resp_status & RES_STATE) != STATE_TRAN)
        {
            /* If not transfer state, issue CMD12 and migrate to transfer state*/
            _sd_card_send_cmd_arg(p_hndl, CMD12, SD_RESP_R1b, p_hndl->rca[0], 0x0000);
            /* Do not check CMD12 error because it is already an error */
        }
    }

    p_hndl->error = mode;

    _sd_clear_int_mask(p_hndl, SD_INFO1_MASK_TRNS_RESP, 0x837f);

    return p_hndl->error;

}
/**********************************************************************************************************************
 * End of function esd_read_sect
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_single_read
 * Description  : read sector data from physical sector number (=psn) by the
 *              : single block transfer
 *              : if SD Driver mode is SD_MODE_SW, data transfer by
 *              : esddev_read_data function
 *              : if SD Driver mode is SD_MODE_DMA, data transfer by DMAC.
 * Arguments    : SDHNDL *hndl : SD handle
 *              : uint8_t *buff : read data buffer
 *              : uint32_t psn : read physical sector number
 *              : int32_t mode : data transfer mode
 *              :   SD_MODE_SW : software
 *              :   SD_MODE_DMA : DMA.
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 *********************************************************************************************************************/
SD_DRV_CODE_SEC
static int32_t _sd_single_read(SDHNDL *p_hndl, uint8_t *buff, uint32_t psn,
        int32_t mode)
{
    int32_t ret;
    int32_t error;
    uint16_t info1_back;
    uint16_t ext_cd_back;
    uint16_t ext_dat3_back;

    /* ---- Enable RespEnd and ILA ---- */
    _sd_set_int_mask(p_hndl, SD_INFO1_MASK_RESP, SD_INFO2_MASK_ILA);

    if (mode == SD_MODE_DMA)
    { /* ==== DMA ==== */

        sd_set_fifo(p_hndl,0x0000); /* Disable FIFO */
        /* ---- Initialize DMAC ---- */
        if (esddev_init_dma((uintptr_t) buff, (uintptr_t) p_hndl->reg_base + SD_BUF0, 512, SD_TRANS_READ) != SD_OK)
        {
            _sd_set_err(p_hndl, SD_ERR_CPU_IF);
            goto ErrExit;
        }

        sd_outp(p_hndl, CC_EXT_MODE, 2); /* Enable DMA */
    }

    /* Issue CMD17 (READ_SINGLE_BLOCK) */

    if (_sd_send_mcmd(p_hndl, CMD17, SET_ACC_ADDR) != SD_OK)
    {

        goto ErrExit_DMA;
    }

    /* ---- Disable RespEnd and ILA ---- */
    _sd_clear_int_mask(p_hndl, SD_INFO1_MASK_RESP, SD_INFO2_MASK_ILA);

    if (mode == SD_MODE_SW)
    { /* ==== PIO ==== */
        /* Enable All end, BRE and errors */
        _sd_set_int_mask(p_hndl, SD_INFO1_MASK_DATA_TRNS, SD_INFO2_MASK_BRE);
        /* Software data transfer */
        ret = _sd_software_trans(p_hndl, buff, 1, SD_TRANS_READ);
    }
    else
    { /* ==== DMA ==== */
        /* Disable card ins&rem interrupt for FIFO */
        info1_back = (uint16_t) (_sd_get_hndls(0)->int_info1_mask
                & SD_INFO1_MASK_DET_CD);
        _sd_clear_int_mask(_sd_get_hndls(0), SD_INFO1_MASK_DET_CD, 0);
        ext_cd_back = sd_inp(p_hndl, EXT_CD_MASK);
        ext_dat3_back = sd_inp(p_hndl, EXT_CD_DAT3_MASK);
        sd_outp(p_hndl, EXT_CD_MASK, 0x00ff); /* Disable all interrupt */
        sd_outp(p_hndl, EXT_CD_DAT3_MASK, 0x00ff); /* Disable all interrupt */

        /* Enable All end and errors */
        _sd_set_int_mask(p_hndl, SD_INFO1_MASK_DATA_TRNS, SD_INFO2_MASK_ERR);
        sd_set_fifo(p_hndl,0x0002); /* Disable FIFO */

        /* esddev_int_wait(10); */
        /* DMA data transfer */
        ret = _sd_dma_trans(p_hndl, 1);

        sd_outp(p_hndl, CC_EXT_MODE, (uint16_t)(sd_inp(p_hndl,CC_EXT_MODE) &
                ~CC_EXT_MODE_DMASDRW));

        sd_set_fifo(p_hndl,0x0000); /* Disable FIFO */
        _sd_set_int_mask(p_hndl, info1_back, 0);
        sd_outp(p_hndl, EXT_CD_MASK, ext_cd_back);
        sd_outp(p_hndl, EXT_CD_DAT3_MASK, ext_dat3_back);
    }

    if (ret != SD_OK)
    {
        goto ErrExit;
    }
    /* ---- Wait All end interrupt ---- */
    if (esddev_int_wait(SD_TIMEOUT_RESP) != SD_OK)
    {
        _sd_set_err(p_hndl, SD_ERR_HOST_TOE);
        goto ErrExit;
    }

    /* ---- Check errors ---- */
    if (p_hndl->int_info2 & SD_INFO2_MASK_ERR)
    {
        _sd_check_info2_err(p_hndl);
        goto ErrExit;
    }

    /* Clear All end bit */
    _sd_clear_info(p_hndl, SD_INFO1_MASK_DATA_TRNS, 0x0000);

    /* Disable All end, BRE and errors */
    _sd_clear_int_mask(p_hndl, SD_INFO1_MASK_DATA_TRNS, SD_INFO2_MASK_BRE);

    /* ==== Check status after read operation ==== */
    if (_sd_card_send_cmd_arg(p_hndl, CMD13, SD_RESP_R1, p_hndl->rca[0], 0x0000) != SD_OK)
    {
        /* Check OUT_OF_RANGE error */
        /* Ignore errors during last block access */
        if (p_hndl->resp_status & 0xffffe008ul)
        {
            if ((psn + 1) != p_hndl->card_sector_size)
            {
                goto ErrExit;
                /* But for last block */
            }
            if (p_hndl->resp_status & 0x7fffe008ul)
            {
                goto ErrExit;
                /* Not OUT_OF_RANGE error */
            }
            /* Clear OUT_OF_RANGE error */
            p_hndl->resp_status &= 0x1f00u;
            p_hndl->error = SD_OK;
        }
        else
        { /* SDHI error, ex)Timeout error so on */
            goto ErrExit;
        }
    }

    return p_hndl->error;

ErrExit_DMA :
    if (mode == SD_MODE_DMA)
    {
        esddev_disable_dma(); /* Disable DMA */
    }

ErrExit :
    error = p_hndl->error;
    _sd_clear_info(p_hndl, SD_INFO1_MASK_TRNS_RESP, 0x837f);
    _sd_clear_int_mask(p_hndl, SD_INFO1_MASK_TRNS_RESP, 0x837f);
    _sd_set_int_mask(p_hndl, SD_INFO1_MASK_DATA_TRNS, 0);
    _sd_card_send_cmd_arg(p_hndl, CMD13, SD_RESP_R1, p_hndl->rca[0], 0x0000);
    _sd_clear_int_mask(p_hndl, SD_INFO1_MASK_TRNS_RESP, 0x837f);
    p_hndl->error = error;

    return p_hndl->error;
}
/**********************************************************************************************************************
 * End of function _sd_single_read
 *********************************************************************************************************************/

/* End of File */
