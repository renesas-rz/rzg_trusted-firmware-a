#!/bin/bash
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

#Checks to see if the path provided exists.
file_exists() {
    local filepath="$1"

    if [ -e "$filepath" ]; then
        echo "File path '$filepath' exists."
    else
        echo "File path '$filepath' does not exist."
        exit $EXIT_FAIL_INVALID_PATH_ERROR
    fi
}

# Function to delete lines from a file based on a search term
check_branch_status(){
	required_status="$1"
	status_check="$(git status | xargs echo)"

	echo "$status_check"
	if [ "$status_check" != "$required_status" ]; then
		echo "git branch is not in the required state"
		exit $EXIT_FAIL_GIT_ERROR
	fi
}

if [ "$3" != "" ]; then
    #Setup Workspace
    file_exists $workspace_path
    rm -r -f $workspace_path/*
    cd $workspace_path
    git clone http://svc_sp3_ci_etcetc:DC_p5HnJoAsvmocwdYer@global-infra-jp-main.dgn.renesas.com/products/common/bootloader/soc/tf-a.git
    cd $tfa_project_path
    #Step 1: checkout release branch
    echo "Getting release branch..."
    git checkout "${tfa_release_branch_name}"
else
    cd $tfa_project_path
fi
#Step 1: Continued
check_branch_status "On branch ${tfa_release_branch_name} nothing to commit, working tree clean"
echo ""

#Step 2: Create tagging branch
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

#Step 3: Create tag
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

if [ "$3" != "" ]; then
    #Step 4: Finishing off...
    #git remote add GitHub_Public "${tfa_public_repo_location}"

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
