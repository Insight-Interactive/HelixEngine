// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "RendererPCH.h"

#include "RenderContextFactoryD3D12.h"


#include "RenderDeviceD3D12.h"
#include "CommandManagerD3D12.h"
#include "CommandContextD3D12.h"
#include "SwapchainD3D12.h"
#include "GeometryBufferManagerD3D12.h"
#include "ConstantBufferManagerD3D12.h"
#include "TextureManagerD3D12.h"
#include "../D3DCommon/D3DCommon.h"



// DirectX
//
IDXGIFactory6* DXGIFactory;


FRenderContextFactoryD3D12::FRenderContextFactoryD3D12()
{
	CreateDXGIFactory();
}

FRenderContextFactoryD3D12::~FRenderContextFactoryD3D12()
{
	HE_COM_SAFE_RELEASE(DXGIFactory);
}

void FRenderContextFactoryD3D12::CreateContext(FRenderContext& OutContext)
{
	HE_ASSERT(DXGIFactory != NULL); // Cannot create D3D12 context with null dxgi factory.

	R_LOG(Log, TEXT("Initializing render context with Direct3D 12 backend."));

	OutContext.SetBackendType(RB_Direct3D12);

	// Initialize all main components for the renderer.
	Super::InitializeMainComponents();

	// Initialize heaps and other resources.
	HE_ASSERT(GDevice != NULL);
	GDevice->CreateDescriptorHeap(TEXT("Scene Texture Descriptors"), RHT_CBV_SRV_UAV, 4096, &GTextureHeap);

	CreateGeometryManager(&GGeometryManager);
	CreateConstantBufferManager(&GConstantBufferManager);
	CreateTextureManager(&GTextureManager);

	R_LOG(Log, TEXT("Render backend initialization complete."));
}

void FRenderContextFactoryD3D12::CreateDevice(FRenderDevice** OutDevice)
{
	FRenderDeviceD3D12* pD3D12Device = CreateRenderComponentObject<FRenderDeviceD3D12>(OutDevice);
	
	DeviceInitParamsD3D12 DeviceInitParams;
	ZeroMemory(&DeviceInitParams, sizeof(DeviceInitParamsD3D12));
	DeviceInitParams.CheckForDXRSupport = true;
	DeviceInitParams.ForceWarpAdapter	= false;
	DeviceInitParams.MinFeatureLevel	= D3D_FEATURE_LEVEL_11_0;
	DeviceInitParams.MinDXRFeatureLevel = D3D_FEATURE_LEVEL_12_1;
	DeviceQueryResultD3D12 DeviceQueryResult;
	ZeroMemory(&DeviceQueryResult, sizeof(DeviceQueryResultD3D12));
	pD3D12Device->Initialize(DeviceInitParams, DeviceQueryResult, RCast<void**>(&DXGIFactory));
}

void FRenderContextFactoryD3D12::CreateDXGIFactory()
{
	uint32 DXGIFactoryFlags = 0u;

#if HE_DEBUG
	// Enable the debug layer (requires the Graphics Tools "optional feature").
	// NOTE: Enabling the debug layer after device creation will invalidate the active device.
	{
		ID3D12Debug* DebugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&DebugController))))
		{
			DebugController->EnableDebugLayer();

			// Enable additional debug layers.
			DXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}

		DebugController->Release();
	}
#endif

	HRESULT hr = CreateDXGIFactory2(DXGIFactoryFlags, IID_PPV_ARGS(&DXGIFactory));
	ThrowIfFailedMsg(hr, TEXT("Failed to create DXGI factory!"));
}

void FRenderContextFactoryD3D12::CreateSwapChain(FSwapChain** OutSwapChain, void* pNativeSurface, uint32 RenderSurfaceWidth, uint32 RenderSurfaceHeight, FCommandManager* InCommandManager, FRenderDevice* InDevice)
{
	FSwapChainD3D12* pD3D12SwapChain = CreateRenderComponentObject<FSwapChainD3D12>(OutSwapChain);

	FCommandManagerD3D12* pCommandManager = DCast<FCommandManagerD3D12*>(InCommandManager);
	HE_ASSERT(pCommandManager != NULL); // Trying to create swap chain with invalid command manager.

	HE_ASSERT(InDevice != NULL);
	ID3D12Device* pID3D12Device = RCast<ID3D12Device*>(InDevice->GetNativeDevice());

	FCommandQueueD3D12* pD3D12CommandQueue = RCast<FCommandQueueD3D12*>(pCommandManager->GetGraphicsQueue());
	FSwapChainDesc SwapChainInitParams;
	ZeroMemory(&SwapChainInitParams, sizeof(FSwapChainDesc));
	SwapChainInitParams.Width = RenderSurfaceWidth;
	SwapChainInitParams.Height = RenderSurfaceHeight;
	SwapChainInitParams.BufferCount = 3;
	SwapChainInitParams.Format = F_R8G8B8A8_UNorm;
	SwapChainInitParams.SampleDesc.Count = 1;
	SwapChainInitParams.SampleDesc.Quality = 0;
	SwapChainInitParams.NativeWindow = pNativeSurface;
	pD3D12SwapChain->Initialize(InDevice);
	pD3D12SwapChain->Create(SwapChainInitParams, &DXGIFactory, pD3D12CommandQueue, pID3D12Device);
}

void FRenderContextFactoryD3D12::CreateCommandManager(FCommandManager** OutCommandManager, FRenderDevice* InDevice)
{
	HE_ASSERT(InDevice != NULL); // Trying to create command manager with null device.
	
	FCommandManagerD3D12* pCommandManagerD3D12 = CreateRenderComponentObject<FCommandManagerD3D12>(OutCommandManager);
	
	pCommandManagerD3D12->Initialize(InDevice);
}

void FRenderContextFactoryD3D12::CreateContextManager(FContextManager** OutCommandContext)
{
	CreateRenderComponentObject<FContextManagerD3D12>(OutCommandContext);
}

void FRenderContextFactoryD3D12::CreateGeometryManager(FGeometryBufferManager** OutGeometryManager)
{
	CreateRenderComponentObject<FGeometryBufferManagerD3D12>(OutGeometryManager);
}

void FRenderContextFactoryD3D12::CreateConstantBufferManager(FConstantBufferManager** OutCBManager)
{
	FConstantBufferManagerD3D12* pD3D12CBManager = CreateRenderComponentObject<FConstantBufferManagerD3D12>(OutCBManager);
	
	pD3D12CBManager->Initialize();
}

void FRenderContextFactoryD3D12::CreateTextureManager(FTextureManager** OutTexManager)
{
	FTextureManagerD3D12* pD3D12TexManager = CreateRenderComponentObject<FTextureManagerD3D12>(OutTexManager);

	pD3D12TexManager->Initialize();
}
