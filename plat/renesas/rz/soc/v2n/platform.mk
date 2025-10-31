#
# Copyright (c) 2024, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

#Set the platform and SOC specific header files 1st
PLAT_INCLUDES	:=	-Iplat/renesas/rz/soc/v2n/include			\
					-Iplat/renesas/rz/soc/v2n/include/cpg

FIP_ALIGN		:=	16

include plat/renesas/rz/common/v2n_common.mk
include plat/renesas/rz/board/${PLAT}_${BOARD}/rz_board.mk

DDR_SOURCES	+=				plat/renesas/rz/soc/v2n/drivers/ddr/ddr.c	\
							plat/renesas/rz/soc/v2n/drivers/ddr/ddr_misc.c	\
							plat/renesas/rz/soc/v2n/plat_ddr_setup.c

PLAT_BL_COMMON_SOURCES	+=	plat/renesas/rz/soc/v2n/plat_security.c						\
							plat/renesas/rz/soc/v2n/drivers/riic.c						\
							plat/renesas/rz/soc/v2n/drivers/cpg/cpg_clk_settings.c		\
							plat/renesas/rz/soc/v2n/drivers/cpg/cpg_mstop_settings.c	\
							plat/renesas/rz/soc/v2n/drivers/cpg/cpg_pll_div_settings.c	\
							plat/renesas/rz/soc/v2n/drivers/cpg/cpg_rst_settings.c		\
							plat/renesas/rz/soc/v2n/drivers/cpg/cpg.c					\
							plat/renesas/rz/soc/v2n/drivers/pwrc/pwrc.c					\
							plat/renesas/rz/soc/v2n/drivers/pwrc/pwrc_stack.S			\
							plat/renesas/rz/soc/v2n/drivers/pfc.c						\
							${DDR_SOURCES}

BL2_SOURCES				+=	plat/renesas/rz/soc/v2n/bl2_plat_setup.c		\
							plat/renesas/rz/soc/v2n/plat_storage.c			\
							plat/renesas/rz/soc/v2n/bl2_plat_mem_params_desc.c	\
							plat/renesas/rz/soc/v2n/drivers/sys.c

BL31_SOURCES			+=	plat/renesas/rz/soc/v2n/bl31_plat_setup.c		\
							plat/renesas/rz/soc/v2n/plat_pm.c				\
							plat/renesas/rz/soc/v2n/drivers/wdt.c			\
							plat/renesas/rz/soc/v2n/rz_plat_sip_handler.c

.PHONY: bptool_make bptool_clean

bptool: bptool_make
distclean realclean clean: bptool_clean

BPTOOLPATH		?=	tools/renesas/rz_boot_param

bptool_make:
	${Q}${MAKE} --no-print-directory -C ${BPTOOLPATH}

bptool_clean:
	${Q}${MAKE} --no-print-directory -C ${BPTOOLPATH} clean

pkg:
	./tools/renesas/bptool build/v2n/${BUILD_TYPE}/bl2.bin build/v2n/${BUILD_TYPE}/bp_spi.bin 0x08103000 spi
	cat build/v2n/${BUILD_TYPE}/bp_spi.bin build/v2n/${BUILD_TYPE}/bl2.bin > build/v2n/${BUILD_TYPE}/bl2_bp_spi.bin
	objcopy -I binary -O srec --adjust-vma=0x8101E00 --srec-forceS3 build/v2n/${BUILD_TYPE}/bl2_bp_spi.bin  build/v2n/${BUILD_TYPE}/bl2_bp_spi.srec
	./tools/renesas/bptool build/v2n/${BUILD_TYPE}/bl2.bin build/v2n/${BUILD_TYPE}/bp_mmc.bin 0x08103000 mmc;\
	cat build/v2n/${BUILD_TYPE}/bp_mmc.bin build/v2n/${BUILD_TYPE}/bl2.bin > build/v2n/${BUILD_TYPE}/bl2_bp_mmc.bin;\
	objcopy -I binary -O srec --adjust-vma=0x8101E00 --srec-forceS3 build/v2n/${BUILD_TYPE}/bl2_bp_mmc.bin  build/v2n/${BUILD_TYPE}/bl2_bp_mmc.srec;\
	./tools/renesas/bptool build/v2n/${BUILD_TYPE}/bl2.bin build/v2n/${BUILD_TYPE}/bp_esd.bin 0x08103000 esd
	cat build/v2n/${BUILD_TYPE}/bp_esd.bin build/v2n/${BUILD_TYPE}/bl2.bin > build/v2n/${BUILD_TYPE}/bl2_bp_esd.bin
	objcopy -I binary -O srec --adjust-vma=0x8101E00 --srec-forceS3 build/v2n/${BUILD_TYPE}/bl2_bp_esd.bin  build/v2n/${BUILD_TYPE}/bl2_bp_esd.srec
	#Generate FIP S-Record if FIP binary is present
	if [ -f build/v2n/${BUILD_TYPE}/fip.bin ]; then  objcopy -I binary -O srec --adjust-vma=0x0 --srec-forceS3 build/v2n/${BUILD_TYPE}/fip.bin build/v2n/${BUILD_TYPE}/fip.srec ; fi ;

ifneq (${DEBUG}, 0)
TF_CFLAGS += -O0 -fstack-usage
ASFLAGS += -O0 -fstack-usage
endif
