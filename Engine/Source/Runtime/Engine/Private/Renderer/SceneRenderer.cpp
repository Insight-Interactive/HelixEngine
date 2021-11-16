#include "EnginePCH.h"

#include "Renderer/SceneRenderer.h"

#include "World/Scene.h"
#include "CommandContext.h"
#include "Renderer/GeometryGenerator.h"
#include "GameFramework/Components/HCameraComponent.h"
#include "Engine/Engine.h"
#include "RendererCore.h"
#include "Renderer/LightManager.h"


FSceneRenderer::FSceneRenderer()
	: m_DepthBuffer(kMaxSceneDepth, 0)
{

}

FSceneRenderer::~FSceneRenderer()
{

}

void FSceneRenderer::Initialize( const FSceneRendererInitParams& InitParams )
{
	m_DepthBuffer.Create( L"[Geometry Pass] Scene Depth Buffer", (uint32)InitParams.RenderingResolution.x, (uint32)InitParams.RenderingResolution.y, F_D32_Float );

	// Init render passes
	m_DeferredShader.GetGeometryPass().SetDepthBuffer( m_DepthBuffer );
	m_DeferredShader.GetLightPass().SetDepthBuffer( m_DepthBuffer );
	m_DeferredShader.Initialize( InitParams.RenderingResolution, InitParams.BackBufferFormat );
	//m_ForwardRenderPass.Initialize( RenderResolution, SwapchainFormat, DepthBufferFormat );
	m_SkyPass.Initialize( InitParams.BackBufferFormat, m_DepthBuffer.GetFormat() );
	//m_PostProcessPass.Initialize( RenderResolution );

	m_pScreenQuadRef = GeometryGenerator::GenerateScreenAlignedQuadMesh();


	// Init constant buffers, one for each frame index.
	uint32 NumBackBuffers = InitParams.NumBackBuffers;
	m_SceneConstantBuffers.resize( (size_t)NumBackBuffers );
	m_LightConstantBuffers.resize( (size_t)NumBackBuffers );
	for (uint32 i = 0; i < NumBackBuffers; ++i)
	{
		WString SceneConstsBufferName = L"Scene Constants Frame: " + i;
		m_SceneConstantBuffers[i].Create( SceneConstsBufferName.c_str() );

		WString SceneLightsBufferName = L"Scene Lights Frame: " + i;
		m_LightConstantBuffers[i].Create( SceneLightsBufferName.c_str() );
	}
}

void FSceneRenderer::Uninitialize()
{
	//m_PostProcessPass.UnInitialize();
	m_DeferredShader.UnInitialize();
	m_SkyPass.UnInitialize();
	uint32 NumBackBuffers = (uint32)m_SceneConstantBuffers.size();
	for (uint32 i = 0; i < NumBackBuffers; ++i)
	{
		m_SceneConstantBuffers[i].Destroy();
		m_LightConstantBuffers[i].Destroy();
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
	
	// Sky Pass
	{
		FColorBuffer& SkyRenderTarget = m_DeferredShader.GetGeometryPass().GetGBufferColorBuffer(FDeferredShadingTech::EGBuffers::GB_Albedo);
		// Add the skybox into the render target for translucent objects to blend with.
		m_SkyPass.Bind(CmdContext, SkyRenderTarget, m_DepthBuffer);
		m_SkyPass.UnBind(CmdContext, m_DepthBuffer);
	}

	// Prep the render target to be rendered too.
	//
	const FColorBuffer* pRTs[] = { &RenderTarget };
	CmdContext.OMSetRenderTargets( 1, pRTs, NULL );

	// Deferred Light Pass
	{
		m_DeferredShader.BindLightPass( CmdContext, Scissor );
		SetCommonRenderState( CmdContext, true, false );
		CmdContext.SetPrimitiveTopologyType( PT_TiangleList );
		CmdContext.BindVertexBuffer( 0, m_pScreenQuadRef->GetVertexBuffer() );
		CmdContext.BindIndexBuffer( m_pScreenQuadRef->GetIndexBuffer() );
		CmdContext.DrawIndexedInstanced( m_pScreenQuadRef->GetNumIndices(), 1, 0, 0, 0 );
		m_DeferredShader.UnBindLightPass( CmdContext );
	}

	// Forward Render Pass
	if (Scene.DoesContainAnyTranslucentObjects())
	{
		m_ForwardRenderPass.Bind( CmdContext, Scissor );
		CmdContext.TransitionResource(m_DepthBuffer, RS_DepthWrite );
		CmdContext.OMSetRenderTargets( 1, pRTs, &m_DepthBuffer );
		
		SetCommonRenderState(CmdContext, true, true);
		CmdContext.SetDescriptorHeap(RHT_CBV_SRV_UAV, GTextureHeap.GetNativeHeap());

		Scene.RenderStaticLitTranslucentObjects(CmdContext);
	
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
		SceneConstantsCBData* pCBData = GetSceneConstBufferForCurrentFrame().GetBufferPointer();
		if (m_pRenderingCamera)
		{
			m_pRenderingCamera->GetViewMatrix().Transpose( pCBData->kViewMat );
			m_pRenderingCamera->GetProjectionMatrix().Transpose( pCBData->kProjMat );

			m_pRenderingCamera->GetViewMatrix().Invert( pCBData->kInverseViewMat );
			m_pRenderingCamera->GetProjectionMatrix().Invert( pCBData->kInverseProjMat );
			pCBData->kInverseViewMat = pCBData->kInverseViewMat.Transpose();
			pCBData->kInverseProjMat = pCBData->kInverseProjMat.Transpose();

			pCBData->kCameraPos = m_pRenderingCamera->GetTransform().GetAbsoluteWorldPosition();
			pCBData->kCameraFarZ = m_pRenderingCamera->GetFarZ();
			pCBData->kCameraNearZ = m_pRenderingCamera->GetNearZ();
			pCBData->kWorldTime = (float)GEngine->GetAppSeconds();
		}
		CmdContext.SetGraphicsConstantBuffer( kSceneConstants, GetSceneConstBufferForCurrentFrame() );
	}

	if (UploadLights)
	{
		SceneLightsCBData* pLights = GetLightConstBufferForCurrentFrame().GetBufferPointer();
		{
			uint64 PointLightBufferSize = sizeof( PointLightCBData ) * GLightManager.GetScenePointLightCount();
			uint64 DirectionalLightBufferSize = sizeof( DirectionalLightCBData ) * GLightManager.GetSceneDirectionalLightCount();

			CopyMemory( pLights->PointLights, GLightManager.GetPointLighBufferPointer(), PointLightBufferSize );
			CopyMemory( pLights->DirectionalLights, GLightManager.GetDirectionalLightBufferPointer(), DirectionalLightBufferSize );

			// TODO: pLights->NumSpotLights = GLightManager.GetSceneSpotLightCount();
			pLights->NumPointLights = GLightManager.GetScenePointLightCount();
			pLights->NumDirectionalLights = GLightManager.GetSceneDirectionalLightCount();
		}
		CmdContext.SetGraphicsConstantBuffer( kLights, GetLightConstBufferForCurrentFrame() );
	}
}