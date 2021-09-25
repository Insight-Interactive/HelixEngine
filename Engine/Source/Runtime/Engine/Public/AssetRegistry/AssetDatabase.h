#pragma once

#include "TSingleton.h"

#include "AssetRegistry/MeshDatabase.h"
#include "AssetRegistry/TextureDatabase.h"
#include "AssetRegistry/MaterialDatabase.h"
#include "AssetRegistry/ActorDatabase.h"

#include "ITextureManager.h"
#include "ModelManager.h"
#include "Renderer/MaterialManager.h"


/*
	Database containing specialized databases for all project assets. Does not hold data for assets,
	mearly just references on where to find them on disk. The use of global asset mangers (such as GTextureManager)
	should should not be used; use this class instead.
*/
class AssetDatabase : public TSingleton<AssetDatabase>
{
	friend class HEngine;
	friend class HEditorEngine;
	friend class AssetImporter;
	friend class MeshImporter;
	friend class HLevel;
public:

	TextureRef GetTexture( const String& Name );
	StaticMeshGeometryRef GetStaticMesh( const String& Name );
	MaterialRef GetMaterial( const String& Name );

protected:
	AssetDatabase();
	virtual ~AssetDatabase();

	void Initialize( const Char* DatabaseFile );
	void UnInitialize();

	bool SaveAssetDatabases();
	bool IsAnyDatabaseDirty() const;

	void RegisterMesh( const Char* MeshName, const Char* Filepath );
	void RegisterTexture( const Char* TextureName, const Char* Filepath );
	void RegisterMaterial( const Char* MaterialName, const Char* Filepath );
	void RegisterActor( const Char* MaterialName, const Char* Filepath );

	const String& LookupMesh( const String& Name ) const;
	const String& LookupTexture( const String& Name ) const;
	const String& LookupMaterial( const String& Name ) const;
	const String& LookupActor( const String& Name ) const;

	MeshDatabase& GetMeshDatabase();
	TextureDatabase& GetTextureDatabase();
	MaterialDatabase& GetMaterialDatabase();
	ActorDatabase& GetActorDatabase();

protected:
	MeshDatabase m_MeshDatabase;
	TextureDatabase m_TextureDatabase;
	MaterialDatabase m_MaterialDatabase;
	ActorDatabase m_ActorDatabase;

};

//
// Inline function implementations
//


inline TextureRef AssetDatabase::GetTexture( const String& Name )
{
	return GTextureManager->LoadTexture( LookupTexture( Name ), DT_Magenta2D, false );
}

inline StaticMeshGeometryRef AssetDatabase::GetStaticMesh( const String& Name )
{
	return GStaticGeometryManager.LoadHAssetMeshFromFile( LookupMesh( Name ) );
}

inline MaterialRef AssetDatabase::GetMaterial( const String& Name )
{
	return GMaterialManager.LoadMaterialFromFile( LookupMaterial( Name ) );
}

inline MeshDatabase& AssetDatabase::GetMeshDatabase()
{
	return m_MeshDatabase;
}

inline TextureDatabase& AssetDatabase::GetTextureDatabase()
{
	return m_TextureDatabase;
}

inline MaterialDatabase& AssetDatabase::GetMaterialDatabase()
{
	return m_MaterialDatabase;
}

inline ActorDatabase& AssetDatabase::GetActorDatabase()
{
	return m_ActorDatabase;
}

inline bool AssetDatabase::SaveAssetDatabases()
{
	m_MeshDatabase.SerializeToFile( "Content/DataCache/MeshCache.json" );
	m_TextureDatabase.SerializeToFile( "Content/DataCache/TextureCache.json" );
	m_MaterialDatabase.SerializeToFile( "Content/DataCache/MaterialCache.json" );
	m_ActorDatabase.SerializeToFile( "Content/DataCache/ActorCache.json" );

	return !IsAnyDatabaseDirty();
}

inline bool AssetDatabase::IsAnyDatabaseDirty() const
{
	return m_MeshDatabase.GetIsDirty() || m_TextureDatabase.GetIsDirty() || m_MaterialDatabase.GetIsDirty() || m_ActorDatabase.GetIsDirty();
}

inline void AssetDatabase::RegisterMesh( const Char* MeshName, const Char* Filepath )
{
	m_MeshDatabase.RegisterAsset( MeshName, Filepath );
}

inline void AssetDatabase::RegisterTexture( const Char* TextureName, const Char* Filepath )
{
	m_TextureDatabase.RegisterAsset( TextureName, Filepath );
}

inline void AssetDatabase::RegisterMaterial( const Char* MaterialName, const Char* Filepath )
{
	m_MaterialDatabase.RegisterAsset( MaterialName, Filepath );
}

inline void AssetDatabase::RegisterActor( const Char* ActorName, const Char* Filepath )
{
	m_ActorDatabase.RegisterAsset( ActorName, Filepath );
}

inline const String& AssetDatabase::LookupMesh( const String& Name ) const
{
	return m_MeshDatabase.GetValueByKey( Name );
}

inline const String& AssetDatabase::LookupTexture( const String& Name ) const
{
	return m_TextureDatabase.GetValueByKey( Name );
}

inline const String& AssetDatabase::LookupMaterial( const String& Name ) const
{
	return m_MaterialDatabase.GetValueByKey( Name );
}

inline const String& AssetDatabase::LookupActor( const String& Name ) const
{
	return m_ActorDatabase.GetValueByKey( Name );
}
