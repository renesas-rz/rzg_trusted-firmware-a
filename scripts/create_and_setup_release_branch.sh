#!/bin/bash
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
# Description: Shell script to create and setup release branch.
#######################################################################################################################

################################################## arguments ##########################################################
manual_rel_prompt() {
    var_name="$1"
    prompt_string="$2"

    while [[ -z "${!var_name//[[:space:]]/}" ]]; do
        read -p "$prompt_string: " input
        declare -g "$var_name=${input}"
		release_type="manual"
    done
}

bsp_name="$1"
bsp_release_number="$2"
workspace_path="$3"
list_devices_to_keep="$4"
tfa_version="$5"
branch_name="$6"
release_type="automated"

manual_rel_prompt bsp_name "Enter BSP name (spaces to be replaced with underscores)"
manual_rel_prompt bsp_release_number "Enter BSP release number"
manual_rel_prompt workspace_path "Enter workspace path, spaces to be replaced with underscores"
manual_rel_prompt list_devices_to_keep "Enter the list of devices, space-separated list"
manual_rel_prompt tfa_version "Enter TF-A version"
manual_rel_prompt branch_name "Enter the base branch name for the release"

tfa_release_branch_name="${tfa_version}/rz_rel_${bsp_name}.${bsp_release_number}"
tfa_rel_feature_branch_name="${tfa_version}/rz_rel_feat_rm_dev_${bsp_name}.${bsp_release_number}"
tfa_branch_name="${branch_name}"

BASEDIR=$(dirname "$(realpath "$0")")

EXIT_PASS=0
EXIT_FAIL_GIT_ERROR=1
EXIT_FAIL_INVALID_PATH_ERROR=2
EXIT_FAIL_DEVICE_REMOVAL=3
EXIT_FAIL_UNKNOWN_DEVICE_TYPE=4
EXIT_FAIL_FILE_REMOVAL=5

################################################## file_exists ########################################################
# Checks to see if the path provided exists.
#######################################################################################################################
file_exists() {
    local filepath="$1"

    if [ -e "$filepath" ]; then
        echo "File path '$filepath' exists."
    else
        echo "File path '$filepath' does not exist."
        exit $EXIT_FAIL_INVALID_PATH_ERROR
    fi
}

########################################### delete_lines_from_file ####################################################
# Function to delete lines from a file based on a search term.
#######################################################################################################################
delete_lines_from_file() {
    local search_term="$1"
    local file_to_change="$2"

	file_exists "$file_to_change"
    # Use sed to delete lines containing the search term from the file
    sed -i "/$search_term/d" "$file_to_change"

    echo "Lines containing '$search_term' deleted from $file_to_change."
}

########################################### check_branch_status #######################################################
# Function to check the branch status by matching it with the passed argument.
#######################################################################################################################
check_branch_status(){
	required_status="$1"
	status_check="$(git status | xargs echo)"

	echo "$status_check"
	if [ "$status_check" != "$required_status" ]; then
		echo "git branch is not in the required state"
		exit $EXIT_FAIL_GIT_ERROR
	fi
}

########################################### create_branch_and_check ###################################################
# Function to create a branch and check it's status.
#######################################################################################################################
create_branch_and_check() {
	branch_name="$1"
	echo "creating branch with name ${branch_name}"
	git checkout -b ${branch_name}
	check_branch_status "On branch ${branch_name} nothing to commit, working tree clean"
}

#Check device removal parameter
if [ -z "$list_devices_to_keep" ]; then
	echo "Devices to keep not provided."
	echo "FAIL"
	exit $EXIT_FAIL_UNKNOWN_DEVICE_TYPE
fi

#Setup workspace
file_exists $workspace_path
rm -r -f $workspace_path/*
cd $workspace_path
git clone http://svc_sp3_ci_etcetc:DC_p5HnJoAsvmocwdYer@global-infra-jp-main.dgn.renesas.com/products/common/bootloader/soc/tf-a.git
cd tf-a
tfa_project_path=$(pwd)

#######################################################################################################################
# Step 1: Creating release and feature branches.
#######################################################################################################################
git checkout "${tfa_branch_name}"
if [[ ${release_type} == "manual" ]]; then
	check_branch_status "On branch ${tfa_branch_name} Your branch is up-to-date with origin/${tfa_branch_name}. nothing to commit, working tree clean"
else
	check_branch_status "On branch ${tfa_branch_name} Your branch is up to date with origin/${tfa_branch_name}. nothing to commit, working tree clean"
fi

create_branch_and_check "${tfa_release_branch_name}"
create_branch_and_check "${tfa_rel_feature_branch_name}"

echo "Step 1 Passed - Created release branch and release-feature branch"
echo ""

#######################################################################################################################
# Step 2: Remove code that should be removed for all releases.
#######################################################################################################################
git rm "./.gitlab-ci.yml"
git rm -r "./gitlab-ci"
git rm -r "./.gitlab"
git rm -r "./scripts"

echo "Step 2 Passed - Removed code that should be removed for all releases"

#######################################################################################################################
# Step 3: Remove references to devices.
#######################################################################################################################
all_unreleased_devices_list="n2h,t2h,g3s,v2h,v2n,g3e,g2l,g2lc,g2ul,v2l"

IFS=' ' read -ra array_keep <<< "$list_devices_to_keep"
IFS=',' read -ra array_remove <<< "$all_unreleased_devices_list"

arr_length=${#array_remove[@]}

for device_keep in "${array_keep[@]}"; do
    for ((i=0; i<=arr_length; i++)); do
        if [ "${array_remove[$i]}" = "$device_keep" ]; then
            unset 'array_remove[i]'
			break
        fi
    done
done

list_devices_for_removal=$(IFS=, ; echo "${array_remove[*]}")
echo "List of devices to remove: ${list_devices_for_removal}"
echo "List of devices to keep: ${list_devices_to_keep}"

if [ "$list_devices_for_removal" = "" ]; then
	echo "No devices to remove"
else
	device_count=0
	for device_for_removal in ${list_devices_for_removal//,/ }
	do
		#the only common file to edit.
		delete_lines_from_file "rz-${device_for_removal}" "./docs/plat/index.rst"

		#remove specified directories and files
		git rm -r "$tfa_project_path/plat/renesas/rz/soc/$device_for_removal"
		git rm "$tfa_project_path/plat/renesas/rz/common/${device_for_removal}_common.mk"
		git rm "$tfa_project_path/docs/plat/rz-${device_for_removal}.rst"
		find "$tfa_project_path/plat/renesas/rz/board" -name "${device_for_removal}*" | xargs git rm -r

		#remove files specific to device type
		case $device_for_removal in
		"n2h")
			echo ""
			;;
		"t2h")
			git rm -r "$tfa_project_path/tools/renesas/rzt2h_boot_param"
			;;
		"g3s")
			git rm "${tfa_project_path}/g3s_smarc_build_script.sh"
			;;
		"g3e")
			echo ""
			;;
		"v2n")
			echo ""
			;;
		"v2h")
			echo ""
			;;
		"g2l")
			echo ""
			;;
		"g2lc")
			echo ""
			;;
		"g2ul")
			echo ""
			;;
		"v2l")
			echo ""
			;;
		*)
			echo "Unknown device type parsed."
			exit $EXIT_FAIL_UNKNOWN_DEVICE_TYPE
			;;
		esac
	done

	echo "Step 3 Passed - Devices removed from code."
	echo ""
fi

#######################################################################################################################
# Step 4: Check for removed phrases in file contents.
#######################################################################################################################
find_command="find $tfa_project_path"
exclude_directories=("${tfa_project_path}/.git*" "${tfa_project_path}/docs/resources/diagrams*" "${tfa_project_path}/include/drivers/st*" "${tfa_project_path}/plat/arm*" "${tfa_project_path}/include/drivers/brcm*" "${tfa_project_path}/plat/mediatek*" "${tfa_project_path}/package-lock.json")
for dir in "${exclude_directories[@]}"; do
	find_command+=" ! -path '$dir' -a"
done

exclude_files=("${tfa_project_path}/package-lock.json")
for file in "${exclude_files[@]}"; do
	find_command+=" ! -wholename '$file' -a"
done

list_phrases_to_check="$list_devices_for_removal,gitlab-ci"
for phrase_check_for_removal in ${list_phrases_to_check//,/ }
do
    find_command=${find_command% -a}
	# Use find and grep to search for the term "${phrase_check_for_removal}" excluding specific directories and their subdirectories
	find_command="$find_command -type f -exec grep -i -H '${phrase_check_for_removal}' {} +"

    device_search_result=$(eval "$find_command")

	if [ -z "$device_search_result" ]; then
		echo "References to the ${phrase_check_for_removal} were not found."
	else
		echo "${device_search_result}"
		echo "Not all references removed for ${phrase_check_for_removal}"
		echo "FAIL"
		exit $EXIT_FAIL_DEVICE_REMOVAL
	fi
done

echo "Step 4 Passed - Checked for removal of device references in file contents"
echo ""

#######################################################################################################################
# Step 5: Check for removed phrases in the file and directory names.
#######################################################################################################################
# Input parameters
exclude_directories=("${tfa_project_path}/.git" "${tfa_project_path}/docs")
search_directory="${tfa_project_path}"
search_phrases=($(echo "$list_phrases_to_check" | tr ',' ' '))

find_command="find '$search_directory' -type d"

for phrase in "${search_phrases[@]}"; do
    find_command+=" -iname '*$phrase*' -o"
done

# Remove trailing -o if any
find_command="${find_command% -o}"

find_command+=" -o -type f"
for phrase in "${search_phrases[@]}"; do
    find_command+=" -iname '*$phrase*' -o"
done

# Remove trailing -o if any
find_command="${find_command% -o}"

for dir in "${exclude_directories[@]}"; do
    find_command+=" | grep -v '$dir'"
done

search_results=$(eval "$find_command")

if [ -z "$search_results" ]; then
    echo "No matches found in '$search_directory'."
else
    echo "Matches found in '$search_directory':"
    echo "$search_results"
	echo ""
	echo "FAIL"
	exit $EXIT_FAIL_DEVICE_REMOVAL
fi

echo "Step 5 Passed - Checked for removal of device references in directory and file names"
echo ""

#######################################################################################################################
# Step 6: Perform git commit commands.
#######################################################################################################################
git add .
git commit -m "Removed devices: $list_devices_for_removal. Removed gitlab ci files."
check_branch_status "On branch ${tfa_rel_feature_branch_name} nothing to commit, working tree clean"
echo "Step 6 Passed - Changes committed."
echo ""

#######################################################################################################################
# Step 7: Running and printing final commands.
#######################################################################################################################
git checkout ${tfa_release_branch_name}
git merge ${tfa_rel_feature_branch_name}
git branch --delete ${tfa_rel_feature_branch_name}
cd ../../tf-a

if [[ ${release_type} == "manual" ]]; then
	echo "Run the following command:"
	echo "./scripts/create_tagging_branch_and_tag.sh"
fi

exit $EXIT_PASS
