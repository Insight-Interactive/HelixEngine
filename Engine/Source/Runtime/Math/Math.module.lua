include ("Helix/HelixModule.lua")


project ("Math")
    location ("./")
    kind ("StaticLib")
    language ("C++")
    cppdialect ("C++17")
    staticruntime ("Off")
    targetname ("Helix-%{prj.name}")
    systemversion ("latest")
    defaultlanguage ("en-US")
    targetdir( heGetBuildFolder() )
    objdir( heGetBuildIntFolder() )
    pchheader("MathPCH.h")
    pchsource("Private/PCH/MathPCH.cpp")

    files
    {
        "Math.module.lua",

        "Public/**.h",
        "Public/**.cpp",
        "Public/**.inl",
        "Private/**.h",
        "Private/**.cpp",
        "Private/**.inl",
        "Private/PCH/**.h",
        "Private/PCH/**.cpp",
        "Private/PCH/**.inl",
    }

    includedirs
    {
        "Public/",
        "Private/",
        "Private/PCH/",
        heGetModulePublicDir( "Core" ),
    }

    links
    {
        "Core",
    }

    defines
    {
        "MATH_MODULE=1"
    }

    flags
    {
        "MultiProcessorCompile",
    }
