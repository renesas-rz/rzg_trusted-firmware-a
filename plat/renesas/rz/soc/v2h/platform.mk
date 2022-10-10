#
# Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

#Set the platform and SOC specific header files 1st
PLAT_INCLUDES	:=	-Iplat/renesas/rz/soc/v2h/include

include plat/renesas/rz/common/v2h_common.mk
include plat/renesas/rz/board/${BOARD}/rz_board.mk

PLAT_BL_COMMON_SOURCES	+=	plat/renesas/rz/soc/v2h/plat_security.c

BL2_SOURCES				+=	plat/renesas/rz/soc/v2h/bl2_plat_setup.c		\
							plat/renesas/rz/soc/v2h/drivers/cpg.c			\
							plat/renesas/rz/soc/v2h/drivers/pfc.c

BL31_SOURCES			+=	plat/renesas/rz/soc/v2h/bl31_plat_setup.c		\
							plat/renesas/rz/soc/v2h/plat_pm.c

XSPI_SOURCE				:=	plat/renesas/rz/soc/v2h/drivers/xspi.c

SPI_MULTI_SOURCE 		:=	plat/renesas/rz/soc/v2h/drivers/spi_multi/spi_multi.c

DDR_SOURCES				+=	plat/renesas/rz/soc/v2h/drivers/ddr/ddr_v2h.c
