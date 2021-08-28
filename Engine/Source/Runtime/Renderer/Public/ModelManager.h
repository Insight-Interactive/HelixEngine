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

namespace ofbx
{
	struct Mesh;
}

class RENDER_API StaticGeometryManager
{
public:
	StaticGeometryManager() = default;
	~StaticGeometryManager() = default;

	void LoadFBXFromFile(const char* FilePath);
	void DestroyMesh(const String& Key);
	bool MeshExists(const String& Name);
	void FlushCache();

	StaticMeshGeometryRef GetStaticMeshByName(const std::string& Name)
	{
		return m_ModelCache.at(Name).get();
	}

	StaticMeshGeometryRef RegisterGeometry(const std::string& Name, void* Verticies, uint32 VertexDataSizeInBytes, uint32 NumVerticies, uint32 VertexSize, void* Indices, uint32 IndexDataSizeInBytes, uint32 NumIndices);

protected:
	void ParseFBXMesh(const ofbx::Mesh& Mesh, std::vector<struct Vertex3D>& OutVerticies, std::vector<uint32>& OutIndices);

private:
	std::map< String, std::unique_ptr<ManagedStaticMeshGeometry> > m_ModelCache;
	CriticalSection m_MapMutex;
};
