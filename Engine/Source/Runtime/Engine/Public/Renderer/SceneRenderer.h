// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "Renderer/Technique/DeferredShadingTech.h"
#include "Renderer/Technique/ForwardRenderPass.h"
#include "Renderer/Technique/PostProcessUber.h"
#include "Renderer/Technique/SkyPass.h"
#include "DepthBuffer.h"
#include "Renderer/ConstantBufferStructures.h"
#include "Common.h"
#include "BatchRenderer.h"


static const EFormat kSceneDepthFormat = F_D32_Float;
static const EFormat kGBufferFormats[] =
{
	F_R8G8B8A8_UNorm,		// Albedo
	F_R16G16B16A16_UNorm,	// Normal
	F_R8_UNorm,				// Roughness
	F_R8_UNorm,				// Metallic
	F_R8_UNorm,				// Specular
};

struct FSceneRendererInitParams
{
	FVector2 RenderingResolution;
	EFormat BackBufferFormat;
};

class HScene;
class FViewportContext;

class FSceneRenderer
{
public:
	FSceneRenderer();
	~FSceneRenderer();

	void Initialize(const FSceneRendererInitParams& InitParams, FViewportContext& OwningViewport);
	void Uninitialize();

	void RenderScene( HScene& Scene, FColorBuffer& RenderTarget, const FViewPort& View, const FRect& Scissor, uint32 SwapchainFrameIndex, HCameraComponent* pRenderingCamera );
	void ResizeBuffers( uint32 Width, uint32 Height );
	void ReloadPipelines();

	FDepthBuffer& GetDepthBuffer();
	static EFormat GetSceneDepthBufferForamt();
	static EFormat GetGBufferFormatForBuffer( FDeferredShadingTech::EGBuffers BufferIndex );
	static uint32 GetNumGBuffers();

	bool ReservedBufferExistsByHashName( int32 NameHash );
	FConstantBufferInterface* GetReservedConstantBufferByHashNameForCurrentFrame( StringHashValue NameHash );
	FConstantBufferInterface* GetReservedConstantBufferByHashNameForFrame( StringHashValue NameHash, uint32 FrameIndex );

	void DrawDebugLine( const FDebugLineRenderInfo& LineInfo );

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
	// Contains hashed names and lookups for the reserved constant buffers in shaders.
	std::unordered_map<StringHashValue, std::vector<FConstantBufferInterface*>> m_ReservedConstBuffers;
	EFormat m_DepthBufferFormat;
	FDepthBuffer m_DepthBuffer;

	uint32 m_SwapchainFrameIndex;
	HCameraComponent* m_pRenderingCamera;
	FViewportContext* m_pOwningViewport;

	FBatchRenderer m_BatchRenderer;
};


//
// Inline function implementations
//


FORCEINLINE /*static*/ EFormat FSceneRenderer::GetSceneDepthBufferForamt()
{
	return kSceneDepthFormat;
}

FORCEINLINE FDepthBuffer& FSceneRenderer::GetDepthBuffer()
{
	return m_DepthBuffer;
}

FORCEINLINE /*static*/ EFormat FSceneRenderer::GetGBufferFormatForBuffer( FDeferredShadingTech::EGBuffers BufferIndex )
{
	HE_ASSERT( BufferIndex < FDeferredShadingTech::GB_NumBuffers && BufferIndex >= 0 );
	return kGBufferFormats[(uint32)BufferIndex];
}

FORCEINLINE /*static*/ uint32 FSceneRenderer::GetNumGBuffers()
{
	return (uint32)FDeferredShadingTech::GB_NumBuffers;
}

FORCEINLINE void FSceneRenderer::ReloadPipelines()
{
	m_DeferredShader.ReloadPipeline();
}

FORCEINLINE TConstantBuffer<SceneConstantsCBData>& FSceneRenderer::GetSceneConstBufferForCurrentFrame()
{
	const StringHashValue StrHash = StringHash(HE_STRINGIFY(SceneConstants_CB));
	TConstantBuffer<SceneConstantsCBData>* pConstBuffer = RCast< TConstantBuffer<SceneConstantsCBData>* >(m_ReservedConstBuffers[StrHash][m_SwapchainFrameIndex]);
	HE_ASSERT(pConstBuffer != NULL);
	return *pConstBuffer;
}

FORCEINLINE TConstantBuffer<SceneLightsCBData>& FSceneRenderer::GetLightConstBufferForCurrentFrame()
{
	const StringHashValue StrHash = StringHash(HE_STRINGIFY(SceneLights_CB));
	TConstantBuffer<SceneLightsCBData>* pConstBuffer = RCast< TConstantBuffer<SceneLightsCBData>* >(m_ReservedConstBuffers[StrHash][m_SwapchainFrameIndex]);
	HE_ASSERT(pConstBuffer != NULL);
	return *pConstBuffer;
}

FORCEINLINE bool FSceneRenderer::ReservedBufferExistsByHashName( int32 NameHash )
{
	return GetReservedConstantBufferByHashNameForCurrentFrame( NameHash ) != nullptr;
}

FORCEINLINE FConstantBufferInterface* FSceneRenderer::GetReservedConstantBufferByHashNameForCurrentFrame( StringHashValue NameHash )
{
	return GetReservedConstantBufferByHashNameForFrame( NameHash, m_SwapchainFrameIndex );
}

FORCEINLINE FConstantBufferInterface* FSceneRenderer::GetReservedConstantBufferByHashNameForFrame( StringHashValue NameHash, uint32 FrameIndex )
{
	HE_ASSERT( FrameIndex <= HE_MAX_SWAPCHAIN_BACK_BUFFERS );

	auto Iter = m_ReservedConstBuffers.find( NameHash );
	if (Iter != m_ReservedConstBuffers.end())
	{
		return Iter->second[FrameIndex];
	}
	return nullptr;
}
