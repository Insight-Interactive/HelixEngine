// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "HelixEdPCH.h"

#include "Panels/WorldOutlinePanel.h"

#include "World/Level.h"
#include "Engine/Engine.h"
#include "Engine/Event/EngineEvent.h"
#include "GameFramework/Actor/AActor.h"


WorldOutlinePanel::WorldOutlinePanel()
	: m_pWorld( nullptr )
{
}

WorldOutlinePanel::~WorldOutlinePanel()
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
		if (m_pWorld)
		{
			HLevel& Level = m_pWorld->GetCurrentLevel();
			for (uint32 j = 0; j < Level.m_Actors.size(); j++)
			{
				AActor* pCurrentActor = Level.m_Actors[j];

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


	}
	ImGui::End();
}
