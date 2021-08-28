#pragma once

#include "StandaloneRenderer/FRenderPass.h"

class IRootSignature;
class IPipelineState;
class IConstantBuffer;
enum EFormat;

class PostProcesssUber : public FRenderPass
{
public:
	PostProcesssUber();
	virtual ~PostProcesssUber();
	
	void Initialize( FVector2 RenderResolution );
	void UnInitialize();

	virtual void Bind( ICommandContext& GfxContext, const Rect& Viewrect ) override;
	virtual void UnBind( ICommandContext& GfxContext ) override;

protected:

	// Pipeline
	IRootSignature* m_pRS;
	IPipelineState* m_pPSO;

	IConstantBuffer* m_pPostProcessSettings;

};