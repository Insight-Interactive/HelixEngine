#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "ICommandContext.h"
#include "DynamicDescriptorHeapD3D12.h"
#include "LinearAllocator.h"



class RENDER_API ContextManagerD3D12 final : public IContextManager
{
	friend class IRenderContextFactory;
	friend class RenderContextFactoryD3D12;
public:
	ContextManagerD3D12();
	virtual ~ContextManagerD3D12();

	virtual ICommandContext* AllocateContext(ECommandListType Type) override;
	virtual void FreeContext(ICommandContext* pContext) override;
	virtual void DestroyAllContexts() override;
};



class RENDER_API CommandContextD3D12 : public ICommandContext
{
	friend ContextManagerD3D12;
public:
	virtual void* GetNativeContext() override { return RCast<void*>(m_pID3D12CommandList); }
	virtual void** GetNativeContextAddress() override { return RCast<void**>(&m_pID3D12CommandList); }
	virtual void  Initialize() override;
	virtual void  UnInitialize() override;
	virtual void  Reset() override;

	virtual void BeginDebugMarker(const TChar* Message) override;
	virtual void EndDebugMarker() override;
	virtual void ClearState(IPipelineState* pNewPipelineState) override;

	virtual void OMSetRenderTargets(uint32 NumRTVs, const IColorBuffer* Targets[], IDepthBuffer* pDepthBuffer) override;
	virtual void ClearDepth(IDepthBuffer& DepthBuffer) override;

	virtual void RSSetViewPorts(uint32 NumViewPorts, const ViewPort* ViewPorts) override;
	virtual void RSSetScissorRects(uint32 NumScissorRects, const Rect* ScissorRects) override;

	virtual void ClearColorBuffer(IColorBuffer& Buffer, const Rect& Rect) override;

	virtual void CreateTexture2D() override;
	virtual void CreateBuffer() override;

	virtual void SetDescriptorHeap(EResourceHeapType Type, IDescriptorHeap* HeapPtr) override;

	DynAlloc ReserveUploadMemory(uint64 SizeInBytes);
	virtual void UpdateSubresources(IGpuResource& Destination, IGpuResource& Intermediate, uint32 IntermediateOffset, uint32 FirstSubresource, uint32 NumSubresources, SubResourceData& SubresourceData) override;

	virtual void SetDepthBufferAsTexture(uint32 RootParameterIndex, const IDepthBuffer* pDepthBuffer) override;
	virtual void SetColorBuffersAsTextures(uint32 RootParameterIndex, uint32 Offset, uint32 Count, const IColorBuffer* Buffers[]) override;
	virtual void SetColorBufferAsTexture(uint32 RootParameterIndex, uint32 Offset, IColorBuffer* Buffer) override;
	virtual void BindVertexBuffer(uint32 Slot, IVertexBuffer& Vertexbuffer) override;
	virtual void BindIndexBuffer(IIndexBuffer& IndexBuffer) override;
	virtual void SetGraphicsConstantBuffer(uint32 RootParameterIndex, IConstantBuffer* pConstantBuffer) override;
	virtual void SetTexture(uint32 RootParameterIndex, TextureRef& pTexture) override;

	virtual void SetPipelineState(IPipelineState& Pipeline) override;
	virtual void SetGraphicsRootSignature(IRootSignature& Signature) override;
	virtual void SetPrimitiveTopologyType(EPrimitiveTopology TopologyType) override;

	virtual void Draw(uint32 VertexCount, uint32 VertexStartOffset = 0) override;
	virtual void DrawIndexed(uint32 IndexCount, uint32 StartIndexLocation = 0, int32 BaseVertexLocation = 0) override;
	virtual void DrawInstanced(uint32 VertexCountPerInstance, uint32 InstanceCount, uint32 StartVertexLocation = 0, uint32 StartInstanceLocation = 0) override;
	virtual void DrawIndexedInstanced(uint32 IndexCountPerInstance, uint32 InstanceCount, UINT StartIndexLocation, uint32 BaseVertexLocation, uint32 StartInstanceLocation) override;


	virtual void TransitionResource(IGpuResource& Resource, EResourceState NewState, bool FlushImmediate = false) override;

	DynamicDescriptorHeapD3D12& GetDynamicHeap() { return m_DynamicViewDescriptorHeap; }

	virtual uint64 Flush(bool WaitForCompletion = false) override;
	virtual uint64 End(bool WaitForCompletion = false) override;

	virtual void FlushResourceBarriers() override;

private:
	CommandContextD3D12(const ECommandListType& Type);
	~CommandContextD3D12();

	virtual void BindDescriptorHeaps() override;

	D3D12_COMMAND_LIST_TYPE m_D3DCmdListType;
	uint32 m_NumBarriersToFlush;
	D3D12_RESOURCE_BARRIER m_ResourceBarrierBuffer[16];

	ID3D12GraphicsCommandList* m_pID3D12CommandList;
	ID3D12CommandAllocator* m_pID3D12CurrentCmdAllocator;

	ID3D12DescriptorHeap* m_CurrentDescriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

	DynamicDescriptorHeapD3D12 m_DynamicViewDescriptorHeap;		// HEAP_TYPE_CBV_SRV_UAV
	DynamicDescriptorHeapD3D12 m_DynamicSamplerDescriptorHeap;	// HEAP_TYPE_SAMPLER

	LinearAllocator m_CpuLinearAllocator;
	LinearAllocator m_GpuLinearAllocator;

};
