-- Copyright 2021 Insight Interactive. All Rights Reserved.
include ("Helix/HelixModule.lua")
include ("Helix/ThirdPartyLibraries.lua")
include ("Helix/PostBuildCommands.lua")


project ("Engine")
	location ("./")
	kind ("WindowedApp")
	language ("C++")
	cppdialect ("C++17")
	staticruntime ("On")
	targetname ("%{prj.name}")
	systemversion ("latest")
	defaultlanguage ("en")
	locale("en-US")
	targetdir( heGetBuildFolder() )
	objdir( heGetBuildIntFolder() )
	pchheader("EnginePCH.h")
	pchsource("Private/PCH/EnginePCH.cpp")


	files
	{
		"Engine.module.lua",

		"Public/**.h",
		"Public/**.cpp",
		"Public/**.inl",
		"Private/GameFramework/**.h",
		"Private/GameFramework/**.cpp",
		"Private/GameFramework/**.inl",
		"Private/Renderer/**.h",
		"Private/Renderer/**.cpp",
		"Private/Renderer/**.inl",
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
		heGetModulePublicDir( "Renderer" ),
		heGetModulePublicDir( "Physics" ),
		heGetModulePublicDir( "Scripting" ),

		-- Third Party
		heGetThirdPartyModule( "rapidxml-1.13" )	.. "Include/",
		heGetThirdPartyModule( "Rapidjson" )		.. "include/",
		heGetThirdPartyModule( "LuaPlus" ),
	}

	links   
	{
		"Core",
		"Math",
		"Renderer",
		"Physics",
		"Shaders",
		"Scripting",

		"Game-Core",
	}

	libdirs
	{

	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"ENGINE_MODULE=1",
		"R_WITH_D3D12=1",
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


	filter ("platforms:Win64 or Win32")
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

	filter ("platforms:Durango or Scorpio")
		files
		{
			"Private/Launch/LaunchWindowsUniversal.cpp",
			-- Win Universal
			"Private/Windows/Universal/**.h",
			"Private/Windows/Universal/**.cpp",
			"Private/Windows/Universal/**.inl",
			"Private/Windows/Universal/**.appxmanifest",
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

	filter { "configurations:DebugEditor or Development", "platforms:Win64 or Win32" }
		links
		{
			"HelixEd",
		}
		includedirs
		{	
			-- Only include the editor for the ability to launch the editor library in editor builds.
			heGetEditorModulePublicDir ( "HelixEd" ),
		}

	filter { "configurations:DebugEditor or Development or Demo", "platforms:Win64 or Win32" }
		links
		{
			"%{libraries.PIXWinDesktop}",
		}
		libdirs
		{
			"%{libraryDirectories.PIXx64}",
		}

	-- Third Party links
	
	-- PhysX

	-- PhysX Win32
	filter { "platforms:Win32" }
		links
		{
			"%{libraries.PhysXCommon32}",
			"%{libraries.PhysX32}",
			"%{libraries.PhysXFoundation32}",
			"%{libraries.PhysXCooking32}",
		}

	filter {"configurations:DebugEditor", "platforms:Win32"}
		libdirs
		{
			"%{libraryDirectories.PhysXx86_debug}",
		}

	filter {"configurations:DebugGame or Demo", "platforms:Win32"}
		libdirs
		{
			"%{libraryDirectories.PhysXx86_debug}",
		}
	
	filter {"configurations:ShippingGame", "platforms:Win32"}
		libdirs
		{
			"%{libraryDirectories.PhysXx86_release}",
		}

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

	filter {"configurations:DebugGame or Demo", "platforms:Win64"}
		libdirs
		{
			"%{libraryDirectories.PhysXx64_debug}",
		}				
	
	filter {"configurations:ShippingGame", "platforms:Win64"}
		libdirs
		{
			"%{libraryDirectories.PhysXx64_release}",
		}
	-- End PysX

--include ("Helix/DeploymentPrep.lua")
