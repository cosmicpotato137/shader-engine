@REM Copy bin\debug to deploy folder
xcopy /s /e /y "bin\debug" "deploy"

@REM Copy res\shaders\compute\* to deploy folder
xcopy /s /e /y "res\shaders\compute" "deploy\res\shaders\compute"

@REM zip deploy folder
powershell -Command "Compress-Archive -Path .\deploy -DestinationPath .\ShaderEngine.zip"