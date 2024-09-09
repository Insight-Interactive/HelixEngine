// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"

struct FPipelineStateDesc;

class FPipelineState
{
public:
	FPipelineState();
	~FPipelineState();
	FPipelineState( const FPipelineState& Other );
	FPipelineState& operator = ( const FPipelineState& Other );

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

FORCEINLINE FPipelineState::FPipelineState( const FPipelineState& Other )
{
	*this = Other;
}

FORCEINLINE FPipelineState& FPipelineState::operator = ( const FPipelineState& Other )
{
#if R_WITH_D3D12
	this->m_pID3D12PipelineState = Other.m_pID3D12PipelineState;
#endif

	return *this;
}

FORCEINLINE void* FPipelineState::GetNativePSO()
{ 
#if R_WITH_D3D12
	return (void*)m_pID3D12PipelineState;
#endif
}
