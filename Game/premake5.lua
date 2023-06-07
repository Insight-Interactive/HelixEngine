-- Copyright Insight Interactive. All rights reserved.

-- The main entry point for the project build system. 

-- Add the premake module to enable C++/WinRT project building.
require ("vstudio");
dofile  ("Modules/winrt/_preload.lua");
require ("Modules/winrt");


include ("Helix/BuildUtilities.lua");


workspace ("Game")

	startproject ("Game-Core")

	configurations
	{
		-- Debug symbols for all engine modules activated. 
		-- Optimal configuration for adding and testing new engine features.
		"DebugEditor",

		-- Debug symbols for engine and game activated.
		"Development",
		
		-- Optimized engine code with debug symbols for game code only. Builds Standalone exe.
		"DebugGame",

		-- DebugGame configuration, but does not launch in standalone mode.
		"Demo",
		
		-- All engine and game code optimizations applied.
		-- Builds a standalone exe that is shipped to the client.
		"ShippingGame",
	}
	
	platforms
	{
		-- Windows 64-bit desktop environment.
		"Win64",
		
		-- Windows 32-bit desktop environment.
		"Win32",
		
		-- Universal Windows environment targeting the Xbox One console.
		"Durango",
		
		-- Universal Windows environment targeting the Xbox One X console.
		"Scorpio",
		
		-- Sony platform targeting the Playstation 5 console.
		"Prospero",
	}

--	filter "platforms:Durango"
--		defines "HE_XBOX_ONE"
--		system  ("WindowsUniversal")
--		consumewinrtextension ("false")
--		generatewinmd ("false")
--		architecture ("x64")
--
--	filter "platforms:Scorpio"
--		defines "HE_XBOX_ONE_X"
--		system  ("WindowsUniversal")
--		consumewinrtextension ("false")
--		generatewinmd ("false")
--		architecture ("x64")

	filter "platforms:Win64"
		defines "HE_WIN64"
		system ("Windows")
		architecture ("x64")

	filter "platforms:Prospero"
		defines "HE_PS5"
		system ("Windows")
		toolset ("clang")
		architecture ("x64")

	filter "platforms:Win32"
		defines "HE_WIN32"
		system ("Windows")
		architecture ("x86_64")

	macros = {}
	macros["debug"]		= "HE_DEBUG" -- 1 if the engine or game is running in a non-production state.
	macros["debugEd"]   = "HE_DEBUG_EDITOR"
	macros["withEd"]	= "HE_WITH_EDITOR"
	macros["dev"]		= "HE_DEVELOPMENT"
	macros["shipping"]	= "HE_SHIPPING"
	macros["debugGame"] = "HE_DEBUG_GAME"
	macros["demo"]		= "HE_DEMO_GAME"
		
	filter "configurations:DebugEditor"
		defines { "%{macros.debug}=1", "%{macros.withEd}=1", "%{macros.debugEd}=1" }
		runtime ("Debug")
		symbols ("On")
		optimize ("Off")
		
	filter "configurations:Development"
		defines { "%{macros.dev}=1", "%{macros.debug}=1", "%{macros.withEd}=1"}
		runtime ("Debug")
		symbols ("On")
		optimize ("Off")
		
	filter "configurations:DebugGame"
		defines { "%{macros.dev}=1", "%{macros.debug}=1", "%{macros.debugGame}" }
		runtime ("Debug")
		symbols ("On")
		optimize ("Off")

	filter "configurations:Demo"
		defines { "%{macros.dev}=1", "%{macros.debug}=1", "%{macros.demo}=1" }
		runtime {"Debug"}
		symbols ("On")
		optimize ("Off")
		
--		runtime ("Release")
--		symbols ("On")
--		optimize ("On")
		
	filter "configurations:ShippingGame"
		defines { "%{macros.shipping}=1" }
		runtime ("Release")
		symbols ("Off")
		optimize ("On")

-- /*********************/
--   Load Engine Modules
-- /*********************/
-- Module names need to be complete strings

include ("Source/Game.module.lua")