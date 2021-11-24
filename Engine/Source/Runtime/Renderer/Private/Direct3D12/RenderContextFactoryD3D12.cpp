// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "RendererPCH.h"
#if R_WITH_D3D12

#include "RenderContextFactory.h"

#include "RenderDevice.h"
#include "CommandManager.h"
#include "CommandContext.h"
#include "Swapchain.h"
#include "GeometryBufferManager.h"
#include "ConstantBuffer.h"
#include "TextureManager.h"


FRenderContextFactory::FRenderContextFactory()
{
	CreateDXGIFactory();
}

FRenderContextFactory::~FRenderContextFactory()
{
	HE_COM_SAFE_RELEASE(m_pDXGIFactory);
}

void FRenderContextFactory::CreateContext(FRenderContext& OutContext)
{
	HE_ASSERT( m_pDXGIFactory != NULL); // Cannot create D3D12 context with null dxgi factory.

	R_LOG(Log, TEXT("Initializing render context with Direct3D 12 backend."));

	// Initialize all main components for the renderer.
	InitializeMainComponents();

	// Initialize heaps and other resources.
	GTextureHeap.Create( L"Scene Texture Descriptors", RHT_CBV_SRV_UAV, 4096 );

	CreateGeometryManager(GGeometryManager);
	CreateTextureManager(GTextureManager);

	R_LOG(Log, TEXT("Render backend initialization complete."));
}

void FRenderContextFactory::CreateDevice(FRenderDevice& OutDevice)
{
	DeviceInitParams DeviceInitParams;
	ZeroMemory(&DeviceInitParams, sizeof(DeviceInitParams));
	DeviceInitParams.CheckForDXRSupport = true;
	DeviceInitParams.ForceWarpAdapter	= false;
	DeviceInitParams.MinFeatureLevel	= D3D_FEATURE_LEVEL_11_0;
	DeviceInitParams.MinDXRFeatureLevel = D3D_FEATURE_LEVEL_12_1;
	DeviceQueryResult DeviceQueryResult;
	ZeroMemory(&DeviceQueryResult, sizeof(DeviceQueryResult));
	OutDevice.Initialize(DeviceInitParams, DeviceQueryResult, RCast<void**>(&m_pDXGIFactory ));
}

void FRenderContextFactory::CreateDXGIFactory()
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

	HRESULT hr = CreateDXGIFactory2(DXGIFactoryFlags, IID_PPV_ARGS(&m_pDXGIFactory ));
	ThrowIfFailedMsg(hr, "Failed to create DXGI factory!");
}

void FRenderContextFactory::CreateSwapChain(FSwapChain& OutSwapChain, void* pNativeSurface, uint32 RenderSurfaceWidth, uint32 RenderSurfaceHeight, FCommandManager& InCommandManager, FRenderDevice& InDevice)
{
	ID3D12Device* pID3D12Device = (ID3D12Device*)InDevice.GetNativeDevice();

	FCommandQueue* pD3D12CommandQueue = InCommandManager.GetGraphicsQueue();
	FSwapChainDesc SwapChainInitParams;
	ZeroMemory(&SwapChainInitParams, sizeof(FSwapChainDesc));
	SwapChainInitParams.Width = RenderSurfaceWidth;
	SwapChainInitParams.Height = RenderSurfaceHeight;
	SwapChainInitParams.BufferCount = HE_MAX_SWAPCHAIN_BACK_BUFFERS;
	SwapChainInitParams.Format = F_R8G8B8A8_UNorm; // TODO: Check for HDR display support
	SwapChainInitParams.SampleDesc.Count = 1;
	SwapChainInitParams.SampleDesc.Quality = 0;
	SwapChainInitParams.NativeWindow = pNativeSurface;
	OutSwapChain.Initialize(InDevice);
	OutSwapChain.Create(SwapChainInitParams, &m_pDXGIFactory, pD3D12CommandQueue, pID3D12Device);
}

void FRenderContextFactory::CreateCommandManager(FCommandManager& OutCommandManager)
{
	OutCommandManager.Initialize(GGraphicsDevice);
}

void FRenderContextFactory::CreateContextManager(FContextManager& OutCommandContext)
{
	//
}

void FRenderContextFactory::CreateGeometryManager(FGeometryBufferManager& OutGeometryManager)
{
	//
}

void FRenderContextFactory::CreateTextureManager(FTextureManager& OutTexManager)
{
	OutTexManager.Initialize();
}
#endif // R_WITH_D3D12