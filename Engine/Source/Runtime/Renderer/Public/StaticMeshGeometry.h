#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "GeometryBufferManager.h"
#include "RendererCore.h"


struct FDrawArgs
{
	FDrawArgs()
		: NumVerts( 0 )
		, NumIndices( 0 )
		, VertexBufferHandle( HE_INVALID_VERTEX_BUFFER_HANDLE )
		, IndexBufferHandle( HE_INVALID_INDEX_BUFFER_HANDLE )
	{
	}
	~FDrawArgs()
	{
		NumVerts = -1;
		NumIndices = -1;
		VertexBufferHandle = HE_INVALID_VERTEX_BUFFER_HANDLE;
		IndexBufferHandle = HE_INVALID_INDEX_BUFFER_HANDLE;
	}
	uint32 NumVerts;
	uint32 NumIndices;
	VertexBufferUID VertexBufferHandle;
	IndexBufferUID IndexBufferHandle;
};

/*
	Base class for all static geometry that exists in the world. That is, a peice of 
	geometry with static unskinned polygons.
*/
class RENDER_API HStaticMeshGeometry
{
	friend class FStaticGeometryManager;
public:
	HStaticMeshGeometry()
	{
		Initialize();
	}
	virtual ~HStaticMeshGeometry()
	{
		UnInitialize();
	}

	//
	// Getters and Setters
	//
	FVertexBuffer& GetVertexBuffer();
	FIndexBuffer& GetIndexBuffer();
	uint32 GetNumVerticies() const;
	uint32 GetNumIndices() const;
	uint64 GetUID() const;

	void Create( void* pVertexData, uint32 NumVerticies, uint32 VertexSize, void* pIndexData, uint32 IndexDataSize, uint32 NumIndices );

private:
	void Initialize();
	void UnInitialize();

	void SetHashName( uint64 NewUID );


protected:
	FDrawArgs m_DrawArgs;

	uint64 m_Uid;
	// TODO AABB for culling
};


//
// Inline function definitions
//


inline FVertexBuffer& HStaticMeshGeometry::GetVertexBuffer()
{
	return GGeometryManager.GetVertexBufferByUID( m_DrawArgs.VertexBufferHandle );
}

inline FIndexBuffer& HStaticMeshGeometry::GetIndexBuffer()
{
	return GGeometryManager.GetIndexBufferByUID( m_DrawArgs.IndexBufferHandle );
}

inline uint32 HStaticMeshGeometry::GetNumVerticies() const
{
	return m_DrawArgs.NumVerts;
}

inline uint32 HStaticMeshGeometry::GetNumIndices() const
{
	return m_DrawArgs.NumIndices;
}

inline uint64 HStaticMeshGeometry::GetUID() const
{
	return m_Uid;
}

inline void HStaticMeshGeometry::Initialize()
{
	m_DrawArgs.VertexBufferHandle = GGeometryManager.AllocateVertexBuffer();
	m_DrawArgs.IndexBufferHandle = GGeometryManager.AllocateIndexBuffer();
}

inline void HStaticMeshGeometry::UnInitialize()
{
	GGeometryManager.DeAllocateVertexBuffer( m_DrawArgs.VertexBufferHandle );
	GGeometryManager.DeAllocateIndexBuffer( m_DrawArgs.IndexBufferHandle );
}

inline void HStaticMeshGeometry::SetHashName( uint64 NewGUID )
{
	m_Uid = NewGUID;
}
