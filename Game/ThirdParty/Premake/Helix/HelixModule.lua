-- Copyright 2021 Insight Interactive. All Rights Reserved.

include ("Helix/BuildUtilities.lua")
dofile ("Helix/CommonEngineMacros.lua")


function heGetModulePublicDir( ModuleName )
    if ModuleName == "Game"
    then
        return heGetGameModuleRootDir() .. "Public/";
    else
        return heGetRuntimeModuleRootDir() .. ModuleName .. "/Public/";
    end
end

function heGetModuleClassesDir( ModuleName )
    if ModuleName == "Game"
    then
        return heGetGameModuleRootDir() .. "Classes/";
    else
        return heGetRuntimeModuleRootDir() .. ModuleName .. "/Classes/";
    end
end

function heGetThirdPartyModule( ModuleName )
    return heGetThirdPartyRootDir() .. ModuleName .. "/";
end

function heGetEditorModulePublicDir( ModuleName )
    return heGetEditorModule() .. ModuleName .. "/Public/";
end
