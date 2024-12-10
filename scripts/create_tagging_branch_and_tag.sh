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
bsp_name="$1"
bsp_release_number="$2"
workspace_path="$3"
tag_name="$4"

EXIT_PASS=0
EXIT_FAIL_GIT_ERROR=1
EXIT_FAIL_INVALID_PATH_ERROR=2
EXIT_FAIL_DEVICE_REMOVAL=3
EXIT_FAIL_UNKNOWN_DEVICE_TYPE=4

if [ "$workspace_path" == "" ]; then
    cd ..
    workspace_path="$(pwd)/workspace"
fi
tfa_project_path="${workspace_path}/tf-a"
tag_version_number="2.7.0"
tfa_release_branch_name="2.7.0/rz_rel_${bsp_name}.${bsp_release_number}"
tfa_public_repo_location="git@github.com:renesas-rz/rzg_trusted-firmware-a.git"

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

# If workspace path is != "", then it is a manual release procedure, setup accordingly.
if [ "$3" != "" ]; then
    #Setup Workspace
    file_exists $workspace_path
    rm -r -f $workspace_path/*
    cd $workspace_path
    git clone http://svc_sp3_ci_etcetc:DC_p5HnJoAsvmocwdYer@global-infra-jp-main.dgn.renesas.com/products/common/bootloader/soc/tf-a.git
    cd $tfa_project_path
    #######################################################################################################################
    # Step 1: Checkout release branch.
    #######################################################################################################################
    echo "Getting release branch..."
    git checkout "${tfa_release_branch_name}"
else
    cd $tfa_project_path
fi

#######################################################################################################################
# Step 1: Continued.
#######################################################################################################################
check_branch_status "On branch ${tfa_release_branch_name} nothing to commit, working tree clean"
echo ""

#######################################################################################################################
# Step 2: Create tagging branch.
#######################################################################################################################
echo "Creating tagging branch..."
echo $tag_name
echo $tag_version_number
tagging_branch_name="$tag_version_number/rz_tagging_$tag_name/$tag_version_number"
git checkout --orphan "${tagging_branch_name}"
git add .
git commit -m "creating tag ${tag_name}"
check_branch_status "On branch ${tagging_branch_name} nothing to commit, working tree clean"
echo "Tagging branch: ${tagging_branch_name} successfully created"
echo ""

#######################################################################################################################
# Step 3: Create tag.
#######################################################################################################################
echo "Creating tag..."
git tag -a "${tag_name}" -m "Release tag created for ${bsp_name} BSP"
git checkout "${tag_name}"
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
# Only required for manual process
#######################################################################################################################
if [ "$3" != "" ]; then
    git remote add GitHub_Public "${tfa_public_repo_location}"
    echo ""
    echo ""
    echo "**************************"
    echo "Successfully created tag without commit history"
    echo "Tag name ${tag_name}"
    echo ""
    echo "After testing, push to the relevant repo using these commands:"
    echo "Command: cd ${tfa_project_path}"
    echo "Command: git push origin ${tag_name}"
    echo "Command: git push GitHub_Public ${tag_name}"
fi
