@echo off

echo /************************************************/
echo    Generating Visual Studio 2022 Project Files.
echo /************************************************/

call Engine\ThirdParty\premake\premake5.exe vs2022

pause
