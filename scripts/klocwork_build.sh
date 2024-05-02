#! /bin/bash
#######################################################################################################################
# Copyright [2025] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
# Description: Shell script to generate static analysis results for given MPU configuration for klocwork.
#######################################################################################################################

# $1 = Project specific klocwork_config.sh file argument
if [ -z "$1" ]; then
	echo "ERROR - No configuration file argument provided"
	exit 1
fi

# Check whether configuration file exists
if [ ! -f $1 ]; then
	echo "ERROR - Configuration file provided cannot be found"
	exit 2
fi

# Assigning MPU arguments
PLAT="$2"
BOARD="$3"
BUILD_TYPE="$4"
TARGET_OS="$5"
CONFIGS="$6"
TEST_TYPE="$7"
ERROR_MSG="$8"

echo "MPU configuration received from GitLab YAML:
PLAT 	   : $PLAT
BOARD 	   : $BOARD
BUILD_TYPE : $BUILD_TYPE
TARGET_OS  : $TARGET_OS
CONFIGS    : $CONFIGS
TEST_TYPE  : $TEST_TYPE
ERROR_MSG  : $ERROR_MSG"

# PROJECT_NAME, BUILD_BAT, and EXCLUDE_LIST are set in the configuration file provided as the $1 argument
source $1 $PLAT

if [ ! -f $BUILD_BAT ]; then
	echo "ERROR - Build File not found in configuration file"
	exit 2
fi

# Set a custom Klocwork build string to come from GitLab
export BUILD_STR="KL-$PROJECT_NAME-$CI_JOB_NAME-$CI_PIPELINE_ID"

if [ ${#BUILD_STR} -gt 64 ]; then
	echo "Build name too long... Shortening build string to fit."
	export BUILD_STR="KL-$CI_JOB_NAME-$CI_PIPELINE_ID"
fi

echo "Running Klocwork analysis...
CI_BUILDS_DIR  : $CI_BUILDS_DIR
CI_JOB_NAME    : $CI_JOB_NAME
CI_PIPELINE_ID : $CI_PIPELINE_ID
CI_PROJECT_DIR : $CI_PROJECT_DIR
BUILD_STR      : $BUILD_STR"

# Set the Klocwork tool paths as installed on the CI build machine
export KWADMIN="/users/svc_sp3_ci_etcetc/Klocwork/kwbuildtools/bin/kwadmin"
export KWINJECT="/users/svc_sp3_ci_etcetc/Klocwork/kwbuildtools/bin/kwinject"
export KWBUILDPROJECT="/users/svc_sp3_ci_etcetc/Klocwork/kwbuildtools/bin/kwbuildproject"
export KWDEPLOY="/users/svc_sp3_ci_etcetc/Klocwork/kwbuildtools/bin/kwdeploy"

if [ ! -f $KWADMIN ]; then
	echo "KWADMIN        : $KWADMIN not found"
	exit 230
fi

echo "KWADMIN        : $KWADMIN"

# Creates a job folder to store the kwInjectOut txt and the kloTables
export JOB_FOLDER="$CI_BUILDS_DIR/jobs/$CI_JOB_NAME/builds/$CI_PIPELINE_ID"
echo "JOB_FOLDER     : $JOB_FOLDER"
mkdir -p $JOB_FOLDER/kwInjectOut
export KLOFOLDER="$JOB_FOLDER/kloTables"
export KWINJECT_OUT="$JOB_FOLDER/kwInjectOut/kwinject_$PLAT.txt"

# Sets the Klocwork server url and name of the project for the required PLAT
export KWURL="--url https://klocwork.global.renesas.com:8443/"
export KWPROJECT="--project $PROJECT_NAME"

echo "Running KWDEPLOY $KWDEPLOY"

$KWDEPLOY sync --url https://klocwork.global.renesas.com:8443/

echo "Running KWINJECT $BUILD_STR"
# The -w option to kwinject _should_ force a complete regeneration of a clean build spec.
# This removes the disconcerting (but expected) warnings
# Warning: <.>\buildspec.out:243: 'compile' line for object file <.>.o is already defined (ignored)
# If there is an EXCLUDE_LIST provided in the config file then it will include it in KWINJECT
if [ -z "$EXCLUDE_LIST" ]; then
	echo "$KWINJECT --output $KWINJECT_OUT sh $BUILD_BAT"
	$KWINJECT --output $KWINJECT_OUT sh $BUILD_BAT $PLAT $BOARD $BUILD_TYPE $TARGET_OS "${CONFIGS}"
else
	echo "$KWINJECT --output $KWINJECT_OUT --ignore-files $EXCLUDE_LIST sh $BUILD_BAT $PLAT $BOARD $BUILD_TYPE $TARGET_OS "${CONFIGS}""
	$KWINJECT --output $KWINJECT_OUT --ignore-files $EXCLUDE_LIST sh $BUILD_BAT $PLAT $BOARD $BUILD_TYPE $TARGET_OS "${CONFIGS}"
fi

# Checking errorlevel
if [ $? -ne 0 ]; then
    echo "Klocwork BAT file failure: $?"
	exit 230
fi

echo "KWINJECT $BUILD_STR result: $?"

echo "Running KWBUILD"

# If there is a .sconf CONFIG_FILE provided in the config batch file then it will include it in KWBUILDPROJECT
if [ -z "$CONFIG_FILE" ]; then
	echo "$KWBUILDPROJECT $KWINJECT_OUT $KWURL$PROJECT_NAME --tables-directory $KLOFOLDER --force"
	$KWBUILDPROJECT $KWINJECT_OUT $KWURL$PROJECT_NAME --tables-directory $KLOFOLDER --force
else
	echo "$KWBUILDPROJECT $KWINJECT_OUT $KWURL$PROJECT_NAME --exclude-issues $CONFIG_FILE --tables-directory $KLOFOLDER --force"
	$KWBUILDPROJECT $KWINJECT_OUT $KWURL$PROJECT_NAME --exclude-issues $CONFIG_FILE --tables-directory $KLOFOLDER --force
fi

exit_code=$?

# Checking errorlevel
if [ $exit_code -ne 0 ]; then
    echo "Klocwork BAT file failure: $exit_code"
	exit 230
fi

echo "KWBUILD $BUILD_STR result: $?"

# Uploading build results to Klocwork portal under the name $BUILD_STR
echo "Running KWADMIN $BUILD_STR"
echo "$KWADMIN $KWURL load $PROJECT_NAME $KLOFOLDER --name $BUILD_STR"
$KWADMIN $KWURL load $PROJECT_NAME $KLOFOLDER --name $BUILD_STR

# Checking errorlevel
if [ $? -ne 0 ]; then
	echo "Klocwork BAT file failure: $?"
	exit 230
fi

echo "KWADMIN $BUILD_STR result: $?"
echo "Klocwork $BUILD_STR finished OK"
exit 0
