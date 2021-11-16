#pragma once

#include "RendererFwd.h"

struct FPipelineStateDesc;

class RENDER_API FPipelineState
{
public:
	FPipelineState();
	~FPipelineState();
	
	void* GetNativePSO();

	void Initialize(const FPipelineStateDesc& Desc);

protected:
#if R_WITH_D3D12
	ID3D12PipelineState* m_pID3D12PipelineState;

#endif

};


//
// Iniline function implementations
//

FORCEINLINE void* FPipelineState::GetNativePSO()
{ 
#if R_WITH_D3D12
	return (void*)m_pID3D12PipelineState;
#endif
}
