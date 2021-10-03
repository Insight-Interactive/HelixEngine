#include "EnginePCH.h"

#include "Engine/GameProject.h"

#include "App/FApp.h"
#include "JsonUtility.h"
#include "StringHelper.h"


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
	HE_ASSERT( WorldJsonSource->IsOpen() );
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
