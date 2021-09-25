-- Copyright 2021 Insight Interactive. All Rights Reserved.
include ("Helix/HelixModule.lua")
include ("Helix/DllCopy.lua")


project ("HelixEd")
	location ("./")
	kind ("StaticLib")
	language ("C++")
	cppdialect ("C++17")
	staticruntime ("Off")
	targetname ("Helix-%{prj.name}")
	systemversion ("latest")
	defaultlanguage ("en-US")
	targetdir ( heGetBuildFolder() )
	objdir ( heGetBuildIntFolder() )

	pchheader("HelixEdPCH.h")
	pchsource("Private/PCH/HelixEdPCH.cpp")

	files
	{
		"HelixEd.module.lua",

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
		"Private/PCH/",
		heGetModulePublicDir( "Core" ),
		heGetModulePublicDir( "Math" ),
		heGetModulePublicDir( "Renderer" ),

		-- Generaly grabbing the engine runtime should never be done.
		-- However, the editor needs it.
		heGetEngineRuntimeModulePublicDir ( "Engine" ),
		heGetEngineRuntimeModulePrivatecDir ( "Renderer" ),

		-- Third Party 
		heGetThirdPartyModule( "ImGui" ),
		heGetThirdPartyModule( "OpenFBX" ) .. "src/",
		heGetThirdPartyModule( "Rapidjson" ) .. "include/",
	}

	links
	{
--		"Core",
	}

	defines
	{
		"HELIXED_MODULE=1",
	}

	flags
	{
		"MultiProcessorCompile",
	}
	

	filter { "configurations:DebugEditor or Development" }
		defines
		{
		}
	
	filter { "platforms:Win64 or Win32 or XboxOne or XboxOneX" }
		postbuildcommands
		{
		}


	dofile ("Helix/CommonEngineMacros.lua")
