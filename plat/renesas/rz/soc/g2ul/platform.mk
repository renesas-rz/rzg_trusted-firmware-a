#
# Copyright (c) 2021, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

include plat/renesas/rz/common/rz_common.mk
include plat/renesas/rz/board/${BOARD}/rz_board.mk

PLAT_INCLUDES			+=	-Iplat/renesas/rz/soc/g2ul/include

DDR_SOURCES				+=	plat/renesas/rz/soc/g2ul/drivers/ddr/ddr_g2ul.c

PLAT_BL_COMMON_SOURCES	+=	${DDR_SOURCES}

RZG2UL := 1
DEVICE_TYPE := 1
ifeq (${SOC_TYPE},2)
DEVICE_TYPE := 2
endif

$(eval $(call add_define,RZG2UL))
$(eval $(call add_define,DEVICE_TYPE))
