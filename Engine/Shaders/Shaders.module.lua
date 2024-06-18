-- Copyright 2021 Insight Interactive. All Rights Reserved.
include ("Helix/HelixModule.lua")

project ("Shaders")
	location ("./")
	kind ("Utility")
	targetname ("%{prj.name}")
	systemversion ("latest")

    --filter{ "configurations:DebugEditor or Development" }
	    targetdir( heGetBuildFolder() .. "/Shaders/Core/" )
	    objdir( heGetBuildIntFolder() .. "/Shaders/Core/" )
	
    --filter{ "configurations:DebugGame or ShippingGame" }
    --    targetdir( heGetBuildFolder() .. "/Data/Shaders/" )
	--    objdir( heGetBuildIntFolder() .. "/Data/Shaders/" )

    files
    {
        "Shaders.module.lua",

        "Public/**.hlsl*",
        "Private/**.hlsl*"
    }                     

    postbuildcommands
    {
    }
    
    filter { "files:**.vs.hlsl" }
        shadertype "Vertex"
        shadermodel "5.1"

    filter { "files:**.ps.hlsl" }
        shadertype "Pixel"
        shadermodel "5.1"

    filter { "files:**.cs.hlsl" }
        shadertype "Compute"
        shadermodel "5.1"
