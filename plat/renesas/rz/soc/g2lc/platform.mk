#
# Copyright (c) 2020-2021, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

include plat/renesas/rz/common/rz_common.mk
include plat/renesas/rz/board/${BOARD}/rz_board.mk

PLAT_INCLUDES			+=	-Iplat/renesas/rz/soc/g2lc/include
FIP_ALIGN				:=	16

DDR_SOURCES				+=	plat/renesas/rz/soc/g2lc/drivers/ddr/ddr_g2lc.c

PLAT_BL_COMMON_SOURCES	+=	${DDR_SOURCES}

.PHONY: bptool_make bptool_clean

bptool: bptool_make
distclean realclean clean: bptool_clean

BPTOOLPATH		?=	tools/renesas/rz_boot_param

bptool_make:
	${Q}${MAKE} TRUSTED_BOARD_BOOT=${TRUSTED_BOARD_BOOT} --no-print-directory -C ${BPTOOLPATH}

bptool_clean:
	${Q}${MAKE} --no-print-directory -C ${BPTOOLPATH} clean

pkg:
	./tools/renesas/bptool build/g2lc/${BUILD_TYPE}/bl2.bin build/g2lc/${BUILD_TYPE}/bp_spi.bin 0x00012000 spi
	cat build/g2lc/${BUILD_TYPE}/bp_spi.bin build/g2lc/${BUILD_TYPE}/bl2.bin > build/g2lc/${BUILD_TYPE}/bl2_bp_spi.bin
	objcopy -I binary -O srec --adjust-vma=0x11E00 --srec-forceS3 build/g2lc/${BUILD_TYPE}/bl2_bp_spi.bin  build/g2lc/${BUILD_TYPE}/bl2_bp_spi.srec
	./tools/renesas/bptool build/g2lc/${BUILD_TYPE}/bl2.bin build/g2lc/${BUILD_TYPE}/bp_mmc.bin 0x00012000 mmc
	cat build/g2lc/${BUILD_TYPE}/bp_mmc.bin build/g2lc/${BUILD_TYPE}/bl2.bin > build/g2lc/${BUILD_TYPE}/bl2_bp_mmc.bin
	objcopy -I binary -O srec --adjust-vma=0x11E00 --srec-forceS3 build/g2lc/${BUILD_TYPE}/bl2_bp_mmc.bin  build/g2lc/${BUILD_TYPE}/bl2_bp_mmc.srec
	./tools/renesas/bptool build/g2lc/${BUILD_TYPE}/bl2.bin build/g2lc/${BUILD_TYPE}/bp_esd.bin 0x00012000 esd
	cat build/g2lc/${BUILD_TYPE}/bp_esd.bin build/g2lc/${BUILD_TYPE}/bl2.bin > build/g2lc/${BUILD_TYPE}/bl2_bp_esd.bin
	objcopy -I binary -O srec --adjust-vma=0x11E00 --srec-forceS3 build/g2lc/${BUILD_TYPE}/bl2_bp_esd.bin  build/g2lc/${BUILD_TYPE}/bl2_bp_esd.srec
	#Generate FIP S-Record if FIP binary is present
	if [ -f build/g2lc/${BUILD_TYPE}/fip.bin ]; then  objcopy -I binary -O srec --adjust-vma=0x00000 --srec-forceS3 build/g2lc/${BUILD_TYPE}/fip.bin build/g2lc/${BUILD_TYPE}/fip.srec ; fi ;
