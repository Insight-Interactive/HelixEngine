#include "PackageCookerPCH.h"

#include "FileSystem.h"
#include "JsonUtility.h"
#include "ThreadPool.h"
#include "System.h"


Logger GLogger;
#define LOG(Severity, Fmt, ...) GLogger.LogHelper(ELogSeverity::Severity, Fmt, HE_FILE, HE_FUNCTION, __LINE__, __VA_ARGS__)

/*
	Available command line arguments
	--------------------------------
	-i   Game project root directory, The project root of the game to build.	( -i C:/Dev/Projects/MyGame )
	-o   Game build directory The directory to store the build result.			( -o C:/Dev/Projects/Builds )
*/

static const char* kInputKey	= "-i";
static const char* kOutputKey	= "-o";

int main( int32 argc, Char* argv[] )
{
	CreateLogger( GLogger, "PackageCooker" );
	GLogger.SetUseConsole( true );

	ThreadPool ThreadPool( System::GetProcessorCount(), NULL );

	LOG( Log, TEXT( "Helix Package Cooker" ) );
	FileSystem::Init();

	Char InputDirectory[HE_MAX_PATH]; ZeroMemory( InputDirectory, sizeof( InputDirectory ) );
	Char OutputDirectory[HE_MAX_PATH]; ZeroMemory( OutputDirectory, sizeof( OutputDirectory ) );
	for (int32 i = 0; i < argc; i++)
	{
		//LOG( Log, TEXT("Cmdline: %s"), CharToTChar(argv[i]) );
		if (strcmp( argv[i], kInputKey ) == 0)
		{
			strcat_s( InputDirectory, argv[i + 1] );
			continue;
		}
		if (strcmp( argv[i], kOutputKey ) == 0)
		{
			strcat_s( OutputDirectory, " " );
			strcat_s( OutputDirectory, argv[i + 1] );
			continue;
		}
	}
	strcat_s( OutputDirectory, "\\Data" );

	// Copy the data to from the project diretory to the build directory.
	Char CopyCmd[HE_MAX_PATH];	ZeroMemory( CopyCmd, sizeof( CopyCmd ) );
	Char SrcDir[HE_MAX_PATH];	ZeroMemory( SrcDir, sizeof( SrcDir ) );
	Char DstDir[HE_MAX_PATH];	ZeroMemory( DstDir, sizeof( DstDir ) );
	// Copy the "Content" directory.
	// Src
	LOG( Log, TEXT("Copying Content directory... ") );
	strcpy_s( SrcDir, InputDirectory );
	strcat_s( SrcDir, "\\Content" );
	// Dst
	strcpy_s( DstDir, OutputDirectory );
	strcat_s( DstDir, "\\Content" );
	sprintf_s( CopyCmd, "xcopy /q /e /y /i %s %s", SrcDir, DstDir );
	// Exec
	system( CopyCmd );

	// Reset the buffers so they can be reused.
	ZeroMemory( CopyCmd, sizeof( CopyCmd ) );
	ZeroMemory( SrcDir, sizeof( SrcDir ) );
	ZeroMemory( DstDir, sizeof( DstDir ) );

	// Copy the "Config" directory.
	// Src
	LOG( Log, TEXT("Copying Config directory... ") );
	strcpy_s( SrcDir, InputDirectory );
	strcat_s( SrcDir, "\\Config" );
	// Dst
	strcpy_s( DstDir, OutputDirectory );
	strcat_s( DstDir, "\\Config" );
	sprintf_s( CopyCmd, "xcopy /q /e /y /i %s %s", SrcDir, DstDir );
	// Exec
	system( CopyCmd );

	// Reset the buffers so they can be reused.
	ZeroMemory( CopyCmd, sizeof( CopyCmd ) );
	ZeroMemory( SrcDir, sizeof( SrcDir ) );
	ZeroMemory( DstDir, sizeof( DstDir ) );

	// Copy the "*.hproject" file.
	// Src
	LOG( Log, TEXT("Copying *.project file... ") );
	strcpy_s( SrcDir, InputDirectory );
	strcat_s( SrcDir, "\\*.hproject" );
	// Dst
	strcpy_s( DstDir, OutputDirectory );
	strcat_s( DstDir, "\\" );
	sprintf_s( CopyCmd, "xcopy /q /y /i %s %s", SrcDir, DstDir );
	// Exec
	system( CopyCmd );

	return 0;
}
