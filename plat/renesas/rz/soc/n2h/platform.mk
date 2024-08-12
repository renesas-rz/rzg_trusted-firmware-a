#
# Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

#Set the platform and SOC specific header files 1st
PLAT_INCLUDES	:=	-Iplat/renesas/rz/soc/t2h/include
FIP_ALIGN		:=	16

include plat/renesas/rz/common/n2h_common.mk
include plat/renesas/rz/board/${PLAT}_${BOARD}/rz_board.mk

PLAT_BL_COMMON_SOURCES	+=	plat/renesas/rz/soc/t2h/plat_security.c			\
							plat/renesas/rz/soc/t2h/plat_storage.c			\
							plat/renesas/rz/soc/t2h/drivers/sys.c

BL2_SOURCES				+=	plat/renesas/rz/soc/t2h/bl2_plat_setup.c		\
							plat/renesas/rz/soc/t2h/drivers/cpg.c			\
							plat/renesas/rz/soc/t2h/drivers/pfc.c

BL31_SOURCES			+=	plat/renesas/rz/soc/t2h/bl31_plat_setup.c		\
							plat/renesas/rz/soc/t2h/plat_pm.c				\
							plat/renesas/rz/soc/t2h/rz_plat_sip_handler.c

DDR_SOURCES				+=	plat/renesas/rz/soc/t2h/drivers/ddr/ddr.c		\
							plat/renesas/rz/soc/t2h/drivers/ddr/ddr_misc.c


.PHONY: pkg
pkg:
	#Get Bl2 filesize and align to 0x200 (512) bytes
	bl2_filesize=`find "./build/n2h/$(BUILD_TYPE)/bl2.bin" -printf "%s"` ; \
	bl2_filesize=`expr $$bl2_filesize + 511` ; \
	bl2_filesize=`expr $$bl2_filesize / 512` ; \
	bl2_filesize=`expr $$bl2_filesize \* 512` ; \
	#Convert size to Hex \
	bl2_filesize=`printf "0x%08x" $$bl2_filesize` ; \
	python3 ./tools/renesas/rzt2h_boot_param/parameter_block_generator.py --output=./build/n2h/$(BUILD_TYPE)/bp_xspi0.bin --cache_flag=1 --loader_addr=0x40000200 --loader_size=$$bl2_filesize --dest_addr=0x10001000 --pll0_ssc_ctr_v=0x00010000 --bootcpu_flg=1  --xspi_cssctl_v=0x07FFFFFF ; \
	python3 ./tools/renesas/rzt2h_boot_param/parameter_block_generator.py --output=./build/n2h/$(BUILD_TYPE)/bp_xspi1.bin --cache_flag=1 --loader_addr=0x50000200 --loader_size=$$bl2_filesize --dest_addr=0x10001000 --pll0_ssc_ctr_v=0x00010000 --bootcpu_flg=1  --xspi_cssctl_v=0x07FFFFFF ; \
	python3 ./tools/renesas/rzt2h_boot_param/parameter_block_generator.py --output=./build/n2h/$(BUILD_TYPE)/bp_emmc.bin  --cache_flag=1 --loader_addr=0x00000400 --loader_size=$$bl2_filesize --dest_addr=0x10001000 --pll0_ssc_ctr_v=0x00010000 --bootcpu_flg=1 ; \
	python3 ./tools/renesas/rzt2h_boot_param/parameter_block_generator.py --output=./build/n2h/$(BUILD_TYPE)/bp_esd.bin   --cache_flag=1 --loader_addr=0x00001000 --loader_size=$$bl2_filesize --dest_addr=0x10001000 --pll0_ssc_ctr_v=0x00010000 --bootcpu_flg=1

	#BL2 with XSPI0 Boot parameters
	cat ./build/n2h/$(BUILD_TYPE)/bp_xspi0.bin ./build/n2h/$(BUILD_TYPE)/bl2.bin > ./build/n2h/$(BUILD_TYPE)/bl2_bp_xspi0.bin
	objcopy -I binary -O srec --adjust-vma=0x10100000 --srec-forceS3 ./build/n2h/$(BUILD_TYPE)/bl2_bp_xspi0.bin ./build/n2h/$(BUILD_TYPE)/bl2_bp_xspi0.srec
	#BL2 with XSPI1 Boot parameters
	cat ./build/n2h/$(BUILD_TYPE)/bp_xspi1.bin ./build/n2h/$(BUILD_TYPE)/bl2.bin > ./build/n2h/$(BUILD_TYPE)/bl2_bp_xspi1.bin
	objcopy -I binary -O srec --adjust-vma=0x10100000 --srec-forceS3 ./build/n2h/$(BUILD_TYPE)/bl2_bp_xspi1.bin ./build/n2h/$(BUILD_TYPE)/bl2_bp_xspi1.srec
	#BL2 with eMMC Boot parameters
	cat ./build/n2h/$(BUILD_TYPE)/bp_emmc.bin ./build/n2h/$(BUILD_TYPE)/bl2.bin > ./build/n2h/$(BUILD_TYPE)/bl2_bp_emmc.bin
	objcopy -I binary -O srec --adjust-vma=0x10100000 --srec-forceS3 ./build/n2h/$(BUILD_TYPE)/bl2_bp_emmc.bin ./build/n2h/$(BUILD_TYPE)/bl2_bp_emmc.srec
	#BL2 with seven copies of eSD Boot parameters
	cat ./build/n2h/$(BUILD_TYPE)/bp_esd.bin ./build/n2h/$(BUILD_TYPE)/bp_esd.bin ./build/n2h/$(BUILD_TYPE)/bp_esd.bin ./build/n2h/$(BUILD_TYPE)/bp_esd.bin ./build/n2h/$(BUILD_TYPE)/bp_esd.bin ./build/n2h/$(BUILD_TYPE)/bp_esd.bin ./build/n2h/$(BUILD_TYPE)/bp_esd.bin ./build/n2h/$(BUILD_TYPE)/bl2.bin > ./build/n2h/$(BUILD_TYPE)/bl2_bp_esd.bin
	objcopy -I binary -O srec --adjust-vma=0x10100000 --srec-forceS3 ./build/n2h/$(BUILD_TYPE)/bl2_bp_esd.bin ./build/n2h/$(BUILD_TYPE)/bl2_bp_esd.srec
	#Generate FIP S-Record if FIP binary is present
	if [ -f build/n2h/${BUILD_TYPE}/fip.bin ]; then  objcopy -I binary -O srec --adjust-vma=0x10100000 --srec-forceS3 ./build/n2h/$(BUILD_TYPE)/fip.bin ./build/n2h/$(BUILD_TYPE)/fip.srec ; fi ;

ifneq (${DEBUG}, 0)
TF_CFLAGS += -O0 -fstack-usage
ASFLAGS += -O0 -fstack-usage
endif
