#! /bin/bash
#######################################################################################################################
# Copyright [2024] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
#
# The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
# and/or its licensors ("Renesas") and subject to statutory and contractual protections.
#
# Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute,
# display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity
# purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE
# SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
# WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
# NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES,
# INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING
# OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may
# be subject to different terms.
#######################################################################################################################

#######################################################################################################################
# Description: Shell script to build MPUs.
#######################################################################################################################

################################################## arguments ##########################################################
PLAT="$1"
BOARD="$2"
BUILD_TYPE="$3"
TARGET_OS="$4"
CONFIGS="$5"
TEST_TYPE="$6"
ERROR_MSG="$7"

if [ "$TARGET_OS" = "" ]; then
	TARGET_OS="Linux"
fi

echo "MPU build configuration:
PLAT        : $PLAT
BOARD       : $BOARD
BUILD_TYPE  : $BUILD_TYPE
TARGET_OS   : $TARGET_OS
CONFIGS     : $CONFIGS
TEST_TYPE   : $TEST_TYPE
ERROR_MSG   : $ERROR_MSG"

################################################## n2h_build ##########################################################
# 1. For tag testing, the current path would be within the workspace directory rather than the default runner tf-a
#	 directory. Hence, the path to u-boot would be different for tag build testing.
# 2. Build commands for building n2h, these are run via the run_command function.
#######################################################################################################################
n2h_build()
{
	U_BOOT_FILE=""
	if [ "$BOARD" = "eval" ] && [ "$TEST_TYPE" = "tag" ]; then
		U_BOOT_FILE="../../u-boot/n2h-u-boot.bin"
	elif [ "$BOARD" = "eval" ] ; then
		U_BOOT_FILE="../u-boot/n2h-u-boot.bin"
	else
		echo "Invalid N2H board: Board doesn't exist"
		exit -1
	fi

	if [ ! -f $U_BOOT_FILE ]; then
		echo "U-boot file doesn't exist, check file path"
		exit -1
	fi

	run_command "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS" "$ERROR_MSG"
	run_command "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS"" fip BL33="$U_BOOT_FILE"" "$ERROR_MSG"
	python3 tools/renesas/rzt2h_boot_param/parameter_block_generator.py --output=param_output
	run_command "make PLAT=$PLAT pkg BOARD=$BOARD ""$CONFIGS" "$ERROR_MSG"
}

################################################## t2h_build ##########################################################
# 1. For tag testing, the current path would be within the workspace directory rather than the default runner tf-a
#	 directory. Hence, the path to u-boot would be different for tag build testing.
# 2. Build commands for building t2h, these are run via the run_command function.
#######################################################################################################################
t2h_build()
{
	U_BOOT_FILE=""
	if [ "$BOARD" = "dev_1" ] && [ "$TEST_TYPE" = "tag" ]; then
		U_BOOT_FILE="../../u-boot/t2h-u-boot.bin"
	elif [ "$BOARD" = "dev_1" ]; then
		U_BOOT_FILE="../u-boot/t2h-u-boot.bin"
	else
		echo "Invalid T2H board: Board doesn't exist"
		exit -1
	fi

	if [ ! -f $U_BOOT_FILE ]; then
		echo "U-boot file doesn't exist, check file path"
		exit -1
	fi

	run_command "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS" "$ERROR_MSG"
	run_command "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS"" fip BL33="$U_BOOT_FILE"" "$ERROR_MSG"
	python3 tools/renesas/rzt2h_boot_param/parameter_block_generator.py --output=param_output
	run_command "make PLAT=$PLAT pkg BOARD=$BOARD ""$CONFIGS" "$ERROR_MSG"
}

################################################## g3s_build ##########################################################
# 1. For tag testing, the current path would be within the workspace directory rather than the default runner tf-a
#	 directory. Hence, the path to u-boot would be different for tag build testing.
# 2. Build commands for building g3s, these are run via the run_command function.
#######################################################################################################################
g3s_build()
{
	U_BOOT_FILE=""
	if [ "$BOARD" = "smarc" ] && [ "$TEST_TYPE" = "tag" ]; then
		U_BOOT_FILE="../../u-boot/g3s-u-boot.bin"
	elif [ "$BOARD" = "smarc" ]; then
		U_BOOT_FILE="../u-boot/g3s-u-boot.bin"
	elif [ "$BOARD" = "dev14_1_lpddr" ] && [ "$TEST_TYPE" = "tag" ]; then
		U_BOOT_FILE="../../u-boot/g3s-dev-u-boot.bin"
	elif [ "$BOARD" = "dev14_1_lpddr" ]; then
		U_BOOT_FILE="../u-boot/g3s-dev-u-boot.bin"
	else
		echo "Invalid G3S board: Board doesn't exist"
		exit -1
	fi

	if [ ! -f $U_BOOT_FILE ]; then
		echo "U-boot file doesn't exist, check file path"
		exit -1
	fi

	TFA_PATH=$(pwd)
	FIP_TOOL_PATH="$TFA_PATH/tools/fiptool"
	BP_TOOL_PATH="$TFA_PATH/tools/renesas/rz_boot_param"
	BUILD_PATH="$TFA_PATH/build/$PLAT/$BUILD_TYPE"

	run_command "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS" "$ERROR_MSG"

	cd "$FIP_TOOL_PATH"
	run_command "make fiptool" "$ERROR_MSG"
	run_command "./fiptool create --align 16 --soc-fw $BUILD_PATH/bl31.bin --nt-fw ../../$U_BOOT_FILE $BUILD_PATH/fip.bin" "$ERROR_MSG"

	cd "$BP_TOOL_PATH"
	run_command "make PLAT=$PLAT bptool BOARD=$BOARD" "$ERROR_MSG"
	run_command "$BP_TOOL_PATH/bptool $BUILD_PATH/bl2.bin $BUILD_PATH/bp_spi.bin 0xA3000 spi" "$ERROR_MSG"
	run_command "cat $BUILD_PATH/bp_spi.bin $BUILD_PATH/bl2.bin > $BUILD_PATH/bl2_bp_spi.bin" "$ERROR_MSG"
	run_command "$BP_TOOL_PATH/bptool $BUILD_PATH/bl2.bin $BUILD_PATH/bp_mmc.bin 0xA3000 mmc" "$ERROR_MSG"
	run_command "cat $BUILD_PATH/bp_mmc.bin $BUILD_PATH/bl2.bin > $BUILD_PATH/bl2_bp_mmc.bin" "$ERROR_MSG"
	cd "$TFA_PATH"
	run_command "${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0x0000 --srec-forceS3 $BUILD_PATH/fip.bin $BUILD_PATH/fip.srec" "$ERROR_MSG"
	run_command "${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0xA1E00 --srec-forceS3 $BUILD_PATH/bl2_bp_spi.bin $BUILD_PATH/bl2_bp_spi.srec" "$ERROR_MSG"
	run_command "${CROSS_COMPILE}objcopy -I binary -O srec --adjust-vma=0xA1E00 --srec-forceS3 $BUILD_PATH/bl2_bp_mmc.bin $BUILD_PATH/bl2_bp_mmc.srec" "$ERROR_MSG"
}

################################################## v2h_build ##########################################################
# 1. For tag testing, the current path would be within the workspace directory rather than the default runner tf-a
#	 directory. Hence, the path to u-boot would be different for tag build testing.
# 2. Build commands for building v2h, these are run via the run_command function.
#######################################################################################################################
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
		exit -1
	fi

	if [ ! -f $U_BOOT_FILE ]; then
		echo "U-boot file doesn't exist, check file path"
		exit -1
	fi

	run_command "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS"" fip BL33="$U_BOOT_FILE"" "$ERROR_MSG"
	run_command "make PLAT=$PLAT bptool BOARD=$BOARD" "$ERROR_MSG"
	run_command "make PLAT=$PLAT pkg BOARD=$BOARD ""$CONFIGS" "$ERROR_MSG"
}

################################################## v2n_build ##########################################################
# 1. For tag testing, the current path would be within the workspace directory rather than the default runner tf-a
#	 directory. Hence, the path to u-boot would be different for tag build testing.
# 2. Build commands for building v2n, these are run via the run_command function.
#######################################################################################################################
v2n_build()
{
	U_BOOT_FILE=""
	if [ "$BOARD" = "dev_1" ] && [ "$TEST_TYPE" = "tag" ]; then
		U_BOOT_FILE="../../u-boot/v2n-dev-1-u-boot.bin"
	elif [ "$BOARD" = "dev_1" ]; then
		U_BOOT_FILE="../u-boot/v2n-dev-1-u-boot.bin"
	elif [ "$BOARD" = "evk_1" ] && [ "$TEST_TYPE" = "tag" ]; then
		U_BOOT_FILE="../../u-boot/v2n-evk-1-u-boot.bin"
	elif [ "$BOARD" = "evk_1" ] ; then
		U_BOOT_FILE="../u-boot/v2n-evk-1-u-boot.bin"
	else
		echo "Invalid V2N board: Board doesn't exist"
		exit -1
	fi

	if [ ! -f $U_BOOT_FILE ]; then
		echo "U-boot file doesn't exist, check file path"
		exit -1
	fi

	run_command "make PLAT=$PLAT all BOARD=$BOARD ""$CONFIGS"" fip BL33="$U_BOOT_FILE"" "$ERROR_MSG"
	run_command "make PLAT=$PLAT bptool BOARD=$BOARD" "$ERROR_MSG"
	run_command "make PLAT=$PLAT pkg BOARD=$BOARD ""$CONFIGS" "$ERROR_MSG"
}

################################################ tfa_build_windows ####################################################
# 1. Build commands for building MPU(PLAT) in a Windows environment.
#######################################################################################################################
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

################################################# run command #########################################################
# 1. Runs the build command passed as an argument, and stores the output in the LOG_FILE.
# 2. ERR_MSG = "" indicates it is a success test case job. If an error is found in the log in this case, then the
#    job should fail.
# 3. ERR_MSG != "" indicates it is a failure test case job. This error could be for any build command. So, first the
#    keyword "error:" is searched for, if found, the ERR_MSG is searched for in the log. If this is found, then the job
#    has failed as expected. If this is not found despite an error in the log, then the job has failed for some other
#	 unexpected reason.
#######################################################################################################################
run_command()
{
	BUILD_COMMAND=$1
	ERR_MSG=$2

	LOG_FILE=../mpu_log.txt
	eval "$BUILD_COMMAND" 2>&1 | tee $LOG_FILE
	if [ "$ERR_MSG" = "" ] && grep -q -i "error:" $LOG_FILE; then
		echo "ERROR: This should be a success test case, unexpected error caused failure."
		exit -1
	elif [ "$ERR_MSG" != "" ] && grep -q -i "error:" $LOG_FILE && grep -q -i "${ERR_MSG}" $LOG_FILE; then
		echo "PASS: This is a failure test case. An expected error caused failure."
		exit 0
	elif [ "$ERR_MSG" != "" ] && grep -q -i "error:" $LOG_FILE && ! grep -q -i "${ERR_MSG}" $LOG_FILE; then
		echo "ERROR: This is a failure test case, however, an unexpected error caused failure."
		exit -1
	else
		echo ""
	fi
}

############################################### final_job_status_check ################################################
# 1. This was written specifically for a failure test case job. In case a failure test case job builds successfully,
#    then it would be caught in this function.
# 2. ERR_MSG != "" indicates it is a failure test case. And a failure test case should not execute this far. It should
#	 exit with either code 0, or -1 within the run_command function when running the build commands.
#######################################################################################################################
final_job_status_check()
{
	if [ "$ERR_MSG" != "" ]; then
		echo "ERROR: This is a failure test case, but, it build successfully."
		exit -1
	else
		exit 0
	fi
}

################################################## Build execution #######################################################
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

#Final check in case failure test case jobs have not failed yet
final_job_status_check
