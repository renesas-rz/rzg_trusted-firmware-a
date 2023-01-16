#
# Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
LPDDR4		:= 1
DDR_PLL4	:= 1866

$(eval $(call add_define,DDR_PLL4))

DDR_SOURCES +=  plat/renesas/rz/soc/${PLAT}/drivers/ddr/ddr.c

ifneq (${LPDDR4}, 1)
DDR_SOURCES +=  plat/renesas/rz/soc/${PLAT}/drivers/ddr/phyinit_param_ddr4.c		\
				plat/renesas/rz/soc/${PLAT}/drivers/ddr/phyinit_swizzle_ddr4.c		\
				plat/renesas/rz/soc/${PLAT}/drivers/ddr/setup_mc_ddr4.c				\
				plat/renesas/rz/soc/${PLAT}/drivers/ddr/decode_streaming_message_ddr4.c \
				plat/renesas/rz/soc/${PLAT}/drivers/ddr/phyfm_ddr4.c
else
DDR_SOURCES +=  plat/renesas/rz/soc/${PLAT}/drivers/ddr/phyinit_param_lpddr4.c		\
				plat/renesas/rz/soc/${PLAT}/drivers/ddr/phyinit_swizzle_lpddr4.c	\
				plat/renesas/rz/soc/${PLAT}/drivers/ddr/setup_mc_lpddr4.c			\
				plat/renesas/rz/soc/${PLAT}/drivers/ddr/decode_streaming_message_lpddr4.c	\
				plat/renesas/rz/soc/${PLAT}/drivers/ddr/phyfm_lpddr4.c
endif
