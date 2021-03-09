#
# Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

BL2_AT_EL3						:= 1
COLD_BOOT_SINGLE_CPU			:= 1
PROGRAMMABLE_RESET_ADDRESS		:= 1
WARMBOOT_ENABLE_DCACHE_EARLY	:= 1
GICV3_SUPPORT_GIC600			:= 1
HW_ASSISTED_COHERENCY			:= 1
USE_COHERENT_MEM				:= 0
DEBUG_RZG2L_FPGA				:= 0
$(eval $(call add_define,DEBUG_RZG2L_FPGA))

RZG2L_DEVELOPMENT_BOARD		:= 0
RZG2L_SMARC_EVK				:= 1
RZG2LC_DEVELOPMENT_BOARD	:= 2
$(eval $(call add_define,RZG2L_DEVELOPMENT_BOARD))
$(eval $(call add_define,RZG2L_SMARC_EVK))
$(eval $(call add_define,RZG2LC_DEVELOPMENT_BOARD))

BOARD_TYPE	:= RZG2L_DEVELOPMENT_BOARD
$(eval $(call add_define,BOARD_TYPE))

# Enable workarounds for selected Cortex-A55 erratas.
ERRATA_A55_1530923	:= 1

PLAT_INCLUDES	:=  -Iplat/renesas/rzg2l/include						\
					-Iplat/renesas/rzg2l

RZG2L_COMMON		:=	plat/renesas/rzg2l/rzg2l_plat_common.c

RZG2L_TIMER_SOURCES := drivers/delay_timer/generic_delay_timer.c		\
					   drivers/delay_timer/delay_timer.c

BL2_SOURCES		+= 	lib/cpus/aarch64/cortex_a55.S						\
					${RZG2L_COMMON}										\
					${RZG2L_TIMER_SOURCES}								\
					common/desc_image_load.c							\
					drivers/io/io_storage.c								\
					drivers/arm/tzc/tzc400.c							\
					plat/renesas/rzg2l/bl2_plat_setup.c					\
					plat/renesas/rzg2l/bl2_plat_mem_params_desc.c		\
					plat/renesas/rzg2l/plat_image_load.c				\
					plat/renesas/rzg2l/plat_storage.c					\
					plat/renesas/rzg2l/plat_security.c					\
					plat/renesas/rzg2l/aarch64/plat_helpers.S			\
					plat/renesas/rzg2l/drivers/syc.c					\
					plat/renesas/rzg2l/drivers/pfc.c					\
					plat/renesas/rzg2l/drivers/cpg.c					\
					plat/renesas/rzg2l/drivers/spi_multi.c				\
					plat/renesas/rzg2l/drivers/ddr/ddr.c				\
					drivers/io/io_memmap.c								\
					drivers/io/io_fip.c

# Include GICv3 driver files
GICV3_IMPL		:= GIC600
include drivers/arm/gic/v3/gicv3.mk

BL31_SOURCES	+=	lib/cpus/aarch64/cortex_a55.S				\
					${RZG2L_COMMON}								\
					${GICV3_SOURCES}							\
					plat/common/plat_gicv3.c					\
					plat/common/plat_psci_common.c				\
					plat/renesas/rzg2l/bl31_plat_setup.c		\
					plat/renesas/rzg2l/plat_pm.c				\
					plat/renesas/rzg2l/plat_topology.c			\
					plat/renesas/rzg2l/plat_gic.c				\
					plat/renesas/rzg2l/aarch64/plat_helpers.S	\
					plat/renesas/rzg2l/drivers/syc.c

include lib/xlat_tables_v2/xlat_tables.mk
PLAT_BL_COMMON_SOURCES	+=	${XLAT_TABLES_LIB_SRCS}				\
							plat/renesas/rzg2l/drivers/scifa.S

