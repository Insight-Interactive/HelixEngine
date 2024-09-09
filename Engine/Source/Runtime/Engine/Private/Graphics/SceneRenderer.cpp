// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Graphics/SceneRenderer.h"

#include "World/Scene.h"
#include "Engine/Renderer/RendererCore.h"
#include "Engine/Engine.h"
#include "Engine/Renderer/CommandContext.h"
#include "Graphics/LightManager.h"
#include "Graphics/ShaderRegisters.h"
#include "Graphics/GeometryGenerator.h"
#include "GameFramework/Components/HCameraComponent.h"
#include "Graphics/FontManager.h"


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

	m_BatchRenderer.Initialize( m_pOwningViewport->GetMainSceneRenderTarget(), m_DepthBuffer );
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
	m_SwapchainFrameIndex	= SwapchainFrameIndex;
	m_pRenderingCamera		= pRenderingCamera;

	FCommandContext& CmdContext = FCommandContext::Begin( L"Render Pass" );

	// Prep the render target to be rendered to.
	CmdContext.TransitionResource( RenderTarget, RS_RenderTarget );
	CmdContext.ClearColorBuffer(RenderTarget, Scissor);
	CmdContext.RSSetViewPorts(1, &View);
	CmdContext.RSSetScissorRects(1, &Scissor);

	// Bind the texture heap
	CmdContext.SetDescriptorHeap(RHT_CBV_SRV_UAV, GTextureHeap.GetNativeHeap());

	// TODO: Scene.CullObjects();
	Scene.SortStaticTransparentObjects();

	//
	// Opaque objects get deferred shading. Translucent or unlit objects get forward shading.
	//
	
	// Render static opaque objects
	{
		// Render the Geometry Pass
		m_DeferredShader.BindGeometryPass(CmdContext, Scissor);
		m_DeferredShader.ClearGBuffers(CmdContext, Scissor);

		// Set common state
		SetCommonRenderState( CmdContext, false, true );

		// Draw 
		Scene.RenderWorldGeo( CmdContext );
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

	// Render Debug Lines
	{
		m_BatchRenderer.SetRenderTarget( m_pOwningViewport->GetMainSceneRenderTarget() );
		m_BatchRenderer.PreRender( CmdContext );
		SetCommonRenderState( CmdContext, false, true );
		m_BatchRenderer.Render( CmdContext );
	}

	// Render Collision
	{
		CmdContext.OMSetRenderTargets( 1, pRTs, &m_DepthBuffer );
		Scene.RenderDebugMeshes( CmdContext );
	}


	// Post-Process Pass
	{
		/*
		m_PostProcessPass.Bind( CmdContext, GetClientRect() );

		m_PostProcessPass.UnBind( CmdContext );
		*/
	}

	CmdContext.End(true);
}

void FSceneRenderer::ResizeBuffers( uint32 Width, uint32 Height )
{
	m_DeferredShader.Resize( Width, Height );
	m_DepthBuffer.Create( L"[Geometry Pass] Scene Depth Buffer", Width, Height, F_D32_Float );
	m_ForwardRenderPass.Resize( Width, Height );
	m_SkyPass.Resize( Width, Height );
	m_PostProcessPass.Resize( Width, Height );
}

void FSceneRenderer::SetCommonRenderState( FCommandContext& CmdContext, bool UploadLights, bool UploadSceneConsts )
{
	// Set Constant Buffer/s
	//
	if (UploadSceneConsts)
	{
		TConstantBuffer<SceneConstantsCBData>& Buffer = GetSceneConstBufferForCurrentFrame();
		{
			if (m_pRenderingCamera != nullptr)
			{
				m_pRenderingCamera->GetViewMatrix().Transpose( Buffer->kViewMat );
				m_pRenderingCamera->GetProjectionMatrix().Transpose( Buffer->kProjMat );

				m_pRenderingCamera->GetViewMatrix().Invert( Buffer->kInverseViewMat );
				m_pRenderingCamera->GetProjectionMatrix().Invert( Buffer->kInverseProjMat );
				Buffer->kInverseViewMat = Buffer->kInverseViewMat.Transpose();
				Buffer->kInverseProjMat = Buffer->kInverseProjMat.Transpose();

				Buffer->kCameraPos = m_pRenderingCamera->GetWorldPosition();
				Buffer->kCameraFarZ = m_pRenderingCamera->GetFarZ();
				Buffer->kCameraNearZ	= m_pRenderingCamera->GetNearZ();
			}
			Buffer->kWorldTime		= (float)GEngine->GetAppSeconds();
		}
		CmdContext.SetGraphicsConstantBuffer( kSceneConstants, Buffer );
	}

	if (UploadLights)
	{
		TConstantBuffer<SceneLightsCBData>& Buffer = GetLightConstBufferForCurrentFrame();
		{
			CopyMemory( Buffer->PointLights, GLightManager.GetPointLighBufferPointer(), sizeof( PointLightData ) * GLightManager.GetScenePointLightCount() );
			CopyMemory( &Buffer->kWorldSun, GLightManager.GetWordSunDirectionalLight(), sizeof( DirectionalLightCBData ) );

			// TODO: Lights->NumSpotLights = GLightManager.GetSceneSpotLightCount();
			Buffer->NumPointLights = GLightManager.GetScenePointLightCount();
		}
		CmdContext.SetGraphicsConstantBuffer( kLights, Buffer);
	}
}

void FSceneRenderer::DrawDebugLine( const FDebugLineRenderInfo& LineInfo )
{
	m_BatchRenderer.SubmitLineRenderRequest( LineInfo );
}
