#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RendererCore.h"

class FCommandContext;

class RENDER_API FIndexBuffer
{
	friend class FGeometryBufferManager;
public:
	virtual ~FIndexBuffer() {}

	virtual void* GetNativeBufferView() = 0;

	virtual void Create(const WChar* Name, uint32 IndexDataSize, void* pIndices) = 0;

protected:
	FIndexBuffer() {}

	void SetUID(IndexBufferUID& NewUID) { m_UID = NewUID; }

	IndexBufferUID m_UID;
};
