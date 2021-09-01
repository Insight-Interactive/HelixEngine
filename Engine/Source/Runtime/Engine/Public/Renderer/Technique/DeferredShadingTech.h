#pragma once

#include "CommonEnums.h"
#include "IPixelBuffer.h"

#include "Renderer/FRenderPass.h"


class ICommandContext;
class IColorBuffer;
class IDepthBuffer;
class IRootSignature;
class IPipelineState;
class IGpuResource;


class DeferredShadingTech
{
public:
	DeferredShadingTech();
	~DeferredShadingTech();

	void Initialize(FVector2 RenderResolution, EFormat SwapchainFormat);
	void UnInitialize();


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

		virtual void Bind( ICommandContext& GfxContext, const Rect& Viewrect ) override;
		virtual void UnBind( ICommandContext& GfxContext ) override;

		inline IColorBuffer& operator[]( size_t Index );
		inline IColorBuffer& GetGBufferColorBuffer( EGBuffers GBuffer ) const;
		inline IDepthBuffer* GetSceneDepthBuffer() const;

		EFormat GetDepthFormat() const;

	protected:
		// Pipeline
		IRootSignature* m_pRS;
		IPipelineState* m_pPSO;
		// Resources
		IColorBuffer* m_RenderTargets[GB_NumBuffers];
		IDepthBuffer* m_pDepthBuffer;
		IGpuResource* m_pDepthBufferGPUResource;
	};

	class LightPass : public FRenderPass
	{
	public:
		LightPass( DeferredShadingTech::GeometryPass& GeometryPass );
		virtual ~LightPass();
		
		void Initialize(const FVector2& RenderResolution, EFormat SwapchainFormatTEMP);
		void UnInitialize();

		virtual void Bind(ICommandContext& GfxContext, const Rect& Viewrect) override;
		virtual void UnBind(ICommandContext& GfxContext) override;

	protected:
		DeferredShadingTech::GeometryPass& m_GeometryPass;

		// Pipeline
		IRootSignature* m_pRS;
		IPipelineState* m_pPSO;

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

inline DeferredShadingTech::GeometryPass& DeferredShadingTech::GetGeometryPass()
{
	return m_GeometryPass;
}

inline DeferredShadingTech::LightPass& DeferredShadingTech::GetLightPass()
{
	return m_LightPass;
}


// Geometry Pass
//

inline IColorBuffer& DeferredShadingTech::GeometryPass::operator[]( size_t Index )
{
	return GetGBufferColorBuffer( (EGBuffers)Index );
}

inline IColorBuffer& DeferredShadingTech::GeometryPass::GetGBufferColorBuffer( EGBuffers GBuffer ) const
{
	HE_ASSERT( GBuffer < GB_NumBuffers&& GBuffer > -1 ); // Index out of range!
	return (*m_RenderTargets[GBuffer]);
}

inline IDepthBuffer* DeferredShadingTech::GeometryPass::GetSceneDepthBuffer() const
{
	return m_pDepthBuffer;
}