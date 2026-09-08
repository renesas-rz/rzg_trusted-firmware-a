
PLAT_BL_COMMON_SOURCES +=	\
			plat/renesas/rz/soc/t2h/drivers/auth/sblib/src/r_sb_api.c		\
			plat/renesas/rz/soc/t2h/drivers/auth/sblib/src/r_sb_cmn_drv.c	\
			plat/renesas/rz/soc/t2h/drivers/auth/sblib/src/r_sb_cmn.c		\
			plat/renesas/rz/soc/t2h/drivers/auth/sblib/src/r_sb_crc.c		\
			plat/renesas/rz/soc/t2h/drivers/auth/sblib/src/r_sb_manifest.c	\
			plat/renesas/rz/soc/t2h/drivers/auth/sblib/src/r_sb_sb.c

PLAT_INCLUDES += \
			-Iplat/renesas/rz/soc/t2h/drivers/auth/sblib/inc
