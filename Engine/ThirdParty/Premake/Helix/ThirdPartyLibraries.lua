-- Copyright 2021 Insight Interactive. All Rights Reserved.

include ("Helix/HelixModule.lua")

-- NOTE: When adding new keys to the arrays use '_'(underscore) instead of '-'(dash) otherwise lua will 
-- attempt to perform arithmetic operations on the keys when referenced in module files which we don't want.

libraryDirectories = {}
-- PIX
libraryDirectories["PIXx64"]            = heGetThirdPartyModule( "WinPixEventRuntime" ) .. "bin/x64/"
-- PhysX 64-bit
libraryDirectories["PhysXx64_debug"]    = heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/debug/"
libraryDirectories["PhysXx64_checked"]  = heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/checked/"
libraryDirectories["PhysXx64_profile"]  = heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/profile/"
libraryDirectories["PhysXx64_release"]  = heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/release/"
-- PhysX 32-bit
libraryDirectories["PhysXx86_debug"]    = heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/debug/"
libraryDirectories["PhysXx86_checked"]  = heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/checked/"
libraryDirectories["PhysXx86_profile"]  = heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/profile/"
libraryDirectories["PhysXx86_release"]  = heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/release/"
-- Lua 
libraryDirectories["LuaCore"]		    = heGetThirdPartyModule( "lua-5.4.4" ) .. "Build/"

libraries = {}
-- PIX
libraries["PIXWinDesktop"]      = "WinPixEventRuntime.lib"
libraries["PIXWinUniversal"]    = "WinPixEventRuntime_UAP.lib"
-- PhysX 64-bit
libraries["PhysXCommon64"]      = "PhysXCommon_64.lib"
libraries["PhysX64"]            = "PhysX_64.lib"
libraries["PhysXFoundation64"]  = "PhysXFoundation_64.lib"
libraries["PhysXCooking64"]     = "PhysXCooking_64.lib"
libraries["PhysXPVD64"]         = "PhysXPvdSDK_static_64.lib"
libraries["PhysXExtentions64"]  = "PhysXExtensions_static_64.lib"
-- PhysX 32-bit
libraries["PhysXCommon32"]      = "PhysXCommon_32.lib"
libraries["PhysX32"]            = "PhysX_32.lib"
libraries["PhysXFoundation32"]  = "PhysXFoundation_32.lib"
libraries["PhysXCooking32"]     = "PhysXCooking_32.lib"
-- Lua
libraries["Lua64_Debug"]     = "LuaCore-Debug-x64.lib"
libraries["Lua64_Release"]   = "LuaCore-Release-x64.lib"
libraries["Lua32_Debug"]     = "LuaCore-Debug-Win32.lib"
libraries["Lua32_Release"]   = "LuaCore-Release-Win32.lib"
