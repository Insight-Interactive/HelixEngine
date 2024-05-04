#include "CorePCH.h"

#include "SystemTime.h"


double SystemTime::SCpuTickDelta = 0.0;

// Query the performance counter frequency
void SystemTime::Initialize( void )
{
    LARGE_INTEGER frequency;
    assert( TRUE == QueryPerformanceFrequency( &frequency ) );
    SCpuTickDelta = 1.0 / static_cast<double>(frequency.QuadPart);
}

// Query the current value of the performance counter
int64 SystemTime::GetCurrentTick( void )
{
    LARGE_INTEGER currentTick;
    assert( TRUE == QueryPerformanceCounter( &currentTick ) );
    return static_cast<int64_t>(currentTick.QuadPart);
}

void SystemTime::BusyLoopSleep( float SleepTime )
{
    int64_t finalTick = (int64_t)((double)SleepTime / SCpuTickDelta) + GetCurrentTick();
    while (GetCurrentTick() < finalTick);
}
