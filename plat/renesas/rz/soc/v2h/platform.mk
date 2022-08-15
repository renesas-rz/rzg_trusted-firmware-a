#
# Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

include plat/renesas/rz/common/v2h_common.mk
include plat/renesas/rz/board/${BOARD}/rz_board.mk

PLAT_INCLUDES	+=	-Iplat/renesas/rz/soc/v2h/include

DDR_SOURCES += plat/renesas/rz/soc/v2h/drivers/ddr/ddr_v2h.c

