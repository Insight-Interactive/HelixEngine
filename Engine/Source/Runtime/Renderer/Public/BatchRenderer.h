// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"

#include "Color.h"
#include "PipelineState.h"
#include "RootSignature.h"
#include "VertexBuffer.h"

static const uint32 kMaxLineRenders = 20;
static const uint32 kMaxNumLineVerts = kMaxLineRenders * 2;

struct FDebugLineRenderInfo
{
	FVector3 Start;
	FVector3 End;
	FColor Color;
	float Thickness;
	float Lifetime;

	FORCEINLINE	FDebugLineRenderInfo()
		: Start( 0.f, 0.f, 0.f )
		, End( 1.f, 0.f, 0.f )
		, Color( 1.f, 0.f, 0.f )
		, Thickness( 1.f )
		, Lifetime( 5.f )
	{
	}
};

struct FDebugLineVertex
{
	FVector3 Position;
	FVector4 Color;
	float Thickness;
	float Lifetime;
};

class FColorBuffer;
class FDepthBuffer;
class FCommandContext;

class RENDER_API FBatchRenderer
{
public:
	FBatchRenderer();
	~FBatchRenderer();

	void Initialize( FColorBuffer& RenderTarget, FDepthBuffer& DepthBufferTarget );
	void UnInitialize();

	void SetRenderTarget( FColorBuffer& RenderTarget );
	void SetDepthBuffer( FDepthBuffer& DepthBuffer );

	void Tick( float DeltaTime );
	void PreRender( FCommandContext& CmdContext );
	void Render( FCommandContext& CmdContext );

	void SubmitLineRenderRequest( const FDebugLineRenderInfo& LineInfo );

protected:
	FDynamicVertexBuffer m_LinesVB;
	FDebugLineVertex m_LineVertexPool[kMaxNumLineVerts];
	uint32 m_NumPendingLines;

	FRootSignature m_RS;
	FPipelineState m_PSO;

	FColorBuffer* m_pRenderTarget;
	FDepthBuffer* m_pDepthBuffer;

};
