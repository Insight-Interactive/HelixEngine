// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "RendererPCH.h"

#include "RenderContextFactoryD3D12.h"


#include "DeviceD3D12.h"
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


RenderContextFactoryD3D12::RenderContextFactoryD3D12()
{
	CreateDXGIFactory();
}

RenderContextFactoryD3D12::~RenderContextFactoryD3D12()
{
	HE_COM_SAFE_RELEASE(DXGIFactory);
}

void RenderContextFactoryD3D12::CreateContext(RenderContext& OutContext)
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

void RenderContextFactoryD3D12::CreateDevice(IDevice** OutDevice)
{
	DeviceD3D12* pD3D12Device = CreateRenderComponentObject<DeviceD3D12>(OutDevice);
	
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

void RenderContextFactoryD3D12::CreateDXGIFactory()
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

void RenderContextFactoryD3D12::CreateSwapChain(ISwapChain** OutSwapChain, void* pNativeSurface, uint32 RenderSurfaceWidth, uint32 RenderSurfaceHeight, ICommandManager* InCommandManager, IDevice* InDevice)
{
	SwapChainD3D12* pD3D12SwapChain = CreateRenderComponentObject<SwapChainD3D12>(OutSwapChain);

	CommandManagerD3D12* pCommandManager = DCast<CommandManagerD3D12*>(InCommandManager);
	HE_ASSERT(pCommandManager != NULL); // Trying to create swap chain with invalid command manager.

	HE_ASSERT(InDevice != NULL);
	ID3D12Device* pID3D12Device = RCast<ID3D12Device*>(InDevice->GetNativeDevice());

	CommandQueueD3D12* pD3D12CommandQueue = RCast<CommandQueueD3D12*>(pCommandManager->GetGraphicsQueue());
	SwapChainDescription SwapChainInitParams;
	ZeroMemory(&SwapChainInitParams, sizeof(SwapChainDescription));
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

void RenderContextFactoryD3D12::CreateCommandManager(ICommandManager** OutCommandManager, IDevice* InDevice)
{
	HE_ASSERT(InDevice != NULL); // Trying to create command manager with null device.
	
	CommandManagerD3D12* pCommandManagerD3D12 = CreateRenderComponentObject<CommandManagerD3D12>(OutCommandManager);
	
	pCommandManagerD3D12->Initialize(InDevice);
}

void RenderContextFactoryD3D12::CreateContextManager(IContextManager** OutCommandContext)
{
	CreateRenderComponentObject<ContextManagerD3D12>(OutCommandContext);
}

void RenderContextFactoryD3D12::CreateGeometryManager(IGeometryBufferManager** OutGeometryManager)
{
	CreateRenderComponentObject<GeometryBufferManagerD3D12>(OutGeometryManager);
}

void RenderContextFactoryD3D12::CreateConstantBufferManager(IConstantBufferManager** OutCBManager)
{
	ConstantBufferManagerD3D12* pD3D12CBManager = CreateRenderComponentObject<ConstantBufferManagerD3D12>(OutCBManager);
	
	pD3D12CBManager->Initialize();
}

void RenderContextFactoryD3D12::CreateTextureManager(ITextureManager** OutTexManager)
{
	TextureManagerD3D12* pD3D12TexManager = CreateRenderComponentObject<TextureManagerD3D12>(OutTexManager);

	pD3D12TexManager->Initialize();
}
