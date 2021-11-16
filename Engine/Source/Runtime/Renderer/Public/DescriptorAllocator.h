#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "CommonEnums.h"
#include "CriticalSection.h"


// This is an unbounded resource descriptor allocator.  It is intended to provide space for CPU-visible
// resource descriptors as resources are created.  For those that need to be made shader-visible, they
// will need to be copied to a DescriptorHeap or a DynamicDescriptorHeap.
class RENDER_API FDescriptorAllocator
{
public:
	FDescriptorAllocator(EResourceHeapType Type)
		: m_Type(Type)
		, m_CurrentHeap(NULL)
		, m_DescriptorSize(0)
	{
		m_CurrentHandle.ptr = HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
	}
	virtual ~FDescriptorAllocator() 
	{
	}

#if R_WITH_D3D12
	D3D12_CPU_DESCRIPTOR_HANDLE Allocate(ID3D12Device* pDevice, uint32 Count);
#endif

	static void DestroyAll(void);

protected:
#if R_WITH_D3D12
	static const uint32_t SNumDescriptorsPerHeap = 256;
	static CriticalSection SAllocationMutex;
	static std::vector<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>> SDescriptorHeapPool;
	static ID3D12DescriptorHeap* RequestNewHeap(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE Type);

	EResourceHeapType m_Type;
	ID3D12DescriptorHeap* m_CurrentHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_CurrentHandle;
	uint32_t m_DescriptorSize;
	uint32_t m_RemainingFreeHandles;

#endif // R_WITH_D3D12
};
