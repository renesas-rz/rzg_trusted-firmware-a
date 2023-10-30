/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RZG_SIP_SVC_H__
#define RZG_SIP_SVC_H__

/* Function ID to get ECC mode */
#define RZG_SIP_SVC_GET_ECC_MODE	U(0x8200000F)

/* Rcar SiP Service Calls version numbers */
#define RZG_SIP_SVC_VERSION_MAJOR	U(0x0)
#define RZG_SIP_SVC_VERSION_MINOR	U(0x0)

#define RZG_SMC_RET_SUCCESS		0
#define RZG_SMC_RET_EFAILED		-2
#define RZG_SMC_RET_PMIC_DISABLE	-3

#endif /* RZG_SIP_SVC_H__ */
