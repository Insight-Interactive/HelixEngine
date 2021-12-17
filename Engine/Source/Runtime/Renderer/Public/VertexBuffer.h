#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "GpuResource.h"

#include "RendererCore.h"


class FCommandContext;

/*
	Vertex buffer whos verticies are copies once to the Gpu and never change.
*/
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

	/*
		Creates the vertex buffer and registers it with the Gpu.
		@param Name - Debug name for the vertex buffer.
		@param VertexDataSize - Size of the vertex data to upload.
		@param VertexStrideSize - Size of each vertex.
		@param pVerticies - The raw vertex data.
		@param IsStaticUpload - Is this vertex buffer re-upoaded every bind call?
	*/
	virtual void Create( const WChar* Name, uint32 VertexDataSize, uint32 VertexStrideSize, void* pVerticies );


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

// FVertexBuffer
//

FORCEINLINE void* FVertexBuffer::GetNativeBufferView()
{
#if R_WITH_D3D12
	return (void*)&m_D3D12VertexBufferView;
#endif
}

/*
	Vertex buffer whos verticies are updated on the Cpu and re-uploaded to the Gpu.
*/
class FDynamicVertexBuffer : public FVertexBuffer
{
public:
	FDynamicVertexBuffer()
		: m_WritePtr( nullptr )
		, m_pVertexData( nullptr )
		, m_VertexDataSize( 0 )
	{
	}
	virtual ~FDynamicVertexBuffer()
	{
	}

	virtual void Create( const WChar* Name, uint32 VertexDataSize, uint32 VertexStrideSize, void* pVerticies ) override;

	void Upload();

	uint8* GetGpuWritePointer();

private:
	uint8* m_WritePtr;
	uint8* m_pVertexData;
	uint32 m_VertexDataSize;

};


//
// Inline function implementations
//

FORCEINLINE uint8* FDynamicVertexBuffer::GetGpuWritePointer()
{
	return m_WritePtr;
}

FORCEINLINE void FDynamicVertexBuffer::Upload()
{
	memcpy( GetGpuWritePointer(), (const void*)m_pVertexData, m_VertexDataSize );
}
