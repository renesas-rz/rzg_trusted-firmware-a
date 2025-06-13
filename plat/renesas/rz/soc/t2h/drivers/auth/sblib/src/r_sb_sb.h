/**********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* www.renesas.com/disclaimer
*
* Copyright (C) 2019-2021 Renesas Electronics Corporation. All rights reserved.
**********************************************************************************************************************/
/*!********************************************************************************************************************
* \file             : r_sb_sb.h
* \par version      : 1.00
* \par Product Name : Renesas-SB-Lib FSBL Custom
* \par Device(s)    : Does not depend on specific hardware
* \par Description  : This file defines the macros, types and declarations in used the SecureBoot module.
**********************************************************************************************************************/
/**********************************************************************************************************************
* History : Version           DD.MM.YYYY Description
*         : 1.00              24.08.2020 First Release.
*         : FSBL Custom 1.00  27.10.2021 First Release.
**********************************************************************************************************************/
#ifndef R_SB_SB_H
/* Multiple inclusion protection macro */
#define R_SB_SB_H

/*=====================================================================================================================
 Public macro definitions
=====================================================================================================================*/
/*!********************************************************************
 * \addtogroup SBLIBSecureBootDefinesInternal
 * \{******************************************************************/

/*! \}*/

/*=====================================================================================================================
 Public type definitions
=====================================================================================================================*/

/*=====================================================================================================================
 Public global variables
=====================================================================================================================*/

/*=====================================================================================================================
 Public global functions
=====================================================================================================================*/
extern sb_ret_t r_sb_sb_verify_cert_chain (const st_sb_key_cert_t * const p_key_cert_st,
                                            const st_sb_code_cert_t * const p_code_cert_st);
extern sb_ret_t r_sb_sb_verify_mac (const st_sb_code_cert_t * const p_code_cert_st,
                                    const st_sb_tlv_t * const p_mac_tlv);
extern sb_ret_t r_sb_sb_check_integrity (const st_sb_key_cert_t * const p_key_cert_st,
                                        const st_sb_code_cert_t * const p_code_cert_st,
                                        const e_sb_mac_type_t mac_type, uint32_t * const p_tag);
#endif /* R_SB_SB_H */
/*=====================================================================================================================
 End of File
=====================================================================================================================*/
