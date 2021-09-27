#pragma once

#include "RendererFwd.h"

#include "IGeometryBufferManager.h"

#include "VertexBufferD3D12.h"
#include "IndexBufferD3D12.h"
#include "CriticalSection.h"


class RENDER_API GeometryBufferManagerD3D12 : public IGeometryBufferManager
{
public:
	GeometryBufferManagerD3D12()
	{
	}
	virtual ~GeometryBufferManagerD3D12()
	{
	}

	virtual VertexBufferUID AllocateVertexBuffer() override;
	virtual IndexBufferUID AllocateIndexBuffer() override;

	virtual void DeAllocateVertexBuffer( VertexBufferUID& UID ) override;
	virtual void DeAllocateIndexBuffer( IndexBufferUID& UID ) override;

	virtual IVertexBuffer& GetVertexBufferByUID( const VertexBufferUID& UID ) override;
	virtual IIndexBuffer& GetIndexBufferByUID( const IndexBufferUID& UID ) override;

protected:
	CriticalSection m_VertexBufferGuard;
	CriticalSection m_IndexBufferGuard;
	std::unordered_map<VertexBufferUID, VertexBufferD3D12> m_VertexBufferLUT;
	std::unordered_map<IndexBufferUID, IndexBufferD3D12> m_IndexBufferLUT;

};

//
// Inline function implementations
//

FORCEINLINE IVertexBuffer& GeometryBufferManagerD3D12::GetVertexBufferByUID( const VertexBufferUID& UID )
{
	HE_ASSERT( UID != HE_INVALID_VERTEX_BUFFER_HANDLE );
	return m_VertexBufferLUT.at( UID );
}

FORCEINLINE IIndexBuffer& GeometryBufferManagerD3D12::GetIndexBufferByUID( const IndexBufferUID& UID )
{
	HE_ASSERT( UID != HE_INVALID_INDEX_BUFFER_HANDLE );
	return m_IndexBufferLUT.at( UID );
}