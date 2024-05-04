// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"
#include "RendererCore.h"

#include "CriticalSection.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"


class RENDER_API FGeometryBufferManager
{
	friend class FRenderContext;
	friend class FRenderContextFactory;
public:
	FGeometryBufferManager()
	{
	}
	virtual ~FGeometryBufferManager()
	{
	}

	VertexBufferUID AllocateVertexBuffer();
	IndexBufferUID AllocateIndexBuffer();

	void DeAllocateVertexBuffer( VertexBufferUID& UID );
	void DeAllocateIndexBuffer( IndexBufferUID& UID );

	FVertexBuffer& GetVertexBufferByUID( const VertexBufferUID& UID );
	FIndexBuffer& GetIndexBufferByUID( const IndexBufferUID& UID );

protected:

	CriticalSection m_VertexBufferGuard;
	CriticalSection m_IndexBufferGuard;
	std::unordered_map<VertexBufferUID, FVertexBuffer> m_VertexBufferLUT;
	std::unordered_map<IndexBufferUID, FIndexBuffer> m_IndexBufferLUT;

	static VertexBufferUID SNextVertexBufferID;
	static IndexBufferUID SNextIndexBufferID;
};

//
// Inline function implementations
//

FORCEINLINE FVertexBuffer& FGeometryBufferManager::GetVertexBufferByUID( const VertexBufferUID& UID )
{
	HE_ASSERT( UID != HE_INVALID_VERTEX_BUFFER_HANDLE );
	return m_VertexBufferLUT.at( UID );
}

FORCEINLINE FIndexBuffer& FGeometryBufferManager::GetIndexBufferByUID( const IndexBufferUID& UID )
{
	HE_ASSERT( UID != HE_INVALID_INDEX_BUFFER_HANDLE );
	return m_IndexBufferLUT.at( UID );
}