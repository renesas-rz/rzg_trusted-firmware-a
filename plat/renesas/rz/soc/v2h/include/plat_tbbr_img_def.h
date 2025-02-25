/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PLAT_TBBR_IMG_DEF_H
#define PLAT_TBBR_IMG_DEF_H

#define V2H_DDR_CONFIG_ID	(MAX_IMG_IDS_WITH_SPMDS + 0)

#ifdef MAX_NUMBER_IDS

#undef MAX_NUMBER_IDS
#define MAX_NUMBER_IDS		(MAX_IMG_IDS_WITH_SPMDS + 4)

#endif /* MAX_NUMBER_IDS */

#endif /* PLAT_TBBR_IMG_DEF_H */
