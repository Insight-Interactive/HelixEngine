-- Copyright 2024 Insight Interactive. All Rights Reserved.
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
	targetdir ( heGetBuildIntFolder() )
	objdir ( heGetBuildIntFolder() )
	
--	pchheader("ScriptingPCH.h")
--	pchsource("Private/PCH/ScriptingPCH.cpp")

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
		heGetThirdPartyModule( "LuaPlus" ) .. "LuaPlus/**.h",
		heGetThirdPartyModule( "LuaPlus" ) .. "LuaPlus/**.c",
		heGetThirdPartyModule( "LuaPlus" ) .. "LuaPlus/**.cpp",
	}

	includedirs
	{
		"Public/",
		"Classes/",
		"Private/PCH/",
		heGetModulePublicDir( "Core" ),
		heGetModulePublicDir( "Math" ),

		-- Third Party
		heGetThirdPartyModule( "LuaPlus" ) .. "LuaPlus/",
		heGetThirdPartyModule( "LuaPlus" ) .. "tilde/",
		heGetThirdPartyModule( "LuaPlus" ),
	}

	defines
	{
		"SCRIPTING_MODULE=1"
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
