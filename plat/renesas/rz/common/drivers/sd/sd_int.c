/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**********************************************************************************************************************
 * File Name    : sd_int.c
 * Version      : 1.0
 * Description  : SD_INFO1 and SD_INFO2 interrupt.
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
 * Function Name: _sd_set_int_mask
 * Description  : set int_info1_mask and int_info2_mask depend on the mask bits value
 *              : if mask bit is one, it is enabled
 *              : if mask bit is zero, it is disabled
 * Arguments    : SDHNDL *hndl : SD handle
 *              : uint16_t mask1 : SD_INFO1_MASK1 bits value
 *              : uint16_t mask2 : SD_INFO1_MASK2 bits value
 * Return Value : SD_OK : end of succeed
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t    _sd_set_int_mask(SDHNDL *hndl, uint16_t mask1, uint16_t mask2)
{
    esddev_loc_cpu();
    
    /* ---- Set int_info1_mask and int_info2_mask ---- */
    hndl->int_info1_mask |= mask1;
    hndl->int_info2_mask |= mask2;

    /* ---- Set hardware mask ---- */
    sd_outp(hndl,SD_INFO1_MASK,(uint16_t)~(hndl->int_info1_mask));
    sd_outp(hndl,SD_INFO2_MASK,(uint16_t)~(hndl->int_info2_mask));

    esddev_unl_cpu();

    return SD_OK;
}
/**********************************************************************************************************************
 * End of function _sd_set_int_mask
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_clear_int_mask
 * Description  : clear int_cc_status_mask depend on the mask bits value
 *              : if mask bit is one, it is disabled
 *              : if mask bit is zero, it is enabled
 * Arguments    : SDHNDL *hndl : SD handle
 *              : uint16_t mask1 : SD_INFO1_MASK1 bits value
 *              : uint16_t mask2 : SD_INFO1_MASK2 bits value
 * Return Value : SD_OK : end of succeed
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_clear_int_mask(SDHNDL *hndl, uint16_t mask1, uint16_t mask2)
{
    esddev_loc_cpu();
    
    /* ---- Clear int_info1_mask and int_info2_mask ---- */
    hndl->int_info1_mask &= (uint16_t)~mask1;
    hndl->int_info2_mask &= (uint16_t)~mask2;
    
    /* ---- Clear hardware mask ---- */
    sd_outp(hndl,SD_INFO1_MASK,(uint16_t)~(hndl->int_info1_mask));
    sd_outp(hndl,SD_INFO2_MASK,(uint16_t)~(hndl->int_info2_mask));
    
    esddev_unl_cpu();
    
    return SD_OK;
}
/**********************************************************************************************************************
 * End of function _sd_clear_int_mask
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_clear_info
 * Description  : clear int_info1 and int_info2 depend on the clear value
 * Arguments    : SDHNDL *hndl : SD handle
 *              : uint16_t clear_info1 : int_info1 clear bits value
 *              : uint16_t clear_info2 : int_info2 clear bits value
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 * Remark       : SD_INFO1 and SD_INFO2 bits are not cleared
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_clear_info(SDHNDL *hndl, uint16_t clear_info1, uint16_t clear_info2)
{
    esddev_loc_cpu();
    
    /* ---- Clear int_info1 and int_info2 ---- */
    hndl->int_info1 &= (uint16_t)~clear_info1;
    hndl->int_info2 &= (uint16_t)~clear_info2;

    esddev_unl_cpu();
    
    return SD_OK;
}
/**********************************************************************************************************************
 * End of function _sd_clear_info
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _sd_get_int
 * Description  : get SD_INFO1 and SD_INFO2 bits
 *              : examine enabled elements
 *              : hearafter, clear SD_INFO1 and SD_INFO2 bits
 *              : save those bits to int_info1 or int_info2
 * Arguments    : SDHNDL *hndl : SD handle
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t _sd_get_int(SDHNDL *hndl)
{
    uint16_t info1;
    uint16_t info2;
    uint16_t ret = 0;

    /* Get SD_INFO1 and SD_INFO2 bits */
    info1 = (uint16_t)(sd_inp(hndl,SD_INFO1) & hndl->int_info1_mask);
    info2 = (uint16_t)(sd_inp(hndl,SD_INFO2) & hndl->int_info2_mask);

    /* Clear SD_INFO1 and SD_INFO2 bits */
    sd_outp(hndl,SD_INFO1,(uint16_t)~info1);
    sd_outp(hndl,SD_INFO2,(uint16_t)~info2);
        
    /* Save enabled elements */
    /* @080218 SDIO support and interrupt delay countermeasures */
    hndl->int_info1 |= info1;
    hndl->int_info2 |= info2;

    if(hndl->port != 0)
    {
        ret = (uint16_t)_sd_get_ext_cd_int(hndl);
    }
    if(info1 || info2 || ret)
    {
        return SD_OK;    /* Any interrupt occured */
    }
    
    return SD_ERR;    /* No interrupt occured */
}
/**********************************************************************************************************************
 * End of function _sd_get_int
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: esd_check_int
 * Description  : check SD_INFO1 and SD_INFO2 interrupt elements
 *              : if any interrupt is detected, return SD_OK
 *              : if no interrupt is detected, return SD_ERR
 * Arguments    : none
 * Return Value : SD_OK : end of succeed
 *              : SD_ERR: end of error
 *********************************************************************************************************************/
SD_DRV_CODE_SEC int32_t esd_check_int(void)

{
    SDHNDL *p_hndl;
    
    p_hndl = _sd_get_hndl();
    if(p_hndl == 0)
    {
        return SD_ERR;    /* Not initialized */
    }
    
    if(p_hndl->int_mode)
    {
        /* ---- Hardware interrupt mode ---- */
        if(p_hndl->int_info1 || p_hndl->int_info2)
        {
            return SD_OK;
        }
        else
        {
            return SD_ERR;
        }
    }

    /* ---- Polling mode ---- */
    return _sd_get_int(p_hndl);
}
/**********************************************************************************************************************
 * End of function esd_check_int
 *********************************************************************************************************************/

/* End of File */
