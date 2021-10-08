/*
	File: FRenderPass.h
	Source: None
	Author Garrett Courtney

	Description:
	Describes the base functionality needed for a render pass.
*/
#pragma once


class HCameraComponent;
class FCommandContext;
struct FRect;

class FRenderPass
{
public:
	/*
		Bind the pass for rendering.
	*/
	virtual void Bind( FCommandContext& GfxContext, const FRect& Viewrect ) = 0;
	
	/*
		UnBinds the pass from rendering.
	*/
	virtual void UnBind( FCommandContext& GfxContext ) = 0;

	/*
		Retuns a reference to the currently rendering camera for this pass.
		Can be null if some passes do not require a camera.
	*/
	HCameraComponent* GetRenderingCamera();

	virtual void ReloadPipeline() = 0;

protected:
	void SetRenderingCamera( HCameraComponent* pCamera );
	
	FRenderPass()
	{
	}
	virtual ~FRenderPass()
	{
	}

	HCameraComponent* m_pRenderingCamera;
};


//
// Inline function implementations
//

inline HCameraComponent* FRenderPass::GetRenderingCamera()
{
	return m_pRenderingCamera;
}

inline void FRenderPass::SetRenderingCamera( HCameraComponent* pCamera )
{
	m_pRenderingCamera = pCamera;
}
