-- Copyright 2021 Insight Interactive. All Rights Reserved.
include ("Helix/HelixModule.lua")
include ("Helix/DllCopy.lua")


workspace("LuaLib")

	startproject("LuaCore")
	
	configurations
	{
		"Debug",
		"Release",
	}
	
	platforms
	{
		"x64",
		"Win32",
	}

	
	filter "platforms:x64"
		system ("Windows")
		architecture ("x64")

	filter "platforms:Win32"
		system ("Windows")
		architecture ("x86_64")
		
	filter "configurations:Debug"
		defines { "_DEBUG" }
		runtime ("Debug")
		symbols ("On")
		optimize ("Off")
		
	filter "configurations:Release"
		runtime ("Release")
		symbols ("Off")
		optimize ("On")



	project ("LuaCore")
		location ("./")
		kind ("StaticLib")
		language ("C++")
		cppdialect ("C++17")
		staticruntime ("On")
		targetname ("%{prj.name}-%{cfg.buildcfg}-%{cfg.platform}")
		systemversion ("latest")
		defaultlanguage ("en")
		targetdir ( "Build/" )
		objdir ( "Build/Intermediate/" )

		files
		{
			"LuaLib.library.lua",

			"src/**.h",
			"src/**.c"
		}

		includedirs
		{
			"src/",
		}

		defines
		{
		}

		flags
		{
			"MultiProcessorCompile",
		}
