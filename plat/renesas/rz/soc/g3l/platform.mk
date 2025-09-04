#
# Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

# Set the platform and SOC specific header files 1st
PLAT_INCLUDES		:=	-Iplat/renesas/rz/soc/g3l/include

include plat/renesas/rz/common/g3l_common.mk
include plat/renesas/rz/board/${PLAT}_${BOARD}/rz_board.mk

PLAT_INCLUDES			+=	-Iplat/renesas/rz/soc/g3l/drivers/ddr

DDR_SOURCES				+=  plat/renesas/rz/soc/g3l/drivers/ddr/ddr.c				\
							plat/renesas/rz/soc/g3l/drivers/ddr/ddr_misc.c

PLAT_BL_COMMON_SOURCES	+=	plat/renesas/rz/soc/g3l/drivers/riic.c					\
							plat/renesas/rz/soc/g3l/drivers/tzc/tzc400.c			\
							plat/renesas/rz/soc/g3l/drivers/pwrc/pwrc.c				\
							plat/renesas/rz/soc/g3l/drivers/pwrc/pwrc_stack.S		\
							plat/renesas/rz/soc/g3l/drivers/cpg.c					\
							${DDR_SOURCES}

BL2_SOURCES				+=	plat/renesas/rz/soc/g3l/bl2_plat_setup.c				\
							plat/renesas/rz/soc/g3l/plat_storage.c					\
							plat/renesas/rz/soc/g3l/plat_security.c					\
							plat/renesas/rz/soc/g3l/bl2_plat_mem_params_desc.c		\
							plat/renesas/rz/soc/g3l/drivers/sys.c					\
							plat/renesas/rz/soc/g3l/drivers/pfc.c					\
							plat/renesas/rz/soc/g3l/plat_ddr_setup.c

BL31_SOURCES			+=	plat/renesas/rz/soc/g3l/bl31_plat_setup.c				\
							plat/renesas/rz/soc/g3l/rz_plat_sip_handler.c			\
							plat/renesas/rz/soc/g3l/plat_pm.c

ifneq (${ENABLE_STACK_PROTECTOR},0)
PLAT_BL_COMMON_SOURCES	+=	plat/renesas/rz/soc/g3l/rz_stack_protector.c
endif

ifneq (${TRUSTED_BOARD_BOOT},0)

	EL3_CPTR_CLEAR_TFP	:= 1
    $(eval $(call add_define,EL3_CPTR_CLEAR_TFP))

	PLAT_BL_COMMON_SOURCES	+=	plat/renesas/rz/soc/g3l/drivers/auth/cip/cip.c

	# Include the selected chain of trust sources.
	ifeq (${COT},tbbr)
		BL2_SOURCES	+=	plat/renesas/rz/soc/g3l/drivers/auth/tbbr/tbbr_cot.c
	else
		$(error Unknown chain of trust ${COT})
	endif
endif


.PHONY: bptool_make bptool_clean

bptool: bptool_make
distclean realclean clean: bptool_clean

BPTOOLPATH		?=	tools/renesas/rz_boot_param

bptool_make:
	${Q}${MAKE} TRUSTED_BOARD_BOOT=${TRUSTED_BOARD_BOOT} --no-print-directory -C ${BPTOOLPATH}

bptool_clean:
	${Q}${MAKE} --no-print-directory -C ${BPTOOLPATH} clean

pkg:
	./tools/renesas/bptool build/g3l/${BUILD_TYPE}/bl2.bin build/g3l/${BUILD_TYPE}/bp_spi.bin 0x00023000 spi
	cat build/g3l/${BUILD_TYPE}/bp_spi.bin build/g3l/${BUILD_TYPE}/bl2.bin > build/g3l/${BUILD_TYPE}/bl2_bp_spi.bin
	objcopy -I binary -O srec --adjust-vma=0x21E00 --srec-forceS3 build/g3l/${BUILD_TYPE}/bl2_bp_spi.bin  build/g3l/${BUILD_TYPE}/bl2_bp_spi.srec
	./tools/renesas/bptool build/g3l/${BUILD_TYPE}/bl2.bin build/g3l/${BUILD_TYPE}/bp_mmc.bin 0x00023000 mmc
	cat build/g3l/${BUILD_TYPE}/bp_mmc.bin build/g3l/${BUILD_TYPE}/bl2.bin > build/g3l/${BUILD_TYPE}/bl2_bp_mmc.bin
	objcopy -I binary -O srec --adjust-vma=0x21E00 --srec-forceS3 build/g3l/${BUILD_TYPE}/bl2_bp_mmc.bin  build/g3l/${BUILD_TYPE}/bl2_bp_mmc.srec
	./tools/renesas/bptool build/g3l/${BUILD_TYPE}/bl2.bin build/g3l/${BUILD_TYPE}/bp_esd.bin 0x00023000 esd
	cat build/g3l/${BUILD_TYPE}/bp_esd.bin build/g3l/${BUILD_TYPE}/bl2.bin > build/g3l/${BUILD_TYPE}/bl2_bp_esd.bin
	objcopy -I binary -O srec --adjust-vma=0x21E00 --srec-forceS3 build/g3l/${BUILD_TYPE}/bl2_bp_esd.bin  build/g3l/${BUILD_TYPE}/bl2_bp_esd.srec
	#Generate FIP S-Record if FIP binary is present
	if [ -f build/g3l/${BUILD_TYPE}/fip.bin ]; then  objcopy -I binary -O srec --adjust-vma=0x00000 --srec-forceS3 build/g3l/${BUILD_TYPE}/fip.bin build/g3l/${BUILD_TYPE}/fip.srec ; fi ;
