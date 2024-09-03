#include "EnginePCH.h"
#if R_WITH_D3D12

#include "Engine/Renderer/DescriptorHeap.h"


//
// DescriptorHeap implementation
//

void FDescriptorHeap::Create(const WChar* Name, EResourceHeapType Type, uint32 MaxCount)
{
    m_HeapDesc.Type = (D3D12_DESCRIPTOR_HEAP_TYPE)Type;
    m_HeapDesc.NumDescriptors = MaxCount;
    m_HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    m_HeapDesc.NodeMask = 1;

    ID3D12Device* pID3D12Device = RCast<ID3D12Device*>(GGraphicsDevice.GetNativeDevice());
    HE_ASSERT(pID3D12Device != NULL);

    pID3D12Device->CreateDescriptorHeap(&m_HeapDesc, IID_PPV_ARGS(m_Heap.ReleaseAndGetAddressOf()));

#if R_DEBUG_GPU_RESOURCES
    m_Heap->SetName(Name);
#else
    (void)Name;
#endif

    m_DescriptorSize = pID3D12Device->GetDescriptorHandleIncrementSize(m_HeapDesc.Type);
    m_NumFreeDescriptors = m_HeapDesc.NumDescriptors;
    FCpuDescriptorHandle CpuHandle{ m_Heap->GetCPUDescriptorHandleForHeapStart().ptr };
    FGpuDescriptorHandle GpuHandle{ m_Heap->GetGPUDescriptorHandleForHeapStart().ptr };
    m_FirstHandle = FDescriptorHandle(CpuHandle, GpuHandle);
    m_NextFreeHandle = m_FirstHandle;
}

FDescriptorHandle FDescriptorHeap::Alloc(uint32 Count)
{
    HE_ASSERT(HasAvailableSpace(Count)); // Descriptor Heap out of space. Increase heap size.
    FDescriptorHandle ret = m_NextFreeHandle;
    m_NextFreeHandle += Count * m_DescriptorSize;
    m_NumFreeDescriptors -= Count;
    return ret;
}

bool FDescriptorHeap::ValidateHandle(const FDescriptorHandle& DHandle) const
{
    if (DHandle.GetCpuPtr() < m_FirstHandle.GetCpuPtr() ||
        DHandle.GetCpuPtr() >= m_FirstHandle.GetCpuPtr() + m_HeapDesc.NumDescriptors * m_DescriptorSize)
        return false;

    if (DHandle.GetGpuPtr() - m_FirstHandle.GetGpuPtr() !=
        DHandle.GetCpuPtr() - m_FirstHandle.GetCpuPtr())
        return false;

    return true;
}

#endif // R_WITH_D3D12
