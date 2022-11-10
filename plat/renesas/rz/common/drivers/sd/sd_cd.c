/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**********************************************************************************************************************
 * File Name    : sd_cd.c
 * Version      : 1.0
 * Description  : Card detect.
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
#define EXT_CD_REM    (0x0001)
#define EXT_CD_INS    (0x0002)

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
 * Function Name: esd_check_media
 * Description  : check card insertion
 *              : if card is inserted, return SD_OK
 *              : if card is not inserted, return SD_ERR
 *              : if SD handle is not initialized, return SD_ERR
 * Arguments    : SDHNDL *hndl : SD handle
 * Return Value : SD_OK : card is inserted
 *              : SD_ERR: card is not inserted
 *********************************************************************************************************************/
int32_t esd_check_media(void)
{
    SDHNDL    *p_hndl;

    p_hndl = _sd_get_hndl();
    if(p_hndl == 0)
    {
        return SD_ERR;    /* Not initialized */
    }
    
    return _sd_check_media(p_hndl);
}
/**********************************************************************************************************************
 * End of function esd_check_media
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_check_media
 * Description  : check card insertion
 *              : if card is inserted, return SD_OK
 *              : if card is not inserted, return SD_ERR
 * Arguments    : SDHNDL *hndl : SD handle
 * Return Value : SD_OK : card is inserted
 *              : SD_ERR: card is not inserted
 *********************************************************************************************************************/
int32_t _sd_check_media(SDHNDL *hndl)
{
    return SD_OK;    /* Inserted */
}
/**********************************************************************************************************************
 * End of function _sd_check_media
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: _sd_get_ext_cd_int
 * Description  : Check&Get Port1-3 Card Ins&Rem Interrupt request.
 * Arguments    : SDHNDL *hndl   ; SD driver handle
 * Return Value : != 0 : interrupt ocurred
 *              : == 0 : interrupt not ocurred
 *********************************************************************************************************************/
int32_t _sd_get_ext_cd_int(SDHNDL *hndl)
{
    uint16_t reg;
    uint16_t sift;
    uint16_t info;
    
    if(hndl->port == 0)
    {
        return SD_ERR;
    }
    sift = (uint16_t)((hndl->port - 1)*3);
    info = 0;
    
    reg = sd_inp(hndl,EXT_CD_MASK);
    reg = (uint16_t)(sd_inp(hndl,EXT_CD) & (~reg));

    sd_outp(hndl,EXT_CD,(uint16_t)~reg);
    
    reg >>= sift;
    
    if(reg & EXT_CD_REM)
    {
        info |= SD_INFO1_MASK_REM_CD;
    }
    else if(reg & EXT_CD_INS)
    {
        info |= SD_INFO1_MASK_INS_CD;
    }
    else
    {
        ; /* DO NOTHING */
    }

    reg = sd_inp(hndl,EXT_CD_DAT3_MASK);
    reg = (uint16_t)(sd_inp(hndl,EXT_CD_DAT3) & (~reg));

    sd_outp(hndl,EXT_CD_DAT3,(uint16_t)~reg);
    
    reg >>= sift;
    
    if(reg & EXT_CD_REM)
    {
        info |= SD_INFO1_MASK_REM_DAT3;
    }
    else if(reg & EXT_CD_INS)
    {
        info |= SD_INFO1_MASK_INS_DAT3;
    }
    else
    {
        ; /* DO NOTHING */
    }
    
    hndl->int_info1 &= (uint16_t)~SD_INFO1_MASK_DET_DAT3_CD;
    
    hndl->int_info1 |= info;

    return (int32_t)info;
}
/**********************************************************************************************************************
 * End of function _sd_get_ext_cd_int
 *********************************************************************************************************************/

/* End of File */
