#! /bin/bash

PLAT="$1"
BOARD="$2"
BUILD_TYPE="$3"
CONFIGS="$4"

n2h_build()
{
	eval "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS"
	eval "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS"" fip BL33=../u-boot/n2h-u-boot.bin"
	python3 tools/renesas/rzt2h_boot_param/parameter_block_generator.py --output=param_output
	eval "make PLAT=$PLAT pkg BOARD=$BOARD ""$CONFIGS"
}

t2h_build()
{
	eval "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS"
	eval "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS"" fip BL33=../u-boot/t2h-u-boot.bin"
	python3 tools/renesas/rzt2h_boot_param/parameter_block_generator.py --output=param_output
	eval "make PLAT=$PLAT pkg BOARD=$BOARD ""$CONFIGS"
}

g3s_build()
{
	TFA_PATH=$(pwd)
	FIP_TOOL_PATH="$TFA_PATH/tools/fiptool"
	BP_TOOL_PATH="$TFA_PATH/tools/renesas/rz_boot_param"
	BUILD_PATH="$TFA_PATH/build/$PLAT/$BUILD_TYPE"

	eval "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS"

	cd "$FIP_TOOL_PATH"
	make fiptool
	eval ./fiptool create --align 16 --soc-fw "$BUILD_PATH"/bl31.bin --nt-fw "../../../u-boot/g3s-u-boot.bin" "$BUILD_PATH"/fip.bin

	cd "$BP_TOOL_PATH"
	make PLAT=$PLAT bptool BOARD=$BOARD
	${BP_TOOL_PATH}/bptool "$BUILD_PATH"/bl2.bin "$BUILD_PATH/bp_spi.bin" 0xA3000 spi
	cat "$BUILD_PATH"/bp_spi.bin "$BUILD_PATH"/bl2.bin > "$BUILD_PATH"/bl2_bp_spi.bin
	${BP_TOOL_PATH}/bptool "$BUILD_PATH"/bl2.bin "$BUILD_PATH/bp_mmc.bin" 0xA3000 mmc
	cat "$BUILD_PATH"/bp_mmc.bin "$BUILD_PATH"/bl2.bin > "$BUILD_PATH"/bl2_bp_mmc.bin
	cd "$TFA_PATH"
	${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0x0000 --srec-forceS3 "$BUILD_PATH"/fip.bin "$BUILD_PATH"/fip.srec
	${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0xA1E00 --srec-forceS3 "$BUILD_PATH"/bl2_bp_spi.bin "$BUILD_PATH"/bl2_bp_spi.srec
	${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0xA1E00 --srec-forceS3 "$BUILD_PATH"/bl2_bp_mmc.bin "$BUILD_PATH"/bl2_bp_mmc.srec
}

v2h_build()
{
	U_BOOT_FILE=""
	if [ "$BOARD" = "evk_alpha" ]; then
		U_BOOT_FILE="v2h-evk-al-u-boot.bin"
	elif [ "$BOARD" = "evk_1" ]; then
		U_BOOT_FILE="v2h-evk-1-u-boot.bin"
	elif [ "$BOARD" = "dev_1" ]; then
		U_BOOT_FILE="v2h-dev-1-u-boot.bin"
	else
		echo "Invalid V2H board: Board doesn't exist"
	fi

	eval "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS"" fip BL33=../u-boot/""$U_BOOT_FILE"
	make PLAT=$PLAT bptool BOARD=$BOARD
	eval "make PLAT=$PLAT pkg BOARD=$BOARD ""$CONFIGS"
}

#Platform specific execution
if [ "$PLAT" = "n2h" ]; then
	n2h_build
elif [ "$PLAT" = "t2h" ]; then
	t2h_build
elif [ "$PLAT" = "g3s" ]; then
	g3s_build
elif [ "$PLAT" = "v2h" ]; then
	v2h_build
else
	echo "Invalid platform: Platform doesn't exist"
	exit -1
fi
