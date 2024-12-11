#!/bin/bash
bsp_name="$1"
bsp_release_number="$2"
workspace_path="$3"
list_devices_to_keep="$4"

if [ "$workspace_path" == "" ]; then
	cd ..
	workspace_path="$(pwd)/workspace"
fi
tfa_project_path="${workspace_path}/tf-a"
tfa_release_branch_name="2.7.0/rz_rel_${bsp_name}.${bsp_release_number}"
tfa_rel_feature_branch_name="2.7.0/rz_rel_feat_rm_dev_${bsp_name}.${bsp_release_number}"
tfa_main_branch_name="2.7.0/rz_main"

BASEDIR=$(dirname "$(realpath "$0")")

EXIT_PASS=0
EXIT_FAIL_GIT_ERROR=1
EXIT_FAIL_INVALID_PATH_ERROR=2
EXIT_FAIL_DEVICE_REMOVAL=3
EXIT_FAIL_UNKNOWN_DEVICE_TYPE=4
EXIT_FAIL_FILE_REMOVAL=5

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
delete_lines_from_file() {
    local search_term="$1"
    local file_to_change="$2"

	file_exists "$file_to_change"
    # Use sed to delete lines containing the search term from the file
    sed -i "/$search_term/d" "$file_to_change"

    echo "Lines containing '$search_term' deleted from $file_to_change."
}

check_branch_status(){
	required_status="$1"
	status_check="$(git status | xargs echo)"

	echo "$status_check"
	if [ "$status_check" != "$required_status" ]; then
		echo "git branch is not in the required state"
		exit $EXIT_FAIL_GIT_ERROR
	fi
}

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
cd $tfa_project_path

#Step 1: Creating release and feature branches
git checkout "${tfa_main_branch_name}"
check_branch_status "On branch ${tfa_main_branch_name} Your branch is up to date with origin/${tfa_main_branch_name}. nothing to commit, working tree clean"

create_branch_and_check "${tfa_release_branch_name}"
create_branch_and_check "${tfa_rel_feature_branch_name}"

echo "Step 1 Passed - Created release branch and release-feature branch"
echo ""

#Step 2: Remove code that should be removed for all releases
git rm "$tfa_project_path/.gitlab-ci.yml"
git rm -r "$tfa_project_path/gitlab-ci"

echo "Step 2 Passed - Removed code that should be removed for all releases"
#Step 3: Remove references to devices
all_unreleased_devices_list="n2h,t2h,g3s,v2h,v2n"

IFS=',' read -ra array_keep <<< "$list_devices_to_keep"
IFS=',' read -ra array_remove <<< "$all_unreleased_devices_list"

for device_keep in "${array_keep[@]}"; do
    for ((i=0; i<${#array_remove[@]}; i++)); do
        if [[ "${array_remove[$i]}" == "$device_keep" ]]; then
            unset 'array_remove[i]'
        fi
    done
done

list_devices_for_removal=$(IFS=, ; echo "${array_remove[*]}")
echo "List of devices to remove: ${list_devices_for_removal}"
echo "List of devices to keep: ${list_devices_to_keep}"

if [ "$list_devices_for_removal" = "" ]; then
	echo "Failed to remove any devices."
	exit $EXIT_FAIL_DEVICE_REMOVAL
else
	device_count=0
	for device_for_removal in ${list_devices_for_removal//,/ }
	do
		#the only common file to edit.
		delete_lines_from_file "rz-${device_for_removal}" "$tfa_project_path/docs/plat/index.rst"
		delete_lines_from_file "RZ${device_for_removal^^}" "$tfa_project_path/plat/renesas/rz/common/rz_plat_sip_handler.c"

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
		"v2n")
			echo ""
			;;
		"v2h")
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
#Step 4: Check for removed phrases in file contents.
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

#Step 5: Check for removed phrases in the file and directory names.
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

#Step 6: Perform git commit commands
git add .
git commit -m "Removed devices: $list_devices_for_removal. Removed gitlab ci files."
check_branch_status "On branch ${tfa_rel_feature_branch_name} nothing to commit, working tree clean"
echo "Step 6 Passed - Changes committed."
echo ""

#Step 7: Printing final commands that should be done manually.
echo ""
echo ""
echo "**************************"
echo "All operations passed"
echo "1) Release branch created: ${tfa_release_branch_name}"
echo "2) Release feature branch created: ${tfa_rel_feature_branch_name}"
echo "3) Devices removed in the feature branch: ${list_devices_for_removal}"
echo ""
echo "To push these branches to the repository:"
echo "Commands: cd ${tfa_project_path}"
echo "Commands: git push -u origin ${tfa_release_branch_name}"
echo "Commands: git push -u origin ${tfa_rel_feature_branch_name}"
exit $EXIT_PASS
