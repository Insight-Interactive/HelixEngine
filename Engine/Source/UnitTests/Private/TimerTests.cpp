#include "PCH.h"

#include "Engine/Timer.h"

/*static*/ double FTimer::SCpuTickDelta = 0.0;

namespace TimerTests
{
	int Run()
	{

		FTimer timer;

		timer.Record();
		std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
		timer.Stop();
		double Time = timer.GetTimeSeconds();
		HE_ASSERT( Time >= 1 );

		return 0;
	}
}