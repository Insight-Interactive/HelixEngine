#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "IPipelineState.h"


class RENDER_API FPipelineStateD3D12 : public FPipelineState
{
public:
	FPipelineStateD3D12();
	virtual ~FPipelineStateD3D12();

	virtual void* GetNativePSO() override { return RCast<void*>(m_pID3D12PipelineState); }

	virtual void Initialize(const FPipelineStateDesc& Desc) override;


protected:
	ID3D12PipelineState* m_pID3D12PipelineState;
};
