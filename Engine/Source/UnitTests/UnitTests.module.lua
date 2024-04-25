-- Copyright 2021 Insight Interactive. All Rights Reserved.
include ("Helix/HelixModule.lua")


project ("UnitTests")
    location ("./")
    kind ("ConsoleApp")
    language ("C++")
    cppdialect ("C++17")
    staticruntime ("On")
    targetname ("%{prj.name}")
    systemversion ("latest")
    defaultlanguage ("en-US")
    targetdir( heGetBuildFolder() )
    objdir( heGetBuildIntFolder() )

    pchheader("PCH.h")
    pchsource("Private/PCH/PCH.cpp")

    files
    {
        "UnitTests.module.lua",

        "Public/**.h",
        "Public/**.cpp",
        "Public/**.inl",
        "Private/**.h",
        "Private/**.cpp",
        "Private/**.inl",
    }

    includedirs
    {
        "Public/",
        "Private/",
        "Private/PCH/",
        heGetModulePublicDir( "Core" ),
        heGetModulePublicDir( "Game" ),
        heGetModulePublicDir( "Engine" ),
        heGetModulePublicDir( "Math" ),
    }

    links
    {
        "Core",
        "Math",
    }

    dofile ("Helix/CommonEngineMacros.lua")
