#include "HelixEdPCH.h"

#include "Panels/WorldOutlinePanel.h"
#include "Engine/Engine.h"
#include "World/Level.h"
#include "GameFramework/Actor/AActor.h"
#include "Engine/Event/EngineEvent.h"


WorldOutlinePanel::WorldOutlinePanel()
{
}

WorldOutlinePanel::~WorldOutlinePanel()
{
}

void WorldOutlinePanel::Initialize()
{
}

void WorldOutlinePanel::UnInitialize()
{
}

void WorldOutlinePanel::Tick( float DeltaTime )
{
}

void WorldOutlinePanel::Render( FCommandContext& CmdCtx )
{
	ImGui::Begin( "World Outliner" );
	{
		HLevel* pLevel = GEngine->GetClientViewport().GetWorld().GetCurrentLevel();
		for (uint32 j = 0; j < pLevel->m_Actors.size(); j++)
		{
			AActor* pCurrentActor = pLevel->m_Actors[j];
			const HName& ActorName = pCurrentActor->GetObjectName();
			if (ImGui::TreeNodeEx( TCharToChar( ActorName ), ImGuiTreeNodeFlags_Leaf ))
			{
				if (ImGui::IsItemClicked())
				{
					ObjectSelectedEvent e;
					e.SetSelectedObject( pCurrentActor );
					EmitEvent( e );
				}
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();
}
