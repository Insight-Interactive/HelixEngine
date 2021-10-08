#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RendererCore.h"

// This handle refers to a descriptor or a descriptor table (contiguous descriptors) that is shader visible.
class FDescriptorHandle
{
public:
    FDescriptorHandle()
    {
        m_CpuHandle.Ptr = HE_INVALID_GPU_ADDRESS;
        m_GpuHandle.Ptr = HE_INVALID_GPU_ADDRESS;
    }

    /*
    // Should we allow constructing handles that might not be shader visible?
    FDescriptorHandle( D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle )
        : m_CpuHandle(CpuHandle)
    {
        m_GpuHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
    }
    */

    FDescriptorHandle(FCpuDescriptorHandle CpuHandle, FGpuDescriptorHandle GpuHandle)
        : m_CpuHandle(CpuHandle), m_GpuHandle(GpuHandle)
    {
    }

    FDescriptorHandle operator+ (int32 OffsetScaledByDescriptorSize) const
    {
        FDescriptorHandle ret = *this;
        ret += OffsetScaledByDescriptorSize;
        return ret;
    }

    void operator += (int32 OffsetScaledByDescriptorSize)
    {
        if (m_CpuHandle.Ptr != HE_INVALID_GPU_ADDRESS)
            m_CpuHandle.Ptr += OffsetScaledByDescriptorSize;
        if (m_GpuHandle.Ptr != HE_INVALID_GPU_ADDRESS)
            m_GpuHandle.Ptr += OffsetScaledByDescriptorSize;
    }

    const FCpuDescriptorHandle* operator&() const { return &m_CpuHandle; }
    operator FCpuDescriptorHandle() const { return m_CpuHandle; }
    operator FGpuDescriptorHandle() const { return m_GpuHandle; }

    size_t GetCpuPtr() const { return m_CpuHandle.Ptr; }
    uint64 GetGpuPtr() const { return m_GpuHandle.Ptr; }
    bool IsNull() const { return m_CpuHandle.Ptr == HE_INVALID_GPU_ADDRESS; }
    bool IsShaderVisible() const { return m_GpuHandle.Ptr != HE_INVALID_GPU_ADDRESS; }

private:
    FCpuDescriptorHandle m_CpuHandle;
    FGpuDescriptorHandle m_GpuHandle;
};


class RENDER_API FDescriptorHeap
{
public:
    virtual void* GetNativeHeap() = 0;

    virtual FDescriptorHandle Alloc(uint32 Count) = 0;

protected:
    FDescriptorHeap() {}
    virtual ~FDescriptorHeap() {}

    virtual void Create(const WChar* DebugHeapName, EResourceHeapType Type, uint32 MaxCount) = 0;

};
