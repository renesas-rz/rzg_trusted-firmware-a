@echo off

REM args[0] = parameter to select the query criteria by Issue status
REM 1. "Open" Issues [status=Defer,Analyze,Fix,Fix in Next Release] = DEFAULT
REM 2. "Closed" Issues [status=Ignore,Not a problem]
REM 3. "All" Issues [status=Analyze, Fix, Fix in Next Release, Defer,Ignore, Not a problem]

REM To amend the list of Klocwork defined module please update the end of the script file e.g
REM rep_module = "rz_tfa_t2h"
REM dumpIssues(rep_project, rep_severity, rep_module, rep_category, rep_status)

REM The rep_module name should correspond with a module name from the Klocwork server, i.e.
REM http://ree-be0klocwork.ree.adwin.renesas.com:8080/review/insight-review.html#modules_goto:project=tfa_270_t2h,view_id=1

REM These might need to be set here now
REM All sorts of trouble running this script with the old REE domain, using Python installed under REE domain username
REM after the GAD domain name migration...

REM Set Python executable (PYTHON_HOME is an environment variable set in the config.toml file for each runner)
SET PY_PATH=%PYTHON_HOME%
SET PY_EXE=python.exe

REM Set the Python script
SET PY_SCRIPT=scripts\kl_query_issues.py

REM Set the Klocwork project name
SET KLPROJECT=tfa_270_%PLAT%

IF EXIST "%PY_PATH%%PY_EXE%" (
    goto FoundPython
) ELSE (
    goto NoPython
)

:NoPython
echo.
echo "Python : %PY_PATH%%PY_EXE% ERROR Not Found"
echo.
goto end

:FoundPython
echo.
echo "Python : %PY_PATH%%PY_EXE% Found"
echo.

IF EXIST "%PY_SCRIPT%" (
    goto  FoundScript
) ELSE (
    goto NoScript
)

:NoScript
echo.
echo Script : %PY_SCRIPT% ERROR Not Found
echo.
goto end

:FoundScript
echo.
echo Script : %PY_SCRIPT% Found
echo.

echo.
echo Running Klocwork server queries...
echo.

REM Set the status criteria from the option arg[0]
REM NOTE: Klocwork issues are either finally 'closed' by setting to
REM 1) Ignore        = correctly identified code issue but justified for an agreed reason
REM 2) Not a problem = incorrectly identified code issue, basically a false +ve due to a tool behaviour

REM Default to "Open" issues
IF "%1" == "" (
    SET KL_STATUS=Defer,Analyze,Fix
)
IF "%1" == "Open" (
    SET KL_STATUS=Defer,Analyze,Fix
)
IF "%1" == "All" (
    SET KL_STATUS=Defer,Analyze,Ignore,Fix,Ignore,"Not a problem"
)
IF "%1" == "Closed" (
    SET KL_STATUS=Ignore,"Not a problem"
)

echo Klocwork Issue status requested is %KL_STATUS%

REM Assign Klocwork project Module from web portal with respect to PLAT argument
SET PLAT=%2
SET MODULE=rz_tfa_%PLAT%
echo Klocwork Module requested is %MODULE%

REM When any instance of executed script (klockwork python script) returns failure, this variable will be set to 1,
REM indicating the overall outcome of this batch file.
SET /A return_status = 0

:get_basic
REM Generate a tab delimited .txt file for:
REM 1) All issues in the list of modules at the end of the python script
REM    Status (Defer,Analyze,Ignore,Fix)
REM    Severity 1-2 (Critical-Error)          AND
REM    Category non-MISRA
%PY_PATH%%PY_EXE% %PY_SCRIPT% -p%KLPROJECT% -tstatus:%KL_STATUS% -sseverity:1-4 -c-category:MISRA,metrics -mmodule:%MODULE%
IF %ERRORLEVEL% NEQ 0 (
    SET /A return_status = 1
)

:get_common
REM Generate a tab delimited .txt file for:
REM 2) All issues in the list of modules at the end of the python script
REM    Status (Defer,Analyze,Ignore,Fix)
REM    Severity 1-2 (Critical-Error)          AND
REM    Category non-MISRA
%PY_PATH%%PY_EXE% %PY_SCRIPT% -p%KLPROJECT% -tstatus:%KL_STATUS% -sseverity:1-4 -c-category:MISRA,metrics -mmodule:rz_tfa_common
IF %ERRORLEVEL% NEQ 0 (
    SET /A return_status = 1
)

@REM :get_misra
@REM REM Generate a tab delimited .txt file for:
@REM REM 3) All issues in the list of modules at the end of the python script
@REM REM    Status (Defer,Analyze,Ignore,Fix)
@REM REM    Severity 4 (Review)          AND
@REM REM    Category MISRA
@REM REM    NOTE: All MISRA taxonomy issues are Review(4)
@REM %PY_PATH%%PY_EXE% %PY_SCRIPT% -p%KLPROJECT% -tstatus:%KL_STATUS% -sseverity:1-4 -ccategory:MISRA -mmodule:%MODULE%
@REM IF %ERRORLEVEL% NEQ 0 (
@REM     SET /A return_status = 1
@REM )

@REM :get_metrics
@REM REM Generate a tab delimited .txt file for:
@REM REM 4) All issues in the list of modules at the end of the python script
@REM REM    Status (Defer,Analyze,Ignore,Fix)
@REM REM    Severity 2-3 (Error-Warning)          AND
@REM REM    Category metrics
@REM %PY_PATH%%PY_EXE% %PY_SCRIPT% -p%KLPROJECT% -tstatus:%KL_STATUS% -sseverity:1-4 -ccategory:metrics -mmodule:%MODULE%
@REM IF %ERRORLEVEL% NEQ 0 (
@REM     SET /A return_status = 1
@REM )

:end
echo.
echo Klocwork server queries complete
echo.

@echo on
exit %return_status%
