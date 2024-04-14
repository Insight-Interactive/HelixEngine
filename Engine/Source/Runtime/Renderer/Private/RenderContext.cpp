#include "RendererPCH.h"

#include "RenderContext.h"
#include "RendererCore.h"
#include "SwapChain.h"
#include "RenderDevice.h"
#include "CommandContext.h"
#include "CommandManager.h"
#include "TextureManager.h"
#include "GeometryBufferManager.h"
#include "ConstantBuffer.h"
#include "ModelManager.h"


void FRenderContext::Initialize()
{

}

void FRenderContext::UnInitialize()
{
	// Wait for all work on the GPU to finish.
	GCommandManager.IdleGpu();
	GGeometryManager.FlushCache();

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

