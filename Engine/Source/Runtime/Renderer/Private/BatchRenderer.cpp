#include "RendererPCH.h"

#include "BatchRenderer.h"

#include "CommandContext.h"
#include "FileSystem.h"
#include "CommonStructHelpers.h"
#include "ColorBuffer.h"
#include "DepthBuffer.h"
#include "Renderer/ShaderRegisters.h"


FBatchRenderer::FBatchRenderer()
	: m_pRenderTarget( nullptr )
	, m_pDepthBuffer( nullptr )
	, m_NumPendingLines( 0 )
{
	ZeroMemory( m_LineVertexPool, sizeof( m_LineVertexPool ) );
}

FBatchRenderer::~FBatchRenderer()
{

}

void FBatchRenderer::Initialize( FColorBuffer& RenderTarget, FDepthBuffer& DepthBufferTarget )
{
	SetRenderTarget( RenderTarget );
	SetDepthBuffer( DepthBufferTarget );
	m_LinesVB.Create( L"Line Vertex Buffer Pool", kMaxLineRenders * sizeof( FDebugLineVertex ), sizeof( FDebugLineVertex ), m_LineVertexPool );
	
	m_RS.Reset( 1 );
	m_RS[kSceneConstants].InitAsConstantBuffer( kSceneConstants, SV_All );
	m_RS.Finalize(L"Debug Line Renderer RootSig", RSF_AllowInputAssemblerLayout );

	// Create the pipeline state.
	//
	DataBlob VSShader = FileSystem::ReadRawData( "Shaders/Core/DebugLine.vs.cso" );
	DataBlob PSShader = FileSystem::ReadRawData( "Shaders/Core/DebugLine.ps.cso" );

	//D3D12_INPUT_ELEMENT_DESC
	FInputElementDesc MeshInputElements[3] =
	{
		{ "POSITION",	0, F_R32G32B32_Float,		0, HE_APPEND_ALIGNED_ELEMENT, IC_PerVertexData, 0 },
		{ "COLOR",		0, F_R32G32B32A32_Float,	0, HE_APPEND_ALIGNED_ELEMENT, IC_PerVertexData, 0 },
		{ "THICKNESS",	0, F_R32_Float,				0, HE_APPEND_ALIGNED_ELEMENT, IC_PerVertexData, 0 },
	};
	const uint32 kNumMeshInputElements = HE_ARRAYSIZE( MeshInputElements );

	FPipelineStateDesc PSODesc = {};
	PSODesc.VertexShader = { VSShader.GetBufferPointer(), VSShader.GetDataSize() };
	PSODesc.PixelShader = { PSShader.GetBufferPointer(), PSShader.GetDataSize() };
	PSODesc.InputLayout.pInputElementDescs = MeshInputElements;
	PSODesc.InputLayout.NumElements = kNumMeshInputElements;
	PSODesc.pRootSignature = &m_RS;
	PSODesc.DepthStencilState = CDepthStencilStateDesc();
	PSODesc.BlendState = CBlendDesc();
	PSODesc.RasterizerDesc = CRasterizerDesc();
	PSODesc.SampleMask = UINT_MAX;
	PSODesc.PrimitiveTopologyType = PTT_Line;
	PSODesc.NumRenderTargets = 1;
	PSODesc.RTVFormats[0] = RenderTarget.GetFormat();
	PSODesc.DSVFormat = DepthBufferTarget.GetFormat();
	PSODesc.SampleDesc = { 1, 0 };
	m_PSO.Initialize( PSODesc );
}

void FBatchRenderer::UnInitialize()
{

}

void FBatchRenderer::SetRenderTarget( FColorBuffer& RenderTarget )
{
	m_pRenderTarget = &RenderTarget;
}

void FBatchRenderer::SetDepthBuffer( FDepthBuffer& DepthBuffer )
{
	m_pDepthBuffer = &DepthBuffer;
}

void FBatchRenderer::Tick( float DeltaTime )
{
	FDebugLineVertex* CBLineData = &m_LineVertexPool[0];

	for (uint32 i = 0; i < m_NumPendingLines; ++i)
	{
		FDebugLineVertex& LineInfo = m_LineVertexPool[i];

		LineInfo.Lifetime -= DeltaTime;
		if (LineInfo.Lifetime <= 0.f)
		{
			ZeroMemory( &m_LineVertexPool[i], sizeof( FDebugLineVertex ) );
			ZeroMemory( &m_LineVertexPool[i + 1], sizeof( FDebugLineVertex ) );

			m_NumPendingLines--;
			continue;
		}
		CBLineData++;
	}
}

void FBatchRenderer::PreRender( FCommandContext& CmdContext )
{
	if (m_pRenderTarget == nullptr)
		return;

	CmdContext.BeginDebugMarker( L"Render Debug Lines" );
	const FColorBuffer* RTs[] = { m_pRenderTarget };
	CmdContext.OMSetRenderTargets( 1, RTs, m_pDepthBuffer );
	CmdContext.SetGraphicsRootSignature( m_RS );
	CmdContext.SetPipelineState( m_PSO );
}

void FBatchRenderer::Render( FCommandContext& CmdContext )
{
	if (m_pRenderTarget == nullptr)
		return;

	m_LinesVB.Upload();
	CmdContext.SetPrimitiveTopologyType( PT_Linelist );
	CmdContext.BindVertexBuffer( 0, m_LinesVB );
	CmdContext.DrawInstanced( m_NumPendingLines * 2, 1, 0, 0 );
	CmdContext.EndDebugMarker();
}

void FBatchRenderer::SubmitLineRenderRequest( const FDebugLineRenderInfo& LineInfo )
{
	// Find two free verticies to emplace into the vertex pool.
	for (uint32 i = 0; i < kMaxLineRenders; i++)
	{
		if (m_LineVertexPool[i].Lifetime <= 0.f)
		{
			m_LineVertexPool[i].Position	= LineInfo.Start;
			m_LineVertexPool[i].Color		= LineInfo.Color.ToVector4();
			m_LineVertexPool[i].Thickness	= LineInfo.Thickness;
			m_LineVertexPool[i].Lifetime	= LineInfo.Lifetime;

			m_LineVertexPool[i + 1].Position	= LineInfo.End;
			m_LineVertexPool[i + 1].Color		= LineInfo.Color.ToVector4();
			m_LineVertexPool[i + 1].Thickness	= LineInfo.Thickness;
			m_LineVertexPool[i + 1].Lifetime	= LineInfo.Lifetime;
			
			m_NumPendingLines++;
			return;
		}
	}

	R_LOG( Warning, TEXT( "Trying to add line to batch renderer but no more slots are available! No new lines where added." ) );
}
