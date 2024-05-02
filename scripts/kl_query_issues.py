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

import urllib, json, sys, os.path, getpass, argparse, datetime, psutil

from urllib import error
from urllib import request
from urllib import parse

# Authorisation parameters
host = "klocwork.global.renesas.com"
port = 8443

# Error status variable
error_status = 0

# NOTE:
# This Python script is to be executed within a GitLab CI instance. This means that many functions, which should return the USER,
# instead return the VM machine name. This caused issues since machine name does NOT equate to the username located inside the Klocwork
# ltoken file, which the script fetches and compares the two values. The 'psutil.users()' function from the 'psutil 'package/library
# returns users currently connected on the system as a list of named tuples (including 'name', 'terminal', 'host', 'started', and 'pid').

# Functions returning machine name when executed from GitLab CI (ERROR)
# get_user = getpass.getuser()
# os_user = os.getenv("username")

# Function returning current users connected to system
connected_users = psutil.users()

project = "no_project_name_provided"
user = "no_user_provided"
url = "https://%s:%d/review/api" % (host, port)
values = {"project": project, "user": user, "action": "search"}

def get_token(host, port, user):
    """
    Subroutine to get the authorisation ltoken, given the host and user configurations.
    :param host:
    :param port:
    :return user:
    :return rd[3]:
    """
    ltoken = os.path.normpath(os.path.expanduser("~/.klocwork/ltoken"))
    ltoken_file = open(ltoken, 'r', encoding='utf-8')
    for r in ltoken_file:
        rd = r.strip().split(';')

        user = get_user(connected_users, rd[2]) + "@adwin.renesas.com"
        if rd[0].lower() == host.lower() and rd[1] == str(port) and rd[2].lower() == user:
            ltoken_file.close()
            return rd[2], rd[3] #error with returning user, as the user that Klocwork is expecting has capitalisation but linux is all lower

    ltoken_file.close()
    return user, None


def get_user(connected_users, ltoken_user):
    """
    Subroutine to search through all connected users to the system, checking for
    a match against user parameters from the Klocwork ltoken file.
    :param connected_users:
    :param ltoken_user:
    :return user:
    """
    for user_index, user in enumerate(connected_users):
        user = connected_users[user_index].name
        if user == ltoken_user:
            return user

    return user


def dump_issues(rep_project, rep_severity, rep_module, rep_category, rep_status):
    """
    Main routine to handle the Klocwork's server request and response. Not a lot in the way of exception handling in here so beware!
    :param rep_project:
    :param rep_severity:
    :param rep_module:
    :param rep_category:
    :param rep_status:
    :return: error_status
    """
    # Pick apart the request values that will be used for the Klocwork server query to derive a meaningful and valid
    # subfolder and filename. Use anything to the RHS of the ':' in the arguments passed

    global open_issue_count
    open_issue_count = 0
    error_status = 4

    # Set the name of the Klocwork project from the arg -p option
    values["project"] = rep_project

    # -sseverity:1-4 --> Extract the '1-4'
    sev_colon = rep_severity.find(':')  # 0 based
    sev_len = len(rep_severity)
    fnname_sev = rep_severity[sev_colon + 1:sev_len]

    # -mmodule:rz_tfa_t2h --> Extract the 'rz_tfa_t2h'
    mod_colon = rep_module.find(':')  # 0 based
    mod_len = len(rep_module)
    fnname_mod = rep_module[mod_colon + 1:mod_len]

    # -ccategory:MISRA --> Extract the 'MISRA'
    cat_colon = rep_category.find(':')  # 0 based
    cat_len = len(rep_category)

    # Specifically to EXCLUDE a category the category has to be preceded by a '-'
    # e.g. to INCLUDE MISRA category:MISRA
    # e.g. to EXCLUDE MISRA -category:MISRA	--> Add EXC_ to the folder path
    if rep_category[0] == '-':
        fnname_cat = "EXC_" + rep_category[cat_colon + 1:cat_len]
    else:
        fnname_cat = rep_category[cat_colon + 1:cat_len]

    print("Processing Module : %s" % fnname_mod)

    # Derive the view name from the module as it follows a convention
    rep_view = "View_" + fnname_mod

    # Make up the query value to go to the server
    # values["query"] = rep_severity + " " + rep_module + " " + rep_category
    values["view"] = rep_view
    values["query"] = rep_module + " grouping:off " + rep_severity + " " + rep_category + " " + rep_status

    # NOTE: To get comment attribute back in the response we need to turn off any "grouping" -->grouping:off
    # For more details see Klocwork support issue https://developer.klocwork.com/support/tickets/00032518

    print("view   # " + values["view"])
    print("query  # " + values["query"])

    try:
        data = urllib.parse.urlencode(values)
        data = data.encode('utf-8')
    except urllib.error.HTTPError as e:
        print("Query Error >> : 1")
        debug = e.read()
        print(debug)
        error_status = 2

    try:
        req = urllib.request.Request(url, data)
    except urllib.error.HTTPError as e:
        print("Query Error >> : 2")
        debug = e.read()
        print(debug)
        error_status = 2

    try:
        # print("req  : %s\n" %req)
        response = urllib.request.urlopen(req)
    except urllib.error.HTTPError as e:
        print("Query Error >> : 3")
        debug = e.read()
        print(debug)
        error_status = 2
    else:
        subdir = ("./generated/issue_report_" + fnname_cat)
        if not os.path.isdir(subdir):
            os.makedirs(subdir)

        with open(subdir + "/" + fnname_mod + ".txt", "wt") as outfile:
            buf_title = "Static Analysis"
            # buf_title = buf_title + "\n------------------"
            buf_title = buf_title + "\nCOMPONENT: \t" + fnname_mod
            buf_title = buf_title + "\nPROJECT: \t" + rep_project
            rightnow = datetime.datetime.now()
            buf_title = buf_title + "\nTIMESTAMP: \t" + rightnow.isoformat() + "\n\n"
            outfile.write(buf_title)

            # Write report header
            outfile.write("Issue\tStatus\tMessage\tFile\tFunction\tLine\tCode\tSeverity\tComment\n")
            for record in response:
                # convert record to string?
                record = record.decode('utf-8')

                # IF 	- Just dump out the json response to test keys...
                # buffer = str(record)
                # ELSE	- Parse the record for human-readable format
                buffer = str(json.loads(record, object_hook=from_json))
                # END
                outfile.write(buffer)
                outfile.write("\n")

                # Increment the Open issue count base on anything detected NOT status Ignore or "Not a Problem"
                if buffer.find("Ignore") == -1:
                    if buffer.find("Not a Problem") == -1:
                        open_issue_count = open_issue_count + 1

            # Write report summary statement
            buffer = "\n\n----- OPEN ANALYSIS FINDINGS: %d " % open_issue_count

            if open_issue_count > 0:
                buffer = buffer + "  [FAIL] -----"
                error_status = 1
            else:
                buffer = buffer + "  [PASS] -----"
                error_status = 0

            outfile.write(buffer)
            outfile.write("\n")

            outfile.close()


    print("Completed Module  : %s\n" % fnname_mod)

    return error_status


class Issue(object):
    """
    Defines an 'Issue' class in terms of the attributes that we can query
    """
    def __init__(self, attrs):
        self.id = attrs["id"]
        self.message = attrs["message"]
        self.file = attrs["file"]
        self.method = attrs["method"]
        self.line = attrs["line"]
        self.code = attrs["code"]
        self.severity = attrs["severity"]
        self.severityCode = attrs["severityCode"]
        self.state = attrs["state"]
        self.status = attrs["status"]
        self.taxonomyName = attrs["taxonomyName"]
        self.url = attrs["url"]
        # self.created=time.ctime(attrs["dateOriginated"]/1000)
        if "comment" in attrs:
            self.comment = attrs["comment"]
        else:
            self.comment = None

    # It would be nice to strip to the left of \macro\...

    def __str__(self):
        # Find file path and trim to the left of the file name to make file paths easier to read
        trimf = os.path.basename(self.file)

        return "%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s(%d)\t%s" % (
            self.id, self.status, self.message, trimf, self.method, self.line, self.code, self.severity,
            self.severityCode, self.comment
        )


def from_json(json_object):
    """
    <Insert description here...>
    :param json_object:
    :return: json_object
    """
    if 'id' in json_object:
        return Issue(json_object)
    return json_object


# Parse the arguments to get the query values
parser = argparse.ArgumentParser()
parser.add_argument("-p", '--project', help="Klocwork project name e.g.\n'-pd1h_rgl_bsp_max'", required=True)
parser.add_argument("-s", '--severity', help="Klocwork severity e.g.\n'-sseverity:1-2'", required=True)
parser.add_argument("-m", '--module', help="Klocwork module name e.g.\n'-module:DRV_SYS_DMA'", required=True)
parser.add_argument("-t", '--status', help="Klocwork status e.g.\n'-tstatus:Analyze,Fix,Defer,Ignore'", required=True)
parser.add_argument("-c", '--category',
                    help="Klocwork category name e.g.\n'-ccategory:MISRA' to INClude MISRA\n'-c-category:MISRA' to EXClude MISRA",
                    required=True)
args = vars(parser.parse_args())

# Get the ltoken $ to pass to the server to authenticate the user
(user, loginToken) = get_token(host, port, user)
if loginToken is not None:
    values["ltoken"] = loginToken
    values["user"] = user
else:
    values["user"] = user
    error_status = 3

# Set up the parameters to pass to the server query dump_issues
# rep_severity= "1-4"		        # Critical(1), Error(2), Warning(3), Review(4)
# rep_category= "-category:MISRA"   # Specify the Category as EXCLUDING MISRA (-) before removes it!
rep_severity = args['severity']
rep_category = args['category']
rep_module = args['module']
rep_status = args['status']
rep_project = args['project']

# Call the main routine with the arguments passed
if error_status == 0:
    error_status = dump_issues(rep_project, rep_severity, rep_module, rep_category, rep_status)

if error_status == 4:
    print("---------- ERROR - Error when writing issues to text file in %s - %s ----------\n" % (rep_category, rep_module))
elif error_status == 3:
    print("---------- ERROR - ltoken for user %s NOT found ----------" % user)
elif error_status == 2:
    print("---------- ERROR - Query error when running script in %s - %s ----------\n" % (rep_category, rep_module))
elif error_status == 1:
    print("---------- FAILURE - %d Open issues found in %s - %s ----------\n" % (open_issue_count, rep_category, rep_module))
elif error_status == 0:
    print("---------- SUCCESS - No open issues in %s - %s ----------\n" % (rep_category, rep_module))
else:
    print("---------- ERROR - Unknown error in %s - %s ----------\n" % (rep_category, rep_module))

# END
sys.exit(error_status)
