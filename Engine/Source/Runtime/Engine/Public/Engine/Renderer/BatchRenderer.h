// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"

#include "Color.h"
#include "PipelineState.h"
#include "RootSignature.h"
#include "VertexBuffer.h"

static const uint32 kMaxLineRenders = 256;
static const uint32 kMaxNumLineVerts = kMaxLineRenders * 2;

struct FDebugLineRenderInfo
{
	FVector3 Start;
	FVector3 End;
	FColor Color;
	bool IgnoreDepth;

	FORCEINLINE	FDebugLineRenderInfo()
		: Start( 0.f, 0.f, 0.f )
		, End( 1.f, 0.f, 0.f )
		, Color( 1.f, 0.f, 0.f )
		, IgnoreDepth( false )
	{
	}
};

struct FDebugLineVertex
{
	FVector3 Position;
	FVector4 Color;
};

class FColorBuffer;
class FDepthBuffer;
class FCommandContext;

class FBatchRenderer
{
public:
	FBatchRenderer();
	~FBatchRenderer();

	void Initialize( FColorBuffer& RenderTarget, FDepthBuffer& DepthBufferTarget );
	void UnInitialize();

	void SetRenderTarget( FColorBuffer& RenderTarget );
	void SetDepthBuffer( FDepthBuffer& DepthBuffer );

	void PreRender( FCommandContext& CmdContext );
	void Render( FCommandContext& CmdContext );

	void SubmitLineRenderRequest( const FDebugLineRenderInfo& LineInfo );
	void ClearLines();

protected:
	struct LineBatchInfo
	{
		LineBatchInfo();
		~LineBatchInfo() = default;

		void Initialize();
		void RenderLines( FCommandContext& CmdContext );
		void AddLine( const FDebugLineRenderInfo& LineInfo );
		bool AnyLines() { return m_NumPendingLines > 0; }
		void ClearLines();

		FDynamicVertexBuffer m_LinesVB;
		FDebugLineVertex m_LineVertexPool[kMaxNumLineVerts];
		uint32 m_NumPendingLines;
		uint32 m_FreeVertexPos;
	};
	LineBatchInfo m_DepthLines;
	LineBatchInfo m_NoDepthLines;

	FRootSignature m_RS;
	FPipelineState m_DepthPSO;
	FPipelineState m_NoDepthPSO;

	FColorBuffer* m_pRenderTarget;
	FDepthBuffer* m_pDepthBuffer;

};
