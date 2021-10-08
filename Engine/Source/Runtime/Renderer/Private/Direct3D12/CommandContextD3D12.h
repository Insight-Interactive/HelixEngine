#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "ICommandContext.h"
#include "DynamicDescriptorHeapD3D12.h"
#include "LinearAllocator.h"



class RENDER_API FContextManagerD3D12 final : public FContextManager
{
	friend class FRenderContextFactory;
	friend class FRenderContextFactoryD3D12;
public:
	FContextManagerD3D12();
	virtual ~FContextManagerD3D12();

	virtual FCommandContext* AllocateContext(ECommandListType Type) override;
	virtual void FreeContext(FCommandContext* pContext) override;
	virtual void DestroyAllContexts() override;
};



class RENDER_API FCommandContextD3D12 : public FCommandContext
{
	friend FContextManagerD3D12;
public:
	virtual void* GetNativeContext() override { return RCast<void*>(m_pID3D12CommandList); }
	virtual void** GetNativeContextAddress() override { return RCast<void**>(&m_pID3D12CommandList); }
	virtual void  Initialize() override;
	virtual void  UnInitialize() override;
	virtual void  Reset() override;

	virtual void BeginDebugMarker(const TChar* Message) override;
	virtual void EndDebugMarker() override;
	virtual void ClearState(FPipelineState* pNewPipelineState) override;

	virtual void OMSetRenderTargets(uint32 NumRTVs, const FColorBuffer* Targets[], FDepthBuffer* pDepthBuffer) override;
	virtual void ClearDepth(FDepthBuffer& DepthBuffer) override;

	virtual void RSSetViewPorts(uint32 NumViewPorts, const FViewPort* ViewPorts) override;
	virtual void RSSetScissorRects(uint32 NumScissorRects, const FRect* ScissorRects) override;

	virtual void ClearColorBuffer(FColorBuffer& Buffer, const FRect& FRect) override;

	virtual void CreateTexture2D() override;
	virtual void CreateBuffer() override;

	virtual void SetDescriptorHeap(EResourceHeapType Type, FDescriptorHeap* HeapPtr) override;

	DynAlloc ReserveUploadMemory(uint64 SizeInBytes);
	virtual void UpdateSubresources(FGpuResource& Destination, FGpuResource& Intermediate, uint32 IntermediateOffset, uint32 FirstSubresource, uint32 NumSubresources, FSubResourceData& SubresourceData) override;

	virtual void SetDepthBufferAsTexture(uint32 RootParameterIndex, const FDepthBuffer* pDepthBuffer) override;
	virtual void SetColorBuffersAsTextures(uint32 RootParameterIndex, uint32 Offset, uint32 Count, const FColorBuffer* Buffers[]) override;
	virtual void SetColorBufferAsTexture(uint32 RootParameterIndex, uint32 Offset, FColorBuffer* Buffer) override;
	virtual void BindVertexBuffer(uint32 Slot, FVertexBuffer& Vertexbuffer) override;
	virtual void BindIndexBuffer(FIndexBuffer& IndexBuffer) override;
	virtual void SetGraphicsConstantBuffer(uint32 RootParameterIndex, FConstantBuffer* pConstantBuffer) override;
	virtual void SetTexture(uint32 RootParameterIndex, HTextureRef& pTexture) override;

	virtual void SetPipelineState(FPipelineState& Pipeline) override;
	virtual void SetGraphicsRootSignature(FRootSignature& Signature) override;
	virtual void SetPrimitiveTopologyType(EPrimitiveTopology TopologyType) override;

	virtual void Draw(uint32 VertexCount, uint32 VertexStartOffset = 0) override;
	virtual void DrawIndexed(uint32 IndexCount, uint32 StartIndexLocation = 0, int32 BaseVertexLocation = 0) override;
	virtual void DrawInstanced(uint32 VertexCountPerInstance, uint32 InstanceCount, uint32 StartVertexLocation = 0, uint32 StartInstanceLocation = 0) override;
	virtual void DrawIndexedInstanced(uint32 IndexCountPerInstance, uint32 InstanceCount, UINT StartIndexLocation, uint32 BaseVertexLocation, uint32 StartInstanceLocation) override;


	virtual void TransitionResource(FGpuResource& Resource, EResourceState NewState, bool FlushImmediate = false) override;

	FDynamicDescriptorHeapD3D12& GetDynamicHeap() { return m_DynamicViewDescriptorHeap; }

	virtual uint64 Flush(bool WaitForCompletion = false) override;
	virtual uint64 End(bool WaitForCompletion = false) override;

	virtual void FlushResourceBarriers() override;

private:
	FCommandContextD3D12(const ECommandListType& Type);
	~FCommandContextD3D12();

	virtual void BindDescriptorHeaps() override;

	D3D12_COMMAND_LIST_TYPE m_D3DCmdListType;
	uint32 m_NumBarriersToFlush;
	D3D12_RESOURCE_BARRIER m_ResourceBarrierBuffer[16];

	ID3D12GraphicsCommandList* m_pID3D12CommandList;
	ID3D12CommandAllocator* m_pID3D12CurrentCmdAllocator;

	ID3D12DescriptorHeap* m_CurrentDescriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

	FDynamicDescriptorHeapD3D12 m_DynamicViewDescriptorHeap;		// HEAP_TYPE_CBV_SRV_UAV
	FDynamicDescriptorHeapD3D12 m_DynamicSamplerDescriptorHeap;	// HEAP_TYPE_SAMPLER

	FLinearAllocator m_CpuLinearAllocator;
	FLinearAllocator m_GpuLinearAllocator;

};
