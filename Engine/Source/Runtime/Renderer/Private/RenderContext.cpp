#include "RendererPCH.h"

#include "RenderContext.h"
#include "RendererCore.h"
#include "ISwapChain.h"
#include "IRenderDevice.h"
#include "ICommandContext.h"
#include "ICommandManager.h"
#include "ITextureManager.h"
#include "IGeometryBufferManager.h"
#include "IConstantBufferManager.h"
#include "ModelManager.h"


const RenderBackendDescription RenderBackends[RB_Count] = 
{
	{ TEXT( HE_D3D11_RENDER_BACKEND_NAME ), RB_Direct3D11 },
	{ TEXT( HE_D3D12_RENDER_BACKEND_NAME ), RB_Direct3D12 },
};


void FRenderContext::Initialize()
{

}

void FRenderContext::UnInitialize()
{
	// Wait for all work on the GPU to finish.
	GCommandManager->IdleGpu();
	GStaticGeometryManager.FlushCache();

	HE_SAFE_DELETE_PTR(GDevice);
	HE_SAFE_DELETE_PTR(GCommandManager);
	HE_SAFE_DELETE_PTR(GContextManager);
	HE_SAFE_DELETE_PTR(GGeometryManager);
	HE_SAFE_DELETE_PTR(GConstantBufferManager);
	HE_SAFE_DELETE_PTR(GTextureManager);
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

