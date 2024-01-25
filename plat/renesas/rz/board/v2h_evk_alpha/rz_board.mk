#
# Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

DDR_SOURCES 		+=	plat/renesas/rz/soc/${PLAT}/drivers/ddr/ddr_setup_lpddr4.c	\
						plat/renesas/rz/soc/${PLAT}/drivers/ddr/ddr_retcsr_lpddr4.c	\
						plat/renesas/rz/soc/${PLAT}/drivers/ddr/ddr_param_def_lpddr4.c

LPDDR4		:= 1
DDR_PLL4	:=1600

$(eval $(call add_define,DDR_PLL4))

PLAT_INCLUDES		+=	-Iplat/renesas/rz/board/v2h_evk_alpha
BL_COMMON_SOURCES	+=	plat/renesas/rz/board/v2h_evk_alpha/pwrc_board.c
