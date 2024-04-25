#include "HelixEdPCH.h"

#include "Panels/ToolbarPanel.h"

#include "Engine/Engine.h"
#include "Engine/Event/EngineEvent.h"


ToolbarPanel::ToolbarPanel()
{
}

ToolbarPanel::~ToolbarPanel()
{
}

void ToolbarPanel::UnInitialize()
{
}

void ToolbarPanel::Tick( float DeltaTime )
{
}

void ToolbarPanel::Render( FCommandContext& CmdCtx )
{
	ImGui::Begin( "Tools" );
	{
		bool IsPlayingInEditor = GEngine->IsPlayingInEditor();
		const Char* PlayButtonText;
		if (IsPlayingInEditor)
		{
			PlayButtonText = "Stop";
		}
		else
		{
			PlayButtonText = "Play";
		}

		if (ImGui::Button( PlayButtonText ) || GetOwningViewport().IsFirstPressed( Key_F5 ))
		{
			if (GEngine->IsPlayingInEditor())
			{
				EmitEvent( EngineEndPlayEvent() );
			}
			else
			{
				EmitEvent( EngineBeginPlayEvent() );
			}
		}
	}
	ImGui::End();
}
