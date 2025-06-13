
PLAT_BL_COMMON_SOURCES +=	\
			plat/renesas/rz/soc/${PLAT}/drivers/auth/rsip/src/r_cip_drv_api.c 				\
			plat/renesas/rz/soc/${PLAT}/drivers/auth/rsip/src/r_cip_drv_param.c				\
			plat/renesas/rz/soc/${PLAT}/drivers/auth/rsip/src/rsip_drv.c					\
			plat/renesas/rz/soc/${PLAT}/drivers/auth/rsip/src/rsip_drv_private.c

PLAT_INCLUDES +=	\
			-Iplat/renesas/rz/soc/${PLAT}/drivers/auth/rsip/inc

LDLIBS += ${SCE_LIB_DIR}/libr_secure_ip_4_0.a.1.0.0
