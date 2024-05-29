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
# Description: Shell script for updating pre-required files and permissions in the runner directories
#######################################################################################################################

############################################## delete_all_files #######################################################
# 1. Delete all the files and folders from the current directory.
# 2. Assumption: the current working directory should be the runner directory before this function is called.
#######################################################################################################################
delete_all_files()
{
    rm -rf "$(pwd)"/* "$(pwd)"/.[!.]* "$(pwd)"/..?*

    if [ $(ls -A "$(pwd)") ]; then
    	echo "ERROR: Couldn't delete all the files."
    	exit -1
    else
    	echo "Deleted all the files within the runner directory"
    fi
}

############################################# add_compiler_files ######################################################
# 1. Adds the toolchain files, and checks the appropriate permissions are in place for the files.
# 2. Assumption: the current working directory should be the runner directory before this function is called.
#######################################################################################################################
add_compiler_files()
{
    cp -r ../../../../../../File_ref_dir/compiler .

    if [ -d "./compiler" ]; then
    	echo "Copied the compiler files"
    else
    	echo "ERROR: Couldn't copy the compiler files"
    	exit -1
    fi

    if [ "$(stat -c "%a" "./compiler")" != "755" ]; then
        chmod 755 "./compiler"
    fi
    echo "Checked and updated compiler file permissions"
}

############################################## add_uboot_files ########################################################
# 1. Adds the u-boot files, and checks the appropriate permissions are in place for the files.
# 2. Assumption: the current working directory should be the runner directory before this function is called.
#######################################################################################################################
add_uboot_files()
{
    cp -r ../../../../../../File_ref_dir/u-boot .

    if [ -d "./u-boot" ]; then
    	echo "Copied the u-boot files"
    else
    	echo "ERROR: Couldn't copy the u-boot files"
    	exit -1
    fi

    find "./u-boot" -type f | while read -r file; do
        if [ "$(stat -c "%a" "$file")" != "664" ]; then
        chmod 664 "$file"
        fi
    done
    echo "Checked and updated u-boot file permissions"
}

################################################ add_scripts ##########################################################
# 1. Adds the scripts, and checks the appropriate permissions are in place for the files.
# 2. Assumption: the current working directory should be the runner directory before this function is called.
#######################################################################################################################
add_scripts()
{
    cp -r ../../../../../../File_ref_dir/scripts .

    if [ -d "./scripts" ]; then
    	echo "Copied the scripts"
    else
    	echo "ERROR: Couldn't copy the scripts"
    	exit -1
    fi

    if [ "$(stat -c "%a" "./scripts/checkpatch/checkpatch.pl")" != "775" ]; then
        chmod 775 "./scripts/checkpatch/checkpatch.pl"
    fi
    echo "Checked and updated the script permissions"
}

############################################### add_workspace #########################################################
# 1. Creates the workspace directory, and checks the appropriate permissions are in place for it.
# 2. Assumption: the current working directory should be the runner directory before this function is called.
#######################################################################################################################
create_workspace()
{
    mkdir "workspace"

    if [ -d "./workspace" ]; then
    	echo "Created the workspace directory"
    else
    	echo "ERROR: Couldn't create the workspace directory"
    	exit -1
    fi

    if [ "$(stat -c "%a" "./workspace")" != "777" ]; then
        chmod 777 "./workspace"
    fi
    echo "Checked and updated the workspace permissions"
}

################################################### main ##############################################################
# Loops through all the runner directories and updates it
#######################################################################################################################
echo "#################### UPDATING ####################"
for RUN_DIR in ./*/; do
	[ -d "$RUN_DIR" ] || continue
    #Reference folder where the updated files should be kept. This folder shouldn't be a part of this loop.
	if [[ "$(basename $RUN_DIR)" == *"File_ref_dir"* ]]; then
		continue
	fi

    echo "#################### PROCESSING RUNNER:"${RUN_DIR}" ####################"
    cd ${RUN_DIR}0/products/common/bootloader/soc/
    pwd
    delete_all_files
	add_compiler_files
   	add_uboot_files
   	add_scripts
    create_workspace

    #Return to the "builds" directory, for the next runner directory to be picked next iteration.
    cd ../../../../../../
done
echo "#################### FINISHED ####################"
