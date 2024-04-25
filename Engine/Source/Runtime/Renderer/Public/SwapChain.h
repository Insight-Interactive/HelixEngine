// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "Color.h"
#include "ColorBuffer.h"
#include "CommonEnums.h"
#include "RendererCore.h"
#include "CommonStructs.h"
#include "CommandManager.h"


class FColorBuffer;
class FRenderDevice;

class RENDER_API FSwapChain
{
	friend class FWindow;
	friend class FRenderContext;
	friend class FRenderContextFactory;
public:
	FSwapChain();
	virtual ~FSwapChain();

	void Initialize( FRenderDevice& Device );

	void SwapBuffers();
	void ToggleFullScreen( bool IsEnabled );
	void Resize( const uint32& Width, const uint32& Height );
	void ToggleVsync( bool IsEnabled );

	//
	// Getters
	//
	void* GetNativeSwapChain() const;
	FSwapChainDesc GetDesc() const;
	uint32 GetCurrentFrameIndex() const;
	FColor GetClearColor() const;
	EFormat GetBackBufferFormat() const;
	bool GetIsVSyncEnabled() const;
	bool GetIsTearingSupported() const;
	uint32 GetNumBackBuffers() const;
	bool GetIsFullScreenEnabled() const;
	FColorBuffer& GetColorBufferForCurrentFrame();
	FColorBuffer& GetColorBufferForFrame( const uint32& Index);

	//
	// Setters
	//
	void SetCurrentFrameIndex( uint32 Index );
	void SetClearColor( const FColor& NewColor );
	void SetIsVSyncEnabled( bool bEnabled );
	void SetIsTearingSupported( bool bSupported );
	void SetBackBufferFormat( const ESwapchainBitDepth& ColorDepth );

protected:
	void UnInitialize();
	void MoveToNextFrame();
	void SetIsFullScreenEnabled( bool bEnabled );
	void CheckDisplayHDRSupport();

protected:
	FSwapChainDesc m_Desc;
	uint32 m_FrameIndex;
	FColor m_ClearColor;
	bool m_bFullScreenEnabled;
	bool m_bVSyncEnabled;
	bool m_bAllowTearing;
	bool m_IsHDRSupported;

	FRenderDevice* m_pDeviceRef;
	FColorBuffer m_DisplayBuffers[HE_MAX_SWAPCHAIN_BACK_BUFFERS];

protected:
#if R_WITH_D3D12
	// Utility
	void ResizeDXGIBuffers();

	// D3D Initialize
	void BindSwapChainBackBuffers();
	void CheckTearingSupport( IDXGIFactory6* pFactory );
	void Create( const FSwapChainDesc& InitParams, IDXGIFactory6** ppDXGIFactory, FCommandQueue& ppCommandQueue, ID3D12Device& Device );

	ID3D12Device* m_pID3D12DeviceRef;
	IDXGIFactory6* m_pDXGIFactoryRef;
	IDXGISwapChain3* m_pDXGISwapChain;

#endif
private: // static
	// HDR
	static EFormat SSwapchainFormats[kSwapchainBitDepthCount];

};


// 
// Inline function implementations
// 


FORCEINLINE FSwapChainDesc FSwapChain::GetDesc() const
{
	return m_Desc;
}

FORCEINLINE uint32 FSwapChain::GetCurrentFrameIndex() const
{
	return m_FrameIndex;
}

FORCEINLINE FColor FSwapChain::GetClearColor() const
{
	return m_ClearColor;
}

FORCEINLINE EFormat FSwapChain::GetBackBufferFormat() const
{
	return SSwapchainFormats[m_Desc.BackBufferColorDepth];
}

FORCEINLINE bool FSwapChain::GetIsVSyncEnabled() const
{
	return m_bVSyncEnabled;
}

FORCEINLINE bool FSwapChain::GetIsTearingSupported() const
{
	return m_bAllowTearing;
}

FORCEINLINE uint32 FSwapChain::GetNumBackBuffers() const
{
	return HE_MAX_SWAPCHAIN_BACK_BUFFERS;
}

FORCEINLINE bool FSwapChain::GetIsFullScreenEnabled() const
{
	return m_bFullScreenEnabled;
}

FORCEINLINE FColorBuffer& FSwapChain::GetColorBufferForCurrentFrame()
{
	return m_DisplayBuffers[GetCurrentFrameIndex()];
}

FORCEINLINE FColorBuffer& FSwapChain::GetColorBufferForFrame( const uint32& Index )
{
	HE_ASSERT( Index >= 0 && Index <= HE_MAX_SWAPCHAIN_BACK_BUFFERS );
	return m_DisplayBuffers[Index];
}

FORCEINLINE void* FSwapChain::GetNativeSwapChain() const
{
#if R_WITH_D3D12
	return SCast<void*>( m_pDXGISwapChain );
#endif
	return NULL;
}

FORCEINLINE void FSwapChain::ToggleVsync( bool IsEnabled ) 
{ 
	SetIsVSyncEnabled( IsEnabled ); 
}

FORCEINLINE void FSwapChain::SetCurrentFrameIndex( uint32 Index )
{
	m_FrameIndex = Index;
}

FORCEINLINE void FSwapChain::SetIsVSyncEnabled( bool bEnabled )
{
	if (m_bFullScreenEnabled)
		m_bVSyncEnabled = bEnabled;
	else
		R_LOG( Error, TEXT( "Trying to enable VSync while in windowed mode! This is not allowed." ) );
}

FORCEINLINE void FSwapChain::SetIsTearingSupported( bool bSupported )
{
	m_bAllowTearing = bSupported;
}

FORCEINLINE void FSwapChain::MoveToNextFrame()
{
	SetCurrentFrameIndex( (GetCurrentFrameIndex() + 1) % GetNumBackBuffers() );
}

FORCEINLINE void FSwapChain::SetClearColor( const FColor& NewColor )
{
	m_ClearColor = NewColor;
	for (uint32 i = 0; i < HE_MAX_SWAPCHAIN_BACK_BUFFERS; ++i)
		m_DisplayBuffers[i].SetClearColor( m_ClearColor );
}

FORCEINLINE void FSwapChain::SetIsFullScreenEnabled( bool bEnabled )
{
	m_bFullScreenEnabled = bEnabled;
}
