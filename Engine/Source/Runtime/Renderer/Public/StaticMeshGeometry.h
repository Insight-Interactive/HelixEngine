#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "IGeometryBufferManager.h"
#include "RendererCore.h"


struct DrawArgs
{
	uint32 NumVerts;
	uint32 NumIndices;
	VertexBufferUID VertexBufferHandle;
	IndexBufferUID IndexBufferHandle;
};

class RENDER_API StaticMeshGeometry
{
	friend class StaticGeometryManager;
public:
	inline StaticMeshGeometry();
	inline virtual ~StaticMeshGeometry();

	//
	// Getters and Setters
	//
	inline IVertexBuffer& GetVertexBuffer();
	inline IIndexBuffer& GetIndexBuffer();
	inline uint32 GetNumVerticies() const;
	inline uint32 GetNumIndices() const;
	inline uint64 GetUID() const;

	void Create(void* pVertexData, uint32 VertexDataSize, uint32 NumVerticies, uint32 VertexSize, void* pIndexData, uint32 IndexDataSize, uint32 NumIndices);

private:
	inline void Initialize();
	inline void UnInitialize();

	inline void SetHashName(uint64 NewUID);


protected:
	DrawArgs m_DrawArgs;

	uint64 m_Uid;
	// TODO AABB for culling
};


//
// Inline function definitions
//

inline StaticMeshGeometry::StaticMeshGeometry()
{
	Initialize();
}

inline StaticMeshGeometry::~StaticMeshGeometry()
{
	UnInitialize();
}

inline IVertexBuffer& StaticMeshGeometry::GetVertexBuffer()
{
	return GGeometryManager->GetVertexBufferByUID(m_DrawArgs.VertexBufferHandle);
}

inline IIndexBuffer& StaticMeshGeometry::GetIndexBuffer()
{
	return GGeometryManager->GetIndexBufferByUID(m_DrawArgs.IndexBufferHandle);
}

inline uint32 StaticMeshGeometry::GetNumVerticies() const
{
	return m_DrawArgs.NumVerts;
}

inline uint32 StaticMeshGeometry::GetNumIndices() const
{
	return m_DrawArgs.NumIndices;
}

inline uint64 StaticMeshGeometry::GetUID() const
{
	return m_Uid;
}

inline void StaticMeshGeometry::Initialize()
{
	m_DrawArgs.VertexBufferHandle = GGeometryManager->AllocateVertexBuffer();
	m_DrawArgs.IndexBufferHandle = GGeometryManager->AllocateIndexBuffer();
}

inline void StaticMeshGeometry::UnInitialize()
{
	GGeometryManager->DeAllocateVertexBuffer(m_DrawArgs.VertexBufferHandle);
	GGeometryManager->DeAllocateIndexBuffer(m_DrawArgs.IndexBufferHandle);
}

inline void StaticMeshGeometry::SetHashName(uint64 NewGUID)
{
	m_Uid = NewGUID;
}
