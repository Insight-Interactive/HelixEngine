// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RendererCore.h"

#include "GpuResource.h"


class FCommandContext;

class RENDER_API FIndexBuffer : public FGpuResource
{
	friend class FGeometryBufferManager;
public:
	FIndexBuffer()
	{
	}
	virtual ~FIndexBuffer() 
	{
	}

	void* GetNativeBufferView();
	void Create(const WChar* Name, uint32 IndexDataSize, void* pIndices);

protected:

	void SetUID(IndexBufferUID& NewUID) { m_UID = NewUID; }

	IndexBufferUID m_UID;

protected:
#if R_WITH_D3D12
	D3D12_INDEX_BUFFER_VIEW m_D3D12IndexBufferView;
#endif

};


//
// Inline function implementations

FORCEINLINE void* FIndexBuffer::GetNativeBufferView()
{
#if R_WITH_D3D12
	return (void*)&m_D3D12IndexBufferView;
#endif
}
