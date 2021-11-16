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
			Char GameName[32];
			JsonUtility::GetString( WorldJsonDoc, "GameName", GameName, sizeof( GameName ) );
			String GameNameStr = GameName;
			const HName TargetName = CharToTChar( GameNameStr );
			FApp::GetInstance()->SetName(TargetName.c_str());
		}
	
		String ProjectRoot = StringHelper::GetDirectoryFromPath( HProjectFilpath ) + "\\";
		SetProjectRootDirectory( ProjectRoot.c_str() );
	}
	else
	{
		System::CreateMessageBox( L"No game project/content found is specified directory!", L"Error", System::MessageDialogInput::MDI_Ok );
	}
}
