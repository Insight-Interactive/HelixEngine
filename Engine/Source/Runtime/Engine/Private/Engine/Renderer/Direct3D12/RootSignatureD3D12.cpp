#include "EnginePCH.h"
#if R_WITH_D3D12

#include "Engine/Renderer/RootSignature.h"


static std::map<size_t, ::Microsoft::WRL::ComPtr<ID3D12RootSignature>> s_RootSignatureHashMap;

FRootSignature::FRootSignature( uint32 NumRootParams, uint32 NumStaticSamplers)
    : m_Finalized( false )
    , m_NumParameters( NumRootParams )
    , m_pID3D12RootSignature(NULL)
{
    Reset( NumRootParams, NumStaticSamplers );
}

FRootSignature::~FRootSignature()
{
}


void FRootSignature::DestroyAll(void)
{
    s_RootSignatureHashMap.clear();
}

void FRootSignature::Initialize(const FRootSignatureDesc& Desc)
{
    ID3D12Device* pID3D12Device = RCast<ID3D12Device*>(GGraphicsDevice.GetNativeDevice());

    CD3DX12_ROOT_SIGNATURE_DESC RSDesc;
    RSDesc.Init(Desc.NumParams, RCast<const D3D12_ROOT_PARAMETER*>(Desc.pParameters), Desc.NumStaticSamplers, RCast<const D3D12_STATIC_SAMPLER_DESC*>(Desc.pStaticSamplers), (D3D12_ROOT_SIGNATURE_FLAGS)Desc.Flags);

    ID3DBlob* pSignature = NULL;
    ID3DBlob* pErrorBuffer = NULL;
    HRESULT hr = D3D12SerializeRootSignature(&RSDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pSignature, &pErrorBuffer);
    ThrowIfFailedMsg(hr, "Failed to deserialize root signature!");
    hr = pID3D12Device->CreateRootSignature(0, pSignature->GetBufferPointer(), pSignature->GetBufferSize(), IID_PPV_ARGS(&m_pID3D12RootSignature));
    ThrowIfFailedMsg(hr, "Failed to create root signature!");

    HE_COM_SAFE_RELEASE(pErrorBuffer);
    HE_COM_SAFE_RELEASE(pSignature);
}

void FRootSignature::Finalize(const WChar* name, ERootSignatureFlags Flags)
{
    if (m_Finalized)
        return;

    HE_ASSERT(m_NumInitializedStaticSamplers == m_NumSamplers);

    D3D12_ROOT_SIGNATURE_DESC RootDesc;
    RootDesc.NumParameters = m_NumParameters;
    RootDesc.pParameters = (const D3D12_ROOT_PARAMETER*)m_ParamArray.get();
    RootDesc.NumStaticSamplers = m_NumSamplers;
    RootDesc.pStaticSamplers = (const D3D12_STATIC_SAMPLER_DESC*)m_SamplerArray.get();
    RootDesc.Flags = (D3D12_ROOT_SIGNATURE_FLAGS)Flags;

    m_DescriptorTableBitMap = 0;
    m_SamplerTableBitMap = 0;

    size_t HashCode = HashState(&RootDesc.Flags);
    HashCode = HashState(RootDesc.pStaticSamplers, m_NumSamplers, HashCode);

    for (UINT Param = 0; Param < m_NumParameters; ++Param)
    {
        const D3D12_ROOT_PARAMETER& RootParam = RootDesc.pParameters[Param];
        m_DescriptorTableSize[Param] = 0;

        if (RootParam.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
        {
            HE_ASSERT(RootParam.DescriptorTable.pDescriptorRanges != nullptr);

            HashCode = HashState(RootParam.DescriptorTable.pDescriptorRanges,
                RootParam.DescriptorTable.NumDescriptorRanges, HashCode);

            // We keep track of sampler descriptor tables separately from CBV_SRV_UAV descriptor tables
            if (RootParam.DescriptorTable.pDescriptorRanges->RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER)
                m_SamplerTableBitMap |= (1 << Param);
            else
                m_DescriptorTableBitMap |= (1 << Param);

            for (UINT TableRange = 0; TableRange < RootParam.DescriptorTable.NumDescriptorRanges; ++TableRange)
                m_DescriptorTableSize[Param] += RootParam.DescriptorTable.pDescriptorRanges[TableRange].NumDescriptors;
        }
        else
            HashCode = HashState(&RootParam, 1, HashCode);
    }

    ID3D12RootSignature** RSRef = nullptr;
    bool firstCompile = false;
    {
        static CriticalSection s_HashMapMutex;
        s_HashMapMutex.Enter();
        auto iter = s_RootSignatureHashMap.find(HashCode);

        // Reserve space so the next inquiry will find that someone got here first.
        if (iter == s_RootSignatureHashMap.end())
        {
            RSRef = s_RootSignatureHashMap[HashCode].GetAddressOf();
            firstCompile = true;
        }
        else
            RSRef = iter->second.GetAddressOf();

        s_HashMapMutex.Exit();
    }

    if (firstCompile)
    {
        ID3D12Device* pD3D12Device = RCast<ID3D12Device*>(GGraphicsDevice.GetNativeDevice());
        ::Microsoft::WRL::ComPtr<ID3DBlob> pOutBlob, pErrorBlob;

        HRESULT hr = D3D12SerializeRootSignature(&RootDesc, D3D_ROOT_SIGNATURE_VERSION_1, pOutBlob.GetAddressOf(), pErrorBlob.GetAddressOf());
        ThrowIfFailedMsg(hr, "Failed to serialize D3D12 root signature!");
        if (pErrorBlob != NULL)
        {
            Char* Error = (Char*)pErrorBlob->GetBufferPointer();
            R_LOG(Error, TEXT("Error while compiling RootSignature: %s"), (Char*)pErrorBlob->GetBufferPointer())
        }
        ResetHr(hr);

        hr = pD3D12Device->CreateRootSignature(1, pOutBlob->GetBufferPointer(), pOutBlob->GetBufferSize(), IID_PPV_ARGS(&m_pID3D12RootSignature));
        ThrowIfFailedMsg(hr, "Failed to create root signature!");
        
#if R_DEBUG_GPU_RESOURCES
        m_pID3D12RootSignature->SetName(name);
#endif
        s_RootSignatureHashMap[HashCode].Attach(m_pID3D12RootSignature);
        HE_ASSERT(*RSRef == m_pID3D12RootSignature);
    }
    else
    {
        while (*RSRef == nullptr)
            std::this_thread::yield();

        m_pID3D12RootSignature = *RSRef;
    }

    m_Finalized = TRUE;
}

#endif // R_WITH_D3D12
