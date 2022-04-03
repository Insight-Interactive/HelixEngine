-- Copyright 2021 Insight Interactive. All Rights Reserved.
include ("Helix/HelixModule.lua")
include ("Helix/DllCopy.lua")


project ("Physics")
	location ("./")
	kind ("StaticLib")
	language ("C++")
	cppdialect ("C++17")
	staticruntime ("On")
	targetname ("Helix-%{prj.name}")
	systemversion ("latest")
	defaultlanguage ("en")
	targetdir ( heGetBuildFolder() )
	objdir ( heGetBuildIntFolder() )

	pchheader("PhysicsPCH.h")
	pchsource("Private/PCH/PhysicsPCH.cpp")

	files
	{
		"Physics.module.lua",

		"Public/**.h",
		"Public/**.cpp",
		"Public/**.inl",
		"Private/**.h",
		"Private/**.cpp",
		"Private/**.inl",

		-- Third Party 

	}

	includedirs
	{
		"Public/",
		"Classes/",
		"Private/PCH/",
		heGetModulePublicDir( "Core" ),
		heGetModulePublicDir( "Math" ),

		--Third Party
		heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/include/",
		heGetThirdPartyModule( "PhysX-4.1" ) .. "pxshared/include/",
		heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/source/foundation/include",
		heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/source/physxextensions/src",
		heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/source/common/src",
	}

	defines
	{
		"PHYSICS_MODULE=1",
	}

	flags
	{
		"MultiProcessorCompile",
	}

	filter {"configurations:DebugEditor"}
		defines
		{
			"HE_DEBUG_PHYSICS=1",
			"P_TRACK_MEMORY_ALLOCATIONS=1",
		}


	-- Dll Copy commands for each configuration

	filter {"configurations:DebugEditor", "platforms:Win64"}
		postbuildcommands
		{
			"%{dllCopyCommands.PhysXx64_debug}",
			"%{dllCopyCommands.PhysXCommonx64_debug}",
			"%{dllCopyCommands.PhysXFoundationx64_debug}",
			"%{dllCopyCommands.PhysXCookingx64_debug}",
			"%{dllCopyCommands.PhysXGpux64_debug}",
		}

	filter {"configurations:DebugEditor", "platforms:Win32"}
		postbuildcommands
		{
			"%{dllCopyCommands.PhysXx86_debug}",
			"%{dllCopyCommands.PhysXCommonx86_debug}",
			"%{dllCopyCommands.PhysXFoundationx86_debug}",
			"%{dllCopyCommands.PhysXCookingx86_debug}",
			"%{dllCopyCommands.PhysXGpux86_debug}",
		}

	filter {"configurations:Development", "platforms:Win64"}
		postbuildcommands
		{
			"%{dllCopyCommands.PhysXx64_checked}",
			"%{dllCopyCommands.PhysXCommonx64_checked}",
			"%{dllCopyCommands.PhysXFoundationx64_checked}",
			"%{dllCopyCommands.PhysXCookingx64_checked}",
			"%{dllCopyCommands.PhysXGpux64_checked}",
		}

	filter {"configurations:Development", "platforms:Win32"}
		postbuildcommands
		{
			"%{dllCopyCommands.PhysXx86_checked}",
			"%{dllCopyCommands.PhysXCommonx86_checked}",
			"%{dllCopyCommands.PhysXFoundationx86_checked}",
			"%{dllCopyCommands.PhysXCookingx86_checked}",
			"%{dllCopyCommands.PhysXGpux86_checked}",
		}

	filter {"configurations:ShippingGame", "platforms:Win64"}
		postbuildcommands
		{
			"%{dllCopyCommands.PhysXx64_release}",
			"%{dllCopyCommands.PhysXCommonx64_release}",
			"%{dllCopyCommands.PhysXFoundationx64_release}",
			"%{dllCopyCommands.PhysXCookingx64_release}",
			"%{dllCopyCommands.PhysXGpux64_release}",
		}

	filter {"configurations:ShippingGame", "platforms:Win32"}
		postbuildcommands
		{
			"%{dllCopyCommands.PhysXx86_release}",
			"%{dllCopyCommands.PhysXCommonx86_release}",
			"%{dllCopyCommands.PhysXFoundationx86_release}",
			"%{dllCopyCommands.PhysXCookingx86_release}",
			"%{dllCopyCommands.PhysXGpux86_release}",
		}

	dofile ("Helix/CommonEngineMacros.lua")
