#include "RendererPCH.h"
#if R_WITH_D3D12

#include "SwapChain.h"

#include "RendererCore.h"
#include "CommandManager.h"
#include "ColorBuffer.h"
#include "APIBridge/CommonFunctions.h"


FSwapChain::FSwapChain()
	: m_FrameIndex( 0 )
	, m_ClearColor( 0.f, 0.f, 0.f )
	, m_bFullScreenEnabled( false )
	, m_bVSyncEnabled( false )
	, m_bAllowTearing( false )
	, m_pDeviceRef( NULL )
	, m_DisplayPlanes()
	, m_pID3D12DeviceRef(NULL)
	, m_pDXGISwapChain(NULL)
{
}

FSwapChain::~FSwapChain()
{
	m_pDeviceRef = NULL;
	UnInitialize();
}

void FSwapChain::Initialize(FRenderDevice& Device)
{
	m_pDeviceRef = &Device;
}

void FSwapChain::Create(const FSwapChainDesc& InitParams, IDXGIFactory6** ppDXGIFactory, FCommandQueue* ppCommandQueue, ID3D12Device* pDevice)
{
	HE_ASSERT(ppCommandQueue != NULL);
	HE_ASSERT(ppDXGIFactory != NULL);
	HE_ASSERT(ppDXGIFactory != NULL);
	HE_ASSERT(pDevice != NULL);

	ID3D12CommandQueue* pCommandQueue = (ID3D12CommandQueue*)ppCommandQueue->GetNativeQueue();
	HE_ASSERT(pCommandQueue != NULL);

	m_pID3D12DeviceRef = pDevice;
	m_Desc = InitParams;
	m_DisplayPlanes.reserve( InitParams.BufferCount );
	for (uint32 i = 0; i < InitParams.BufferCount; ++i)
		m_DisplayPlanes.emplace_back();


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

void FSwapChain::UnInitialize()
{
	// Swapchain can be destroyed manually or via the destructor. Either way, the 
	// destructor will eventually get here. Make sure there is something to destory before doing it.
	if (m_pDXGISwapChain) 
	{
		HRESULT hr = m_pDXGISwapChain->SetFullscreenState(FALSE, NULL);
		ThrowIfFailedMsg(hr, "Failed to bring the swapchain out of fullscreen mode!");

		HE_COM_SAFE_RELEASE(m_pDXGISwapChain);
	}
}

void FSwapChain::CheckTearingSupport(IDXGIFactory6* pFactory)
{
	HE_ASSERT(pFactory != NULL);

	BOOL AllowTearing = 0;
	HRESULT hr = pFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &AllowTearing, sizeof(AllowTearing));
	SetIsTearingSupported(SUCCEEDED(hr) && AllowTearing);
}

void FSwapChain::SwapBuffers()
{
	uint32 PresetFlags = (GetIsTearingSupported() && m_bFullScreenEnabled)
		? DXGI_PRESENT_ALLOW_TEARING : 0;
	HRESULT hr = m_pDXGISwapChain->Present(SCast<uint32>(m_bVSyncEnabled), PresetFlags);
	ThrowIfFailedMsg( hr, "Failed to present image from DXGI swapchain!" );

	MoveToNextFrame();
}

void FSwapChain::Resize(const uint32& Width, const uint32& Height)
{
	if (m_pDXGISwapChain == NULL)
		return;

	// Flush the GPU and wait for all frames to finish rendering 
	// before destroying the swapchain buffers.
	GCommandManager.IdleGpu();

	m_Desc.Width = Width;
	m_Desc.Height = Height;
	for (uint64 i = 0; i < m_DisplayPlanes.size(); ++i)
	{
		m_DisplayPlanes[i].Destroy();
	}
	ResizeDXGIBuffers();
	BindSwapChainBackBuffers();
	SetCurrentFrameIndex( m_pDXGISwapChain->GetCurrentBackBufferIndex() );

	GCommandManager.IdleGpu();
}

void FSwapChain::SetNumBackBuffes(uint32 NumBuffers)
{
	HE_ASSERT(NumBuffers <= DXGI_MAX_SWAP_CHAIN_BUFFERS);

	m_Desc.BufferCount = NumBuffers;
	ResizeDXGIBuffers();
}

void FSwapChain::SetBackBufferFormat(EFormat& Format)
{
	m_Desc.Format = Format;
	ResizeDXGIBuffers();
}

void FSwapChain::ToggleFullScreen(bool IsEnabled)
{
	SetIsFullScreenEnabled( IsEnabled );

	if (!GetIsTearingSupported())
	{
		HRESULT hr = S_OK;
		BOOL FullScreenState;
		hr = m_pDXGISwapChain->GetFullscreenState(&FullScreenState, NULL);
		ThrowIfFailedMsg( hr, "Failed to get full screen state for swap chain!" );

		if (IsEnabled && FullScreenState)
		{
			R_LOG(Warning, TEXT("Full screen state is already active."));
			return;
		}

		hr = m_pDXGISwapChain->SetFullscreenState(!FullScreenState, NULL);
		ThrowIfFailedMsg(hr, "Failed to set fullscreen state for swap chain!");
	}
}

void FSwapChain::ResizeDXGIBuffers()
{
	if (m_pDXGISwapChain == NULL)
		return;

	DXGI_FORMAT Format = (DXGI_FORMAT)m_Desc.Format;
	DXGI_SWAP_CHAIN_DESC1 DXGIDesc = { 0 };
	m_pDXGISwapChain->GetDesc1(&DXGIDesc);

	HRESULT hr = m_pDXGISwapChain->ResizeBuffers(m_Desc.BufferCount, m_Desc.Width, m_Desc.Height, Format, DXGIDesc.Flags);
	ThrowIfFailedMsg(hr, "Failed to resize DXGI swap chain.");

	m_pDXGISwapChain->GetDesc1(&DXGIDesc);
	SetIsTearingSupported(DXGIDesc.Flags & DXGI_PRESENT_ALLOW_TEARING);
}

void FSwapChain::BindSwapChainBackBuffers()
{
	if (m_pDXGISwapChain == NULL)
		return;

	for (uint32 i = 0; i < m_Desc.BufferCount; i++)
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> DisplayPlane;
		HRESULT hr = m_pDXGISwapChain->GetBuffer(i, IID_PPV_ARGS(&DisplayPlane));
		ThrowIfFailedMsg(hr, "Failed to bind buffer with DXGI swapchain back buffer!");

		m_DisplayPlanes[i].CreateFromSwapChain(TEXT("SwapChain display plane"), DisplayPlane.Detach());
	}
}

#endif // R_WITH_D3D12
