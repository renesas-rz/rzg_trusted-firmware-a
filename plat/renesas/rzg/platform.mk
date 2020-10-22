#
# Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

PROGRAMMABLE_RESET_ADDRESS      := 0
COLD_BOOT_SINGLE_CPU            := 1
ARM_CCI_PRODUCT_ID              := 500
TRUSTED_BOARD_BOOT              := 1
RESET_TO_BL31                   := 1
GENERATE_COT                    := 1
BL2_AT_EL3                      := 1
ENABLE_SVE_FOR_NS               := 0
MULTI_CONSOLE_API               := 1

CRASH_REPORTING                 := 1
HANDLE_EA_EL3_FIRST             := 1

$(eval $(call add_define,PLAT_EXTRA_LD_SCRIPT))

ifeq (${SPD},none)
  SPD_NONE:=1
  $(eval $(call add_define,SPD_NONE))
endif

# LSI setting common define
RZG_G2H	:=0
RZG_G2M	:=1
RZG_G2N	:=2
RZG_G2E	:=3
RZG_AUTO:=99
$(eval $(call add_define,RZG_G2M))
$(eval $(call add_define,RZG_G2N))
$(eval $(call add_define,RZG_G2E))
$(eval $(call add_define,RZG_G2H))
$(eval $(call add_define,RZG_AUTO))
RZG_CUT_10:=0
RZG_CUT_11:=1
RZG_CUT_13:=3
RZG_CUT_20:=10
RZG_CUT_30:=20
$(eval $(call add_define,RZG_CUT_10))
$(eval $(call add_define,RZG_CUT_11))
$(eval $(call add_define,RZG_CUT_13))
$(eval $(call add_define,RZG_CUT_20))
$(eval $(call add_define,RZG_CUT_30))

ifndef LSI
  $(error "Error: Unknown LSI. Please use LSI=<LSI name> to specify the LSI")
else
  ifeq (${LSI},AUTO)
    RZG_LSI:=${RZG_AUTO}
  else ifeq (${LSI},G2H)
    RZG_LSI:=${RZG_G2H}
    ifndef LSI_CUT
      # enable compatible function.
      RZG_LSI_CUT_COMPAT := 1
      $(eval $(call add_define,RZG_LSI_CUT_COMPAT))
    else
      # disable compatible function.
      ifeq (${LSI_CUT},30)
        RZG_LSI_CUT:=20
      else
        $(error "Error: ${LSI_CUT} is not supported.")
      endif
      $(eval $(call add_define,RZG_LSI_CUT))
    endif
  else ifeq (${LSI},G2M)
    RZG_LSI:=${RZG_G2M}
    ifndef LSI_CUT
      # enable compatible function.
      RZG_LSI_CUT_COMPAT := 1
      $(eval $(call add_define,RZG_LSI_CUT_COMPAT))
    else
      # disable compatible function.
      ifeq (${LSI_CUT},11)
        RZG_LSI_CUT:=1
      else ifeq (${LSI_CUT},13)
        RZG_LSI_CUT:=3
      else ifeq (${LSI_CUT},30)
        RZG_LSI_CUT:=20
      else
        $(error "Error: ${LSI_CUT} is not supported.")
      endif
      $(eval $(call add_define,RZG_LSI_CUT))
    endif
  else ifeq (${LSI},G2N)
    RZG_LSI:=${RZG_G2N}
    ifndef LSI_CUT
      # enable compatible function.
      RZG_LSI_CUT_COMPAT := 1
      $(eval $(call add_define,RZG_LSI_CUT_COMPAT))
    else
      # disable compatible function.
      ifeq (${LSI_CUT},11)
        RZG_LSI_CUT:=1
      else
        $(error "Error: ${LSI_CUT} is not supported.")
      endif
      $(eval $(call add_define,RZG_LSI_CUT))
    endif
  else ifeq (${LSI},G2E)
    RZG_LSI:=${RZG_G2E}
    ifndef LSI_CUT
      # enable compatible function.
      RZG_LSI_CUT_COMPAT := 1
      $(eval $(call add_define,RZG_LSI_CUT_COMPAT))
    else
      # disable compatible function.
      ifeq (${LSI_CUT},10)
        RZG_LSI_CUT:=0
      else ifeq (${LSI_CUT},11)
        RZG_LSI_CUT:=1
      else
        $(error "Error: ${LSI_CUT} is not supported.")
      endif
      $(eval $(call add_define,RZG_LSI_CUT))
    endif
  else
    $(error "Error: ${LSI} is not supported.")
  endif
  $(eval $(call add_define,RZG_LSI))
endif

# lock RPC HYPERFLASH access by default
# unlock to repogram the ATF firmware from u-boot
ifndef RZG_RPC_HYPERFLASH_LOCKED
RZG_RPC_HYPERFLASH_LOCKED := 1
endif
$(eval $(call add_define,RZG_RPC_HYPERFLASH_LOCKED))

# Process RZG2_SECURE_BOOT flag
ifndef RZG2_SECURE_BOOT
RZG2_SECURE_BOOT := 0
endif
$(eval $(call add_define,RZG2_SECURE_BOOT))

# Process RZG_QOS_TYPE flag
ifndef RZG_QOS_TYPE
RZG_QOS_TYPE := 0
endif
$(eval $(call add_define,RZG_QOS_TYPE))

# Process RZG_DRAM_SPLIT flag
ifndef RZG_DRAM_SPLIT
RZG_DRAM_SPLIT := 0
endif
$(eval $(call add_define,RZG_DRAM_SPLIT))

# Process RZG_BL33_EXECUTION_EL flag
ifndef RZG_BL33_EXECUTION_EL
RZG_BL33_EXECUTION_EL := 0
endif
$(eval $(call add_define,RZG_BL33_EXECUTION_EL))

# Process RZG_LOSSY_ENABLE flag
ifndef RZG_LOSSY_ENABLE
RZG_LOSSY_ENABLE := 0
endif
$(eval $(call add_define,RZG_LOSSY_ENABLE))

# Process LIFEC_DBSC_PROTECT_ENABLE flag
ifndef LIFEC_DBSC_PROTECT_ENABLE
LIFEC_DBSC_PROTECT_ENABLE := 1
endif
$(eval $(call add_define,LIFEC_DBSC_PROTECT_ENABLE))

# Process PMIC_ROHM_BD9571 flag
ifndef PMIC_ROHM_BD9571
PMIC_ROHM_BD9571 := 0
endif
$(eval $(call add_define,PMIC_ROHM_BD9571))

# Process PMIC_LEVEL_MODE flag
ifndef PMIC_LEVEL_MODE
PMIC_LEVEL_MODE := 0
endif
$(eval $(call add_define,PMIC_LEVEL_MODE))

# Process RZG_REF_INT flag
ifndef RZG_REF_INT
RZG_REF_INT := 0
endif
$(eval $(call add_define,RZG_REF_INT))

# Process RZG_REWT_TRAINING flag
ifndef RZG_REWT_TRAINING
RZG_REWT_TRAINING := 1
endif
$(eval $(call add_define,RZG_REWT_TRAINING))

# Process RZG_SYSTEM_SUSPEND flag
ifndef RZG_SYSTEM_SUSPEND
RZG_SYSTEM_SUSPEND := 0
endif
$(eval $(call add_define,RZG_SYSTEM_SUSPEND))

# SYSTEM_SUSPEND requires power control of PMIC etc.
# When executing SYSTEM_SUSPEND other than HiHope RZ/G2N,
# processing equivalent to that implemented in PMIC_ROHM_BD9571 is necessary.
ifeq (${RZG_SYSTEM_SUSPEND},1)
  ifeq (${PMIC_ROHM_BD9571},0)
    $(error "Error: When you want RZG_SYSTEM_SUSPEND to be enable, please also set PMIC_ROHM_BD9571 to enable.")
  endif
endif

# Process RZG_DRAM_LPDDR4_MEMCONF flag
ifndef RZG_DRAM_LPDDR4_MEMCONF
RZG_DRAM_LPDDR4_MEMCONF :=1
endif
$(eval $(call add_define,RZG_DRAM_LPDDR4_MEMCONF))

# Process RZG_DRAM_DDR3L_MEMCONF flag
ifndef RZG_DRAM_DDR3L_MEMCONF
RZG_DRAM_DDR3L_MEMCONF :=1
endif
$(eval $(call add_define,RZG_DRAM_DDR3L_MEMCONF))

# Process RZG_DRAM_DDR3L_MEMDUAL flag
ifndef RZG_DRAM_DDR3L_MEMDUAL
RZG_DRAM_DDR3L_MEMDUAL :=1
endif
$(eval $(call add_define,RZG_DRAM_DDR3L_MEMDUAL))

# Process RZG_BL33_ARG0 flag
ifdef RZG_BL33_ARG0
$(eval $(call add_define,RZG_BL33_ARG0))
endif

#Process RZG_BL2_DCACHE flag
ifndef RZG_BL2_DCACHE
RZG_BL2_DCACHE := 0
endif
$(eval $(call add_define,RZG_BL2_DCACHE))

# Process RZG_DRAM_CHANNEL flag
ifndef RZG_DRAM_CHANNEL
RZG_DRAM_CHANNEL :=15
endif
$(eval $(call add_define,RZG_DRAM_CHANNEL))

#Process RZG_SYSTEM_RESET_KEEPON_DDR flag
ifndef RZG_SYSTEM_RESET_KEEPON_DDR
RZG_SYSTEM_RESET_KEEPON_DDR := 0
endif
$(eval $(call add_define,RZG_SYSTEM_RESET_KEEPON_DDR))

# RZG_SYSTEM_RESET_KEEPON_DDR requires power control of PMIC etc.
# When executing SYSTEM_SUSPEND other than HiHope RZ/G2N Platform,
# processing equivalent to that implemented in PMIC_ROHM_BD9571 is necessary.
# Also, it is necessary to enable RZG_SYSTEM_SUSPEND.
ifeq (${RZG_SYSTEM_RESET_KEEPON_DDR},1)
  ifeq (${PMIC_ROHM_BD9571},0)
    $(error "Error: When you want RZG_SYSTEM_RESET_KEEPON_DDR to be enable, please also set PMIC_ROHM_BD9571 to enable.")
  endif
  ifeq (${RZG_SYSTEM_SUSPEND},0)
    $(error "Error: When you want RZG_SYSTEM_RESET_KEEPON_DDR to be enable, please also set RZG_SYSTEM_SUSPEND to enable.")
  endif
endif

# Enable workarounds for selected Cortex-A53 erratas.
ERRATA_A53_835769  := 1
ifdef ERRATA_A53_843419
ERRATA_A53_843419  := 1
else
OLD_BINUTILS := $(shell ${LD} --fix-cortex-a53-843419 2>&1 >/dev/null | grep 'unrecognized option' >/dev/null; echo $$?)
ifeq ($(OLD_BINUTILS),0)
# Fix ld error 'unrecognized option' with old binutils
TF_LDFLAGS_aarch64    += --fix-cortex-a53
else
TF_LDFLAGS_aarch64    += --fix-cortex-a53-843419
endif
endif
ERRATA_A53_855873  := 1

# Enable workarounds for selected Cortex-A57 erratas.
ERRATA_A57_859972  := 1
ERRATA_A57_813419  := 1

include drivers/renesas/rzg/ddr/ddr.mk
include drivers/renesas/rzg/qos/qos.mk
include drivers/renesas/rzg/pfc/pfc.mk
include lib/libfdt/libfdt.mk

PLAT_INCLUDES	:=	-Idrivers/renesas/rzg/ddr		\
			-Idrivers/renesas/rzg/qos		\
			-Idrivers/renesas/rzg/iic_dvfs		\
			-Idrivers/renesas/rzg/board		\
			-Idrivers/renesas/rzg/delay		\
			-Idrivers/renesas/rzg/rom		\
			-Idrivers/renesas/rzg/scif		\
			-Idrivers/renesas/rzg/emmc		\
			-Idrivers/renesas/rzg/pwrc		\
			-Idrivers/renesas/rzg/io		\
			-Iplat/renesas/rzg			\
			-Iplat/renesas/rzg/include		\
			-Iplat/renesas/rzg/include/registers

PLAT_BL_COMMON_SOURCES	:=	drivers/renesas/rzg/iic_dvfs/iic_dvfs.c \
				plat/renesas/rzg/rzg_common.c

RZG_GIC_SOURCES		:=	drivers/arm/gic/common/gic_common.c	\
				drivers/arm/gic/v2/gicv2_main.c		\
				drivers/arm/gic/v2/gicv2_helpers.c	\
				plat/common/plat_gicv2.c

BL2_SOURCES	+=	${RZG_GIC_SOURCES}				\
			lib/cpus/aarch64/cortex_a53.S			\
			lib/cpus/aarch64/cortex_a57.S			\
			${LIBFDT_SRCS}					\
			common/desc_image_load.c			\
			plat/renesas/rzg/aarch64/platform_common.c	\
			plat/renesas/rzg/aarch64/plat_helpers.S		\
			plat/renesas/rzg/bl2_interrupt_error.c		\
			plat/renesas/rzg/bl2_secure_setting.c		\
			plat/renesas/rzg/bl2_plat_setup.c		\
			plat/renesas/rzg/plat_storage.c			\
			plat/renesas/rzg/bl2_plat_mem_params_desc.c	\
			plat/renesas/rzg/plat_image_load.c		\
			plat/renesas/rzg/bl2_cpg_init.c			\
			drivers/renesas/rzg/console/rzg_printf.c	\
			drivers/renesas/rzg/scif/scif.S			\
			drivers/renesas/rzg/common.c			\
			drivers/renesas/rzg/io/io_emmcdrv.c		\
			drivers/renesas/rzg/io/io_memdrv.c		\
			drivers/renesas/rzg/io/io_rzg.c			\
			drivers/renesas/rzg/auth/auth_mod.c		\
			drivers/renesas/rzg/rpc/rpc_driver.c		\
			drivers/renesas/rzg/dma/dma_driver.c		\
			drivers/renesas/rzg/delay/micro_delay.c		\
			drivers/renesas/rzg/emmc/emmc_interrupt.c	\
			drivers/renesas/rzg/emmc/emmc_utility.c		\
			drivers/renesas/rzg/emmc/emmc_mount.c		\
			drivers/renesas/rzg/emmc/emmc_init.c		\
			drivers/renesas/rzg/emmc/emmc_read.c		\
			drivers/renesas/rzg/emmc/emmc_cmd.c		\
			drivers/renesas/rzg/watchdog/swdt.c		\
			drivers/renesas/rzg/rom/rom_api.c		\
			drivers/renesas/rzg/board/board.c		\
			drivers/io/io_storage.c

BL31_SOURCES	+=	${RZG_GIC_SOURCES}				\
			lib/cpus/aarch64/cortex_a53.S			\
			lib/cpus/aarch64/cortex_a57.S			\
			plat/common/plat_psci_common.c			\
			plat/renesas/rzg/plat_topology.c		\
			plat/renesas/rzg/aarch64/plat_helpers.S		\
			plat/renesas/rzg/aarch64/platform_common.c	\
			plat/renesas/rzg/bl31_plat_setup.c		\
			plat/renesas/rzg/plat_pm.c			\
			drivers/renesas/rzg/console/rzg_console.S	\
			drivers/renesas/rzg/console/rzg_printf.c	\
			drivers/renesas/rzg/delay/micro_delay.c		\
			drivers/renesas/rzg/pwrc/call_sram.S		\
			drivers/renesas/rzg/pwrc/pwrc.c			\
			drivers/renesas/rzg/common.c			\
			drivers/arm/cci/cci.c

include lib/xlat_tables_v2/xlat_tables.mk
include drivers/auth/mbedtls/mbedtls_crypto.mk
PLAT_BL_COMMON_SOURCES	+=	${XLAT_TABLES_LIB_SRCS}

# build the layout images for the bootrom and the necessary srecords
rzg: rzg_layout_create rzg_srecord
distclean realclean clean: clean_layout_tool clean_srecord

# layout images
LAYOUT_TOOLPATH ?= tools/renesas/rzg_layout_create

clean_layout_tool:
	@echo "clean layout tool"
	${Q}${MAKE} -C ${LAYOUT_TOOLPATH} clean

.PHONY: rzg_layout_create
rzg_layout_create:
	@echo "generating layout srecs"
	${Q}${MAKE} CPPFLAGS="-D=AARCH64" ARCH=$(ARCH) --no-print-directory -C ${LAYOUT_TOOLPATH}

# srecords
SREC_PATH	= ${BUILD_PLAT}
BL2_ELF_SRC	= ${SREC_PATH}/bl2/bl2.elf
BL31_ELF_SRC	= ${SREC_PATH}/bl31/bl31.elf

clean_srecord:
	@echo "clean bl2 and bl31 srecs"
	rm -f ${SREC_PATH}/bl2.srec ${SREC_PATH}/bl31.srec

.PHONY: rzg_srecord
rzg_srecord: $(BL2_ELF_SRC) $(BL31_ELF_SRC)
	@echo "generating srec: ${SREC_PATH}/bl2.srec"
	$(Q)$(OC) -O srec --srec-forceS3 ${BL2_ELF_SRC}  ${SREC_PATH}/bl2.srec
	@echo "generating srec: ${SREC_PATH}/bl31.srec"
	$(Q)$(OC) -O srec --srec-forceS3 ${BL31_ELF_SRC} ${SREC_PATH}/bl31.srec
