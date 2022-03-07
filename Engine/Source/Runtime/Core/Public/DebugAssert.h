// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "DataTypes.h"


#if HE_ENABLE_ASSERTS
#	if HE_COMPILE_MSVC
#		define HE_DEBUG_BREAK() __debugbreak()
#	else
#		define HE_DEBUG_BREAK() _CrtDbgbreak()
#	endif

#else
#	define HE_DEBUG_BREAK()

#endif

#if HE_ENABLE_ASSERTS
#	define HE_ASSERT(Expr) if( (Expr) != true) { HE_DEBUG_BREAK(); }
#	define HE_PANIC_BREAK() HE_ASSERT( false )

#else
#	define HE_ASSERT(Expr)
#	define HE_PANIC_BREAK()

#endif


