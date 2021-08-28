// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "PCH.h"

#include "UnitTestMacros.h"

#include "TestsFwd.h"
#include "Logger.h"

int main(int argc, char* argv[])
{
	GUnitTestLogger.Initialize(TEXT("UnitTests"));
	GUnitTestLogger.SetUseConsole(true);

	ContainerTests::Run();
	MathTests::Run();

	UT_LOG(Log, TEXT("All tests passed!"));
	return 0;
}