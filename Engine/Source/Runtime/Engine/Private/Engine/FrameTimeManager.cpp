// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/FrameTimeManager.h"


/*static*/ double FTimer::SCpuTickDelta = 0.0;

// FTimer
//

FTimer::FTimer()
	: m_Ticks( 0.0 )
	, m_CurrentTick( 0 )
	, m_FrameStartTick( 0 )
{
	if (SCpuTickDelta == 0.0)
		SCpuTickDelta = double( System::QueryPerfFrequency() );
}

FTimer::~FTimer()
{

}

void FTimer::Record()
{
	m_CurrentTick = System::QueryPerfCounter();
}

void FTimer::Stop()
{
	m_Ticks = double(m_CurrentTick - m_FrameStartTick);
	m_FrameStartTick = m_CurrentTick;
}
