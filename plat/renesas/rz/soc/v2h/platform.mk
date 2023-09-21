#
# Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

#Set the platform and SOC specific header files 1st
PLAT_INCLUDES	:=	-Iplat/renesas/rz/soc/v2h/include

include plat/renesas/rz/common/v2h_common.mk
include plat/renesas/rz/board/${PLAT}_${BOARD}/rz_board.mk

PLAT_BL_COMMON_SOURCES	+=	plat/renesas/rz/soc/v2h/plat_security.c

BL2_SOURCES				+=	plat/renesas/rz/soc/v2h/bl2_plat_setup.c		\
							plat/renesas/rz/soc/v2h/drivers/cpg.c			\
							plat/renesas/rz/soc/v2h/drivers/sys.c			\
							plat/renesas/rz/soc/v2h/drivers/pfc.c

BL31_SOURCES			+=	plat/renesas/rz/soc/v2h/bl31_plat_setup.c		\
							plat/renesas/rz/soc/v2h/plat_pm.c				\
							plat/renesas/rz/soc/v2h/rz_plat_sip_handler.c

DDR_SOURCES				+=	plat/renesas/rz/soc/v2h/drivers/ddr/ddr.c		\
							plat/renesas/rz/soc/v2h/drivers/ddr/ddr_misc.c

.PHONY: bptool_make bptool_clean

bptool: bptool_make
distclean realclean clean: bptool_clean

BPTOOLPATH		?=	tools/renesas/rz_boot_param

bptool_make:
	${Q}${MAKE} --no-print-directory -C ${BPTOOLPATH}

bptool_clean:
	${Q}${MAKE} --no-print-directory -C ${BPTOOLPATH} clean

pkg:
	./tools/renesas/bptool build/v2h/${BUILD_TYPE}/bl2.bin build/v2h/${BUILD_TYPE}/bp.bin 0x08103000 spi
	cat build/v2h/${BUILD_TYPE}/bp.bin build/v2h/${BUILD_TYPE}/bl2.bin > build/v2h/${BUILD_TYPE}/bl2_bp.bin
	objcopy -I binary -O srec --adjust-vma=0x8101E00 --srec-forceS3 build/v2h/${BUILD_TYPE}/bl2_bp.bin  build/v2h/${BUILD_TYPE}/bl2_bp.srec
	./tools/renesas/bptool build/v2h/${BUILD_TYPE}/bl2.bin build/v2h/${BUILD_TYPE}/bp_mmc.bin 0x08103000 mmc
	cat build/v2h/${BUILD_TYPE}/bp_mmc.bin build/v2h/${BUILD_TYPE}/bl2.bin > build/v2h/${BUILD_TYPE}/bl2_bp_mmc.bin
	objcopy -I binary -O srec --adjust-vma=0x8101E00 --srec-forceS3 build/v2h/${BUILD_TYPE}/bl2_bp_mmc.bin  build/v2h/${BUILD_TYPE}/bl2_bp_mmc.srec
	./tools/renesas/bptool build/v2h/${BUILD_TYPE}/bl2.bin build/v2h/${BUILD_TYPE}/bp_esd.bin 0x08103000 esd
	cat build/v2h/${BUILD_TYPE}/bp_esd.bin build/v2h/${BUILD_TYPE}/bl2.bin > build/v2h/${BUILD_TYPE}/bl2_bp_esd.bin
	objcopy -I binary -O srec --adjust-vma=0x8101E00 --srec-forceS3 build/v2h/${BUILD_TYPE}/bl2_bp_esd.bin  build/v2h/${BUILD_TYPE}/bl2_bp_esd.srec
	./tools/fiptool/fiptool create --align 16 --soc-fw build/v2h/${BUILD_TYPE}/bl31.bin --nt-fw ../u-boot.bin build/v2h/${BUILD_TYPE}/fip.bin
	objcopy -I binary -O srec --adjust-vma=0x8101E00 --srec-forceS3 build/v2h/${BUILD_TYPE}/fip.bin build/v2h/${BUILD_TYPE}/fip.srec
	tar zcvf ../tf-a.tar.gz $(shell pwd)

ifneq (${DEBUG}, 0)
TF_CFLAGS += -O0 -fstack-usage
ASFLAGS += -O0 -fstack-usage
endif
