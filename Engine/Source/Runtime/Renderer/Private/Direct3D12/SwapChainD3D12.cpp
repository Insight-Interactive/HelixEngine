#include "RendererPCH.h"

#include "SwapchainD3D12.h"

#include "RendererCore.h"
#include "CommandManagerD3D12.h"
#include "ColorBufferD3D12.h"
#include "APIBridge/CommonFunctions.h"


SwapChainD3D12::SwapChainD3D12()
	: m_pID3D12DeviceRef(NULL)
	, m_pDXGISwapChain(NULL)
{
}

SwapChainD3D12::~SwapChainD3D12()
{
	UnInitialize();
}

void SwapChainD3D12::Initialize(IDevice* pDevice)
{
	m_pDeviceRef = pDevice;
}

void SwapChainD3D12::Create(const SwapChainDescription& InitParams, IDXGIFactory6** ppDXGIFactory, CommandQueueD3D12* ppCommandQueue, ID3D12Device* pDevice)
{
	HE_ASSERT(ppCommandQueue != NULL);
	HE_ASSERT(ppDXGIFactory != NULL);
	HE_ASSERT(ppDXGIFactory != NULL);
	HE_ASSERT(pDevice != NULL);

	ID3D12CommandQueue* pCommandQueue = RCast<ID3D12CommandQueue*>(ppCommandQueue->GetNativeQueue());
	HE_ASSERT(pCommandQueue != NULL);

	m_pID3D12DeviceRef = pDevice;
	m_Desc = InitParams;
	for (uint32 i = 0; i < InitParams.BufferCount; ++i)
		m_DisplayPlanes.push_back(new ColorBufferD3D12());


	CheckTearingSupport((*ppDXGIFactory));

	DXGI_SAMPLE_DESC SampleDesc;
	ZeroMemory(&SampleDesc, sizeof(DXGI_SAMPLE_DESC));
	SampleDesc.Count = 1;
	SampleDesc.Quality = 0;

	DXGI_SWAP_CHAIN_DESC1 Desc;
	ZeroMemory(&Desc, sizeof(DXGI_SWAP_CHAIN_DESC1));
	Desc.Width = InitParams.Width;
	Desc.Height = InitParams.Height;
	Desc.BufferCount = InitParams.BufferCount;
	Desc.SampleDesc = SampleDesc;
	Desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	Desc.Format = (DXGI_FORMAT)InitParams.Format;
	Desc.Flags = GetIsTearingSupported() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

	APIBridge::CreateSwapChain(InitParams.NativeWindow, &Desc, GetIsTearingSupported(), ppDXGIFactory, pCommandQueue, &m_pDXGISwapChain);

	SetCurrentFrameIndex(m_pDXGISwapChain->GetCurrentBackBufferIndex());

	BindSwapChainBackBuffers();
}

void SwapChainD3D12::UnInitialize()
{
	HRESULT hr = m_pDXGISwapChain->SetFullscreenState(FALSE, NULL);
	ThrowIfFailedMsg(hr, TEXT("Failed to bring the swapchain out of fullscreen mode!"));

	HE_COM_SAFE_RELEASE(m_pDXGISwapChain);
}

void SwapChainD3D12::CheckTearingSupport(IDXGIFactory6* pFactory)
{
	HE_ASSERT(pFactory != NULL);

	BOOL AllowTearing = 0;
	HRESULT hr = pFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &AllowTearing, sizeof(AllowTearing));
	SetIsTearingSupported(SUCCEEDED(hr) && AllowTearing);
}

void SwapChainD3D12::SwapBuffers()
{
	uint32 PresetFlags = (GetIsTearingSupported() && m_bFullScreenEnabled)
		? DXGI_PRESENT_ALLOW_TEARING : 0;
	m_pDXGISwapChain->Present(SCast<uint32>(m_bVSyncEnabled), PresetFlags);

	MoveToNextFrame();
}

void SwapChainD3D12::Resize(const uint32& Width, const uint32& Height)
{
	// Flush the GPU and wait for all frames to finish rendering 
	// before destroying the swapchain buffers.
	GCommandManager->IdleGPU();

	m_Desc.Width = Width;
	m_Desc.Height = Height;
	for (uint64 i = 0; i < m_DisplayPlanes.size(); ++i)
	{
		DCast<GpuResourceD3D12*>(m_DisplayPlanes[i])->Destroy();
	}
	ResizeDXGIBuffers();
	BindSwapChainBackBuffers();
	SetCurrentFrameIndex( m_pDXGISwapChain->GetCurrentBackBufferIndex() );

	GCommandManager->IdleGPU();
}

void SwapChainD3D12::SetNumBackBuffes(uint32 NumBuffers)
{
	HE_ASSERT(NumBuffers <= DXGI_MAX_SWAP_CHAIN_BUFFERS);

	m_Desc.BufferCount = NumBuffers;
	ResizeDXGIBuffers();
}

void SwapChainD3D12::SetBackBufferFormat(EFormat& Format)
{
	m_Desc.Format = Format;
	ResizeDXGIBuffers();
}

void SwapChainD3D12::ToggleFullScreen(bool IsEnabled)
{
	ISwapChain::ToggleFullScreen(IsEnabled);

	if (!GetIsTearingSupported())
	{
		HRESULT hr = S_OK;
		BOOL FullScreenState;
		hr = m_pDXGISwapChain->GetFullscreenState(&FullScreenState, NULL);
		ThrowIfFailedMsg(hr, TEXT("Failed to get full screen state for swap chain!"))

			if (IsEnabled && FullScreenState)
			{
				R_LOG(Warning, TEXT("Full screen state is already active."));
				return;
			}

		hr = m_pDXGISwapChain->SetFullscreenState(!FullScreenState, NULL);
		ThrowIfFailedMsg(hr, TEXT("Failed to set fullscreen state for swap chain!"));
	}
}

void SwapChainD3D12::ResizeDXGIBuffers()
{
	DXGI_FORMAT Format = (DXGI_FORMAT)m_Desc.Format;
	DXGI_SWAP_CHAIN_DESC1 DXGIDesc = { 0 };
	m_pDXGISwapChain->GetDesc1(&DXGIDesc);

	HRESULT hr = m_pDXGISwapChain->ResizeBuffers(m_Desc.BufferCount, m_Desc.Width, m_Desc.Height, Format, DXGIDesc.Flags);
	ThrowIfFailedMsg(hr, TEXT("Failed to resize DXGI swap chain."));

	m_pDXGISwapChain->GetDesc1(&DXGIDesc);
	SetIsTearingSupported(DXGIDesc.Flags & DXGI_PRESENT_ALLOW_TEARING);
}

void SwapChainD3D12::BindSwapChainBackBuffers()
{
	for (uint32 i = 0; i < m_Desc.BufferCount; i++)
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> DisplayPlane;
		HRESULT hr = m_pDXGISwapChain->GetBuffer(i, IID_PPV_ARGS(&DisplayPlane));
		ThrowIfFailedMsg(hr, TEXT("Failed to bind buffer with DXGI swapchain back buffer!"));

		HE_ASSERT(m_DisplayPlanes[i] != NULL);
		m_DisplayPlanes[i]->CreateFromSwapChain(m_pDeviceRef, TEXT("SwapChain display plane"), DisplayPlane.Detach());
	}
}