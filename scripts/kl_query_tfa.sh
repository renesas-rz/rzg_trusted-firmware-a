#!/bin/bash
# #######################################################################################################################
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
# #######################################################################################################################

KW_ISSUES="$1"
PLAT="$2"

# Set the Python script
PY_SCRIPT="scripts/kl_query_issues.py"

# Set the Klocwork project name
KLPROJECT="LDRE_kwtfa2105_rz${PLAT}_c"

if [ -f "$PY_SCRIPT" ]; then
    # File exists
    echo "$PY_SCRIPT Found"
else
    # File not found
    echo "$PY_SCRIPT Error Not Found"
fi

echo "Running Klocwork server queries..."

# Set the status criteria from the option KW_ISSUES[0]
# NOTE: Klocwork issues are either finally 'closed' by setting to
# 1) Ignore        = correctly identified code issue but justified for an agreed reason
# 2) Not a problem = incorrectly identified code issue, basically a false +ve due to a tool behaviour

# Default to "Open" issues
if [ -z "$KW_ISSUES" ]; then
    KL_STATUS="Defer,Analyze,Fix"
elif [ "$KW_ISSUES" = "Open" ]; then
    KL_STATUS="Defer,Analyze,Fix"
elif [ "$KW_ISSUES" = "All" ]; then
    KL_STATUS='Defer,Analyze,Ignore,Fix,Ignore,"Not a problem"'
elif [ "$KW_ISSUES" = "Closed" ]; then
    KL_STATUS='Ignore,"Not a problem"'
fi

echo "Klocwork Issue status requested is $KL_STATUS"

# Assign Klocwork project Module from web portal with respect to PLAT argument
MODULE="rz_tfa_$PLAT"
echo "Klocwork Module requested is $MODULE"

# When any instance of executed script (klocwork python script) returns failure, this variable will be set to 1,
# indicating the overall outcome of this batch file.
return_status=0

# Generate a tab delimited .txt file for:
# 1) All issues in the list of modules at the end of the python script
#    Status (Defer,Analyze,Ignore,Fix)
#    Severity 1-2 (Critical-Error)          AND
#    Category non-MISRA
python3 "$PY_SCRIPT" -p"$KLPROJECT" -tstatus:"$KL_STATUS" -sseverity:1-4 -c-category:MISRA,metrics -mmodule:"$MODULE" || return_status=1

# Generate a tab delimited .txt file for:
# 2) All issues in the list of modules at the end of the python script
#    Status (Defer,Analyze,Ignore,Fix)
#    Severity 1-2 (Critical-Error)          AND
#    Category non-MISRA
python3 "$PY_SCRIPT" -p"$KLPROJECT" -tstatus:"$KL_STATUS" -sseverity:1-4 -c-category:MISRA,metrics -mmodule:rz_tfa_common || return_status=1

echo "Klocwork server queries complete"

exit $return_status
