// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "DataTypes.h"
#include "Cast.h"
#include "CoreFwd.h"

/*
	Valid log categories for HE_LOG
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
	void Initialize(TChar* Name);

	/*
		Set the name of the logger.
		@param [in] Name - The new name of the logger.
	*/
	void SetLoggerName(TChar* Name);

	/*
		Weather to use a on screen console window. More useful for unit tests. (Does not launch a console window)
		@param [in] bUse - Weather to print to a onscreen console window or not.
	*/
	FORCEINLINE void SetUseConsole(bool bUse);

	/*
		Returns true if the logger is currently allowed to print to a windowed console using printf().
	*/
	FORCEINLINE bool GetShouldUseConsole();

	/*
		Returns the name of the logger.
		@returns A constant pointer to the name of the logger.
	*/
	FORCEINLINE const TChar* GetLoggerName();

	// Logger that will log output to the console window. Recommended that you dont call directly. 
	// Instead, use IE_LOG so logs will be stripped from release builds.
	void LogHelper(ELogSeverity Severity, const TChar* Fmt, const TChar* File, const TChar* Function, int Line, ...);

private:
	TChar m_LoggerName[kMakLoggerNameLength];
	bool m_UseConsole;
};

bool Logger::GetShouldUseConsole()
{
	return m_UseConsole;
}

void Logger::SetUseConsole(bool bUse)
{
	m_UseConsole = bUse;
}

const TChar* Logger::GetLoggerName()
{
	return RCast<const TChar*>(m_LoggerName);
}

#if HE_ENABLE_LOGS
#	define CreateLogger(Logger, Name) (Logger).Initialize( TEXT(Name) )
#else
#	define CreateLogger(Logger, Name)
#endif
