-- Copyright 2021 Insight Interactive. All Rights Reserved.
include ("Helix/HelixProgram.lua")
include ("Helix/ThirdPartyLibraries.lua")
include ("Helix/PostBuildCommands.lua")


project ("PackageCooker")
	location ("./")
	kind ("ConsoleApp")
	language ("C++")
	cppdialect ("C++17")
	staticruntime ("On")
	targetname ("%{prj.name}")
	systemversion ("latest")
	
	targetdir( heGetBuildFolder() )
	objdir( heGetBuildIntFolder() )
	pchheader("PackageCookerPCH.h")
	pchsource("Private/PCH/PackageCookerPCH.cpp")


	files
	{
		"PackageCooker.program.lua",

		"Public/**.h",
		"Public/**.cpp",
		"Public/**.inl",
		"Private/**.h",
		"Private/**.cpp",
		"Private/**.inl",
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
		heGetEngineRuntimeModulePublicDir ( "Engine" ),

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
		
		-- Third Party libs
	}

	defines
	{
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
	}

	dofile ("Helix/CommonEngineMacros.lua")



--include ("Helix/DeploymentPrep.lua")
