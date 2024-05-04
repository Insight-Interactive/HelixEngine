-- Copyright 2024 Insight Interactive. All Rights Reserved.
include ("Helix/HelixModule.lua")


project ("Core")
    location ("./")
    kind ("StaticLib")
    language ("C++")
    cppdialect ("C++17")
    staticruntime ("On")
    targetname ("Helix-%{prj.name}")
    systemversion ("10.0.18362.0:latest")
    defaultlanguage ("en")
    targetdir ( heGetBuildFolder() )
    objdir ( heGetBuildIntFolder() )

    pchheader("CorePCH.h")
    pchsource("Private/PCH/CorePCH.cpp")

    files
    {
        "Core.module.lua",

        "Public/*.h",
        "Public/*.cpp",
        "Public/*.inl",
        "Private/*.h",
        "Private/*.cpp",
        "Private/*.inl",
        "Private/PCH/*.h",
        "Private/PCH/*.cpp",
        "Private/PCH/*.inl",
    }


    includedirs
    {
        "Public/",
        "Private/PCH/",
    }
    
    defines
    {
        "CORE_MODULE=1"
    }

    flags
    {
        "MultiProcessorCompile",
    }

    dofile ("Helix/CommonEngineMacros.lua")

    

    filter ("platforms:Win64 or Win32")
        files
        {
            "Private/Windows/Desktop/**.h",
            "Private/Windows/Desktop/**.cpp",
            "Private/Windows/Desktop/**.inl",
        }

   filter ("platforms:Durango or Scorpio")
       files
       {
           "Private/Windows/Universal/**.h",
           "Private/Windows/Universal/**.cpp",
           "Private/Windows/Universal/**.inl",
           "Private/Windows/Universal/**.appxmanifest",
       }

    filter ("platforms:Durango or Scorpio or Win64 or Win32")
        files
        {
            "Private/Windows/*.h",
            "Private/Windows/*.cpp",
            "Private/Windows/*.inl",
        }