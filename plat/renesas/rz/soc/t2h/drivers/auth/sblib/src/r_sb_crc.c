/*
 * Copyright (C) 2019-2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*!********************************************************************************************************************
* \file             : r_sb_crc.c
* \par version      : 1.00
* \par Product Name : Renesas-SB-Lib FSBL Custom
* \par Device(s)    : Does not depend on specific hardware
* \par Description  : This file implements the CRC module APIs.
**********************************************************************************************************************/

/*!***********************************************************
 * \addtogroup  SBLIBCRC
 * \{
 * \addtogroup  SBLIBCRCAPIs              CRC APIs
 * \addtogroup  SBLIBCRCDefinesInternal   CRC Internal Definitions
 * \addtogroup  SBLIBCRCAPIsInternal      CRC Internal APIs
 * \}********************************************************/

/*=====================================================================================================================
 Includes <System Includes> , "Project Includes"
=====================================================================================================================*/
#include <stddef.h>
#include "r_cip_drv_api.h"
#include "r_sb_build_config.h"
#include "r_sb_api.h"
#include "r_sb_manifest.h"
#include "r_sb_crc.h"
#include "r_sb_cmn.h"
#include "r_sb_cmn_drv.h"

/*=====================================================================================================================
 Private macro definitions
=====================================================================================================================*/
/*!********************************************************************
 * \addtogroup SBLIBCRCDefinesInternal
 * \{
 * \addtogroup  group_tlv_search_crc
 * \}*****************************************************************/

/** TLVs to search by CRC */
/**\addtogroup group_tlv_search_crc TLV search CRC */
/*! \{*/
#define SB_PRV_TLV_CRC_NUM                          (1U)  /**< Number of CRC TLVs */
#define SB_PRV_TLV_CRC_IDX                          (0U)  /**< Index number of CRC */

/** Class & Use type of CRC */
#define SB_PRV_TLV_CRC_IMG_TYPE                     (SB_PRV_TLV_TYPE_CLS_CRC  | SB_PRV_TLV_TYPE_CLS_CRC_UT_IMG)
/** Class mask & Use type mask of CRC */
#define SB_PRV_TLV_CRC_IMG_MASK                     (SB_PRV_TLV_TYPE_CLS_MASK | SB_PRV_TLV_TYPE_CLS_CRC_UT_MASK)

/*! \}*/

/*=====================================================================================================================
 Private type definitions
=====================================================================================================================*/

/*=====================================================================================================================
 Public global variables
=====================================================================================================================*/

/*=====================================================================================================================
 Private global variables
=====================================================================================================================*/

/*=====================================================================================================================
 Private function prototypes
=====================================================================================================================*/

/*=====================================================================================================================
 Public function definitions
=====================================================================================================================*/
/**********************************************************************************************************************
* Function Name : r_sb_crc_check_crc
**********************************************************************************************************************/
/*!****************************************************************************************************************//**
 * \ingroup     SBLIBCRCAPIsInternal
 * \brief       Check the CRC of the image
 * \param       [in]   p_code_cert_st          Start address of Code Certificate Structure
 * \return
 *  - #SB_RET_SUCCESS:                             Successful completion
 *  - #SB_RET_ERR_INTERNAL_FAIL:                   Internal processing failed
 *  - #SB_RET_ERR_MANI_TLV_FIELD_ERR:              Missing required TLV fields
 *  - #SB_RET_ERR_MANI_TLV_INVALID_LEN:            The length of the TLV field has a size that exceeds the end of
 *                                                 the manifest.
 *  - #SB_RET_ERR_CRC_MISMATCH:                    CRC mismatch
 *  - #SB_RET_ERR_CRYPTO_UNSUPPORTED_ALGORITHM:    An algorithm was specified that the manifest does not support
 * \par Global Variables
 *  - None
 * \par Call SB-Driver API
 *  - R_CIP_DRV_PrcCheckCRC
 * \par Precondition
 *  - None
 * \par Security components
 *  - no
 * \par Detailed description
 *      - Check the value by parsing the TLV from the manifest
 *      - CRC calculation depends on the polynomial set for the type of TLV
 *      - Since the CRC check is for data error detection, glitch countermeasure isn't taken
 *
 * Pseudo-code for the function can be found below.
 * -# Initialize the variable ret with #SB_RET_ERR_INTERNAL_FAIL
 * -# Initialize the variable cip_ret with CIP_DRV_RET_FAIL
 * -# Set TLV search condition of CRC to search_tlv_crc
 * -# If (NULL != p_code_cert_st) && (NULL != p_code_cert_st->p_header)
 *   -# Parse the code certificate TLV with #r_sb_mani_parse_tlvs and get the CRC TLV
 *   -# Set ret to the function result
 *   -# Set ret to #SB_RET_ERR_INTERNAL_FAIL
 *   -# If #SB_RET_SUCCESS == ret
 *     -# If NULL != crc_tlvs[#SB_PRV_TLV_CRC_IDX].p_val
 *       -# Set img_addr to p_code_cert_st->p_header->dest_addr
 *       -# Assign the polynomial field in crc_tlvs[#SB_PRV_TLV_CRC_IDX].type to crc_param.crc_algo
 *       -# Set crc_param.img_len to p_code_cert_st->p_header->img_len
 *       -# Set crc_param.p_crc to crc_tlvs[SB_PRV_TLV_CRC_IDX].p_val
 *       -# Set crc_param.p_img to img_addr
 *       -# Call SB-Driver API R_CIP_DRV_PrcCheckCRC
 *       -# Set cip_ret to the SB-Driver API result
 *       -# Get the return code from the value of cpi_ret with #r_sb_cmn_drv_get_sb_ret_from_cip_ret
 *     -# Else
 *       -# Set ret to #SB_RET_ERR_MANI_TLV_FIELD_ERR
 *   -# Else
 *     -# Do nothing
 * -# Else
 *   -# Do nothing
 * -# Return ret
 *
 * Note: The call graph below dose not include SB-Driver APIs.
 * \callgraph
 *********************************************************************************************************************/
#if (SB_CFG_CHECK_CRC == 1U)
sb_ret_t r_sb_crc_check_crc(const st_sb_code_cert_t* const p_code_cert_st)
{
    /*-----------------------------------------------------------------------------------------------------------------
     Local variables
    -----------------------------------------------------------------------------------------------------------------*/
    sb_ret_t                            ret = SB_RET_ERR_INTERNAL_FAIL;

    cip_drv_ret_t                       cip_ret = CIP_DRV_RET_FAIL;
    st_cip_drv_crc_param_t              crc_param;
    volatile uintptr_t                  img_addr;
    st_sb_tlv_t                         crc_tlvs[SB_PRV_TLV_CRC_NUM];
    const st_sb_search_tlv_type_t       search_tlv_crc[SB_PRV_TLV_CRC_NUM] =
    {
        {SB_PRV_TLV_CRC_IMG_TYPE, SB_PRV_TLV_CRC_IMG_MASK}
    };

    /*-----------------------------------------------------------------------------------------------------------------
     Function body
    -----------------------------------------------------------------------------------------------------------------*/

    if ((NULL != p_code_cert_st) && (NULL != p_code_cert_st->p_header))
    {
        ret = r_sb_mani_parse_tlvs(p_code_cert_st->p_tlv_top, p_code_cert_st->tlv_len, SB_PRV_TLV_CRC_NUM,
                                    search_tlv_crc, crc_tlvs);
        if (SB_RET_SUCCESS == ret)
        {
            /* Clear ret */
            ret = SB_RET_ERR_INTERNAL_FAIL;

            if (NULL != crc_tlvs[SB_PRV_TLV_CRC_IDX].p_val)
            {
                /* Assign the address set in dest_addr once to a uintptr_t type variable
                    (32/64bit MPU countermeasures) */
                img_addr = p_code_cert_st->p_header->dest_addr;

                /* Set CRC parameters */
                crc_param.crc_algo  = (crc_tlvs[SB_PRV_TLV_CRC_IDX].type & SB_PRV_TLV_TYPE_CLS_CRC_POLY_MASK)
                                    >> SB_PRV_TLV_TYPE_CLS_CRC_POLY_POS;
                crc_param.img_len   = p_code_cert_st->p_header->img_len;
                crc_param.p_crc     = crc_tlvs[SB_PRV_TLV_CRC_IDX].p_val;

                /* Cast a uintptr_t type variable to a uint32_t* type pointer */
                crc_param.p_img     = (const uint32_t*)img_addr;

                /* Calculate and check CRC */
                /* Call SB-Driver API */
                cip_ret = R_CIP_DRV_PrcCheckCRC(&crc_param);
                ret     = r_sb_cmn_drv_get_sb_ret_from_cip_ret(cip_ret);
            }
            else
            {
                ret = SB_RET_ERR_MANI_TLV_FIELD_ERR;
            }
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* If there are null arguments, return SB_RET_ERR_INTERNAL_FAIL */
    }

    return ret;

}
#endif /* (SB_CFG_CHECK_CRC == 1U) */
/**********************************************************************************************************************
* End of function r_sb_crc_check_crc()
**********************************************************************************************************************/

/*=====================================================================================================================
 Private function definitions
=====================================================================================================================*/

/*=====================================================================================================================
 End of file
=====================================================================================================================*/
