#include "EnginePCH.h"

#include "Engine/Renderer/BatchRenderer.h"

#include "Graphics/ShaderRegisters.h"
#include "Engine/Renderer/CommandContext.h"


FBatchRenderer::FBatchRenderer()
	: m_pRenderTarget( nullptr )
	, m_pDepthBuffer( nullptr )
{
}

FBatchRenderer::~FBatchRenderer()
{

}

void FBatchRenderer::Initialize( FColorBuffer& RenderTarget, FDepthBuffer& DepthBufferTarget )
{
	m_DepthLines.Initialize();
	m_NoDepthLines.Initialize();

	SetRenderTarget( RenderTarget );
	SetDepthBuffer( DepthBufferTarget );
	
	m_RS.Reset( 1 );
	m_RS[kSceneConstants].InitAsConstantBuffer( kSceneConstants, SV_All );
	m_RS.Finalize(L"Debug Line Renderer RootSig", RSF_AllowInputAssemblerLayout );

	// Create the pipeline state.
	//
	DataBlob VSShader = FileSystem::ReadRawData( "Shaders/Core/DebugLine.vs.cso" );
	DataBlob PSShader = FileSystem::ReadRawData( "Shaders/Core/DebugLine.ps.cso" );

	FInputElementDesc MeshInputElements[2] =
	{
		{ "POSITION",	0, F_R32G32B32_Float,		0, HE_APPEND_ALIGNED_ELEMENT, IC_PerVertexData, 0 },
		{ "COLOR",		0, F_R32G32B32A32_Float,	0, HE_APPEND_ALIGNED_ELEMENT, IC_PerVertexData, 0 },
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
	m_DepthPSO.Initialize( PSODesc );

	PSODesc.DepthStencilState.DepthEnable = false;
	m_NoDepthPSO.Initialize( PSODesc );
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

void FBatchRenderer::PreRender( FCommandContext& CmdContext )
{
	if (m_pRenderTarget == nullptr)
		return;

	CmdContext.BeginDebugMarker( L"Render Debug Lines" );
	const FColorBuffer* RTs[] = { m_pRenderTarget };
	CmdContext.OMSetRenderTargets( 1, RTs, m_pDepthBuffer );
	CmdContext.SetGraphicsRootSignature( m_RS );
}

void FBatchRenderer::Render( FCommandContext& CmdContext )
{
	if (m_pRenderTarget == nullptr)
		return;

	if (m_NoDepthLines.AnyLines())
	{
		CmdContext.SetPipelineState( m_NoDepthPSO );
		m_NoDepthLines.RenderLines( CmdContext );
	}

	if (m_DepthLines.AnyLines())
	{
		CmdContext.SetPipelineState( m_DepthPSO );
		m_DepthLines.RenderLines( CmdContext );
	}
}

void FBatchRenderer::SubmitLineRenderRequest( const FDebugLineRenderInfo& LineInfo )
{
	if (LineInfo.IgnoreDepth)
		m_NoDepthLines.AddLine( LineInfo );
	else
		m_DepthLines.AddLine( LineInfo );
}

void FBatchRenderer::ClearLines()
{
	m_NoDepthLines.ClearLines();
	m_DepthLines.ClearLines();
}


// FBatchRenderer::LineBatchInfo
//

FBatchRenderer::LineBatchInfo::LineBatchInfo()
: m_NumPendingLines( 0 )
, m_FreeVertexPos( 0 )
{
	ZeroMemory( m_LineVertexPool, sizeof( m_LineVertexPool ) );
}

void FBatchRenderer::LineBatchInfo::Initialize()
{
	m_LinesVB.Create( L"Line Vertex Buffer Pool", kMaxNumLineVerts * sizeof( FDebugLineVertex ), sizeof( FDebugLineVertex ), m_LineVertexPool );
}

void FBatchRenderer::LineBatchInfo::AddLine( const FDebugLineRenderInfo& LineInfo )
{
	if (m_NumPendingLines == kMaxLineRenders)
	{
		R_LOG( Warning, "Trying to add line to batch renderer but no more slots are available! No new lines where added." );
		return;
	}

	m_LineVertexPool[m_FreeVertexPos].Position = LineInfo.Start;
	m_LineVertexPool[m_FreeVertexPos + 1].Position = LineInfo.End;

	m_LineVertexPool[m_FreeVertexPos].Color = LineInfo.Color.ToVector4();
	m_LineVertexPool[m_FreeVertexPos + 1].Color = LineInfo.Color.ToVector4();

	m_NumPendingLines++;
	m_FreeVertexPos += 2;
}

void FBatchRenderer::LineBatchInfo::ClearLines()
{
	// Reset the line buffer. Only draw lines for one frame.
	ZeroMemory( &m_LineVertexPool, sizeof( m_LineVertexPool ) );
	m_NumPendingLines = 0;
	m_FreeVertexPos = 0;
}

void FBatchRenderer::LineBatchInfo::RenderLines( FCommandContext& CmdContext )
{
	m_LinesVB.Upload();
	CmdContext.SetPrimitiveTopologyType( PT_Linelist );
	CmdContext.BindVertexBuffer( 0, m_LinesVB );
	CmdContext.DrawInstanced( m_NumPendingLines * 2, 1, 0, 0 );
	CmdContext.EndDebugMarker();
}