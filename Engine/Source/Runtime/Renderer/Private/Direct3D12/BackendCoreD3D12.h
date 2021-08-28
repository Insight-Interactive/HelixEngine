#pragma once

#include "RendererCore.h"
#include "DescriptorAllocatorD3D12.h"

extern DescriptorAllocatorD3D12 GDescriptorAllocator[];
inline D3D12_CPU_DESCRIPTOR_HANDLE AllocateDescriptor(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE Type, uint32 Count = 1)
{
	return GDescriptorAllocator[Type].Allocate(pDevice, Count);
}
