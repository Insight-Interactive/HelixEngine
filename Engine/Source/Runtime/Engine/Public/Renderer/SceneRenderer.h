#pragma once

#include "Renderer/Technique/DeferredShadingTech.h"
#include "Renderer/Technique/ForwardRenderPass.h"
#include "Renderer/Technique/PostProcessUber.h"
#include "Renderer/Technique/SkyPass.h"
#include "DepthBuffer.h"
#include "Renderer/ConstantBufferStructures.h"


static const EFormat kSceneDepthFormat = F_D32_Float;
static const EFormat kGBufferFormats[] =
{
	F_R8G8B8A8_UNorm, // Albedo
	F_R8G8B8A8_UNorm, // Normal
};

struct FSceneRendererInitParams
{
	FVector2 RenderingResolution;
	EFormat BackBufferFormat;
	uint32 NumBackBuffers;
};

class HScene;

class FSceneRenderer
{
public:
	FSceneRenderer();
	~FSceneRenderer();

	void Initialize(const FSceneRendererInitParams& InitParams);
	void Uninitialize();

	void RenderScene( HScene& Scene, FColorBuffer& RenderTarget, const FViewPort& View, const FRect& Scissor, uint32 SwapchainFrameIndex, HCameraComponent* pRenderingCamera );

	void ReloadPipelines();

	static EFormat GetSceneDepthBufferForamt();
	static EFormat GetGBufferFormatForBuffer( FDeferredShadingTech::EGBuffers BufferIndex );
	static uint32 GetNumGBuffers();

private:
	void SetCommonRenderState( FCommandContext& CmdContext, bool UploadLights, bool UploadSceneConsts );
	TConstantBuffer<SceneConstantsCBData>& GetSceneConstBufferForCurrentFrame();
	TConstantBuffer<SceneLightsCBData>& GetLightConstBufferForCurrentFrame();

private:

	// Rendering Resources
	//
	FDeferredShadingTech m_DeferredShader;
	FForwardRenderPass m_ForwardRenderPass;
	FSkyboxPass m_SkyPass;
	FPostProcessUber m_PostProcessPass;
	StaticMeshGeometryRef m_pScreenQuadRef;
	// Constant buffers
	std::vector<TConstantBuffer<SceneConstantsCBData>> m_SceneConstantBuffers; // One buffer per-frame
	std::vector<TConstantBuffer<SceneLightsCBData>> m_LightConstantBuffers; // One buffer per-frame
	EFormat m_DepthBufferFormat;
	FDepthBuffer m_DepthBuffer;

	uint32 m_SwapchainFrameIndex;
	HCameraComponent* m_pRenderingCamera;
};


//
// Inline function implementations
//


FORCEINLINE /*static*/ EFormat FSceneRenderer::GetSceneDepthBufferForamt()
{
	return kSceneDepthFormat;
}

FORCEINLINE /*static*/ EFormat FSceneRenderer::GetGBufferFormatForBuffer( FDeferredShadingTech::EGBuffers BufferIndex )
{
	HE_ASSERT( BufferIndex < FDeferredShadingTech::GB_NumBuffers && BufferIndex >= 0 );
	return kGBufferFormats[(uint32)BufferIndex];
}

FORCEINLINE void FSceneRenderer::ReloadPipelines()
{
	m_DeferredShader.ReloadPipeline();
}

FORCEINLINE /*static*/ uint32 FSceneRenderer::GetNumGBuffers()
{
	return (uint32)FDeferredShadingTech::GB_NumBuffers;
}

FORCEINLINE TConstantBuffer<SceneConstantsCBData>& FSceneRenderer::GetSceneConstBufferForCurrentFrame()
{
	return m_SceneConstantBuffers[m_SwapchainFrameIndex];
}

FORCEINLINE TConstantBuffer<SceneLightsCBData>& FSceneRenderer::GetLightConstBufferForCurrentFrame()
{
	return m_LightConstantBuffers[m_SwapchainFrameIndex];
}
