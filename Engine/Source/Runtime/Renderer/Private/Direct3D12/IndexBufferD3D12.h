#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "IIndexBuffer.h"
#include "GpuResourceD3D12.h"


class RENDER_API IndexBufferD3D12 : public IIndexBuffer, public GpuResourceD3D12
{
	friend class GeometryBufferManagerD3D12;
public:
	IndexBufferD3D12()
	{
	}
	virtual ~IndexBufferD3D12() 
	{
	}

	virtual void* GetNativeBufferView() override { return RCast<void*>(&m_D3D12IndexBufferView); }

	virtual void Create(const WChar* Name, uint32 IndexDataSize, void* pIndices) override;


protected:
	D3D12_INDEX_BUFFER_VIEW m_D3D12IndexBufferView;

};
