// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Renderer/SceneRenderer.h"

#include "World/Scene.h"
#include "RendererCore.h"
#include "Engine/Engine.h"
#include "CommandContext.h"
#include "Renderer/LightManager.h"
#include "Renderer/ShaderRegisters.h"
#include "Renderer/GeometryGenerator.h"
#include "GameFramework/Components/HCameraComponent.h"


FSceneRenderer::FSceneRenderer()
	: m_DepthBuffer( HE_MAX_SCENE_DEPTH, 0 )
	, m_SwapchainFrameIndex( 0 )
	, m_pRenderingCamera( NULL )
	, m_pOwningViewport( NULL )
{

}

FSceneRenderer::~FSceneRenderer()
{

}

void FSceneRenderer::Initialize( const FSceneRendererInitParams& InitParams, FViewportContext& OwningViewport )
{
	m_pOwningViewport = &OwningViewport;

	m_DepthBuffer.Create( L"[Geometry Pass] Scene Depth Buffer", (uint32)InitParams.RenderingResolution.x, (uint32)InitParams.RenderingResolution.y, F_D32_Float );

	const StringHashValue SceneConstsStrHash = StringHash(HE_STRINGIFY(SceneConstants_CB));
	m_ReservedConstBuffers[SceneConstsStrHash] = std::vector<FConstantBufferInterface*>{};

	const StringHashValue SceneLightsStrHash = StringHash(HE_STRINGIFY(SceneLights_CB));
	m_ReservedConstBuffers[SceneLightsStrHash] = std::vector<FConstantBufferInterface*>{};

	for (uint32 i = 0; i < HE_MAX_SWAPCHAIN_BACK_BUFFERS; ++i)
	{
		WString SceneConstsBufferName = L"Scene Constants Frame: " + i;
		TConstantBuffer<SceneConstantsCBData>* pSConstBuffer = (TConstantBuffer<SceneConstantsCBData>*)m_ReservedConstBuffers[SceneConstsStrHash].emplace_back( new TConstantBuffer<SceneConstantsCBData>() );
		pSConstBuffer->Create(SceneConstsBufferName.c_str());

		WString SceneLightsBufferName = L"Scene Lights Frame: " + i;
		TConstantBuffer<SceneLightsCBData>* pLConstBuffer = (TConstantBuffer<SceneLightsCBData>*)m_ReservedConstBuffers[SceneLightsStrHash].emplace_back( new TConstantBuffer<SceneLightsCBData>() );
		pLConstBuffer->Create(SceneLightsBufferName.c_str());
	}

	// Init render passes
	// 
	// Deferred shading pass
	m_DeferredShader.SetOwningViewport(m_pOwningViewport);
	m_DeferredShader.GetGeometryPass().SetDepthBuffer( m_DepthBuffer );
	m_DeferredShader.GetLightPass().SetDepthBuffer( m_DepthBuffer );
	m_DeferredShader.Initialize( InitParams.RenderingResolution, InitParams.BackBufferFormat );
	// Forward shading pass
	m_ForwardRenderPass.SetOwningViewport(m_pOwningViewport);
	m_ForwardRenderPass.Initialize( InitParams.RenderingResolution, InitParams.BackBufferFormat, m_DepthBuffer.GetFormat() );
	// Sky pass
	m_SkyPass.SetOwningViewport(m_pOwningViewport);
	m_SkyPass.Initialize( InitParams.BackBufferFormat, m_DepthBuffer.GetFormat() );
	// TODO: PostProcess pass
	//m_PostProcessPass.Initialize( RenderResolution );

	m_pScreenQuadRef = GeometryGenerator::GenerateScreenAlignedQuadMesh();
}

void FSceneRenderer::Uninitialize()
{
	//m_PostProcessPass.UnInitialize();
	m_DeferredShader.UnInitialize();
	m_SkyPass.UnInitialize();

	// Destroy all cached constant buffers.
	for (auto& Iter : m_ReservedConstBuffers)
	{
		std::vector<FConstantBufferInterface*>& ConstBuffers = Iter.second;
		for (uint32 i = 0; i < ConstBuffers.size(); ++i)
		{
			delete ConstBuffers[i];
		}
	}
}

void FSceneRenderer::RenderScene( HScene& Scene, FColorBuffer& RenderTarget, const FViewPort& View, const FRect& Scissor, uint32 SwapchainFrameIndex, HCameraComponent* pRenderingCamera )
{
	m_SwapchainFrameIndex = SwapchainFrameIndex;
	m_pRenderingCamera = pRenderingCamera;

	FCommandContext& CmdContext = FCommandContext::Begin( L"Render Pass" );

	// Prep the render target to be rendered too.
	CmdContext.TransitionResource( RenderTarget, RS_RenderTarget );
	CmdContext.ClearColorBuffer(RenderTarget, Scissor);
	CmdContext.RSSetViewPorts(1, &View);
	CmdContext.RSSetScissorRects(1, &Scissor);

	// Bind the texture heap
	CmdContext.SetDescriptorHeap(RHT_CBV_SRV_UAV, GTextureHeap.GetNativeHeap());

	// TODO: Scene.CullObjects();
	Scene.SortStaticTransparentObjects();

	//
	// Opaque objects get defered shading. Translucent or unlit objects get forward shading.
	//
	
	// Render static opaque objects
	{
		// Render the Geometry Pass
		m_DeferredShader.BindGeometryPass(CmdContext, Scissor);
		m_DeferredShader.ClearGBuffers(CmdContext, Scissor);

		// Set common state
		SetCommonRenderState( CmdContext, false, true );

		// Draw 
		Scene.RenderStaticLitOpaqueObjects( CmdContext );

		m_DeferredShader.UnBindGeometryPass( CmdContext );
	}
	

	// Prep the render target to be rendered too.
	//
	const FColorBuffer* pRTs[] = { &RenderTarget };
	CmdContext.OMSetRenderTargets( 1, pRTs, NULL );

	// Deferred Light Pass
	{
		m_DeferredShader.BindLightPass( CmdContext, Scissor );
		SetCommonRenderState( CmdContext, true, true );
		CmdContext.SetPrimitiveTopologyType( PT_TiangleList );
		CmdContext.BindVertexBuffer( 0, m_pScreenQuadRef->GetVertexBuffer() );
		CmdContext.BindIndexBuffer( m_pScreenQuadRef->GetIndexBuffer() );
		CmdContext.DrawIndexedInstanced( m_pScreenQuadRef->GetNumIndices(), 1, 0, 0, 0 );
		m_DeferredShader.UnBindLightPass( CmdContext );
	}

	// Sky Pass
	{
		CmdContext.SetDescriptorHeap(RHT_CBV_SRV_UAV, GTextureHeap.GetNativeHeap());
		SetCommonRenderState(CmdContext, false, true);
		// Add the skybox into the render target for translucent objects to blend with.
		m_SkyPass.Bind(CmdContext, RenderTarget, m_DepthBuffer);
		m_SkyPass.UnBind(CmdContext, m_DepthBuffer);
	}

	// Forward Render Pass
	{
		m_ForwardRenderPass.Bind( CmdContext, Scissor );
		CmdContext.TransitionResource(m_DepthBuffer, RS_DepthWrite );
		CmdContext.OMSetRenderTargets( 1, pRTs, &m_DepthBuffer );
		
		SetCommonRenderState(CmdContext, true, true);
		CmdContext.SetDescriptorHeap(RHT_CBV_SRV_UAV, GTextureHeap.GetNativeHeap());

		Scene.RenderStaticTranslucentAndUnlitObjects(CmdContext);
	
		m_ForwardRenderPass.UnBind( CmdContext );
	}

	// Post-Process Pass
	{
		/*
		m_PostProcessPass.Bind( CmdContext, GetClientRect() );

		m_PostProcessPass.UnBind( CmdContext );
		*/
	}

	CmdContext.End();
}


void FSceneRenderer::SetCommonRenderState( FCommandContext& CmdContext, bool UploadLights, bool UploadSceneConsts )
{
	// Set Constant Buffer/s
	//
	if (UploadSceneConsts)
	{
		TConstantBuffer<SceneConstantsCBData>& Buffer = GetSceneConstBufferForCurrentFrame();
		SceneConstantsCBData& pCBData = *Buffer.GetBufferPointer();
		{
			if (m_pRenderingCamera)
			{
				m_pRenderingCamera->GetViewMatrix().Transpose( pCBData.kViewMat );
				m_pRenderingCamera->GetProjectionMatrix().Transpose( pCBData.kProjMat );

				m_pRenderingCamera->GetViewMatrix().Invert( pCBData.kInverseViewMat );
				m_pRenderingCamera->GetProjectionMatrix().Invert( pCBData.kInverseProjMat );
				pCBData.kInverseViewMat = pCBData.kInverseViewMat.Transpose();
				pCBData.kInverseProjMat = pCBData.kInverseProjMat.Transpose();

				pCBData.kCameraPos		= m_pRenderingCamera->GetAbsoluteWorldPosition();
				pCBData.kCameraFarZ		= m_pRenderingCamera->GetFarZ();
				pCBData.kCameraNearZ	= m_pRenderingCamera->GetNearZ();
			}
			pCBData.kWorldTime		= (float)GEngine->GetAppSeconds();

			Buffer.SetDirty(true);
		}
		CmdContext.SetGraphicsConstantBuffer( kSceneConstants, Buffer );
	}

	if (UploadLights)
	{
		TConstantBuffer<SceneLightsCBData>& Buffer = GetLightConstBufferForCurrentFrame();
		SceneLightsCBData& pLights = *Buffer.GetBufferPointer();
		{
			uint64 PointLightBufferSize = sizeof( PointLightCBData ) * GLightManager.GetScenePointLightCount();
			uint64 DirectionalLightBufferSize = sizeof( DirectionalLightCBData ) * GLightManager.GetSceneDirectionalLightCount();

			CopyMemory( pLights.PointLights, GLightManager.GetPointLighBufferPointer(), PointLightBufferSize );
			CopyMemory( pLights.DirectionalLights, GLightManager.GetDirectionalLightBufferPointer(), DirectionalLightBufferSize );

			// TODO: pLights->NumSpotLights = GLightManager.GetSceneSpotLightCount();
			pLights.NumPointLights = GLightManager.GetScenePointLightCount();
			pLights.NumDirectionalLights = GLightManager.GetSceneDirectionalLightCount();

			Buffer.SetDirty(true);
		}
		CmdContext.SetGraphicsConstantBuffer( kLights, Buffer);
	}
}