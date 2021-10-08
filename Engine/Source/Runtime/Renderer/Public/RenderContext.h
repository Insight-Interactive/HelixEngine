// Copyright Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "TSingleton.h"


class FSwapChain;


enum ERenderBackend
{
	RB_Invalid,
	RB_Direct3D11,
	RB_Direct3D12,

	RB_Count,
};

struct RenderBackendDescription
{
	TChar			APIName[32];
	ERenderBackend	Type;
};
extern const RenderBackendDescription RenderBackends[];


/*
	Responsible for handling rendering related messaged and resource management.
*/
class RENDER_API FRenderContext : public TSingleton<FRenderContext>
{
	friend class FRenderContextFactory;
	friend class FRenderContextFactoryD3D12;
	friend class FRendererInitializer;
public:
	FRenderContext()
		: m_BackendType(RB_Invalid)
	{
	}
	virtual ~FRenderContext()
	{
	}

	virtual void PreFrame();
	virtual void SubmitFrame();
	void PostFrame();


	void OnWindowWindowedModeEntered();
	void OnWindowBorderlessModeEntered();
	void OnNativeResolutionChanged(const uint32& Width, const uint32& height);

	//
	// Getters/Setters
	//
	FORCEINLINE ERenderBackend GetBackendType() const;
	FORCEINLINE bool IsReady() const;

protected:
	virtual void Initialize();
	virtual void UnInitialize();

	FORCEINLINE void SetBackendType(ERenderBackend Type);

protected:
	ERenderBackend m_BackendType;
	
};

//
// Inline Function Implementations
//

FORCEINLINE ERenderBackend FRenderContext::GetBackendType() const
{
	return m_BackendType;
}

FORCEINLINE void FRenderContext::SetBackendType(ERenderBackend Type)
{
	m_BackendType = Type;
}

FORCEINLINE bool FRenderContext::IsReady() const
{
	return (m_BackendType != RB_Invalid);
}
