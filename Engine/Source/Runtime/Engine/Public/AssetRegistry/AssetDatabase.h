// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "TSingleton.h"

#include "AssetRegistry/ShaderDatabase.h"

#include "ModelManager.h"
#include "TextureManager.h"
#include "Engine/GameProject.h"
#include "Renderer/MaterialManager.h"
#include "Renderer/FontManager.h"
#include "LuaScript.h"
#include "Engine/ScriptManager.h"


static const char* kAssetManifestFilename = "AssetManifest.json";

/*
	Master database containing specialized databases for all project assets. Does not hold data for assets,
	just references on where to find them on disk. The use of global asset mangers (such as GTextureManager)
	should should not be used; use this class instead.

	All assets must abide by the folder structure.
*/
class FAssetDatabase
{
	friend class HEngine;
	friend class HEditorEngine;
	friend class AssetImporter;
	friend class MeshImporter;
	friend class HLevel;
public:

	static HStaticMesh GetStaticMesh( const char* MeshName );
	static HTexture GetTexture( const char* TextureName );
	static HMaterial GetMaterial( const char* MaterialName );
	static LuaScriptRef GetScript( const char* ScriptName );
	static HFont GetFont( const char* FontName );

	static String LookupShaderPath( const FGUID& Guid );


protected:
	FAssetDatabase();
	virtual ~FAssetDatabase();

	/*
		Initialize and load each asset database.
		@param ManifestFile - Manifest file containing the references for each asset database.
	*/
	static void Initialize();
	static void Uninitialize();

	static bool SaveAssetDatabases();

	static void RegisterShader( const FGUID& ShaderGuid, const Char* Filepath );

	static const String LookupShader( const FGUID& Guid );

protected:
	FShaderDatabase		m_ShaderDatabase;

private:
	static FAssetDatabase* SInstance;

};

//
// Inline function implementations
//


/*static*/ FORCEINLINE HStaticMesh FAssetDatabase::GetStaticMesh( const char* MeshName )
{
	FPath Path;
	sprintf_s( Path.m_Path, "%sModels\\%s", FGameProject::GetInstance()->GetContentFolder(), MeshName );
	HStaticMesh Mesh = GStaticGeometryManager.LoadHAssetMeshFromFile( Path.m_Path );
	HE_ASSERT( Mesh->IsValid() );
	return Mesh;
}

/*static*/ FORCEINLINE HTexture FAssetDatabase::GetTexture( const char* TextureName )
{
	FPath Path;
	sprintf_s( Path.m_Path, "%sTextures\\%s", FGameProject::GetInstance()->GetContentFolder(), TextureName );
	HTexture Texture = GTextureManager.LoadTexture( Path.m_Path, DT_Magenta2D, false );
	HE_ASSERT( Texture.IsValid() );

	return Texture;
}

/*static*/ FORCEINLINE HMaterial FAssetDatabase::GetMaterial( const char* MaterialName )
{
	FPath Path;
	sprintf_s( Path.m_Path, "%sMaterials\\%s", FGameProject::GetInstance()->GetContentFolder(), MaterialName );
	HMaterial Material = GMaterialManager.FindOrLoadMaterialFromFile( Path.m_Path );
	HE_ASSERT( Material.IsValid() );

	return Material;
}

/*static*/ FORCEINLINE LuaScriptRef FAssetDatabase::GetScript( const char* ScriptName )
{
	FPath Path;
	sprintf_s( Path.m_Path, "%sScripts\\%s", FGameProject::GetInstance()->GetContentFolder(), ScriptName );
	LuaScriptRef Script = GScriptManager.FindOrLoadScript( Path.m_Path );

	return Script;
}

/*static*/ FORCEINLINE HFont FAssetDatabase::GetFont( const char* FontName )
{
	FPath Path;
	sprintf_s( Path.m_Path, "%sFonts\\%s", FGameProject::GetInstance()->GetContentFolder(), FontName );
	HFont Font = GFontManager.FindOrLoadFont( Path.m_Path );

	return Font;
}

/*static*/ FORCEINLINE String FAssetDatabase::LookupShaderPath( const FGUID& Guid )
{
	return SInstance->LookupShader( Guid );
}

/*static*/ FORCEINLINE bool FAssetDatabase::SaveAssetDatabases()
{
	rapidjson::StringBuffer StrBuffer;
	JsonUtility::WriteContext Writer( StrBuffer );

	Writer.StartObject();
	{
		Writer.Key( HE_STRINGIFY( FAssetDatabase ) );
		Writer.StartArray();

		Writer.StartObject();
		SInstance->m_ShaderDatabase.Serialize( Writer );
		Writer.EndObject();
		Writer.EndArray();
	}
	Writer.EndObject();

	char DestinationFilepath[HE_MAX_PATH];
	FGameProject::GetInstance()->GetProjectDirectoryFullPath( kAssetManifestFilename, DestinationFilepath, sizeof( DestinationFilepath ) );
	bool IsWriteSuccessful = JsonUtility::WriteDocument( DestinationFilepath, StrBuffer );

	return IsWriteSuccessful;
}

/*static*/ FORCEINLINE void FAssetDatabase::RegisterShader( const FGUID& ShaderGuid, const Char* Filepath )
{
	SInstance->m_ShaderDatabase.RegisterAsset( ShaderGuid, Filepath );
}

/*static*/ FORCEINLINE const String FAssetDatabase::LookupShader( const FGUID& Guid )
{
	return SInstance->m_ShaderDatabase.GetValueByKey( Guid );
}
