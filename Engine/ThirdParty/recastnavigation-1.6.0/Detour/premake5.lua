include ("Helix/HelixModule.lua")

project ("Detour")
    location ("./")
    kind ("StaticLib")
    language ("C++")
    cppdialect ("C++17")
    staticruntime ("On")
    targetname ("%{prj.name}")
    systemversion ("10.0.18362.0:latest")
    defaultlanguage ("en")
    targetdir ( heGetBuildIntFolder() )
    objdir ( heGetBuildIntFolder() )

	 files
    {
        "detour.lua",
		
        "Include/*.h",
        "Include/*.cpp",
        "Include/*.inl",
        "Source/*.h",
        "Source/*.cpp",
        "Source/*.inl",
    }


    includedirs
    {
        "Include/",
    }