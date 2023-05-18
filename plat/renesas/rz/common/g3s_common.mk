#
# Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

BL2_AT_EL3						:= 1
RESET_TO_BL31					:= 1
COLD_BOOT_SINGLE_CPU			:= 1
PROGRAMMABLE_RESET_ADDRESS		:= 0
WARMBOOT_ENABLE_DCACHE_EARLY	:= 1
GICV3_SUPPORT_GIC600			:= 1
GICV3_OVERRIDE_DISTIF_PWR_OPS	:= 1
HW_ASSISTED_COHERENCY			:= 1
USE_COHERENT_MEM				:= 0
PLAT_M33_BOOT_SUPPORT			:= 0
PLAT_SYSTEM_SUSPEND				:= 0
PLAT_SUSPEND_MODE				:= none
TRUSTED_BOARD_BOOT				:= 0
PROTECTED_CHIPID				:= 1
SECURE_RTC						:= 0
DEBUG_FPGA						:= 0

# This option gets enabled automatically if the TRUSTED_BOARD_BOOT
# is set via root Makefile, but Renesas support Trusted-Boot without
# Crypto module.
override CRYPTO_SUPPORT			:= 0

ifneq (${PLAT_SYSTEM_SUSPEND},0)
ifneq ($(filter awo vbat,${PLAT_SYSTEM_SUSPEND}),)
override PLAT_SUSPEND_MODE		:= ${PLAT_SYSTEM_SUSPEND}
override PLAT_SYSTEM_SUSPEND	:= 1
else
$(error Unknown suspend mode ${PLAT_SYSTEM_SUSPEND})
endif
endif

ifeq (${PLAT_SUSPEND_MODE},awo)
override PLAT_M33_BOOT_SUPPORT	:= 1
endif

$(eval $(call add_define,DEBUG_FPGA))
$(eval $(call add_define,SECURE_RTC))
$(eval $(call add_define,PROTECTED_CHIPID))
$(eval $(call add_define,PLAT_TBBR_IMG_DEF))
$(eval $(call add_define,PLAT_EXTRA_LD_SCRIPT))
$(eval $(call add_define,PLAT_M33_BOOT_SUPPORT))
$(eval $(call add_define,PLAT_SYSTEM_SUSPEND))
$(eval $(call add_define,PLAT_SYSTEM_SUSPEND_${PLAT_SUSPEND_MODE}))

# Enable workarounds for selected Cortex-A55 erratas.
ERRATA_A55_768277				:= 1
ERRATA_A55_778703 				:= 1
ERRATA_A55_798797 				:= 1
ERRATA_A55_846532 				:= 1
ERRATA_A55_903758 				:= 1
ERRATA_A55_1221012				:= 1
ERRATA_A55_1530923				:= 1

PLAT_INCLUDES			+=	-Iplat/renesas/rz/common/include						\
							-Iplat/renesas/rz/common/drivers/io						\
							-Iplat/renesas/rz/common/drivers/sd

RZ_TIMER_SOURCES		:=	drivers/delay_timer/generic_delay_timer.c				\
							drivers/delay_timer/delay_timer.c

EMMC_SOURCES			:=

XSPI_SOURCES			:=	plat/renesas/rz/common/drivers/xspi.c

SD_SOURCES				:=	plat/renesas/rz/common/drivers/sd/sd_init.c				\
							plat/renesas/rz/common/drivers/sd/sd_mount.c			\
							plat/renesas/rz/common/drivers/sd/sd_util.c				\
							plat/renesas/rz/common/drivers/sd/sd_cd.c				\
							plat/renesas/rz/common/drivers/sd/sd_cmd.c				\
							plat/renesas/rz/common/drivers/sd/sd_int.c				\
							plat/renesas/rz/common/drivers/sd/sd_trns.c				\
							plat/renesas/rz/common/drivers/sd/sd_read.c				\
							plat/renesas/rz/common/drivers/sd/sd_write.c

BL_COMMON_SOURCES		+=	lib/cpus/aarch64/cortex_a55.S

include lib/xlat_tables_v2/xlat_tables.mk
PLAT_BL_COMMON_SOURCES	:=	${XLAT_TABLES_LIB_SRCS}									\
							plat/renesas/rz/common/plat_rz_common.c					\
							plat/renesas/rz/common/aarch64/plat_helpers.S			\
							plat/renesas/rz/common/drivers/syc.c					\
							plat/renesas/rz/common/drivers/scifa.S

BL2_SOURCES				+=	common/desc_image_load.c								\
							drivers/io/io_storage.c									\
							drivers/io/io_fip.c										\
							plat/renesas/rz/common/drivers/io/io_xspidrv.c			\
							plat/renesas/rz/common/drivers/io/io_emmcdrv.c			\
							plat/renesas/rz/common/drivers/io/io_sddrv.c			\
							plat/renesas/rz/common/plat_image_load.c				\
							${RZ_TIMER_SOURCES}										\
							${EMMC_SOURCES}											\
							${XSPI_SOURCES}											\
							${SD_SOURCES}

# Include GICv3 driver files
include drivers/arm/gic/v3/gicv3.mk

BL31_SOURCES			+=	plat/common/plat_gicv3.c								\
							plat/common/plat_psci_common.c							\
							plat/renesas/rz/common/plat_topology.c					\
							plat/renesas/rz/common/plat_gic.c						\
							plat/renesas/rz/common/rz_plat_sip_handler.c			\
							plat/renesas/rz/common/rz_sip_svc.c						\
							${GICV3_SOURCES}

ifneq (${TRUSTED_BOARD_BOOT},0)

	# Include common TBB sources
	AUTH_SOURCES		:=	drivers/auth/img_parser_mod.c

	# Include RZ TBB sources
	AUTH_SOURCES		+=	plat/renesas/rz/common/drivers/auth/auth_mod.c				\
							plat/renesas/rz/common/drivers/auth/sblib/crypto_sblib.c	\
							plat/renesas/rz/common/drivers/auth/sblib/sblib_parser.c

	BL2_SOURCES		+=	${AUTH_SOURCES}

endif
