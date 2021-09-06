-- Copyright 2021 Insight Interactive. All Rights Reserved.
include ("Helix/HelixModule.lua")
include ("Helix/DllCopy.lua")


project ("Renderer")
	location ("./")
--	kind ("SharedItems")
	kind ("StaticLib")
	language ("C++")
	cppdialect ("C++17")
	staticruntime ("Off")
	targetname ("Helix-%{prj.name}")
	systemversion ("latest")
	defaultlanguage ("en-US")
	targetdir ( heGetBuildFolder() )
	objdir ( heGetBuildIntFolder() )

	pchheader("RendererPCH.h")
	pchsource("Private/PCH/RendererPCH.cpp")

	files
	{
		"Renderer.module.lua",

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

		heGetEngineRuntimeModulePublicDir ( "Engine" ),

		-- Third Party 
		heGetThirdPartyModule( "WinPixEventRuntime" ) .. "Include/",
	}

	links
	{
--		"Core",
	}

	defines
	{
		"RENDERER_MODULE=1",
		"R_WITH_D3D12=1",
		"R_WITH_DXR=1",
	}

	flags
	{
		"MultiProcessorCompile",
	}
	

	filter { "configurations:DebugEditor or Development" }
		defines
		{
			"R_TRACK_RENDER_EVENTS=1",
			"R_DEBUG_GPU_RESOURCES=1",
			"R_DEBUG=1",
		}
	
	filter { "configurations:DebugEditor or Development", "platforms:Win64 or Win32 or XboxOne or XboxOneX" }
		postbuildcommands
		{
			"%{dllCopyCommands.PIXWinDesktopx64}" -- Library linked in Engine.module.lua
		}


	dofile ("Helix/CommonEngineMacros.lua")
