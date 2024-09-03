#include "EnginePCH.h"

#include "Engine/Renderer/RenderContext.h"


void FRenderContext::Initialize()
{

}

void FRenderContext::UnInitialize()
{
	// Wait for all work on the GPU to finish.
	GCommandManager.IdleGpu();
	GStaticGeometryManager.FlushCache();

	GGraphicsDevice.UnInitialize();
}

void FRenderContext::PreFrame()
{

}

void FRenderContext::SubmitFrame()
{

}

void FRenderContext::PostFrame()
{

}

void FRenderContext::OnWindowWindowedModeEntered()
{

}

void FRenderContext::OnWindowBorderlessModeEntered()
{

}

void FRenderContext::OnNativeResolutionChanged(const uint32& Width, const uint32& height)
{

}

