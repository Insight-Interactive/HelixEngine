// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "CommonEnums.h"
#include "CommandManager.h"


extern FCommandManager GCommandManager;

class RENDER_API FGpuResource
{
	friend class FCommandContext;
public:
	FGpuResource();
	FGpuResource( void* pApiResource, EResourceState CurrentState );
	FGpuResource( EResourceState CurrentState, EResourceState TransitionState );
	virtual ~FGpuResource()
	{
		Destroy();
	}

	void Destroy();

	//
	// Getters/Setters
	//
	FORCEINLINE void SetVersionID(uint32 NewID)						{ m_VersionID = NewID; }
	FORCEINLINE void SetUsageState(EResourceState NewState)			{ m_UsageState = NewState; }
	FORCEINLINE void SetTransitionState(EResourceState NewState)	{ m_TransitioningState = NewState; }
	FORCEINLINE uint32 GetVersionID()						const	{ return m_VersionID; }
	FORCEINLINE EResourceState GetUsageState()				const	{ return m_UsageState; }
	FORCEINLINE EResourceState GetTransitionState()			const	{ return m_TransitioningState; }


	//
	// Getters/Setters
	//
	void* GetResource() const;
	uint64 GetGPUVirtualAddress() const;
	void* operator -> ();
	const void* operator->() const;
	void** GetAddressOf();

protected:
	EResourceState m_UsageState;
	EResourceState m_TransitioningState;
	uint32 m_VersionID;

#if R_WITH_D3D12
	Microsoft::WRL::ComPtr<ID3D12Resource> m_pID3D12Resource;
	D3D12_GPU_VIRTUAL_ADDRESS m_GpuVirtualAddress;

#endif

};


//
// Inline function implementation
//

FORCEINLINE FGpuResource::FGpuResource()
	: m_UsageState( RS_Common )
	, m_TransitioningState( RESOURCE_STATE_INVALID )
	, m_VersionID( 0u )
#if R_WITH_D3D12
	, m_pID3D12Resource( NULL )
	, m_GpuVirtualAddress( HE_D3D12_GPU_VIRTUAL_ADDRESS_NULL )
#endif
{
}

FORCEINLINE FGpuResource::FGpuResource( void* pApiResource, EResourceState CurrentState )
	: m_UsageState( CurrentState )
	, m_TransitioningState( RESOURCE_STATE_INVALID )
	, m_VersionID( 0u )
#if R_WITH_D3D12
	, m_pID3D12Resource( (ID3D12Resource*)pApiResource )
	, m_GpuVirtualAddress( HE_D3D12_GPU_VIRTUAL_ADDRESS_NULL )
#endif
{
}

FORCEINLINE FGpuResource::FGpuResource( EResourceState CurrentState, EResourceState TransitionState )
	: m_UsageState( CurrentState )
	, m_TransitioningState( TransitionState )
	, m_VersionID( 0u )
#if R_WITH_D3D12
	, m_pID3D12Resource( NULL )
	, m_GpuVirtualAddress( HE_D3D12_GPU_VIRTUAL_ADDRESS_NULL )
#endif
{
}

FORCEINLINE void FGpuResource::Destroy()
{
	GCommandManager.IdleGpu();
#if R_WITH_D3D12
	m_pID3D12Resource	= nullptr;
	m_GpuVirtualAddress = HE_D3D12_GPU_VIRTUAL_ADDRESS_NULL;
	++m_VersionID;
#endif
}


FORCEINLINE void* FGpuResource::GetResource() const
{ 
#if R_WITH_D3D12
	return (void*)m_pID3D12Resource.Get();
#endif
}

FORCEINLINE uint64 FGpuResource::GetGPUVirtualAddress() const
{ 
#if R_WITH_D3D12
	return (uint64)m_GpuVirtualAddress;
#endif
}

FORCEINLINE void* FGpuResource::operator -> ()
{ 
#if R_WITH_D3D12
	return (void*)m_pID3D12Resource.Get();
#endif
}

FORCEINLINE const void* FGpuResource::operator->() const
{ 
#if R_WITH_D3D12
	return (void*)m_pID3D12Resource.Get();
#endif
}

FORCEINLINE void** FGpuResource::GetAddressOf()
{ 
#if R_WITH_D3D12
	return (void**)m_pID3D12Resource.GetAddressOf();
#endif
}
