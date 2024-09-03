// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "StaticMeshGeometry.h"
#include "CriticalSection.h"

#include "VertexLayouts.h"
#include "Path.h"


/*
	Light wrapper around HStaticMeshGeometry to define higher level loading state and 
	cache storage inside FStaticGeometryManager.
*/
class RENDER_API HManagedStaticMeshGeometry : public HStaticMeshGeometry
{
	friend class FStaticGeometryManager;
public:
	virtual ~HManagedStaticMeshGeometry()
	{
		m_IsValid = false;
		m_IsLoading = false;
	}
	void WaitForLoad()	const;
	bool IsValid()		const { return m_IsValid; }

protected:
	HManagedStaticMeshGeometry( const String& HashName )
		: m_MapKey( HashName )
		, m_IsValid( false )
		, m_IsLoading( true )
	{
	}

	void SetLoadCompleted( bool IsCompleted )
	{
		m_IsLoading = !IsCompleted;
		m_IsValid = IsCompleted;
	}

	void Unload();

private:
	String m_MapKey; // For deleting from the map later.
	bool m_IsValid;
	bool m_IsLoading;

};
typedef std::shared_ptr<HManagedStaticMeshGeometry> StaticMeshGeometryRef;
typedef StaticMeshGeometryRef HStaticMesh;


/*
	Keeps track of the static mesh geometry currently loaded in the world.
*/
class RENDER_API FStaticGeometryManager
{
public:
	FStaticGeometryManager()
	{
	}
	~FStaticGeometryManager()
	{
	}

	StaticMeshGeometryRef LoadHAssetMeshFromFile( const String& FilePath );
	void LoadGometry( FPath& FilePath, std::vector<FSimpleVertex3D>& outVerticies, uint32& outVertexCount, std::vector<uint32>& outIndices, uint32& outIndexCount );

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

	StaticMeshGeometryRef GetStaticMeshByName( const String& Name );
	StaticMeshGeometryRef RegisterGeometry( const std::string& Name, void* VertexData, uint32 NumVerticies, uint32 VertexSizeInBytes, void* IndexData, uint32 IndexDataSizeInBytes, uint32 NumIndices );


private:
	CriticalSection m_MapMutex;
	std::unordered_map< String, StaticMeshGeometryRef > m_ModelCache;

};


//
// Inline function implementations
//

inline StaticMeshGeometryRef FStaticGeometryManager::GetStaticMeshByName( const String& Name )
{
	auto Iter = m_ModelCache.find( Name );
	if (Iter != m_ModelCache.end())
	{
		return m_ModelCache.at( Name );
	}
	else
	{
		HE_ASSERT( false );
	}

	return NULL;
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
