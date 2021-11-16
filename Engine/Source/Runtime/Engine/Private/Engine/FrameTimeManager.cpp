#include "EnginePCH.h"

#include "Engine/FrameTimeManager.h"


/*static*/ double FTimer::SCpuTickDelta = 0.0;

// FTimer
//

FTimer::FTimer()
	: m_Ticks( 0.0 )
	, m_CurrentTick( 0 )
	, m_FrameStartTick( 0 )
	, m_IsInitilized( false )
{
	if (SCpuTickDelta == 0.0)
		SCpuTickDelta = double( System::QueryPerfFrequency() );
}

FTimer::~FTimer()
{

}

void FTimer::Record()
{
	if (!m_IsInitilized)
	{
		HE_LOG( Warning, TEXT( "Trying to tick a timer that has not been initilized! Timer measurements may be off by several seconds. Initialize the timer just before the first tick." ) );
	}
	m_CurrentTick = System::QueryPerfCounter();
}

void FTimer::Stop()
{
	m_Ticks = double(m_CurrentTick - m_FrameStartTick);
	m_FrameStartTick = m_CurrentTick;
}
