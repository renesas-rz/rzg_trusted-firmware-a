#
# Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

#Set the platform and SOC specific header files 1st
PLAT_INCLUDES	:=	-Iplat/renesas/rz/soc/g3e/include
FIP_ALIGN		:=	16

include plat/renesas/rz/common/g3e_common.mk
include plat/renesas/rz/board/${PLAT}_${BOARD}/rz_board.mk
include lib/libfdt/libfdt.mk

DDR_SOURCES				+=	plat/renesas/rz/soc/g3e/drivers/ddr/ddr.c		\
							plat/renesas/rz/soc/g3e/drivers/ddr/ddr_misc.c

PLAT_BL_COMMON_SOURCES	+=	plat/renesas/rz/soc/g3e/drivers/cpg.c				\
							plat/renesas/rz/soc/g3e/drivers/pwrc/pwrc.c			\
							plat/renesas/rz/soc/g3e/drivers/pwrc/pwrc_stack.S	\
							plat/renesas/rz/soc/g3e/drivers/riic.c				\
							plat/renesas/rz/soc/g3e/plat_security.c				\
							plat/renesas/rz/soc/g3e/drivers/pfc.c				\
							${DDR_SOURCES}

BL2_SOURCES				+=	plat/renesas/rz/soc/g3e/bl2_plat_setup.c			\
							plat/renesas/rz/soc/g3e/plat_storage.c				\
							plat/renesas/rz/soc/g3e/bl2_plat_mem_params_desc.c	\
							plat/renesas/rz/soc/g3e/drivers/sys.c				\
							plat/renesas/rz/soc/g3e/plat_ddr_setup.c

BL31_SOURCES			+=	plat/renesas/rz/soc/g3e/bl31_plat_setup.c		\
							plat/renesas/rz/soc/g3e/plat_pm.c				\
							plat/renesas/rz/soc/g3e/drivers/core_ctrl.c		\
							plat/renesas/rz/soc/g3e/drivers/wdt.c			\
							plat/renesas/rz/soc/g3e/rz_plat_sip_handler.c


.PHONY: bptool_make bptool_clean

bptool: bptool_make
distclean realclean clean: bptool_clean

BPTOOLPATH		?=	tools/renesas/rz_boot_param

bptool_make:
	${Q}${MAKE} --no-print-directory -C ${BPTOOLPATH}

bptool_clean:
	${Q}${MAKE} --no-print-directory -C ${BPTOOLPATH} clean

pkg:
	./tools/renesas/bptool build/g3e/${BUILD_TYPE}/bl2.bin build/g3e/${BUILD_TYPE}/bp_spi.bin 0x08004000 spi
	cat build/g3e/${BUILD_TYPE}/bp_spi.bin build/g3e/${BUILD_TYPE}/bl2.bin > build/g3e/${BUILD_TYPE}/bl2_bp_spi.bin
	objcopy -I binary -O srec --adjust-vma=0x08003600 --srec-forceS3 build/g3e/${BUILD_TYPE}/bl2_bp_spi.bin  build/g3e/${BUILD_TYPE}/bl2_bp_spi.srec
	./tools/renesas/bptool build/g3e/${BUILD_TYPE}/bl2.bin build/g3e/${BUILD_TYPE}/bp_mmc.bin 0x08004000 mmc
	cat build/g3e/${BUILD_TYPE}/bp_mmc.bin build/g3e/${BUILD_TYPE}/bl2.bin > build/g3e/${BUILD_TYPE}/bl2_bp_mmc.bin
	objcopy -I binary -O srec --adjust-vma=0x08003600 --srec-forceS3 build/g3e/${BUILD_TYPE}/bl2_bp_mmc.bin  build/g3e/${BUILD_TYPE}/bl2_bp_mmc.srec
	./tools/renesas/bptool build/g3e/${BUILD_TYPE}/bl2.bin build/g3e/${BUILD_TYPE}/bp_esd.bin 0x08004000 esd
	cat build/g3e/${BUILD_TYPE}/bp_esd.bin build/g3e/${BUILD_TYPE}/bl2.bin > build/g3e/${BUILD_TYPE}/bl2_bp_esd.bin
	objcopy -I binary -O srec --adjust-vma=0x08003600 --srec-forceS3 build/g3e/${BUILD_TYPE}/bl2_bp_esd.bin  build/g3e/${BUILD_TYPE}/bl2_bp_esd.srec
	#Generate FIP S-Record if FIP binary is present
	if [ -f build/g3e/${BUILD_TYPE}/fip.bin ]; then  objcopy -I binary -O srec --adjust-vma=0x0 --srec-forceS3 build/g3e/${BUILD_TYPE}/fip.bin build/g3e/${BUILD_TYPE}/fip.srec ; fi ;

ifneq (${DEBUG}, 0)
TF_CFLAGS += -O0 -fstack-usage
ASFLAGS += -O0 -fstack-usage
endif
