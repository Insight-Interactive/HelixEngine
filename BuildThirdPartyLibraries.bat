@echo off

echo /************************************************/
echo		Compiling Third-Party Libraries.
echo /************************************************/

pushd %~dp0


call DependencyBuild\Compile-PhysX-Source.bat
call DependencyBuild\Compile-Lua-Source.bat


echo Finished compiling third-party libraries

@pause
