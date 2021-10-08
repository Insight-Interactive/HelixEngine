#include "EnginePCH.h"

#include "Engine/FrameTimeManager.h"

#include "System.h"


void FFrameTimeManager::Initialize()
{
	m_CpuTickDelta = 1.0 / (double)System::QueryPerfFrequency();
}

double FFrameTimeManager::GetCurrentTick()
{
	return (double)System::QueryPerfCounter();
}

void FFrameTimeManager::Tick( bool VSyncEnabled, bool LimitTo30Hz )
{
	int64 CurrentTick = System::QueryPerfCounter();

	if (VSyncEnabled)
		m_FrameTime = (LimitTo30Hz ? 2.f : 1.f) / 60.f;
	else
		m_FrameTime = TimeBetweenTicks( m_FrameStartTick, CurrentTick );

	m_FrameStartTick = CurrentTick;
}