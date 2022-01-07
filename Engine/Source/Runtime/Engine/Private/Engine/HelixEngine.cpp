// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/HelixEngine.h"

#if HE_WITH_EDITOR
#	include "Editor/EditorEngine.h"
#else
#	include "Engine/Engine.h"
#endif // HE_WITH_EDITOR

/*
	Private class used to boostrap and launch the engine.
*/
class HEngineLaunchBootstraper
{
public:
	static void Execute( HEngine** pEngine, WChar* CommandLine )
	{
		FCommandLine Args;
		Args.Process( CommandLine );

#if HE_WITH_EDITOR
		if (Args.ArgumentEquals( L"-launchcfg", L"LaunchEditor" ))
		{
			(*pEngine) = new HEditorEngine( Args );
		}
		else if (Args.ArgumentEquals( L"-launchcfg", L"LaunchGame" ))
#endif // HE_WITH_EDITOR
		{
			(*pEngine) = new HEngine( Args );
		}
		else
		{
			WChar ErrMsg[1024];
			ZeroMemory( ErrMsg, sizeof( ErrMsg ) );
			swprintf_s( ErrMsg, L"Unable to determine launch configuration!\nIncomplete or corrupt command line arguments given.\nCommand line: \"%s\"", CommandLine );
			System::CreateMessageBox( ErrMsg, L"Launch Failed", System::MessageDialogInput::MDI_Ok, System::MessageDialogIcon::MDIcon_Critical );
		}

		HE_ASSERT( (*pEngine) != NULL );
		(*pEngine)->EngineMain();

		HE_SAFE_DELETE_PTR( *pEngine );
	}
};

void GuardedMain( WChar* CmdLine )
{
#if HE_USE_EXCEPTIONS
	__try
#endif
	{
		HEngineLaunchBootstraper::Execute( &GEngine, CmdLine );
	}
#if HE_USE_EXCEPTIONS
	catch (...)
	{

	}
#endif

}
