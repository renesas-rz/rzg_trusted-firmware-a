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
# Description: Shell script to create local tag.
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
tag_name="$4"
tfa_version="$5"
branch_name="$6"
release_type="automated"

manual_rel_prompt bsp_name "Enter BSP name (spaces to be replaces with underscores)"
manual_rel_prompt bsp_release_number "Enter BSP release number"
manual_rel_prompt workspace_path "Enter workspace path, spaces to be replaces with underscores"
manual_rel_prompt tag_name "Enter tag name, spaces to be replaces with underscores"
manual_rel_prompt tfa_version "Enter TF-A version"
manual_rel_prompt branch_name "Enter the base branch name for the release"

EXIT_PASS=0
EXIT_FAIL_GIT_ERROR=1
EXIT_FAIL_INVALID_PATH_ERROR=2
EXIT_FAIL_DEVICE_REMOVAL=3
EXIT_FAIL_UNKNOWN_DEVICE_TYPE=4

tfa_project_path="${workspace_path}/tf-a"
tag_version_number="${tfa_version}"
tfa_release_branch_name="${tfa_version}/rz_rel_${bsp_name}.${bsp_release_number}"
tfa_public_repo_location="git@github.com:renesas-rz/rzg_trusted-firmware-a.git"
tfa_branch_name="${branch_name}"

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

#######################################################################################################################
# Step 1: Continued.
#######################################################################################################################
cd $tfa_project_path
check_branch_status "On branch ${tfa_release_branch_name} nothing to commit, working tree clean"
echo ""

#######################################################################################################################
# Step 2: Create tagging branch.
#######################################################################################################################
echo "Creating tagging branch..."
base_branch_commit_id=$(git rev-parse ${tfa_branch_name})
echo $tag_name
echo $tag_version_number
tagging_branch_name="$tag_version_number/rz_tagging_$tag_name/$tag_version_number"
git checkout --orphan "${tagging_branch_name}"
git add .
git commit -m "Release tag created for ${bsp_name} BSP off branch ${tfa_branch_name} at commit: ${base_branch_commit_id}"
check_branch_status "On branch ${tagging_branch_name} nothing to commit, working tree clean"
echo "Tagging branch: ${tagging_branch_name} successfully created"
echo ""

#######################################################################################################################
# Step 3: Create tag.
#######################################################################################################################
echo "Creating tag..."
git tag -a "${tag_name}" -m "Release tag created for ${bsp_name} BSP off branch ${tfa_branch_name} at commit: ${base_branch_commit_id}"
git checkout "${tag_name}"
pwd
check_branch_status "HEAD detached at ${tag_name} nothing to commit, working tree clean"
echo "Tag: ${tag_name} successfully created"
echo ""

echo ""
echo ""
echo "**************************"
echo "Successfully created tag without commit history"
echo "Tag name ${tag_name}"
echo ""

#######################################################################################################################
# Step 4: Finishing off...
#######################################################################################################################
git remote add GitHub_Public "${tfa_public_repo_location}"
echo ""
echo ""
echo "**************************"
echo "Successfully created tag without commit history"
echo "Tag name ${tag_name}"
echo ""

if [[ ${release_type} == "manual" ]]; then
    echo "Go to ${tfa_project_path} and test the tag. After testing, push the tag using the following command:"
    echo "Command: git push origin ${tag_name}"
fi
