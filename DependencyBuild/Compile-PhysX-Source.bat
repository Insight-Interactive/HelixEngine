@echo off

SET DEVENV_EXE_LOCATION=C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\IDE\devenv
pushd %~dp0

echo Compiling PhysX (version 4.1)...
echo ------------------
SET PHYSX_SDK_ROOT=..\Engine\ThirdParty\PhysX-4.1


echo Configuration Debug -> x64
"%DEVENV_EXE_LOCATION%" /build "debug|x64" /project ALL_BUILD %PHYSX_SDK_ROOT%\physx\compiler\vc16win64\PhysXSDK.sln
echo Done.
echo Configuration Checked -> x64
"%DEVENV_EXE_LOCATION%" /build "checked|x64" /project ALL_BUILD %PHYSX_SDK_ROOT%\physx\compiler\vc16win64\PhysXSDK.sln
echo Done.
echo Configuration Profile -> x64
"%DEVENV_EXE_LOCATION%" /build "profile|x64" /project ALL_BUILD %PHYSX_SDK_ROOT%\physx\compiler\vc16win64\PhysXSDK.sln
echo Done.
echo Configuration Release -> x64
"%DEVENV_EXE_LOCATION%" /build "release|x64" /project ALL_BUILD %PHYSX_SDK_ROOT%\physx\compiler\vc16win64\PhysXSDK.sln
echo Done.

echo PhysX library compilation completed!