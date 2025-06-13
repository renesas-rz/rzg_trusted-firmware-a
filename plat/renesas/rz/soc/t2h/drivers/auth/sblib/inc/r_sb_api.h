/*
 * Copyright (C) 2019-2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*!********************************************************************************************************************
* \file             : r_sb_api.h
* \par version      : 1.00
* \par Product Name : Renesas-SB-Lib FSBL Custom
* \par Device(s)    : Does not depend on specific hardware
* \par Description  : This file defines the public macros, types and declarations in used the SB-Lib.
**********************************************************************************************************************/

#ifndef R_SB_API_H
/* Multiple inclusion protection macro */
#define R_SB_API_H

#include <stdint.h>
/*!***********************************************************
 * \addtogroup  SBLIB           SecureBoot Library
 * \{
 * \addtogroup  SBLIBMod        SecureBoot Library Modules
 * \addtogroup  SBLIBCfg        SecureBoot Library Configurations
 * \}********************************************************/

/*!***********************************************************
 * \addtogroup  SBLIBMod          SecureBoot Library Modules
 * \{
 * \addtogroup  SBLIBCommon       SB-Lib Common module
 * \addtogroup  SBLIBSecureBoot   SB-Lib SecureBoot module
 * \addtogroup  SBLIBCRC          SB-Lib CRC module
 * \addtogroup  SBLIBManifest     SB-Lib Manifest module
 * \}********************************************************/

/*!***********************************************************
 * \addtogroup  SBLIBCfg        SecureBoot Library Configurations
 * \{
 * \addtogroup  SBLIBBC         SB-Lib BuildConfiguration
 * \}********************************************************/

/*=====================================================================================================================
 Public macro definitions
=====================================================================================================================*/
/*!*********************************************************
 * \addtogroup  SBLIBCommonDefines
 * \{
 * \addtogroup  group_return_code
 * \addtogroup  group_manifest_size
 * \addtogroup  group_unused_area
 * \}******************************************************/
/** SB-Lib return value definition */
/**\addtogroup group_return_code Return code */
/*! \{*/
#define SB_RET_SUCCESS                          ((sb_ret_t)0x55555555UL)  /**< API succeeded */
#define SB_RET_SAME_IMAGE_VERSION               ((sb_ret_t)0x55005501UL)  /**< An image of the same version as the
                                                                               current version is input
                                                                               (verification completed successfully) */
#define SB_RET_ERR_INTERNAL_FAIL                ((sb_ret_t)0xAAAA0000UL)  /**< A internal failure */
#define SB_RET_ERR_INVALID_ARG                  ((sb_ret_t)0xAAAA0001UL)  /**< An invalid argument was entered */
#define SB_RET_ERR_UNSUPPORTED_FUNCTION         ((sb_ret_t)0xAAAA0002UL)  /**< Unsupported function executed */
#define SB_RET_ERR_INVALID_ALIGNMENT            ((sb_ret_t)0xAAAA0003UL)  /**< Data entered with incorrect alignment */
#define SB_RET_ERR_MANI_INVALID_MAGIC           ((sb_ret_t)0xAAAA0100UL)  /**< An invalid magic number is set */
#define SB_RET_ERR_MANI_UNSUPPORTED_VERSION     ((sb_ret_t)0xAAAA0101UL)  /**< Unsupported version is set */
#define SB_RET_ERR_MANI_OUT_OF_RANGE_LEN        ((sb_ret_t)0xAAAA0102UL)  /**< Out of range TLV Length is set */
#define SB_RET_ERR_MANI_TLV_FIELD_ERR           ((sb_ret_t)0xAAAA0103UL)  /**< Missing required TLV field */
#define SB_RET_ERR_MANI_TLV_INVALID_LEN         ((sb_ret_t)0xAAAA0104UL)  /**< The length exceeding the end of
                                                                               the manifest is specified in Length
                                                                               of the TLV field */
#define SB_RET_ERR_MANI_INVALID_IMAGE_LEN       ((sb_ret_t)0xAAAA0105UL)  /**< An invalid image length is set */
#define SB_RET_ERR_MANI_MISMATCH_SIGN_ALGORITHM ((sb_ret_t)0xAAAA0106UL)  /**< There is a wrong combination of
                                                                               signature algorithms */
#define SB_RET_ERR_MANI_UNSUPPORTED_ALGORITHM   ((sb_ret_t)0xAAAA0107UL)  /**< An algorithm was specified that
                                                                               the manifest does not support */
#define SB_RET_ERR_CRYPTO_FAIL                  ((sb_ret_t)0xAAAA0200UL)  /**< Cryptographic processing failure */
#define SB_RET_ERR_CRYPTO_AUTH_FAIL             ((sb_ret_t)0xAAAA0201UL)  /**< Verification failed */
#define SB_RET_ERR_CRYPTO_UNSUPPORTED_ALGORITHM ((sb_ret_t)0xAAAA0202UL)  /**< Unsupported algorithm */
#define SB_RET_ERR_CRYPTO_RESOURCE_CONFLICT     ((sb_ret_t)0xAAAA0203UL)  /**< Other resource is using CryptoIP. */
#define SB_RET_ERR_CRYPTO_PARAM_ERR             ((sb_ret_t)0xAAAA0204UL)  /**< Parameter error */
#define SB_RET_ERR_CRC_MISMATCH                 ((sb_ret_t)0xAAAA0300UL)  /**< CRC mismatch */
#define SB_RET_ERR_LOWER_IMAGE_VERSION          ((sb_ret_t)0xAAAA0400UL)  /**< Image version lower than the current
                                                                               image version is installed */
/*! \}*/

/** Manifest size definition */
/**\addtogroup group_manifest_size Manifest size */
/*! \{*/
#define SB_MANIFEST_LEN_MIN   (36UL)      /**< Minimum manifest length */
#define SB_MANIFEST_LEN_MAX   (10240UL)   /**< Maximum manifest length */
/*! \}*/

/** Reserved WORD size definition */
/**\addtogroup group_unused_area Reserved WORD size */
/*! \{*/
#define SB_KEY_CERT_RESERVED_WORD_SIZE   (5U)      /**< Key Certificate header reserved WORD size */
/*! \}*/

/*=====================================================================================================================
 Public type definitions
=====================================================================================================================*/
/*!*********************************************************
 * \addtogroup  SBLIBCommonTypes
 * \{*******************************************************/
typedef uint32_t sb_ret_t;                  /**< SB-Lib return type */

/** MAC type value enumeration */
typedef enum
{
    SB_MAC_TYPE_NONE = 0,                   /**< Type not used */
    SB_MAC_TYPE_HMAC_SHA2_224,              /**< HMAC SHA2-224 */
    SB_MAC_TYPE_HMAC_SHA2_256,              /**< HMAC SHA2-256 */
    SB_MAC_TYPE_HMAC_SHA2_384,              /**< HMAC SHA2-384 */
    SB_MAC_TYPE_HMAC_SHA2_512,              /**< HMAC SHA2-512 */
    SB_MAC_TYPE_HMAC_SHA3_224,              /**< HMAC SHA3-224 */
    SB_MAC_TYPE_HMAC_SHA3_256,              /**< HMAC SHA3-256 */
    SB_MAC_TYPE_HMAC_SHA3_384,              /**< HMAC SHA3-384 */
    SB_MAC_TYPE_HMAC_SHA3_512,              /**< HMAC SHA3-512 */
    SB_MAC_TYPE_CMAC_AES_128,               /**< CMAC AES-128 */
    SB_MAC_TYPE_CMAC_AES_192,               /**< CMAC AES-192 */
    SB_MAC_TYPE_CMAC_AES_256                /**< CMAC AES-256 */
} e_sb_mac_type_t;

/*! \}*/

/*!*********************************************************
 * \addtogroup  SBLIBManifestTypes
 * \{*******************************************************/
/** Key Certificate header structure */
typedef struct
{
    uint32_t    magic;                                          /**< Magic number unique to KeyCertificate */
    uint32_t    manifest_version;                               /**< Manifest version */
    uint32_t    flags;                                          /**< KeyCertificate flag (Unused in V.1.00) */
    uint32_t    reserved[SB_KEY_CERT_RESERVED_WORD_SIZE];   /**< Unused area */
} st_sb_key_cert_header_t;

/** Code Certificate header structure */
typedef struct
{
    uint32_t    magic;                      /**< Magic number unique to CodeCertificate */
    uint32_t    manifest_version;           /**< Manifest version */
    uint32_t    flags;                      /**< CodeCertificate flag */
    uint32_t    load_addr;                  /**< Image storage address */
    uint32_t    dest_addr;                  /**< Image expansion destination address */
    uint32_t    img_len;                    /**< Image byte size */
    uint32_t    img_version;                /**< Image version */
    uint32_t    build_num;                  /**< Image build number */
} st_sb_code_cert_header_t;

/** TLV type of search result */
typedef struct
{
    uint32_t         type;                  /**< TLV type */
    uint32_t         byte_len;              /**< TLV value length */
    const uint32_t * p_val;                 /**< TLV value field Address */
} st_sb_tlv_t;

/** TLV type search condition structure */
typedef struct
{
    uint32_t type;                          /**< TLV type to search */
    uint32_t mask;                          /**< TLV type mask range */
} st_sb_search_tlv_type_t;

/** Code Certificate structure */
typedef struct
{
    const st_sb_code_cert_header_t * p_header;   /**< Header information address */
    uint32_t                         tlv_len;    /**< TLV length */
    const uint8_t *                  p_tlv_top;  /**< Start address of TLV field */
} st_sb_code_cert_t;

/*! \}*/

/*=====================================================================================================================
 Public global variables
=====================================================================================================================*/

/*=====================================================================================================================
 Public global functions
=====================================================================================================================*/
/**********************************************************************************************************************
* Function Name : R_SB_SecureBoot
**********************************************************************************************************************/
/*!****************************************************************************************************************//**
 * \ingroup     SBLIBSecureBootAPIs
 * \brief       Perform a secure boot
 * \param       [in]   p_key_cert           Start address of Key Certificate \n
 *                                          Data must be aligned on 4-byte boundaries \n
 *                                          Don't care when using secure boot with MAC.
 * \param       [in]   key_cert_len_max     Maximum byte length that Key Certificate can take \n
 *                                          Don't care when using secure boot with MAC. \n
 *                                          [range] #SB_MANIFEST_LEN_MIN - #SB_MANIFEST_LEN_MAX
 * \param       [in]   p_code_cert          Start address of Code Certificate \n
 *                                          Data must be aligned on 4-byte boundaries
 * \param       [in]   code_cert_len_max    Maximum byte length that Code Certificate can take \n
 *                                          [range] #SB_MANIFEST_LEN_MIN - #SB_MANIFEST_LEN_MAX
 * \param       [in]   p_mac_tlv            Start address of Code Certificate verification data (TLV format) \n
 *                                          Data must be aligned on 4-byte boundaries \n
 *                                          Specify NULL when verifying with a certificate chain
 * \return
 *  - #SB_RET_SUCCESS:                          Successful completion
 *  - #SB_RET_ERR_INTERNAL_FAIL:                Internal processing failed
 *  - #SB_RET_ERR_INVALID_ARG:                  Invalid argument entered
 *  - #SB_RET_ERR_UNSUPPORTED_FUNCTION:         Unsupported function executed
 *  - #SB_RET_ERR_INVALID_ALIGNMENT:            Data entered with incorrect alignment
 *  - #SB_RET_ERR_MANI_INVALID_MAGIC:           Incorrect magic number is set
 *  - #SB_RET_ERR_MANI_UNSUPPORTED_VERSION:     Unsupported version is set
 *  - #SB_RET_ERR_MANI_OUT_OF_RANGE_LEN:        TLV Length out of range is set
 *  - #SB_RET_ERR_MANI_TLV_FIELD_ERR:           Missing required TLV fields
 *  - #SB_RET_ERR_MANI_TLV_INVALID_LEN:         The length of the TLV field has a size that exceeds the end of
 *                                              the manifest.
 *  - #SB_RET_ERR_MANI_INVALID_IMAGE_LEN:       An invalid image length is set
 *  - #SB_RET_ERR_MANI_MISMATCH_SIGN_ALGORITHM: There is a wrong combination of signature algorithms
 *  - #SB_RET_ERR_MANI_UNSUPPORTED_ALGORITHM:   An algorithm was specified that the manifest does not support
 *  - #SB_RET_ERR_CRYPTO_FAIL:                  Encryption failed
 *  - #SB_RET_ERR_CRYPTO_AUTH_FAIL:             Verification failed
 *  - #SB_RET_ERR_CRYPTO_UNSUPPORTED_ALGORITHM: Unsupported algorithm
 *  - #SB_RET_ERR_CRYPTO_PARAM_ERR:             Parameter error
 *  - #SB_RET_ERR_LOWER_IMAGE_VERSION:          Image version lower than the current image version is installed
 * \par Global Variables
 *  - Image data    [in]    Binary data of the image to be verified \n
 *                          Data must be aligned on 4-byte boundaries
 * \par Call SB-Driver API
 *  - R_CIP_DRV_PrcDeriveMacKeyFromHuk (This API is acutually called by #r_sb_sb_verify_mac)
 *  - R_CIP_DRV_PrcVerifyMac (This API is actually called by #r_sb_sb_verify_mac)
 *  - R_CIP_DRV_PrcVerifyCertChain (This API is acutually called by #r_sb_sb_verify_cert_chain)
 *  - R_CIP_DRV_PrcDecryptImage (This API is actually called by #sb_decrypt_img)
 *  - R_CIP_DRV_CheckImageVersion (This API is acutually called by #r_sb_sb_verify_cert_chain, #r_sb_sb_verify_mac)
 * \par Precondition
 *  - Cryptographic IP initialization is complete.
 *  - The target boot loader program has been copied to the Dest Addr field of the Code Certificate Header.
 * \par Security components
 *  - yes
 * \par Detailed description
 *      - Verification type is determined by the combination of set arguments.
 *| p_key_cert| p_code_cert| p_mac_tlv| verification type            |
 *|:----------|:-----------|:---------|:-----------------------------|
 *|Set        |Set         |Set       |MAC verification (do not use p_key_cert)|
 *|NULL       |Set         |Set       |MAC verification              |
 *|Set        |Set         |NULL      |Certificate chain verification|
 * *Other combinations are errors.
 * \par
 *      - The TLV fields of Key Certificate, Code Certificate,
 *        and MAC referenced by each verification type are described below. \n
 *        < MAC verification >
 *| Type           | Required/Choose/\n Optional| TLV Type(Class) | TLV Type(Use Type)| Description                             |
 *|:---------------|:---------------------------|:----------------|:------------------|:----------------------------------------|
 *|Code Certificate|Required(*)                 |Key              |Image public key|Public key for signature verification of Code Certificate \n (*Only certificate chain verification using the IMG PK of the previous stage configlation enable)|
 *|^               |Optional                    |Image Cipher info|Image Encryption/Decryption Information|Image decryption information \n (Required when using Image Cipher and using a non-default value for the key or output destination address)|
 *|^               |Optional                    |IV               |Image Cipher IV    |IV used in decoding the image \n (Required when Image Cipher info IV select is "IV stored in Code Certificate")|
 *|MAC             |Choose (Use Type)           |MAC              |Code Certificate + \n Image MAC|MAC calculated for Code Certificate and image \n Select Uset type depending on whether the image for MAC is encrypted|
 *|^               |^                           |^                |Code Certificate + \n Encrypted Image MAC|^                  |
 * \n
 *        < Certificate chain verification >
 *| Type           | Required/Choose/\n Optional| TLV Type(Class) | TLV Type(Use Type)| Description                             |
 *|:---------------|:---------------------------|:----------------|:------------------|:----------------------------------------|
 *|Key Certificate |Choose (Use Type)           |Key              |OEM root public key|Public key for signature verification of Key Certificate \n \<When verifying using RoT\> \n Set "OEM root public key" to Use type \n \<When verifying using the previous IMG PK\> \n Set Use type to "Image public key"|
 *|^               |^                           |^                |Image public key   |^                                        |
 *|^               |Required                    |Hash             |Image public key Hash|Hash value of public key for signature verification of Code Certificate|
 *|^               |Required                    |Signature        |Key Certificate signature|Signature value calculated for Key Certificate|
 *|Code Certificate|Required                    |Key              |Image public key   |Public key for signature verification of Code Certificate|
 *|^               |Choose (Use Type)           |Signature        |Code Certificate + \n Image Signature|Code Certificate and signature value calculated by image \n Select Uset type depending on whether the image to be signed is encrypted|
 *|^               |^                           |^                |Code Certificate + \n Encrypted Image Signature|^            |
 *|^               |Optional                    |Image Cipher info|Image Encryption/Decryption Information|Image decryption information \n (Required when using Image Cipher and using a non-default value for the key or output destination address)|
 *|^               |Optional                    |IV               |Image Cipher IV    |IV used in decoding the image            |
 * \par
 *      - Returns an error if the set TLV is in the following cases
 *        - "Required" TLV is not set
 *        - "Choose" TLV is not set
 *        - Two or more "Choose" TLVs are set
 *      - When multiple TLVs with the same Class and Use type are set,
 *        use the TLV found earlier when parsing from the beginning of the TLV field.
 *      - In the MAC calculation in MAC verification, input data in the following order
 *        -# Code Certificate
 *        -# Image
 *      - In hash calculation for signature verification of Code Certificate in certificate chain verification,
 *        input data in the following order
 *        -# Code Certificate (excluding signature value TLV)
 *        -# Image
 *      - Use a key derived from HUK for MAC verification \n This key is generated by Crypto IP
 *      - If Image Cipher in the Code Certificate \> Header \> Flags field is present,
 *        decrypt the image at the time of verification \n Decryption is performed at the following timing
 *        - MAC verification: decrypt after verification
 *        - Certificate chain verification: \n
 *          If the signature target is a plain text image, decrypt it before verification \n
 *          If the signature target is an encrypted image, decrypt it after verification
 *      - Certificate chain verification processing is only compiled if the
 *        build configuration #SB_CFG_SB_CERT_CHAIN_VERIFICATION is enabled
 *      - MAC verification process is only compiled if build configuration #SB_CFG_SB_MAC_VERIFICATION is enabled
 *      - Compare image versions \n
 *        If the image version stored in the anti-rollback counter is lower than the Image version set
 *        in the Code Certificate Header, return #SB_RET_ERR_LOWER_IMAGE_VERSION
 *
 * Pseudo-code for the function can be found below.
 * -# Initialize the variable ret with #SB_RET_ERR_INTERNAL_FAIL
 * -# Set #SB_PRV_FALSE to flow_correct_ret
 * -# Set the initial value for the flow counter with #r_sb_cmn_fc_sb_init
 * -# If NULL == p_mac_tlv
 *   -# <B>--- If #SB_CFG_SB_CERT_CHAIN_VERIFICATION is enabled ---</B>
 *   -# If (NULL != p_key_cert) && (NULL != p_code_cert)
 *     -# If (((uintptr_t)p_key_cert & #SB_PRV_REMAINDER_DIV4) == 0UL) &&
 *           (((uintptr_t)p_code_cert & #SB_PRV_REMAINDER_DIV4) == 0UL)
 *       -# Set Key cert structure with #r_sb_mani_set_key_cert_st
 *       -# If #SB_RET_SUCCESS == ret
 *         -# To clear ret, set ret to #SB_RET_ERR_INTERNAL_FAIL
 *         -# Check Key cert with #r_sb_mani_chk_key_cert
 *         -# If #SB_RET_SUCCESS == ret
 *           -# To clear ret, set ret to #SB_RET_ERR_INTERNAL_FAIL
 *           -# Set code cert structure with #r_sb_mani_set_code_cert_st
 *           -# If #SB_RET_SUCCESS == ret
 *             -# To clear ret, set ret to #SB_RET_ERR_INTERNAL_FAIL
 *             -# Check code cert with #r_sb_mani_chk_code_cert
 *             -# If #SB_RET_SUCCESS == ret
 *               -# To clear ret, set ret to #SB_RET_ERR_INTERNAL_FAIL
 *               -# Verify certificate chain with #r_sb_sb_verify_cert_chain
 *             -# Else
 *               -# Do nothing
 *           -# Else
 *             -# Do nothing
 *         -# Else
 *           -# Do nothing
 *       -# Else
 *         -# Do nothing
 *     -# Else
 *       -# Set #SB_RET_ERR_INVALID_ALIGNMENT to ret
 *   -# Else
 *     -# Set #SB_RET_ERR_INVALID_ARG to ret
 *   -# <B>--- Else #SB_CFG_SB_CERT_CHAIN_VERIFICATION is enabled ---</B>
 *   -# Set #SB_RET_ERR_UNSUPPORTED_FUNCTION to ret
 *   -# <B>--- Endif #SB_CFG_SB_CERT_CHAIN_VERIFICATION is enabled ---</B>
 * -# Else
 *   -# <B>--- If #SB_CFG_SB_MAC_VERIFICATION is enabled ---</B>
 *   -# If NULL != p_code_cert
 *     -# If (((uintptr_t)p_code_cert & #SB_PRV_REMAINDER_DIV4) == 0UL) &&
 *           (((uintptr_t)p_mac_tlv & #SB_PRV_REMAINDER_DIV4) == 0UL)
 *       -# Set code cert structure with #r_sb_mani_set_code_cert_st
 *       -# If #SB_RET_SUCCESS == ret
 *         -# To clear ret, set ret to #SB_RET_ERR_INTERNAL_FAIL
 *         -# Check code cert with #r_sb_mani_chk_code_cert
 *         -# If #SB_RET_SUCCESS == ret
 *           -# To clear ret, set ret to #SB_RET_ERR_INTERNAL_FAIL
 *           -# Set MAC TLV structure with #r_sb_mani_set_mac_tlv_st
 *           -# If #SB_RET_SUCCESS == ret
 *             -# To clear ret, set ret to #SB_RET_ERR_INTERNAL_FAIL
 *             -# Perform MAC verification with r_sb_sb_verify_mac
 *           -# Else
 *             -# Do nothing
 *         -# Else
 *           -# Do nothing
 *       -# Else
 *         -# Do nothing
 *     -# Else
 *       -# Set #SB_RET_ERR_INVALID_ALIGNMENT to ret
 *   -# Else
 *     -# Set #SB_RET_ERR_INVALID_ARG to ret
 *   -# <B>--- Else #SB_CFG_SB_MAC_VERIFICATION is enabled ---</B>
 *   -# Set #SB_RET_ERR_UNSUPPORTED_FUNCTION to ret
 *   -# <B>--- Endif #SB_CFG_SB_MAC_VERIFICATION is enabled ---</B>
 * -# Check if the SecureBoot flow is correct using #r_sb_cmn_fc_is_sb_flow_correct
 * -# Set function result to flow_correct_ret
 * -# If #SB_RET_SUCCESS == ret
 *   -# If #SB_PRV_TRUE == flow_correct_ret
 *     -# Do nothing
 *   -# Else
 *     -# Set #SB_RET_ERR_INTERNAL_FAIL to ret
 * -# Else
 *   -# Do nothing
 * -# Return ret
 *
 * Note: The call graph below dose not include SB-Driver APIs.
 * \callgraph
 *********************************************************************************************************************/
extern sb_ret_t R_SB_SecureBoot (const uint8_t * const p_key_cert,
                                const uint32_t key_cert_len_max,
                                const uint8_t * const p_code_cert,
                                const uint32_t code_cert_len_max,
                                const uint8_t * const p_mac_tlv);

/**********************************************************************************************************************
* Function Name : R_SB_CheckIntegrity
**********************************************************************************************************************/
/*!****************************************************************************************************************//**
 * \ingroup     SBLIBSecureBootAPIs
 * \brief       Performs image integrity verification using a certificate chain,
 *              If the verification is successful, generate a MAC for the image and Code Certificate
 * \param       [in]   p_key_cert           Specify the start address of Key Certificate \n
 *                                          Data must be aligned on 4-byte boundaries
 * \param       [in]   key_cert_len_max     Maximum byte length that Key Certificate can take \n
 *                                          Don't care when using secure boot with MAC. \n
 *                                          [range] #SB_MANIFEST_LEN_MIN - #SB_MANIFEST_LEN_MAX
 * \param       [in]   p_code_cert          Specify the start address of Code Certificate \n
 *                                          Data must be aligned on 4-byte boundaries
 * \param       [in]   code_cert_len_max    Maximum byte length that Code Certificate can take \n
 *                                          [range] #SB_MANIFEST_LEN_MIN - #SB_MANIFEST_LEN_MAX
 * \param       [in]   mac_type             Output MAC type \n
 * \param       [out]  p_tag                Specify MAC tag storage address \n
 *                                          Allocate necessary buffer size for the specified type \n
 *                                          Data must be aligned on 4-byte boundaries \n
 *                                          Values are output in big endian \n
 *                                          Specify NULL to not generate MAC
 * \return
 *  - #SB_RET_SUCCESS:                           Successful completion
 *  - #SB_RET_SAME_IMAGE_VERSION:                The same version of the image has been entered \n
 *                                               (Verification and MAC generation completed normally)
 *  - #SB_RET_ERR_INTERNAL_FAIL:                 Internal processing failed
 *  - #SB_RET_ERR_INVALID_ARG:                   Invalid argument entered
 *  - #SB_RET_ERR_UNSUPPORTED_FUNCTION:          Unsupported function executed
 *  - #SB_RET_ERR_INVALID_ALIGNMENT:             Data entered with incorrect alignment
 *  - #SB_RET_ERR_MANI_INVALID_MAGIC:            Incorrect magic number set in manifest
 *  - #SB_RET_ERR_MANI_UNSUPPORTED_VERSION:      An unsupported version is set in the manifest
 *  - #SB_RET_ERR_MANI_OUT_OF_RANGE_LEN:         Manifest size out of range
 *  - #SB_RET_ERR_MANI_TLV_FIELD_ERR:            Missing required TLV fields or
 *                                               One or more TLV fields for selection are set
 *  - #SB_RET_ERR_MANI_TLV_INVALID_LEN:          The length of the TLV field has a size that exceeds the end of
 *                                               the manifest.
 *  - #SB_RET_ERR_MANI_INVALID_IMAGE_LEN:        An invalid image length is set
 *  - #SB_RET_ERR_MANI_MISMATCH_SIGN_ALGORITHM:  There is a wrong combination of signature algorithms
 *  - #SB_RET_ERR_MANI_UNSUPPORTED_ALGORITHM:    An algorithm was specified that the manifest does not support
 *  - #SB_RET_ERR_CRYPTO_FAIL:                   Encryption failed
 *  - #SB_RET_ERR_CRYPTO_AUTH_FAIL:              Verification failed
 *  - #SB_RET_ERR_CRYPTO_UNSUPPORTED_ALGORITHM:  Unsupported algorithm
 *  - #SB_RET_ERR_CRYPTO_PARAM_ERR:              Parameter error
 *  - #SB_RET_ERR_LOWER_IMAGE_VERSION:           An image version lower than the current image version is installed
 * \par Global Variables
 *  - Image data    [in]    Binary data of the image to be verified \n
 *                          Data must be aligned on 4-byte boundaries
 * \par Call SB-Driver API
 *  - R_CIP_DRV_PrcDeriveMacKeyFromHuk (This API is actually called by #r_sb_sb_check_integrity)
 *  - R_CIP_DRV_PrcCheckIntegrity (This API is actually called by #r_sb_sb_check_integrity)
 *  - R_CIP_DRV_CheckImageVersion (This API is actually called by #r_sb_sb_check_integrity)
 * \par Precondition
 *  - Cryptographic IP initialization is complete.
 *  - The target boot loader program has been copied to the Dest Addr field of the Code Certificate Header.
 * \par Security components
 *  - yes
 * \par Detailed description
 *      - The TLV field of Key Certificate and Code Certificate referenced by API is described below.
 *| Type           | Required/Choose/\n Optional| TLV Type(Class) | TLV Type(Use Type)| Description                             |
 *|:---------------|:---------------------------|:----------------|:------------------|:----------------------------------------|
 *|Key Certificate |Choose (Use Type)           |Key              |OEM root public key|Public key for signature verification of Key Certificate \n \<When verifying using RoT\> \n Set "OEM root public key" to Use type \n \<When verifying using the previous IMG PK\> \n Set Use type to "Image public key"|
 *|^               |^                           |^                |Image public key   |^                                        |
 *|^               |Required                    |Hash             |Image public key Hash|Hash value of public key for signature verification of Code Certificate|
 *|^               |Required                    |Signature        |Key Certificate signature|Signature value calculated for Key Certificate|
 *|Code Certificate|Required                    |Key              |Image public key|Public key for signature verification of Code Certificate|
 *|^               |Required                    |Signature        |Code Certificate + \n Image Signature|Signature value calculated for Code Certificate and image|
 *|^               |Optional                    |Image Cipher info|Image Encryption/Decryption Information|Image encryption information \n (Required when using Image Cipher and using a key other than the default value)|
 *|^               |Optional                    |IV               |Image Cipher IV    |IV used in decoding the image \n (Required when Image Cipher info IV select is "IV stored in Code Certificate")|
 *|^               |Optional                    |Image Cipher info|Tenporary Image Decryption Information|Decryption information for temporarily encrypted images \n (Required when using Image Cipher and using a value other than the device default for decryption information)|
 *|^               |Optional                    |IV               |Tenporary Image Decryption IV|IV used in decrypting temporarily encrypted images|
 * \par
 *      - Returns an error if the set TLV is in the following cases
 *        - "Required" TLV is not set
 *        - "Choose" TLV is not set
 *        - Two or more "Choose" TLVs are set
 *      - When multiple TLVs with the same Class and Use type are set,
 *        use the TLV found earlier when parsing from the beginning of the TLV field.
 *      - In MAC calculation in MAC generation, input data in the following order
 *        -# Code Certificate
 *        -# Image
 *      - In hash calculation for signature verification of Code Certificate in certificate chain verification,
 *        input data in the following order
 *        -# Code Certificate
 *        -# Image
 *      - Use a key derived from HUK for MAC verification \n This key is generated by Crypto IP
 *      - If the encryption process of the image in the Code Certificate \> Header \> Flags field is "1",
 *        encrypt the image after verifying the certificate chain \n
 *        In this case, MAC generation is performed on the encrypted image
 *      - If the decryption processing of the temporarily encrypted image in the
 *        Code Certificate \> Header \> Flags field is "1",
 *        decrypt the temporary encrypted image before verifying the certificate chain
 *      - Integrity verification process is only compiled if build configuration #SB_CFG_CHECK_INTEGRITY is enabled
 *      - Compare image versions \n
 *        If the image version stored in the anti-rollback counter is lower than the Image version set
 *        in the Code Certificate Header, return #SB_RET_ERR_LOWER_IMAGE_VERSION \n
 *        Returns #SB_RET_SAME_IMAGE_VERSION if they are the same
 *
 * Pseudo-code for the function can be found below.
 * -# <B>--- If #SB_CFG_CHECK_INTEGRITY is enabled ---</B>
 * -# Initialize the variable ret with #SB_RET_ERR_INTERNAL_FAIL
 * -# Set #SB_PRV_FALSE to flow_correct_ret
 * -# Set the initial value for the flow counter with #r_sb_cmn_fc_ci_init
 * -# If (NULL != p_key_cert) && (NULL != p_code_cert) && (NULL != p_tag)
 *   -# If (((uintptr_t)p_key_cert & #SB_PRV_REMAINDER_DIV4) == 0UL) &&
 *         (((uintptr_t)p_code_cert & #SB_PRV_REMAINDER_DIV4) == 0UL) &&
 *         (((uintptr_t)p_tag & #SB_PRV_REMAINDER_DIV4) == 0UL)
 *     -# Set Key cert structure with #r_sb_mani_set_key_cert_st
 *     -# If #SB_RET_SUCCESS == ret
 *       -# To clear ret, set ret to #SB_RET_ERR_INTERNAL_FAIL
 *       -# Check Key cert with #r_sb_mani_chk_key_cert
 *       -# If #SB_RET_SUCCESS == ret
 *         -# To clear ret, set ret to #SB_RET_ERR_INTERNAL_FAIL
 *         -# Set code cert structure with #r_sb_mani_set_code_cert_st
 *         -# If #SB_RET_SUCCESS == ret
 *           -# To clear ret, set ret to #SB_RET_ERR_INTERNAL_FAIL
 *           -# Check code cert with #r_sb_mani_chk_code_cert
 *           -# If #SB_RET_SUCCESS == ret
 *             -# To clear ret, set ret to #SB_RET_ERR_INTERNAL_FAIL
 *             -# Check Integrity with #r_sb_sb_check_integrity
 *           -# Else
 *             -# Do nothing
 *         -# Else
 *           -# Do nothing
 *       -# Else
 *         -# Do nothing
 *     -# Else
 *       -# Do nothing
 *   -# Else
 *     -# Set #SB_RET_ERR_INVALID_ALIGNMENT to ret
 * -# Else
 *   -# Set #SB_RET_ERR_INVALID_ARG to ret
 * -# Check if the CheckIntegrity flow is correct using #r_sb_cmn_fc_is_ci_flow_correct
 * -# Set function result to flow_correct_ret
 * -# If (#SB_RET_SUCCESS == ret) || (#SB_RET_SAME_IMAGE_VERSION == ret)
 *   -# If #SB_PRV_TRUE == flow_correct_ret
 *     -# Do nothing
 *   -# Else
 *     -# Set #SB_RET_ERR_INTERNAL_FAIL to ret
 * -# Else
 *   -# Do nothing
 * -# Return ret
 * -# <B>--- Else #SB_CFG_CHECK_INTEGRITY is enabled ---</B>
 * -# Return #SB_RET_ERR_UNSUPPORTED_FUNCTION
 * -# <B>--- Endif #SB_CFG_CHECK_INTEGRITY is enabled ---</B>
 *
 * Note: The call graph below dose not include SB-Driver APIs.
 * \callgraph
 *********************************************************************************************************************/
extern sb_ret_t R_SB_CheckIntegrity (const uint8_t * const p_key_cert,
                                    const uint32_t key_cert_len_max,
                                    const uint8_t * const p_code_cert,
                                    const uint32_t code_cert_len_max,
                                    const e_sb_mac_type_t mac_type,
                                    uint32_t* const p_tag);

/**********************************************************************************************************************
* Function Name : R_SB_CheckCRC
**********************************************************************************************************************/
/*!****************************************************************************************************************//**
 * \ingroup     SBLIBCRCAPIs
 * \brief       Check image CRC
 * \param       [in]   p_code_cert          Start address of Code Certificate \n
 *                                          Data must be aligned on 4-byte boundaries
 * \param       [in]   code_cert_len_max    Maximum byte length that Code Certificate can take \n
 *                                          [range] #SB_MANIFEST_LEN_MIN - #SB_MANIFEST_LEN_MAX
 * \return
 *  - #SB_RET_SUCCESS:                         Successful completion
 *  - #SB_RET_ERR_INTERNAL_FAIL:               Internal processing failed
 *  - #SB_RET_ERR_INVALID_ARG:                 Invalid argument entered
 *  - #SB_RET_ERR_UNSUPPORTED_FUNCTION:        Unsupported function executed
 *  - #SB_RET_ERR_INVALID_ALIGNMENT:           Data entered with incorrect alignment
 *  - #SB_RET_ERR_MANI_INVALID_MAGIC:          Incorrect magic number is set
 *  - #SB_RET_ERR_MANI_UNSUPPORTED_VERSION:    Unsupported version is set
 *  - #SB_RET_ERR_MANI_OUT_OF_RANGE_LEN:       TLV Length out of range is set
 *  - #SB_RET_ERR_MANI_TLV_FIELD_ERR:          Missing required TLV fields
 *  - #SB_RET_ERR_MANI_TLV_INVALID_LEN:        The size exceeding the end of the manifest is specified in
 *                                             the Length of the TLV field.
 *  - #SB_RET_ERR_MANI_INVALID_IMAGE_LEN:      An invalid image length is set
 *  - #SB_RET_ERR_CRC_MISMATCH:                CRC mismatch
 *  - #SB_RET_ERR_MANI_UNSUPPORTED_ALGORITHM:  An algorithm was specified that the manifest does not support
 * \par Global Variables
 *  - Image data    [in]    Binary data of the image to be verified \n
 *                          Data must be aligned on 4-byte boundaries
 * \par Call SB-Driver API
 *  - R_CIP_DRV_PrcCheckCRC (This API is actually called by #r_sb_crc_check_crc)
 * \par Precondition
 *  - The target boot loader program has been copied to the Dest Addr field of the Code Certificate Header.
 * \par Security components
 *  - no
 * \par Detailed description
 *      - The TLV field of Code Certificate referred by API is described below.
 *| Type           | Required/Choose/\n Optional| TLV Type(Class) | TLV Type(Use Type)| Description                   |
 *|:---------------|:---------------------------|:----------------|:------------------|:------------------------------|
 *|Code Certificate|Required                    |CRC              |Image CRC          |Image CRC                      |
 * \par
 *      - Returns an error if the set TLV is in the following cases
 *        - Required TLV is not set
 *      - When multiple TLVs with the same Class and Use type are set,
 *        use the TLV found earlier when parsing from the beginning of the TLV field.
 *      - The type of CRC algorithm is specified by the polynomial field in the Code Certificate.
 *        The correspondence between the type of CRC algorithm and the polynomial field depends on
 *        the SB-Driver specification.
 *      - CRC check process is only compiled if build configuration #SB_CFG_CHECK_CRC is enabled
 *
 * Pseudo-code for the function can be found below.
 * -# <B>--- If #SB_CFG_CHECK_CRC is enabled ---</B>
 * -# Initialize the variable ret with #SB_RET_ERR_INTERNAL_FAIL
 * -# If NULL != p_code_cert
 *   -# If ((uintptr_t)p_code_cert & #SB_PRV_REMAINDER_DIV4) == 0UL
 *     -# Set code cert structure with #r_sb_mani_set_code_cert_st
 *     -# If #SB_RET_SUCCESS == ret
 *       -# To clear ret, set ret to #SB_RET_ERR_INTERNAL_FAIL
 *       -# Check code cert with #r_sb_mani_chk_code_cert
 *       -# If #SB_RET_SUCCESS == ret
 *         -# To clear ret, set ret to #SB_RET_ERR_INTERNAL_FAIL
 *         -# Check image CRC with #r_sb_crc_check_crc
 *       -# Else
 *         -# Do nothing
 *     -# Else
 *       -# Do nothing
 *   -# Else
 *     -# Set #SB_RET_ERR_INVALID_ALIGNMENT to ret
 * -# Else
 *   -# Set #SB_RET_ERR_INVALID_ARG to ret
 * -# Return ret
 * -# <B>--- Else #SB_CFG_CHECK_CRC is enabled ---</B>
 * -# Return #SB_RET_ERR_UNSUPPORTED_FUNCTION
 * -# <B>--- Endif #SB_CFG_CHECK_CRC is enabled ---</B>
 *
 * Note: The call graph below dose not include SB-Driver APIs.
 * \callgraph
 *********************************************************************************************************************/
extern sb_ret_t R_SB_CheckCRC (const uint8_t * const p_code_cert,
                                const uint32_t code_cert_len_max);

/**********************************************************************************************************************
* Function Name : R_SB_GetVersion
**********************************************************************************************************************/
/*!****************************************************************************************************************//**
 * \ingroup     SBLIBCommonAPIs
 * \brief       Get version information of SB-Lib.
 * \param       [out] p_major       An address of major version
 * \param       [out] p_minor       An address of minor version
 * \return
 *  - #SB_RET_SUCCESS:                         Successful completion
 *  - #SB_RET_ERR_INVALID_ARG:                 Invalid argument entered
 * \par Global Variables
 *  - None
 * \par Call SB-Driver API
 *  - None
 * \par Precondition
 *  - None
 * \par Security components
 *  - no
 * \par Detailed description
 *      - Get version information of SB-Lib.
 *
 * Pseudo-code for the function can be found below.
 * -# Initialize the variable ret with #SB_RET_ERR_INTERNAL_FAIL
 * -# If (p_major != NULL) && (p_minor != NULL)
 *   -# Set #SB_PRV_VERSION_MAJOR to p_major
 *   -# Set #SB_PRV_VERSION_MINOR to p_minor
 *   -# Set #SB_RET_SUCCESS to ret
 * -# Else
 *   -# Set #SB_RET_ERR_INVALID_ARG to ret
 * -# Return ret
 *
 * \callgraph
 *********************************************************************************************************************/
extern sb_ret_t R_SB_GetVersion (uint32_t * const p_major, uint32_t * const p_minor);

/**********************************************************************************************************************
* Function Name : R_SB_ParseManiTLVs
**********************************************************************************************************************/
/*!****************************************************************************************************************//**
 * \ingroup     SBLIBCommonAPIs
 * \brief       Search TLV of specified type from TLV field
 * \param       [in]   p_top                   Start address of TLV field
 * \param       [in]   tlv_len                 TLV field length
 * \param       [in]   num_of_search_type      The number of elements in the argument search_types
 * \param       [in]   search_types            Start address of search TLV type \n * Set the same number of search
 *                                             targets as the number specified in num_of_search_type
 * \param       [out]  tlvs                    Start address of the structure that stores the parsed TLV \n * Allocate
 *                                             the same number of values as the number specified by num_of_search_type
 * \return
 *  - #SB_RET_SUCCESS:                          Successful completion
 *  - #SB_RET_ERR_INTERNAL_FAIL:                Internal processing failed
 *  - #SB_RET_ERR_MANI_TLV_INVALID_LEN:         The length of the TLV field has a size that exceeds the end of
 *                                              the manifest.
 * \par Global Variables
 *  - None
 * \par Call SB-Driver API
 *  - None
 * \par Precondition
 *  - None
 * \par Security components
 *  - yes
 * \par Detailed description
 *      - Search TLV of specified type from TLV field
 *      - If the target type in search_types[k] (k is an integer between 1 and num_of_search_type-1 inclusive)
 *        is found, set tlvs[k] to the value
 *      - If the target type is not found, set NULL to p_val member of tlvs
 *      - Terminate search when all search targets are found or parsed to end of TLV field
 *      - Return #SB_RET_SUCCESS even if there is no target
 *      - If the end of the TLV field is exceeded during parsing, return #SB_RET_ERR_MANI_TLV_INVALID_LEN
 *
 * Pseudo-code for the function can be found below.
 * -# Initialize the variable ret with #SB_RET_ERR_INTERNAL_FAIL
 * -# Get TLVs of the type specified by search_types, set tlvs to the result with #r_sb_mani_parse_tlvs,
 *    and set ret to the return value
 * -# Return ret
 *
 * \callgraph
 *********************************************************************************************************************/
sb_ret_t R_SB_ParseManiTLVs (const uint8_t * const p_top, const uint32_t tlv_len, const uint32_t num_of_search_type,
                                const st_sb_search_tlv_type_t search_types[], st_sb_tlv_t tlvs[]);

/**********************************************************************************************************************
* Function Name : R_SB_GetCodeCertInfo
**********************************************************************************************************************/
/*!****************************************************************************************************************//**
 * \ingroup     SBLIBCommonAPIs
 * \brief       Set value in code certificate structure
 * \param       [in]   p_code_cert          Start address of Code Certificate
 * \param       [in]   code_cert_len_max    Maximum byte length that Code Certificate can take \n
 *                                          [range] #SB_MANIFEST_LEN_MIN - #SB_MANIFEST_LEN_MAX
 * \param       [out]  p_code_cert_st       Start address of Code Certificate Structure
 * \return
 *  - #SB_RET_SUCCESS:                          Successful completion
 *  - #SB_RET_ERR_INTERNAL_FAIL:                Internal processing failed
 *  - #SB_RET_ERR_INVALID_ARG:                  Invalid argument entered
 *  - #SB_RET_ERR_INVALID_ALIGNMENT:            Data entered with incorrect alignment
 *  - #SB_RET_ERR_MANI_INVALID_MAGIC:           Incorrect magic number is set
 *  - #SB_RET_ERR_MANI_UNSUPPORTED_VERSION:     Unsupported version is set
 *  - #SB_RET_ERR_MANI_OUT_OF_RANGE_LEN:        Out of range TLV Length is set
 *  - #SB_RET_ERR_MANI_INVALID_IMAGE_LEN:       An invalid image length is set
 * \par Global Variables
 *  - None
 * \par Call SB-Driver API
 *  - None
 * \par Precondition
 *  - None
 * \par Security components
 *  - yes
 * \par Detailed description
 *      - Set value in code certificate structure
 *      - Set the starting address of the header of the code certificate, the length of the TLV,
 *        and the starting address of the TLV
 *
 * Pseudo-code for the function can be found below.
 * -# Initialize the variable ret with #SB_RET_ERR_INTERNAL_FAIL
 * -# If (NULL != p_code_cert) && (NULL != p_code_cert_st)
 *   -# If (((uintptr_t)p_code_cert & (uintptr_t)#SB_PRV_REMAINDER_DIV4) == (uintptr_t)0UL)
 *     -# Get information of code certificate in p_code_cert,
 *        and code certificate structure p_code_cert_st with #r_sb_mani_set_code_cert_st
 *     -# Set ret to the return value
 *     -# If #SB_RET_SUCCESS == ret
 *       -# To clear ret, set ret to #SB_RET_ERR_INTERNAL_FAIL
 *       -# Check the code certificate with #r_sb_mani_chk_code_cert
 *     -# Else
 *       -# Do nothing
 *   -# Else
 *     -# Set ret to #SB_RET_ERR_INVALID_ALIGNMENT.
 * -# Else
 *   -# Set ret to #SB_RET_ERR_INVALID_ARG
 * -# Return ret
 *
 * \callgraph
 *********************************************************************************************************************/
sb_ret_t R_SB_GetCodeCertInfo (const uint8_t * const p_code_cert, const uint32_t code_cert_len_max,
                            st_sb_code_cert_t * const p_code_cert_st);

#endif /* R_SB_API_H */
/*=====================================================================================================================
 End of File
=====================================================================================================================*/
