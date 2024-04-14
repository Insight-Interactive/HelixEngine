#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "GeometryBufferManager.h"
#include "GUID.h"


struct RENDER_API FMeshDrawArgs 
{
	FMeshDrawArgs()
		: NumVerts( 0 )
		, NumIndices( 0 )
		, BaseVert( 0 )
		, BaseIndex( 0 )
		, VertexBufferHandle( HE_INVALID_VERTEX_BUFFER_HANDLE )
		, IndexBufferHandle( HE_INVALID_INDEX_BUFFER_HANDLE )
	{
	}
	~FMeshDrawArgs()
	{
		NumVerts = -1;
		NumIndices = -1;
		VertexBufferHandle = HE_INVALID_VERTEX_BUFFER_HANDLE;
		IndexBufferHandle = HE_INVALID_INDEX_BUFFER_HANDLE;
	}
	uint32 NumVerts;
	uint32 NumIndices;
	uint32 BaseVert;
	uint32 BaseIndex;
	VertexBufferUID VertexBufferHandle;
	IndexBufferUID IndexBufferHandle;

};

class RENDER_API FMeshGeometry
{
	friend class FGeometryManager;
public:
	//
	// Getters and Setters
	//
	FVertexBuffer& GetVertexBuffer();
	FIndexBuffer& GetIndexBuffer();
	uint32 GetNumVerticies() const;
	uint32 GetNumIndices() const;
	FGUID GetGUID() const;

	FMeshGeometry()
	{
		Initialize();
	}
	virtual ~FMeshGeometry()
	{
		UnInitialize();
	}

protected:
	void Initialize();
	void UnInitialize();
	void SetHashName( const FGUID& NewUID );
	void SetName( const String& Name );
	void Create( void* pVertexData, uint32 NumVerticies, uint32 VertexSize, void* pIndexData, uint32 IndexDataSize, uint32 NumIndices );

protected:
	FMeshDrawArgs m_DrawArgs;
	FGUID m_Guid;
	String m_Name;
	// TODO AABB for culling

};

//
// Inline function definitions
//


inline FVertexBuffer& FMeshGeometry::GetVertexBuffer()
{
	HE_ASSERT( m_DrawArgs.VertexBufferHandle != HE_INVALID_VERTEX_BUFFER_HANDLE );
	return GGeometryBufferManager.GetVertexBufferByUID( m_DrawArgs.VertexBufferHandle );
}

inline FIndexBuffer& FMeshGeometry::GetIndexBuffer()
{
	HE_ASSERT( m_DrawArgs.IndexBufferHandle != HE_INVALID_INDEX_BUFFER_HANDLE );
	return GGeometryBufferManager.GetIndexBufferByUID( m_DrawArgs.IndexBufferHandle );
}

inline uint32 FMeshGeometry::GetNumVerticies() const
{
	return m_DrawArgs.NumVerts;
}

inline uint32 FMeshGeometry::GetNumIndices() const
{
	return m_DrawArgs.NumIndices;
}

inline FGUID FMeshGeometry::GetGUID() const
{
	return m_Guid;
}

inline void FMeshGeometry::Initialize()
{
	HE_ASSERT( m_DrawArgs.VertexBufferHandle == HE_INVALID_VERTEX_BUFFER_HANDLE );
	HE_ASSERT( m_DrawArgs.IndexBufferHandle == HE_INVALID_INDEX_BUFFER_HANDLE );

	m_DrawArgs.VertexBufferHandle = GGeometryBufferManager.AllocateVertexBuffer();
	m_DrawArgs.IndexBufferHandle = GGeometryBufferManager.AllocateIndexBuffer();
}

inline void FMeshGeometry::UnInitialize()
{
	HE_ASSERT( m_DrawArgs.VertexBufferHandle != HE_INVALID_VERTEX_BUFFER_HANDLE );
	HE_ASSERT( m_DrawArgs.IndexBufferHandle != HE_INVALID_INDEX_BUFFER_HANDLE );

	GGeometryBufferManager.DeAllocateVertexBuffer( m_DrawArgs.VertexBufferHandle );
	GGeometryBufferManager.DeAllocateIndexBuffer( m_DrawArgs.IndexBufferHandle );
}

inline void FMeshGeometry::SetHashName( const FGUID& NewGUID )
{
	m_Guid = NewGUID;
}

inline void FMeshGeometry::SetName( const String& Name )
{
	m_Name = Name;
}