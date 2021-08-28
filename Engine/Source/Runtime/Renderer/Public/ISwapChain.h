#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "Color.h"
#include "CommonEnums.h"
#include "CommonStructs.h"
#include "IColorBuffer.h"


class IColorBuffer;
class IDevice;


class RENDER_API ISwapChain
{
	friend class RenderContext;
	friend class Window;
public:
	virtual void Initialize(IDevice* pDevice) = 0;

	virtual void SwapBuffers() = 0;
	virtual void Resize(const uint32& Width, const uint32& Height) = 0;
	virtual void SetNumBackBuffes(uint32 NumBuffers) = 0;
	virtual void SetBackBufferFormat(EFormat& Format) = 0;

	virtual void* GetNativeSwapChain() const = 0;

	virtual void ToggleFullScreen(bool IsEnabled) { SetIsFullScreenEnabled(IsEnabled); }
	virtual void ToggleVsync(bool IsEnabled) { SetIsVSyncEnabled(IsEnabled); }

	FORCEINLINE void MoveToNextFrame();

	//
	// Getters
	//
	FORCEINLINE SwapChainDescription GetDesc()		const { return m_Desc; }
	FORCEINLINE uint32 GetCurrentFrameIndex()		const { return m_FrameIndex; }
	FORCEINLINE Color GetClearColor()				const { return m_ClearColor; }
	FORCEINLINE EFormat GetBackBufferFormat()		const { return m_Desc.Format; }
	FORCEINLINE bool GetIsVSyncEnabled()			const { return m_bVSyncEnabled; }
	FORCEINLINE bool GetIsTearingSupported()		const { return m_bAllowTearing; }
	FORCEINLINE uint32 GetNumBackBuffers()			const { return m_Desc.BufferCount; }
	FORCEINLINE bool GetIsFullScreenEnabled()		const { return m_bFullScreenEnabled; }
	FORCEINLINE IColorBuffer* GetColorBufferForCurrentFrame() const { return m_DisplayPlanes[GetCurrentFrameIndex()]; }

	//
	// Setters
	//
	FORCEINLINE void SetCurrentFrameIndex(uint32 Index) { m_FrameIndex = Index; }
	FORCEINLINE void SetClearColor(const Color& NewColor);
	FORCEINLINE void SetIsFullScreenEnabled(bool bEnabled) { m_bFullScreenEnabled = bEnabled; }
	FORCEINLINE void SetIsVSyncEnabled(bool bEnabled)
	{
		if (m_bFullScreenEnabled)
			m_bVSyncEnabled = bEnabled;
		else
			R_LOG(Error, TEXT("Trying to enable VSync while in windowed mode! This is not allowed."));
	}
	FORCEINLINE void SetIsTearingSupported(bool bSupported) { m_bAllowTearing = bSupported; }

protected:
	ISwapChain()
		: m_FrameIndex(0)
		, m_ClearColor(0.f, 0.f, 0.f)
		, m_bFullScreenEnabled(false)
		, m_bVSyncEnabled(false)
		, m_bAllowTearing(false)
		, m_pDeviceRef(NULL)
		, m_DisplayPlanes()
	{
	}

	virtual ~ISwapChain()
	{
		m_pDeviceRef = NULL;

		for (uint64 i = 0; i < m_DisplayPlanes.size(); ++i)
			delete m_DisplayPlanes[i];
	}

	//
	// Virtual Methods
	// 
	virtual void UnInitialize() = 0;

protected:
	SwapChainDescription m_Desc;
	uint32 m_FrameIndex;
	Color m_ClearColor;
	bool m_bFullScreenEnabled;
	bool m_bVSyncEnabled;
	bool m_bAllowTearing;

	IDevice* m_pDeviceRef;
	std::vector<IColorBuffer*> m_DisplayPlanes;
};


// 
// Inline function implementations
// 

void ISwapChain::MoveToNextFrame()
{
	SetCurrentFrameIndex((GetCurrentFrameIndex() + 1) % GetNumBackBuffers());
}

void ISwapChain::SetClearColor(const Color& NewColor)
{
	m_ClearColor = NewColor;
	for (uint32 i = 0; i < m_DisplayPlanes.size(); ++i)
		m_DisplayPlanes[i]->SetClearColor(m_ClearColor);
}
