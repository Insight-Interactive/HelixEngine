-- Copyright 2021 Insight Interactive. All Rights Reserved.
include ("Helix/HelixModule.lua")
include ("Helix/DllCopy.lua")


project ("Renderer")
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
		"Classes/",
		"Private/PCH/",
		heGetModulePublicDir( "Core" ),
		heGetModulePublicDir( "Math" ),

		heGetEngineRuntimeModulePublicDir ( "Engine" ), -- To get access to *.hasset headers

		-- Third Party 
		heGetThirdPartyModule( "WinPixEventRuntime" )	.. "Include/",
		heGetThirdPartyModule( "OpenFBX" )				.. "src/",
		heGetThirdPartyModule( "assimp-5.3.1" )			.. "include/",
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
	

	filter { "configurations:DebugEditor or Development or Demo" }
		defines
		{
			"R_TRACK_RENDER_EVENTS=1",
			"R_DEBUG_GPU_RESOURCES=1",
			"R_DEBUG=1",
		}
	
	filter { "configurations:DebugEditor or Development or Demo", "platforms:Win64 or Win32 or Durango or Scorpio" }
		postbuildcommands
		{
			"%{dllCopyCommands.PIXWinDesktopx64}" -- Library linked in Engine.module.lua
		}

	-- Third Party 

	-- Assimp

	filter {"configurations:DebugEditor or DebugGame or Demo"}
		postbuildcommands
		{
			"%{dllCopyCommands.Assimpx64_debug}" -- Library linked in Engine.module.lua
		}

	filter {"configurations:Development or ShippingGame"}
		postbuildcommands
		{
			"%{dllCopyCommands.Assimpx64_release}" -- Library linked in Engine.module.lua
		}

	-- End Assimp

	dofile ("Helix/CommonEngineMacros.lua")
