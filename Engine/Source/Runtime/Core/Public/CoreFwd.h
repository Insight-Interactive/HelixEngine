// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once


//#if HE_DEBUG
//#	define HE_ENABLE_LOGS		1
//#	define HE_DEBUG_THREADS		1
//#	define HE_ENABLE_ASSERTS	1
//#	define HE_DEBUG_FILESYSTEM	1
//#else
//#	define HE_ENABLE_LOGS		0
//#	define HE_DEBUG_THREADS		0
//#	define HE_ENABLE_ASSERTS	0
//#	define HE_DEBUG_FILESYSTEM	0
//#endif

#include "DataTypes.h"

#if CORE_MODULE
#	define CORE_API //__declspec(dllexport)
#else
#	define CORE_API //__declspec(dllimport)
#endif

#include "DebugAssert.h"
#include "Logger.h"
#include "Cast.h"
