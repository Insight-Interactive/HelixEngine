// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/ViewportContext.h"

#include "Engine/Engine.h"
#include "Input/MouseEvent.h"
#include "Input/KeyEvent.h"
#include "IRenderDevice.h"
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
#include "GameFramework/Components/HCameraComponent.h"
#include "World/World.h"
#include "Engine/Event/EngineEvent.h"


FViewportContext::FViewportContext()
	: m_WorldInView( NULL )
{
	m_DeferredShader	= new FDeferredShadingTech();
	m_SkyPass			= new FSkyboxPass();
	m_PostProcessPass	= new FPostProcessUber();
}

FViewportContext::~FViewportContext()
{
	HE_SAFE_DELETE_PTR( m_PostProcessPass );
	HE_SAFE_DELETE_PTR( m_SkyPass );
	HE_SAFE_DELETE_PTR( m_DeferredShader );
}

void FViewportContext::Initialize( const FWindow::Description& WindowDesc )
{
	m_Window.Create( WindowDesc );
	m_Window.GetSwapChain()->SetClearColor( FColor( .25f, 0.f, 1.f ) );
	m_Window.AddListener( this, &FViewportContext::OnEvent );

	m_InputDispatcher.Initialize( &m_Window );

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

	InitializeRenderingResources();
}

void FViewportContext::Uninitialize()
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

	HE_SAFE_DELETE_PTR( m_pSceneRenderTarget );
}

void FViewportContext::Update( float DeltaTime )
{
	// Update the world inputs.
	m_InputDispatcher.UpdateInputs( DeltaTime );
}

void FViewportContext::Render()
{
	FColorBuffer* pSwapChainBackBuffer = m_Window.GetRenderSurface();

	FCommandContext& CmdContext = FCommandContext::Begin( TEXT( "Scene Pass" ) );
	{
		if (GEngine->GetIsEditorPresent())
		{
			RenderWorld( CmdContext, *m_pSceneRenderTarget );
		}
		else
		{
			FGpuResource& SwapChainGpuResource = *DCast<FGpuResource*>( pSwapChainBackBuffer );
			CmdContext.TransitionResource( SwapChainGpuResource, RS_RenderTarget );
			CmdContext.ClearColorBuffer( *pSwapChainBackBuffer, GetClientRect() );

			RenderWorld( CmdContext, *pSwapChainBackBuffer );
		}
	}
	CmdContext.End();
}

void FViewportContext::RenderWorld( FCommandContext& CmdContext, FColorBuffer& RenderTarget  )
{
	if (m_WorldInView == NULL) return;

	FGpuResource& RenderTargetResource = *RenderTarget.As<FGpuResource*>();
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
		m_WorldInView->Render( CmdContext );

		m_DeferredShader->GetGeometryPass().UnBind( CmdContext );
	}

	// Prep the render target to be rendered too.
	//
	CmdContext.ClearColorBuffer( RenderTarget, GetClientRect() );
	const FColorBuffer* pRTs[] = {
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

void FViewportContext::InitializeRenderingResources()
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

void FViewportContext::SetCommonRenderState( FCommandContext& CmdContext )
{
	// Set View and Scissor
	//
	CmdContext.RSSetViewPorts( 1, &GetClientViewport() );
	CmdContext.RSSetScissorRects( 1, &GetClientRect() );

	// Set Constant Buffers
	//
	SceneConstantsCBData* pCBData = GetSceneConstBufferForCurrentFrame()->GetBufferPointer<SceneConstantsCBData>();

	HCameraComponent* pCurrentCamera = m_WorldInView->GetCurrentSceneRenderCamera();
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

void FViewportContext::ReloadRenderPipelines()
{
	//m_PostProcessPass->ReloadPipeline();
	m_DeferredShader->ReloadPipeline();
	//m_SkyPass->ReloadPipeline();
}

//
// Event Processing
//


void FViewportContext::OnEvent( Event& e )
{
	EventDispatcher Dispatcher(e);

	// Window
	Dispatcher.Dispatch<WindowLostFocusEvent>( this, &FViewportContext::OnWindowLostFocus );
	Dispatcher.Dispatch<WindowFocusEvent>( this, &FViewportContext::OnWindowFocus );

	// Mouse
	Dispatcher.Dispatch<MouseRawPointerMovedEvent>( this, &FViewportContext::OnMouseRawPointerMoved );
	Dispatcher.Dispatch<MouseWheelScrolledEvent>( this, &FViewportContext::OnMouseWheelScrolled );
	Dispatcher.Dispatch<MouseButtonPressedEvent>( this, &FViewportContext::OnMouseButtonPressed );
	Dispatcher.Dispatch<MouseButtonReleasedEvent>( this, &FViewportContext::OnMouseButtonReleased );

	// Keyboard
	Dispatcher.Dispatch<KeyPressedEvent>( this, &FViewportContext::OnKeyPressed );
	Dispatcher.Dispatch<KeyReleasedEvent>( this, &FViewportContext::OnKeyReleased );

}

bool FViewportContext::OnWindowLostFocus( WindowLostFocusEvent& e )
{

	return false;
}

bool FViewportContext::OnWindowFocus( WindowFocusEvent& e )
{

	return false;
}

bool FViewportContext::OnMouseRawPointerMoved( MouseRawPointerMovedEvent& e )
{
	GetInputDispatcher()->GetInputSureyor().SetMouseMoveDelta( e.GetX(), e.GetY() );
	return false;
}

bool FViewportContext::OnMouseWheelScrolled( MouseWheelScrolledEvent& e )
{
	GetInputDispatcher()->GetInputSureyor().SetMouseScrollDelta( e.GetXOffset(), e.GetYOffset() );
	return false;
}

bool FViewportContext::OnMouseButtonPressed( MouseButtonPressedEvent& e )
{
	GetInputDispatcher()->GetInputSureyor().SetMouseButton( e.GetKeyCode() - Mouse0, true );
	return false;
}

bool FViewportContext::OnMouseButtonReleased( MouseButtonReleasedEvent& e )
{
	GetInputDispatcher()->GetInputSureyor().SetMouseButton( e.GetKeyCode() - Mouse0, false );
	return false;
}

bool FViewportContext::OnKeyPressed( KeyPressedEvent& e )
{
	GetInputDispatcher()->GetInputSureyor().SetKey( (uint8)e.GetPlatformKeycode(), true );
	return false;
}

bool FViewportContext::OnKeyReleased( KeyReleasedEvent& e )
{
	GetInputDispatcher()->GetInputSureyor().SetKey( (uint8)e.GetPlatformKeycode(), false );
	return false;
}

