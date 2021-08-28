#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RendererCore.h"

class ICommandContext;

class RENDER_API IIndexBuffer
{
	friend class IGeometryBufferManager;
public:
	virtual ~IIndexBuffer() {}

	virtual void* GetNativeBufferView() = 0;

	virtual void Create(const WChar* Name, uint32 IndexDataSize, void* pIndices) = 0;

protected:
	IIndexBuffer() {}

	void SetUID(IndexBufferUID& NewUID) { m_UID = NewUID; }

	IndexBufferUID m_UID;
};
