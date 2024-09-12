#
# Copyright (c) 2020-2021, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

include plat/renesas/rz/common/rz_common.mk
include plat/renesas/rz/board/${BOARD}/rz_board.mk

PLAT_INCLUDES			+=	-Iplat/renesas/rz/soc/g2l/include

DDR_SOURCES				+=	plat/renesas/rz/soc/g2l/drivers/ddr/ddr_g2l.c

PLAT_BL_COMMON_SOURCES	+=	${DDR_SOURCES}

.PHONY: bptool_make bptool_clean

bptool: bptool_make
distclean realclean clean: bptool_clean

BPTOOLPATH		?=	tools/renesas/rz_boot_param

bptool_make:
	${Q}${MAKE} TRUSTED_BOARD_BOOT=${TRUSTED_BOARD_BOOT} --no-print-directory -C ${BPTOOLPATH}

bptool_clean:
	${Q}${MAKE} --no-print-directory -C ${BPTOOLPATH} clean
	