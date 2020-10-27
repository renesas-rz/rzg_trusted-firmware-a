/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

#include <common/debug.h>
#include <lib/mmio.h>

#include "qos_init.h"
#include "qos_common.h"
#include "qos_reg.h"
#include "rzg_def.h"
#if RZG_LSI == RZG_AUTO
#include "G2M/qos_init_g2m_v11.h"
#include "G2M/qos_init_g2m_v30.h"
#include "G2N/qos_init_g2n.h"
#include "G2H/qos_init_g2h.h"
#endif
#if RZG_LSI == RZG_G2H		/* G2H */
#include "G2H/qos_init_g2h.h"
#endif
#if RZG_LSI == RZG_G2M		/* G2M */
#include "G2M/qos_init_g2m_v11.h"
#include "G2M/qos_init_g2m_v30.h"
#endif
#if RZG_LSI == RZG_G2N		/* G2N */
#include "G2N/qos_init_g2n.h"
#endif
#if RZG_LSI == RZG_G2E		/* G2E */
#include "G2E/qos_init_g2e.h"
#endif

#if (RZG_LSI != RZG_G2E)

#define DRAM_CH_CNT			0x04
uint32_t qos_init_ddr_ch;
uint8_t qos_init_ddr_phyvalid;
#endif

#define PRR_PRODUCT_ERR(reg)				\
	do {						\
		ERROR("LSI Product ID(PRR=0x%x) QoS "	\
		"initialize not supported.\n", reg);	\
		panic();				\
	} while (0)

#define PRR_CUT_ERR(reg)				\
	do {						\
		ERROR("LSI Cut ID(PRR=0x%x) QoS "	\
		"initialize not supported.\n", reg);	\
		panic();				\
	} while (0)

void rzg_qos_init(void)
{
	uint32_t reg;
#if (RZG_LSI != RZG_G2E)
	uint32_t i;

	qos_init_ddr_ch = 0;
	qos_init_ddr_phyvalid = get_boardcnf_phyvalid();
	for (i = 0; i < DRAM_CH_CNT; i++) {
		if ((qos_init_ddr_phyvalid & (1 << i))) {
			qos_init_ddr_ch++;
		}
	}
#endif

	reg = mmio_read_32(PRR);
#if (RZG_LSI == RZG_AUTO) || RZG_LSI_CUT_COMPAT
	switch (reg & PRR_PRODUCT_MASK) {
	case PRR_PRODUCT_G2H:
#if (RZG_LSI == RZG_AUTO) || (RZG_LSI == RZG_G2H)
		qos_init_g2h();
#else
		PRR_PRODUCT_ERR(reg);
#endif
		break;
	case PRR_PRODUCT_G2M:
#if (RZG_LSI == RZG_AUTO) || (RZG_LSI == RZG_G2M)
		switch (reg & PRR_CUT_MASK) {
		case PRR_PRODUCT_20: /* G2M Cut 11 */
			qos_init_g2m_v11();
			break;
		case PRR_PRODUCT_21: /* G2M Cut 13 */
			qos_init_g2m_v11();
			break;
		case PRR_PRODUCT_30: /* G2M Cut 30 */
		default:
			qos_init_g2m_v30();
			break;
		}
#else
		PRR_PRODUCT_ERR(reg);
#endif
		break;
	case PRR_PRODUCT_G2N:
#if (RZG_LSI == RZG_AUTO) || (RZG_LSI == RZG_G2N)
		qos_init_g2n();
#else
		PRR_PRODUCT_ERR(reg);
#endif
		break;
	case PRR_PRODUCT_G2E:
#if (RZG_LSI == RZG_G2E)
		qos_init_g2e();
#else
		PRR_PRODUCT_ERR(reg);
#endif
		break;
	default:
		PRR_PRODUCT_ERR(reg);
		break;
	}
#else
#if RZG_LSI == RZG_G2H		/* G2H */
	if ((PRR_PRODUCT_G2H)
	    != (reg & (PRR_PRODUCT_MASK))) {
		PRR_PRODUCT_ERR(reg);
	}
	qos_init_g2h();
#elif RZG_LSI == RZG_G2M	/* G2M */
#if RZG_LSI_CUT == RZG_CUT_11
	/* G2M Cut 11 */
	if ((PRR_PRODUCT_G2M | PRR_PRODUCT_20)
	    != (reg & (PRR_PRODUCT_MASK | PRR_CUT_MASK))) {
		PRR_PRODUCT_ERR(reg);
	}
	qos_init_g2m_v11();
#elif RZG_LSI_CUT == RZG_CUT_13
	/* G2M Cut 13 */
	if ((PRR_PRODUCT_G2M | PRR_PRODUCT_21)
	    != (reg & (PRR_PRODUCT_MASK | PRR_CUT_MASK))) {
		PRR_PRODUCT_ERR(reg);
	}
	qos_init_g2m_v11();
#else
	/* G2M Cut 30 or later */
	if ((PRR_PRODUCT_G2M)
	    != (reg & (PRR_PRODUCT_MASK))) {
		PRR_PRODUCT_ERR(reg);
	}
	qos_init_g2m_v30();
#endif
#elif RZG_LSI == RZG_G2N	/* G2N */
	if ((PRR_PRODUCT_G2N)
	    != (reg & (PRR_PRODUCT_MASK))) {
		PRR_PRODUCT_ERR(reg);
	}
	qos_init_g2n();
#elif RZG_LSI == RZG_G2E	/* G2E */
	if ((PRR_PRODUCT_G2E)
	    != (reg & (PRR_PRODUCT_MASK))) {
		PRR_PRODUCT_ERR(reg);
	}
	qos_init_g2e();
#else
#error "Don't have QoS initialize routine(Unknown chip)."
#endif
#endif
}

#if (RZG_LSI != RZG_G2E)
uint32_t get_refperiod(void)
{
	uint32_t refperiod = QOSWT_WTSET0_CYCLE;

#if (RZG_LSI == RZG_AUTO) || RZG_LSI_CUT_COMPAT
	uint32_t reg;

	reg = mmio_read_32(PRR);
	switch (reg & PRR_PRODUCT_MASK) {
#if (RZG_LSI == RZG_G2H)
	case PRR_PRODUCT_G2H:
		refperiod = REFPERIOD_CYCLE;
		break;
#endif
#if (RZG_LSI == RZG_AUTO) || (RZG_LSI == RZG_G2M)
	case PRR_PRODUCT_G2M:
		refperiod = REFPERIOD_CYCLE;
		break;
#endif
#if (RZG_LSI == RZG_AUTO) || (RZG_LSI == RZG_G2N)
	case PRR_PRODUCT_G2N:
		refperiod = REFPERIOD_CYCLE;
		break;
#endif
	default:
		break;
	}
#elif RZG_LSI == RZG_G2H
	/* G2H Cut 30 or later */
	refperiod = REFPERIOD_CYCLE;
#elif RZG_LSI == RZG_G2M
	/* G2M Cut 11 */
	/* G2M Cut 13 */
	/* G2M Cut 30 or later */
	refperiod = REFPERIOD_CYCLE;
#elif RZG_LSI == RZG_G2N
	refperiod = REFPERIOD_CYCLE;
#endif

	return refperiod;
}
#endif

void rzg_qos_dbsc_setting(struct rzg_gen2_dbsc_qos_settings *qos,
			   unsigned int qos_size, bool dbsc_wren)
{
	int i;

	/* Register write enable */
	if (dbsc_wren)
		io_write_32(DBSC_DBSYSCNT0, 0x00001234U);

	for (i = 0; i < qos_size; i++)
		io_write_32(qos[i].reg, qos[i].val);

	/* Register write protect */
	if (dbsc_wren)
		io_write_32(DBSC_DBSYSCNT0, 0x00000000U);
}
