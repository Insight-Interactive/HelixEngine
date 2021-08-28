#pragma once

#include "CoreFwd.h"

#if HE_DEBUG
/*
	Throws if an HRESULT fails. DO NOT put anything important, such as a whole function call, into the "Hr"
	parameter or they will be optimized out! Only pass the return value of a function into it.
*/
#	define ThrowIfFailedMsg(Hr, Msg)	if( FAILED(Hr) )	 { R_LOG(Error, TEXT("HRESULT Failed: %u - %s"), Hr, Msg); HE_ASSERT(false); }
#	define ASSERT_SUCCEEDED(Hr)			if( !SUCCEEDED(hr) ) { R_LOG(Error, TEXT("HRESULT Failed: %u"), Hr); HE_ASSERT(false); }
#else
/*
	Throws if an HRESULT fails. DO NOT put anything important, such as a whole function call, into the "Hr" 
	parameter or they will be optimized out! Only pass the return value of a function into it.
*/
#	define ThrowIfFailedMsg(Hr, Msg)
#	define ASSERT_SUCCEEDED(Hr)	
#endif

#define HE_D3D12_GPU_VIRTUAL_ADDRESS_NULL      ((D3D12_GPU_VIRTUAL_ADDRESS)0)
#define HE_D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN   ((D3D12_GPU_VIRTUAL_ADDRESS)-1)
