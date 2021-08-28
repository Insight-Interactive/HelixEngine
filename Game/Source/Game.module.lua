include ("Helix/HelixModule.lua")


project ("Game")
    location ("./")
    kind ("SharedLIb")
    language ("C++")
    cppdialect ("C++17")
    staticruntime ("Off")
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
        "GAME_MODULE=1",
    }

    includedirs
    {
        "Public/",
        "Private/PCH/",
        heGetModulePublicDir( "Core" ),
        heGetModulePublicDir( "Engine" ),
    }
    
    links
    {
        "Core",
    }
    
