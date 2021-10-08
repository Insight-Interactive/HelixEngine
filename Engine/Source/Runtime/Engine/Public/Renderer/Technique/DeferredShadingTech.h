#pragma once

#include "CommonEnums.h"
#include "IPixelBuffer.h"

#include "Renderer/RenderPass.h"


class FCommandContext;
class FColorBuffer;
class FDepthBuffer;
class FRootSignature;
class FPipelineState;
class FGpuResource;


class FDeferredShadingTech
{
public:
	FDeferredShadingTech();
	~FDeferredShadingTech();

	void Initialize(FVector2 RenderResolution, EFormat SwapchainFormat);
	void UnInitialize();

	void ReloadPipeline();

	enum EGBuffers
	{
		GB_Albedo = 0,
		GB_Normal = 1,
		GB_Position = 2,

		GB_NumBuffers = 3,
	};

	class GeometryPass : public FRenderPass
	{
	public:
		GeometryPass();
		virtual ~GeometryPass();

		void Initialize(const FVector2& RenderResolution);
		void UnInitialize();

		virtual void Bind( FCommandContext& GfxContext, const FRect& Viewrect ) override;
		virtual void UnBind( FCommandContext& GfxContext ) override;

		virtual void ReloadPipeline() override;

		inline FColorBuffer& operator[]( size_t Index );
		inline FColorBuffer& GetGBufferColorBuffer( EGBuffers GBuffer ) const;
		inline FDepthBuffer* GetSceneDepthBuffer() const;

		EFormat GetDepthFormat() const;

	protected:
		// Pipeline
		FRootSignature* m_pRS;
		FPipelineState* m_pPSO;
		// Resources
		FColorBuffer* m_RenderTargets[GB_NumBuffers];
		FDepthBuffer* m_pDepthBuffer;
		FGpuResource* m_pDepthBufferGPUResource;
	};

	class LightPass : public FRenderPass
	{
	public:
		LightPass( FDeferredShadingTech::GeometryPass& GeometryPass );
		virtual ~LightPass();
		
		void Initialize(const FVector2& RenderResolution, EFormat SwapchainFormatTEMP);
		void UnInitialize();

		virtual void Bind(FCommandContext& GfxContext, const FRect& Viewrect) override;
		virtual void UnBind(FCommandContext& GfxContext) override;

		virtual void ReloadPipeline() override;

	protected:
		FDeferredShadingTech::GeometryPass& m_GeometryPass;

		// Pipeline
		FRootSignature* m_pRS;
		FPipelineState* m_pPSO;

		EFormat m_RenderTargetFormat;
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

inline void FDeferredShadingTech::ReloadPipeline()
{
	m_GeometryPass.ReloadPipeline();
	m_LightPass.ReloadPipeline();
}

inline FDeferredShadingTech::GeometryPass& FDeferredShadingTech::GetGeometryPass()
{
	return m_GeometryPass;
}

inline FDeferredShadingTech::LightPass& FDeferredShadingTech::GetLightPass()
{
	return m_LightPass;
}


// Geometry Pass
//

inline FColorBuffer& FDeferredShadingTech::GeometryPass::operator[]( size_t Index )
{
	return GetGBufferColorBuffer( (EGBuffers)Index );
}

inline FColorBuffer& FDeferredShadingTech::GeometryPass::GetGBufferColorBuffer( EGBuffers GBuffer ) const
{
	HE_ASSERT( GBuffer < GB_NumBuffers&& GBuffer > -1 ); // Index out of range!
	return (*m_RenderTargets[GBuffer]);
}

inline FDepthBuffer* FDeferredShadingTech::GeometryPass::GetSceneDepthBuffer() const
{
	return m_pDepthBuffer;
}