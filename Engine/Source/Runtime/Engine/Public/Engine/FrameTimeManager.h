#pragma once


class FrameTimeManager
{
public:
	FrameTimeManager()
		: m_FrameTime( 0.0 )
		, m_FrameRate( 0.0 )
		, m_FrameStartTick( 0 )
		, m_CpuTickDelta( 0.0 )
	{
	}
	~FrameTimeManager()
	{
	}

	void Initialize();
	void Update( bool VSyncEnabled, bool LimitTo30Hz );

	double GetFrameTime() const;
	double GetFrameRate() const;
	double GetCurrentTick();

private:
	double TicksToSeconds( int64_t TickCount );
	double TicksToMillisecs( int64_t TickCount );
	double TimeBetweenTicks( int64_t Tick1, int64_t Tick2 );

private:
	double m_FrameTime;
	double m_FrameRate;
	int64 m_FrameStartTick;
	double m_CpuTickDelta;

};

// 
// Inline function implementations
//

inline double FrameTimeManager::GetFrameTime() const
{
	return m_FrameTime;
}

inline double FrameTimeManager::GetFrameRate() const
{
	return (m_FrameTime == 0.f) ? 0.f : 1.f / m_FrameTime;
}

inline double FrameTimeManager::TicksToSeconds( int64 TickCount )
{
	return (double)TickCount * m_CpuTickDelta;
}

inline double FrameTimeManager::TicksToMillisecs( int64 TickCount )
{
	return (double)TickCount * m_CpuTickDelta * 1000.0;
}

inline double FrameTimeManager::TimeBetweenTicks( int64 Tick1, int64 Tick2 )
{
	return TicksToSeconds( Tick2 - Tick1 );
}
