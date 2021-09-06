#pragma once


class FrameTimeManager
{
public:
	FrameTimeManager()
		: m_FrameTime( 0.f )
		, m_FrameStartTick( 0 )
		, m_CpuTickDelta( 0 )
	{
	}
	~FrameTimeManager()
	{
	}

	void Initialize();
	void Update( bool VSyncEnabled, bool LimitTo30Hz );

	double GetFrameTime() const;
	double GetCurrentTick();

private:
	double TicksToSeconds( int64_t TickCount );
	double TicksToMillisecs( int64_t TickCount );
	double TimeBetweenTicks( int64_t tick1, int64_t tick2 );

private:
	double m_FrameTime;
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

inline double FrameTimeManager::TicksToSeconds( int64 TickCount )
{
	return (double)TickCount * m_CpuTickDelta;
}

inline double FrameTimeManager::TicksToMillisecs( int64 TickCount )
{
	return (double)TickCount * m_CpuTickDelta * 1000.0;
}

inline double FrameTimeManager::TimeBetweenTicks( int64 tick1, int64 tick2 )
{
	return TicksToSeconds( tick2 - tick1 );
}
