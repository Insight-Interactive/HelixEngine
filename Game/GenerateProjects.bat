@echo off

echo /************************************************/
echo    Generating Visual Studio 2019 Project Files.
echo /************************************************/

call ThirdParty\Premake\premake5.exe vs2019

pause
