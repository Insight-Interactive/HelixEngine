include ("Helix/HelixModule.lua")

project ("DetourCrowd")
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
        "detour-crowd.lua",
		
        "Include/*.h",
        "Source/*.cpp",
    }

    includedirs
    {
        "Include/",
        heGetThirdPartyModule( "recastnavigation-1.6.0" ) .. "Detour/Include/",
        heGetThirdPartyModule( "recastnavigation-1.6.0" ) .. "Recast/Include/",
    }