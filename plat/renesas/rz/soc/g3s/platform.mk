#
# Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

#Set the platform and SOC specific header files 1st
PLAT_INCLUDES		:=	-Iplat/renesas/rz/soc/g3s/include

include plat/renesas/rz/common/g3s_common.mk
include plat/renesas/rz/board/${PLAT}_${BOARD}/rz_board.mk

BL_COMMON_SOURCES	+=	plat/renesas/rz/soc/g3s/drivers/cpg.c					\
						plat/renesas/rz/soc/g3s/drivers/riic.c					\
						plat/renesas/rz/soc/g3s/drivers/tzc/tzc400.c			\
						plat/renesas/rz/soc/g3s/drivers/pwrc/pwrc.c				\
						plat/renesas/rz/soc/g3s/drivers/pwrc/pwrc_stack.S

BL2_SOURCES			+=	plat/renesas/rz/soc/g3s/bl2_plat_setup.c				\
						plat/renesas/rz/soc/g3s/plat_storage.c					\
						plat/renesas/rz/soc/g3s/plat_security.c					\
						plat/renesas/rz/soc/g3s/bl2_plat_mem_params_desc.c		\
						plat/renesas/rz/soc/g3s/drivers/sys.c					\
						plat/renesas/rz/soc/g3s/drivers/pfc.c					\
						plat/renesas/rz/soc/g3s/plat_ddr_setup.c

BL31_SOURCES		+=	plat/renesas/rz/soc/g3s/bl31_plat_setup.c				\
						plat/renesas/rz/soc/g3s/plat_pm.c

EMMC_SOURCES		+=	plat/renesas/rz/soc/g3s/drivers/emmc/emmc_interrupt.c	\
						plat/renesas/rz/soc/g3s/drivers/emmc/emmc_utility.c		\
						plat/renesas/rz/soc/g3s/drivers/emmc/emmc_mount.c		\
						plat/renesas/rz/soc/g3s/drivers/emmc/emmc_init.c		\
						plat/renesas/rz/soc/g3s/drivers/emmc/emmc_read.c		\
						plat/renesas/rz/soc/g3s/drivers/emmc/emmc_cmd.c			\
						plat/renesas/rz/soc/g3s/drivers/emmc/emmc_write.c

XSPI_SOURCES		+=	plat/renesas/rz/common/drivers/xspi.c

DDR_SOURCES			+=  plat/renesas/rz/soc/${PLAT}/drivers/ddr/ddr.c			\
						plat/renesas/rz/soc/${PLAT}/drivers/ddr/ddr_retention_entry.c

ifneq (${TRUSTED_BOARD_BOOT},0)
	# Include the selected chain of trust sources.
	ifeq (${COT},tbbr)
		BL2_SOURCES	+=	plat/renesas/rz/common/drivers/auth/tbbr/tbbr_cot.c		# TODO
	else
		$(error Unknown chain of trust ${COT})
	endif
endif


.PHONY: bptool_make bptool_clean

bptool: bptool_make
distclean realclean clean: bptool_clean

BPTOOLPATH		?=	tools/renesas/rz_boot_param
BPTOOL			?=	${BPTOOLPATH}/bptool${BIN_EXT}

bptool_make:
	${Q}${MAKE} TRUSTED_BOARD_BOOT=${TRUSTED_BOARD_BOOT} --no-print-directory -C ${BPTOOLPATH}

bptool_clean:
	${Q}${MAKE} --no-print-directory -C ${BPTOOLPATH} clean
