#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "GpuResource.h"

#include "RendererCore.h"


class FCommandContext;

class RENDER_API FVertexBuffer : public FGpuResource
{
	friend class FGeometryBufferManager;
public:
	FVertexBuffer()
	{
	}
	virtual ~FVertexBuffer() 
	{
	}

	void* GetNativeBufferView();

	void Create(const WChar* Name, uint32 VertexDataSize, uint32 VertexStrideSize, void* pVerticies);

protected:
	void SetUID(VertexBufferUID& NewUID) { m_UID = NewUID; }

	VertexBufferUID m_UID;

protected:
#if R_WITH_D3D12
	D3D12_VERTEX_BUFFER_VIEW m_D3D12VertexBufferView;

#endif
};


//
// Inline function implementations

FORCEINLINE void* FVertexBuffer::GetNativeBufferView()
{
#if R_WITH_D3D12
	return (void*)&m_D3D12VertexBufferView;
#endif
}

