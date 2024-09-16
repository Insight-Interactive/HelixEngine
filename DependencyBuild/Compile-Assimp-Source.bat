@echo off

SET DEVENV_EXE_LOCATION=C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\IDE\devenv
pushd %~dp0

echo Compiling PhysX (version 4.1)...
echo ------------------
SET PHYSX_SDK_ROOT=..\Engine\ThirdParty\assimp-5.4.3


echo Configuration Debug -> x64
"%DEVENV_EXE_LOCATION%" /build "Debug|x64" /project ALL_BUILD %PHYSX_SDK_ROOT%\Assimp.sln
echo Done.
echo Configuration Release -> x64
"%DEVENV_EXE_LOCATION%" /build "Release|x64" /project ALL_BUILD %PHYSX_SDK_ROOT%\Assimp.sln
echo Done.

echo PhysX library compilation completed!