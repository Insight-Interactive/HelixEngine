// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "StaticMeshGeometry.h"
#include "CriticalSection.h"

class RENDER_API ManagedStaticMeshGeometry : public StaticMeshGeometry
{
	friend class StaticMeshGeometryRef;
	friend class StaticGeometryManager;
public:
	ManagedStaticMeshGeometry() = default;
	virtual ~ManagedStaticMeshGeometry() = default;

	void WaitForLoad() const;

protected:
	ManagedStaticMeshGeometry(const std::string& HashName)
		: m_MapKey(HashName)
	{
	}
	void SetLoadCompleted(bool Completed)
	{
		m_IsLoading = !Completed;
	}

protected:
	bool IsValid(void) const { return m_IsValid; }
	void Unload();

	std::string m_MapKey; // For deleting from the map later.
	bool m_IsValid;
	bool m_IsLoading;
	uint64 m_ReferenceCount;
};


class RENDER_API StaticMeshGeometryRef
{
public:
	StaticMeshGeometryRef(const StaticMeshGeometryRef& ref);
	StaticMeshGeometryRef(ManagedStaticMeshGeometry* tex = nullptr);
	~StaticMeshGeometryRef();
	StaticMeshGeometryRef& operator=(const StaticMeshGeometryRef& Other)
	{
		this->m_Ref = Other.m_Ref;
		return *this;
	}


	void operator= (std::nullptr_t);
	void operator= (StaticMeshGeometryRef& rhs);

	// Check that this points to a valid peice of static geometry (which loaded successfully)
	bool IsValid() const;

	// Get the texture pointer.  Client is responsible to not dereference
	// null pointers.
	StaticMeshGeometry* Get();

	StaticMeshGeometry* operator->();

private:
	ManagedStaticMeshGeometry* m_Ref;
};


class RENDER_API StaticGeometryManager
{
public:
	StaticGeometryManager() = default;
	~StaticGeometryManager() = default;

	StaticMeshGeometryRef LoadHAssetMeshFromFile(const char* FilePath);
	void DestroyMesh(const String& Key);
	bool MeshExists(const String& Name);
	void FlushCache();

	StaticMeshGeometryRef GetStaticMeshByName( const String& Name );

	StaticMeshGeometryRef RegisterGeometry(const std::string& Name, void* Verticies, uint32 NumVerticies, uint32 VertexSizeInBytes, void* Indices, uint32 IndexDataSizeInBytes, uint32 NumIndices);


private:
	std::map< String, std::unique_ptr<ManagedStaticMeshGeometry> > m_ModelCache;
	CriticalSection m_MapMutex;
};


//
// Inline function implementations
//

inline StaticMeshGeometryRef StaticGeometryManager::GetStaticMeshByName( const String& Name )
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

	return null;
}