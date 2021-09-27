#include "EnginePCH.h"

#include "Renderer/Technique/SkyPass.h"

#include "FileSystem.h"
#include "Renderer/GeometryGenerator.h"
#include "Renderer/Common.h"
#include "Renderer/ShaderRegisters.h"
#include "CommonStructHelpers.h"
#include "IDevice.h"
#include "IRootSignature.h"
#include "ICommandContext.h"
#include "IGpuResource.h"
#include "IDepthBuffer.h"
#include "IColorBuffer.h"
#include "IPipelineState.h"


SkyboxPass::SkyboxPass()
{
}

SkyboxPass::~SkyboxPass()
{
}

void SkyboxPass::Initialize(EFormat RenderTargetFormat, EFormat DepthBufferFormat)
{
	m_DepthTargetFormat = DepthBufferFormat;
	m_RenderTargetFormat = RenderTargetFormat;

	m_SkyGeometry = GeometryGenerator::GenerateSphere(10, 20, 20);
	m_SkyDiffuse = GTextureManager->LoadTexture("Content/Textures/Skyboxes/PlainSunset/PlainSunset_Diff.dds", DT_Magenta2D, false);

	GDevice->CreateRootSignature(&m_pRS);
	m_pRS->Reset(5, 1);
	(*m_pRS).InitStaticSampler(0, GLinearWrapSamplerDesc, SV_Pixel);
	// Common
	(*m_pRS)[kSceneConstants].InitAsConstantBuffer(kSceneConstants, SV_All);
	(*m_pRS)[kMeshWorld].InitAsConstantBuffer(kMeshWorld, SV_Vertex);
	(*m_pRS)[kMaterial].InitAsConstantBuffer(kMaterial, SV_Pixel);
	(*m_pRS)[kLights].InitAsConstantBuffer(kLights, SV_Pixel);
	// Pipeline
	// Sky Cubemap
	(*m_pRS)[SPRP_Diffuse].InitAsDescriptorTable(1, SV_Pixel);
	(*m_pRS)[SPRP_Diffuse].SetTableRange(0, DRT_ShaderResourceView, 0, 1);
	m_pRS->Finalize(TEXT("Skybox Pass RootSignature"), RSF_AllowInputAssemblerLayout);

	// Create the pipeline state.
	//
	DataBlob VSShader = FileSystem::ReadRawData("Shaders/SkyboxPass.vs.cso");
	DataBlob PSShader = FileSystem::ReadRawData("Shaders/SkyboxPass.ps.cso");

	InputElementDesc InputElements[1] =
	{
		{ "POSITION",	0, F_R32G32B32_Float,	0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	};
	const uint32 kNumInputElements = HE_ARRAYSIZE(InputElements);

	DepthStencilStateDesc DepthStateDesc = {};
	DepthStateDesc.DepthEnable = true;
	DepthStateDesc.DepthWriteMask = DWM_All;
	DepthStateDesc.DepthFunc = CF_LessEqual;

	RasterizerDesc RasterStateDesc = {};
	RasterStateDesc.DepthClipEnabled = true;
	RasterStateDesc.CullMode = CM_Front;
	RasterStateDesc.FillMode = FM_Solid;

	PipelineStateDesc PSODesc = {};
	PSODesc.VertexShader = { VSShader.GetBufferPointer(), VSShader.GetDataSize() };
	PSODesc.PixelShader = { PSShader.GetBufferPointer(), PSShader.GetDataSize() };
	PSODesc.InputLayout.pInputElementDescs = InputElements;
	PSODesc.InputLayout.NumElements = kNumInputElements;
	PSODesc.pRootSignature = m_pRS;
	PSODesc.DepthStencilState = DepthStateDesc;
	PSODesc.BlendState = CBlendDesc();
	PSODesc.RasterizerDesc = RasterStateDesc;
	PSODesc.SampleMask = UINT_MAX;
	PSODesc.PrimitiveTopologyType = PTT_Triangle;
	PSODesc.NumRenderTargets = 1;
	PSODesc.RTVFormats[0] = RenderTargetFormat;
	PSODesc.SampleDesc = { 1, 0 };
	PSODesc.DSVFormat = DepthBufferFormat;

	GDevice->CreatePipelineState(PSODesc, &m_pPSO);
}

void SkyboxPass::UnInitialize()
{
	HE_SAFE_DELETE_PTR( m_pRS );
	HE_SAFE_DELETE_PTR( m_pPSO );
}

void SkyboxPass::Bind(ICommandContext& GfxContext, IColorBuffer& RenderTarget, IDepthBuffer& DepthBuffer)
{
	GfxContext.BeginDebugMarker(TEXT("Sky Pass"));

	//Transition
	// 
	// Color
	IGpuResource& AlbedoGBufferResource = *DCast<IGpuResource*>(&RenderTarget);
	GfxContext.TransitionResource(AlbedoGBufferResource, RS_RenderTarget);
	// Depth
	IGpuResource& DepthBufferResource = *DCast<IGpuResource*>(&DepthBuffer);
	GfxContext.TransitionResource(DepthBufferResource, RS_DepthWrite);

	// Set
	//
	GfxContext.SetGraphicsRootSignature(*m_pRS);
	GfxContext.SetPipelineState(*m_pPSO);

	const IColorBuffer* pRTs[] = {
				&RenderTarget,
	};
	GfxContext.OMSetRenderTargets(1, pRTs, &DepthBuffer);


}

void SkyboxPass::UnBind(ICommandContext& GfxContext, IDepthBuffer& DepthBuffer)
{
	// Render
	//
	GfxContext.SetTexture(SPRP_Diffuse, m_SkyDiffuse);

	if (m_SkyGeometry->IsValid())
	{
		GfxContext.SetPrimitiveTopologyType(PT_TiangleList);
		GfxContext.BindVertexBuffer(0, m_SkyGeometry->GetVertexBuffer());
		GfxContext.BindIndexBuffer(m_SkyGeometry->GetIndexBuffer());
		GfxContext.DrawIndexedInstanced(m_SkyGeometry->GetNumIndices(), 1, 0, 0, 0);
	}
	else
		HE_LOG(Warning, TEXT("Trying to render a skybox with invalid geometry!"));

	// Transition
	//
	IGpuResource& DepthBufferResource = *DCast<IGpuResource*>(&DepthBuffer);
	GfxContext.TransitionResource(DepthBufferResource, RS_DepthRead);

	GfxContext.EndDebugMarker();
}

void SkyboxPass::ReloadPipeline()
{
	HE_SAFE_DELETE_PTR( m_pPSO );

	// Create the pipeline state.
	//
	DataBlob VSShader = FileSystem::ReadRawData( "Shaders/SkyboxPass.vs.cso" );
	DataBlob PSShader = FileSystem::ReadRawData( "Shaders/SkyboxPass.ps.cso" );

	InputElementDesc InputElements[1] =
	{
		{ "POSITION",	0, F_R32G32B32_Float,	0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	};
	const uint32 kNumInputElements = HE_ARRAYSIZE( InputElements );

	DepthStencilStateDesc DepthStateDesc = {};
	DepthStateDesc.DepthEnable = true;
	DepthStateDesc.DepthWriteMask = DWM_All;
	DepthStateDesc.DepthFunc = CF_LessEqual;

	RasterizerDesc RasterStateDesc = {};
	RasterStateDesc.DepthClipEnabled = true;
	RasterStateDesc.CullMode = CM_Front;
	RasterStateDesc.FillMode = FM_Solid;

	PipelineStateDesc PSODesc = {};
	PSODesc.VertexShader = { VSShader.GetBufferPointer(), VSShader.GetDataSize() };
	PSODesc.PixelShader = { PSShader.GetBufferPointer(), PSShader.GetDataSize() };
	PSODesc.InputLayout.pInputElementDescs = InputElements;
	PSODesc.InputLayout.NumElements = kNumInputElements;
	PSODesc.pRootSignature = m_pRS;
	PSODesc.DepthStencilState = DepthStateDesc;
	PSODesc.BlendState = CBlendDesc();
	PSODesc.RasterizerDesc = RasterStateDesc;
	PSODesc.SampleMask = UINT_MAX;
	PSODesc.PrimitiveTopologyType = PTT_Triangle;
	PSODesc.NumRenderTargets = 1;
	PSODesc.RTVFormats[0] = m_RenderTargetFormat;
	PSODesc.SampleDesc = { 1, 0 };
	PSODesc.DSVFormat = m_DepthTargetFormat;

	GDevice->CreatePipelineState( PSODesc, &m_pPSO );
}
