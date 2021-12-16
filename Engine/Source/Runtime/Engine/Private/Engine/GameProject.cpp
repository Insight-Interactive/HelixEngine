// Copyright 2021 Insight Interactive. All Rights Reserved.
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
	FileRef WorldJsonSource( HProjectFilpath, FUM_Read );
	if (WorldJsonSource->IsOpen())
	{
		JsonUtility::LoadDocument( WorldJsonSource, WorldJsonDoc );
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
