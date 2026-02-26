#!/bin/bash
# #######################################################################################################################
# Copyright [2026] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
# #######################################################################################################################

PLAT="$1"
BOARD="$2"
BUILD_TYPE="$3"

declare -A default_build=(
	["g2l_smarc_pmic_2"]="rzg2l-release-smarc-pmic-2"
    ["g2lc_smarc_1"]="rzg2lc-release-smarc-1"
	["g2ul_smarc"]="rzg2ul-release-g2ul-smarc"
	["g3e_smarc"]="rzg3e-release-smarc"
    ["g3e_dev_1"]="rzg3e-release-dev-1"
    ["g3s_smarc"]="rzg3s-release-no-configs-smarc"
    ["g3s_dev"]="rzg3s-release-no-configs-dev"
	["n2h"]="rzn2h-release-core4"
	["t2h"]="rzt2h-release-core4"
	["v2h_dev_1"]="rzv2h-release-dev-1"
	["v2h_evk_1"]="rzv2h-release-evk-1"
	["v2h_evk_2"]="rzv2h-release-evk-2"
	["v2h_evk_alpha"]="rzv2h-release-evk-alpha"
	["v2l_smarc_pmic_2"]="rzv2l-release-smarc-pmic-2"
	["v2n_evk_1"]="rzv2n-release-evk-1"
    ["v2n_evk_2"]="rzv2n-release-evk-2"
)

if [[ -n "${default_build[${PLAT}_${BOARD}]}" ]]; then
	default_job_name="${default_build[${PLAT}_${BOARD}]}"

    if [[ "$CI_JOB_NAME" == "$default_job_name" ]]; then
        export ACCESS_TOKEN=$(cat /home/gitlab-runner/artifactory_access_token.txt | tr -d '$\r\n')

        CI=true jf rt upload --flat=true --url="https://artifactory.global.renesas.com/artifactory" --access-token="${ACCESS_TOKEN}" "./build/${PLAT}/${BUILD_TYPE}/*.bin" "public-rz_tfa_testing-generic-local/tf-a/rz_development/latest_tfa/${PLAT}/${BOARD}/"
        CI=true jf rt upload --flat=true --url="https://artifactory.global.renesas.com/artifactory" --access-token="${ACCESS_TOKEN}" "./build/${PLAT}/${BUILD_TYPE}/*.srec" "public-rz_tfa_testing-generic-local/tf-a/rz_development/latest_tfa/${PLAT}/${BOARD}/"
    else
        echo "Not uploading to Artifactory as build type is not default..."
    fi
fi
