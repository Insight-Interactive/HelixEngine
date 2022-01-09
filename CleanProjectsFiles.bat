@echo off

echo /********************************************/
echo    Removing Visual Studio C++ Project Files
echo /********************************************/

cd Engine/Source
del /q /s *.sln
del /q /s *.vcxproj
del /q /s *.vcxproj.user

cd ..
cd Shaders
del /q /s *.sln
del /q /s *.vcxproj
del /q /s *.vcxproj.user


pause
