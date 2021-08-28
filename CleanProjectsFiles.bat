@echo off

echo /********************************************/
echo    Removing Visual Studio C++ Project Files
echo /********************************************/
del /q /s *.sln
del /q /s *.vcxproj
del /q /s *.vcxproj.user

pause
