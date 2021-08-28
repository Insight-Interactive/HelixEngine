#include "RendererPCH.h"

#include "DescriptorAllocatorD3D12.h"
#include "../D3DCommon/D3DCommon.h"

CriticalSection DescriptorAllocatorD3D12::sm_AllocationMutex;
std::vector<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>> DescriptorAllocatorD3D12::sm_DescriptorHeapPool;

void DescriptorAllocatorD3D12::DestroyAll(void)
{
    sm_DescriptorHeapPool.clear();
}

ID3D12DescriptorHeap* DescriptorAllocatorD3D12::RequestNewHeap(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE Type)
{
    sm_AllocationMutex.Enter();

    HE_ASSERT(pDevice != NULL);

    D3D12_DESCRIPTOR_HEAP_DESC Desc;
    Desc.Type = Type;
    Desc.NumDescriptors = sm_NumDescriptorsPerHeap;
    Desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    Desc.NodeMask = 1;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> pHeap;
    HRESULT hr = pDevice->CreateDescriptorHeap(&Desc, IID_PPV_ARGS(&pHeap));
    ThrowIfFailedMsg(hr, TEXT("Failed ot create descriptor heap!"));
    sm_DescriptorHeapPool.emplace_back(pHeap);

    sm_AllocationMutex.Exit();
    return pHeap.Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorAllocatorD3D12::Allocate(ID3D12Device* pDevice, uint32 Count)
{
    HE_ASSERT(pDevice != NULL);

    if (m_CurrentHeap == NULL || m_RemainingFreeHandles < Count)
    {
        m_CurrentHeap = RequestNewHeap(pDevice, m_Type);
        m_CurrentHandle = m_CurrentHeap->GetCPUDescriptorHandleForHeapStart();
        m_RemainingFreeHandles = sm_NumDescriptorsPerHeap;

        if (m_DescriptorSize == 0)
            m_DescriptorSize = pDevice->GetDescriptorHandleIncrementSize(m_Type);
    }

    D3D12_CPU_DESCRIPTOR_HANDLE ret = m_CurrentHandle;
    m_CurrentHandle.ptr += Count * m_DescriptorSize;
    m_RemainingFreeHandles -= Count;

    return ret;
}
