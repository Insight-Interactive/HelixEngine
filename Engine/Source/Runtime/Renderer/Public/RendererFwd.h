#pragma once


#if RENDERER_MODULE
#	define RENDER_API //__declspec(dllexport)
#else
#	define RENDER_API //__declspec(dllimport)
#endif

#if R_WITH_D3D12 || R_WITH_D3D11
#	define R_WITH_D3D 1
#else
#	define R_WITH_D3D 0
#endif

#define R_MAX_DEVICE_NAME_LENGTH ( 64 )

#if HE_ENABLE_LOGS
extern class Logger GRendererLogger;
/*
	Log a message to the console.
	@param Severety - The severity of the error.
	@param fmt - the format to display when writing to the console.
	@param __VA_ARGS__ - Optional arguments to supply when printing.
*/
#	define R_LOG(Severity, Fmt, ...) GRendererLogger.LogHelper(ELogSeverity::Severity, Fmt, HE_FILE, HE_FUNCTION, __LINE__, __VA_ARGS__);

#else
#	define R_LOG(Severity, Fmt, ...)
#endif
