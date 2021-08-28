#pragma once

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "CommonEnums.h"


class RENDER_API IGpuResource
{
	friend class ICommandContext;
public:
	//
	// Getters/Setters
	//
	FORCEINLINE void SetUsageState(EResourceState NewState) { m_UsageState = NewState; }
	FORCEINLINE void SetTransitionState(EResourceState NewState) { m_TransitioningState = NewState; }
	FORCEINLINE void SetVersionID(uint32 NewID) { m_VersionID = NewID; }
	FORCEINLINE EResourceState GetUsageState() const { return m_UsageState; }
	FORCEINLINE EResourceState GetTransitionState() const { return m_TransitioningState; }
	FORCEINLINE uint32 GetVersionID() const { return m_VersionID; }

protected:
	IGpuResource(EResourceState CurrentState, EResourceState TransitionState)
		: m_UsageState(CurrentState)
		, m_TransitioningState(TransitionState)
		, m_VersionID(0u)
	{
	}
	IGpuResource()
		: m_UsageState(RS_Common)
		, m_TransitioningState(RESOURCE_STATE_INVALID)
		, m_VersionID(0u)
	{
	}
	virtual ~IGpuResource()
	{
	}
	virtual void Destroy() = 0;

	//virtual void InitializeGPUResource() = 0;

	EResourceState m_UsageState;
	EResourceState m_TransitioningState;
	uint32 m_VersionID;
};
