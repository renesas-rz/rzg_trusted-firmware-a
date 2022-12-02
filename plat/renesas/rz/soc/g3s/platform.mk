#
# Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

#Set the platform and SOC specific header files 1st
PLAT_INCLUDES	:=	-Iplat/renesas/rz/soc/g3s/include

include plat/renesas/rz/common/g3s_common.mk
include plat/renesas/rz/board/${PLAT}_${BOARD}/rz_board.mk

PLAT_BL_COMMON_SOURCES	+=	plat/renesas/rz/soc/g3s/plat_security.c

BL2_SOURCES				+=	plat/renesas/rz/soc/g3s/bl2_plat_setup.c		\
							plat/renesas/rz/soc/g3s/drivers/cpg.c			\
							plat/renesas/rz/soc/g3s/drivers/pfc.c

BL31_SOURCES			+=	plat/renesas/rz/soc/g3s/bl31_plat_setup.c		\
							plat/renesas/rz/soc/g3s/plat_pm.c

XSPI_SOURCE				:=	plat/renesas/rz/soc/g3s/drivers/xspi.c

SPI_MULTI_SOURCE 		:=	plat/renesas/rz/soc/g3s/drivers/spi_multi/spi_multi.c

DDR_SOURCES				+=	plat/renesas/rz/soc/g3s/drivers/ddr/ddr_g3s.c
