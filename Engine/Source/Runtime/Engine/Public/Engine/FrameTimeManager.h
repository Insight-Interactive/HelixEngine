#pragma once


/*
	Standard timer that starts and stops recording explicitly.
*/
class FTimer
{
public:
	FTimer();
	~FTimer();

	void Record();
	void Stop();

	double GetTimeMicroSeconds() const;
	double GetTimeMiliSeconds() const;
	double GetTimeSeconds() const;

	static double GetCpuTickDelta();

private:
	double m_Ticks;
	int64 m_CurrentTick;
	int64 m_FrameStartTick;

	static double SCpuTickDelta;
};

/*
	Timer that measures the timer between each Tick() call.
*/
class FFrameTimer : public FTimer
{
public:
	FFrameTimer()
	{
	}
	~FFrameTimer()
	{
	}

	void Tick();

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

FORCEINLINE double FTimer::GetTimeMicroSeconds() const
{
	return ( m_Ticks * 1000000) / SCpuTickDelta;
}

FORCEINLINE double FTimer::GetTimeMiliSeconds() const
{
	return m_Ticks * (1 / SCpuTickDelta);
}

FORCEINLINE double FTimer::GetTimeSeconds() const
{
	return m_Ticks / SCpuTickDelta;
}

/*static*/ FORCEINLINE double FTimer::GetCpuTickDelta()
{
	return SCpuTickDelta;
}


// FFrameTimer
//

FORCEINLINE void FFrameTimer::Tick()
{
	Record();
	Stop();
}
