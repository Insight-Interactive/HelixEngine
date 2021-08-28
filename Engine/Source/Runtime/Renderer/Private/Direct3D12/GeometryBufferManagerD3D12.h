#pragma once

#include "RendererFwd.h"

#include "IGeometryBufferManager.h"

#include "VertexBufferD3D12.h"
#include "IndexBufferD3D12.h"


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

	virtual void DeAllocateVertexBuffer(VertexBufferUID& UID) override;
	virtual void DeAllocateIndexBuffer(IndexBufferUID& UID) override;

	FORCEINLINE virtual IVertexBuffer& GetVertexBufferByUID(VertexBufferUID& UID) override;
	FORCEINLINE virtual IIndexBuffer& GetIndexBufferByUID(IndexBufferUID& UID) override;


protected:
	std::unordered_map<VertexBufferUID, VertexBufferD3D12> m_VertexBufferLUT;
	std::unordered_map<IndexBufferUID, IndexBufferD3D12> m_IndexBufferLUT;
};

//
// Inline function implementations
//

FORCEINLINE IVertexBuffer& GeometryBufferManagerD3D12::GetVertexBufferByUID(VertexBufferUID& UID)
{
	HE_ASSERT(UID != HE_INVALID_VERTEX_BUFFER_HANDLE);
	return m_VertexBufferLUT.at(UID);
}

FORCEINLINE IIndexBuffer& GeometryBufferManagerD3D12::GetIndexBufferByUID(IndexBufferUID& UID)
{
	HE_ASSERT(UID != HE_INVALID_INDEX_BUFFER_HANDLE);
	return m_IndexBufferLUT.at(UID);
}