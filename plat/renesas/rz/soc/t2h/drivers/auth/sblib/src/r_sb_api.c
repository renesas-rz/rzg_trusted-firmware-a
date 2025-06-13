/*
 * Copyright (C) 2019-2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*!********************************************************************************************************************
* \file             : r_sb_api.c
* \par version      : 1.00
* \par Product Name : Renesas-SB-Lib FSBL Custom
* \par Device(s)    : Does not depend on specific hardware
* \par Description  : This file implements the SB-Lib public APIs.
**********************************************************************************************************************/

/*=====================================================================================================================
 Includes <System Includes> , "Project Includes"
=====================================================================================================================*/
#include <stddef.h>
#include "r_sb_api.h"
#include "r_sb_build_config.h"
#include "r_sb_cmn.h"
#include "r_sb_manifest.h"
#include "r_sb_sb.h"
#include "r_sb_crc.h"

/*=====================================================================================================================
 Private macro definitions
=====================================================================================================================*/
/* Error checking needs to be done where the settings are used, not in a configuration file. */
#if (SB_CFG_PARAM_CHECKING_ENABLE == 1U)
    #if (0U != SB_CFG_SB_MAC_VERIFICATION) && (1U != SB_CFG_SB_MAC_VERIFICATION)
    #error This is an error check from the build configuration. Cause: SB_CFG_SB_MAC_VERIFICATION
    #endif
    #if (0U != SB_CFG_SB_CERT_CHAIN_VERIFICATION) && (1U != SB_CFG_SB_CERT_CHAIN_VERIFICATION)
    #error This is an error check from the build configuration. Cause: SB_CFG_SB_CERT_CHAIN_VERIFICATION
    #endif
    #if (0U != SB_CFG_SB_CERT_CHAIN_USE_IMG_PK) && (1U != SB_CFG_SB_CERT_CHAIN_USE_IMG_PK)
    #error This is an error check from the build configuration. Cause: SB_CFG_SB_CERT_CHAIN_USE_IMG_PK
    #endif
    #if (0U != SB_CFG_IMAGE_ENC_DEC) && (1U != SB_CFG_IMAGE_ENC_DEC)
    #error This is an error check from the build configuration. Cause: SB_CFG_IMAGE_ENC_DEC
    #endif
    #if (0U != SB_CFG_CHECK_CRC) && (1U != SB_CFG_CHECK_CRC)
    #error This is an error check from the build configuration. Cause: SB_CFG_CHECK_CRC
    #endif
    #if (0U != SB_CFG_CHECK_INTEGRITY) && (1U != SB_CFG_CHECK_INTEGRITY)
    #error This is an error check from the build configuration. Cause: SB_CFG_CHECK_INTEGRITY
    #endif
#endif

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
* Function Name : R_SB_SecureBoot
**********************************************************************************************************************/
sb_ret_t R_SB_SecureBoot(const uint8_t* const p_key_cert, const uint32_t key_cert_len_max,
                            const uint8_t* const p_code_cert, const uint32_t code_cert_len_max,
                            const uint8_t* const p_mac_tlv)
{
    /*-----------------------------------------------------------------------------------------------------------------
     Local variables
    -----------------------------------------------------------------------------------------------------------------*/
    sb_ret_t  ret               = SB_RET_ERR_INTERNAL_FAIL;

    sb_bool_t flow_correct_ret  = SB_PRV_FALSE;
#if (SB_CFG_SB_CERT_CHAIN_VERIFICATION == 1U)
    st_sb_key_cert_t key_cert_st;
#endif /* (SB_CFG_SB_CERT_CHAIN_VERIFICATION == 1U) */
#if (SB_CFG_SB_CERT_CHAIN_VERIFICATION == 1U) || (SB_CFG_SB_MAC_VERIFICATION == 1U)
    st_sb_code_cert_t code_cert_st;
#endif /* (SB_CFG_SB_CERT_CHAIN_VERIFICATION == 1U) || (SB_CFG_SB_MAC_VERIFICATION == 1U) */
#if (SB_CFG_SB_MAC_VERIFICATION == 1U)
    st_sb_tlv_t mac_tlv_st;
#endif /* (SB_CFG_SB_MAC_VERIFICATION == 1U) */

    /*-----------------------------------------------------------------------------------------------------------------
     Function body
    -----------------------------------------------------------------------------------------------------------------*/
    /* Initialize flow counter */
    r_sb_cmn_fc_sb_init();

    /* Select verification mode */
    if (NULL == p_mac_tlv)
    {
#if (SB_CFG_SB_CERT_CHAIN_VERIFICATION == 1U)
        /* Check NULL argument excepted mac_tlv */
        if ((NULL != p_key_cert) && (NULL != p_code_cert))
        {
            /* Check alignment */
            /* No problem with casting to pointer size */
            if ((((uintptr_t)p_key_cert & (uintptr_t)SB_PRV_REMAINDER_DIV4) == (uintptr_t)0UL) &&
                (((uintptr_t)p_code_cert & (uintptr_t)SB_PRV_REMAINDER_DIV4) == (uintptr_t)0UL)) /*Same cast as above*/
            {
                /* Set Key cert structure */
                ret = r_sb_mani_set_key_cert_st(p_key_cert, &key_cert_st);
                if (SB_RET_SUCCESS == ret)
                {
                    /* Clear ret */
                    ret = SB_RET_ERR_INTERNAL_FAIL;

                    /* Check key cert */
                    ret = r_sb_mani_chk_key_cert(&key_cert_st, key_cert_len_max);
                    if (SB_RET_SUCCESS == ret)
                    {
                        /* Clear ret */
                        ret = SB_RET_ERR_INTERNAL_FAIL;

                        /* Set Code cert structure */
                        ret = r_sb_mani_set_code_cert_st(p_code_cert, &code_cert_st);
                        if (SB_RET_SUCCESS == ret)
                        {
                            /* Clear ret */
                            ret = SB_RET_ERR_INTERNAL_FAIL;

                            /* Check code cert */
                            ret = r_sb_mani_chk_code_cert(&code_cert_st, code_cert_len_max);
                            if (SB_RET_SUCCESS == ret)
                            {
                                /* Clear ret */
                                ret = SB_RET_ERR_INTERNAL_FAIL;

                                /* Verify certificate chain */
                                ret = r_sb_sb_verify_cert_chain(&key_cert_st, &code_cert_st);
                            }
                            else
                            {
                                /* Do nothing */
                            }
                        }
                        else
                        {
                            /* Do nothing */
                        }
                    }
                    else
                    {
                        /* Do nothing */
                    }
                }
                else
                {
                    /* Do nothing. */
                }
            }
            else
            {
                /* Invalid alignment */
                ret = SB_RET_ERR_INVALID_ALIGNMENT;
            }
        }
        else
        {
            /* Invalid argument */
            ret = SB_RET_ERR_INVALID_ARG;
        }
#else  /* (SB_CFG_SB_CERT_CHAIN_VERIFICATION == 1U) */
        ret = SB_RET_ERR_UNSUPPORTED_FUNCTION;
#endif /* (SB_CFG_SB_CERT_CHAIN_VERIFICATION == 1U) */
    }
    else
    {
#if (SB_CFG_SB_MAC_VERIFICATION == 1U)
        /* Check NULL argument excepted mac_tlv */
        if (NULL != p_code_cert)
        {
            /* Check alignment */
            /* No problem with casting to pointer size */
            if ((((uintptr_t)p_code_cert & (uintptr_t)SB_PRV_REMAINDER_DIV4) == (uintptr_t)0UL) &&
                (((uintptr_t)p_mac_tlv & (uintptr_t)SB_PRV_REMAINDER_DIV4) == (uintptr_t)0UL)) /* Same cast as above */
            {
                /* Set Code cert structure */
                ret = r_sb_mani_set_code_cert_st(p_code_cert, &code_cert_st);
                if (SB_RET_SUCCESS == ret)
                {
                    /* Clear ret */
                    ret = SB_RET_ERR_INTERNAL_FAIL;

                    /* Check code cert */
                    ret = r_sb_mani_chk_code_cert(&code_cert_st, code_cert_len_max);
                    if (SB_RET_SUCCESS == ret)
                    {
                        /* Clear ret */
                        ret = SB_RET_ERR_INTERNAL_FAIL;

                        /* Set MAC TLV structure */
                        ret = r_sb_mani_set_mac_tlv_st(p_mac_tlv, &mac_tlv_st);
                        if (SB_RET_SUCCESS == ret)
                        {
                            /* Clear ret */
                            ret = SB_RET_ERR_INTERNAL_FAIL;

                            /* Verify MAC */
                            ret = r_sb_sb_verify_mac(&code_cert_st, &mac_tlv_st);
                        }
                        else
                        {
                            /* Do nothing */
                        }
                    }
                    else
                    {
                        /* Do nothing. */
                    }
                }
                else
                {
                    /* Do nothing. */
                }
            }
            else
            {
                /* Invalid alignment */
                ret = SB_RET_ERR_INVALID_ALIGNMENT;
            }
        }
        else
        {
            /* Invalid argument */
            ret = SB_RET_ERR_INVALID_ARG;
        }
#else   /* (SB_CFG_SB_MAC_VERIFICATION == 1U) */
        ret = SB_RET_ERR_UNSUPPORTED_FUNCTION;
#endif  /* (SB_CFG_SB_MAC_VERIFICATION == 1U) */
    }

    /* Verify flow counter */
    flow_correct_ret = r_sb_cmn_fc_is_sb_flow_correct();
    if (SB_RET_SUCCESS == ret)
    {
        if (SB_PRV_TRUE == flow_correct_ret)
        {
            /* Do nothing */
        }
        else
        {
            /* Flow counter mismatch  */
            ret = SB_RET_ERR_INTERNAL_FAIL;
        }
    }
    else
    {
        /* Do nothing */
    }

    return ret;
}
/**********************************************************************************************************************
* End of function R_SB_SecureBoot()
**********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name : R_SB_CheckIntegrity
**********************************************************************************************************************/
sb_ret_t R_SB_CheckIntegrity(const uint8_t* const p_key_cert, const uint32_t key_cert_len_max,
                                const uint8_t* const p_code_cert, const uint32_t code_cert_len_max,
                                const e_sb_mac_type_t mac_type, uint32_t* const p_tag)
{
#if (SB_CFG_CHECK_INTEGRITY == 1U)
    /*-----------------------------------------------------------------------------------------------------------------
     Local variables
    -----------------------------------------------------------------------------------------------------------------*/
    sb_ret_t ret = SB_RET_ERR_INTERNAL_FAIL;

    sb_bool_t         flow_correct_ret = SB_PRV_FALSE;
    st_sb_key_cert_t  key_cert_st;
    st_sb_code_cert_t code_cert_st;

    /*-----------------------------------------------------------------------------------------------------------------
     Function body
    -----------------------------------------------------------------------------------------------------------------*/
    /* Initialize flow counter */
    r_sb_cmn_fc_ci_init();

    /* Check NULL argument excepted mac_tlv */
    if ((NULL != p_key_cert) && (NULL != p_code_cert) && (NULL != p_tag))
    {
        /* Check alignment */
        /* No problem with casting to pointer size */
        if ((((uintptr_t)p_key_cert & (uintptr_t)SB_PRV_REMAINDER_DIV4) == (uintptr_t)0UL) &&
            (((uintptr_t)p_code_cert & (uintptr_t)SB_PRV_REMAINDER_DIV4) == (uintptr_t)0UL) && /* Same cast as above */
            (((uintptr_t)p_tag & (uintptr_t)SB_PRV_REMAINDER_DIV4) == (uintptr_t)0UL)) /* Same cast as above */
        {
            /* Set Key cert structure */
            ret = r_sb_mani_set_key_cert_st(p_key_cert, &key_cert_st);
            if (SB_RET_SUCCESS == ret)
            {
                /* Clear ret */
                ret = SB_RET_ERR_INTERNAL_FAIL;

                /* Check key cert */
                ret = r_sb_mani_chk_key_cert(&key_cert_st, key_cert_len_max);
                if (SB_RET_SUCCESS == ret)
                {
                    /* Clear ret */
                    ret = SB_RET_ERR_INTERNAL_FAIL;

                    /* Set Code cert structure */
                    ret = r_sb_mani_set_code_cert_st(p_code_cert, &code_cert_st);
                    if (SB_RET_SUCCESS == ret)
                    {
                        /* Clear ret */
                        ret = SB_RET_ERR_INTERNAL_FAIL;

                        /* Check code cert */
                        ret = r_sb_mani_chk_code_cert(&code_cert_st, code_cert_len_max);
                        if (SB_RET_SUCCESS == ret)
                        {
                            /* Clear ret */
                            ret = SB_RET_ERR_INTERNAL_FAIL;

                            /* Verifies the image integrity with a certificate chain and generate MAC */
                            ret = r_sb_sb_check_integrity(&key_cert_st, &code_cert_st, mac_type, p_tag);
                        }
                        else
                        {
                            /* Do nothing */
                        }
                    }
                    else
                    {
                        /* Do nothing */
                    }
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                /* Do nothing. */
            }
        }
        else
        {
            /* Invalid alignment */
            ret = SB_RET_ERR_INVALID_ALIGNMENT;
        }
    }
    else
    {
        /* Invalid argument */
        ret = SB_RET_ERR_INVALID_ARG;
    }

    /* Verify flow counter */
    flow_correct_ret = r_sb_cmn_fc_is_ci_flow_correct();
    if ((SB_RET_SUCCESS == ret) || (SB_RET_SAME_IMAGE_VERSION == ret))
    {
        if (SB_PRV_TRUE == flow_correct_ret)
        {
            /* Do nothing */
        }
        else
        {
            /* Flow counter mismatch  */
            ret = SB_RET_ERR_INTERNAL_FAIL;
        }
    }
    else
    {
        /* Do nothing */
    }

    return ret;
#else /* (SB_CFG_CHECK_INTEGRITY == 1U) */
    return SB_RET_ERR_UNSUPPORTED_FUNCTION;
#endif /* (SB_CFG_CHECK_INTEGRITY == 1U) */
}
/**********************************************************************************************************************
* End of function R_SB_CheckIntegrity()
**********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name : R_SB_CheckCRC
**********************************************************************************************************************/
sb_ret_t R_SB_CheckCRC(const uint8_t* const p_code_cert, const uint32_t code_cert_len_max)
{
#if (SB_CFG_CHECK_CRC == 1U)
    /*-----------------------------------------------------------------------------------------------------------------
     Local variables
    -----------------------------------------------------------------------------------------------------------------*/
    sb_ret_t            ret = SB_RET_ERR_INTERNAL_FAIL;

    st_sb_code_cert_t   code_cert_st;

    /*-----------------------------------------------------------------------------------------------------------------
     Function body
    -----------------------------------------------------------------------------------------------------------------*/
    /* Check NULL argument */
    if (NULL != p_code_cert)
    {
        /* Check alignment */
        /* No problem with casting to pointer size */
        if (((uintptr_t)p_code_cert & (uintptr_t)SB_PRV_REMAINDER_DIV4) == (uintptr_t)0UL)
        {

            /* Set Code cert structure */
            ret = r_sb_mani_set_code_cert_st(p_code_cert, &code_cert_st);

            if (SB_RET_SUCCESS == ret)
            {
                /* Clear ret */
                ret = SB_RET_ERR_INTERNAL_FAIL;

                /* Check code cert */
                ret = r_sb_mani_chk_code_cert(&code_cert_st, code_cert_len_max);

                if (SB_RET_SUCCESS == ret)
                {
                    /* Clear ret */
                    ret = SB_RET_ERR_INTERNAL_FAIL;

                    /* Check CRC */
                    ret = r_sb_crc_check_crc(&code_cert_st);
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Invalid alignment */
            ret = SB_RET_ERR_INVALID_ALIGNMENT;
        }
    }
    else
    {
        /* Invalid argument */
        ret = SB_RET_ERR_INVALID_ARG;
    }

    return ret;

#else /* (SB_CFG_CHECK_CRC == 1U) */
    return SB_RET_ERR_UNSUPPORTED_FUNCTION;
#endif /*  (SB_CFG_CHECK_CRC == 1U) */

}
/**********************************************************************************************************************
* End of function R_SB_CheckCRC()
**********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name : R_SB_GetVersion
**********************************************************************************************************************/
sb_ret_t R_SB_GetVersion(uint32_t* const p_major, uint32_t* const p_minor)
{
    /*-----------------------------------------------------------------------------------------------------------------
     Local variables
    -----------------------------------------------------------------------------------------------------------------*/
    sb_ret_t ret = SB_RET_ERR_INTERNAL_FAIL;

    /*-----------------------------------------------------------------------------------------------------------------
     Function body
    -----------------------------------------------------------------------------------------------------------------*/
    /* Check NULL argument */
    if ((NULL != p_major) && (NULL != p_minor))
    {
        /* Get major version */
        *p_major = SB_PRV_VERSION_MAJOR;

        /* Get minor version */
        *p_minor = SB_PRV_VERSION_MINOR;

        ret = SB_RET_SUCCESS;
    }
    else
    {
        ret = SB_RET_ERR_INVALID_ARG;
    }

    return ret;
}
/**********************************************************************************************************************
* End of function R_SB_GetVersion()
**********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name : R_SB_ParseManiTLVs
**********************************************************************************************************************/
sb_ret_t R_SB_ParseManiTLVs(const uint8_t* const p_top, const uint32_t tlv_len, const uint32_t num_of_search_type,
                                const st_sb_search_tlv_type_t search_types[], st_sb_tlv_t tlvs[])
{
    sb_ret_t ret = SB_RET_ERR_INTERNAL_FAIL;

    ret = r_sb_mani_parse_tlvs(p_top, tlv_len, num_of_search_type, search_types, tlvs);

    return ret;
}
/**********************************************************************************************************************
* End of function R_SB_ParseManiTLVs()
**********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name : R_SB_GetCodeCertInfo
**********************************************************************************************************************/
sb_ret_t R_SB_GetCodeCertInfo(const uint8_t* const p_code_cert, const uint32_t code_cert_len_max,
                                st_sb_code_cert_t* const p_code_cert_st)
{
    sb_ret_t ret = SB_RET_ERR_INTERNAL_FAIL;

    if ((NULL != p_code_cert) && (NULL != p_code_cert_st))
    {
        /* Check alignment */
        /* No problem with casting to pointer size */
        if (((uintptr_t)p_code_cert & (uintptr_t)SB_PRV_REMAINDER_DIV4) == (uintptr_t)0UL)
        {
            ret = r_sb_mani_set_code_cert_st(p_code_cert, p_code_cert_st);

            if (SB_RET_SUCCESS == ret)
            {
                /* Clear ret */
                ret = SB_RET_ERR_INTERNAL_FAIL;

                ret = r_sb_mani_chk_code_cert(p_code_cert_st, code_cert_len_max);
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            ret = SB_RET_ERR_INVALID_ALIGNMENT;
        }
    }
    else
    {
        ret = SB_RET_ERR_INVALID_ARG;
    }

    return ret;
}
/**********************************************************************************************************************
* End of function R_SB_GetCodeCertInfo()
**********************************************************************************************************************/

/*=====================================================================================================================
 Private function definitions
=====================================================================================================================*/

/*=====================================================================================================================
 End of file
=====================================================================================================================*/
