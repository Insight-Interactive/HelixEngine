#include "EnginePCH.h"

#include "Renderer/Technique/ForwardRenderPass.h"

#include "Renderer/ShaderRegisters.h"
#include "Renderer/Common.h"
#include "RendererCore.h"
#include "RenderDevice.h"
#include "PipelineState.h"
#include "RootSignature.h"
#include "CommonStructHelpers.h"
#include "CommandContext.h"


FForwardRenderPass::FForwardRenderPass()
{

}

FForwardRenderPass::~FForwardRenderPass()
{

}

void FForwardRenderPass::Initialize( const FVector2& RenderResolution, const EFormat& RenderTargetFormat, const EFormat& DepthFormat )
{
	return;
	m_RS.Reset( 6, 1 );
	m_RS.InitStaticSampler( 0, GLinearWrapSamplerDesc, SV_Pixel );
	// Common
	m_RS[kSceneConstants].InitAsConstantBuffer( kSceneConstants, SV_All );
	m_RS[kMeshWorld].InitAsConstantBuffer( kMeshWorld, SV_Vertex );
	m_RS[kMaterial].InitAsConstantBuffer( kMaterial, SV_Pixel );
	m_RS[kLights].InitAsConstantBuffer( kLights, SV_Pixel );
	// Pipeline
	// Albedo
	m_RS[GRP_MaterialTextureAlbedo].InitAsDescriptorTable( 1, SV_Pixel );
	m_RS[GRP_MaterialTextureAlbedo].SetTableRange( 0, DRT_ShaderResourceView, 0, 1 );
	// Normal
	m_RS[GRP_MaterialTextureNormal].InitAsDescriptorTable( 1, SV_Pixel );
	m_RS[GRP_MaterialTextureNormal].SetTableRange( 0, DRT_ShaderResourceView, 1, 1 );
	m_RS.Finalize( L"[Forward Renderer] RootSig", RSF_AllowInputAssemblerLayout );


	// Create the pipeline state.
	//
	DataBlob VSShader = FileSystem::ReadRawData( "Shaders/Core/GenericForwardPass.vs.cso" );
	DataBlob PSShader = FileSystem::ReadRawData( "Shaders/Core/GenericForwardPass.ps.cso" );

	FPipelineStateDesc PSODesc = {};
	PSODesc.VertexShader = { VSShader.GetBufferPointer(), VSShader.GetDataSize() };
	PSODesc.PixelShader = { PSShader.GetBufferPointer(), PSShader.GetDataSize() };
	PSODesc.InputLayout.pInputElementDescs = GSceneMeshInputElements;
	PSODesc.InputLayout.NumElements = kNumSceneMeshCommonInputElements;
	PSODesc.pRootSignature = &m_RS;
	PSODesc.DepthStencilState = CDepthStencilStateDesc();
	//PSODesc.DepthStencilState.DepthFunc = CF_GreaterEqual;
	PSODesc.BlendState.RenderTarget[0].BlendEnable = true;
	PSODesc.BlendState.RenderTarget[0].BlendOp = BO_Add;
	PSODesc.BlendState.RenderTarget[0].SourceBlend = EBlend::B_One;
	PSODesc.BlendState.RenderTarget[0].DestBlend = EBlend::B_One;
	PSODesc.BlendState.RenderTarget[0].DestBlendAlpha = EBlend::B_One;
	PSODesc.BlendState.RenderTarget[0].SourceBlendAlpha = EBlend::B_One;
	PSODesc.BlendState.RenderTarget[0].BlendOpAlpha = EBlendOp::BO_Add;
	PSODesc.RasterizerDesc = CRasterizerDesc();
	PSODesc.SampleMask = UINT_MAX;
	PSODesc.PrimitiveTopologyType = PTT_Triangle;
	PSODesc.NumRenderTargets = 1;
	PSODesc.RTVFormats[0] = RenderTargetFormat;
	PSODesc.DSVFormat = DepthFormat;
	PSODesc.SampleDesc = { 1, 0 };
	m_PSO.Initialize( PSODesc );
};

void FForwardRenderPass::Bind( FCommandContext& GfxContext, const FRect& Viewrect )
{
	GfxContext.BeginDebugMarker( TEXT( "Geometry Pass" ) );

	//GfxContext.SetPipelineState( m_PSO );
	//GfxContext.SetGraphicsRootSignature( m_RS );

	GfxContext.EndDebugMarker();
}

void FForwardRenderPass::UnBind( FCommandContext& GfxContext ) 
{

}

void FForwardRenderPass::ReloadPipeline()
{

}
