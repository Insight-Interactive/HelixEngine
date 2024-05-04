// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "PCH.h"

#include "UnitTestMacros.h"

#include "TestsFwd.h"
#include "Logger.h"
#include "System.h"


int main(int argc, char* argv[])
{
	System::InitializePlatform();
	GUnitTestLogger.Initialize(TEXT("UnitTests"));
	GUnitTestLogger.SetUseConsole(true);

	//ContainerTests::Run();
	//MathTests::Run();
	TimerTests::Run();

	UT_LOG(Log, TEXT("All tests passed!"));
	return 0;
}