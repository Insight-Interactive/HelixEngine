// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "DataTypes.h"
#include "Cast.h"
#include "CoreFwd.h"

#include "ConsoleWindow.h"


class OutputBuffer final
{
public:
	OutputBuffer()
	{
	}
	~OutputBuffer()
	{
	}

	inline void operator << ( const char* Str )
	{
		m_Stream << Str;
	}

	void FlushBuffer();
	const char* GetStringBuffer();


private:
	std::stringstream m_Stream;

};


/*
	Valid log categories for logging instance.
*/
enum class ELogSeverity
{
	Log,
	Verbose,
	Warning,
	Error,
	Critical,
};

const uint32 kMakLoggerNameLength = 32;

class CORE_API Logger
{
public:
	Logger();
	~Logger();

	/*
		Initialize the logger.
	*/
	void Initialize(char* Name);

	/*
		Returns a reference to the global log buffer where log messages are stored.
	*/
	static OutputBuffer& GetOutputGlobalBuffer();

	/*
		Flush the log buffer and erase all log messages.
	*/
	static void FlushGlobalLogBuffer();

	/*
		Set the name of the logger.
		@param [in] Name - The new name of the logger.
	*/
	void SetLoggerName(char* Name);

	/*
		Returns the name of the logger.
		@returns A constant pointer to the name of the logger.
	*/
	FORCEINLINE const char* GetLoggerName();

	// Logger that will log output to the console window. Recommended that you dont call directly. 
	// Instead, use macro to wrap this so logs will be stripped from release builds.
	void LogHelper(ELogSeverity Severity, const char* Fmt, const char* File, const char* Function, int Line, ...);

private:
	char m_LoggerName[kMakLoggerNameLength];

	static ConsoleWindow s_ConsoleWindow;
	static bool s_UseConsole;
	static OutputBuffer SOutputBuffer;
};

const char* Logger::GetLoggerName()
{
	return RCast<const char*>(m_LoggerName);
}

#if HE_ENABLE_LOGS
#	define CreateLogger(LoggerInstance, Name) ( LoggerInstance ).Initialize( Name )
#else
#	define CreateLogger(LoggerInstance, Name)
#endif

//
// Inline function implementations
//

/*static*/ inline OutputBuffer& Logger::GetOutputGlobalBuffer()
{
	return SOutputBuffer;
}

/*static*/ inline void Logger::FlushGlobalLogBuffer()
{
	SOutputBuffer.FlushBuffer();
}