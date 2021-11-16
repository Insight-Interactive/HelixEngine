// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"
HE_DISABLE_MSVC_WARNINGS

#include "Engine/HelixEngine.h"


HE_ENTRY_POINT()
{
	GuardedMain( CmdLine );

	return 0;
}

HE_RESTORE_MSVC_WARNINGS
