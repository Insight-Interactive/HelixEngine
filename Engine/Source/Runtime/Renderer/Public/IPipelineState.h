#pragma once

#include "RendererFwd.h"

struct PipelineStateDesc;

class RENDER_API IPipelineState
{
public:
	virtual ~IPipelineState()
	{
	}
	virtual void* GetNativePSO() = 0;

	virtual void Initialize(const PipelineStateDesc& Desc) = 0;

protected:
	IPipelineState() {}

};
