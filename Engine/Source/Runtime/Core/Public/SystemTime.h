// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

class SystemTime
{
public:

    // Query the performance counter frequency
    static void Initialize( void );

    // Query the current value of the performance counter
    static int64 GetCurrentTick( void );

    static void BusyLoopSleep( float SleepTime );

    static inline double TicksToSeconds( int64 TickCount )
    {
        return TickCount * SCpuTickDelta;
    }

    static inline double TicksToMillisecs( int64 TickCount )
    {
        return TickCount * SCpuTickDelta * 1000.0;
    }

    static inline double TimeBetweenTicks( int64 tick1, int64 tick2 )
    {
        return TicksToSeconds( tick2 - tick1 );
    }

private:

    // The amount of time that elapses between ticks of the performance counter
    static double SCpuTickDelta;
};


class CpuTimer
{
public:

    CpuTimer()
    {
        m_StartTick = 0ll;
        m_ElapsedTicks = 0ll;
    }

    void Start()
    {
        if (m_StartTick == 0ll)
            m_StartTick = SystemTime::GetCurrentTick();
    }

    void Stop()
    {
        if (m_StartTick != 0ll)
        {
            m_ElapsedTicks += SystemTime::GetCurrentTick() - m_StartTick;
            m_StartTick = 0ll;
        }
    }

    void Reset()
    {
        m_ElapsedTicks = 0ll;
        m_StartTick = 0ll;
    }

    double GetTime() const
    {
        return SystemTime::TicksToSeconds( m_ElapsedTicks );
    }

private:

    int64 m_StartTick;
    int64 m_ElapsedTicks;
};
