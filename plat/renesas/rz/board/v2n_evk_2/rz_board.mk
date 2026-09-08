#
# Copyright (c) 2024, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

DDR_SOURCES 		+=	plat/renesas/rz/soc/${PLAT}/drivers/ddr/ddr_setup_lpddr4.c	\
						plat/renesas/rz/soc/${PLAT}/drivers/ddr/ddr_retcsr_lpddr4.c	\
						plat/renesas/rz/soc/${PLAT}/drivers/ddr/ddr_param_def_lpddr4.c

LPDDR4		:= 1
DDR_PLL4	:=1600

$(eval $(call add_define,DDR_PLL4))

#
# Modules useful for testing
#

LED_CONTROL :=0

ifneq ($(LED_CONTROL),0)
PLAT_INCLUDES	+=	-Iplat/renesas/rz/board/v2n_evk_2/include
BL_COMMON_SOURCES	+=	plat/renesas/rz/board/v2n_evk_2/led_control.c
endif

ENABLE_PMIC_CONTROL				:= 0

ifneq ($(ENABLE_PMIC_CONTROL),0)
BL_COMMON_SOURCES	+=	plat/renesas/rz/board/v2n_evk_2/pwrc_board.c
endif
