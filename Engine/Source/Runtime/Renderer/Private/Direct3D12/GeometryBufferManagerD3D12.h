#pragma once

#include "RendererFwd.h"

#include "IGeometryBufferManager.h"

#include "VertexBufferD3D12.h"
#include "IndexBufferD3D12.h"
#include "CriticalSection.h"


class RENDER_API FGeometryBufferManagerD3D12 : public FGeometryBufferManager
{
public:
	FGeometryBufferManagerD3D12()
	{
	}
	virtual ~FGeometryBufferManagerD3D12()
	{
	}

	virtual VertexBufferUID AllocateVertexBuffer() override;
	virtual IndexBufferUID AllocateIndexBuffer() override;

	virtual void DeAllocateVertexBuffer( VertexBufferUID& UID ) override;
	virtual void DeAllocateIndexBuffer( IndexBufferUID& UID ) override;

	virtual FVertexBuffer& GetVertexBufferByUID( const VertexBufferUID& UID ) override;
	virtual FIndexBuffer& GetIndexBufferByUID( const IndexBufferUID& UID ) override;

protected:
	CriticalSection m_VertexBufferGuard;
	CriticalSection m_IndexBufferGuard;
	std::unordered_map<VertexBufferUID, FVertexBufferD3D12> m_VertexBufferLUT;
	std::unordered_map<IndexBufferUID, FIndexBufferD3D12> m_IndexBufferLUT;

};

//
// Inline function implementations
//

FORCEINLINE FVertexBuffer& FGeometryBufferManagerD3D12::GetVertexBufferByUID( const VertexBufferUID& UID )
{
	HE_ASSERT( UID != HE_INVALID_VERTEX_BUFFER_HANDLE );
	return m_VertexBufferLUT.at( UID );
}

FORCEINLINE FIndexBuffer& FGeometryBufferManagerD3D12::GetIndexBufferByUID( const IndexBufferUID& UID )
{
	HE_ASSERT( UID != HE_INVALID_INDEX_BUFFER_HANDLE );
	return m_IndexBufferLUT.at( UID );
}