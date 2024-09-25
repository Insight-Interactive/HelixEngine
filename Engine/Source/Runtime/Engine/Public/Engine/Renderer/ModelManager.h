// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Engine/Renderer/StaticMesh.h"
#include "Graphics/StaticWorldMesh.h"
#include "Engine/Renderer/VertexLayouts.h"
#include "Engine/Renderer/ConstantBuffer.h"
#include "Graphics/ConstantBufferStructures.h"
#include "Path.h"
#include "CriticalSection.h"

/*
	Keeps track of the static mesh geometry currently loaded in the world.
*/
class FStaticGeometryManager
{
public:
	FStaticGeometryManager()
	{
	}
	~FStaticGeometryManager()
	{
	}

	HStaticMesh LoadHAssetMeshFromFile( const String& FilePath );
	void LoadBasicGometry( FPath& FilePath, std::vector<FSimpleVertex3D>& outVerticies, uint32& outVertexCount, std::vector<uint32>& outIndices, uint32& outIndexCount );

	/*
		Destroys a mesh from the cache and returns true if succeeded, false if not.
	*/
	bool DestroyMesh( const String& Key );

	/*
		Checks if a mesh exists in the cache and returns true if it does, false if not.
	*/
	bool MeshExists( const String& Name ) const;

	/*
		Destroys all meshes in the cache.
	*/
	void FlushCache();

	HStaticMesh GetStaticMeshByName( const String& Name );
	HStaticMesh RegisterGeometry( const String& Name, void* VertexData, uint32 NumVerticies, uint32 VertexSizeInBytes, void* IndexData, uint32 IndexDataSizeInBytes, uint32 NumIndices );
	/*
		Register geometry that is not cleared on level reloads or level switches.
	*/
	HStaticMesh RegisterAllwaysLoadedGeometry( const String& Name, void* VertexData, uint32 NumVerticies, uint32 VertexSizeInBytes, void* IndexData, uint32 IndexDataSizeInBytes, uint32 NumIndices );

	void LoadLevelGeo( const String& FilePath, std::vector<FWorldMesh*>& OutWorld );

private:
	void ProcessMesh( struct aiMesh* mesh, const struct aiScene* scene, HStaticMesh& OutMesh );
	HStaticMesh ProcessNode( struct aiNode* node, aiNode* Parent, const struct aiScene* scene, std::vector<FWorldMesh*>& OutWorld );


private:
	CriticalSection m_MapMutex;
	std::unordered_map< String, std::unique_ptr<ManagedStaticMesh> > m_ModelCache;
	CriticalSection m_AllwaysLoadedMapMutex;
	std::unordered_map< String, std::unique_ptr<ManagedStaticMesh> > m_ModelCacheAllwysLoaded;

	//std::map< String, std::pair< std::unique_ptr<ManagedStaticMesh>, std::vector<TConstantBuffer<MeshWorldCBData>> > > m_NewModelCache;
};


//
// Inline function implementations
//

inline HStaticMesh FStaticGeometryManager::GetStaticMeshByName( const String& Name )
{
	auto Iter = m_ModelCache.find( Name );
	if (Iter != m_ModelCache.end())
	{
		return m_ModelCache.at( Name ).get();
	}
	else
	{
		HE_ASSERT( false );
	}

	return nullptr;
}

inline bool FStaticGeometryManager::DestroyMesh( const String& Key )
{
	auto Iter = m_ModelCache.find( Key );
	if (Iter != m_ModelCache.end())
	{
		ScopedCriticalSection Guard( m_MapMutex );
		m_ModelCache.erase( Iter );
		return true;
	}

	return false;
}

inline bool FStaticGeometryManager::MeshExists( const String& Name ) const
{
	auto Iter = m_ModelCache.find( Name );
	return Iter != m_ModelCache.end();
}

inline void FStaticGeometryManager::FlushCache()
{
	R_LOG( Warning, TEXT( "Model cache being flushed!" ) );
	ScopedCriticalSection Guard( m_MapMutex );

	for (auto Iter = m_ModelCache.begin(); Iter != m_ModelCache.end(); ++Iter)
	{
		Iter->second.reset();
	}
	m_ModelCache.clear();
}
