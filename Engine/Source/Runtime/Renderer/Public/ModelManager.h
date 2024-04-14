// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "StaticMeshGeometry.h"
#include "SkeletalMeshGeometry.h"
#include "CriticalSection.h"


/*
	Light wrapper around HStaticMeshGeometry to define higher level loading state and 
	cache storage inside FGeometryManager.
*/
template <typename BaseClass>
class RENDER_API ManagedMeshGeometry : public BaseClass
{
	friend class FGeometryManager;
public:
	virtual ~ManagedMeshGeometry()
	{
		m_IsValid = false;
		m_IsLoading = false;
	}
	void WaitForLoad()	const
	{
		while ((volatile bool&)m_IsLoading)
			std::this_thread::yield();
	}
	inline bool IsValid()		const { HE_ASSERT( this != nullptr ); return m_IsValid; }

protected:
	ManagedMeshGeometry( const String& HashName )
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

	void Unload()
	{
		GGeometryManager.DestroyMesh( m_MapKey );
	}

private:
	String m_MapKey; // For deleting from the map later.
	bool m_IsValid;
	bool m_IsLoading;

};

typedef ManagedMeshGeometry<HStaticMeshGeometry> HManagedStaticMeshGeometry;
typedef std::shared_ptr<HManagedStaticMeshGeometry> StaticMeshGeometryRef;
typedef StaticMeshGeometryRef HStaticMesh;

typedef ManagedMeshGeometry<HSkeletalMeshGeometry> HManagedSkeletalMeshGeometry;
typedef std::shared_ptr<HManagedSkeletalMeshGeometry> SkeletalMeshGeometryRef;
typedef SkeletalMeshGeometryRef HSkeletalMesh;


struct aiNode;
struct aiScene;
struct aiMesh;
struct aiBone;
struct FVertex3D;

/*
	Keeps track of the static mesh geometry currently loaded in the world.
*/
class RENDER_API FGeometryManager
{
public:
	FGeometryManager()
	{
	}
	~FGeometryManager()
	{
	}

	StaticMeshGeometryRef LoadHAssetMeshFromFile( const String& FilePath );
	SkeletalMeshGeometryRef LoadSkeletalMeshFromFile( const String& FilePath );

	/*
		Destroys a static mesh from the cache and returns true if succeeded, false if not.
	*/
	bool DestroyStaticMesh( const String& Key );

	bool DestroySkeletalMesh( const String& Key );

	/*
		Checks if a mesh exists in the cache and returns true if it does, false if not.
	*/
	bool StaticMeshExists( const String& Name ) const;

	bool SkeletalMeshExists( const String& Name ) const;


	/*
		Destroys all meshes in the cache.
	*/
	void FlushCache();

	StaticMeshGeometryRef GetStaticMeshByName( const String& Name );
	SkeletalMeshGeometryRef GetSkeletalMeshByName( const String& Name );
	StaticMeshGeometryRef RegisterGeometry( const std::string& Name, void* VertexData, uint32 NumVerticies, uint32 VertexSizeInBytes, void* IndexData, uint32 IndexDataSizeInBytes, uint32 NumIndices );

private:
	StaticMeshGeometryRef ProcessNode( aiNode* node, const aiScene* scene );
	StaticMeshGeometryRef ProcessMesh( aiMesh* mesh, const aiScene* scene );

	SkeletalMeshGeometryRef SK_ParseMeshes( const aiScene* pScene );
	SkeletalMeshGeometryRef SK_ParseScene( const aiScene* pScene );


private:
	CriticalSection m_StaticModelCacheMutex;
	std::unordered_map< String, StaticMeshGeometryRef > m_StaticModelCache;
	CriticalSection m_SkeletalModelCacheMutex;
	std::unordered_map< String, SkeletalMeshGeometryRef > m_SkeletalModelCache;

};


//
// Inline function implementations
//

inline StaticMeshGeometryRef FGeometryManager::GetStaticMeshByName( const String& Name )
{
	auto Iter = m_StaticModelCache.find( Name );
	if (Iter != m_StaticModelCache.end())
	{
		return m_StaticModelCache.at( Name );
	}
	else
	{
		HE_ASSERT( false );
	}

	return NULL;
}

inline SkeletalMeshGeometryRef FGeometryManager::GetSkeletalMeshByName( const String& Name )
{
	auto Iter = m_SkeletalModelCache.find( Name );
	if (Iter != m_SkeletalModelCache.end())
	{
		return m_SkeletalModelCache.at( Name );
	}
	else
	{
		HE_ASSERT( false );
	}

	return NULL;
}

inline bool FGeometryManager::DestroyStaticMesh( const String& Key )
{
	auto Iter = m_StaticModelCache.find( Key );
	if (Iter != m_StaticModelCache.end())
	{
		ScopedCriticalSection Guard( m_StaticModelCacheMutex );
		m_StaticModelCache.erase( Iter );
		return true;
	}

	return false;
}

inline bool FGeometryManager::DestroySkeletalMesh( const String& Key )
{
	auto Iter = m_SkeletalModelCache.find( Key );
	if (Iter != m_SkeletalModelCache.end())
	{
		ScopedCriticalSection Guard( m_SkeletalModelCacheMutex );
		m_SkeletalModelCache.erase( Iter );
		return true;
	}

	return false;
}

inline bool FGeometryManager::StaticMeshExists( const String& Name ) const
{
	auto Iter = m_SkeletalModelCache.find( Name );
	return Iter != m_SkeletalModelCache.end();
}

inline bool FGeometryManager::SkeletalMeshExists( const String& Name ) const
{
	auto Iter = m_StaticModelCache.find( Name );
	return Iter != m_StaticModelCache.end();
}

inline void FGeometryManager::FlushCache()
{
	R_LOG( Warning, TEXT( "Model cache being flushed!" ) );

	{
		ScopedCriticalSection Guard( m_StaticModelCacheMutex );

		for (auto Iter = m_StaticModelCache.begin(); Iter != m_StaticModelCache.end(); ++Iter)
		{
			Iter->second.reset();
		}
		m_StaticModelCache.clear();
	}

	{
		ScopedCriticalSection Guard( m_SkeletalModelCacheMutex );

		for (auto Iter = m_SkeletalModelCache.begin(); Iter != m_SkeletalModelCache.end(); ++Iter)
		{
			Iter->second.reset();
		}
		m_SkeletalModelCache.clear();
	}
}
