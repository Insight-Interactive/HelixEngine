#include "CorePCH.h"

#include "Logger.h"

#include "CriticalSection.h"


// Output Buffer
//

void OutputBuffer::FlushBuffer()
{
	m_Stream.str( TEXT( "" ) );
}

HName OutputBuffer::GetStringBuffer()
{
	return m_Stream.str();
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

void Logger::Initialize(TChar* Name)
{
	SetLoggerName(Name);
	LogHelper(ELogSeverity::Log, TEXT("Logger initialized with name: %s"), HE_FILE, HE_FUNCTION, __LINE__, GetLoggerName());

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

void Logger::SetLoggerName(TChar* Name)
{
	CopyMemory(
		m_LoggerName,
		Name,
		kMakLoggerNameLength * sizeof(TChar)
	);
}

void Logger::LogHelper(ELogSeverity Severity, const TChar* Fmt, const TChar* File, const TChar* Function, int Line, ...)
{
	const uint32 kMaxLogLength = 1024;

	TChar TraceBuffer[kMaxLogLength];
	TChar OutputBuffer[kMaxLogLength];

	// Initialize the output message buffer.
	va_list args;
	va_start(args, Line); // Start capturing arguments after the 'Line' parameter in the method.
	{
		VSPrintBuffer(OutputBuffer, sizeof(OutputBuffer), Fmt, args);
	}
	va_end(args);

	switch (Severity)
	{
	case ELogSeverity::Log:
		PrintBuffer(TraceBuffer, TEXT("[%s][Log] - "), GetLoggerName());
		break;
	case ELogSeverity::Verbose:
		PrintBuffer(TraceBuffer, TEXT("[%s][Verbose][%s-%s-%i] - "), GetLoggerName(), File, Function, Line);
		break;
	case ELogSeverity::Warning:
		PrintBuffer(TraceBuffer, TEXT("[%s][Warning] - "), GetLoggerName());
		break;
	case ELogSeverity::Error:
		PrintBuffer(TraceBuffer, TEXT("[%s][Error] - "), GetLoggerName());
		break;
	case ELogSeverity::Critical:
		PrintBuffer(TraceBuffer, TEXT("[%s][Critical] - "), GetLoggerName());
		break;
	default:
		PrintBuffer(TraceBuffer, TEXT("Invalid log severity given to logger. Choose one option from ELogSeverity enum."));
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
		SOutputBuffer << TEXT( "\n" );
	}
	if (s_UseConsole)
	{
		Printf( TraceBuffer );
		Printf( OutputBuffer );
		Printf( TEXT("\n") );
	}
#if HE_COMPILE_MSVC
	else if ( IsDebuggerPresent() )
	{
		OutputDebugString(TraceBuffer);
		OutputDebugString(OutputBuffer);
		OutputDebugString(TEXT("\n"));
	}
#endif
}
