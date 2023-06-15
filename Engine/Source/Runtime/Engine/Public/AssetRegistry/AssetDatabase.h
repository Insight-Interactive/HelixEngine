#pragma once

#include "TSingleton.h"

#include "AssetRegistry/MeshDatabase.h"
#include "AssetRegistry/ActorDatabase.h"
#include "AssetRegistry/ShaderDatabase.h"
#include "AssetRegistry/TextureDatabase.h"
#include "AssetRegistry/MaterialDatabase.h"
#include "AssetRegistry/ScriptDatabase.h"
#include "AssetRegistry/FontDatabase.h"

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
*/
class FAssetDatabase
{
	friend class HEngine;
	friend class HEditorEngine;
	friend class AssetImporter;
	friend class MeshImporter;
	friend class HLevel;
public:

	static HTexture GetTexture( const FGUID& Guid );
	static StaticMeshGeometryRef GetStaticMesh( const FGUID& Guid );
	static HMaterial GetMaterial( const FGUID& Guid );
	static HMaterial CreateOneOffMaterial( const FGUID& ParentGuid );
	static String LookupShaderPath( const FGUID& Guid );
	static LuaScriptRef GetScript( const FGUID& Guid );
	static const String LookupMaterial( const FGUID& Guid );
	static HFont GetFont( const FGUID& FontGuid );

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
	static bool IsAnyDatabaseDirty();

	static void RegisterMesh( const FGUID& MeshGuid, const Char* Filepath );
	static void RegisterTexture( const FGUID& TextureGuid, const Char* Filepath );
	static void RegisterMaterial( const FGUID& MaterialGuid, const Char* Filepath );
	static void RegisterActor( const FGUID& ActorGuid, const Char* Filepath );
	static void RegisterShader( const FGUID& ShaderGuid, const Char* Filepath );
	static void RegisterScript( const FGUID& ScriptGuid, const Char* Filepath );
	static void RegisterFont( const FGUID& FontGuid, const Char* Filepath );

	static const String LookupMesh( const FGUID& Guid );
	static const String LookupTexture( const FGUID& Guid );
	static const String LookupActor( const FGUID& Guid );
	static const String LookupShader( const FGUID& Guid );
	static const String LookupScript( const FGUID& Guid );
	static const String LookupFont( const FGUID& Guid );

	static FMeshDatabase& GetMeshDatabase();
	static FTextureDatabase& GetTextureDatabase();
	static FMaterialDatabase& GetMaterialDatabase();
	static FActorDatabase& GetActorDatabase();
	static FScriptDatabase& GetScriptDatabase();
	static FFontDatabase& GetFontDatabase();

protected:
	FMeshDatabase		m_MeshDatabase;
	FActorDatabase		m_ActorDatabase;
	FShaderDatabase		m_ShaderDatabase;
	FTextureDatabase	m_TextureDatabase;
	FMaterialDatabase	m_MaterialDatabase;
	FScriptDatabase		m_ScriptDatabase;
	FFontDatabase		m_FontDatabase;

private:
	static FAssetDatabase* SInstance;

};

//
// Inline function implementations
//


/*static*/ FORCEINLINE HTexture FAssetDatabase::GetTexture( const FGUID& Guid )
{
	return GTextureManager.LoadTexture( SInstance->LookupTexture( Guid ), DT_Magenta2D, false );
}

/*static*/ FORCEINLINE StaticMeshGeometryRef FAssetDatabase::GetStaticMesh( const FGUID& Guid )
{
	return GStaticGeometryManager.LoadHAssetMeshFromFile( SInstance->LookupMesh( Guid ) );
}

/*static*/ FORCEINLINE HMaterial FAssetDatabase::GetMaterial( const FGUID& Guid )
{
	return GMaterialManager.FindOrLoadMaterialFromFile( SInstance->LookupMaterial( Guid ) );
}

/*static*/ FORCEINLINE HMaterial FAssetDatabase::CreateOneOffMaterial( const FGUID& ParentGuid )
{
	return GMaterialManager.CreateOneOffMaterial( SInstance->LookupMaterial( ParentGuid ) );
}

/*static*/ FORCEINLINE String FAssetDatabase::LookupShaderPath( const FGUID& Guid )
{
	return SInstance->LookupShader( Guid );
}

/*static*/ FORCEINLINE LuaScriptRef FAssetDatabase::GetScript( const FGUID& Guid )
{
	return GScriptManager.FindOrLoadScript( SInstance->LookupScript( Guid ) );
}

/*static*/ FORCEINLINE HFont FAssetDatabase::GetFont( const FGUID& FontGuid )
{
	return GFontManager.FindOrLoadFont( SInstance->LookupFont( FontGuid ) );
}

/*static*/ FORCEINLINE FMeshDatabase& FAssetDatabase::GetMeshDatabase()
{
	return SInstance->m_MeshDatabase;
}

/*static*/ FORCEINLINE FTextureDatabase& FAssetDatabase::GetTextureDatabase()
{
	return SInstance->m_TextureDatabase;
}

/*static*/ FORCEINLINE FMaterialDatabase& FAssetDatabase::GetMaterialDatabase()
{
	return SInstance->m_MaterialDatabase;
}

/*static*/ FORCEINLINE FActorDatabase& FAssetDatabase::GetActorDatabase()
{
	return SInstance->m_ActorDatabase;
}

/*static*/ FORCEINLINE FScriptDatabase& FAssetDatabase::GetScriptDatabase()
{
	return SInstance->m_ScriptDatabase;
}

/*static*/ FORCEINLINE FFontDatabase& FAssetDatabase::GetFontDatabase()
{
	return SInstance->m_FontDatabase;
}

/*static*/ FORCEINLINE bool FAssetDatabase::SaveAssetDatabases()
{
	rapidjson::StringBuffer StrBuffer;
	WriteContext Writer( StrBuffer );

	Writer.StartObject();
	{
		Writer.Key( HE_STRINGIFY( FAssetDatabase ) );
		Writer.StartArray();
		Writer.StartObject();
		SInstance->m_MeshDatabase.Serialize( Writer );
		Writer.EndObject();

		Writer.StartObject();
		SInstance->m_TextureDatabase.Serialize( Writer );
		Writer.EndObject();

		Writer.StartObject();
		SInstance->m_MaterialDatabase.Serialize( Writer );
		Writer.EndObject();

		Writer.StartObject();
		SInstance->m_ActorDatabase.Serialize( Writer );
		Writer.EndObject();

		Writer.StartObject();
		SInstance->m_ShaderDatabase.Serialize( Writer );
		Writer.EndObject();
		Writer.EndArray();
	}
	Writer.EndObject();

	String DestinationFilepath = FGameProject::GetInstance()->GetProjectRoot() + "/AssetManifest.json";
	bool IsWriteSuccessful = JsonUtility::WriteDocument( DestinationFilepath.c_str(), StrBuffer );

	return !IsAnyDatabaseDirty() && IsWriteSuccessful;
}

/*static*/ FORCEINLINE bool FAssetDatabase::IsAnyDatabaseDirty()
{
	return SInstance->m_MeshDatabase.GetIsDirty() || SInstance->m_TextureDatabase.GetIsDirty() || SInstance->m_MaterialDatabase.GetIsDirty() || SInstance->m_ActorDatabase.GetIsDirty();
}

/*static*/ FORCEINLINE void FAssetDatabase::RegisterMesh( const FGUID& MeshGuid, const Char* Filepath )
{
	SInstance->m_MeshDatabase.RegisterAsset( MeshGuid, Filepath );
}

/*static*/ FORCEINLINE void FAssetDatabase::RegisterTexture( const FGUID& TextureGuid, const Char* Filepath )
{
	SInstance->m_TextureDatabase.RegisterAsset( TextureGuid, Filepath );
}

/*static*/ FORCEINLINE void FAssetDatabase::RegisterMaterial( const FGUID& MaterialGuid, const Char* Filepath )
{
	SInstance->m_MaterialDatabase.RegisterAsset( MaterialGuid, Filepath );
}

/*static*/ FORCEINLINE void FAssetDatabase::RegisterActor( const FGUID& ActorGuid, const Char* Filepath )
{
	SInstance->m_ActorDatabase.RegisterAsset( ActorGuid, Filepath );
}

/*static*/ FORCEINLINE void FAssetDatabase::RegisterShader( const FGUID& ShaderGuid, const Char* Filepath )
{
	SInstance->m_ShaderDatabase.RegisterAsset( ShaderGuid, Filepath );
}

/*static*/ FORCEINLINE void FAssetDatabase::RegisterScript( const FGUID& ScriptGuid, const Char* Filepath )
{
	SInstance->m_ScriptDatabase.RegisterAsset( ScriptGuid, Filepath );
}

/*static*/ FORCEINLINE void FAssetDatabase::RegisterFont( const FGUID& FontGuid, const Char* Filepath )
{
	SInstance->m_FontDatabase.RegisterAsset( FontGuid, Filepath );
}

/*static*/ FORCEINLINE const String FAssetDatabase::LookupMesh( const FGUID& Guid )
{
	return FGameProject::GetInstance()->GetProjectRoot() + SInstance->m_MeshDatabase.GetValueByKey( Guid );
}

/*static*/ FORCEINLINE const String FAssetDatabase::LookupTexture( const FGUID& Guid )
{
	return FGameProject::GetInstance()->GetProjectRoot() + SInstance->m_TextureDatabase.GetValueByKey( Guid );
}

/*static*/ FORCEINLINE const String FAssetDatabase::LookupMaterial( const FGUID& Guid )
{
	return FGameProject::GetInstance()->GetProjectRoot() + SInstance->m_MaterialDatabase.GetValueByKey( Guid );
}

/*static*/ FORCEINLINE const String FAssetDatabase::LookupActor( const FGUID& Guid )
{
	return FGameProject::GetInstance()->GetProjectRoot() + SInstance->m_ActorDatabase.GetValueByKey( Guid );
}

/*static*/ FORCEINLINE const String FAssetDatabase::LookupShader( const FGUID& Guid )
{
	return SInstance->m_ShaderDatabase.GetValueByKey( Guid );
}

/*static*/ FORCEINLINE const String FAssetDatabase::LookupScript( const FGUID& Guid )
{
	return FGameProject::GetInstance()->GetProjectRoot() + SInstance->m_ScriptDatabase.GetValueByKey( Guid );
}

/*static*/ FORCEINLINE const String FAssetDatabase::LookupFont( const FGUID& Guid )
{
	return FGameProject::GetInstance()->GetProjectRoot() + SInstance->m_FontDatabase.GetValueByKey( Guid );
}
