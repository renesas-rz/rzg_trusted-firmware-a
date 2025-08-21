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
PIPELINE_TYPE="$6"
ERROR_MSG="$7"
Bl33="$8"

echo "MPU build configuration:
PLAT        	: $PLAT
BOARD       	: $BOARD
BUILD_TYPE  	: $BUILD_TYPE
TARGET_OS   	: $TARGET_OS
CONFIGS     	: $CONFIGS
PIPELINE_TYPE	: $PIPELINE_TYPE
ERROR_MSG   	: $ERROR_MSG
BL33			: $BL33"

################################################## g3l_build ##########################################################
# 1. For release and merge gateway pipelines, the current path would be within the workspace directory rather than the
#	 default runner tf-a directory. Hence, the path to u-boot would be different for these two cases.
# 2. Build command for building g3l, these are run via the run_command function.
#######################################################################################################################
g3l_build()
{
	U_BOOT_FILE=""
	if [[ "$BOARD" = "smarc" ]] && [[ "$PIPELINE_TYPE" = "release" || "$PIPELINE_TYPE" = "merge" ]]; then
		pwd
		U_BOOT_FILE="../../u-boot/g3l-smarc-u-boot.bin"
	elif [[ "$BOARD" = "smarc" ]]; then
		U_BOOT_FILE="../u-boot/g3l-smarc-u-boot.bin"
	elif [[ "$BOARD" = "dev_1" ]] && [[ "$PIPELINE_TYPE" = "release" || "$PIPELINE_TYPE" = "merge" ]]; then
		U_BOOT_FILE="../../u-boot/g3l-dev-1-u-boot.bin"
	elif [[ "$BOARD" = "dev_1" ]]; then
		U_BOOT_FILE="../u-boot/g3l-dev-1-u-boot.bin"
	else
		echo "Invalid G3L board: Board doesn't exist"
		exit 1
	fi

	check_file_exists "$U_BOOT_FILE"

	make PLAT=$PLAT realclean BOARD=$BOARD
	run_command "make PLAT=$PLAT BOARD=$BOARD ""$CONFIGS"" BL33="$U_BOOT_FILE" bl2 fip bptool pkg" "$ERROR_MSG"
}

################################################## n2h_build ##########################################################
# 1. For release and merge gateway pipelines, the current path would be within the workspace directory rather than the
#	 default runner tf-a directory. Hence, the path to u-boot would be different for these two cases.
# 2. Build command for building n2h, these are run via the run_command function.
#######################################################################################################################
n2h_build()
{
	BL33_FILE=""
	if [[ "$PIPELINE_TYPE" = "release" || "$PIPELINE_TYPE" = "merge" ]]; then
		BL33_PATH="../../"
		CONFIG_EXT="SCE_LIB_DIR="../../""
	else
		BL33_PATH="../"
		CONFIG_EXT="SCE_LIB_DIR="../""
	fi

	if [[ "$BL33" = "U-Boot" ]]; then
		if [[ "$BOARD" = "eval" ]]; then
			BL33_FILE="${BL33_PATH}u-boot/n2h-u-boot.bin"
		else
			echo "Invalid N2H board: Board doesn't exist"
			exit 1
		fi
	elif [[ "$BL33" = "TFTF" ]]; then
		BL33_FILE="${BL33_PATH}tftf/tftf_t2h.bin"
	else
		echo "Invalid BL33 Type. It should be either U-Boot or TFTF"
		exit 1
	fi

	check_file_exists "$BL33_FILE"

	make PLAT=$PLAT realclean BOARD=$BOARD
	run_command "make PLAT=$PLAT BOARD=$BOARD ""$CONFIGS"" $CONFIG_EXT"" BL33="$BL33_FILE" bl2 fip pkg" "$ERROR_MSG"
}

################################################## t2h_build ##########################################################
# 1. For release and merge gateway pipelines, the current path would be within the workspace directory rather than the
#	 default runner tf-a directory. Hence, the path to u-boot would be different for these two cases.
# 2. Build command for building t2h, these are run via the run_command function.
#######################################################################################################################
t2h_build()
{
	BL33_FILE=""
	if [[ "$PIPELINE_TYPE" = "release" || "$PIPELINE_TYPE" = "merge" ]]; then
		BL33_PATH="../../"
		CONFIG_EXT="SCE_LIB_DIR="../../""
	else
		BL33_PATH="../"
		CONFIG_EXT="SCE_LIB_DIR="../""
	fi

	if [[ "$BL33" = "U-Boot" ]]; then
		if [[ "$BOARD" = "dev_1" ]]; then
			BL33_FILE="${BL33_PATH}u-boot/t2h-u-boot.bin"
		else
			echo "Invalid T2H board: Board doesn't exist"
			exit 1
		fi
	elif [[ "$BL33" = "TFTF" ]]; then
		BL33_FILE="${BL33_PATH}tftf/tftf_t2h.bin"
	else
		echo "Invalid BL33 Type. It should be either U-Boot or TFTF"
		exit 1
	fi

	check_file_exists "$BL33_FILE"

	make PLAT=$PLAT realclean BOARD=$BOARD
	run_command "make PLAT=$PLAT BOARD=$BOARD ""$CONFIGS"" $CONFIG_EXT"" BL33="$BL33_FILE" bl2 fip pkg" "$ERROR_MSG"
}

################################################## g3s_build ##########################################################
# 1. For release and merge gateway pipelines, the current path would be within the workspace directory rather than the
#	 default runner tf-a directory. Hence, the path to u-boot would be different for these two cases.
# 2. Build command for building g3s, these are run via the run_command function.
#######################################################################################################################
g3s_build()
{
	BL33_FILE=""
	if [[ "$PIPELINE_TYPE" = "release" || "$PIPELINE_TYPE" = "merge" ]]; then
		BL33_PATH="../../"
	else
		BL33_PATH="../"
	fi

	if [[ "$BL33" = "U-Boot" ]]; then
		if [[ "$BOARD" = "smarc" ]]; then
			BL33_FILE="${BL33_PATH}u-boot/g3s-u-boot.bin"
		elif [[ "$BOARD" = "dev14_1_lpddr" ]]; then
			BL33_FILE="${BL33_PATH}u-boot/g3s-dev-u-boot.bin"
		else
			echo "Invalid G3S board: Board doesn't exist"
			exit 1
		fi
	elif [[ "$BL33" = "TFTF" ]]; then
		BL33_FILE="${BL33_PATH}tftf/tftf_g3s.bin"
	else
		echo "Invalid BL33 Type. It should be either U-Boot or TFTF"
		exit 1
	fi

	check_file_exists "$BL33_FILE"

	make PLAT=$PLAT realclean BOARD=$BOARD
	run_command "make PLAT=$PLAT BOARD=$BOARD ""$CONFIGS"" BL33="$BL33_FILE" bl2 fip bptool pkg" "$ERROR_MSG"
}

################################################## v2h_build ##########################################################
# 1. For release and merge gateway pipelines, the current path would be within the workspace directory rather than the
#	 default runner tf-a directory. Hence, the path to u-boot would be different for these two cases.
# 2. Build command for building v2h, these are run via the run_command function.
#######################################################################################################################
v2h_build()
{
	BL33_FILE=""
	if [[ "$PIPELINE_TYPE" = "release" || "$PIPELINE_TYPE" = "merge" ]]; then
		BL33_PATH="../../"
	else
		BL33_PATH="../"
	fi

	if [[ "$BL33" = "U-Boot" ]]; then
		if [[ "$BOARD" = "evk_alpha" ]]; then
			BL33_FILE="${BL33_PATH}u-boot/v2h-evk-al-u-boot.bin"
		elif [[ "$BOARD" = "evk_1" ]]; then
			BL33_FILE="${BL33_PATH}u-boot/v2h-evk-1-u-boot.bin"
		elif [[ "$BOARD" = "evk_2" ]]; then
			BL33_FILE="${BL33_PATH}u-boot/v2h-evk-2-u-boot.bin"
		elif [[ "$BOARD" = "dev_1" ]]; then
			BL33_FILE="${BL33_PATH}u-boot/v2h-dev-1-u-boot.bin"
		else
			echo "Invalid V2H board: Board doesn't exist"
			exit 1
		fi
	elif [[ "$BL33" = "TFTF" ]]; then
		BL33_FILE="${BL33_PATH}tftf/tftf_v2h.bin"
	else
		echo "Invalid BL33 Type. It should be either U-Boot or TFTF"
		exit 1
	fi

	check_file_exists "$BL33_FILE"

	make PLAT=$PLAT realclean BOARD=$BOARD
	if [[ $CONFIGS == *"TRUSTED_BOARD_BOOT=1"* ]]; then
		run_command "make PLAT=$PLAT BOARD=$BOARD ""$CONFIGS"" bl2 bl31" "$ERROR_MSG"
	else
		run_command "make PLAT=$PLAT BOARD=$BOARD ""$CONFIGS"" BL33="$BL33_FILE" bl2 fip bptool pkg" "$ERROR_MSG"
	fi
}

################################################## g3e_build ##########################################################
# 1. For release and merge gateway pipelines, the current path would be within the workspace directory rather than the
#	 default runner tf-a directory. Hence, the path to u-boot would be different for these two cases.
# 2. Build command for building g3e, these are run via the run_command function.
#######################################################################################################################
g3e_build()
{
	BL33_FILE=""
	if [[ "$PIPELINE_TYPE" = "release" || "$PIPELINE_TYPE" = "merge" ]]; then
		BL33_PATH="../../"
	else
		BL33_PATH="../"
	fi

	if [[ "$BL33" = "U-Boot" ]]; then
		if [[ "$BOARD" = "dev_1" ]]; then
			BL33_FILE="${BL33_PATH}u-boot/g3e-u-boot.bin"
		elif [[ "$BOARD" = "smarc" ]]; then
			BL33_FILE="${BL33_PATH}u-boot/g3e-smarc-u-boot.bin"
		else
			echo "Invalid G3E board: Board doesn't exist"
			exit 1
		fi
	elif [[ "$BL33" = "TFTF" ]]; then
		BL33_FILE="${BL33_PATH}tftf/tftf_g3e.bin"
	else
		echo "Invalid BL33 Type. It should be either U-Boot or TFTF"
		exit 1
	fi

	check_file_exists "$BL33_FILE"

	make PLAT=$PLAT realclean BOARD=$BOARD
	run_command "make PLAT=$PLAT BOARD=$BOARD ""$CONFIGS"" BL33="$BL33_FILE" bl2 fip bptool pkg" "$ERROR_MSG"
}

################################################## v2n_build ##########################################################
# 1. For release and merge gateway pipelines, the current path would be within the workspace directory rather than the
#	 default runner tf-a directory. Hence, the path to u-boot would be different for these two cases.
# 2. Build command for building v2n, these are run via the run_command function.
#######################################################################################################################
v2n_build()
{
	BL33_FILE=""
	if [[ "$PIPELINE_TYPE" = "release" || "$PIPELINE_TYPE" = "merge" ]]; then
		BL33_PATH="../../"
	else
		BL33_PATH="../"
	fi

	if [[ "$BL33" = "U-Boot" ]]; then
		if [[ "$BOARD" = "evk_1" ]]; then
			BL33_FILE="${BL33_PATH}u-boot/v2n-evk-1-u-boot.bin"
		elif [[ "$BOARD" = "evk_2" ]]; then
			BL33_FILE="${BL33_PATH}u-boot/v2n-evk-2-u-boot.bin"
		else
			echo "Invalid V2N board: Board doesn't exist"
			exit 1
		fi
	elif [[ "$BL33" = "TFTF" ]]; then
		BL33_FILE="${BL33_PATH}tftf/tftf_v2n.bin"
	else
		echo "Invalid BL33 Type. It should be either U-Boot or TFTF"
		exit 1
	fi

	check_file_exists "$BL33_FILE"

	make PLAT=$PLAT realclean BOARD=$BOARD
	if [[ $CONFIGS == *"TRUSTED_BOARD_BOOT=1"* ]]; then
		run_command "make PLAT=$PLAT BOARD=$BOARD ""$CONFIGS"" bl2 bl31" "$ERROR_MSG"
	else
		run_command "make PLAT=$PLAT BOARD=$BOARD ""$CONFIGS"" BL33="$BL33_FILE" bl2 fip bptool pkg" "$ERROR_MSG"
	fi
}

################################################## g2l_build ##########################################################
# 1. For release and merge gateway pipelines, the current path would be within the workspace directory rather than the
#	 default runner tf-a directory. Hence, the path to u-boot would be different for these two cases.
# 2. Build command for building g2l, these are run via the run_command function.
#######################################################################################################################
g2l_build()
{
	BL33_FILE=""
	if [[ "$PIPELINE_TYPE" = "release" || "$PIPELINE_TYPE" = "merge" ]]; then
		BL33_PATH="../../"
	else
		BL33_PATH="../"
	fi

	if [[ "$BL33" = "U-Boot" ]]; then
		if [[ "$BOARD" = "smarc_pmic_2" ]]; then
			BL33_FILE="${BL33_PATH}u-boot/g2l-smarc-pmic-2-u-boot.bin"
		else
			echo "Invalid G2L board: Board doesn't exist"
			exit 1
		fi
	elif [[ "$BL33" = "TFTF" ]]; then
		BL33_FILE="${BL33_PATH}tftf/tftf_g2l.bin"
	else
		echo "Invalid BL33 Type. It should be either U-Boot or TFTF"
		exit 1
	fi

	check_file_exists "$BL33_FILE"

	make PLAT=$PLAT realclean BOARD=$BOARD
	run_command "make PLAT=$PLAT BOARD=$BOARD ""$CONFIGS"" BL33="$BL33_FILE" bl2 fip bptool pkg" "$ERROR_MSG"
}

################################################## g2lc_build ##########################################################
# 1. For release and merge gateway pipelines, the current path would be within the workspace directory rather than the
#	 default runner tf-a directory. Hence, the path to u-boot would be different for these two cases.
# 2. Build command for building g2lc, these are run via the run_command function.
#######################################################################################################################
g2lc_build()
{
	BL33_FILE=""
	if [[ "$PIPELINE_TYPE" = "release" || "$PIPELINE_TYPE" = "merge" ]]; then
		BL33_PATH="../../"
	else
		BL33_PATH="../"
	fi

	if [[ "$BL33" = "U-Boot" ]]; then
		if [[ "$BOARD" = "smarc_1" ]]; then
			BL33_FILE="${BL33_PATH}u-boot/g2lc-smarc-1-u-boot.bin"
		else
			echo "Invalid G2LC board: Board doesn't exist"
			exit 1
		fi
	elif [[ "$BL33" = "TFTF" ]]; then
		BL33_FILE="${BL33_PATH}tftf/tftf_g2l.bin"
	else
		echo "Invalid BL33 Type. It should be either U-Boot or TFTF"
		exit 1
	fi

	check_file_exists "$BL33_FILE"

	make PLAT=$PLAT realclean BOARD=$BOARD
	run_command "make PLAT=$PLAT BOARD=$BOARD ""$CONFIGS"" BL33="$BL33_FILE" bl2 fip bptool pkg" "$ERROR_MSG"

	cd ./build
	mkdir -p $PLAT && cd $PLAT
	cp -r ../g2l/$BUILD_TYPE/ .
}

################################################## g2ul_build ##########################################################
# 1. For release and merge gateway pipelines, the current path would be within the workspace directory rather than the
#	 default runner tf-a directory. Hence, the path to u-boot would be different for these two cases.
# 2. Build command for building g2ul, these are run via the run_command function.
#######################################################################################################################
g2ul_build()
{
	BL33_FILE=""
	if [[ "$PIPELINE_TYPE" = "release" || "$PIPELINE_TYPE" = "merge" ]]; then
		BL33_PATH="../../"
	else
		BL33_PATH="../"
	fi

	if [[ "$BL33" = "U-Boot" ]]; then
		if [[ "$BOARD" = "g2ul_smarc" ]]; then
			BL33_FILE="${BL33_PATH}u-boot/g2ul-smarc-u-boot.bin"
		else
			echo "Invalid G2UL board: Board doesn't exist"
			exit 1
		fi
	else
		echo "Invalid BL33 Type. It should be U-Boot"
		exit 1
	fi

	check_file_exists "$BL33_FILE"

	make PLAT=$PLAT realclean BOARD=$BOARD
	run_command "make PLAT=$PLAT BOARD=$BOARD ""$CONFIGS"" BL33="$BL33_FILE" bl2 fip bptool pkg" "$ERROR_MSG"
}

################################################## v2l_build ##########################################################
# 1. For release and merge gateway pipelines, the current path would be within the workspace directory rather than the
#	 default runner tf-a directory. Hence, the path to u-boot would be different for these two cases.
# 2. Build command for building v2l, these are run via the run_command function.
#######################################################################################################################
v2l_build()
{
	BL33_FILE=""
	if [[ "$PIPELINE_TYPE" = "release" || "$PIPELINE_TYPE" = "merge" ]]; then
		BL33_PATH="../../"
	else
		BL33_PATH="../"
	fi

	if [[ "$BL33" = "U-Boot" ]]; then
		if [[ "$BOARD" = "smarc_pmic_2" ]]; then
			BL33_FILE="${BL33_PATH}u-boot/v2l-smarc-pmic-2-u-boot.bin"
		else
			echo "Invalid V2L board: Board doesn't exist"
			exit 1
		fi
	else
		echo "Invalid BL33 Type. It should be U-Boot"
		exit 1
	fi

	check_file_exists "$BL33_FILE"

	make PLAT=$PLAT realclean BOARD=$BOARD
	run_command "make PLAT=$PLAT BOARD=$BOARD ""$CONFIGS"" BL33="$BL33_FILE" bl2 fip bptool pkg" "$ERROR_MSG"
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

############################################### check_file_exists ################################################
# 1. Utility function to check if the file path being passed to it as an argument exists.
# 2. If a file doesn't exist it will immeditately print an appropriate message and exit with an error code 1.
#######################################################################################################################
check_file_exists()
{
	FILE_PATH="$1"
	if [ ! -f "$FILE_PATH" ]; then
		echo "File doesn't exist, check file path: "$FILE_PATH""
		exit 1
	fi
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
	BUILD_COMMAND="$1"
	ERR_MSG="$2"

	LOG_FILE=../mpu_log.txt
	eval "$BUILD_COMMAND" 2>&1 | tee "$LOG_FILE"
	if [[ "$ERR_MSG" = "" ]] && { grep -qi "error:" "$LOG_FILE" || grep -Eqi "Error [0-9]+" "$LOG_FILE"; }; then
		echo "ERROR: This should be a success test case, unexpected error caused failure."
		exit 1
	elif [[ "$ERR_MSG" != "" ]] && { grep -qi "error:" "$LOG_FILE" || grep -Eqi "Error [0-9]+" "$LOG_FILE"; } && grep -qi "${ERR_MSG}" "$LOG_FILE"; then
		echo "PASS: This is a failure test case. An expected error caused failure."
		exit 0
	elif [[ "$ERR_MSG" != "" ]] && { grep -qi "error:" "$LOG_FILE" || grep -Eqi "Error [0-9]+" "$LOG_FILE"; } && ! grep -qi "${ERR_MSG}" "$LOG_FILE"; then
		echo "ERROR: This is a failure test case, however, an unexpected error caused failure."
		exit 1
	fi
}

############################################### final_job_status_check ################################################
# 1. This was written specifically for a failure test case job. In case a failure test case job builds successfully,
#    then it would be caught in this function.
# 2. ERR_MSG != "" indicates it is a failure test case. And a failure test case should not execute this far. It should
#	 exit with either code 0, or 1 within the run_command function when running the build commands.
#######################################################################################################################
final_job_status_check()
{
	if [ "$ERR_MSG" != "" ]; then
		echo "ERROR: This is a failure test case, but, it build successfully."
		exit 1
	else
		exit 0
	fi
}

################################################## Build execution #######################################################
# Linux specific execution
if [ "$PLAT" = "g3l" ] && [ "$TARGET_OS" != "windows" ]; then
	g3l_build
elif [ "$PLAT" = "n2h" ] && [ "$TARGET_OS" != "windows" ]; then
	n2h_build
elif [ "$PLAT" = "t2h" ] && [ "$TARGET_OS" != "windows" ]; then
	t2h_build
elif [ "$PLAT" = "g3s" ] && [ "$TARGET_OS" != "windows" ]; then
	g3s_build
elif [ "$PLAT" = "v2h" ] && [ "$TARGET_OS" != "windows" ]; then
	v2h_build
elif [ "$PLAT" = "g3e" ] && [ "$TARGET_OS" != "windows" ]; then
	g3e_build
elif [ "$PLAT" = "v2n" ] && [ "$TARGET_OS" != "windows" ]; then
	v2n_build
elif [ "$PLAT" = "g2l" ] && [ "$TARGET_OS" != "windows" ]; then
	g2l_build
elif [ "$PLAT" = "g2lc" ] && [ "$TARGET_OS" != "windows" ]; then
	g2lc_build
elif [ "$PLAT" = "g2ul" ] && [ "$TARGET_OS" != "windows" ]; then
	g2ul_build
elif [ "$PLAT" = "v2l" ] && [ "$TARGET_OS" != "windows" ]; then
	v2l_build

# Windows specific execution
elif [ "$PLAT" = "g3l" ] && [ "$TARGET_OS" = "windows" ]; then
	tfa_build_windows
elif [ "$PLAT" = "n2h" ] && [ "$TARGET_OS" = "windows" ]; then
	tfa_build_windows
elif [ "$PLAT" = "t2h" ] && [ "$TARGET_OS" = "windows" ]; then
	tfa_build_windows
elif [ "$PLAT" = "g3s" ] && [ "$TARGET_OS" = "windows" ]; then
	tfa_build_windows
elif [ "$PLAT" = "v2h" ] && [ "$TARGET_OS" = "windows" ]; then
	tfa_build_windows
elif [ "$PLAT" = "g3e" ] && [ "$TARGET_OS" = "windows" ]; then
	tfa_build_windows
elif [ "$PLAT" = "v2n" ] && [ "$TARGET_OS" = "windows" ]; then
	tfa_build_windows

# Error handling
else
	echo "Invalid platform: Platform doesn't exist"
	exit 1
fi

#Final check in case failure test case jobs have not failed yet
final_job_status_check
