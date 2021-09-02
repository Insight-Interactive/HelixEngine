#include "HelixEdPCH.h"

#include "Panels/ToolbarPanel.h"

#include "Engine/HEngine.h"


ToolbarPanel::ToolbarPanel()
{
}

ToolbarPanel::~ToolbarPanel()
{
}

void ToolbarPanel::Initialize()
{
}

void ToolbarPanel::UnInitialize()
{
}

void ToolbarPanel::Tick( float DeltaTime )
{
}

void ToolbarPanel::Render( ICommandContext& CmdCtx )
{
	ImGui::Begin( "Tools" );
	{
		/*bool IsPlayingInEditor = GEngine->IsPlayingInEditor();
		const Char* PlayButtonText;
		if (IsPlayingInEditor)
		{
			PlayButtonText = "Stop";
		}
		else
		{
			PlayButtonText = "Start";
		}

		if (ImGui::Button( PlayButtonText ))
		{
			GEngine->SetIsPlayingInEditor( !GEngine->IsPlayingInEditor() );
		}*/
	}
	ImGui::End();
}
