#pragma once

#include "Renderer/RenderPass.h"

class FRootSignature;
class FPipelineState;
class FConstantBuffer;
enum EFormat;

class FPostProcessUber : public FRenderPass
{
public:
	FPostProcessUber();
	virtual ~FPostProcessUber();
	
	void Initialize( FVector2 RenderResolution );
	void UnInitialize();

	virtual void Bind( FCommandContext& GfxContext, const FRect& Viewrect ) override;
	virtual void UnBind( FCommandContext& GfxContext ) override;

	virtual void ReloadPipeline() override;

protected:

	// Pipeline
	FRootSignature* m_pRS;
	FPipelineState* m_pPSO;

	FConstantBuffer* m_pPostProcessSettings;

};