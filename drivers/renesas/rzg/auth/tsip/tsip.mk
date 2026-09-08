#
# Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

ifeq (${TSIP_LIB_PATH},)
  $(error Error: TSIP_LIB_PATH not set)
endif

TSIP_DIR		:= drivers/renesas/rzg/auth/tsip

LIBTSIP_SRCS	:= $(TSIP_DIR)/wrapper/TSIP_Wrapper_Init_API.c			\
				   $(TSIP_DIR)/wrapper/TSIP_Wrapper_Boot_API.c			\
				   $(TSIP_DIR)/wrapper/TSIP_Wrapper_Local_API.c			\
				   $(TSIP_DIR)/core/TSIP_Core_Init_API.c				\
				   $(TSIP_DIR)/core/TSIP_Core_Boot_API.c				\
				   $(TSIP_DIR)/core/TSIP_Core_Proc_API.c				\
				   $(TSIP_DIR)/core/TSIP_Core_Local_API.c				\
				   $(TSIP_DIR)/stub/TSIP_Driver_nonos.c					\
				   $(TSIP_DIR)/stub/TSIP_Stub_API_nonos.c				\
				   $(TSIP_DIR)/tsip_lib.c

TF_CFLAGS		+= -D_TSIP_SB

PLAT_INCLUDES	+= -I$(TSIP_DIR) -I$(TSIP_DIR)/common/ -I$(TSIP_DIR)/stub/

$(eval $(call MAKE_LIB,tsip))

libraries: $(TSIP_LIB_PATH) $(LIB_DIR)
	@ln -nfs $(TSIP_LIB_PATH) $(LIB_DIR)/libr_secure_ip.a

LDLIBS			+= $(LIB_DIR)/libr_secure_ip.a
