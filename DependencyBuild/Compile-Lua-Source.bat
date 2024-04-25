@echo off

SET DEVENV_EXE_LOCATION=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv


echo Compiling Lua Source (version 5.4.4)...
echo ------------------
SET LUA_SDK_ROOT=..\Engine\ThirdParty\lua-5.4.4


echo Configuration Debug -> x64
"%DEVENV_EXE_LOCATION%" /build "Debug|x64" /project LuaCore %LUA_SDK_ROOT%\LuaLib.sln
echo Done.
echo Configuration Release -> x64
"%DEVENV_EXE_LOCATION%" /build "Release|x64" /project LuaCore %LUA_SDK_ROOT%\LuaLib.sln
echo Done.

echo Configuration Debug -> Win32
"%DEVENV_EXE_LOCATION%" /build "Debug|Win32" /project LuaCore %LUA_SDK_ROOT%\LuaLib.sln
echo Done.
echo Configuration Release -> Win32
"%DEVENV_EXE_LOCATION%" /build "Release|Win32" /project LuaCore %LUA_SDK_ROOT%\LuaLib.sln
echo Done.

echo Lua library compilation completed!
