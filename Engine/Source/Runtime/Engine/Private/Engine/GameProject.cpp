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

void FGameProject::Startup( const Char* HProjectFilpath )
{
	rapidjson::Document WorldJsonDoc;
	m_HProject->Load( HProjectFilpath, FUM_Read, CM_Text );
	if (m_HProject->IsOpen())
	{
		JsonUtility::LoadDocument( m_HProject, WorldJsonDoc );
		if (WorldJsonDoc.IsObject())
		{
			Char NameBuffer[32];

			// Set game name.
			JsonUtility::GetString( WorldJsonDoc, "GameName", NameBuffer, sizeof( NameBuffer ) );
			FApp::GetInstance()->SetName( CharToTChar( String( NameBuffer) ) );

			ZeroMemory( NameBuffer, sizeof( NameBuffer ) );

			// Set project name.
			JsonUtility::GetString( WorldJsonDoc, "ProjectName", NameBuffer, sizeof( NameBuffer ) );
			m_ProjectName = CharToTChar( String( NameBuffer ) );
		}
	
		String ProjectRoot = StringHelper::GetDirectoryFromPath( HProjectFilpath ) + "\\";
		SetProjectRootDirectory( ProjectRoot.c_str() );
	}
	else
	{
		System::CreateMessageBox( L"No game project/content found is specified directory!", L"Error", System::MessageDialogInput::MDI_Ok, System::MessageDialogIcon::MDIcon_Critical);
	}
}

const String FGameProject::GetDefaultLevelPath() const
{
	rapidjson::Document WorldJsonDoc;

	JsonUtility::LoadDocument( m_HProject, WorldJsonDoc );
	if (WorldJsonDoc.IsObject())
	{
		Char LevelPath[HE_MAX_PATH];
		JsonUtility::GetString( WorldJsonDoc, "DefaultLevel", LevelPath, sizeof( LevelPath ) );

		return GetContentFolder() + "/" + String(LevelPath);
	}

	return String( "" );
}
