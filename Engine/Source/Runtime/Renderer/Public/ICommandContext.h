#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "CommonEnums.h"
#include "CriticalSection.h"
#include "RendererCore.h"

constexpr int cx_ContextPoolSize = 4;
constexpr int cx_AvailableContextPoolSize = cx_ContextPoolSize;

class ICommandContext;

/*
	Manages command context lifetime and creation.
*/
class RENDER_API IContextManager
{
	friend class RenderContext;
public:
	virtual ICommandContext* AllocateContext(ECommandListType Type) = 0;
	virtual void FreeContext(ICommandContext* pContext) = 0;
	virtual void DestroyAllContexts() = 0;

protected:
	IContextManager();
	virtual ~IContextManager();

	std::vector<ICommandContext*> m_ContextPool[cx_ContextPoolSize];
	std::queue<ICommandContext*> m_AvailableContexts[cx_AvailableContextPoolSize];
	CriticalSection m_ContextAllocationMutex;
};

/*
	Command reciever that executes graphics commands. Draw, texture binds, etc.
*/
class RENDER_API ICommandContext
{
	friend class D3D12CommandManager;
	friend IContextManager;
public:
	virtual void* GetNativeContext() = 0;
	virtual void** GetNativeContextAddress() = 0;

	virtual void Initialize() = 0;
	virtual void UnInitialize() = 0;
	virtual void Reset() = 0;

	static ICommandContext& Begin(const TChar* ID);

	virtual void BeginDebugMarker(const TChar* Message) = 0;
	virtual void EndDebugMarker() = 0;
	virtual void ClearState(IPipelineState* pNewPipelineState) = 0;

	virtual uint64 Flush(bool WaitForCompletion = false) = 0;
	virtual uint64 End(bool WaitForCompletion = false) = 0;

	virtual void OMSetRenderTargets(uint32 NumRTVs, const IColorBuffer* Targets[], IDepthBuffer* pDepthBuffer) = 0;

	virtual void RSSetViewPorts(uint32 NumViewPorts, const ViewPort* ViewPorts) = 0;
	virtual void RSSetScissorRects(uint32 NumScissorRects, const Rect* ScissorRects) = 0;

	virtual void ClearColorBuffer(IColorBuffer& Buffer, const Rect& Rect) = 0;
	virtual void ClearDepth(IDepthBuffer& DepthBuffer) = 0;

	virtual void CreateTexture2D() = 0;
	virtual void CreateBuffer() = 0;

	virtual void SetDescriptorHeap(EResourceHeapType Type, IDescriptorHeap* HeapPtr) = 0;

	virtual void UpdateSubresources(IGpuResource& Destination, IGpuResource& Intermediate, uint32 IntermediateOffset, uint32 FirstSubresource, uint32 NumSubresources, SubResourceData& SubresourceData) = 0;

	virtual void SetDepthBufferAsTexture(uint32 RootParameterIndex, const IDepthBuffer* pDepthBuffer) = 0;
	virtual void SetColorBuffersAsTextures(uint32 RootParameterIndex, uint32 Offset, uint32 Count, const IColorBuffer* Buffers[]) = 0;
	virtual void SetColorBufferAsTexture(uint32 RootParameterIndex, uint32 Offset, IColorBuffer* Buffer) = 0;
	virtual void BindVertexBuffer(uint32 Slot, IVertexBuffer& Vertexbuffer) = 0;
	virtual void BindIndexBuffer(IIndexBuffer& IndexBuffer) = 0;
	virtual void SetGraphicsConstantBuffer(uint32 RootParameterIndex, IConstantBuffer* pConstantBuffer) = 0;
	virtual void SetTexture(uint32 Slot, TextureRef& pTexture) = 0;

	virtual void SetPipelineState(IPipelineState& Pipeline) = 0;
	virtual void SetGraphicsRootSignature(IRootSignature& Signature) = 0;
	virtual void SetPrimitiveTopologyType(EPrimitiveTopology TopologyType) = 0;

	virtual void Draw(uint32 VertexCount, uint32 VertexStartOffset) = 0;
	virtual void DrawIndexed(uint32 IndexCount, uint32 StartIndexLocation, int32 BaseVertexLocation) = 0;
	virtual void DrawInstanced(uint32 VertexCountPerInstance, uint32 InstanceCount, uint32 StartVertexLocation, uint32 StartInstanceLocation) = 0;
	virtual void DrawIndexedInstanced(uint32 IndexCountPerInstance, uint32 InstanceCount, UINT StartIndexLocation, uint32 BaseVertexLocation, uint32 StartInstanceLocation) = 0;

	virtual void TransitionResource(IGpuResource& Resource, EResourceState NewState, bool FlushImmediate = false) = 0;

	virtual void FlushResourceBarriers() = 0;

	void SetID(const TChar* NewID) { m_ID = NewID; }

protected:
	ICommandContext(const ECommandListType& Type)
		: m_ID()
		, m_Type(Type)

	{
	}
	virtual ~ICommandContext()
	{
	}

	virtual void BindDescriptorHeaps() = 0;

	const TChar* m_ID;
	ECommandListType m_Type;
};
