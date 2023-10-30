/*
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
* System Name  : SDHI Driver
* File Name    : sys_sel.h
* Version      : 1.30
******************************************************************************/
#ifndef SYS_SEL_H
#define SYS_SEL_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
/* target configration */
#define TARGET_RZ_MPU

#if defined(TARGET_RZ_MPU)
#define DRIVER_NAME             "RENESAS RZ SD Driver Ver1.00"
#else
	error"!!"
#endif

/* ==== number of ports ==== */
#define NUM_PORT                (1)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/


#endif /* SYS_SEL_H */

/* End of File */
