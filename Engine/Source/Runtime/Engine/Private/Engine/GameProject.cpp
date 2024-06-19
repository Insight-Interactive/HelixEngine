// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Engine/GameProject.h"

#include "App/App.h"


FGameProject::FGameProject()
{
}

FGameProject::~FGameProject()
{

}

void FGameProject::Startup( FPath& HProjectFilpath )
{
	rapidjson::Document WorldJsonDoc;
	FileRef m_HProject( HProjectFilpath, FUM_Read, CM_Text );
	if (m_HProject->IsOpen())
	{
		JsonUtility::LoadDocument( m_HProject, WorldJsonDoc );
		if (WorldJsonDoc.IsObject())
		{
			Char NameBuffer[32];

			// Set game name.
			JsonUtility::GetString( WorldJsonDoc, "GameName", NameBuffer, sizeof( NameBuffer ) );
			FApp::GetInstance()->SetName( CharToTChar( String( NameBuffer) ) );

			// Set project name.
			JsonUtility::GetString( WorldJsonDoc, "ProjectName", m_ProjectName, sizeof( m_ProjectName ) );
		}
	
		String ProjectRoot = StringHelper::GetDirectoryFromPath( HProjectFilpath.GetFullPath() ) + "\\";
		SetProjectRootDirectory( ProjectRoot.c_str() );
	}
	else
	{
		System::CreateMessageBox( L"No game project/content found is specified directory!", L"Error", System::MessageDialogInput::MDI_Ok, System::MessageDialogIcon::MDIcon_Critical);
	}
}

const void FGameProject::GetDefaultLevelPath( FPath& outPath ) const
{
	HE_ASSERT( m_ProjectRoot.IsValid() ); // Project was not initialized yet!

	rapidjson::Document WorldJsonDoc;

	FPath HProjectPath;
	HProjectPath.SetPath( m_ProjectRoot.GetFullPath() );
	HProjectPath.Concat( "Game.hproject" );

	FileRef HProject( HProjectPath, FUM_Read, CM_Text );
	JsonUtility::LoadDocument( HProject, WorldJsonDoc );
	if (WorldJsonDoc.IsObject())
	{
		char LevelPath[HE_MAX_PATH];
		JsonUtility::GetString( WorldJsonDoc, "DefaultLevel", LevelPath, sizeof( LevelPath ) );

		FGameProject::GetInstance()->GetContentDirectoryFullPath( LevelPath, outPath.m_Path, sizeof( outPath.m_Path ) );
	}
}
