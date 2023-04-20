/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SYS_SEL_H
#define SYS_SEL_H

/* #define SD_HOSTBUS32 1 */


/* ==== Configuration ===== */

	/* 16bits width */


/* #define __SD_DEBUG__*/        /* Debug print on */

/* ==== IP selection ==== */
/* FIFO equipped between CPU bus and SDHI data BUF */
/* #define WITH_FIFO */

/* ==== Number of ports ==== */
#define NUM_PORT (1)

#endif /* SYS_SEL_H */
/* End of File */
