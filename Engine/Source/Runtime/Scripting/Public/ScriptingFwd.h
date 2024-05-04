// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#if SCRIPTING_MODULE
#	define SCRIPT_API
#else
#	define SCRIPT_API
#endif

#if HE_ENABLE_LOGS
extern class Logger GScriptEngineLogger;
extern class Logger GScriptLogger;
/*
	Log a message to the console.
	@param Severety - The severity of the error.
	@param fmt - the format to display when writing to the console.
	@param __VA_ARGS__ - Optional arguments to supply when printing.
*/
#	define S_LOG(Severity, Fmt, ...) GScriptLogger.LogHelper(ELogSeverity::Severity, Fmt, HE_FILE, HE_FUNCTION, __LINE__, __VA_ARGS__)
#	define S_LOG_EXPLICIT_SEVERITY(Severity, Fmt, ...) GScriptLogger.LogHelper(Severity, Fmt, HE_FILE, HE_FUNCTION, __LINE__, __VA_ARGS__)

#	define SE_LOG(Severity, Fmt, ...) GScriptEngineLogger.LogHelper(ELogSeverity::Severity, Fmt, HE_FILE, HE_FUNCTION, __LINE__, __VA_ARGS__)
#	define SE_LOG_EXPLICIT_SEVERITY(Severity, Fmt, ...) GScriptEngineLogger.LogHelper(Severity, Fmt, HE_FILE, HE_FUNCTION, __LINE__, __VA_ARGS__)

#else
#	define S_LOG(Severity, Fmt, ...)
#endif
