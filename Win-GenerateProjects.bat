@echo off

echo /************************************************/
echo    Generating Visual Studio 2019 Project Files.
echo /************************************************/

call Engine\ThirdParty\premake\premake5.exe vs2019

pause
