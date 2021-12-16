// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/SplashScreen.h"

#include "RenderDevice.h"
#include "SwapChain.h"
#include "PixelBuffer.h"
#include "GpuResource.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "CommandManager.h"
#include "CommandContext.h"
#include "CommonStructHelpers.h"
#include "Renderer/Common.h"
#include "Renderer/GeometryGenerator.h"

#include "FileSystem.h"
#include "DataTypeWrappers.h"


FSplashScreen::FSplashScreen( const String& SplashTexturePath/* = "" */)
	: FWindow( TEXT( "App Splash" ), HE_SPLASH_WIDTH, HE_SPLASH_HEIGHT, false, true, NULL )
{
	m_ViewPort.TopLeftX = 0.f;
	m_ViewPort.TopLeftY = 0.f;
	m_ViewPort.MinDepth = 0.f;
	m_ViewPort.MaxDepth = 1.f;
	m_ViewPort.Width = (float)GetWidth();
	m_ViewPort.Height = (float)GetHeight();

	m_ScissorRect.Left = 0;
	m_ScissorRect.Top = 0;
	m_ScissorRect.Right = GetWidth();
	m_ScissorRect.Bottom = GetHeight();

	GetSwapChain()->SetClearColor( FColor( 0.f, 0.f, 0.f ) );

	m_SplashTexture = GTextureManager.LoadTexture( SplashTexturePath, DT_Magenta2D, false );
	m_ScreenQuadRef = GeometryGenerator::GenerateScreenAlignedQuadMesh();

	m_RootSig.Reset( 1, 1 );
	m_RootSig.InitStaticSampler( 0, GLinearWrapSamplerDesc, SV_Pixel );
	// Common
	m_RootSig[kSplashTextureRootIndex].InitAsDescriptorTable( 1, SV_Pixel );
	m_RootSig[kSplashTextureRootIndex].SetTableRange( 0, DRT_ShaderResourceView, 0, 1 );
	m_RootSig.Finalize( L"[App Splash] RootSig", RSF_AllowInputAssemblerLayout );

	// Create the pipeline state.
	//
	DataBlob VSShader = FileSystem::ReadRawData( "Shaders/Core/ScreenAlignedQuad.vs.cso" );
	DataBlob PSShader = FileSystem::ReadRawData( "Shaders/Core/AppSlash.ps.cso" );

	FPipelineStateDesc PSODesc = {};
	PSODesc.VertexShader = { VSShader.GetBufferPointer(), VSShader.GetDataSize() };
	PSODesc.PixelShader = { PSShader.GetBufferPointer(), PSShader.GetDataSize() };
	PSODesc.InputLayout.pInputElementDescs = GScreenSpaceInputElements;
	PSODesc.InputLayout.NumElements = 2;
	PSODesc.pRootSignature = &m_RootSig;
	PSODesc.BlendState = CBlendDesc();
	PSODesc.RasterizerDesc = CRasterizerDesc();
	PSODesc.SampleMask = UINT_MAX;
	PSODesc.PrimitiveTopologyType = PTT_Triangle;
	PSODesc.NumRenderTargets = 1;
	PSODesc.RTVFormats[0] = GetRenderSurface().GetFormat();
	PSODesc.SampleDesc = { 1, 0 };
	m_Pipeline.Initialize( PSODesc );
}

FSplashScreen::~FSplashScreen()
{
	// Flush the Gpu before destroying resources.
	GCommandManager.IdleGpu();
}

void FSplashScreen::Render( FCommandContext& CmdContext )
{
	FColorBuffer& CurrentBackBuffer = GetRenderSurface();

	CmdContext.TransitionResource( CurrentBackBuffer, RS_RenderTarget );

	CmdContext.ClearColorBuffer( CurrentBackBuffer, GetScissorRect() );
	CmdContext.RSSetViewPorts( 1, &GetViewport() );

	CmdContext.RSSetViewPorts( 1, &GetViewport() );
	CmdContext.RSSetScissorRects( 1, &GetScissorRect() );

	const FColorBuffer* RTs[] = {
		&CurrentBackBuffer
	};
	CmdContext.OMSetRenderTargets( 1, RTs, NULL );
	CmdContext.SetGraphicsRootSignature( m_RootSig );
	CmdContext.SetPipelineState( m_Pipeline );

	CmdContext.SetDescriptorHeap( RHT_CBV_SRV_UAV, GTextureHeap.GetNativeHeap() );
	CmdContext.SetTexture( kSplashTextureRootIndex, m_SplashTexture );
	
	CmdContext.SetPrimitiveTopologyType( PT_TiangleList );
	CmdContext.BindVertexBuffer( 0, m_ScreenQuadRef->GetVertexBuffer() );
	CmdContext.BindIndexBuffer( m_ScreenQuadRef->GetIndexBuffer() );
	CmdContext.DrawIndexedInstanced( m_ScreenQuadRef->GetNumIndices(), 1, 0, 0, 0 );

	CmdContext.TransitionResource( CurrentBackBuffer, RS_Present );

}
