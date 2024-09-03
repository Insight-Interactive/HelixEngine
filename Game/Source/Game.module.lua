include ("Helix/HelixModule.lua")


project ("Game-Core")
    location ("./")
    kind ("StaticLib")
    language ("C++")
    cppdialect ("C++17")
    staticruntime ("On")
    targetname ("%{prj.name}")
    systemversion ("latest")
    defaultlanguage ("en-US")
    targetdir( heGetBuildFolder() )
    objdir( heGetBuildIntFolder() )
    
    pchheader("GamePCH.h")
    pchsource("Private/PCH/GamePCH.cpp")

    files
    {
        "Game.module.lua",

        "Public/**.h",
        "Public/**.cpp",
        "Public/**.inl",
        "Private/**.h",
        "Private/**.cpp",
        "Private/**.inl",
    }

    defines
    {
		"_CRT_SECURE_NO_WARNINGS",
        "GAME_MODULE=1",
		"R_WITH_D3D12=1",
    }

    includedirs
    {
        "Public/",
        "Private/PCH/",
		heGetModulePublicDir( "Engine" ),
		heGetModulePublicDir( "Core" ),
		heGetModulePublicDir( "Math" ),
		heGetModulePublicDir( "Physics" ),
		heGetModulePublicDir( "Scripting" ),

		-- Third Party
		heGetThirdPartyModule( "rapidxml-1.13" )	.. "Include/",
		heGetThirdPartyModule( "Rapidjson" )		.. "include/",
		heGetThirdPartyModule( "LuaPlus" ),
        
    }
    
    links
    {
    }
    
