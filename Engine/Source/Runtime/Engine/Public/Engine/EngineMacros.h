// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

// Entry Points
//
#ifdef HE_WINDOWS						
#	define HE_ENTRY_POINT() int APIENTRY wWinMain(					\
							_In_		HINSTANCE hInstance,		\
							_In_opt_	HINSTANCE hPrevInstance,	\
							_In_		LPWSTR CmdLine,				\
							_In_		int nCmdShow)				

#elif defined HE_PLAYSTATION
#	define HE_ENTRY_POINT() int main(int nCmdShow, wchar_t* CmdLine[]) 

#endif


#if HE_ENABLE_LOGS
	extern class Logger GEngineLogger;
	/*
		Log a message to the console. Text should be in TChar's.
		@param Severety - The severity of the error.
		@param fmt - The format to display when writing to the console.
		@param __VA_ARGS__ - Optional arguments to supply when printing.
	*/
#	define HE_LOG(Severity, Fmt, ...) GEngineLogger.LogHelper(ELogSeverity::Severity, Fmt, HE_FILE, HE_FUNCTION, __LINE__, __VA_ARGS__)
#	define HE_LOG_EXPLICIT_SEVERITY(Severity, Fmt, ...) GEngineLogger.LogHelper(Severity, Fmt, HE_FILE, HE_FUNCTION, __LINE__, __VA_ARGS__)

#else
#	define HE_LOG(Severity, Fmt, ...)
#endif


/*
	String describing the current build configuration of the engine.
*/
#ifdef HE_DEBUG_EDITOR
#	define HE_CONFIG_STRING "Debug Editor"
#elif HE_DEVELOPMENT
#	define HE_CONFIG_STRING "Development"
#elif HE_DEBUG_GAME
#	define HE_CONFIG_STRING "Debug Game"
#elif HE_DEMO_GAME
#	define HE_CONFIG_STRING "Demo Game"
#elif HE_SHIPPING
#	define HE_CONFIG_STRING "Shipping Game"
#endif

/*
	String describing the current build platform of the engine.
*/
#ifdef HE_WIN64
#	define HE_PLATFORM_STRING "Windows 64"
#elif HE_WIN32
#	define HE_PLATFORM_STRING "Windows 32"
#elif HE_DURANGO
#	define HE_PLATFORM_STRING "Xbox One (Durango)"
#elif HE_SCORPIO
#	define HE_PLATFORM_STRING "Xbox One X (Scorpio)"
#elif HE_PROSPERO
#	define HE_PLATFORM_STRING "PlayStation 5 (Prospero)"
#endif
