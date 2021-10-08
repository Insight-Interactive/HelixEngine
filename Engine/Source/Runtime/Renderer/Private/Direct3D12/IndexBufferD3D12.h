#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "IIndexBuffer.h"
#include "GpuResourceD3D12.h"


class RENDER_API FIndexBufferD3D12 : public FIndexBuffer, public FGpuResourceD3D12
{
	friend class FGeometryBufferManagerD3D12;
public:
	FIndexBufferD3D12()
	{
	}
	virtual ~FIndexBufferD3D12() 
	{
	}

	virtual void* GetNativeBufferView() override { return RCast<void*>(&m_D3D12IndexBufferView); }

	virtual void Create(const WChar* Name, uint32 IndexDataSize, void* pIndices) override;


protected:
	D3D12_INDEX_BUFFER_VIEW m_D3D12IndexBufferView;

};
