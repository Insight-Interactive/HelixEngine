#pragma once

#include "TSingleton.h"

#include "AssetRegistry/MeshDatabase.h"
#include "AssetRegistry/TextureDatabase.h"
#include "AssetRegistry/MaterialDatabase.h"
#include "AssetRegistry/ActorDatabase.h"
#include "AssetRegistry/ShaderDatabase.h"

#include "TextureManager.h"
#include "ModelManager.h"
#include "Renderer/MaterialManager.h"
#include "Engine/GameProject.h"


/*
	Master database containing specialized databases for all project assets. Does not hold data for assets,
	just references on where to find them on disk. The use of global asset mangers (such as GTextureManager)
	should should not be used; use this class instead.
*/
class FAssetDatabase : public TSingleton<FAssetDatabase>
{
	friend class HEngine;
	friend class HEditorEngine;
	friend class AssetImporter;
	friend class MeshImporter;
	friend class HLevel;
public:

	HTextureRef GetTexture( const FGUID& Guid );
	StaticMeshGeometryRef GetStaticMesh( const FGUID& Guid );
	MaterialRef GetMaterial( const FGUID& Guid );
	String LookupShaderPath( const FGUID& Guid );

protected:
	FAssetDatabase();
	virtual ~FAssetDatabase();

	/*
		Initialize and load each asset database.
		@param ManifestFile - Manifest file containing the references for each asset database.
	*/
	void Initialize( const Char* ManifestFile );
	void Uninitialize();

	bool SaveAssetDatabases();
	bool IsAnyDatabaseDirty() const;

	void RegisterMesh( const FGUID& MeshGuid, const Char* Filepath );
	void RegisterTexture( const FGUID& TextureGuid, const Char* Filepath );
	void RegisterMaterial( const FGUID& MaterialGuid, const Char* Filepath );
	void RegisterActor( const FGUID& ActorGuid, const Char* Filepath );
	void RegisterShader( const FGUID& ShaderGuid, const Char* Filepath );

	const String LookupMesh( const FGUID& Guid ) const;
	const String LookupTexture( const FGUID& Guid ) const;
	const String LookupMaterial( const FGUID& Guid ) const;
	const String LookupActor( const FGUID& Guid ) const;
	const String LookupShader( const FGUID& Guid ) const;

	FMeshDatabase& GetMeshDatabase();
	FTextureDatabase& GetTextureDatabase();
	FMaterialDatabase& GetMaterialDatabase();
	FActorDatabase& GetActorDatabase();

protected:
	FMeshDatabase		m_MeshDatabase;
	FTextureDatabase	m_TextureDatabase;
	FMaterialDatabase	m_MaterialDatabase;
	FActorDatabase		m_ActorDatabase;
	FShaderDatabase		m_ShaderDatabase;

};

//
// Inline function implementations
//


FORCEINLINE HTextureRef FAssetDatabase::GetTexture( const FGUID& Guid )
{
	return GTextureManager.LoadTexture( LookupTexture( Guid ), DT_Magenta2D, false );
}

FORCEINLINE StaticMeshGeometryRef FAssetDatabase::GetStaticMesh( const FGUID& Guid )
{
	return GStaticGeometryManager.LoadHAssetMeshFromFile( LookupMesh( Guid ) );
}

FORCEINLINE MaterialRef FAssetDatabase::GetMaterial( const FGUID& Guid )
{
	return GMaterialManager.FindOrLoadMaterialFromFile( LookupMaterial( Guid ) );
}

FORCEINLINE String FAssetDatabase::LookupShaderPath( const FGUID& Guid )
{
	return LookupShader( Guid );
}

FORCEINLINE FMeshDatabase& FAssetDatabase::GetMeshDatabase()
{
	return m_MeshDatabase;
}

FORCEINLINE FTextureDatabase& FAssetDatabase::GetTextureDatabase()
{
	return m_TextureDatabase;
}

FORCEINLINE FMaterialDatabase& FAssetDatabase::GetMaterialDatabase()
{
	return m_MaterialDatabase;
}

FORCEINLINE FActorDatabase& FAssetDatabase::GetActorDatabase()
{
	return m_ActorDatabase;
}

FORCEINLINE bool FAssetDatabase::SaveAssetDatabases()
{
	rapidjson::StringBuffer StrBuffer;
	WriteContext Writer( StrBuffer );
	
	Writer.StartObject();
	{
		Writer.Key( HE_STRINGIFY( FAssetDatabase ) );
		Writer.StartArray();
			Writer.StartObject();
				m_MeshDatabase.Serialize(Writer);
			Writer.EndObject();

			Writer.StartObject();
				m_TextureDatabase.Serialize(Writer);
			Writer.EndObject();
			
			Writer.StartObject();
				m_MaterialDatabase.Serialize(Writer);
			Writer.EndObject();
			
			Writer.StartObject();
				m_ActorDatabase.Serialize(Writer);
			Writer.EndObject();
			
			Writer.StartObject();
				m_ShaderDatabase.Serialize(Writer);
			Writer.EndObject();
		Writer.EndArray();
	}
	Writer.EndObject();

	String DestinationFilepath = FGameProject::GetInstance()->GetProjectRoot() + "/AssetManifest.json";
	bool IsWriteSuccessful = JsonUtility::WriteDocument( DestinationFilepath.c_str(), StrBuffer );

	return !IsAnyDatabaseDirty() && IsWriteSuccessful;
}

FORCEINLINE bool FAssetDatabase::IsAnyDatabaseDirty() const
{
	return m_MeshDatabase.GetIsDirty() || m_TextureDatabase.GetIsDirty() || m_MaterialDatabase.GetIsDirty() || m_ActorDatabase.GetIsDirty();
}

FORCEINLINE void FAssetDatabase::RegisterMesh( const FGUID& MeshGuid, const Char* Filepath )
{
	m_MeshDatabase.RegisterAsset( MeshGuid, Filepath );
}

FORCEINLINE void FAssetDatabase::RegisterTexture( const FGUID& TextureGuid, const Char* Filepath )
{
	m_TextureDatabase.RegisterAsset( TextureGuid, Filepath );
}

FORCEINLINE void FAssetDatabase::RegisterMaterial( const FGUID& MaterialGuid, const Char* Filepath )
{
	m_MaterialDatabase.RegisterAsset( MaterialGuid, Filepath );
}

FORCEINLINE void FAssetDatabase::RegisterActor( const FGUID& ActorGuid, const Char* Filepath )
{
	m_ActorDatabase.RegisterAsset( ActorGuid, Filepath );
}

FORCEINLINE void FAssetDatabase::RegisterShader( const FGUID& ShaderGuid, const Char* Filepath )
{
	m_ShaderDatabase.RegisterAsset( ShaderGuid, Filepath );
}

FORCEINLINE const String FAssetDatabase::LookupMesh( const FGUID& Guid ) const
{
	return FGameProject::GetInstance()->GetProjectRoot() + m_MeshDatabase.GetValueByKey( Guid );
}

FORCEINLINE const String FAssetDatabase::LookupTexture( const FGUID& Guid ) const
{
	return FGameProject::GetInstance()->GetProjectRoot() + m_TextureDatabase.GetValueByKey( Guid );
}

FORCEINLINE const String FAssetDatabase::LookupMaterial( const FGUID& Guid ) const
{
	return FGameProject::GetInstance()->GetProjectRoot() + m_MaterialDatabase.GetValueByKey( Guid );
}

FORCEINLINE const String FAssetDatabase::LookupActor( const FGUID& Guid ) const
{
	return FGameProject::GetInstance()->GetProjectRoot() + m_ActorDatabase.GetValueByKey( Guid );
}

FORCEINLINE const String FAssetDatabase::LookupShader( const FGUID& Guid ) const
{
	return m_ShaderDatabase.GetValueByKey( Guid );
}
