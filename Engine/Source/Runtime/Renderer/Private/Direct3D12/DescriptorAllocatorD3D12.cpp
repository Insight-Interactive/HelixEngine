#include "RendererPCH.h"
#if R_WITH_D3D12

#include "DescriptorAllocator.h"


CriticalSection FDescriptorAllocator::SAllocationMutex;
std::vector<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>> FDescriptorAllocator::SDescriptorHeapPool;

void FDescriptorAllocator::DestroyAll(void)
{
    SDescriptorHeapPool.clear();
}

ID3D12DescriptorHeap* FDescriptorAllocator::RequestNewHeap(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE Type)
{
    SAllocationMutex.Enter();

    HE_ASSERT(pDevice != NULL);

    D3D12_DESCRIPTOR_HEAP_DESC Desc;
    Desc.Type = Type;
    Desc.NumDescriptors = SNumDescriptorsPerHeap;
    Desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    Desc.NodeMask = 1;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> pHeap;
    HRESULT hr = pDevice->CreateDescriptorHeap(&Desc, IID_PPV_ARGS(&pHeap));
    ThrowIfFailedMsg(hr, "Failed ot create descriptor heap!");
    SDescriptorHeapPool.emplace_back(pHeap);

    SAllocationMutex.Exit();
    return pHeap.Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE FDescriptorAllocator::Allocate(ID3D12Device* pDevice, uint32 Count)
{
    HE_ASSERT(pDevice != NULL);

    if (m_CurrentHeap == NULL || m_RemainingFreeHandles < Count)
    {
        m_CurrentHeap = RequestNewHeap(pDevice, (D3D12_DESCRIPTOR_HEAP_TYPE)m_Type);
        m_CurrentHandle = m_CurrentHeap->GetCPUDescriptorHandleForHeapStart();
        m_RemainingFreeHandles = SNumDescriptorsPerHeap;

        if (m_DescriptorSize == 0)
            m_DescriptorSize = pDevice->GetDescriptorHandleIncrementSize( (D3D12_DESCRIPTOR_HEAP_TYPE)m_Type);
    }

    D3D12_CPU_DESCRIPTOR_HANDLE ret = m_CurrentHandle;
    m_CurrentHandle.ptr += Count * m_DescriptorSize;
    m_RemainingFreeHandles -= Count;

    return ret;
}

#endif // R_WITH_D3D12
