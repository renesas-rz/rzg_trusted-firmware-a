/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**********************************************************************************************************************
 * File Name    : sd_cmd.c
 * Version      : 1.0
 * Description  : command issue, response receive and register check.
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
/* ==== Response errors ==== */
/* For internal error detail    */
static const int32_t RespErrTbl[]={
    SD_ERR_OUT_OF_RANGE,
    SD_ERR_ADDRESS_ERROR,
    SD_ERR_BLOCK_LEN_ERROR,
    SD_ERR_CARD_ERASE,
    SD_ERR_CARD_ERASE,
    SD_ERR_WP,
    SD_ERR_CARD_LOCK,
    SD_ERR_CARD_UNLOCK,
    SD_ERR_HOST_CRC,
    SD_ERR_ILLEGAL_COMMAND,
    SD_ERR_CARD_ECC,
    SD_ERR_CARD_CC,
    SD_ERR_CARD_ERROR,
    SD_ERR_RESERVED_ERROR18,
    SD_ERR_RESERVED_ERROR17,
    SD_ERR_OVERWRITE,
};

/* ==== IO errors ==== */
static const int32_t IOErrTbl[]={
    SD_ERR_INTERNAL,
    SD_ERR_INTERNAL,
    SD_ERR_INTERNAL,
    SD_ERR_INTERNAL,
    SD_ERR_INTERNAL,
    SD_ERR_INTERNAL,
    SD_ERR_INTERNAL,
    SD_ERR_INTERNAL,
    SD_ERR_HOST_CRC,
    SD_ERR_INTERNAL,
    SD_ERR_INTERNAL,
    SD_ERR_INTERNAL,
    SD_ERR_CARD_ERROR,
    SD_ERR_INTERNAL,
    SD_ERR_ILL_FUNC,
    SD_ERR_INTERNAL,
};

/* ==== SD_INFO2 errors table ==== */


        /* For internal error detail    */
static const int32_t Info2ErrTbl[]={
    SD_ERR_RES_TOE,        /* SD_INFO2_MASK_ERR6    0x0040 */
    SD_ERR_ILL_READ,    /* SD_INFO2_MASK_ERR5    0x0020 */
    SD_ERR_ILL_WRITE,    /* SD_INFO2_MASK_ERR4    0x0010 */
    SD_ERR_CARD_TOE,    /* SD_INFO2_MASK_ERR3    0x0008 */
    SD_ERR_END_BIT,        /* SD_INFO2_MASK_ERR2    0x0004 */
    SD_ERR_CRC,            /* SD_INFO2_MASK_ERR1    0x0002 */
    SD_ERR_CMD_ERROR,    /* SD_INFO2_MASK_ERR0    0x0001 */
};


/* ==== Transfer speed table ==== */
static const uint16_t TranSpeed[8]={
    1,        /* 100kbit/s */
    10,        /* 1Mbit/s   */
    100,    /* 10Mbit/s  */
    1000,    /* 100Mbit/s */
    1000,    /* reserved  */
    1000,    /* reserved  */
    1000,    /* reserved  */
    1000,    /* reserved  */
};
static const uint8_t TimeValue[16]={
    0,10,12,13,15,20,25,30,35,40,45,50,55,60,70,80
};

SD_DRV_CODE_SEC static void _sd_get_info2(SDHNDL *hndl);

/**********************************************************************************************************************
 * Function Name: _sd_send_cmd
 * Description  : issue SD command, hearafter wait recive response.
 * Arguments    : SDHNDL *hndl : SD handle
 *              : uint16_t cmd : command code
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 * Remark       : not get response and check response errors
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_send_cmd(SDHNDL *hndl, uint16_t cmd)
{
    int32_t time;
    
    hndl->error = SD_OK;

    if(cmd == CMD38)
    {    /* Erase command */
        time = SD_TIMEOUT_ERASE_CMD;    /* Extend timeout 1 sec */
    }
    else if(cmd == ACMD46)
    {    /* ACMD46 */
        time = SD_TIMEOUT_MULTIPLE;    /* Same as write timeout */
    }
    /* @090106 R1b command lengthens timeout time*/
    else if(cmd == CMD7)
    {
        time = SD_TIMEOUT_RESP;    /* Same as write timeout */
    }
    else if(cmd == CMD12)
    {
        time = SD_TIMEOUT_RESP;    /* Same as write timeout */
    }

    /* eSD Addendum Version 2.10 to Part 1 Physical Layer Specifications  */
    /* Refer "2.4.6.1 SELECT_PARTITION (CMD43)" */
    /* Adjusted to the physical partition area that is not FAST_BOOT attribute */
    else if(cmd == CMD43)
    {
        time = SD_TIMEOUT_RESP;
    }
    else if(cmd == CMD45)
    {
        time = SD_TIMEOUT_RESP;
    }
    else
    {
        time = SD_TIMEOUT_CMD;
    }
    
    /* Enable resp end and illegal access interrupts */
    _sd_set_int_mask(hndl,SD_INFO1_MASK_RESP,0);

    /* ---- Issue command ---- */

    sd_outp32(hndl,SD_CMD,cmd);
    
    /* ---- Wait resp end ---- */
    if(esddev_int_wait(time) != SD_OK)
    {
        _sd_set_err(hndl,SD_ERR_HOST_TOE);
        _sd_clear_int_mask(hndl,SD_INFO1_MASK_RESP,SD_INFO2_MASK_ILA);
        return hndl->error;
    }

    /* Disable resp end and illegal access interrupts */
    _sd_clear_int_mask(hndl,SD_INFO1_MASK_RESP,SD_INFO2_MASK_ILA);

    _sd_get_info2(hndl);    /* Get SD_INFO2 register */

    _sd_check_info2_err(hndl);    /* Check SD_INFO2 error bits */

    if(!(hndl->int_info1 & SD_INFO1_MASK_RESP))
    {
        /* For internal error detail    */
        _sd_set_err(hndl,SD_ERR_NO_RESP_ERROR);        /* No response */
    }

    /* ---- Clear previous errors ---- */
    _sd_clear_info(hndl,SD_INFO1_MASK_RESP,SD_INFO2_MASK_ERR);

    return hndl->error;
}
/**********************************************************************************************************************
 * End of function _sd_send_cmd
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_send_acmd
 * Description  : issue application specific command, hearafter wait recive response
 *              : issue CMD55 preceide application specific command
 * Arguments    : SDHNDL *hndl : SD handle
 *              : uint16_t cmd : command code
 *              : uint16_t h_arg : command argument high [31:16]
 *              : uint16_t l_arg : command argument low [15:0]
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_send_acmd(SDHNDL *hndl, uint16_t cmd, uint16_t h_arg,
        uint16_t l_arg)
{
    /* ---- Issue CMD 55 ---- */
    _sd_set_arg(hndl,hndl->rca[0],0);
    if(_sd_send_cmd(hndl, CMD55) != SD_OK)
    {
        return SD_ERR;
    }
    
    if(_sd_get_resp(hndl,SD_RESP_R1) != SD_OK)
    {
        return SD_ERR;
    }
    
    /* ---- Issue ACMD ---- */
    _sd_set_arg(hndl,h_arg,l_arg);
    if(_sd_send_cmd(hndl, cmd) != SD_OK)
    {
        return SD_ERR;
    }
    
    return SD_OK;
}
/**********************************************************************************************************************
 * End of function _sd_send_acmd
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_send_mcmd
 * Description  : issue multiple command (CMD18 or CMD25)
 *              : wait response
 *              : set read start address to startaddr
 *              : after this function finished, start data transfer
 * Arguments    : SDHNDL *hndl : SD handle
 *              : uint16_t cmd : command code (CMD18 or CMD25)
 *              : uint32_t startaddr : data address (command argument)
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_send_mcmd(SDHNDL *hndl, uint16_t cmd, uint32_t startaddr)
{
    _sd_set_arg(hndl,(uint16_t)(startaddr>>16),(uint16_t)startaddr);
    /* ---- Issue command ---- */
    sd_outp32(hndl,SD_CMD,cmd);

    /* ---- Wait resp end ---- */
    if(esddev_int_wait(SD_TIMEOUT_CMD) != SD_OK)
    {
        _sd_set_err(hndl,SD_ERR_HOST_TOE);
        return hndl->error;
    }

    _sd_get_info2(hndl);    /* Get SD_INFO2 register */

    _sd_check_info2_err(hndl);    /* Check SD_INFO2 error bits */

    if(hndl->int_info1 & SD_INFO1_MASK_RESP)
    {
        if(!hndl->error)
        {
            _sd_get_resp(hndl,SD_RESP_R1);    /* Check R1 resp */
        }
    }
    else
    {
        /* For internal error detail    */
        _sd_set_err(hndl,SD_ERR_NO_RESP_ERROR);        /* No response */
    }

    /* ---- Clear previous errors ---- */
    _sd_clear_info(hndl,SD_INFO1_MASK_RESP,SD_INFO2_MASK_ERR);

    return hndl->error;
}
/**********************************************************************************************************************
 * End of function _sd_send_mcmd
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_card_send_cmd_arg
 * Description  : issue command specified cmd code
 *              : get and check response
 * Arguments    : SDHNDL *hndl : SD handle
 *              : uint16_t cmd : command code (CMD18 or CMD25)
 *              : int32_t  resp : command response
 *              : uint16_t h_arg : command argument high [31:16]
 *              : uint16_t l_arg : command argument low [15:0]
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 * Remark       : if argument value is zero,
 *              : use macro function, that is _sd_card_send_cmd()
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_card_send_cmd_arg(SDHNDL *hndl, uint16_t cmd, int32_t resp,
        uint16_t h_arg, uint16_t l_arg)
{
    int32_t ret;

    _sd_set_arg(hndl,h_arg,l_arg);
    
    /* ---- Issue command ---- */
    if((ret =_sd_send_cmd(hndl,cmd)) == SD_OK)
    {
        ret = _sd_get_resp(hndl,resp);    /* get and check response */
    }
    return ret;
}
/**********************************************************************************************************************
 * End of function _sd_card_send_cmd_arg
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_set_arg
 * Description  : set command argument to SDHI
 *              : h_arg means higher 16bits [31:16] and  set SD_ARG0
 *              : l_arg means lower 16bits [15:0] and set SD_ARG1
 * Arguments    : SDHNDL *hndl : SD handle
 *              : uint16_t h_arg : command argument high [31:16]
 *              : uint16_t l_arg : command argument low [15:0]
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 * Remark       : SD_ARG0 and SD_ARG1 are like little endian order
 *********************************************************************************************************************/
SD_DRV_CODE_SEC void _sd_set_arg(SDHNDL *hndl, uint16_t h_arg, uint16_t l_arg)
{
    sd_outp(hndl,SD_ARG0,l_arg);
    sd_outp(hndl,SD_ARG1,h_arg);
}
/**********************************************************************************************************************
 * End of function _sd_set_arg
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_card_send_ocr
 * Description  : get OCR register and check card operation voltage
 *              : if type is SD_MEDIA_SD, issue ACMD41
 *              : if type is SD_MEDIA_MMC, issue CMD1
 *              : if type is SD_MEDIA_IO, issue CMD5
 * Arguments    : SDHNDL *hndl : SD handle
 *              : int32_t type : card type
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_card_send_ocr(SDHNDL *hndl, int32_t type)
{
    int32_t ret;
    int32_t i;
    int32_t j=0;

    /* ===== Distinguish card type issuing CMD5, ACMD41 or CMD1 ==== */
    for(i=0; i < 200; i++)
    {
        switch(type)
        {
            case SD_MEDIA_UNKNOWN:    /* Unknown media (read OCR) */
                /* ---- Issue CMD5 ---- */
                _sd_set_arg(hndl,0,0);
                ret = _sd_send_cmd(hndl,CMD5);
                if(ret == SD_OK)
                {
                    return _sd_get_resp(hndl,SD_RESP_R4);    /* check R4 resp */
                }
                else
                {
                    return ret;
                }

            case SD_MEDIA_IO:
                /* ---- Issue CMD5 ---- */
                _sd_set_arg(hndl,(uint16_t)(hndl->voltage>>16),
                    (uint16_t)hndl->voltage);
                ret = _sd_send_cmd(hndl,CMD5);
                break;
        
            case SD_MEDIA_SD:
            case SD_MEDIA_COMBO:
                if(hndl->sup_ver == SD_MODE_VER2X)
                {
                    /* Set HCS bit */
                    hndl->voltage |= 0x40000000;
                }
                /* ---- Issue ACMD41 ---- */
                ret = _sd_send_acmd(hndl,ACMD41,
                    (uint16_t)(hndl->voltage>>16),
                    (uint16_t)hndl->voltage);
                break;
        
            case SD_MEDIA_MMC:    /* MMC */
                /* ---- Issue CMD1 ---- */
                _sd_set_arg(hndl,(uint16_t)(hndl->voltage>>16),
                    (uint16_t)hndl->voltage);
                ret = _sd_send_cmd(hndl,CMD1);
                break;

            default:
                hndl->resp_status = 0;
                /* For internal error detail    */
                /* But not need to change        */
                hndl->error = SD_ERR_INTERNAL;
                return SD_ERR;
        }
        
        if(ret == SD_OK)
        {
            if(type == SD_MEDIA_IO)
            {    /* IO */
                _sd_get_resp(hndl,SD_RESP_R4);    /* Check R4 resp */
                /* ---- Polling busy bit ---- */
                if(hndl->io_ocr[0] & 0x8000)
                {    /* Busy cleared */
                    break;
                }
                else
                {
                    ret = SD_ERR;    /* Busy */
                    esddev_int_wait(5);    /* Add wait function because retry interval is too short */
                }
            }
            else
            {    /* Memory */
                _sd_get_resp(hndl,SD_RESP_R3);    /* Check R3 resp */
                /* ---- Polling busy bit ---- */
                if(hndl->ocr[0] & 0x8000)
                {    /* Busy cleared */
                    break;
                }
                else
                {
                    ret = SD_ERR;    /* Busy */
                    esddev_int_wait(5);    /* Add wait function because retry interval is too short */
                }
            }
        }
        
        /* If more than 3 times response timeout occured, retry stop quick distinction to MMC */ 
        if(hndl->error == SD_ERR_RES_TOE)
        {
            ++j;
            if( j == 3)
            {
                break;
            }
        }
        else
        {
            j = 0;
        } 
    }
    return ret;
}
/**********************************************************************************************************************
 * End of function _sd_card_send_ocr
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_check_resp_error
 * Description  : distinguish error bit from R1 response
 *              : set the error bit to hndl->error
 * Arguments    : SDHNDL *hndl : SD handle
 * Return Value : SD_OK : no error detected
 *              : SD_ERR: any errors detected
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_check_resp_error(SDHNDL *hndl)
{
    uint16_t status;
    int32_t bit;
    

    if(hndl->media_type == SD_MEDIA_IO)
    {
        /* IO */
        status = (uint16_t)(hndl->resp_status & 0xfff9u);

        /* Serch R5 error bit */
        bit = _sd_bit_search(status);
        
        /* R5 resp errors bits */
        _sd_set_err(hndl,IOErrTbl[bit]);
    }
    else
    {
        /* SD or MMC card */
        status = (uint16_t)((hndl->resp_status >>16 ) & 0xfff9u);
        
        /* ---- Search R1 error bit ---- */
        bit = _sd_bit_search(status);
        
        if(bit != (-1))
        {
            /* R1 resp errors bits but for AKE_SEQ_ERROR */
            _sd_set_err(hndl,RespErrTbl[bit]);
            return SD_ERR;
        }
        else if(hndl->resp_status & RES_AKE_SEQ_ERROR)
        {
            /* Authentication process sequence error */
            _sd_set_err(hndl,SD_ERR_AKE_SEQ);
            return SD_ERR;
        }
        else
        {
            /* No operation */
        }
    }
    
    return SD_OK;
}
/**********************************************************************************************************************
 * End of function _sd_check_resp_error
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_get_resp
 * Description  : get response value from RESP register
 *              : R1, R2, R3,(R4, R5) and R6 types are available
 *              : specify response type by the argument resp
 *              : set response value to SD handle member
 * Arguments    : SDHNDL *hndl : SD handle
 *              : int32_t resp : response type
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_get_resp(SDHNDL *hndl, int32_t resp)
{
    uint32_t status;
    uint16_t *p_ptr;
    
    /* Select RESP register depend on the response type */
    switch(resp)
    {
        case SD_RESP_NON:    /* No response */
            /* NOP */
            break;
        case SD_RESP_R1:    /* Nomal response (32bits length) */
        case SD_RESP_R1b:    /* Nomal response with an optional busy signal */
            status = sd_inp(hndl,SD_RESP1);
            status <<=16;
            status |= sd_inp(hndl,SD_RESP0);
            hndl->resp_status = status;
        
            if(status & 0xffffe008)
            {
                /* Any status error */
                return _sd_check_resp_error(hndl);
            }
            break;
        
        case SD_RESP_R1_SCR:    /* Nomal response with an optional busy signal */
            hndl->scr[0] = sd_inp(hndl,SD_RESP1);
            hndl->scr[1] = sd_inp(hndl,SD_RESP0);
            break;
        
        case SD_RESP_R2_CID:    /* CID register (128bits length) */
            p_ptr = hndl->cid;
            *p_ptr++ = sd_inp(hndl,SD_RESP7);
            *p_ptr++ = sd_inp(hndl,SD_RESP6);
            *p_ptr++ = sd_inp(hndl,SD_RESP5);
            *p_ptr++ = sd_inp(hndl,SD_RESP4);
            *p_ptr++ = sd_inp(hndl,SD_RESP3);
            *p_ptr++ = sd_inp(hndl,SD_RESP2);
            *p_ptr++ = sd_inp(hndl,SD_RESP1);
            *p_ptr++ = sd_inp(hndl,SD_RESP0);
            break;
        
        case SD_RESP_R2_CSD:    /* CSD register (128bits length) */
            p_ptr = hndl->csd;
            *p_ptr++ = sd_inp(hndl,SD_RESP7);
            *p_ptr++ = sd_inp(hndl,SD_RESP6);
            *p_ptr++ = sd_inp(hndl,SD_RESP5);
            *p_ptr++ = sd_inp(hndl,SD_RESP4);
            *p_ptr++ = sd_inp(hndl,SD_RESP3);
            *p_ptr++ = sd_inp(hndl,SD_RESP2);
            *p_ptr++ = sd_inp(hndl,SD_RESP1);
            *p_ptr++ = sd_inp(hndl,SD_RESP0);
            break;
        
        case SD_RESP_R3:    /* OCR register (32bits length) */
            hndl->ocr[0] = sd_inp(hndl,SD_RESP1);
            hndl->ocr[1] = sd_inp(hndl,SD_RESP0);
            break;

        case SD_RESP_R4:    /* IO OCR register (24bits length) */
            hndl->io_ocr[0] = sd_inp(hndl,SD_RESP1);
            hndl->io_ocr[1] = sd_inp(hndl,SD_RESP0);
            break;
        
        case SD_RESP_R6:        /* Published RCA response (32bits length) */
            hndl->rca[0] = sd_inp(hndl,SD_RESP1);
            hndl->rca[1] = sd_inp(hndl,SD_RESP0);
            break;
        
        case SD_RESP_R5:        /* IO RW response */
            status = sd_inp(hndl,SD_RESP1);
            status <<=16;
            status |= sd_inp(hndl,SD_RESP0);
            hndl->resp_status = status;
        
            if(status & 0xcb00)
            {
                /* Any status error */
                return _sd_check_resp_error(hndl);
            }
            break;

        /* @050805 for Ver2.0 */
        case SD_RESP_R7:        /* IF_COND response */
            hndl->if_cond[0] = sd_inp(hndl,SD_RESP1);
            hndl->if_cond[1] = sd_inp(hndl,SD_RESP0);
            break;
    
        default:
            /* Unknown type */
            hndl->resp_status = 0;
            /* For internal error detail    */
            /* But not need to change        */
            hndl->error = SD_ERR_INTERNAL;
            return SD_ERR;
    }
    
    return SD_OK;
}
/**********************************************************************************************************************
 * End of function _sd_get_resp
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_check_csd
 * Description  : check CSD register and get following information
 *              : Transfer Speed
 *              : Command Class
 *              : Read Block Length
 *              : Copy Bit
 *              : Write Protect Bit
 *              : File Format Group
 *              : Number of Erase Sector
 * Arguments    : SDHNDL *hndl : SD handle
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_check_csd(SDHNDL *hndl)
{
    uint32_t transpeed;
    uint32_t timevalue;
    uint32_t erase_sector_size;
    uint32_t erase_group_size;

    /* ---- CSD Structure ---- */    /* For Ver2.0 */

    if(hndl->media_type == SD_MEDIA_MMC)
    {
        hndl->csd_structure = 0;
    }
    else
    {
        hndl->csd_structure = (uint8_t)((hndl->csd[0] & 0x00c0u) >> 6u);
    }
    
    /* ---- TAAC/NSAC ---- */
    /* No check, to be obsolete */

    /* ---- TRAN_SPEED  ---- */
    transpeed = (hndl->csd[2] & 0x0700u) >> 8u;
    timevalue = (hndl->csd[2] & 0x7800u) >> 11u;
    
    transpeed = (uint32_t)(TranSpeed[transpeed] * TimeValue[timevalue]);
    
    /* ---- Set transfer speed (memory access) ---- */
    if(transpeed >= 5000)
    {
        hndl->csd_tran_speed = SD_CLK_50MHz;
    }
    else if(transpeed >= 2500)
    {
        hndl->csd_tran_speed = SD_CLK_25MHz;
    }
    else if(transpeed >= 2000)
    {
        hndl->csd_tran_speed = SD_CLK_20MHz;
    }
    else if(transpeed >= 1000)
    {
        hndl->csd_tran_speed = SD_CLK_10MHz;
    }
    else if(transpeed >= 500)
    {
        hndl->csd_tran_speed = SD_CLK_5MHz;
    }
    else if(transpeed >= 100)
    {
        hndl->csd_tran_speed = SD_CLK_1MHz;
    }
    else
    {
        hndl->csd_tran_speed = SD_CLK_400kHz;
    }
    
    /* ---- CCC  ---- */
    hndl->csd_ccc = (uint16_t)(((hndl->csd[2] & 0x00ffu) << 4u) |
        ((hndl->csd[3] & 0xf000u) >> 12u));
    
    
    /* ---- COPY ---- */
    hndl->csd_copy = (uint8_t)(hndl->csd[7] & 0x0040u);
    
    /* ---- PERM/TMP_WRITE_PROTECT ---- */
    hndl->write_protect |= (uint8_t)((hndl->csd[7] & 0x0030u)>>3u);
    
    /* ---- FILE_FORMAT ---- */
    hndl->csd_file_format = (uint8_t)(hndl->csd[7] & 0x008cu);
    if(hndl->csd_file_format & 0x80u)
    {
        _sd_set_err(hndl, SD_ERR_FILE_FORMAT);
        return SD_ERR;
    }
    
    /* ---- Calculate the number of erase sectors ---- */
    if(hndl->media_type & SD_MEDIA_SD)
    {
        erase_sector_size = ((hndl->csd[5] & 0x003fu) << 1u) | 
            ((hndl->csd[6] & 0x8000) >> 15);
        erase_group_size = (hndl->csd[6] & 0x7f00u) >> 8u; 
    }
    else
    {
        erase_sector_size = (hndl->csd[5] & 0x007cu) >> 2u;
        erase_group_size = ((hndl->csd[5] & 0x0003u) << 3u) | 
            ((hndl->csd[6] & 0xe000u) >> 13u); 
    }
    hndl->erase_sect = (erase_sector_size+1) * (erase_group_size+1);
    return SD_OK;
}
/**********************************************************************************************************************
 * End of function _sd_check_csd
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_check_info2_err
 * Description  : check error bit of SD_INFO2 register
 *              : set the error bit to hndl->error
 * Arguments    : SDHNDL *hndl : SD handle
 * Return Value : SD_OK : end of succeed
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_check_info2_err(SDHNDL *hndl)
{
    uint16_t info2;
    int32_t bit;
    
    info2 = (uint16_t)(hndl->int_info2 & SD_INFO2_MASK_ERR);
    
    /* ---- Search error bit ---- */
    bit = _sd_bit_search(info2);
    
    if(bit == (-1))
    {    /* No error */
        _sd_set_err(hndl,SD_OK);
    }
    else if(bit == 0)
    {    /* CMD error */
        /* For internal error detail    */
        _sd_set_err(hndl,SD_ERR_CMD_ERROR);
    }
    else
    {    /* Other error */
        _sd_set_err(hndl,Info2ErrTbl[bit-9]);
    }
    
    return SD_OK;
}
/**********************************************************************************************************************
 * End of function _sd_check_info2_err
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_get_info2
 * Description  : get SD_INFO2 register
 *              : set the register value to hndl->int_info2
 * Arguments    : SDHNDL *hndl : SD handle
 * Return Value : none.
 *********************************************************************************************************************/
SD_DRV_CODE_SEC static void _sd_get_info2(SDHNDL *hndl)
{
    uint16_t info2_reg;

    info2_reg = (uint16_t)((sd_inp(hndl,SD_INFO2) & SD_INFO2_MASK_ERR));
    sd_outp(hndl,SD_INFO2,(uint16_t)~info2_reg);
    hndl->int_info2 = (uint16_t)(hndl->int_info2 | info2_reg);
}
/**********************************************************************************************************************
 * End of function _sd_get_info2
 *********************************************************************************************************************/

/* End of File */
