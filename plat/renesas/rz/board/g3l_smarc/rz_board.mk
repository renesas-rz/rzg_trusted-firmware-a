#
# Copyright (c) 2024, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

DDR_SOURCES +=	plat/renesas/rz/soc/${PLAT}/drivers/ddr/ddr_setup_lpddr4.c	\
				plat/renesas/rz/soc/${PLAT}/drivers/ddr/ddr_retcsr_lpddr4.c	\
				plat/renesas/rz/board/g3l_smarc/ddr_param_def_lpddr4.c	\
				plat/renesas/rz/soc/${PLAT}/drivers/ddr/ddr_setup_lpddr4.c


LPDDR4		:= 1
DDR_PLL4	:=2133

$(eval $(call add_define,DDR_PLL4))
