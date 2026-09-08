#
# Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
LPDDR4		:= 1
DDR_PLL4	:= 1600

$(eval $(call add_define,LPDDR4))
$(eval $(call add_define,DDR_PLL4))

BL_COMMON_SOURCES	+=	plat/renesas/rz/board/g3s_dev14_1_lpddr/pwrc_board.c

DDR_SOURCES 		+=  plat/renesas/rz/soc/g3s/drivers/ddr/ddr_setup_lpddr4.c					\
						plat/renesas/rz/soc/g3s/drivers/ddr/ddr_param_def_lpddr4.c				\
						plat/renesas/rz/soc/g3s/drivers/ddr/ddr_retcsr_lpddr4.c					\
						plat/renesas/rz/soc/g3s/drivers/ddr/decode_streaming_message_lpddr4.c

PLAT_BL_COMMON_SOURCES	+=	plat/renesas/rz/board/g3s_dev14_1_lpddr/drivers/cpg.c

BL2_SOURCES				+=	plat/renesas/rz/board/g3s_dev14_1_lpddr/drivers/pfc.c

