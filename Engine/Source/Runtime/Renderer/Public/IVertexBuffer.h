#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RendererCore.h"


class ICommandContext;

class RENDER_API IVertexBuffer
{
	friend class IGeometryBufferManager;
public:
	virtual ~IVertexBuffer() {}

	virtual void* GetNativeBufferView() = 0;

	virtual void Create(const WChar* Name, uint32 VertexDataSize, uint32 VertexStrideSize, void* pVerticies) = 0;

protected:
	IVertexBuffer() {}

	void SetUID(VertexBufferUID& NewUID) { m_UID = NewUID; }

	VertexBufferUID m_UID;
};
