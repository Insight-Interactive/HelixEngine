#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "Color.h"
#include "CommonEnums.h"
#include "CommonStructs.h"
#include "ColorBuffer.h"
#include "CommandManager.h"


class FColorBuffer;
class FRenderDevice;


class RENDER_API FSwapChain
{
	friend class FRenderContextFactory;
	friend class FRenderContext;
	friend class FWindow;
public:
	FSwapChain();
	virtual ~FSwapChain();

	void Initialize(FRenderDevice& Device);

	void SwapBuffers();
	void Resize(const uint32& Width, const uint32& Height);
	void SetNumBackBuffes(uint32 NumBuffers);
	void SetBackBufferFormat(EFormat& Format);

	void* GetNativeSwapChain() const;

	void ToggleFullScreen( bool IsEnabled );
	void ToggleVsync(bool IsEnabled)		{ SetIsVSyncEnabled(IsEnabled); }

	FORCEINLINE void MoveToNextFrame();

	//
	// Getters
	//
	FSwapChainDesc GetDesc() const;
	uint32 GetCurrentFrameIndex() const;
	FColor GetClearColor() const;
	EFormat GetBackBufferFormat() const;
	bool GetIsVSyncEnabled() const;
	bool GetIsTearingSupported() const;
	uint32 GetNumBackBuffers() const;
	bool GetIsFullScreenEnabled() const;
	FColorBuffer& GetColorBufferForCurrentFrame();

	//
	// Setters
	//
	void SetCurrentFrameIndex( uint32 Index );
	void SetClearColor(const FColor& NewColor);
	void SetIsVSyncEnabled( bool bEnabled );
	void SetIsTearingSupported( bool bSupported );

protected:
	//
	// Virtual Methods
	// 
	void UnInitialize();
	void SetIsFullScreenEnabled( bool bEnabled );

protected:
	FSwapChainDesc m_Desc;
	uint32 m_FrameIndex;
	FColor m_ClearColor;
	bool m_bFullScreenEnabled;
	bool m_bVSyncEnabled;
	bool m_bAllowTearing;

	FRenderDevice* m_pDeviceRef;
	std::vector<FColorBuffer> m_DisplayPlanes;

protected:
#if R_WITH_D3D12
	// Utility
	void ResizeDXGIBuffers();
	
	// D3D Initialize
	void BindSwapChainBackBuffers();
	void CheckTearingSupport( IDXGIFactory6* pFactory );
	void Create( const FSwapChainDesc& InitParams, IDXGIFactory6** ppDXGIFactory, FCommandQueue* ppCommandQueue, ID3D12Device* pDevice );

	ID3D12Device* m_pID3D12DeviceRef;
	IDXGISwapChain3* m_pDXGISwapChain;
	
#endif

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
	return m_Desc.Format; 
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
	return m_Desc.BufferCount; 
}

FORCEINLINE bool FSwapChain::GetIsFullScreenEnabled() const
{ 
	return m_bFullScreenEnabled; 
}

FORCEINLINE FColorBuffer& FSwapChain::GetColorBufferForCurrentFrame()
{ 
	return m_DisplayPlanes[GetCurrentFrameIndex()]; 
}

FORCEINLINE void* FSwapChain::GetNativeSwapChain() const 
{ 
#if R_WITH_D3D12
	return SCast<void*>( m_pDXGISwapChain );
#endif
	return NULL;
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
	SetCurrentFrameIndex((GetCurrentFrameIndex() + 1) % GetNumBackBuffers());
}

FORCEINLINE void FSwapChain::SetClearColor(const FColor& NewColor)
{
	m_ClearColor = NewColor;
	for (uint32 i = 0; i < m_DisplayPlanes.size(); ++i)
		m_DisplayPlanes[i].SetClearColor(m_ClearColor);
}

FORCEINLINE void FSwapChain::SetIsFullScreenEnabled( bool bEnabled )
{ 
	m_bFullScreenEnabled = bEnabled; 
}
