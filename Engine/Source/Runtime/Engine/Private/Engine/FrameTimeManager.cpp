#include "EnginePCH.h"

#include "Engine/FrameTimeManager.h"

#include "System.h"


void FrameTimeManager::Initialize()
{
	m_CpuTickDelta = 1.0 / (double)System::QueryPerformanceFrequency();
}

double FrameTimeManager::GetCurrentTick()
{
	return (double)System::QueryPerformanceCounter();
}

void FrameTimeManager::Tick( bool VSyncEnabled, bool LimitTo30Hz )
{
	int64 CurrentTick = System::QueryPerformanceCounter();

	if (VSyncEnabled)
		m_FrameTime = (LimitTo30Hz ? 2.f : 1.f) / 60.f;
	else
		m_FrameTime = TimeBetweenTicks( m_FrameStartTick, CurrentTick );

	m_FrameStartTick = CurrentTick;
}