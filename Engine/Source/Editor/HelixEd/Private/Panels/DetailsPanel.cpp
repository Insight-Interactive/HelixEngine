#include "HelixEdPCH.h"

#include "Panels/DetailsPanel.h"
#include "GameFramework/Actor/AActor.h"
#include "StringHelper.h"


DetailsPanel::DetailsPanel()
	: m_pSelectedObject(NULL)
{
}

DetailsPanel::~DetailsPanel()
{
}

void DetailsPanel::Initialize()
{
}

void DetailsPanel::UnInitialize()
{
}

void DetailsPanel::Tick( float DeltaTime )
{
}

void DetailsPanel::Render( ICommandContext& CmdCtx )
{
	ImGui::Begin( "Details" );
	{
		if (m_pSelectedObject)
		{
			if (AActor* pActor = DCast<AActor*>( m_pSelectedObject ))
			{
				PreviewActor( pActor );
			}

		}
	}
	ImGui::End();
}

void DetailsPanel::PreviewActor( AActor* pActor )
{
	FVector3 Values = pActor->GetTransform().GetPosition();
	ImGui::DragFloat( "Pos X: ", &Values.x );
	ImGui::DragFloat( "Pos Y: ", &Values.y );
	ImGui::DragFloat( "Pos Z: ", &Values.z );

	pActor->GetTransform().SetPosition( Values );
}
