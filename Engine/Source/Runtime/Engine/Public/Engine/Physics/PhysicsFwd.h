// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#if PHYSICS_MODULE
#	define PHYSICS_API
#else
#	define PHYSICS_API
#endif

#if HE_ENABLE_LOGS
extern class Logger GPhysicsLogger;
/*
	Log a message to the console.
	@param Severety - The severity of the error.
	@param fmt - the format to display when writing to the console.
	@param __VA_ARGS__ - Optional arguments to supply when printing.
*/
#	define P_LOG(Severity, Fmt, ...) GPhysicsLogger.LogHelper(ELogSeverity::Severity, Fmt, HE_FILE, HE_FUNCTION, __LINE__, __VA_ARGS__);
#	define P_LOG_EXPLICIT_SEVERITY(Severity, Fmt, ...) GPhysicsLogger.LogHelper(Severity, Fmt, HE_FILE, HE_FUNCTION, __LINE__, __VA_ARGS__)

#else
#	define P_LOG(Severity, Fmt, ...)
#	define P_LOG_EXPLICIT_SEVERITY(Severity, Fmt, ...) 
#endif

#include "Engine/Physics/CommonMacros.h"
