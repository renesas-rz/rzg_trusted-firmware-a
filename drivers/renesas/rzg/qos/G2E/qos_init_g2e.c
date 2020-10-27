/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

#include <common/debug.h>

#include "../qos_common.h"
#include "../qos_reg.h"
#include "qos_init_g2e.h"

#define	RZG_QOS_VERSION		"rev.0.05"

#define REF_ARS_ARBSTOPCYCLE_G2E	(((SL_INIT_SSLOTCLK_G2E) - 5U) << 16U)

#if RZG_QOS_TYPE  == RZG_QOS_TYPE_DEFAULT

#if RZG_REF_INT == RZG_REF_DEFAULT
#include "qos_init_g2e_mstat390.h"
#else
#include "qos_init_g2e_mstat780.h"
#endif

#endif

struct rzg_gen2_dbsc_qos_settings g2e_qos[] = {
	/* BUFCAM settings */
	{ DBSC_DBCAM0CNF1, 0x00043218 },
	{ DBSC_DBCAM0CNF2, 0x000000F4 },
	{ DBSC_DBSCHCNT0, 0x000F0037 },
	{ DBSC_DBSCHSZ0, 0x00000001 },
	{ DBSC_DBSCHRW0, 0x22421111 },

	/* DDR3 */
	{ DBSC_SCFCTST2, 0x012F1123 },

	/* QoS Settings */
	{ DBSC_DBSCHQOS00, 0x00000F00 },
	{ DBSC_DBSCHQOS01, 0x00000B00 },
	{ DBSC_DBSCHQOS02, 0x00000000 },
	{ DBSC_DBSCHQOS03, 0x00000000 },
	{ DBSC_DBSCHQOS40, 0x00000300 },
	{ DBSC_DBSCHQOS41, 0x000002F0 },
	{ DBSC_DBSCHQOS42, 0x00000200 },
	{ DBSC_DBSCHQOS43, 0x00000100 },
	{ DBSC_DBSCHQOS90, 0x00000100 },
	{ DBSC_DBSCHQOS91, 0x000000F0 },
	{ DBSC_DBSCHQOS92, 0x000000A0 },
	{ DBSC_DBSCHQOS93, 0x00000040 },
	{ DBSC_DBSCHQOS130, 0x00000100 },
	{ DBSC_DBSCHQOS131, 0x000000F0 },
	{ DBSC_DBSCHQOS132, 0x000000A0 },
	{ DBSC_DBSCHQOS133, 0x00000040 },
	{ DBSC_DBSCHQOS140, 0x000000C0 },
	{ DBSC_DBSCHQOS141, 0x000000B0 },
	{ DBSC_DBSCHQOS142, 0x00000080 },
	{ DBSC_DBSCHQOS143, 0x00000040 },
	{ DBSC_DBSCHQOS150, 0x00000040 },
	{ DBSC_DBSCHQOS151, 0x00000030 },
	{ DBSC_DBSCHQOS152, 0x00000020 },
	{ DBSC_DBSCHQOS153, 0x00000010 },
};

void qos_init_g2e(void)
{
	rzg_qos_dbsc_setting(g2e_qos, ARRAY_SIZE(g2e_qos), true);

	/* DRAM Split Address mapping */
#if RZG_DRAM_SPLIT == RZG_DRAM_SPLIT_4CH
#if RZG_LSI == RZG_G2E
#error "Don't set DRAM Split 4ch(G2E)"
#else
	ERROR("DRAM Split 4ch not supported.(G2E)");
	panic();
#endif
#elif (RZG_DRAM_SPLIT == RZG_DRAM_SPLIT_2CH)
#if RZG_LSI == RZG_G2E
#error "Don't set DRAM Split 2ch(G2E)"
#else
	ERROR("DRAM Split 2ch not supported.(G2E)");
	panic();
#endif
#else
	NOTICE("BL2: DRAM Split is OFF\n");
#endif

#if !(RZG_QOS_TYPE == RZG_QOS_NONE)
#if RZG_QOS_TYPE  == RZG_QOS_TYPE_DEFAULT
	NOTICE("BL2: QoS is default setting(%s)\n", RZG_QOS_VERSION);
#endif

#if RZG_REF_INT == RZG_REF_DEFAULT
	NOTICE("BL2: DRAM refresh interval 3.9 usec\n");
#else
	NOTICE("BL2: DRAM refresh interval 7.8 usec\n");
#endif

	io_write_32(QOSCTRL_RAS, 0x00000020U);
	io_write_64(QOSCTRL_DANN, 0x0404020002020201UL);
	io_write_32(QOSCTRL_DANT, 0x00100804U);
	io_write_32(QOSCTRL_FSS, 0x0000000AU);
	io_write_32(QOSCTRL_INSFC, 0x06330001U);
	io_write_32(QOSCTRL_EARLYR, 0x00000000U);
	io_write_32(QOSCTRL_RACNT0, 0x00010003U);

	io_write_32(QOSCTRL_SL_INIT,
		    SL_INIT_REFFSSLOT | SL_INIT_SLOTSSLOT |
		    SL_INIT_SSLOTCLK_G2E);
	io_write_32(QOSCTRL_REF_ARS, REF_ARS_ARBSTOPCYCLE_G2E);

	/* QOSBW SRAM setting */
	uint32_t i;

	for (i = 0U; i < ARRAY_SIZE(mstat_fix); i++) {
		io_write_64(QOSBW_FIX_QOS_BANK0 + i * 8, mstat_fix[i]);
		io_write_64(QOSBW_FIX_QOS_BANK1 + i * 8, mstat_fix[i]);
	}
	for (i = 0U; i < ARRAY_SIZE(mstat_be); i++) {
		io_write_64(QOSBW_BE_QOS_BANK0 + i * 8, mstat_be[i]);
		io_write_64(QOSBW_BE_QOS_BANK1 + i * 8, mstat_be[i]);
	}

	/* RT bus Leaf setting */
	io_write_32(RT_ACT0, 0x00000000U);
	io_write_32(RT_ACT1, 0x00000000U);

	/* CCI bus Leaf setting */
	io_write_32(CPU_ACT0, 0x00000003U);
	io_write_32(CPU_ACT1, 0x00000003U);

	io_write_32(QOSCTRL_RAEN, 0x00000001U);

	io_write_32(QOSCTRL_STATQC, 0x00000001U);
#else
	NOTICE("BL2: QoS is None\n");

	io_write_32(QOSCTRL_RAEN, 0x00000001U);
#endif
}
