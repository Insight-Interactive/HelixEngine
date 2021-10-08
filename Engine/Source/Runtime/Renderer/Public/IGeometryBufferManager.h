#pragma once

#include "RendererFwd.h"
#include "RendererCore.h"


class RENDER_API FGeometryBufferManager
{
	friend class FRenderContext;
public:
	virtual VertexBufferUID AllocateVertexBuffer() = 0;
	virtual IndexBufferUID AllocateIndexBuffer() = 0;

	virtual void DeAllocateVertexBuffer( VertexBufferUID& UID ) = 0;
	virtual void DeAllocateIndexBuffer( IndexBufferUID& UID ) = 0;

	virtual FVertexBuffer& GetVertexBufferByUID( const VertexBufferUID& UID ) = 0;
	virtual FIndexBuffer& GetIndexBufferByUID( const IndexBufferUID& UID ) = 0;

protected:
	FGeometryBufferManager() {}
	virtual ~FGeometryBufferManager() {}

	static VertexBufferUID SNextVertexBufferID;
	static IndexBufferUID SNextIndexBufferID;
};
