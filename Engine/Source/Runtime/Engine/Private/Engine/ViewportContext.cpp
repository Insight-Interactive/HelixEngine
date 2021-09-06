// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/ViewportContext.h"

#include "Engine/HEngine.h"
#include "Input/MouseEvent.h"
#include "IDevice.h"
#include "IGpuResource.h"
#include "ICommandContext.h"
#include "IConstantBufferManager.h"
#include "Renderer/Common.h"
#include "Renderer/LightManager.h"
#include "Renderer/MaterialManager.h"
#include "Renderer/ShaderRegisters.h"
#include "Renderer/GeometryGenerator.h"
#include "Renderer/ConstantBufferStructures.h"
#include "Renderer/Technique/SkyPass.h"
#include "Renderer/Technique/DeferredShadingTech.h"
#include "Renderer/Technique/PostProcessUber.h"
#include "GameFramework/Actor/Components/HCameraComponent.h"


ViewportContext::ViewportContext()
{
	m_DeferredShader = new DeferredShadingTech();
	m_SkyPass = new SkyboxPass();
	m_PostProcessPass = new PostProcesssUber();
}

ViewportContext::~ViewportContext()
{
	HE_SAFE_DELETE_PTR( m_DeferredShader );
	HE_SAFE_DELETE_PTR( m_SkyPass );
	HE_SAFE_DELETE_PTR( m_PostProcessPass );
}

void ViewportContext::Initialize( const Window::Description& WindowDesc )
{
	m_Window.Create( WindowDesc );
	m_InputDispatcher.Initialize( m_Window.GetNativeWindow() );

	m_GameWorld.SetViewport( this );
	m_GameWorld.Initialize();
	m_GameWorld.BeginPlay();

	m_ViewPort.TopLeftX = 0.f;
	m_ViewPort.TopLeftY = 0.f;
	m_ViewPort.MinDepth = 0.f;
	m_ViewPort.MaxDepth = 1.f;
	m_ViewPort.Width = (float)m_Window.GetWidth();
	m_ViewPort.Height = (float)m_Window.GetHeight();

	m_ScissorRect.Left = 0;
	m_ScissorRect.Top = 0;
	m_ScissorRect.Right = m_Window.GetWidth();
	m_ScissorRect.Bottom = m_Window.GetHeight();

	m_Window.GetSwapChain()->SetClearColor( Color( .25f, 0.f, 1.f ) );

	m_Window.AddListener( this, &ViewportContext::OnEvent );
	m_PostProcessPass = new PostProcesssUber();

	InitializeRenderingResources();
}

void ViewportContext::Uninitialize()
{
	for (size_t i = 0; i < m_pSceneConstantBuffers.size(); ++i)
	{
		GConstantBufferManager->DestroyConstantBuffer( m_pSceneConstantBuffers[i]->GetUID() );
		GConstantBufferManager->DestroyConstantBuffer( m_pLightConstantBuffers[i]->GetUID() );
	}
	m_PostProcessPass->UnInitialize();
	m_DeferredShader->UnInitialize();
	m_SkyPass->UnInitialize();

	m_InputDispatcher.UnInitialize();
	m_Window.Destroy();
	m_GameWorld.Flush();

	HE_SAFE_DELETE_PTR( m_pSceneRenderTarget );
}

void ViewportContext::Update( float DeltaTime )
{
	m_InputDispatcher.UpdateInputs( DeltaTime );

	m_GameWorld.Tick( DeltaTime );
}

void ViewportContext::Render()
{
	IColorBuffer* pSwapChainBackBuffer = m_Window.GetRenderSurface();

	ICommandContext& CmdContext = ICommandContext::Begin( TEXT( "Scene Pass" ) );
	{
		if (GEngine->GetIsEditorPresent())
		{
			RenderWorld( CmdContext, *m_pSceneRenderTarget );
		}
		else
		{
			IGpuResource& SwapChainGpuResource = *DCast<IGpuResource*>( pSwapChainBackBuffer );
			CmdContext.TransitionResource( SwapChainGpuResource, RS_RenderTarget );
			CmdContext.ClearColorBuffer( *pSwapChainBackBuffer, GetClientRect() );

			RenderWorld( CmdContext, *pSwapChainBackBuffer );
		}
	}
	CmdContext.End();
}

void ViewportContext::RenderWorld( ICommandContext& CmdContext, IColorBuffer& RenderTarget  )
{
	IGpuResource& RenderTargetResource = *RenderTarget.As<IGpuResource*>();
	CmdContext.TransitionResource( RenderTargetResource, RS_RenderTarget );

	// Deferred Pass
	{
		// Bind the texture heap
		//
		CmdContext.SetDescriptorHeap( RHT_CBV_SRV_UAV, GTextureHeap );

		// Render the Geometry Pass
		//
		m_DeferredShader->GetGeometryPass().Bind( CmdContext, GetClientRect() );

		// Set common state
		//
		SetCommonRenderState( CmdContext );

		// Draw world
		//
		m_GameWorld.Render( CmdContext );

		m_DeferredShader->GetGeometryPass().UnBind( CmdContext );
	}

	// Prep the render target to be rendered too.
	//
	CmdContext.ClearColorBuffer( RenderTarget, GetClientRect() );
	const IColorBuffer* pRTs[] = {
		&RenderTarget,
	};
	CmdContext.OMSetRenderTargets( 1, pRTs, NULL );
	CmdContext.RSSetViewPorts( 1, &GetClientViewport() );
	CmdContext.RSSetScissorRects( 1, &GetClientRect() );

	// Light Pass
	{
		// Render the Light Pass
		//
		m_DeferredShader->GetLightPass().Bind( CmdContext, GetClientRect() );
		SetCommonRenderState( CmdContext );
		CmdContext.SetPrimitiveTopologyType( PT_TiangleList );
		CmdContext.BindVertexBuffer( 0, m_pScreenQuadRef->GetVertexBuffer() );
		CmdContext.BindIndexBuffer( m_pScreenQuadRef->GetIndexBuffer() );
		CmdContext.DrawIndexedInstanced( m_pScreenQuadRef->GetNumIndices(), 1, 0, 0, 0 );
		m_DeferredShader->GetLightPass().UnBind( CmdContext );

		// Render the sky last to prevent overdraw
		//
		m_SkyPass->Bind( CmdContext, RenderTarget, *m_DeferredShader->GetGeometryPass().GetSceneDepthBuffer() );
		CmdContext.SetDescriptorHeap( RHT_CBV_SRV_UAV, GTextureHeap );
		SetCommonRenderState( CmdContext );
		m_SkyPass->UnBind( CmdContext, *m_DeferredShader->GetGeometryPass().GetSceneDepthBuffer() );
	}

	// Post-Process Pass
	{
		/*m_PostProcessPass.Bind( CmdContext, GetClientRect() );

		m_PostProcessPass.UnBind( CmdContext );*/
	}
}

void ViewportContext::InitializeRenderingResources()
{
	FVector2 RenderResolution( (float)m_Window.GetWidth(), (float)m_Window.GetHeight() );
	m_DeferredShader->Initialize( RenderResolution, m_Window.GetSwapChain()->GetBackBufferFormat() );
	m_SkyPass->Initialize( m_Window.GetSwapChain()->GetDesc().Format, m_DeferredShader->GetGeometryPass().GetDepthFormat() );
	m_PostProcessPass->Initialize( RenderResolution );

	m_pScreenQuadRef = GeometryGenerator::GenerateScreenAlignedQuadMesh();
	
	GDevice->CreateColorBuffer( TEXT( "Pre-Display Buffer" ), (uint32)RenderResolution.x, (uint32)RenderResolution.y, 1, m_Window.GetSwapChain()->GetBackBufferFormat(), &m_pSceneRenderTarget );


	//
	// Init constant buffers, one for each frame index.
	//
	uint32 NumBackBuffers = m_Window.GetSwapChain()->GetNumBackBuffers();
	m_pSceneConstantBuffers.resize( (size_t)NumBackBuffers );
	m_pLightConstantBuffers.resize( (size_t)NumBackBuffers );
	for (uint32 i = 0; i < NumBackBuffers; ++i)
	{
		WString SceneConstsBufferName = L"Scene Constants Frame: " + i;
		GConstantBufferManager->CreateConstantBuffer( SceneConstsBufferName.c_str(), &m_pSceneConstantBuffers[i], sizeof( SceneConstantsCBData ) );

		WString SceneLightsBufferName = L"Scene Lights Frame: " + i;
		GConstantBufferManager->CreateConstantBuffer( SceneLightsBufferName.c_str(), &m_pLightConstantBuffers[i], sizeof( SceneLightsCBData ) );
	}
}

void ViewportContext::SetCommonRenderState( ICommandContext& CmdContext )
{
	// Set View and Scissor
	//
	CmdContext.RSSetViewPorts( 1, &GetClientViewport() );
	CmdContext.RSSetScissorRects( 1, &GetClientRect() );

	// Set Constant Buffers
	//
	SceneConstantsCBData* pCBData = GetSceneConstBufferForCurrentFrame()->GetBufferPointer<SceneConstantsCBData>();
	HCameraComponent* pCurrentCamera = m_GameWorld.GetCurrentSceneRenderCamera();
	if (pCurrentCamera)
	{
		pCBData->ViewMat = pCurrentCamera->GetViewMatrix().Transpose();
		pCBData->ProjMat = pCurrentCamera->GetProjectionMatrix().Transpose();
		pCBData->ViewMat.Invert( pCBData->InverseViewMat );
		pCBData->ProjMat.Invert( pCBData->InverseProjMat );
		pCBData->CameraPos = pCurrentCamera->GetTransform().GetAbsoluteWorldPosition();
		pCBData->CameraFarZ = pCurrentCamera->GetFarZ();
		pCBData->CameraNearZ = pCurrentCamera->GetNearZ();
	}
	//pData->WorldTime = (float)m_GFXTimer.Seconds();
	CmdContext.SetGraphicsConstantBuffer( kSceneConstants, GetSceneConstBufferForCurrentFrame() );

	// Upload the lights
	SceneLightsCBData* pLights = GetLightConstBufferForCurrentFrame()->GetBufferPointer<SceneLightsCBData>();
	{
		uint64 PointLightBufferSize			= sizeof( PointLightCBData ) * GLightManager.GetScenePointLightCount();
		uint64 DirectionalLightBufferSize	= sizeof( DirectionalLightCBData ) * GLightManager.GetSceneDirectionalLightCount();
	
		CopyMemory( pLights->PointLights, GLightManager.GetPointLighBufferPointer(), PointLightBufferSize );
		CopyMemory( pLights->DirectionalLights, GLightManager.GetDirectionalLightBufferPointer(), DirectionalLightBufferSize );

		// TODO: pLights->NumSpotLights = GLightManager.GetSceneSpotLightCount();
		pLights->NumPointLights = GLightManager.GetScenePointLightCount();
		pLights->NumDirectionalLights = GLightManager.GetSceneDirectionalLightCount();
	}
	CmdContext.SetGraphicsConstantBuffer( kLights, GetLightConstBufferForCurrentFrame() );
}

void ViewportContext::ReloadRenderPipelines()
{
	//m_PostProcessPass->ReloadPipeline();
	m_DeferredShader->ReloadPipeline();
	//m_SkyPass->ReloadPipeline();
}

//
// Event Processing
//


void ViewportContext::OnEvent( Event& e )
{
	EventDispatcher Dispatcher(e);

	// Mouse
	Dispatcher.Dispatch<MouseRawPointerMovedEvent>( this, &ViewportContext::OnMouseRawPointerMoved );
}

bool ViewportContext::OnMouseRawPointerMoved( MouseRawPointerMovedEvent& e )
{
	GetInputDispatcher()->GetInputSureyor().SetMouseMoveDelta( e.GetX(), e.GetY() );
	return false;
}
