#! /bin/bash

PLAT="$1"
BOARD="$2"
BUILD_TYPE="$3"
TARGET_OS="$4"
CONFIGS="$5"
TEST_TYPE="$6"

if [ "$TARGET_OS" = "" ]; then
	TARGET_OS="Linux"
fi

echo "MPU build configuration:
PLAT        : $PLAT
BOARD       : $BOARD
BUILD_TYPE  : $BUILD_TYPE
TARGET_OS   : $TARGET_OS
CONFIGS     : $CONFIGS
TEST_TYPE   : $TEST_TYPE"

n2h_build()
{
	if [ "$TEST_TYPE" = "tag" ]; then
		U_BOOT_FILE="../../u-boot/n2h-u-boot.bin"
	else
		U_BOOT_FILE="../u-boot/n2h-u-boot.bin"
	fi
	eval "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS"
	eval "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS"" fip BL33="$U_BOOT_FILE""
	python3 tools/renesas/rzt2h_boot_param/parameter_block_generator.py --output=param_output
	eval "make PLAT=$PLAT pkg BOARD=$BOARD ""$CONFIGS"
}

t2h_build()
{
	if [ "$TEST_TYPE" = "tag" ]; then
		U_BOOT_FILE="../../u-boot/t2h-u-boot.bin"
	else
		U_BOOT_FILE="../u-boot/t2h-u-boot.bin"
	fi
	eval "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS"
	eval "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS"" fip BL33="$U_BOOT_FILE""
	python3 tools/renesas/rzt2h_boot_param/parameter_block_generator.py --output=param_output
	eval "make PLAT=$PLAT pkg BOARD=$BOARD ""$CONFIGS"
}

g3s_build()
{
	TFA_PATH=$(pwd)
	FIP_TOOL_PATH="$TFA_PATH/tools/fiptool"
	BP_TOOL_PATH="$TFA_PATH/tools/renesas/rz_boot_param"
	BUILD_PATH="$TFA_PATH/build/$PLAT/$BUILD_TYPE"
	if [ "$TEST_TYPE" = "tag" ]; then
		U_BOOT_FILE="../../../../u-boot/g3s-u-boot.bin"
	else
		U_BOOT_FILE="../../../u-boot/g3s-u-boot.bin"
	fi

	eval "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS"

	cd "$FIP_TOOL_PATH"
	make fiptool
	eval ./fiptool create --align 16 --soc-fw "$BUILD_PATH"/bl31.bin --nt-fw "$U_BOOT_FILE" "$BUILD_PATH"/fip.bin

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
	if [ "$BOARD" = "evk_alpha" ] && [ "$TEST_TYPE" = "tag" ]; then
		U_BOOT_FILE="../../u-boot/v2h-evk-al-u-boot.bin"
	elif [ "$BOARD" = "evk_alpha" ]; then
		U_BOOT_FILE="../u-boot/v2h-evk-al-u-boot.bin"
	elif [ "$BOARD" = "evk_1" ] && [ "$TEST_TYPE" = "tag" ]; then
		U_BOOT_FILE="../../u-boot/v2h-evk-1-u-boot.bin"
	elif [ "$BOARD" = "evk_1" ] ; then
		U_BOOT_FILE="../u-boot/v2h-evk-1-u-boot.bin"
	elif [ "$BOARD" = "dev_1" ] && [ "$TEST_TYPE" = "tag" ]; then
		U_BOOT_FILE="../../u-boot/v2h-dev-1-u-boot.bin"
	elif [ "$BOARD" = "dev_1" ]; then
		U_BOOT_FILE="../u-boot/v2h-dev-1-u-boot.bin"
	else
		echo "Invalid V2H board: Board doesn't exist"
	fi

	eval "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS"" fip BL33="$U_BOOT_FILE""
	make PLAT=$PLAT bptool BOARD=$BOARD
	eval "make PLAT=$PLAT pkg BOARD=$BOARD ""$CONFIGS"
}


v2n_build()
{
	U_BOOT_FILE=""
	if [ "$BOARD" = "dev_1" ] && [ "$TEST_TYPE" = "tag" ]; then
		U_BOOT_FILE="../../u-boot/v2n-dev-1-u-boot.bin"
	elif [ "$BOARD" = "dev_1" ]; then
		U_BOOT_FILE="../u-boot/v2n-dev-1-u-boot.bin"
	else
		echo "Invalid V2N board: Board doesn't exist"
	fi

	eval "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS"" fip BL33="$U_BOOT_FILE""
	make PLAT=$PLAT bptool BOARD=$BOARD
	eval "make PLAT=$PLAT pkg BOARD=$BOARD ""$CONFIGS"
}

tfa_build_windows()
{
	# Path to Windows version of the ARM cross compiler toolchain
	export CROSS_COMPILE=../compiler/gcc-arm-11.2-2022.02-mingw-w64-i686-aarch64-none-elf/bin/aarch64-none-elf-

	# Path to directory containing make.exe
	export PATH=$PATH

	echo "Configuring realclean for $PLAT $BOARD ..."
	make PLAT=$PLAT realclean BOARD=$BOARD

	eval "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS"
}

# Linux specific execution
if [ "$PLAT" = "n2h" ] && [ "$TARGET_OS" != "windows" ]; then
	n2h_build
elif [ "$PLAT" = "t2h" ] && [ "$TARGET_OS" != "windows" ]; then
	t2h_build
elif [ "$PLAT" = "g3s" ] && [ "$TARGET_OS" != "windows" ]; then
	g3s_build
elif [ "$PLAT" = "v2h" ] && [ "$TARGET_OS" != "windows" ]; then
	v2h_build
elif [ "$PLAT" = "v2n" ] && [ "$TARGET_OS" != "windows" ]; then
	v2n_build

# Windows specific execution
elif [ "$PLAT" = "n2h" ] && [ "$TARGET_OS" = "windows" ]; then
	tfa_build_windows
elif [ "$PLAT" = "t2h" ] && [ "$TARGET_OS" = "windows" ]; then
	tfa_build_windows
elif [ "$PLAT" = "g3s" ] && [ "$TARGET_OS" = "windows" ]; then
	tfa_build_windows
elif [ "$PLAT" = "v2h" ] && [ "$TARGET_OS" = "windows" ]; then
	tfa_build_windows
elif [ "$PLAT" = "v2n" ] && [ "$TARGET_OS" = "windows" ]; then
	tfa_build_windows
# Error handling
else
	echo "Invalid platform: Platform doesn't exist"
	exit -1
fi
