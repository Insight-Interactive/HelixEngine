#include "EnginePCH.h"

#include "Engine/SplashScreen.h"

#include "IRenderDevice.h"
#include "ISwapChain.h"
#include "IPixelBuffer.h"
#include "IGpuResource.h"
#include "IRootSignature.h"
#include "IPipelineState.h"
#include "ICommandManager.h"
#include "ICommandContext.h"
#include "CommonStructHelpers.h"
#include "Renderer/Common.h"
#include "Renderer/GeometryGenerator.h"

#include "FileSystem.h"
#include "DataTypeWrappers.h"


FSplashScreen::FSplashScreen( const String& SplashTexturePath/* = "" */)
	: FWindow( TEXT( "App Splash" ), HE_SPLASH_WIDTH, HE_SPLASH_HEIGHT, false, true, NULL )
	, m_pRootSig(NULL)
	, m_pPipeline(NULL)
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

	m_SplashTexture = GTextureManager->LoadTexture( SplashTexturePath, DT_Magenta2D, false );
	m_ScreenQuadRef = GeometryGenerator::GenerateScreenAlignedQuadMesh();

	GDevice->CreateRootSignature( &m_pRootSig );
	m_pRootSig->Reset( 1, 1 );
	(*m_pRootSig).InitStaticSampler( 0, GLinearWrapSamplerDesc, SV_Pixel );
	// Common
	(*m_pRootSig)[kSplashTextureRootIndex].InitAsDescriptorTable( 1, SV_Pixel );
	(*m_pRootSig)[kSplashTextureRootIndex].SetTableRange( 0, DRT_ShaderResourceView, 0, 1 );
	(*m_pRootSig).Finalize( L"[App Splash] RootSig", RSF_AllowInputAssemblerLayout );

	// Create the pipeline state.
	//
	DataBlob VSShader = FileSystem::ReadRawData( "Shaders/ScreenAlignedQuad.vs.cso" );
	DataBlob PSShader = FileSystem::ReadRawData( "Shaders/AppSlash.ps.cso" );

	FPipelineStateDesc PSODesc = {};
	PSODesc.VertexShader = { VSShader.GetBufferPointer(), VSShader.GetDataSize() };
	PSODesc.PixelShader = { PSShader.GetBufferPointer(), PSShader.GetDataSize() };
	PSODesc.InputLayout.pInputElementDescs = GScreenSpaceInputElements;
	PSODesc.InputLayout.NumElements = 2;
	PSODesc.pRootSignature = m_pRootSig;
	PSODesc.BlendState = CBlendDesc();
	PSODesc.RasterizerDesc = CRasterizerDesc();
	PSODesc.SampleMask = UINT_MAX;
	PSODesc.PrimitiveTopologyType = PTT_Triangle;
	PSODesc.NumRenderTargets = 1;
	PSODesc.RTVFormats[0] = DCast<FPixelBuffer*>( GetRenderSurface() )->GetFormat();
	PSODesc.SampleDesc = { 1, 0 };
	GDevice->CreatePipelineState( PSODesc, &m_pPipeline );
}

FSplashScreen::~FSplashScreen()
{
	// Flush the Gpu before destroying resources.
	GCommandManager->IdleGpu();

	HE_SAFE_DELETE_PTR( m_pRootSig );
	HE_SAFE_DELETE_PTR( m_pPipeline );
}

void FSplashScreen::Render( FCommandContext& CmdContext )
{
	FColorBuffer* pCurrentBackBuffer = GetRenderSurface();
	FGpuResource& BackBufferResource = *pCurrentBackBuffer->As<FGpuResource*>();

	CmdContext.TransitionResource( BackBufferResource, RS_RenderTarget );

	CmdContext.ClearColorBuffer( *pCurrentBackBuffer, GetScissorRect() );
	CmdContext.RSSetViewPorts( 1, &GetViewport() );

	CmdContext.RSSetViewPorts( 1, &GetViewport() );
	CmdContext.RSSetScissorRects( 1, &GetScissorRect() );

	const FColorBuffer* RTs[] = {
		pCurrentBackBuffer
	};
	CmdContext.OMSetRenderTargets( 1, RTs, NULL );
	CmdContext.SetGraphicsRootSignature( *m_pRootSig );
	CmdContext.SetPipelineState( *m_pPipeline );

	CmdContext.SetDescriptorHeap( RHT_CBV_SRV_UAV, GTextureHeap );
	CmdContext.SetTexture( kSplashTextureRootIndex, m_SplashTexture );
	
	CmdContext.SetPrimitiveTopologyType( PT_TiangleList );
	CmdContext.BindVertexBuffer( 0, m_ScreenQuadRef->GetVertexBuffer() );
	CmdContext.BindIndexBuffer( m_ScreenQuadRef->GetIndexBuffer() );
	CmdContext.DrawIndexedInstanced( m_ScreenQuadRef->GetNumIndices(), 1, 0, 0, 0 );

	CmdContext.TransitionResource( BackBufferResource, RS_Present );

}
