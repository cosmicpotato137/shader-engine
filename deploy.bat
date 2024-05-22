@REM Copy bin\debug to deploy folder
xcopy /s /e /y "bin\debug" "deploy"

@REM Copy res\shaders\compute\* to deploy folder

xcopy /s /e /y "res\shaders\compute" "deploy\res\shaders\compute"
