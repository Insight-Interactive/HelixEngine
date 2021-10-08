#include "HelixEdPCH.h"

#include "Tools/PackageMaker.h"
#include "FileSystem.h"
#include "Developer/ConsoleWindow.h"
#include "StringHelper.h"


const Char* kBuildPlatformStrings[BuildPlatform_Count]
{
	HE_STRINGIFY( Prospero ),
	HE_STRINGIFY( Win32 ),
	HE_STRINGIFY( Win64 ),
	HE_STRINGIFY( Durango ),
	HE_STRINGIFY( Scorpio ),
};

const Char* kBuildConfigStrings[BuildConfig_Count]
{
	HE_STRINGIFY( DebugGame ),
	HE_STRINGIFY( ShippingGame ),
};

/*
	Default location for visual studio devenv executable. Does not include .exe extention.
*/
const Char* kVisualStudioDevEnvNoExe = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Enterprise\\Common7\\IDE\\devenv";
/*
	Default location for visual studio devenv executable.
*/
const Char* kVisualStudioDevEnv = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Enterprise\\Common7\\IDE\\devenv.exe";


bool PackageMaker::CheckVisualStudioInstalled()
{
	return FileSystem::DoesFileExist( kVisualStudioDevEnv );
}

/*static*/ bool PackageMaker::BuildPackage( EBuildConfig BuildConfig, EBuildPlatform BuildPlatform, const Char* GameName, const Char* ProjectDir, const Char* BuildDir )
{
	if( !CheckVisualStudioInstalled() )
	{
		HE_LOG( Error, TEXT( "Visual Studio is not installed on this machine! Game binaries cannot be build, canceling build process..." ) );
		return false;
	}

	ConsoleWindow Console;
	Console.Create( ConsoleWindowDesc{} );


	// Assemble the game build root folder.
	// Ex) C:/BuildsFolder/<GameName>
	Char GameBuildRootDir[HE_MAX_PATH];
	sprintf_s( GameBuildRootDir, "%s\\%s", BuildDir, GameName );

	// Make the root diretory for the game.
	Char MkDirCmd[HE_MAX_PATH];
	sprintf_s( MkDirCmd, "mkdir %s", GameBuildRootDir );
	system( MkDirCmd );


	// Build the engine in Standalone mode.
	const Char* BuildPlatformStr = PackageMaker::GetBuildPlatformString( BuildPlatform );
	const Char* BuildConfigStr = PackageMaker::GetBuildConfigString( BuildConfig );
	HE_LOG( Log, TEXT( "Compiling engine source code in configuration: %s - %s" ), CharToTChar( BuildConfigStr ), CharToTChar( BuildPlatformStr ) );
	Char EngineSourceBuildCmd[HE_MAX_PATH];
	sprintf_s( EngineSourceBuildCmd, "\"\"%s\" /build \"%s|%s\" /project Engine ..\\..\\HelixEngine.sln\"", kVisualStudioDevEnvNoExe, BuildConfigStr, BuildPlatformStr );
	int CompileResult = system( EngineSourceBuildCmd );
	if (CompileResult != EXIT_SUCCESS)
	{
		HE_LOG( Error, TEXT( "Engine source compilation fialed! View log for details. Code: %i" ), CompileResult );
		return false;
	}


	// Copy the engine build to the game build directory.
	printf( "Copying Helix engine build. " );
	Char EngineBuildDir[HE_MAX_PATH];
	sprintf_s( EngineBuildDir, "..\\%s-%s", BuildConfigStr, BuildPlatformStr );
	Char EngineCopyCmd[HE_MAX_PATH];
	sprintf_s( EngineCopyCmd, "xcopy /q /e /y /i %s %s", EngineBuildDir, GameBuildRootDir );
	system( EngineCopyCmd );


	// Build the game assets.
	HE_LOG( Log, TEXT( "Building %s data..." ), CharToTChar( GameName ) );
	char Cmd[512];
	sprintf_s( Cmd, "PackageCooker.exe -i %s -o %s", ProjectDir, GameBuildRootDir );
	system( Cmd );
	HE_LOG( Log, TEXT( "Success." ) );
	HE_LOG( Log, TEXT( "Standalone game build in directory %s" ), CharToTChar( GameBuildRootDir ) );

	return false;
}
