// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "StaticMeshGeometry.h"
#include "CriticalSection.h"


class RENDER_API HManagedStaticMeshGeometry : public HStaticMeshGeometry
{
	friend class FStaticGeometryManager;
public:
	HManagedStaticMeshGeometry()
		: m_MapKey( "<default>" )
		, m_IsValid( false )
		, m_IsLoading( true )
		, m_ReferenceCount( 0 )
	{
	}
	virtual ~HManagedStaticMeshGeometry() = default;

	void WaitForLoad() const;
	bool IsValid( void ) const { return m_IsValid; }

protected:
	HManagedStaticMeshGeometry( const std::string& HashName )
		: m_MapKey( HashName )
		, m_IsValid( false )
		, m_IsLoading( true )
		, m_ReferenceCount( 0 )
	{
	}
	void SetLoadCompleted( bool Completed )
	{
		m_IsLoading = !Completed;
		m_IsValid = Completed;
	}

protected:
	void Unload();

	String m_MapKey; // For deleting from the map later.
	bool m_IsValid;
	bool m_IsLoading;
	uint64 m_ReferenceCount;
};
typedef std::shared_ptr<HManagedStaticMeshGeometry> StaticMeshGeometryRef;


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
	for (auto Iter = m_ModelCache.begin(); Iter != m_ModelCache.end(); ++Iter)
	{
		Iter->second.reset();
	}
	m_ModelCache.clear();
}
