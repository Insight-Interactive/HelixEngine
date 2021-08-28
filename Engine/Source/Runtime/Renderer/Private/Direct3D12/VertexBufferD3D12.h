#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "IVertexBuffer.h"
#include "GpuResourceD3D12.h"


class RENDER_API VertexBufferD3D12 : public IVertexBuffer, public GpuResourceD3D12
{
	friend class GeometryBufferManagerD3D12;
public:
	VertexBufferD3D12() {}
	virtual ~VertexBufferD3D12() {}

	virtual void* GetNativeBufferView() override { return RCast<void*>(&m_D3D12VertexBufferView); }

	virtual void Create(const WChar* Name, uint32 VertexDataSize, uint32 VertexStrideSize, void* pVerticies) override;

protected:
	D3D12_VERTEX_BUFFER_VIEW m_D3D12VertexBufferView;
};
