#include "CorePCH.h"

#include "Logger.h"

#include "CriticalSection.h"


// Output Buffer
//

void OutputBuffer::FlushBuffer()
{
	m_Stream.str( "" );
}

const char* OutputBuffer::GetStringBuffer()
{
	return m_Stream.str().c_str();
}


// Logger
//

OutputBuffer Logger::SOutputBuffer;
CriticalSection GStreamGuard;

bool Logger::s_UseConsole = true;
ConsoleWindow Logger::s_ConsoleWindow;

Logger::Logger()
{
	ZeroMemory( m_LoggerName, sizeof( m_LoggerName ) );
}

Logger::~Logger()
{
}

void Logger::Initialize(char* Name)
{
	SetLoggerName(Name);
	LogHelper(ELogSeverity::Log, "Logger initialized with name: %s", HE_FILE, HE_FUNCTION, __LINE__, GetLoggerName());

	if (s_UseConsole && !s_ConsoleWindow.IsValid())
	{
		ConsoleWindowDesc WindowDesc;
		WindowDesc.CanClose = false;
		WindowDesc.BufferDimsX = 700;
		WindowDesc.BufferDimsY = 320;
		WindowDesc.WindowDimsX = 170;
		WindowDesc.WindowDimsY = 42;
		s_ConsoleWindow.Create( WindowDesc );
	}
}

void Logger::SetLoggerName( char* Name)
{
	CopyMemory(
		m_LoggerName,
		Name,
		kMakLoggerNameLength * sizeof( char )
	);
}

void Logger::LogHelper(ELogSeverity Severity, const char* Fmt, const char* File, const char* Function, int Line, ...)
{
	const uint32 kMaxLogLength = 1024;

	char TraceBuffer[kMaxLogLength];
	char OutputBuffer[kMaxLogLength];

	// Initialize the output message buffer.
	va_list args;
	va_start(args, Line); // Start capturing arguments after the 'Line' parameter in the method.
	{
		vsnprintf_s(OutputBuffer, sizeof(OutputBuffer), Fmt, args);
	}
	va_end(args);

	switch (Severity)
	{
	case ELogSeverity::Log:
		sprintf_s(TraceBuffer, "[%s][Log] - ", GetLoggerName());
		break;
	case ELogSeverity::Verbose:
		sprintf_s(TraceBuffer, "[%s][Verbose][%s-%s-%i] - ", GetLoggerName(), File, Function, Line);
		break;
	case ELogSeverity::Warning:
		sprintf_s(TraceBuffer, "[%s][Warning] - ", GetLoggerName());
		break;
	case ELogSeverity::Error:
		sprintf_s(TraceBuffer, "[%s][Error] - ", GetLoggerName());
		break;
	case ELogSeverity::Critical:
		sprintf_s(TraceBuffer, "[%s][Critical] - ", GetLoggerName());
		break;
	default:
		sprintf_s(TraceBuffer, "Invalid log severity given to logger. Choose one option from ELogSeverity enum.");
		break;
	}

	// Ensure the buffers are null terminated.
	TraceBuffer[kMaxLogLength - 1] = '\0';
	OutputBuffer[kMaxLogLength - 1] = '\0';

	// Print message
	{
		ScopedCriticalSection StreamGuard( GStreamGuard );
		SOutputBuffer << TraceBuffer;
		SOutputBuffer << OutputBuffer;
		SOutputBuffer << "\n";
	}
	if (s_UseConsole)
	{
		printf( TraceBuffer );
		printf( OutputBuffer );
		printf( "\n" );
	}
//#if HE_COMPILE_MSVC
//	else if ( IsDebuggerPresent() )
//	{
//		OutputDebugString(TraceBuffer);
//		OutputDebugString(OutputBuffer);
//		OutputDebugString(TEXT("\n"));
//	}
//#endif
}
