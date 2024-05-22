@REM Accept the first argument as the zip name
@REM this should be a required argument

set zipname=%1

if "%zipname%"=="" (
    echo Error: No argument provided for zipname.
    echo Usage: deploy.bat [zipname]
    exit /b 1
)

@REM Make deploy folder if it doesn't exist
if not exist deploy mkdir deploy

@REM Remove contents of deploy folder
rmdir /s /q deploy

@REM Copy bin\debug to deploy folder
xcopy /s /e /y "bin\Debug\" "deploy\"

@REM Copy res\ to deploy folder
xcopy /s /e /y "res\" "deploy\res\"

@REM Clean the data folder
if exist deploy\data rmdir /s /q deploy\data

@REM Remove old zip file if it exists
if exist %zipname%.zip del %zipname%.zip

@REM zip deploy folder
powershell -Command "Compress-Archive -Path .\deploy\* -DestinationPath .\%zipname%.zip"