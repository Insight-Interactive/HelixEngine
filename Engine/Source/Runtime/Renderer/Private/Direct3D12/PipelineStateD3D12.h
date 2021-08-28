#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "IPipelineState.h"


class RENDER_API PipelineStateD3D12 : public IPipelineState
{
public:
	PipelineStateD3D12();
	virtual ~PipelineStateD3D12();

	virtual void* GetNativePSO() override { return RCast<void*>(m_pID3D12PipelineState); }

	virtual void Initialize(const PipelineStateDesc& Desc) override;


protected:
	ID3D12PipelineState* m_pID3D12PipelineState;
};
