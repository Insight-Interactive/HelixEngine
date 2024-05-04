// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "RendererPCH.h"
#if R_WITH_D3D12

#include "SwapChain.h"

#include "ColorBuffer.h"
#include "RendererCore.h"
#include "CommandManager.h"
#include "APIBridge/CommonFunctions.h"


/*static*/ EFormat FSwapChain::SSwapchainFormats[kSwapchainBitDepthCount]
{
	F_R8G8B8A8_UNorm,
	F_R10G10B10A2_UNorm,
	F_R16G16B16A16_Float
};

FSwapChain::FSwapChain()
	: m_FrameIndex( 0 )
	, m_ClearColor( 0.f, 0.f, 0.f )
	, m_bFullScreenEnabled( false )
	, m_bVSyncEnabled( false )
	, m_bAllowTearing( false )
	, m_IsHDRSupported( false )
	, m_pDeviceRef( nullptr )
	, m_DisplayBuffers()
	, m_pID3D12DeviceRef( nullptr )
	, m_pDXGIFactoryRef( nullptr )
	, m_pDXGISwapChain( nullptr )
{
}

FSwapChain::~FSwapChain()
{
	m_pDeviceRef = nullptr;
	UnInitialize();
}

void FSwapChain::Initialize( FRenderDevice& Device )
{
	m_pDeviceRef = &Device;
}

void FSwapChain::Create( const FSwapChainDesc& InitParams, IDXGIFactory6** ppDXGIFactory, FCommandQueue& ppCommandQueue, ID3D12Device& Device )
{
	HE_ASSERT( ppDXGIFactory != nullptr );

	ID3D12CommandQueue* pCommandQueue = (ID3D12CommandQueue*)ppCommandQueue.GetNativeQueue();
	HE_ASSERT( pCommandQueue != nullptr );

	m_pID3D12DeviceRef	= &Device;
	m_pDXGIFactoryRef	= *ppDXGIFactory;
	m_Desc				= InitParams;
	

	CheckTearingSupport( *ppDXGIFactory );

	DXGI_SAMPLE_DESC SampleDesc = { };
	SampleDesc.Count = 1;
	SampleDesc.Quality = 0;

	DXGI_SWAP_CHAIN_DESC1 Desc = { };
	Desc.Width			= InitParams.Width;
	Desc.Height			= InitParams.Height;
	Desc.BufferCount	= HE_MAX_SWAPCHAIN_BACK_BUFFERS;
	Desc.SampleDesc		= SampleDesc;
	Desc.SwapEffect		= DXGI_SWAP_EFFECT_FLIP_DISCARD;
	Desc.BufferUsage	= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	Desc.Format			= (DXGI_FORMAT)SSwapchainFormats[m_Desc.BackBufferColorDepth];
	Desc.Flags			= GetIsTearingSupported() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
	APIBridge::CreateSwapChain( InitParams.NativeWindow, &Desc, GetIsTearingSupported(), ppDXGIFactory, pCommandQueue, &m_pDXGISwapChain );

	// Check display HDR support and initialize ST.2084 support to match the display's support.
	CheckDisplayHDRSupport();

	SetCurrentFrameIndex( m_pDXGISwapChain->GetCurrentBackBufferIndex() );
	BindSwapChainBackBuffers();
}

void FSwapChain::UnInitialize()
{
	// Swapchain can be destroyed manually or via the destructor. Either way, the 
	// destructor will eventually get here. Make sure there is something to destory before doing it.
	if (m_pDXGISwapChain)
	{
		if (!GetIsTearingSupported())
		{
			HRESULT hr = m_pDXGISwapChain->SetFullscreenState( FALSE, NULL );
			ThrowIfFailedMsg( hr, "Failed to bring the swapchain out of fullscreen mode!" );
		}

		HE_COM_SAFE_RELEASE( m_pDXGISwapChain );
	}
}

void FSwapChain::CheckTearingSupport( IDXGIFactory6* pFactory )
{
	HE_ASSERT( pFactory != NULL );

	BOOL AllowTearing = FALSE;
	HRESULT hr = pFactory->CheckFeatureSupport( DXGI_FEATURE_PRESENT_ALLOW_TEARING, &AllowTearing, sizeof( AllowTearing ) );
	SetIsTearingSupported( SUCCEEDED( hr ) && AllowTearing );
}

void FSwapChain::SwapBuffers()
{
	const UINT PresetFlags = (GetIsTearingSupported() && m_bFullScreenEnabled) ? DXGI_PRESENT_ALLOW_TEARING : 0;
	HRESULT hr = m_pDXGISwapChain->Present( (UINT)m_bVSyncEnabled, PresetFlags );
	ThrowIfFailedMsg( hr, "Failed to present image from DXGI swapchain!" );

	MoveToNextFrame();
}

void FSwapChain::Resize( const uint32& Width, const uint32& Height )
{
	if (m_pDXGISwapChain == NULL)
		return;

	// Flush the GPU and wait for all frames to finish rendering 
	// before destroying the swapchain buffers.
	GCommandManager.IdleGpu();

	m_Desc.Width = Width;
	m_Desc.Height = Height;
	for (uint64 i = 0; i < HE_MAX_SWAPCHAIN_BACK_BUFFERS; ++i)
	{
		m_DisplayBuffers[i].Destroy();
	}
	ResizeDXGIBuffers();
	BindSwapChainBackBuffers();
	SetCurrentFrameIndex( m_pDXGISwapChain->GetCurrentBackBufferIndex() );

	GCommandManager.IdleGpu();
}

void FSwapChain::SetBackBufferFormat( const ESwapchainBitDepth& ColorDepth )
{
	m_Desc.BackBufferColorDepth = ColorDepth;
	ResizeDXGIBuffers();
}

void FSwapChain::ToggleFullScreen( bool IsEnabled )
{
	SetIsFullScreenEnabled( IsEnabled );

	if (!GetIsTearingSupported())
	{
		HRESULT hr = S_OK;
		BOOL FullScreenState;
		hr = m_pDXGISwapChain->GetFullscreenState( &FullScreenState, NULL );
		ThrowIfFailedMsg( hr, "Failed to get full screen state for swap chain!" );

		if (IsEnabled && FullScreenState)
		{
			R_LOG( Warning, TEXT( "Full screen state is already active." ) );
			return;
		}

		hr = m_pDXGISwapChain->SetFullscreenState( !FullScreenState, NULL );
		ThrowIfFailedMsg( hr, "Failed to set fullscreen state for swap chain!" );
	}
}

void FSwapChain::ResizeDXGIBuffers()
{
	if (m_pDXGISwapChain == nullptr)
		return;

	DXGI_FORMAT Format = (DXGI_FORMAT)SSwapchainFormats[m_Desc.BackBufferColorDepth];
	DXGI_SWAP_CHAIN_DESC1 DXGIDesc = { 0 };
	m_pDXGISwapChain->GetDesc1( &DXGIDesc );

	HRESULT hr = m_pDXGISwapChain->ResizeBuffers( HE_MAX_SWAPCHAIN_BACK_BUFFERS, m_Desc.Width, m_Desc.Height, Format, DXGIDesc.Flags );
	ThrowIfFailedMsg( hr, "Failed to resize DXGI swap chain." );

	m_pDXGISwapChain->GetDesc1( &DXGIDesc );
	SetIsTearingSupported( DXGIDesc.Flags & DXGI_PRESENT_ALLOW_TEARING );

	m_FrameIndex = m_pDXGISwapChain->GetCurrentBackBufferIndex();
}

// Compute the overlay area of two rectangles, A and B.
// (ax1, ay1) = left-top coordinates of A; (ax2, ay2) = right-bottom coordinates of A
// (bx1, by1) = left-top coordinates of B; (bx2, by2) = right-bottom coordinates of B
inline int ComputeIntersectionArea( int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2 )
{
	return max( 0, min( ax2, bx2 ) - max( ax1, bx1 ) ) * max( 0, min( ay2, by2 ) - max( ay1, by1 ) );
}

void FSwapChain::CheckDisplayHDRSupport()
{
	if (!m_pDXGIFactoryRef->IsCurrent())
	{
		HRESULT hr = CreateDXGIFactory2( 0, IID_PPV_ARGS( &m_pDXGIFactoryRef ) );
		ThrowIfFailedMsg( hr, "Failed to create DXGI factory when checking for HDR display support in swapchain!" );
	}

	// First, the method must determine the app's current display. 
	// We don't recommend using IDXGISwapChain::GetContainingOutput method to do that because of two reasons:
	//    1. Swap chains created with CreateSwapChainForComposition do not support this method.
	//    2. Swap chains will return a stale dxgi output once DXGIFactory::IsCurrent() is false. In addition, 
	//       we don't recommend re-creating swapchain to resolve the stale dxgi output because it will cause a short 
	//       period of black screen.
	// Instead, we suggest enumerating through the bounds of all dxgi outputs and determine which one has the greatest 
	// intersection with the app window bounds. Then, use the DXGI output found in previous step to determine if the 
	// app is on a HDR capable display. 

	// Retrieve the current default adapter.
	Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter;
	ThrowIfFailedMsg( m_pDXGIFactoryRef->EnumAdapters1( 0, &dxgiAdapter ), "Failed to get default adapter when checking for HDR display support!");

	// Iterate through the DXGI outputs associated with the DXGI adapter,
	// and find the output whose bounds have the greatest overlap with the
	// app window (i.e. the output for which the intersection area is the
	// greatest).

	UINT i = 0;
	Microsoft::WRL::ComPtr<IDXGIOutput> currentOutput;
	Microsoft::WRL::ComPtr<IDXGIOutput> bestOutput;
	float bestIntersectArea = -1.f;
	RECT WindowBounds = {};
	GetWindowRect( *(HWND*)m_Desc.NativeWindow, &WindowBounds );

	while (dxgiAdapter->EnumOutputs( i, &currentOutput ) != DXGI_ERROR_NOT_FOUND)
	{
		// Get the retangle bounds of the app window
		int ax1 = WindowBounds.left;
		int ay1 = WindowBounds.top;
		int ax2 = WindowBounds.right;
		int ay2 = WindowBounds.bottom;

		// Get the rectangle bounds of current output
		DXGI_OUTPUT_DESC desc;
		ThrowIfFailedMsg( currentOutput->GetDesc( &desc ), "Failed to get current output while determining HDR display support!");
		RECT r = desc.DesktopCoordinates;
		int bx1 = r.left;
		int by1 = r.top;
		int bx2 = r.right;
		int by2 = r.bottom;

		// Compute the intersection
		int intersectArea = ComputeIntersectionArea( ax1, ay1, ax2, ay2, bx1, by1, bx2, by2 );
		if (intersectArea > bestIntersectArea)
		{
			bestOutput = currentOutput;
			bestIntersectArea = static_cast<float>(intersectArea);
		}

		i++;
	}

	// Having determined the output (display) upon which the app is primarily being 
	// rendered, retrieve the HDR capabilities of that display by checking the color space.
	Microsoft::WRL::ComPtr<IDXGIOutput6> output6;
	ThrowIfFailedMsg( bestOutput.As( &output6 ), "Failed to get best display outpus as IDXGIOutput6!");

	DXGI_OUTPUT_DESC1 desc1;
	ThrowIfFailedMsg( output6->GetDesc1( &desc1 ), "Failed to get description for best display output!");

	m_IsHDRSupported = (desc1.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020);
}

void FSwapChain::BindSwapChainBackBuffers()
{
	if (m_pDXGISwapChain == nullptr)
		return;

	for (uint32 i = 0; i < HE_MAX_SWAPCHAIN_BACK_BUFFERS; i++)
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> DisplayPlane;
		HRESULT hr = m_pDXGISwapChain->GetBuffer( i, IID_PPV_ARGS( &DisplayPlane ) );
		ThrowIfFailedMsg( hr, "Failed to bind buffer with DXGI swapchain back buffer!" );

		m_DisplayBuffers[i].CreateFromSwapChain( TEXT( "SwapChain display buffer" ), DisplayPlane.Detach() );
	}

	m_FrameIndex = m_pDXGISwapChain->GetCurrentBackBufferIndex();
}

#endif // R_WITH_D3D12
