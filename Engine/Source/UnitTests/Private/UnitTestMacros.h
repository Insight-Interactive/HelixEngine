#pragma once

#include "Engine/EngineMacros.h" // For HE_LOG's

extern class Logger GUnitTestLogger;

/*
	Log a message to the console.
	@param Severety - The severity of the error.
	@param fmt - the format to display when writing to the console.
	@param __VA_ARGS__ - Optional arguments to supply when printing.
*/
#define UT_LOG(Severity, Fmt, ...) GUnitTestLogger.LogHelper(ELogSeverity::Severity, Fmt, HE_FILE, HE_FUNCTION, __LINE__, __VA_ARGS__);
