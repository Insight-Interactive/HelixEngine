-- Copyright 2021 Insight Interactive. All Rights Reserved.
include ("Helix/HelixModule.lua")
include ("Helix/ThirdPartyLibraries.lua")
include ("Helix/PostBuildCommands.lua")


project ("Engine")
	location ("./")
	kind ("WindowedApp")
	language ("C++")
	cppdialect ("C++17")
	staticruntime ("Off")
	targetname ("%{prj.name}")
	systemversion ("latest")
	defaultlanguage ("en")
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

		-- Third Party
		heGetThirdPartyModule( "rapidxml-1.13" ) .. "Include/",
		heGetThirdPartyModule( "Rapidjson" ) .. "include/",
	}

	links   
	{
		"Core",
		--"Game",
		"Math",
		"Renderer",
		"Shaders",
		--"PackageMaker",
		
		-- Third Party libs
		"D3D12.lib",
		"DXGI.lib",
	}

	defines
	{
		"ENGINE_MODULE=1",
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
		"%{postBuildCommands.clearModuleLibs}",
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

	filter ("configurations:DebugEditor or Development")
		debugargs
		{
			"-launchcfg LaunchEditor",
		}

	filter ("configurations:DebugGame or ShippingGame")
		debugargs
		{
			"-launchcfg LaunchGame",
		}

	filter ("configurations:DebugEditor or Development", "platforms:Win64 or Win32")
		links
		{
			"%{libraries.PIXWinDesktop}",
			"HelixEd"
		}
		libdirs
		{
			"%{libraryDirectories.PIXx64}"
		}
		includedirs
		{	
			-- Only include the editor for the ability to launch the editor library in editor builds.
			heGetEditorModulePublicDir ( "HelixEd" ),
		}


--include ("Helix/DeploymentPrep.lua")
