/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights
 * reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

#include <lib/utils_def.h>

#include <iic_dvfs.h>
#include <lib/mmio.h>
#include "rzg_def.h"

#include "board.h"

#ifndef BOARD_DEFAULT
#if (RZG_LSI == RZG_G2E)
#define BOARD_DEFAULT		(BOARD_EK874 << BOARD_CODE_SHIFT)
#elif (RZG_LSI == RZG_G2N)
#define BOARD_DEFAULT		(BOARD_HIHOPE_RZG2N << BOARD_CODE_SHIFT)
#elif (RZG_LSI == RZG_G2H)
#define BOARD_DEFAULT		(BOARD_HIHOPE_RZG2H << BOARD_CODE_SHIFT)
#else
#define BOARD_DEFAULT		(BOARD_HIHOPE_RZG2M << BOARD_CODE_SHIFT)
#endif
#endif

#define BOARD_CODE_MASK		(0xF8)
#define BOARD_REV_MASK		(0x07)
#define BOARD_CODE_SHIFT	(0x03)
#define BOARD_ID_UNKNOWN	(0xFF)


#define	GPIO_INDT5		0xE605500C
#define	GP5_19_BIT		(0x01 << 19)
#define	GP5_21_BIT		(0x01 << 21)
#define	GP5_25_BIT		(0x01 << 25)

#define HM_ID	{ 0x10U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU }
#define HN_ID	{ 0x20U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU }
#define HH_ID	{ 0x40U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU }
#define EK_ID	{ 0x10U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU }

#if (RZG_LSI == RZG_G2E)
extern char ek874_board_rev;
#endif /* RZG_LSI == RZG_G2E */

const char *g_board_tbl[] = {
	[BOARD_HIHOPE_RZG2M]	= "HiHope RZ/G2M",
	[BOARD_HIHOPE_RZG2N]	= "HiHope RZ/G2N",
	[BOARD_HIHOPE_RZG2H]	= "HiHope RZ/G2H",
	[BOARD_EK874]		= "EK874 RZ/G2E",
	[BOARD_UNKNOWN]		= "unknown"
};

int32_t rzg_get_board_type(uint32_t *type, uint32_t *rev)
{
	int32_t ret = 0;
	const uint8_t board_tbl[][8] = {
		[BOARD_HIHOPE_RZG2M] = HM_ID,
		[BOARD_HIHOPE_RZG2N] = HN_ID,
		[BOARD_HIHOPE_RZG2H] = HH_ID,
		[BOARD_EK874] = EK_ID,
	};
	static uint8_t board_id = BOARD_ID_UNKNOWN;
#if (RZG_LSI == RZG_G2N) | (RZG_LSI == RZG_G2M)
	uint32_t reg, boardInfo, read_rev;
#elif (RZG_LSI == RZG_G2H)
	uint32_t boardInfo;
#elif (RZG_LSI != RZG_G2E)
	uint32_t read_rev;
#endif

	if (board_id != BOARD_ID_UNKNOWN)
		goto get_type;

	board_id = BOARD_DEFAULT;

get_type:
	*type = ((uint32_t) board_id & BOARD_CODE_MASK) >> BOARD_CODE_SHIFT;

	if (*type >= ARRAY_SIZE(board_tbl)) {
		/* no revision information, set Rev0.0. */
		*rev = 0;
		return ret;
	}

#if (RZG_LSI == RZG_G2M)
	reg = mmio_read_32(RZG_PRR);
	if (RZG_G2M_CUT_VER11 == (reg & PRR_CUT_MASK))
	{
		read_rev = (uint8_t)(board_id & BOARD_REV_MASK);
		*rev = board_tbl[*type][read_rev];
	}
	else
	{
		boardInfo = mmio_read_32(GPIO_INDT5) & (GP5_19_BIT |GP5_21_BIT);
		*rev = (((boardInfo & GP5_19_BIT) >> 14) | ((boardInfo & GP5_21_BIT) >> 17)) + 0x30;
	}
#elif (RZG_LSI == RZG_G2N)
	reg = mmio_read_32(GPIO_INDT5);
	if (reg & GP5_25_BIT)
	{
		read_rev = (uint8_t)(board_id & BOARD_REV_MASK);
		*rev = board_tbl[*type][read_rev];
	}
	else
	{
		boardInfo = reg & (GP5_19_BIT |GP5_21_BIT);
		*rev = (((boardInfo & GP5_19_BIT) >> 14) | ((boardInfo & GP5_21_BIT) >> 17)) + 0x30;
	}
#elif (RZG_LSI == RZG_G2H)
	boardInfo = mmio_read_32(GPIO_INDT5) & (GP5_19_BIT |GP5_21_BIT);
	*rev = (((boardInfo & GP5_19_BIT) >> 14) | ((boardInfo & GP5_21_BIT) >> 17)) + 0x30;
#elif (RZG_LSI == RZG_G2E)
	*rev = ek874_board_rev;
#else
	read_rev = (uint8_t)(board_id & BOARD_REV_MASK);
	*rev = board_tbl[*type][read_rev];
#endif
	return ret;
}
