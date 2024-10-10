// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Graphics/Technique/SkyPass.h"

#include "FileSystem.h"
#include "Graphics/GeometryGenerator.h"
#include "Graphics/Common.h"
#include "Graphics/ShaderRegisters.h"
#include "Engine/GameProject.h"


FSkyboxPass::FSkyboxPass()
{
}

FSkyboxPass::~FSkyboxPass()
{
}

void FSkyboxPass::Initialize(EFormat RenderTargetFormat, EFormat DepthBufferFormat)
{
	m_DepthTargetFormat = DepthBufferFormat;
	m_RenderTargetFormat = RenderTargetFormat;

	// Resources
	//
	m_SkyGeometry = GeometryGenerator::GenerateSphere(10, 20, 20); 
	char TexturePath[HE_MAX_PATH];
	FGameProject::GetInstance()->GetContentDirectoryFullPath( "Textures/Skyboxes/PlainSunset/PlainSunset_Diff.dds", TexturePath, sizeof(TexturePath) );
	m_SkyDiffuse = GTextureManager.LoadTexture( TexturePath, DT_Magenta2D, false);


	// Create the pipeline state.
	//
	m_RS.Reset(5, 1);
	m_RS.InitStaticSampler(0, GLinearWrapSamplerDesc, SV_Pixel);
	// Common
	m_RS[kSceneConstants].InitAsConstantBuffer(kSceneConstants, SV_All);
	m_RS[kMeshWorld].InitAsConstantBuffer(kMeshWorld, SV_Vertex);
	m_RS[kLights].InitAsConstantBuffer(kLights, SV_Pixel);
	m_RS[kSkeletonJoints].InitAsConstantBuffer( kSkeletonJoints, SV_Vertex );
	// Pipeline
	// Sky Cubemap
	m_RS[SPRP_Diffuse].InitAsDescriptorTable(1, SV_Pixel);
	m_RS[SPRP_Diffuse].SetTableRange(0, DRT_ShaderResourceView, 0, 1);
	m_RS.Finalize(TEXT("Skybox Pass RootSignature"), RSF_AllowInputAssemblerLayout);

	DataBlob VSShader = FileSystem::ReadRawData("Shaders/Core/SkyboxPass.vs.cso");
	DataBlob PSShader = FileSystem::ReadRawData("Shaders/Core/SkyboxPass.ps.cso");

	FInputElementDesc InputElements[1] =
	{
		{ "POSITION",	0, F_R32G32B32_Float,	0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	};
	const uint32 kNumInputElements = HE_ARRAYSIZE(InputElements);

	FDepthStencilStateDesc DepthStateDesc = {};
	DepthStateDesc.DepthEnable = true;
	DepthStateDesc.DepthWriteMask = DWM_All;
	DepthStateDesc.DepthFunc = CF_LessEqual;

	FRasterizerDesc RasterStateDesc = {};
	RasterStateDesc.DepthClipEnabled = true;
	RasterStateDesc.CullMode = CM_Front;
	RasterStateDesc.FillMode = FM_Solid;

	FPipelineStateDesc PSODesc = {};
	PSODesc.VertexShader = { VSShader.GetBufferPointer(), VSShader.GetDataSize() };
	PSODesc.PixelShader = { PSShader.GetBufferPointer(), PSShader.GetDataSize() };
	PSODesc.InputLayout.pInputElementDescs = InputElements;
	PSODesc.InputLayout.NumElements = kNumInputElements;
	PSODesc.pRootSignature = &m_RS;
	PSODesc.DepthStencilState = DepthStateDesc;
	PSODesc.BlendState = CBlendDesc();
	PSODesc.RasterizerDesc = RasterStateDesc;
	PSODesc.SampleMask = UINT_MAX;
	PSODesc.PrimitiveTopologyType = PTT_Triangle;
	PSODesc.NumRenderTargets = 1;
	PSODesc.RTVFormats[0] = RenderTargetFormat;
	PSODesc.SampleDesc = { 1, 0 };
	PSODesc.DSVFormat = DepthBufferFormat;
	m_PSO.Initialize( PSODesc );
}

void FSkyboxPass::UnInitialize()
{
}

void FSkyboxPass::Bind(FCommandContext& GfxContext, FColorBuffer& RenderTarget, FDepthBuffer& DepthBuffer)
{
	GfxContext.BeginDebugMarker( L"Sky Pass" );

	//Transition
	// 
	// Color
	GfxContext.TransitionResource( RenderTarget, RS_RenderTarget);
	// Depth
	GfxContext.TransitionResource( DepthBuffer, RS_DepthWrite);

	// Set
	//
	GfxContext.SetGraphicsRootSignature(m_RS);
	GfxContext.SetPipelineState(m_PSO);

	const FColorBuffer* pRTs[] = {
				&RenderTarget,
	};
	GfxContext.OMSetRenderTargets(1, pRTs, &DepthBuffer);
}

void FSkyboxPass::UnBind(FCommandContext& GfxContext, FDepthBuffer& DepthBuffer)
{
	// Render
	//
	GfxContext.SetTexture(SPRP_Diffuse, m_SkyDiffuse);

	if (m_SkyGeometry.IsValid())
	{
		GfxContext.SetPrimitiveTopologyType(PT_TiangleList);
		GfxContext.BindVertexBuffer(0, m_SkyGeometry->GetMesh().GetVertexBuffer());
		GfxContext.BindIndexBuffer(m_SkyGeometry->GetMesh().GetIndexBuffer());
		GfxContext.DrawIndexedInstanced(m_SkyGeometry->GetMesh().GetNumIndices(), 1, 0, 0, 0);
	}
	else
		HE_LOG(Warning, "Trying to render a skybox with invalid geometry!");

	GfxContext.EndDebugMarker();
}

void FSkyboxPass::ReloadPipeline()
{
	// Create the pipeline state.
	//
	DataBlob VSShader = FileSystem::ReadRawData( "Shaders/Core/SkyboxPass.vs.cso" );
	DataBlob PSShader = FileSystem::ReadRawData( "Shaders/Core/SkyboxPass.ps.cso" );

	FInputElementDesc InputElements[1] =
	{
		{ "POSITION",	0, F_R32G32B32_Float,	0, HE_APPEND_ALIGNED_ELEMENT,	IC_PerVertexData, 0 },
	};
	const uint32 kNumInputElements = HE_ARRAYSIZE( InputElements );

	FDepthStencilStateDesc DepthStateDesc = {};
	DepthStateDesc.DepthEnable = true;
	DepthStateDesc.DepthWriteMask = DWM_All;
	DepthStateDesc.DepthFunc = CF_LessEqual;

	FRasterizerDesc RasterStateDesc = {};
	RasterStateDesc.DepthClipEnabled = true;
	RasterStateDesc.CullMode = CM_Front;
	RasterStateDesc.FillMode = FM_Solid;

	FPipelineStateDesc PSODesc = {};
	PSODesc.VertexShader = { VSShader.GetBufferPointer(), VSShader.GetDataSize() };
	PSODesc.PixelShader = { PSShader.GetBufferPointer(), PSShader.GetDataSize() };
	PSODesc.InputLayout.pInputElementDescs = InputElements;
	PSODesc.InputLayout.NumElements = kNumInputElements;
	PSODesc.pRootSignature = &m_RS;
	PSODesc.DepthStencilState = DepthStateDesc;
	PSODesc.BlendState = CBlendDesc();
	PSODesc.RasterizerDesc = RasterStateDesc;
	PSODesc.SampleMask = UINT_MAX;
	PSODesc.PrimitiveTopologyType = PTT_Triangle;
	PSODesc.NumRenderTargets = 1;
	PSODesc.RTVFormats[0] = m_RenderTargetFormat;
	PSODesc.SampleDesc = { 1, 0 };
	PSODesc.DSVFormat = m_DepthTargetFormat;
	m_PSO.Initialize( PSODesc );
}

void FSkyboxPass::Resize( const uint32& Width, const uint32& Height ) 
{

}
