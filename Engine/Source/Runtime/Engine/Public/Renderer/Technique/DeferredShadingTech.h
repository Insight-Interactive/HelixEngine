// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "Renderer/RenderPass.h"

#include "CommonEnums.h"
#include "PixelBuffer.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "ColorBuffer.h"
#include "DepthBuffer.h"


class FColorBuffer;
class FDepthBuffer;
class FGpuResource;
class FRootSignature;
class FPipelineState;
class FCommandContext;

class FDeferredShadingTech
{
public:
	FDeferredShadingTech();
	~FDeferredShadingTech();

	void Initialize( FVector2 RenderResolution, EFormat SwapchainFormat );
	void UnInitialize();

	void ReloadPipeline();
	void SetOwningViewport( FViewportContext* pViewport );
	void Resize( const uint32& Width, const uint32& Height );

	void BindGeometryPass( FCommandContext& GfxContext, const FRect& Viewrect );
	void BindLightPass( FCommandContext& GfxContext, const FRect& Viewrect );
	void UnBindGeometryPass( FCommandContext& GfxContext );
	void UnBindLightPass( FCommandContext& GfxContext );

	void ClearGBuffers( FCommandContext& GfxContext, const FRect& Viewrect );

	enum EGBuffers
	{
		GB_Albedo = 0,
		GB_Normal = 1,
		GB_Roughness = 2,
		GB_Metallic = 3,
		GB_Specular = 4,

		GB_NumBuffers,
	};

	class GeometryPass : public FRenderPass
	{
		friend class FDeferredShadingTech;
	public:
		GeometryPass();
		virtual ~GeometryPass();

		void Initialize( const FVector2& RenderResolution );
		void UnInitialize();

		void ClearGBuffers( FCommandContext& GfxContext, const FRect& Viewrect );
		FColorBuffer& GetGBufferColorBuffer( EGBuffers GBuffer );
		void SetDepthBuffer( FDepthBuffer& DepthBuffer );

	private:
		virtual void Bind( FCommandContext& GfxContext, const FRect& Viewrect ) override;
		virtual void UnBind( FCommandContext& GfxContext ) override;
		virtual void ReloadPipeline() override;
		virtual void Resize( const uint32& Width, const uint32& Height ) override;

	protected:
		// Pipeline
		FRootSignature m_RS;
		FPipelineState m_PSO;
		// Resources
		FColorBuffer m_RenderTargets[GB_NumBuffers];
		// References
		FDepthBuffer* m_pSceneDepthBufferRef;
	};

	class LightPass : public FRenderPass
	{
		friend class FDeferredShadingTech;
	public:
		LightPass( FDeferredShadingTech::GeometryPass& GeometryPass );
		virtual ~LightPass();

		void Initialize( const FVector2& RenderResolution, EFormat SwapchainFormatTEMP );
		void UnInitialize();

		void SetDepthBuffer( FDepthBuffer& DepthBuffer );

	private:
		virtual void Bind( FCommandContext& GfxContext, const FRect& Viewrect ) override;
		virtual void UnBind( FCommandContext& GfxContext ) override;
		virtual void ReloadPipeline() override;
		virtual void Resize( const uint32& Width, const uint32& Height ) override;

	protected:
		FDeferredShadingTech::GeometryPass& m_GeometryPass;

		// Pipeline
		FRootSignature m_RS;
		FPipelineState m_PSO;

		// Resources
		EFormat m_RenderTargetFormat;
		// References
		FDepthBuffer* m_pSceneDepthBufferRef;
	};

	GeometryPass& GetGeometryPass();
	LightPass& GetLightPass();

private:
	GeometryPass m_GeometryPass;
	LightPass m_LightPass;

};


//
// Inline function implementations
//


// Deferred Shading Tech
//

FORCEINLINE void FDeferredShadingTech::ReloadPipeline()
{
	m_GeometryPass.ReloadPipeline();
	m_LightPass.ReloadPipeline();
}

FORCEINLINE void FDeferredShadingTech::SetOwningViewport( FViewportContext* pViewport )
{
	GetGeometryPass().SetOwningViewport( pViewport );
	GetLightPass().SetOwningViewport( pViewport );
}

FORCEINLINE void FDeferredShadingTech::Resize( const uint32& Width, const uint32& Height )
{
	GetGeometryPass().Resize( Width, Height );
	GetLightPass().Resize( Width, Height );
}

FORCEINLINE void FDeferredShadingTech::BindGeometryPass( FCommandContext& GfxContext, const FRect& Viewrect )
{
	GetGeometryPass().Bind( GfxContext, Viewrect );
}

FORCEINLINE void FDeferredShadingTech::BindLightPass( FCommandContext& GfxContext, const FRect& Viewrect )
{
	GetLightPass().Bind( GfxContext, Viewrect );
}

FORCEINLINE void FDeferredShadingTech::UnBindGeometryPass( FCommandContext& GfxContext )
{
	GetGeometryPass().UnBind( GfxContext );
}

FORCEINLINE void FDeferredShadingTech::UnBindLightPass( FCommandContext& GfxContext )
{
	GetLightPass().UnBind( GfxContext );
}

FORCEINLINE void FDeferredShadingTech::ClearGBuffers( FCommandContext& GfxContext, const FRect& Viewrect )
{
	GetGeometryPass().ClearGBuffers( GfxContext, Viewrect );
}

FORCEINLINE FDeferredShadingTech::GeometryPass& FDeferredShadingTech::GetGeometryPass()
{
	return m_GeometryPass;
}

FORCEINLINE FDeferredShadingTech::LightPass& FDeferredShadingTech::GetLightPass()
{
	return m_LightPass;
}


// Geometry Pass
//

FORCEINLINE FColorBuffer& FDeferredShadingTech::GeometryPass::GetGBufferColorBuffer( EGBuffers GBuffer )
{
	HE_ASSERT( GBuffer < GB_NumBuffers&& GBuffer > -1 ); // Index out of range!
	return m_RenderTargets[GBuffer];
}

FORCEINLINE void FDeferredShadingTech::GeometryPass::SetDepthBuffer( FDepthBuffer& DepthBuffer )
{
	m_pSceneDepthBufferRef = &DepthBuffer;
}


// Light Pass
//

FORCEINLINE void FDeferredShadingTech::LightPass::SetDepthBuffer( FDepthBuffer& DepthBuffer )
{
	m_pSceneDepthBufferRef = &DepthBuffer;
}
