-- Copyright 2024 Insight Interactive. All Rights Reserved.
include ("Helix/HelixModule.lua")
include ("Helix/ThirdPartyLibraries.lua")
include ("Helix/PostBuildCommands.lua")


project ("Engine")
	location ("./")
	kind ("WindowedApp")
	language ("C++")
	cppdialect ("C++17")
	staticruntime ("On")
	systemversion ("latest")
	defaultlanguage ("en")
	locale("en-US")
	targetdir( heGetBuildFolder() )
	objdir( heGetBuildIntFolder() )
	pchheader("EnginePCH.h")
	pchsource("Private/PCH/EnginePCH.cpp")
	targetname ("%{prj.name}")

	files
	{
		"Engine.module.lua",

		"Public/**.h",
		"Public/**.cpp",
		"Public/**.inl",
		"Private/GameFramework/**.h",
		"Private/GameFramework/**.cpp",
		"Private/GameFramework/**.inl",
		"Private/Graphics/**.h",
		"Private/Graphics/**.cpp",
		"Private/Graphics/**.inl",
		"Private/Input/**.h",
		"Private/Input/**.cpp",
		"Private/Input/**.inl",
		"Private/Actor/**.h",
		"Private/Actor/**.cpp",
		"Private/Actor/**.inl",
		"Private/App/**.h",
		"Private/App/**.cpp",
		"Private/App/**.inl",
		"Private/Engine/**.h",
		"Private/Engine/**.cpp",
		"Private/Engine/**.inl",
		"Private/Jobs/**.h",
		"Private/Jobs/**.cpp",
		"Private/Jobs/**.inl",
		"Private/World/**.h",
		"Private/World/**.cpp",
		"Private/World/**.inl",
		"Private/AssetRegistry/**.h",
		"Private/AssetRegistry/**.cpp",
		"Private/AssetRegistry/**.inl",
		"Private/UI/**.h",
		"Private/UI/**.cpp",
		"Private/UI/**.inl",
		"Private/AI/**.h",
		"Private/AI/**.cpp",
		"Private/AI/**.inl",
		"Private/PCH/**.h",
		"Private/PCH/**.cpp",
		"Private/PCH/**.inl",
	}

	includedirs
	{
		"Public/",
		"Private/PCH/",
		heGetModulePublicDir( "Core" ),
		heGetModulePublicDir( "Math" ),
		heGetModulePublicDir( "Scripting" ),

		-- Third Party
		heGetThirdPartyModule( "LuaPlus" ),
		heGetThirdPartyModule( "WinPixEventRuntime" )		.. "Include/",
		heGetThirdPartyModule( "assimp-5.4.3" )				.. "include/",
		heGetThirdPartyModule( "Rapidjson" )				.. "include/",
		heGetThirdPartyModule( "recastnavigation-1.6.0" )	.. "Detour/Include/",
		heGetThirdPartyModule( "recastnavigation-1.6.0" )	.. "DebugUtils/Include/",
		heGetThirdPartyModule( "PhysX-4.1" )				.. "physx/include/",
		heGetThirdPartyModule( "PhysX-4.1" )				.. "pxshared/include/",
		heGetThirdPartyModule( "PhysX-4.1" )				.. "physx/source/foundation/include",
		heGetThirdPartyModule( "PhysX-4.1" )				.. "physx/source/common/src",
		heGetThirdPartyModule( "PhysX-4.1" )				.. "physx/source/physxextensions/src",
	}

	links   
	{
		"Core",
		"Math",
		"Shaders",
		"Scripting",

		"Game-Core",

		"Detour",

	}

	libdirs
	{

	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"ENGINE_MODULE=1",
		"HELIX_ENGINE=1",
		"R_WITH_D3D12=1",
		"R_WITH_DXR=1",
	}

	flags
	{
		"MultiProcessorCompile",
	}

	postbuildcommands
	{
-- 		"%{postBuildCommands.gameConfigCopy}",
--		"%{postBuildCommands.clearModuleLibs}",
--		"%{postBuildCommands.clearModuleLibLinkExtras}",
--		"%{postBuildCommands.clearModuleLibs}",
	}

	dofile ("Helix/CommonEngineMacros.lua")


	filter { "configurations:ShippingGame" }
		defines
		{
			"NDEBUG"
		}

	filter ("platforms:Win64")
		files
		{
			"Private/Launch/LaunchWindowsDesktop.cpp",
			-- Win Desktop
			"Private/Windows/Desktop/**.h",
			"Private/Windows/Desktop/**.cpp",
			"Private/Windows/Desktop/**.inl",
			"Private/Windows/Desktop/**.ico",
			"Private/Windows/Desktop/**.rc",
			-- Shared
			"Private/Windows/*.h",
			"Private/Windows/*.cpp",
			"Private/Windows/*.inl",
		}

	filter { "configurations:DebugEditor or Development" }
		debugargs
		{
			"-launchcfg LaunchEditor",
		}
		links
		{
			"PackageCooker",
		}
		targetname ("HelixEditor")
		libdirs
		{

		}

	filter { "configurations:DebugGame or Demo or ShippingGame" }
		debugargs
		{
			"-launchcfg LaunchGame",
		}

	filter { "configurations:DebugEditor or Development", "platforms:Win64" }
		links
		{
			"HelixEd",
		}
		includedirs
		{	
			-- Only include the editor for the ability to launch the editor library in editor builds.
			heGetEditorModulePublicDir ( "HelixEd" ),
		}

	filter { "configurations:DebugEditor or Development or Demo", "platforms:Win64" }
		links
		{
			"%{libraries.PIXWinDesktop}",
		}
		libdirs
		{
			"%{libraryDirectories.PIXx64}",
		}
		defines
		{
			"R_TRACK_RENDER_EVENTS=1",
			"R_DEBUG_GPU_RESOURCES=1",
			"R_DEBUG=1",
		}
		postbuildcommands
		{
			"%{dllCopyCommands.PIXWinDesktopx64}" -- Library linked in Engine.module.lua
		}

	-- Third Party links
	
	-- Assimp

	filter {"configurations:DebugEditor or DebugGame or Demo"}
		libdirs
		{
			"%{libraryDirectories.Assimp_debug}",
		}
		links
		{
			"%{libraries.Assimp_Debug}",
		}
		postbuildcommands
		{
			"%{dllCopyCommands.Assimpx64_debug}" -- Library linked in Engine.module.lua
		}

	filter {"configurations:Development or ShippingGame"}
		libdirs
		{
			"%{libraryDirectories.Assimp_release}",
		}
		links
		{
			"%{libraries.Assimp_Release}",
		}
		postbuildcommands
		{
			"%{dllCopyCommands.Assimpx64_release}" -- Library linked in Engine.module.lua
		}

	-- PhysX

	-- PhysX Win64

	filter { "platforms:Win64" }
		links
		{
			"%{libraries.PhysXCommon64}",
			"%{libraries.PhysX64}",
			"%{libraries.PhysXFoundation64}",
			"%{libraries.PhysXCooking64}",
			"%{libraries.PhysXExtentions64}",
			"%{libraries.PhysXPVD64}",
		}

	filter {"configurations:DebugEditor or Development", "platforms:Win64"}
		libdirs
		{
			"%{libraryDirectories.PhysXx64_debug}",
		}
		postbuildcommands
		{
			"%{dllCopyCommands.PhysXx64_debug}",
			"%{dllCopyCommands.PhysXCommonx64_debug}",
			"%{dllCopyCommands.PhysXFoundationx64_debug}",
			"%{dllCopyCommands.PhysXCookingx64_debug}",
			"%{dllCopyCommands.PhysXGpux64_debug}",
		}
		links
		{
			heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/debug/PhysXCharacterKinematic_static_64.lib"
		}

	filter {"configurations:DebugGame or Demo", "platforms:Win64"}
		libdirs
		{
			"%{libraryDirectories.PhysXx64_debug}",
		}
		postbuildcommands
		{
			"%{dllCopyCommands.PhysXx64_debug}",
			"%{dllCopyCommands.PhysXCommonx64_debug}",
			"%{dllCopyCommands.PhysXFoundationx64_debug}",
			"%{dllCopyCommands.PhysXCookingx64_debug}",
			"%{dllCopyCommands.PhysXGpux64_debug}",
		}
		links
		{
			heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/debug/PhysXCharacterKinematic_static_64.lib"
		}

	filter {"configurations:ShippingGame", "platforms:Win64"}
		libdirs
		{
			"%{libraryDirectories.PhysXx64_release}",
		}
		postbuildcommands
		{
			"%{dllCopyCommands.PhysXx64_release}",
			"%{dllCopyCommands.PhysXCommonx64_release}",
			"%{dllCopyCommands.PhysXFoundationx64_release}",
			"%{dllCopyCommands.PhysXCookingx64_release}",
			"%{dllCopyCommands.PhysXGpux64_release}",
		}
		links
		{
			heGetThirdPartyModule( "PhysX-4.1" ) .. "physx/bin/win.x86_64.vc142.mt/release/PhysXCharacterKinematic_static_64.lib"
		}
	-- End PysX

--include ("Helix/DeploymentPrep.lua")
