-- Copyright 2021 Insight Interactive. All Rights Reserved.

include ("Helix/HelixModule.lua")

dllCopyCommands = {}
-- Third Party dlls
-- PIX
dllCopyCommands["PIXWinDesktopx64"]             = "{COPY} " .. heGetThirdPartyModule( "WinPixEventRuntime" ) .. "bin/x64/WinPixEventRuntime.dll %{cfg.targetdir}"
dllCopyCommands["PIXWinUniversalx64"]           = "{COPY} " .. heGetThirdPartyModule( "WinPixEventRuntime" ) .. "bin/x64/WinPixEventRuntime_UAP.dll %{cfg.targetdir}"
-- PhysX 64-bit debug
dllCopyCommands["PhysXx64_debug"]               = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/debug/" .. "PhysX_64.dll %{cfg.targetdir}"
dllCopyCommands["PhysXCommonx64_debug"]         = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/debug/" .. "PhysXCommon_64.dll %{cfg.targetdir}"
dllCopyCommands["PhysXFoundationx64_debug"]     = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/debug/" .. "PhysXFoundation_64.dll %{cfg.targetdir}"
dllCopyCommands["PhysXCookingx64_debug"]        = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/debug/" .. "PhysXCooking_64.dll %{cfg.targetdir}"
dllCopyCommands["PhysXGpux64_debug"]            = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/debug/" .. "PhysXGPU_64.dll %{cfg.targetdir}"
-- PhysX 32-bit debug
dllCopyCommands["PhysXx86_debug"]               = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/debug/" .. "PhysX_32.dll %{cfg.targetdir}"
dllCopyCommands["PhysXCommonx86_debug"]         = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/debug/" .. "PhysXCommon_32.dll %{cfg.targetdir}"
dllCopyCommands["PhysXFoundationx86_debug"]     = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/debug/" .. "PhysXFoundation_32.dll %{cfg.targetdir}"
dllCopyCommands["PhysXCookingx86_debug"]        = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/debug/" .. "PhysXCooking_32.dll %{cfg.targetdir}"
dllCopyCommands["PhysXGPUx86_debug"]            = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/debug/" .. "PhysXGPU_32.dll %{cfg.targetdir}"
-- PhysX 64-bit checked
dllCopyCommands["PhysXx64_checked"]            = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/checked/" .. "PhysX_64.dll %{cfg.targetdir}"
dllCopyCommands["PhysXCommonx64_checked"]      = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/checked/" .. "PhysXCommon_64.dll %{cfg.targetdir}"
dllCopyCommands["PhysXFoundationx64_checked"]  = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/checked/" .. "PhysXFoundation_64.dll %{cfg.targetdir}"
dllCopyCommands["PhysXCookingx64_checked"]     = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/checked/" .. "PhysXCooking_64.dll %{cfg.targetdir}"
dllCopyCommands["PhysXGpux64_checked"]         = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/checked/" .. "PhysXGPU_64.dll %{cfg.targetdir}"
-- PhysX 32-bit checked
dllCopyCommands["PhysXx86_checked"]            = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/checked/" .. "PhysX_32.dll %{cfg.targetdir}"
dllCopyCommands["PhysXCommonx86_checked"]      = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/checked/" .. "PhysXCommon_32.dll %{cfg.targetdir}"
dllCopyCommands["PhysXFoundationx86_checked"]  = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/checked/" .. "PhysXFoundation_32.dll %{cfg.targetdir}"
dllCopyCommands["PhysXCookingx86_checked"]     = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/checked/" .. "PhysXCooking_32.dll %{cfg.targetdir}"
dllCopyCommands["PhysXGPUx86_checked"]         = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/checked/" .. "PhysXGPU_32.dll %{cfg.targetdir}"
-- PhysX 64-bit profile
dllCopyCommands["PhysXx64_profile"]            = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/profile/" .. "PhysX_64.dll %{cfg.targetdir}"
dllCopyCommands["PhysXCommonx64_profile"]      = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/profile/" .. "PhysXCommon_64.dll %{cfg.targetdir}"
dllCopyCommands["PhysXFoundationx64_profile"]  = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/profile/" .. "PhysXFoundation_64.dll %{cfg.targetdir}"
dllCopyCommands["PhysXCookingx64_profile"]     = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/profile/" .. "PhysXCooking_64.dll %{cfg.targetdir}"
dllCopyCommands["PhysXGpux64_profile"]         = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/profile/" .. "PhysXGPU_64.dll %{cfg.targetdir}"
-- PhysX 32-bit profile
dllCopyCommands["PhysXx86_profile"]            = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/profile/" .. "PhysX_32.dll %{cfg.targetdir}"
dllCopyCommands["PhysXCommonx86_profile"]      = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/profile/" .. "PhysXCommon_32.dll %{cfg.targetdir}"
dllCopyCommands["PhysXFoundationx86_profile"]  = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/profile/" .. "PhysXFoundation_32.dll %{cfg.targetdir}"
dllCopyCommands["PhysXCookingx86_profile"]     = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/profile/" .. "PhysXCooking_32.dll %{cfg.targetdir}"
dllCopyCommands["PhysXGPUx86_profile"]         = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/profile/" .. "PhysXGPU_32.dll %{cfg.targetdir}"
-- PhysX 64-bit release
dllCopyCommands["PhysXx64_release"]            = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/release/" .. "PhysX_64.dll %{cfg.targetdir}"
dllCopyCommands["PhysXCommonx64_release"]      = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/release/" .. "PhysXCommon_64.dll %{cfg.targetdir}"
dllCopyCommands["PhysXFoundationx64_release"]  = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/release/" .. "PhysXFoundation_64.dll %{cfg.targetdir}"
dllCopyCommands["PhysXCookingx64_release"]     = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/release/" .. "PhysXCooking_64.dll %{cfg.targetdir}"
dllCopyCommands["PhysXGpux64_release"]         = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/release/" .. "PhysXGPU_64.dll %{cfg.targetdir}"
-- PhysX 32-bit release
dllCopyCommands["PhysXx86_release"]            = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/release/" .. "PhysX_32.dll %{cfg.targetdir}"
dllCopyCommands["PhysXCommonx86_release"]      = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/release/" .. "PhysXCommon_32.dll %{cfg.targetdir}"
dllCopyCommands["PhysXFoundationx86_release"]  = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/release/" .. "PhysXFoundation_32.dll %{cfg.targetdir}"
dllCopyCommands["PhysXCookingx86_release"]     = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/release/" .. "PhysXCooking_32.dll %{cfg.targetdir}"
dllCopyCommands["PhysXGPUx86_release"]         = "{COPY} " .. heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_32.vc142.mt/release/" .. "PhysXGPU_32.dll %{cfg.targetdir}"
