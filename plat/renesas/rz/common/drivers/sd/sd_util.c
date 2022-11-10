/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**********************************************************************************************************************
 * File Name    : sd_util.c
 * Version      : 1.0
 * Description  : Function setting.
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
 * Function Name: _sd_set_clock
 * Description  : supply or halt SD clock
 *              : if enable is SD_CLOCK_ENABLE, supply SD clock
 *              : if enable is SD_CLOCK_DISKABLE, halt SD clock.
 * Arguments    : SDHNDL *hndl : SD handle
 *              : int32_t clock : SD clock frequency
 *              : int32_t enable : supply or halt SD clock.
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error.
 *********************************************************************************************************************/
int32_t _sd_set_clock(SDHNDL *hndl, int32_t clock, int32_t enable)
{
    uint32_t div;

    if (enable == SD_CLOCK_ENABLE)
    {
        /* Convert clock frequency to clock divide ratio */
        div = esddev_get_clockdiv(clock);

        if (div > SD_DIV_512)
        {
            _sd_set_err(hndl, SD_ERR_CPU_IF);
            return SD_ERR;
        }

        sd_outp(hndl, SD_CLK_CTRL, (uint16_t )(div | 0x0100u));
    }
    else
    {
        if ((sd_inp(hndl,SD_INFO2) & SD_INFO2_MASK_CBSY) == SD_INFO2_MASK_CBSY)
        {
            /* For internal error detail    */
            hndl->error = SD_ERR_CBSY_ERROR;

        }
        sd_outp(hndl, SD_CLK_CTRL, 0); /* Halt */

    }
    return SD_OK;
}
/**********************************************************************************************************************
 * End of function _sd_set_clock
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_set_port
 * Description  : change data bus width
 *              : if port is SD_PORT_SERIAL, set data bus width 1bit
 *              : if port is SD_PORT_PARALEL, set data bus width 4bits
 *              : change between 1bit and 4bits by ACMD6.
 * Arguments    : SDHNDL *hndl : SD handle
 *              : int32_t port : setting bus with.
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error.
 * Remark       : before execute this function, check card supporting bus width
 *              : SD memory card is 4bits support mandatory
 *********************************************************************************************************************/
int32_t _sd_set_port(SDHNDL *hndl, int32_t port)
{
    uint16_t reg;
    uint16_t arg;

    if (hndl->media_type & SD_MEDIA_SD)
    { /* SD or COMBO */
        /* ---- Check card state ---- */
        if ((hndl->resp_status & RES_STATE) == STATE_TRAN)
        { /* transfer state */
            if (port == SD_PORT_SERIAL)
            {
                arg = ARG_ACMD6_1bit;
            }
            else
            {
                arg = ARG_ACMD6_4bit;
            }
            /* ==== Change card bus width (issue ACMD6) ==== */
            if (_sd_send_acmd(hndl, ACMD6, 0, arg) != SD_OK)
            {
                return SD_ERR;
            }
            if (_sd_get_resp(hndl, SD_RESP_R1) != SD_OK)
            {
                return SD_ERR;
            }
        }
    }

    /* ==== Change SDHI bus width ==== */
    if (port == SD_PORT_SERIAL)
    { /* 1bit */
        esddev_set_port(port);
        reg = sd_inp(hndl, SD_OPTION);
        reg |= 0x8000u;
        sd_outp(hndl, SD_OPTION, reg);
    }
    else
    { /* 4bits */
        reg = sd_inp(hndl, SD_OPTION);
        reg &= 0x7fffu;
        sd_outp(hndl, SD_OPTION, reg);
        esddev_set_port(port);
    }

    hndl->if_mode = (uint8_t) port;

    return SD_OK;
}
/**********************************************************************************************************************
 * End of function _sd_set_port
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_iswp
 * Description  : check hardware write protect refer to SDHI register
 *              : if WP pin is disconnected to SDHI, return value has no meaning.
 * Arguments    : SDHNDL *hndl : SD handle
 * Return Value : SD_WP_OFF (0): write protected
 *              : SD_WP_HW (1): not write protected
 * Remark       : don't check CSD write protect bits and ROM card
 *********************************************************************************************************************/
int32_t _sd_iswp(SDHNDL *hndl)
{
    int32_t wp;

    if (hndl->port == 0)
    {
        /* ===== Check SD_INFO1 WP bit ==== */
        wp = ( (~sd_inp(hndl, SD_INFO1)) & 0x0080u) >> 7;
    }
    else
    {
        wp = ~(sd_inp(hndl, EXT_WP));
        wp >>= (hndl->port - 1); /* Bit sift to b0 (sift 2,1,0bit)*/
        wp &= 0x0001;
    }

    return wp;
}
/**********************************************************************************************************************
 * End of function _sd_iswp
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_bit_search
 * Description  : check every bits of argument (data) from LSB
 *              : return first bit whose value is 1'b
 *              : bit number is big endian (MSB is 0).
 * Arguments    : uint16_t data : checked data
 * Return Value : not less than 0 : bit number has 1'b
 *              : -1 : no bit has 1'b
 * Remark       : just 16bits value can be applied
 *********************************************************************************************************************/
int32_t _sd_bit_search(uint16_t data)
{
    int32_t i;

    for (i = 15; i >= 0; i--)
    {
        if (data & 1u)
        {
            return i;
        }
        data >>= 1;
    }

    return -1;
}
/**********************************************************************************************************************
 * End of function _sd_bit_search
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_set_err
 * Description  : set error information (=error) to SD Handle member (=hndl->error).
 * Arguments    : SDHNDL *hndl : SD handle
 *              : int32_t error : setting error information
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 * Remark       : if hndl->error was already set, no overwrite it
 *********************************************************************************************************************/
int32_t _sd_set_err(SDHNDL *hndl, int32_t error)
{
    if (hndl->error == SD_OK)
    {
        hndl->error = error;
    }

    return SD_OK;
}
/**********************************************************************************************************************
 * End of function _sd_set_err
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: esd_get_type
 * Description  : get mounting card type, current and supported speed mode
 *              : and capacity type
 *              : (if SD memory card)
 *              : following card types are defined
 *              : SD_MEDIA_UNKNOWN : unknown media
 *              : SD_MEDIA_MMC : MMC card
 *              : SD_MEDIA_SD : SD Memory card
 *              : SD_MEDIA_IO : SD IO card (IO spec ver1.10)
 *              : SD_MEDIA_COMBO : SD COMBO card (IO spec ver1.10)
 * Arguments    : uint16_t *type : mounting card type
 *              : uint8_t *speed : speed mode
 *              :   supported speed is bit4, current speed is bit0
 *              : uint8_t *speed : speed mode
 *              :   Standard capacity:0, High capacity:1
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 * Remark       : if pointer has NULL ,the value isn't returned
 *              : only SD memory card, speed mode has meaning
 *********************************************************************************************************************/
int32_t esd_get_type(uint16_t *type, uint8_t *speed, uint8_t *capa)
{
    SDHNDL *p_hndl;

    p_hndl = _sd_get_hndl();
    if (p_hndl == 0)
    {
        return SD_ERR; /* Not initialized */
    }

    if (type)
    {
        *type = (uint16_t) p_hndl->media_type;
    }
    else
    {
        return SD_ERR;
    }
    if (p_hndl->partition_id > 0)
    {
        (*type) |= SD_MEDIA_EMBEDDED;
    }
    if (speed)
    {
        *speed = p_hndl->speed_mode;
    }
    if (capa)
    {
        *capa = p_hndl->csd_structure;
    }
    return SD_OK;
}
/**********************************************************************************************************************
 * End of function esd_get_type
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_get_size
 * Description  : get memory card size
 * Arguments    : SDHNDL *hndl : SD handle
 *              : uint32_t area : memory area (bit0:user area, bit1:protect area)
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 * Remark       : protect area is just the number of all sectors
 *********************************************************************************************************************/
int32_t _sd_get_size(SDHNDL *hndl, uint32_t area)
{
    uint32_t c_mult;
    uint32_t c_size;
    uint32_t read_bl_len;

    /* ---- READ BL LEN ---- */
    read_bl_len = (hndl->csd[3] & 0x0f00u) >> 8;

    /* ---- C_SIZE_MULT ---- */
    c_mult = ((hndl->csd[5] & 0x0380u) >> 7);

    if (area & SD_PROT_AREA)
    {
        /* Calculate the number of all sectors */

        if ((hndl->sup_ver == SD_MODE_VER2X) && (hndl->csd_structure == 0x01))
        {
            hndl->prot_sector_size = (((uint32_t) hndl->sdstatus[2] << 16u)
                    | ((uint32_t) hndl->sdstatus[3])) / 512;
        }
        else
        {
            hndl->prot_sector_size = ((hndl->sdstatus[3] * ((uint32_t) 1 << (c_mult + 2)))
                    * ((uint32_t) 1 << read_bl_len)) / 512;
        }

    }

    if (area & SD_USER_AREA)
    {
        if ((hndl->sup_ver == SD_MODE_VER2X) && (hndl->csd_structure == 0x01))
        {

            c_size = ((((uint32_t) hndl->csd[4] & 0x3fffu) << 8u) |
                    (((uint32_t) hndl->csd[5] & 0xff00u) >> 8u));
            /* Memory capacity = C_SIZE*512K byte */
            /* Sector_size = memory capacity/512 */
            hndl->card_sector_size = ((c_size + 1) << 10u);

        }
        else
        {
            /* ---- C_SIZE ---- */
            c_size = ((hndl->csd[3] & 0x0003u) << 10)
                    | ((hndl->csd[4] & 0xffc0u) >> 6);

            /* Calculate the number of all sectors */
            hndl->card_sector_size = ((uint32_t) (c_size + 1) *
                    ((uint32_t) 1 << (c_mult + 2)) * ((uint32_t) 1
                            << read_bl_len)) / 512;
        }
    }

    return SD_OK;
}
/**********************************************************************************************************************
 * End of function _sd_get_size
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: esd_set_buffer
 * Description  : initialize SD driver work buffer
 *              : this buffer is used for mainly MKB process
 * Arguments    : void *buff : work buffer address
 *              : uint32_t size : work buffer size
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 * Remark       : if applied to CPRM, allocating more than 8K bytes
 *********************************************************************************************************************/
int32_t esd_set_buffer(void *buff, uint32_t size)
{
    SDHNDL *p_hndl;

    /* Check buffer boundary (quadlet unit) */
    if (((uintptr_t) buff & 0x00000003u) != 0)
    {
        return SD_ERR;
    }

    p_hndl = _sd_get_hndl();
    if (p_hndl == 0)
    {
        return SD_ERR; /* not initialized */
    }

    /* Initialize buffer area */
    p_hndl->rw_buff = (uint8_t*) buff;

    /* Initialize buffer size */
    p_hndl->buff_size = size;

    return SD_OK;
}
/**********************************************************************************************************************
 * End of function esd_set_buffer
 *********************************************************************************************************************/

/* End of File */
