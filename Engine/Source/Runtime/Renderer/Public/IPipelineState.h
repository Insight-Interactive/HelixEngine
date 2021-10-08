#pragma once

#include "RendererFwd.h"

struct FPipelineStateDesc;

class RENDER_API FPipelineState
{
public:
	virtual ~FPipelineState()
	{
	}
	virtual void* GetNativePSO() = 0;

	virtual void Initialize(const FPipelineStateDesc& Desc) = 0;

protected:
	FPipelineState() {}

};
