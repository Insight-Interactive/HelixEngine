#pragma once

#if HE_DEBUG
	extern class Logger GGameLogger;
	/*
		Log a message to the console.
		@param Severety - The severity of the error.
		@param fmt - the format to display when writing to the console.
		@param __VA_ARGS__ - Optional arguments to supply when printing.
	*/
#	define GM_LOG(Severity, Fmt, ...) GGameLogger.LogHelper(ELogSeverity::Severity, Fmt, HE_FILE, HE_FUNCTION, __LINE__, __VA_ARGS__);

#else
#	define GM_LOG(Severity, Fmt, ...)
#endif