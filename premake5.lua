-- Copyright Insight Interactive. All rights reserved.

-- The main entry point for the project build system. 

-- LUA_PATH = "Engine/ThirdParty/premake/"

-- Add the premake module to enable WinRT project building.
require ("vstudio");
dofile  ("Modules/winrt/_preload.lua");
require ("Modules/winrt");


include ("Helix/BuildUtilities.lua");


workspace ("HelixEngine")

	startproject ("Engine")

	configurations
	{
		-- Debug symbols for all engine modules activated. Optimal configuration for
		-- adding and testing new engine features.
		"DebugEditor",

		-- Debug symbols for engine and game activated.
		"Development",
		
		-- Optimized engine code with debug symbols for game code only.
		"DebugGame",

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
		"XboxOne",
		
		-- Universal Windows environment targeting the Xbox One X console.
		"XboxOneX",
		
		-- Sony platform targeting the Playstation 5 console.
		"PlayStation5",
	}

	-- filter "platforms:XboxOne"
	-- 	defines "HE_XBOX_ONE"
	-- 	system  ("WindowsUniversal")
	-- 	consumewinrtextension ("false")
	-- 	generatewinmd ("false")
	-- 	architecture ("x64")

	-- filter "platforms:XboxOneX"
	-- 	defines "HE_XBOX_ONE_X"
	-- 	system  ("WindowsUniversal")
	-- 	consumewinrtextension ("false")
	-- 	generatewinmd ("false")
	-- 	architecture ("x64")

	filter "platforms:Win64"
		defines "HE_WIN64"
		system ("Windows")
		architecture ("x64")

	filter "platforms:PlayStation5"
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
		
	filter "configurations:DebugEditor"
		defines { "%{macros.debug}=1", "%{macros.withEd}=1", "%{macros.debugEd}=1" }
		runtime ("Debug")
		symbols ("On")
		optimize ("Off")
		
	filter "configurations:Development"
		defines { "%{macros.dev}=1", "%{macros.debug}=1", "%{macros.withEd}=1"}
		runtime ("Release")
		symbols ("On")
		optimize ("Off")
		
	filter "configurations:DebugGame"
		defines { "%{macros.dev}=1", "%{macros.debug}=1", "%{macros.debugGame}" }
		runtime ("Release")
		symbols ("On")
		optimize ("On")
		
	filter "configurations:ShippingGame"
		defines { "%{macros.shipping}=1" }
		runtime ("Release")
		symbols ("Off")
		optimize ("On")

-- /*********************/
--   Load Engine Modules
-- /*********************/
-- Module names need to be complete strings

include ( "Engine/Source/Runtime/Engine/Engine.module.lua" );
include ( "Engine/Source/UnitTests/UnitTests.module.lua" );
include ( "Engine/Shaders/Shaders.module.lua" );
include ( "Game/Source/Game.module.lua" );

group ("Modules")
	include ( "Engine/Source/Runtime/Core/Core.module.lua" );
	include ( "Engine/Source/Runtime/Math/Math.module.lua" );
	include ( "Engine/Source/Runtime/Renderer/Renderer.module.lua" );
group ("")

group ("Editor")
	include ( "Engine/Source/Editor/HelixEd/HelixEd.module.lua" );
group ("")
