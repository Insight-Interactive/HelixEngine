#include "EnginePCH.h"

#include "Engine/HelixEngine.h"

#if HE_WITH_EDITOR
#	include "Editor/HEditorEngine.h"
#else
#	include "Engine/HEngine.h"
#endif // HE_WITH_EDITOR


void GuardedMain(WChar* CmdLine)
{
	CommandLine Args;
	Args.Process( CmdLine );
	//Args.Process( L"-launchcfg LaunchGame");

#if HE_WITH_EDITOR
	if (Args[L"-launchcfg"] == L"LaunchEditor")
	{
		GEngine = new HEditorEngine( Args );
	}
	else if (Args[L"-launchcfg"] == L"LaunchGame")
#endif // HE_WITH_EDITOR
	{
		GEngine = new HEngine( Args );
	}

	HE_ASSERT( GEngine != NULL );
	GEngine->EngineMain();

	HE_SAFE_DELETE_PTR( GEngine );
}
