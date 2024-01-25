#
# Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

PLAT_SOC_RZV2H					:= 1
BL2_AT_EL3						:= 1
COLD_BOOT_SINGLE_CPU			:= 1
PROGRAMMABLE_RESET_ADDRESS		:= 0
WARMBOOT_ENABLE_DCACHE_EARLY	:= 1
GICV3_SUPPORT_GIC600			:= 1
GICV3_OVERRIDE_DISTIF_PWR_OPS	:= 1
HW_ASSISTED_COHERENCY			:= 1
USE_COHERENT_MEM				:= 0
TRUSTED_BOARD_BOOT				:= 0
PROTECTED_CHIPID				:= 1
DEBUG_FPGA						:= 0
PLAT_DDR_ECC					:= 0
PLAT_SYSTEM_SUSPEND				:= 0
RESET_TO_BL31					:= 1

ifneq (${PLAT_SYSTEM_SUSPEND},0)
override PLAT_SYSTEM_SUSPEND	:= 1
endif

$(eval $(call add_define,PLAT_SOC_RZV2H))
$(eval $(call add_define,PROTECTED_CHIPID))
$(eval $(call add_define,DEBUG_FPGA))
$(eval $(call add_define,PLAT_DDR_ECC))
$(eval $(call add_define,PLAT_SYSTEM_SUSPEND))
$(eval $(call add_define,PLAT_EXTRA_LD_SCRIPT))
ifeq (${BOARD}, evk_1)
$(eval $(call add_define,BOOT_MODE_eMMC_NOT_SUPPORTED))
endif

# Enable workarounds for selected Cortex-A55 erratas.
ERRATA_A55_768277				:= 1
ERRATA_A55_778703 				:= 1
ERRATA_A55_798797 				:= 1
ERRATA_A55_846532 				:= 1
ERRATA_A55_903758 				:= 1
ERRATA_A55_1221012				:= 1
ERRATA_A55_1530923				:= 1

PLAT_INCLUDES			+=	-Iplat/renesas/rz/common/include					\
							-Iplat/renesas/rz/common/drivers/emmc				\
							-Iplat/renesas/rz/common/drivers/io					\
							-Idrivers/renesas/common/io							\
							-Iplat/renesas/rz/common/include/drivers/spi_multi	\
							-Iplat/renesas/rz/common/drivers/sd

RZ_TIMER_SOURCES		:=	drivers/delay_timer/generic_delay_timer.c			\
							drivers/delay_timer/delay_timer.c

BL_COMMON_SOURCES		+=	lib/cpus/aarch64/cortex_a55.S						\
							drivers/arm/tzc/tzc400.c

include lib/xlat_tables_v2/xlat_tables.mk
PLAT_BL_COMMON_SOURCES		:=	${XLAT_TABLES_LIB_SRCS}							\
							plat/renesas/rz/common/plat_rz_common.c				\
							plat/renesas/rz/common/aarch64/plat_helpers.S		\
							plat/renesas/rz/common/drivers/syc.c				\
							plat/renesas/rz/common/drivers/scifa.S			\
							${RZ_TIMER_SOURCES}

ifneq (${ENABLE_STACK_PROTECTOR},0)
PLAT_BL_COMMON_SOURCES	+=	plat/renesas/rz/common/rz_stack_protector.c
endif

XSPI_SOURCES			:=	plat/renesas/rz/common/drivers/xspi.c	\
							plat/renesas/rz/common/drivers/io/io_xspidrv.c
ifneq (${BOARD}, evk_1)
EMMC_SOURCES			:=	plat/renesas/rz/common/drivers/io/io_emmcdrv.c		\
							plat/renesas/rz/common/drivers/emmc/emmc_interrupt.c	\
							plat/renesas/rz/common/drivers/emmc/emmc_utility.c	\
							plat/renesas/rz/common/drivers/emmc/emmc_mount.c	\
							plat/renesas/rz/common/drivers/emmc/emmc_init.c		\
							plat/renesas/rz/common/drivers/emmc/emmc_read.c		\
							plat/renesas/rz/common/drivers/emmc/emmc_cmd.c
endif
SD_SOURCES				:=	plat/renesas/rz/common/drivers/sd/sd_init.c			\
							plat/renesas/rz/common/drivers/sd/sd_mount.c		\
							plat/renesas/rz/common/drivers/sd/sd_util.c			\
							plat/renesas/rz/common/drivers/sd/sd_cd.c			\
							plat/renesas/rz/common/drivers/sd/sd_cmd.c			\
							plat/renesas/rz/common/drivers/sd/sd_int.c			\
							plat/renesas/rz/common/drivers/sd/sd_trns.c			\
							plat/renesas/rz/common/drivers/sd/sd_read.c			\
							plat/renesas/rz/common/drivers/sd/sd_write.c		\
							plat/renesas/rz/common/drivers/sd/sd_dev_low.c		\
							plat/renesas/rz/common/drivers/io/io_sddrv.c

BL2_SOURCES				+=	common/desc_image_load.c							\
							drivers/io/io_storage.c								\
							drivers/io/io_memmap.c								\
							drivers/io/io_fip.c									\
							plat/renesas/rz/common/plat_image_load.c			\
							plat/renesas/rz/common/plat_storage.c				\
							plat/renesas/rz/common/bl2_plat_mem_params_desc.c	\
							${XSPI_SOURCES}										\
							${EMMC_SOURCES}										\
							${SD_SOURCES}

# Include GICv3 driver files
GICV3_IMPL				:=	GIC600
include drivers/arm/gic/v3/gicv3.mk

BL31_SOURCES			:=	plat/common/plat_gicv3.c							\
							plat/common/plat_psci_common.c						\
							plat/renesas/rz/common/plat_topology.c				\
							plat/renesas/rz/common/plat_gic.c					\
							plat/renesas/rz/common/rz_sip_svc.c					\
							${GICV3_SOURCES}

ifneq (${TRUSTED_BOARD_BOOT},0)

	# Include common TBB sources
	AUTH_SOURCES		:=	drivers/auth/img_parser_mod.c

	# Include the selected chain of trust sources.
	ifeq (${COT},tbbr)
		AUTH_SOURCES	+=	plat/renesas/rz/common/drivers/auth/tbbr/tbbr_cot.c
	else
		$(error Unknown chain of trust ${COT})
	endif

	# Include RZ TBB sources
	AUTH_SOURCES		+=	plat/renesas/rz/common/drivers/auth/auth_mod.c				\
							plat/renesas/rz/common/drivers/auth/sblib/crypto_sblib.c	\
							plat/renesas/rz/common/drivers/auth/sblib/sblib_parser.c

	BL2_SOURCES		+=	${AUTH_SOURCES}

endif
