
-- File: BuildUtilities.lua
-- Author: Garrett Courtney
-- Description: Contains common methods and variables for project building.



-- Executable output directory for engine builds.
buildFolder = "Build-%{cfg.toolset}/";
-- Intermediate output directory for engine builds.
intermediateFolder = buildFolder .. "Intermediates/";



-- Returns the absolute path to the executable build directoy. 
function heGetBuildFolder()
	return "%{wks.location}/" .. buildFolder .. "/%{cfg.buildcfg}-%{cfg.platform}/";
end

-- Returns the absolute path to the intermediate build directoy. 
function heGetBuildIntFolder()
	return "%{wks.location}/" .. intermediateFolder .. "/%{cfg.buildcfg}-%{cfg.platform}/";
end

-- Returns the root directory of the runtime engine library.
function heGetRuntimeModuleRootDir()
    return "%{wks.location}/Engine/Source/Runtime/";
end

-- Returns the root directory of the shader library.
function heGetShaderRootDir()
    return "%{wks.location}/Engine/Shaders/";
end

-- Returns the root path of the Editor module.
function heGetEditorModule( ModuleName )
    return "%{wks.location}/Engine/Source/Editor/";
end

-- Returns the root path of a third party module.
function heGetThirdPartyRootDir()
    return "%{wks.location}/Engine/ThirdParty/";
end

-- Assemble and return the filepath of a engine runtime module. 
function heRuntimeModule( ModuleName )
    return heGetRuntimeModuleRootDir() .. ModuleName .. "/" .. ModuleName .. ".module.lua";
end

-- Returns the root directory of the game module.
function heGetGameModuleRootDir()
    return "%{wks.location}/Game/Source/";
end

-- Returns the directory holding the source code for the engine.
function heGetEngineSourceDir()
    return "%{wks.location}/Engine/Source/";
end

-- Returns the public directory of a specified engine module.
function heGetEngineRuntimeModulePublicDir( ModuleName )
    return heGetRuntimeModuleRootDir() .. ModuleName .. "/Public/";
end

-- Returns the private directory of a specified engine module.
function heGetEngineRuntimeModulePrivateDir( ModuleName )
    return heGetRuntimeModuleRootDir() .. ModuleName .. "/Private/";
end
