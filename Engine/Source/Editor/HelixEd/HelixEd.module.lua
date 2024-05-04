-- Copyright 2024 Insight Interactive. All Rights Reserved.
include ("Helix/HelixModule.lua")
include ("Helix/DllCopy.lua")


project ("HelixEd")
	location ("./")
	kind ("StaticLib")
	language ("C++")
	cppdialect ("C++17")
	staticruntime ("On")
	targetname ("Helix-%{prj.name}")
	systemversion ("latest")
	defaultlanguage ("en-US")
	targetdir ( heGetBuildFolder() )
	objdir ( heGetBuildIntFolder() )

	pchheader("HelixEdPCH.h")
	pchsource("Private/PCH/HelixEdPCH.cpp")

	filter "configurations:DebugEditor or Development"
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
		heGetModulePublicDir( "Physics" ),
		heGetModulePublicDir( "Scripting" ),

		-- Generaly grabbing the engine runtime should never be done.
		-- However, the editor needs it.
		heGetEngineRuntimeModulePublicDir ( "Engine" ),
		heGetEngineRuntimeModulePrivateDir ( "Renderer" ),

		-- Third Party 
		heGetThirdPartyModule( "ImGui" ),
		heGetThirdPartyModule( "OpenFBX" )		.. "src/",
		heGetThirdPartyModule( "Rapidjson" )	.. "include/",
		heGetThirdPartyModule( "PhysX-4.1" )	.. "physx/include/",
		heGetThirdPartyModule( "PhysX-4.1" )	.. "pxshared/include/",
		heGetThirdPartyModule( "LuaPlus" ),
		heGetThirdPartyModule( "Lua535" ),
	}

	defines
	{
		"HELIXED_MODULE=1",
		"R_WITH_D3D12=1",
	}

	flags
	{
		"MultiProcessorCompile",
	}
	

	filter { "configurations:DebugEditor or Development" }
		defines
		{
		}
	
	filter { "platforms:Win64 or Win32 or Durango or Scorpio" }
		postbuildcommands
		{
		}


	dofile ("Helix/CommonEngineMacros.lua")
