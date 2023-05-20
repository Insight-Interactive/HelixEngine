#include "RendererPCH.h"
#if R_WITH_D3D12

#include "RenderDevice.h"

#include "RendererCore.h"
#include "PipelineState.h"
#include "RootSignature.h"
#include "ColorBuffer.h"
#include "DescriptorHeap.h"
#include "DepthBuffer.h"
#include "Texture.h"


FRenderDevice::FRenderDevice()
	: m_pD3DDevice(NULL)
{
}

FRenderDevice::~FRenderDevice()
{
	UnInitialize();
}

void FRenderDevice::Initialize(const DeviceInitParams& InitParams, DeviceQueryResult& OutDeviceQueryResult, void** ppFactoryContext)
{
	IDXGIFactory6** ppDXGIFactory = RCast<IDXGIFactory6**>(ppFactoryContext);
	HE_ASSERT( ppDXGIFactory != NULL );

	// Create the Device
	//
	if (InitParams.ForceWarpAdapter)
	{
		// Force the device to be a software adapter. NOT recommended for games.
		IDXGIAdapter* pWarpAdapter = NULL;
		HRESULT hr = (*ppDXGIFactory)->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter));
		ThrowIfFailedMsg(hr, "Failed to enumerate warp adapter!");

		hr = D3D12CreateDevice(
			pWarpAdapter,
			InitParams.MinFeatureLevel,
			IID_PPV_ARGS(&m_pD3DDevice)
		);
		ThrowIfFailedMsg(hr, "Failed to create D3D12 warp device!");
		
		pWarpAdapter->Release();
	}
	else
	{
		IDXGIAdapter1* pHardwareAdapter;
		GetHardwareAdapter((*ppDXGIFactory), &pHardwareAdapter, InitParams, OutDeviceQueryResult );

		HRESULT hr = D3D12CreateDevice(
			pHardwareAdapter,
			InitParams.MinFeatureLevel,
			IID_PPV_ARGS(&m_pD3DDevice)
		);
		ThrowIfFailedMsg(hr, "Failed to create D3D12 device!");
	}
}

void FRenderDevice::CopyDescriptors(uint32 NumDestDescriptorRanges, const FCpuDescriptorHandle* pDestDescriptorRangeStarts, const uint32* pDestDescriptorRangeSizes, uint32 NumSrcDescriptorRanges, const FTexture** pSrcDescriptorRangeStarts, const uint32* pSrcDescriptorRangeSizes, EResourceHeapType DescriptorHeapsType)
{
	constexpr uint32 kMaxHandles = 12;
	D3D12_CPU_DESCRIPTOR_HANDLE SourceStarts[kMaxHandles];

	HE_ASSERT(NumSrcDescriptorRanges <= kMaxHandles);

	D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle{ pDestDescriptorRangeStarts->Ptr };
	for (uint32 i = 0; i < NumSrcDescriptorRanges; i++)
	{
		const FTexture* Tex = DCast<const FTexture*>(pSrcDescriptorRangeStarts[i]);
		SourceStarts[i] = Tex->GetSRV();
	}
	m_pD3DDevice->CopyDescriptors(NumDestDescriptorRanges, &CpuHandle, (const UINT*)pDestDescriptorRangeSizes, NumSrcDescriptorRanges, SourceStarts, (const UINT*)pSrcDescriptorRangeSizes, (D3D12_DESCRIPTOR_HEAP_TYPE)DescriptorHeapsType);
}

void FRenderDevice::GetHardwareAdapter(IDXGIFactory6* pFactory, IDXGIAdapter1** ppAdapter, const DeviceInitParams& InitParams, DeviceQueryResult& OutDeviceQueryResult)
{
	Microsoft::WRL::ComPtr<IDXGIAdapter1> pAdapter;
	*ppAdapter = NULL;
	uint32 CurrentMemory = 0;
	DXGI_ADAPTER_DESC1 Desc;

	for (uint32 AdapterIndex = 0;
		DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapterByGpuPreference(AdapterIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&pAdapter));
		++AdapterIndex)
	{
		ZeroMemory(&Desc, sizeof(DXGI_ADAPTER_DESC1));
		pAdapter->GetDesc1(&Desc);

		// Make sure we get the video card that is not a software adapter
		// and it has the most video memory.
		//
		if (Desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE || Desc.DedicatedVideoMemory < CurrentMemory) continue;

#if R_WITH_DXR
		// Check if we can support ray tracing with the device.
		if (InitParams.CheckForDXRSupport)
		{
			// Pass in a temp device to poll feature support later.
			Microsoft::WRL::ComPtr<ID3D12Device5> TempDevice;
			if (SUCCEEDED(D3D12CreateDevice(pAdapter.Get(), InitParams.MinDXRFeatureLevel, __uuidof(ID3D12Device5), &TempDevice)))
			{
				// If the device supports the feature level check to see if it can support DXR.
				//
				if (CheckDXRSupport(TempDevice.Get()))
				{
					CurrentMemory = SCast<uint32>(Desc.DedicatedVideoMemory);
					if (*ppAdapter != NULL)
						(*ppAdapter)->Release();

					*ppAdapter = pAdapter.Detach();
					OutDeviceQueryResult.IsRealtimeRTSupported = true;

					R_LOG(Log, TEXT("Found suitable D3D 12 hardware that can support DXR: %s"), Desc.Description);
					continue;
				}
			}
		}
#endif // R_WITH_DXR

		// If we cannot support ray tracing, see if D3D12 is supported on the adapter.
		//
		if (SUCCEEDED(D3D12CreateDevice(pAdapter.Get(), InitParams.MinFeatureLevel, __uuidof(ID3D12Device), NULL)))
		{
			CurrentMemory = static_cast<UINT>(Desc.DedicatedVideoMemory);
			if (*ppAdapter != NULL) (*ppAdapter)->Release();

			*ppAdapter = pAdapter.Detach();
			R_LOG(Log, TEXT("Found suitable D3D 12 hardware: %s"), Desc.Description);
		}
	}
	HE_ASSERT( *ppAdapter != NULL ); // Could not locate a proper adapter that supports D3D12.

	ZeroMemory(&Desc, sizeof(DXGI_ADAPTER_DESC1));
	(*ppAdapter)->GetDesc1(&Desc);
	
	::wcscpy_s(OutDeviceQueryResult.DeviceName, Desc.Description);
	R_LOG(Log, TEXT("\"%s\" selected as D3D 12 graphics hardware."), OutDeviceQueryResult.DeviceName);
}

void FRenderDevice::UnInitialize()
{
	HE_COM_SAFE_RELEASE(m_pD3DDevice);
}



// 
// Utility Functions
// 

bool FRenderDevice::CheckSM6Support(ID3D12Device* pDevice)
{
	HE_ASSERT(pDevice != NULL); // Cannot check for device feature support with a null device.
	D3D12_FEATURE_DATA_SHADER_MODEL sm6_0{ D3D_SHADER_MODEL_6_0 };
	HRESULT hr = pDevice->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &sm6_0, sizeof(sm6_0));
	ThrowIfFailedMsg(hr, "Failed to query feature support for shader model 6 with device.");
	return (sm6_0.HighestShaderModel >= D3D_SHADER_MODEL_6_0);
}

bool FRenderDevice::CheckDXRSupport(ID3D12Device* pDevice)
{
	HE_ASSERT(pDevice != NULL); // Cannot check for device feature support with a null device.
	D3D12_FEATURE_DATA_D3D12_OPTIONS5 Options5 = {};
	HRESULT hr = pDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &Options5, sizeof(Options5));
	ThrowIfFailedMsg(hr, "Failed to query feature support for ray trace with device.");
	return (Options5.RaytracingTier > D3D12_RAYTRACING_TIER_1_0);
}

#endif // R_WITH_D3D12
