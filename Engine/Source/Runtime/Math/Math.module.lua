include ("Helix/HelixModule.lua")


project ("Math")
    location ("./")
    kind ("StaticLib")
    language ("C++")
    cppdialect ("C++17")
    staticruntime ("On")
    targetname ("Helix-%{prj.name}")
    systemversion ("latest")
<<<<<<< Updated upstream
    defaultlanguage ("en")
=======
    --defaultlanguage ("en-US")
>>>>>>> Stashed changes
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
--        "Core",
    }

    defines
    {
        "MATH_MODULE=1"
    }

    flags
    {
        "MultiProcessorCompile",
    }
