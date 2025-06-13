#
# Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

ENABLE_STACK_PROTECTOR		:= default

DDR_SOURCES 		+=  plat/renesas/rz/soc/t2h/drivers/ddr/ddr_setup_lpddr4.c					\
						plat/renesas/rz/soc/t2h/drivers/ddr/ddr_param_def_lpddr4.c
