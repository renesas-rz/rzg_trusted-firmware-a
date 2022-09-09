/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**********************************************************************************************************************
 * File Name    : sd_mount.c
 * Version      : 1.0
 * Description  : Card mount.
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
SD_DRV_ZIDATA_SEC static uint16_t stat_buff[64/sizeof(uint16_t)];

/**********************************************************************************************************************
 * Function Name: esd_mount
 * Description  : mount SD memory card user area
 *              : can be access user area after this function is finished
 *              : without errors
 *              : turn on power
 *              : 
 *              : following is available SD Driver mode
 *              : SD_MODE_POLL: software polling
 *              : SD_MODE_HWINT: hardware interrupt
 *              : SD_MODE_SW: software data transfer (SD_BUF)
 *              : SD_MODE_DMA: DMA data transfer (SD_BUF)
 *              : SD_MODE_MEM: only memory cards
 *              : SD_MODE_IO: memory and io cards
 *              : SD_MODE_COMBO: memory ,io and combo cards
 *              : SD_MODE_DS: only default speed
 *              : SD_MODE_HS: default and high speed
 *              : SD_MODE_VER1X: ver1.1 host
 *              : SD_MODE_VER2X: ver2.x host
 * Arguments    : uint32_t mode : SD Driver operation mode.
 *              : uint32_t voltage : operation voltage.
 * Return Value : hndl->error : SD handle error value
 *              : SD_OK : end of succeed
 *              : other: end of error
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t esd_mount(uint32_t mode, uint32_t voltage)
{
    SDHNDL     *p_hndl;
    uint8_t  spec;
    uint16_t info1_back;

    p_hndl = _sd_get_hndl();
    if(p_hndl == 0)
    {
        return SD_ERR;    /* Not initialized */
    }

    /* ==== Check work buffer is allocated ==== */
    if(p_hndl->rw_buff == 0)
    {
        return SD_ERR;    /* Not allocated yet */
    }
    
    /* ==== Initialize parameter ==== */
    _sd_init_hndl(p_hndl, mode, voltage);
    p_hndl->error = SD_OK;
    
    /* ==== Is card inserted? ==== */
    if(_sd_check_media(p_hndl) != SD_OK)
    {
        _sd_set_err(p_hndl,SD_ERR_NO_CARD);
        return p_hndl->error;        /* Not inserted */
    }
    
    /* ==== Power on sequence ==== */
    /* ---- Turn on voltage ---- */
    if(esddev_power_on() != SD_OK)
    {
        _sd_set_err(p_hndl,SD_ERR_CPU_IF);
        goto ERR_EXIT;
    }

    /* ---- Set single port ---- */
    _sd_set_port(p_hndl,SD_PORT_SERIAL);

    /* ---- Supply clock (card-identification ratio) ---- */
    if(_sd_set_clock(p_hndl,SD_CLK_400kHz,SD_CLOCK_ENABLE) != SD_OK)
    {
        return p_hndl->error;        /* Not inserted */
    }

    esddev_int_wait(2);    /* Add wait function  */

    esddev_loc_cpu();

    info1_back = sd_inp(p_hndl,SD_INFO1);
    info1_back &= 0xfff8;
    sd_outp(p_hndl,SD_INFO1,info1_back);
    sd_outp(p_hndl,SD_INFO2,0);

    esddev_unl_cpu();

    /* ==== Initialize card and distinguish card type ==== */
    if(_sd_card_init(p_hndl) != SD_OK)
    {
        goto ERR_EXIT;    /* Failed card initialize */
    }

    if(p_hndl->media_type & SD_MEDIA_MEM)     /* With memory part */
    {
        /* ==== Check card registers ==== */
        /* ---- Check CSD register ---- */
        if(_sd_check_csd(p_hndl) != SD_OK)
        {
            goto ERR_EXIT;
        }
        
        /* ---- No check other registers (to be create) ---- */
        
        /* Get user area size */
        if(_sd_get_size(p_hndl,SD_USER_AREA) != SD_OK)
        {
            goto ERR_EXIT;
        }
            
        /* Check write protect */
        p_hndl->write_protect |= (uint8_t)_sd_iswp(p_hndl);
    }
    
    if(p_hndl->media_type & SD_MEDIA_MEM)     /* With memory part */
    {
        if(_sd_mem_mount(p_hndl) != SD_OK)
        {
            goto ERR_EXIT;
        }
    }

    /* If SD memory card, get SCR register */
    if(p_hndl->media_type & SD_MEDIA_SD)
    {
        if(_sd_card_get_scr(p_hndl) != SD_OK)
        {
            goto ERR_EXIT;
        }

        spec = (uint8_t)((p_hndl->scr[0] & 0x0F00u) >> 8u);
        if(spec)    /* ---- More than phys spec ver1.10 ---- */
        {
            p_hndl->sd_spec = spec;
            if(p_hndl->sup_speed == SD_MODE_HS)
            {
                /* Set memory part speed */
                if(_sd_set_mem_speed(p_hndl) != SD_OK)
                {
                    goto ERR_EXIT;
                }
            }
            _sd_calc_erase_sector(p_hndl);

        }

        else        /* ---- Phys spec ver1.00 or ver1.01 ---- */
        {
            p_hndl->sd_spec = SD_SPEC_10;
        }
    }

    /* ---- Set mount flag ---- */
    p_hndl->mount = 1;

    return p_hndl->error;

ERR_EXIT:
    /* ---- Halt clock ---- */
    _sd_set_clock(p_hndl,0,SD_CLOCK_DISABLE);
    return p_hndl->error;
}
/**********************************************************************************************************************
 * End of function esd_mount
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_card_init
 * Description  : initialize card from idle state to stand-by
 *              : distinguish card type (SD, MMC, IO or COMBO)
 *              : get CID, RCA, CSD from the card.
 * Arguments    : SDHNDL *hndl : SD handle.
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_card_init(SDHNDL *hndl)
{
    int32_t ret;
    int32_t i;

    /* ==== Transfer idle state (issue CMD0) ==== */
    for(i=0; i < 3; i++)
    {
        ret = _sd_send_cmd(hndl,CMD0);
        if(ret == SD_OK)
        {
            break;
        }
    }

    if(ret != SD_OK)
    {
        return SD_ERR;    /* Error for CMD0 */
    }
    /* Clear error by reissuing CMD0 */
    hndl->error = SD_OK;

    hndl->media_type |= SD_MEDIA_SD;

    /* Refer "2.4.7.2 Booting from eSD correspondence" */
    /* When CMD0 is executed, the physical partition returns to #0 which is UserArea */
    hndl->partition_id = 0;

    if(hndl->sup_ver == SD_MODE_VER2X)
    {
        ret = _sd_card_send_cmd_arg(hndl,CMD8,SD_RESP_R7,hndl->if_cond[0],
            hndl->if_cond[1]);
        if(ret == SD_OK)
        {
            /* Check R7 response */
            if(hndl->if_cond[0] & 0xf000)
            {
                hndl->error = SD_ERR_IFCOND_VER;
                return SD_ERR;
            }
            if((hndl->if_cond[1] & 0x00ff) != 0x00aa)
            {
                hndl->error = SD_ERR_IFCOND_ECHO;
                return SD_ERR;
            }
        }
        else
        {
            /* ==== Clear illegal command error for CMD8 ==== */
            for(i=0; i < 3; i++)
            {
                ret = _sd_send_cmd(hndl,CMD0);
                if(ret == SD_OK)
                {
                    break;
                }
            }

            hndl->error = SD_OK;
        }
    }

    /* Set OCR (issue ACMD41) */
    ret = _sd_card_send_ocr(hndl,(int32_t)hndl->media_type);

    if(hndl->error == SD_ERR_CARD_LOCK)
    {
        return SD_ERR;
    }

    /* Clear error due to card distinction */
    hndl->error = SD_OK;
    
    if(ret != SD_OK)
    {
        /* Softreset for error clear (issue CMD0) */
        for(i=0; i < 3; i++)
        {
            ret = _sd_send_cmd(hndl,CMD0);
            if( ret == SD_OK)
            {
                break;
            }
        }
        if(ret != SD_OK)
        {
            return SD_ERR;    /* Error for CMD0 */
        }
        /* Clear error by reissuing CMD0 */
        hndl->error = SD_OK;
        /* ---- Get OCR (issue CMD1) ---- */
        if((ret = _sd_card_send_ocr(hndl,SD_MEDIA_MMC)) == SD_OK)
        {
            /* MMC */
            hndl->media_type = SD_MEDIA_MMC;
            hndl->error = SD_OK;
        }
        else
        {
            /* Unknown card */
            hndl->media_type = SD_MEDIA_UNKNOWN;
            _sd_set_err(hndl,SD_ERR_CARD_TYPE);
            return SD_ERR;
        }
    }

    /* ---- Get CID (issue CMD2) ---- */
    if(_sd_card_send_cmd(hndl,CMD2,SD_RESP_R2_CID) != SD_OK)
    {
        return SD_ERR;
    }
    
/* GET_RCA: */
    /* ---- Get RCA (issue CMD3) ---- */
    if(hndl->media_type & SD_MEDIA_COMBO)       /* IO or SD */
    {
        for(i=0; i < 3; i++)
        {
            if(_sd_card_send_cmd(hndl,CMD3,SD_RESP_R6) != SD_OK)
            {
                return SD_ERR;
            }
            if(hndl->rca[0] != 0x00)
            {
                if(hndl->media_type & SD_MEDIA_IO)
                {
                    hndl->io_flag |= SD_IO_POWER_INIT;
                }
                break;
            }
        }
        /* Illegal RCA */
        if(i == 3)
        {
            _sd_set_err(hndl,SD_ERR_CARD_CC);
            return SD_ERR;
        }
    }
    else
    {
        hndl->rca[0] = (uint16_t)(hndl->port + 1);
        if(_sd_card_send_cmd_arg(hndl,CMD3,SD_RESP_R1,hndl->rca[0],0x0000) != SD_OK)
        {
            return SD_ERR;
        }
    }

    /* ==== Stand-by state  ==== */
    
    if(hndl->media_type == SD_MEDIA_IO)
    {
        return SD_OK;
    }

    /* ---- Get CSD (issue CMD9) ---- */
    if(_sd_card_send_cmd_arg(hndl,CMD9,SD_RESP_R2_CSD,hndl->rca[0],0x0000) != SD_OK)
    {
        return SD_ERR;
    }

    hndl->dsr[0] = 0x0000;

    if(hndl->media_type & SD_MEDIA_MEM)
    {
        /* Is DSR implimented? */
        if(hndl->csd[3] & 0x0010u)
        {    /* Implimented */
            /* Set DSR (issue CMD4) */
            hndl->dsr[0] = 0x0404;
        }
    }
    
    return SD_OK;
}
/**********************************************************************************************************************
 * End of function _sd_card_init
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_mem_mount
 * Description  : mount memory part from stand-by to transfer state.
 * Arguments    : SDHNDL *hndl : SD handle.
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error.
 * Remark       : Added processing to select the physical partition #1
 *              : If you can select #1, issue CMD45.
 *              : After that, the currently selected the physical partition is obtained 
 *              : and saved in the internal variable hndl->partition_id.
 *              : *** About reason not to issue CMD45 unconditionally
 *              : *** See esd_select_partition() function column
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_mem_mount(SDHNDL *hndl)
{
    /* Case of combo, already supplied data transfer clock */
    if((hndl->media_type & SD_MEDIA_IO) == 0)
    {
        /* Def ESD_DRIVER_CLOCK_SWITCH *** Must be enabled */
        /* ---- Supply clock (data-transfer ratio) ---- */
        if(_sd_set_clock(hndl,(int)hndl->csd_tran_speed,SD_CLOCK_ENABLE) != SD_OK)
        {
            goto ERR_EXIT;
        }
    }
    
    /* ==== Data-transfer mode ==== */
    if(_sd_card_send_cmd_arg(hndl,CMD7,SD_RESP_R1b,hndl->rca[0],0x0000) != SD_OK)
    {
        goto ERR_EXIT;
    }

    /* select the physical partition #1 */
    if(_sd_card_select_partition(hndl, 1) == SD_OK)
    {
        /* Get changed partition ID from device */
        int32_t tmp_id;
        if(_esd_get_partition_id(hndl, &tmp_id) == SD_OK)
        {
            /* Save ID obtained from device */
            hndl->partition_id = tmp_id;
        }
    }
    else
    {
        /* It is NG in the internal function, but it is treated as SD_OK considering the subsequent processing */
        hndl->error = SD_OK;
        /* Don't save ID */
    }
    
    /* ---- Set block length (issue CMD16) ---- */
    if(_sd_card_send_cmd_arg(hndl,CMD16,SD_RESP_R1,0x0000,0x0200) != SD_OK)
    {
        goto ERR_EXIT;
    }

    /* If 4bits transfer supported (SD memory card mandatory), change bus width 4bits */
    if(hndl->media_type & SD_MEDIA_SD)
    {
        _sd_set_port(hndl,SD_PORT_PARALLEL);
    }

    /* Clear pull-up DAT3 */
    if(hndl->media_type & SD_MEDIA_SD)
    {
        if(_sd_send_acmd(hndl,ACMD42,0,0) != SD_OK)
        {
            goto ERR_EXIT;
        }
        /* Check R1 resp */
        if(_sd_get_resp(hndl,SD_RESP_R1) != SD_OK)
        {
            goto ERR_EXIT;
        }
    }

    /* If SD memory card, get SD Status */
    if(hndl->media_type & SD_MEDIA_SD)
    {
        if(_sd_card_get_status(hndl) != SD_OK)
        {
            goto ERR_EXIT;
        }
        /* Get protect area size */
        if(_sd_get_size(hndl,SD_PROT_AREA) != SD_OK)
        {
            goto ERR_EXIT;
        }
    }

    return SD_OK;

ERR_EXIT:
    return hndl->error;
}
/**********************************************************************************************************************
 * End of function _sd_mem_mount
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_set_mem_speed
 * Description  : query high speed supported
 *              : transfer card high speed mode.
 * Arguments    : SDHNDL *hndl : SD handle.
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error.
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_set_mem_speed(SDHNDL *hndl)
{
    /* Query func */
    if(_sd_card_switch_func(hndl,0x00FF,0xFF00) != SD_OK)
    {
        goto ERR_EXIT;
    }
    if(hndl->speed_mode & SD_SUP_SPEED)     /* High speed supported */
    {

        /* Make transfer card high speed mode */
        if(_sd_card_switch_func(hndl,0x80FF,0xFF01) != SD_OK)
        {
            goto ERR_EXIT;
        }

        /* Case of combo card, set clock frequency high speed after transfering  io part high speed */
        if(hndl->media_type == SD_MEDIA_SD)
        {
            hndl->csd_tran_speed = SD_CLK_50MHz;
            hndl->speed_mode |= SD_CUR_SPEED;
        }

        hndl->csd[2] &= 0x00ff;
        hndl->csd[2] |= 0x5a00;

    }

    return SD_OK;

ERR_EXIT:
    return hndl->error;
}
/**********************************************************************************************************************
 * End of function _sd_set_mem_speed
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_card_switch_func
 * Description  : issue SWITCH FUNC command
 *              : query card is high speed supported
 *              : make transfer card high speed mode.
 * Arguments    : SDHNDL *hndl : SD handle
 *              : uint16_t h_arg : command argument high [31:16]
 *              : uint16_t l_arg : command argument low [15:0]
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_card_switch_func(SDHNDL *hndl, uint16_t h_arg, uint16_t l_arg)
{
    int32_t i;
    uint8_t *p_rw_buff = (uint8_t *)stat_buff;
    
    if(_sd_read_byte(hndl,CMD6,h_arg,l_arg,p_rw_buff,STATUS_DATA_BYTE) != SD_OK)
    {
        return SD_ERR;
    }

    /* ---- Save STATUS DATA ---- */
    for(i=0; i<9 ;i++)
    {
        hndl->status_data[i] = (stat_buff[i] << 8) | (stat_buff[i] >> 8);
    }
    
    if(!(h_arg & 0x8000))       /* Case of query */
    {
        /* ---- Save high speed support ---- */
        if(hndl->status_data[6] & 0x0002u)
        {
            hndl->speed_mode |= SD_SUP_SPEED;
        }
        else
        {
            hndl->speed_mode &= (uint8_t)~SD_SUP_SPEED;
        }
    }

    return SD_OK;
}
/**********************************************************************************************************************
 * End of function _sd_card_switch_func
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_card_get_status
 * Description  : get SD Status (issue ACMD13).
 * Arguments    : SDHNDL *hndl : SD handle
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_card_get_status(SDHNDL *hndl)
{
    int32_t ret;
    int32_t i;
    uint8_t *p_rw_buff = (uint8_t *)stat_buff;
    
    /* ---- Get SD Status (issue ACMD13) ---- */
    if(_sd_read_byte(hndl,ACMD13,0,0,p_rw_buff,SD_STATUS_BYTE) != SD_OK)
    {
        return SD_ERR;
    }
    
    /* ---- Distinguish SD ROM card ---- */
    /* Modifies for Little endian */
    if((p_rw_buff[2] & 0xffu) == 0x00)    /* [495:488] = 0x00 */
    {
        ret = SD_OK;
        /* Correspond to SD ROM Card judgment position error */
        if((p_rw_buff[3] & 0xffu) == 0x01)
        {
            hndl->write_protect |= SD_WP_ROM;
        }
    }
    else
    {
        ret = SD_ERR;
        _sd_set_err(hndl,SD_ERR_CARD_ERROR);
    }

    /* @060526 for SD phy ver2.0 */
    hndl->speed_class = p_rw_buff[8];
    hndl->perform_move = p_rw_buff[9];

    
    /* ---- Save SD STATUS ---- */
    /* @050916 for expand SD STATUS relevant fields */
    for(i = 0;i < (14/sizeof(uint16_t)) ;i++)
    {
        /* Modifies for Little endian */
        hndl->sdstatus[i] = (stat_buff[i] << 8) | (stat_buff[i] >> 8); 

    }
    
    return ret;
}
/**********************************************************************************************************************
 * End of function _sd_card_get_status
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_card_get_scr
 * Description  : get SCR register (issue ACMD51).
 * Arguments    : SDHNDL *hndl : SD handle
 * Return Value : SD_OK : end of succeed
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_card_get_scr(SDHNDL *hndl)
{
    uint8_t *p_rw_buff = (uint8_t *)stat_buff;

    /* ---- Get SCR register (issue ACMD51) ---- */
    if(_sd_read_byte(hndl,ACMD51,0,0,p_rw_buff,SD_SCR_REGISTER_BYTE) != SD_OK)
    {
        return SD_ERR;
    }

    /* ---- Save SCR register ---- */
    /* Modifies for Little endian */
    hndl->scr[0] = (stat_buff[0] << 8) | (stat_buff[0] >> 8);
    hndl->scr[1] = (stat_buff[1] << 8) | (stat_buff[1] >> 8);
    hndl->scr[2] = (stat_buff[2] << 8) | (stat_buff[2] >> 8);
    hndl->scr[3] = (stat_buff[3] << 8) | (stat_buff[3] >> 8);

    return SD_OK;
}
/**********************************************************************************************************************
 * End of function _sd_card_get_scr
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_read_byte
 * Description  : read byte data from card
 *              : issue byte data read command and read data from SD_BUF
 *              : using following commands
 *              : SD STATUS(ACMD13),SCR(ACMD51),NUM_WRITE_BLOCK(ACMD22),
 *              : SWITCH FUNC(CMD6).
 * Arguments    : SDHNDL *hndl :      SD handle
 *              : uint16_t cmd :      command code
 *              : uint16_t h_arg :    command argument high [31:16]
 *              : uint16_t l_arg :    command argument low [15:0]
 *              : uint8_t *readbuff : read data buffer
 *              : uint16_t byte :     the number of read bytes
 * Return Value : SD_OK : end of succeed.
 * Remark       : transfer type is PIO
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_read_byte(SDHNDL *hndl, uint16_t cmd, uint16_t h_arg,
    uint16_t l_arg, uint8_t *readbuff, uint16_t byte)
{
    /* ---- Disable SD_SECCNT ---- */
    sd_outp(hndl,SD_STOP,0x0000);

    /* ---- Set transfer bytes ---- */
    sd_outp(hndl,SD_SIZE,byte);

    /* ---- Issue command ---- */
    if(cmd & 0x0040u)       /* ACMD13, ACMD22 and ACMD51 */
    {
        if(_sd_send_acmd(hndl,cmd,h_arg,l_arg) != SD_OK)
        {
            /* For SD_ERR_END_BIT and SD_ERR_CRC, processing is continued because data is stored in FIFO */
            if((hndl->error == SD_ERR_END_BIT) || (hndl->error == SD_ERR_CRC))
            {
                ; /* DO NOTHING */
            }
            else
            {
                goto ErrExit;
            }
        }
    }
    else                /* CMD6 and CMD30 */ /* CMD45 */
    {
        _sd_set_arg(hndl,h_arg,l_arg);
        if(_sd_send_cmd(hndl,cmd) != SD_OK)
        {
            /* CMD45 non-compatible device countermeasures.To error handling*/
            goto ErrExit;
        }
    }
    /* ---- Check R1 response ---- */
    if(_sd_get_resp(hndl,SD_RESP_R1) != SD_OK)
    {
        goto ErrExit;
    }

    /* Enable All end, BRE and errors */
    _sd_set_int_mask(hndl,SD_INFO1_MASK_DATA_TRNS,SD_INFO2_MASK_BRE);

    /* ---- Wait BRE interrupt ---- */
    if(esddev_int_wait(SD_TIMEOUT_MULTIPLE) != SD_OK)
    {
        _sd_set_err(hndl,SD_ERR_HOST_TOE);
        goto ErrExit;
    }    

    /* ---- Check errors ---- */
    if(hndl->int_info2&SD_INFO2_MASK_ERR)
    {
        _sd_check_info2_err(hndl);
        goto ErrExit;
    }

    _sd_clear_info(hndl,0x0000,SD_INFO2_MASK_RE);    /* clear BRE bit */

    /* Transfer data */
    if(esddev_read_data(readbuff,(uintptr_t)(hndl->reg_base+SD_BUF0), (uint32_t)byte) != SD_OK)
    {
        _sd_set_err(hndl,SD_ERR_CPU_IF);
        goto ErrExit;
    }

    /* Wait All end interrupt */
    if(esddev_int_wait(SD_TIMEOUT_RESP) != SD_OK)
    {
        _sd_set_err(hndl,SD_ERR_HOST_TOE);
        goto ErrExit;
    }

    /* ---- Check errors ---- */
    if(hndl->int_info2&SD_INFO2_MASK_ERR)
    {
        _sd_check_info2_err(hndl);
        goto ErrExit;
    }

    _sd_clear_info(hndl,SD_INFO1_MASK_DATA_TRNS,SD_INFO2_MASK_ERR);    /* clear All end bit */

    /* Disable all interrupts */
    _sd_clear_int_mask(hndl,SD_INFO1_MASK_DATA_TRNS,SD_INFO2_MASK_BRE);

    return SD_OK;

ErrExit:
    sd_outp(hndl,SD_STOP,0x0001);    /* stop data transfer */

    _sd_clear_info(hndl,SD_INFO1_MASK_DATA_TRNS,SD_INFO2_MASK_ERR);    /* clear All end bit */

    /* Disable all interrupts */
    _sd_clear_int_mask(hndl,SD_INFO1_MASK_DATA_TRNS,SD_INFO2_MASK_BRE);

    {
        uint16_t info1;
        /* Countermeasures when the access end is 1 */
        info1 = (uint16_t)sd_inp(hndl,SD_INFO1);
        info1 &= 0xFFFB;
        sd_outp(hndl,SD_INFO1,(uint16_t)info1);
    }
    
    return SD_ERR;
}
/**********************************************************************************************************************
 * End of function _sd_read_byte
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_calc_erase_sector
 * Description  : This function calculate erase sector for SD Phy Ver2.0..
 * Arguments    : SDHNDL *hndl :      SD handle
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 * Remark       : transfer type is PIO
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_calc_erase_sector(SDHNDL *hndl)
{
    uint16_t au;
    uint16_t erase_size;
    
    if((hndl->scr[0] & 0x0f00) == 0x0200)
    {
        /* AU is not defined,set to fixed value */
        hndl->erase_sect = SD_ERASE_SECTOR;

        /* Get AU size */
        au = hndl->sdstatus[5] >> 12;

        if((au > 0) && (au < 0x0a))
        {
            /* Get AU_SIZE(sectors) */
            hndl->erase_sect = ((8*1024)/512) << au;

            /* Get ERASE_SIZE */ 
            erase_size = (hndl->sdstatus[5] << 8) | (hndl->sdstatus[6] >> 8);
            if(erase_size != 0)
            {
                hndl->erase_sect *= erase_size;
            }
        }
        
    }
    else
    {
        ; /* If card is not Ver2.0,it use ERASE_BLK_LEN in CSD */
    }
    return SD_OK;
}
/**********************************************************************************************************************
 * End of function _sd_calc_erase_sector
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_card_query_partitions
 * Description  : Issue CMD45 and get QUERY_PARTITIONS information
 *              : If you can get it, it will be saved in the area of rw_buff.
 * Arguments    : SDHNDL *hndl             : SD handle
 *              : int32_t sub              : Sub Command code (0xA1 can be set)
 *              : uint8_t *rw_buff         : 512byte area
 * Return Value : SD_OK : end of succeed
 * Call functions : _sd_read_byte, _sd_card_send_cmd_arg
 * Note         : rw_buff needs 512Byte area
 *********************************************************************************************************************/
SD_DRV_CODE_SEC static int32_t _sd_card_query_partitions(SDHNDL *hndl, int32_t sub, uint8_t *rw_buff)
{
    /* ---- Get QUERY PARTITIONS (issue CMD45) ---- */
    if(_sd_read_byte(hndl,CMD45,(uint16_t)(sub<<8),0,rw_buff,SD_QUERY_PARTITION_LIST_BYTE) == SD_OK)
    {
        /* If CMD45(eSD) is supported */
        _sd_card_send_cmd_arg(hndl,CMD13,SD_RESP_R1,hndl->rca[0],0x0000);
        /* returns the value of CMD13 */
    }
    else
    {
        /* CMD45(eSD) isn't supported */
        /* Since next command responds with an error, issue CMD13 to clear the error */
        int32_t tmp = hndl->error;
        _sd_card_send_cmd_arg(hndl,CMD13,SD_RESP_R1,hndl->rca[0],0x0000);
        hndl->error = tmp;
        /* I want to return the error value at the time of _sd_read_byte() execution,
         * so temporarily evacuate so that CMD13 is not overwritten */
    }
    return hndl->error;
}
/**********************************************************************************************************************
 * End of function _sd_card_query_partitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_card_select_partition
 * Description  : SELECT PARTITIONS information [issue CMD43].
 * Arguments    : SDHNDL *hndl : SD handle
 *              : int32_t id   : Partition ID
 * Return Value : SD_OK               : SELECT_PARTITION success
 *              : SD_ERR_RES_TOE      : SELECT_PARTITION not supported
 *              : SD_ERR_OUT_OF_RANGE : SELECT_PARTITION supported but the specified partition does not exist
 * Call functions : _sd_card_send_cmd_arg, 
 * Remark       : Summary of behavior for #SELECT_PARTITION(CMD43)
 *              : If eSDv2.10(Made by SanDisk eSD) is supported 
 *              :  - Corresponding device is forcibly terminated regardless of partition
 *              :  - If it can be changed to the specified partition, next command(CMD13) ends with SD_OK
 *              :  - Next command(CMD13) returns an OUT_OF_RANGE error if the specified partition does not exist
 *              : 
 *              : If eSDv2.10(Made by Tosiba eSD/marketing SDSC/SDHC etc)isn't supported
 *              :  - If device is not supported, CMD45 ends with NO_RESPONSE
 *              :  - Since next command responds with an error, issue CMD13 to clear the error
 *              :  - I want to return the error value at the time of CMD43 execution,
 *              :    so temporarily evacuate so that CMD13 is not overwritten
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_card_select_partition(SDHNDL *hndl, int32_t id)
{
    /* ==== SELECT PARTITIONS(Physical partition #id) ==== */
    if(_sd_card_send_cmd_arg(hndl,CMD43,SD_RESP_R1b,(uint16_t)(id<<8),0x0000) == SD_OK)
    {
        /* If eSDv2.10(Made by SanDisk eSD) is supported */
        _sd_card_send_cmd_arg(hndl,CMD13,SD_RESP_R1,hndl->rca[0],0x0000);
        /* Returns the value of CMD13 */
    }
    else
    {
        /* If eSDv2.10(Made by Toshiba eSD/marketing SDSC/SDHC etc) isn't supported */
        /* Since next command responds with an error, issue CMD13 to clear the error */
        int32_t tmp = hndl->error;
        _sd_card_send_cmd_arg(hndl,CMD13,SD_RESP_R1,hndl->rca[0],0x0000);
        hndl->error = tmp;
        /* I want to return the error value at the time of CMD43 execution,
         * so temporarily evacuate so that CMD13 is not overwritten */
    }
    return hndl->error;
}
/**********************************************************************************************************************
 * End of function _sd_card_select_partition
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _esd_get_partition_id
 * Description  : Issue CMD45 to device.
 *              : If failed.
 *              : Terminates with an error and sets nothing to the argument ID.
 * Arguments    : SDHNDL *hndl : SD handle
 *              : int32_t *id  : Partition ID
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR : end of error
 * Call functions : _sd_get_hndl, _sd_set_clock, _sd_card_query_partitions
 * Remark       : If CMD45 is issued to v2.10 non-compatible device ,
 *              : the subsequent SINGLE/MULTI_READ will result in an error(SD_ERR_ILL_READ).
 *              : Therefore, it does not issue CMD45 to non-compliant devices.
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _esd_get_partition_id(SDHNDL *hndl, int32_t *id)
{

    /* Issue the QUERY_PARTITION(CMD45) command  */
    if(_sd_card_query_partitions(hndl, 0xA1, hndl->rw_buff) == SD_OK)
    {
        /* Stores the currently selected the physical partition number at the 511 byte location */
        *id = hndl->rw_buff[511];
    }

    return hndl->error;
}
/**********************************************************************************************************************
 * End of function _esd_get_partition_id
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: esd_get_partition_id
 * Description  : Get the currently selected partitionID from internal variables.
 * Arguments    : int32_t *id       : Partition ID
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR : not initialized
 * Call functions : _sd_get_hndl
 * Remark       : Get directly from device when partition is selected
 *              : Therefore,it does not issue CMD45
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t esd_get_partition_id(int32_t *id)
{
    SDHNDL    *p_hndl;

    p_hndl = _sd_get_hndl();
    if(p_hndl == 0)
    {
        return SD_ERR;    /* Not initialized */
    }
    if(id != 0)
    {
        *id = p_hndl->partition_id;
    }
    return SD_OK;
}
/**********************************************************************************************************************
 * End of function esd_get_partition_id
 *********************************************************************************************************************/

/* End of File */
