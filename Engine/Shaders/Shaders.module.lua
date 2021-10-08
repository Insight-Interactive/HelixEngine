-- Copyright 2021 Insight Interactive. All Rights Reserved.
include ("Helix/HelixModule.lua")

project ("Shaders")
	location ("./")
	kind ("Utility")
	targetname ("%{prj.name}")
	systemversion ("latest")
	defaultlanguage ("en-US")
	targetdir( heGetBuildFolder() .. "/Shaders/" )
	objdir( heGetBuildIntFolder() .. "/Shaders/" )
	
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