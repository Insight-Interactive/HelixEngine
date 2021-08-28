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

#if HE_WITH_EDITOR
	if (Args[L"-launchcfg"] == L"LaunchEditor")
	{
		GEngine = new HEditorEngine();
	}
	else if (Args[L"-launchcfg"] == L"LaunchGame")
#endif // HE_WITH_EDITOR
	{
		GEngine = new HEngine();
	}

	GEngine->EngineMain();

	HE_SAFE_DELETE_PTR( GEngine );
}
