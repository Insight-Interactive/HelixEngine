-- Copyright 2021 Insight Interactive. All Rights Reserved.

include ("Helix/BuildUtilities.lua")
dofile ("Helix/CommonEngineMacros.lua")

function heGetProgramOutputDir()
    return "%{wks.location}/Engine/Programs/";
end

function heGetProgramOutputIntDir()
    return "%{wks.location}/Engine/Programs/Intermediate/";
end
