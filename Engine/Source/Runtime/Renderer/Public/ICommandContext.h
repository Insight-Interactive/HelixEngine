#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "CommonEnums.h"
#include "CriticalSection.h"
#include "RendererCore.h"

constexpr int cx_ContextPoolSize = 4;
constexpr int cx_AvailableContextPoolSize = cx_ContextPoolSize;

class FCommandContext;

/*
	Manages command context lifetime and creation.
*/
class RENDER_API FContextManager
{
	friend class FRenderContext;
public:
	virtual FCommandContext* AllocateContext(ECommandListType Type) = 0;
	virtual void FreeContext(FCommandContext* pContext) = 0;
	virtual void DestroyAllContexts() = 0;

protected:
	FContextManager();
	virtual ~FContextManager();

	std::vector<FCommandContext*> m_ContextPool[cx_ContextPoolSize];
	std::queue<FCommandContext*> m_AvailableContexts[cx_AvailableContextPoolSize];
	CriticalSection m_ContextAllocationMutex;
};

/*
	Command reciever that executes graphics commands. Draw, texture binds, etc.
*/
class RENDER_API FCommandContext
{
	friend FContextManager;
public:
	virtual void* GetNativeContext() = 0;
	virtual void** GetNativeContextAddress() = 0;

	virtual void Initialize() = 0;
	virtual void UnInitialize() = 0;
	virtual void Reset() = 0;

	static FCommandContext& Begin(const TChar* ID);

	virtual void BeginDebugMarker(const TChar* Message) = 0;
	virtual void EndDebugMarker() = 0;
	virtual void ClearState(FPipelineState* pNewPipelineState) = 0;

	virtual uint64 Flush(bool WaitForCompletion = false) = 0;
	virtual uint64 End(bool WaitForCompletion = false) = 0;

	virtual void OMSetRenderTargets(uint32 NumRTVs, const FColorBuffer* Targets[], FDepthBuffer* pDepthBuffer) = 0;

	virtual void RSSetViewPorts(uint32 NumViewPorts, const FViewPort* ViewPorts) = 0;
	virtual void RSSetScissorRects(uint32 NumScissorRects, const FRect* ScissorRects) = 0;

	virtual void ClearColorBuffer(FColorBuffer& Buffer, const FRect& FRect) = 0;
	virtual void ClearDepth(FDepthBuffer& DepthBuffer) = 0;

	virtual void CreateTexture2D() = 0;
	virtual void CreateBuffer() = 0;

	virtual void SetDescriptorHeap(EResourceHeapType Type, FDescriptorHeap* HeapPtr) = 0;

	virtual void UpdateSubresources(FGpuResource& Destination, FGpuResource& Intermediate, uint32 IntermediateOffset, uint32 FirstSubresource, uint32 NumSubresources, FSubResourceData& SubresourceData) = 0;

	virtual void SetDepthBufferAsTexture(uint32 RootParameterIndex, const FDepthBuffer* pDepthBuffer) = 0;
	virtual void SetColorBuffersAsTextures(uint32 RootParameterIndex, uint32 Offset, uint32 Count, const FColorBuffer* Buffers[]) = 0;
	virtual void SetColorBufferAsTexture(uint32 RootParameterIndex, uint32 Offset, FColorBuffer* Buffer) = 0;
	virtual void BindVertexBuffer(uint32 Slot, FVertexBuffer& Vertexbuffer) = 0;
	virtual void BindIndexBuffer(FIndexBuffer& IndexBuffer) = 0;
	virtual void SetGraphicsConstantBuffer(uint32 RootParameterIndex, FConstantBuffer* pConstantBuffer) = 0;
	virtual void SetTexture(uint32 Slot, HTextureRef& pTexture) = 0;

	virtual void SetPipelineState(FPipelineState& Pipeline) = 0;
	virtual void SetGraphicsRootSignature(FRootSignature& Signature) = 0;
	virtual void SetPrimitiveTopologyType(EPrimitiveTopology TopologyType) = 0;

	virtual void Draw(uint32 VertexCount, uint32 VertexStartOffset) = 0;
	virtual void DrawIndexed(uint32 IndexCount, uint32 StartIndexLocation, int32 BaseVertexLocation) = 0;
	virtual void DrawInstanced(uint32 VertexCountPerInstance, uint32 InstanceCount, uint32 StartVertexLocation, uint32 StartInstanceLocation) = 0;
	virtual void DrawIndexedInstanced(uint32 IndexCountPerInstance, uint32 InstanceCount, UINT StartIndexLocation, uint32 BaseVertexLocation, uint32 StartInstanceLocation) = 0;

	virtual void TransitionResource(FGpuResource& Resource, EResourceState NewState, bool FlushImmediate = false) = 0;

	virtual void FlushResourceBarriers() = 0;

	void SetID(const TChar* NewID) { m_ID = NewID; }

protected:
	FCommandContext(const ECommandListType& Type)
		: m_ID()
		, m_Type(Type)

	{
	}
	virtual ~FCommandContext()
	{
	}

	virtual void BindDescriptorHeaps() = 0;

	const TChar* m_ID;
	ECommandListType m_Type;
};
