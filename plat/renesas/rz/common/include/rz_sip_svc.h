/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RZ_SIP_SVC_H__
#define __RZ_SIP_SVC_H__

/* Function ID to get Device ID */
#define RZ_SIP_SVC_GET_DEVID		U(0x82000010)

/* Function ID to get Chip ID */
#define RZ_SIP_SVC_GET_CHIPID		U(0x82000011)

/* Function ID to get Product ID */
#define RZ_SIP_SVC_GET_PRODUCTID	U(0x82000012)

/* Function ID to set PCIE RST_RSM_B */
#define RZ_SIP_SVC_SET_PCIE_RST_RSMB	U(0x82000013)

/* Function ID to set USB Power Ready. */
#define RZ_SIP_SVC_SET_USB_PWRRDY		U(0x82000014)

/* Function ID to get PCIe register values */
#define RZ_SIP_SVC_GET_SYSPCIE		U(0x82000020)

/* Function ID to set PCIe register values */
#define RZ_SIP_SVC_SET_SYSPCIE		U(0x82000021)

#endif /* __RZ_SIP_SVC_H__ */
