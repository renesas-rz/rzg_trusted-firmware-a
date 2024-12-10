#! /bin/bash
# Klocwork configuration file for .mpu_klocwork_build_job_template

PLAT="$1"

# Klocwork project name (on portal)
export PROJECT_NAME="tfa_270_$PLAT"

# Path to TF-A MPU build script
export BUILD_BAT="scripts/mpu_build.sh"

# Path to sconf file
export CONFIG_FILE="scripts/exclude_issues_$PLAT.sconf"

# List of files to ignore
export EXCLUDE_LIST=""

echo "Using configuration file for $PROJECT_NAME"
