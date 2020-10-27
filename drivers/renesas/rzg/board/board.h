/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights
 * reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BOARD_H
#define BOARD_H

#define BOARD_EK874			(0x00U)
#define BOARD_HIHOPE_RZG2M		(0x01U)
#define BOARD_HIHOPE_RZG2N		(0x02U)
#define BOARD_HIHOPE_RZG2H		(0x03U)
#define BOARD_UNKNOWN			(BOARD_HIHOPE_RZG2H + 1U)

#define BOARD_REV_UNKNOWN		(0xFF)

extern const char *g_board_tbl[];

/************************************************************************
 * Revisions are expressed in 8 bits.
 *  The upper 4 bits are major version.
 *  The lower 4 bits are minor version.
 ************************************************************************/
#define GET_BOARD_MAJOR(a)	((uint32_t)(a) >> 0x4)
#define GET_BOARD_MINOR(a)	((uint32_t)(a) &  0xF)
#define GET_BOARD_NAME(a)	(g_board_tbl[(a)])

int32_t rzg_get_board_type(uint32_t *type, uint32_t *rev);

#endif /* BOARD_H */
