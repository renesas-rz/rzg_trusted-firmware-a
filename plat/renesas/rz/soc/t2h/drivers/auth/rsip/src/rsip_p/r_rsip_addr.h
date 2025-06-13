/*
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef R_RSIP_ADDR_H
#define R_RSIP_ADDR_H

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define RSIP_PRV_ADDR_BASE    ((uintptr_t)gp_sce)

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
extern volatile uint32_t * gp_sce;

#endif                                 /* R_RSIP_ADDR_H */
