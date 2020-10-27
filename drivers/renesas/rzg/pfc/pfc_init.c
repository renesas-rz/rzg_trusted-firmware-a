/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

#include <common/debug.h>
#include <lib/mmio.h>

#include "rzg_def.h"
#if RZG_LSI == RZG_AUTO
#include "G2H/pfc_init_g2h.h"
#include "G2M/pfc_init_g2m.h"
#include "G2N/pfc_init_g2n.h"
#endif
#if RZG_LSI == RZG_G2H	/* G2H */
#include "G2H/pfc_init_g2h.h"
#endif
#if RZG_LSI == RZG_G2M	/* G2M */
#include "G2M/pfc_init_g2m.h"
#endif
#if RZG_LSI == RZG_G2N	/* G2N */
#include "G2N/pfc_init_g2n.h"
#endif
#if RZG_LSI == RZG_G2E	/* G2E */
#include "G2E/pfc_init_g2e.h"
#endif

#define PRR_PRODUCT_ERR(reg)				\
	do {						\
		ERROR("LSI Product ID(PRR=0x%x) PFC initialize not supported.\n", \
			reg);				\
		panic();				\
	} while (0)

#define PRR_CUT_ERR(reg)				\
	do {						\
		ERROR("LSI Cut ID(PRR=0x%x) PFC initialize not supported.\n", \
			reg);				\
		panic();\
	} while (0)

void rzg_pfc_init(void)
{
	uint32_t reg;

	reg = mmio_read_32(RZG_PRR);
#if RZG_LSI == RZG_AUTO
	switch (reg & PRR_PRODUCT_MASK) {
	case PRR_PRODUCT_G2H:
		pfc_init_g2h();
		break;
	case PRR_PRODUCT_G2M:
		pfc_init_g2m();
		break;
	case PRR_PRODUCT_G2N:
		pfc_init_g2n();
		break;
	default:
		PRR_PRODUCT_ERR(reg);
		break;
	}
#else
#if RZG_LSI == RZG_G2H	/* G2H */
	if ((PRR_PRODUCT_G2H) != (reg & PRR_PRODUCT_MASK)) {
		PRR_PRODUCT_ERR(reg);
	}
	pfc_init_g2h();
#elif RZG_LSI == RZG_G2M	/* G2M */
	if ((PRR_PRODUCT_G2M) != (reg & PRR_PRODUCT_MASK)) {
		PRR_PRODUCT_ERR(reg);
	}
	pfc_init_g2m();
#elif RZG_LSI == RZG_G2N	/* G2N */
	if ((PRR_PRODUCT_G2N) != (reg & PRR_PRODUCT_MASK)) {
		PRR_PRODUCT_ERR(reg);
	}
	pfc_init_g2n();
#elif RZG_LSI == RZG_G2E	/* G2E */
	if ((PRR_PRODUCT_G2E) != (reg & PRR_PRODUCT_MASK)) {
		PRR_PRODUCT_ERR(reg);
	}
	pfc_init_g2e();
#else
#error "Don't have PFC initialize routine(unknown)."
#endif
#endif
}
