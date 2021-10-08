#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RendererCore.h"


class FCommandContext;

class RENDER_API FVertexBuffer
{
	friend class FGeometryBufferManager;
public:
	virtual ~FVertexBuffer() 
	{
	}

	virtual void* GetNativeBufferView() = 0;

	virtual void Create(const WChar* Name, uint32 VertexDataSize, uint32 VertexStrideSize, void* pVerticies) = 0;

protected:
	FVertexBuffer() 
	{
	}

	void SetUID(VertexBufferUID& NewUID) { m_UID = NewUID; }

	VertexBufferUID m_UID;
};
