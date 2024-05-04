// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "System.h"

/*
	Standard timer that starts and stops recording explicitly.
*/
class FTimer
{
public:
	FTimer()
		: m_Ticks( 0 )
		, m_StartTick( 0 )
		, m_EndTick( 0 )
	{
		if (SCpuTickDelta == 0.0)
			SCpuTickDelta = 1.0 / double( System::QueryPerfFrequency() );
	}
	~FTimer()
	{
	}

	void Record();
	void Stop();

	double GetTimeSeconds() const;

	static double GetCpuTickDelta();

private:
	int64 m_Ticks;
	int64 m_StartTick;
	int64 m_EndTick;

	static double SCpuTickDelta;
};

/*
	Timer that starts when constructed and ends when destructed.
*/
class FScopeTimer : public FTimer
{
public:
	FScopeTimer()
	{
		Record();
	}
	~FScopeTimer()
	{
		Stop();
	}

};

// 
// Inline function implementations
//


// FTimer
//

FORCEINLINE void FTimer::Record()
{
	m_StartTick = System::QueryPerfCounter();
}

FORCEINLINE void FTimer::Stop()
{
	m_EndTick = System::QueryPerfCounter();
	m_Ticks = int64( m_EndTick - m_StartTick );
}

FORCEINLINE double FTimer::GetTimeSeconds() const
{
	return m_Ticks * SCpuTickDelta;
}

/*static*/ FORCEINLINE double FTimer::GetCpuTickDelta()
{
	return SCpuTickDelta;
}
