-- Copyright 2021 Insight Interactive. All Rights Reserved.
include ("Helix/HelixModule.lua")
include ("Helix/DllCopy.lua")


project ("Scripting")
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
	
	pchheader("ScriptingPCH.h")
	pchsource("Private/PCH/ScriptingPCH.cpp")

	files
	{
		"Scripting.module.lua",

		"Public/**.h",
		"Public/**.cpp",
		"Public/**.inl",
		"Private/**.h",
		"Private/**.cpp",
		"Private/**.inl",
		"Private/**.c",

		-- Third Party 
		--heGetThirdPartyModule( "lua-5.4.4" ) .. "src/*.h",
		--heGetThirdPartyModule( "lua-5.4.4" ) .. "src/*.hpp",
		--heGetThirdPartyModule( "lua-5.4.4" ) .. "src/*.c",
		--heGetThirdPartyModule( "lua-5.4.4" ) .. "src/*.cpp",
	}

	includedirs
	{
		"Public/",
		"Classes/",
		"Private/PCH/",
		heGetModulePublicDir( "Core" ),
		heGetModulePublicDir( "Math" ),

		-- Third Party
		heGetThirdPartyModule( "lua-5.4.4" ) .. "src",
	}

	defines
	{
	}

	flags
	{
		"MultiProcessorCompile",
	}
	

	filter { "configurations:DebugEditor or Development" }
		defines
		{
		}
	
	filter { "configurations:DebugEditor or Development", "platforms:Win64 or Win32 or Durango or Scorpio" }
		postbuildcommands
		{
		}


	dofile ("Helix/CommonEngineMacros.lua")
