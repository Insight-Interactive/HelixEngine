#include "HelixEdPCH.h"

#include "Editor/Tabs/PreferencesTab.h"

#include "Editor/EditorEngine.h"


PreferencesTab::PreferencesTab()
	: ContentEditorInterface( TEXT("Editor Preferences") )
{

}

PreferencesTab::~PreferencesTab()
{

}

void PreferencesTab::Render( FCommandContext& CmdContext )
{
	EditorPreferences& EdPrefs = GEditorEngine->GetPreferences();
		
	ImGui::Begin( "##CommonSettings" );
	{
		ImGui::Checkbox( "Enable Dark Mode ", &EdPrefs.EnableDarkMode );
	}
	ImGui::End();
}

void PreferencesTab::SetupPanels()
{
}

