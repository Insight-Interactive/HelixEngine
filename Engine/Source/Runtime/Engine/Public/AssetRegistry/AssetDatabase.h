#pragma once

#include "TSingleton.h"

#include "AssetRegistry/MeshDatabase.h"
#include "AssetRegistry/TextureDatabase.h"
#include "AssetRegistry/MaterialDatabase.h"
#include "AssetRegistry/ActorDatabase.h"

#include "ITextureManager.h"
#include "ModelManager.h"
#include "Renderer/MaterialManager.h"
#include "Engine/GameProject.h"


/*
	Database containing specialized databases for all project assets. Does not hold data for assets,
	mearly just references on where to find them on disk. The use of global asset mangers (such as GTextureManager)
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

	HTextureRef GetTexture( const String& Name );
	StaticMeshGeometryRef GetStaticMesh( const String& Name );
	MaterialRef GetMaterial( const String& Name );

protected:
	FAssetDatabase();
	virtual ~FAssetDatabase();

	void Initialize( const Char* DatabaseFile );
	void UnInitialize();

	bool SaveAssetDatabases();
	bool IsAnyDatabaseDirty() const;

	void RegisterMesh( const Char* MeshName, const Char* Filepath );
	void RegisterTexture( const Char* TextureName, const Char* Filepath );
	void RegisterMaterial( const Char* MaterialName, const Char* Filepath );
	void RegisterActor( const Char* MaterialName, const Char* Filepath );

	const String LookupMesh( const String& Name ) const;
	const String LookupTexture( const String& Name ) const;
	const String LookupMaterial( const String& Name ) const;
	const String LookupActor( const String& Name ) const;

	FMeshDatabase& GetMeshDatabase();
	FTextureDatabase& GetTextureDatabase();
	FMaterialDatabase& GetMaterialDatabase();
	FActorDatabase& GetActorDatabase();

protected:
	FMeshDatabase m_MeshDatabase;
	FTextureDatabase m_TextureDatabase;
	FMaterialDatabase m_MaterialDatabase;
	FActorDatabase m_ActorDatabase;

};

//
// Inline function implementations
//


inline HTextureRef FAssetDatabase::GetTexture( const String& Name )
{
	return GTextureManager->LoadTexture( LookupTexture( Name ), DT_Magenta2D, false );
}

inline StaticMeshGeometryRef FAssetDatabase::GetStaticMesh( const String& Name )
{
	return GStaticGeometryManager.LoadHAssetMeshFromFile( LookupMesh( Name ) );
}

inline MaterialRef FAssetDatabase::GetMaterial( const String& Name )
{
	return GMaterialManager.LoadMaterialFromFile( LookupMaterial( Name ) );
}

inline FMeshDatabase& FAssetDatabase::GetMeshDatabase()
{
	return m_MeshDatabase;
}

inline FTextureDatabase& FAssetDatabase::GetTextureDatabase()
{
	return m_TextureDatabase;
}

inline FMaterialDatabase& FAssetDatabase::GetMaterialDatabase()
{
	return m_MaterialDatabase;
}

inline FActorDatabase& FAssetDatabase::GetActorDatabase()
{
	return m_ActorDatabase;
}

inline bool FAssetDatabase::SaveAssetDatabases()
{
	m_MeshDatabase.SerializeToFile( "Content/DataCache/MeshCache.json" );
	m_TextureDatabase.SerializeToFile( "Content/DataCache/TextureCache.json" );
	m_MaterialDatabase.SerializeToFile( "Content/DataCache/MaterialCache.json" );
	m_ActorDatabase.SerializeToFile( "Content/DataCache/ActorCache.json" );

	return !IsAnyDatabaseDirty();
}

inline bool FAssetDatabase::IsAnyDatabaseDirty() const
{
	return m_MeshDatabase.GetIsDirty() || m_TextureDatabase.GetIsDirty() || m_MaterialDatabase.GetIsDirty() || m_ActorDatabase.GetIsDirty();
}

inline void FAssetDatabase::RegisterMesh( const Char* MeshName, const Char* Filepath )
{
	m_MeshDatabase.RegisterAsset( MeshName, Filepath );
}

inline void FAssetDatabase::RegisterTexture( const Char* TextureName, const Char* Filepath )
{
	m_TextureDatabase.RegisterAsset( TextureName, Filepath );
}

inline void FAssetDatabase::RegisterMaterial( const Char* MaterialName, const Char* Filepath )
{
	m_MaterialDatabase.RegisterAsset( MaterialName, Filepath );
}

inline void FAssetDatabase::RegisterActor( const Char* ActorName, const Char* Filepath )
{
	m_ActorDatabase.RegisterAsset( ActorName, Filepath );
}

inline const String FAssetDatabase::LookupMesh( const String& Name ) const
{
	return FGameProject::GetInstance()->GetProjectRoot() + m_MeshDatabase.GetValueByKey( Name );
}

inline const String FAssetDatabase::LookupTexture( const String& Name ) const
{
	return FGameProject::GetInstance()->GetProjectRoot() + m_TextureDatabase.GetValueByKey( Name );
}

inline const String FAssetDatabase::LookupMaterial( const String& Name ) const
{
	return FGameProject::GetInstance()->GetProjectRoot() + m_MaterialDatabase.GetValueByKey( Name );
}

inline const String FAssetDatabase::LookupActor( const String& Name ) const
{
	return FGameProject::GetInstance()->GetProjectRoot() + m_ActorDatabase.GetValueByKey( Name );
}
