#pragma once

#include "RendererFwd.h"
#include "RendererCore.h"


class RENDER_API IGeometryBufferManager
{
	friend class RenderContext;
public:
	virtual VertexBufferUID AllocateVertexBuffer() = 0;
	virtual IndexBufferUID AllocateIndexBuffer() = 0;

	virtual void DeAllocateVertexBuffer(VertexBufferUID& UID) = 0;
	virtual void DeAllocateIndexBuffer(IndexBufferUID& UID) = 0;

	virtual IVertexBuffer& GetVertexBufferByUID(VertexBufferUID& UID) = 0;
	virtual IIndexBuffer& GetIndexBufferByUID(IndexBufferUID& UID) = 0;

protected:
	IGeometryBufferManager() {}
	virtual ~IGeometryBufferManager() {}

	static VertexBufferUID s_NextVertexBufferID;
	static IndexBufferUID s_NextIndexBufferID;
};
