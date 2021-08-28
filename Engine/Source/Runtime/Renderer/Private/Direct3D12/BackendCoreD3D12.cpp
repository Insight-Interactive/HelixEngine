#include "RendererPCH.h"

#include "BackendCoreD3D12.h"

DescriptorAllocatorD3D12 GDescriptorAllocator[RHT_HeapType_Count]
{
	RHT_CBV_SRV_UAV,
	RHT_Sampler,
	RHT_RTV,
	RHT_DSV,
};