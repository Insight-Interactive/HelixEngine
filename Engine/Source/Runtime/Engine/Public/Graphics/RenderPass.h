// Copyright 2024 Insight Interactive. All Rights Reserved.
/*
	File: FRenderPass.h
	Source: None
	Author Garrett Courtney

	Description:
	Describes the base functionality needed for a render pass.
*/
#pragma once


class HCameraComponent;
class FViewportContext;
class FCommandContext;
struct FRect;

class FRenderPass
{
	friend class FSceneRenderer;
public:
	/*
		Bind the pass for rendering.
	*/
	virtual void Bind( FCommandContext& GfxContext, const FRect& Viewrect ) = 0;
	
	/*
		UnBinds the pass from rendering.
	*/
	virtual void UnBind( FCommandContext& GfxContext ) = 0;

	virtual void Resize( const uint32& Width, const uint32& Height ) = 0;

	/*
		Retuns a reference to the currently rendering camera for this pass.
		Can be null if some passes do not require a camera.
	*/
	HCameraComponent* GetRenderingCamera();
	FViewportContext* GetOwningViewport();

	virtual void ReloadPipeline() = 0;

protected:
	void SetRenderingCamera( HCameraComponent* pCamera );
	void SetOwningViewport( FViewportContext* pViewport );

	FRenderPass()
	{
	}
	virtual ~FRenderPass()
	{
	}

	// The currently rendering camera for this render pass.
	HCameraComponent* m_pRenderingCamera;
	// The viewport that owns this render pass.
	FViewportContext* m_pOwningViewport;

};


//
// Inline function implementations
//

FORCEINLINE HCameraComponent* FRenderPass::GetRenderingCamera()
{
	return m_pRenderingCamera;
}

FORCEINLINE FViewportContext* FRenderPass::GetOwningViewport()
{
	return m_pOwningViewport;
}

FORCEINLINE void FRenderPass::SetRenderingCamera( HCameraComponent* pCamera )
{
	m_pRenderingCamera = pCamera;
}

FORCEINLINE void FRenderPass::SetOwningViewport(FViewportContext* pViewport)
{
	if (pViewport == NULL)
	{
		HE_LOG(Warning, "Setting null viewport for render pass. Was this intended?");
	}
	m_pOwningViewport = pViewport;
}
