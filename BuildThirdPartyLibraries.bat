@echo off

echo /************************************************/
echo		Compiling Third-Party Libraries.
echo /************************************************/

pushd %~dp0

SET DEVENV_EXE_LOCATION=C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\IDE\devenv


echo Compiling PhysX...
echo ------------------
SET PHYSX_SDK_ROOT=Engine\ThirdParty\PhysX-4.1

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

echo Configuration Debug -> Win32
"%DEVENV_EXE_LOCATION%" /build "debug|Win32" /project ALL_BUILD %PHYSX_SDK_ROOT%\physx\compiler\vc16win64\PhysXSDK.sln
echo Done.
echo Configuration Checked -> Win32
"%DEVENV_EXE_LOCATION%" /build "checked|Win32" /project ALL_BUILD %PHYSX_SDK_ROOT%\physx\compiler\vc16win64\PhysXSDK.sln
echo Done.
echo Configuration Profile -> Win32
"%DEVENV_EXE_LOCATION%" /build "profile|Win32" /project ALL_BUILD %PHYSX_SDK_ROOT%\physx\compiler\vc16win64\PhysXSDK.sln
echo Done.
echo Configuration Release -> Win32
"%DEVENV_EXE_LOCATION%" /build "release|Win32" /project ALL_BUILD %PHYSX_SDK_ROOT%\physx\compiler\vc16win64\PhysXSDK.sln
echo Done.

echo PhysX library compilation completed!



echo Finished compiling third-party libraries

@pause
