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

void DetailsPanel::Render( FCommandContext& CmdCtx )
{
	ImGui::Begin( "Details" );
	{
		if (m_pSelectedObject)
		{
			if (AActor* pActor = SCast<AActor*>( m_pSelectedObject ))
			{
				PreviewActor( pActor );
			}

		}
	}
	ImGui::End();
}

void DetailsPanel::PreviewActor( AActor* pActor )
{
	FVector3 Position = pActor->GetTransform().GetPosition();
	ImGui::DragFloat( "Position X: ", &Position.x );
	ImGui::DragFloat( "Position Y: ", &Position.y );
	ImGui::DragFloat( "Position Z: ", &Position.z );
	pActor->GetTransform().SetPosition(Position);

	FVector3 Rotation = pActor->GetTransform().GetRotation();
	ImGui::DragFloat("Rotation X: ", &Rotation.x);
	ImGui::DragFloat("Rotation Y: ", &Rotation.y);
	ImGui::DragFloat("Rotation Z: ", &Rotation.z);
	pActor->GetTransform().SetRotation(Rotation);

	FVector3 Scale = pActor->GetTransform().GetScale();
	ImGui::DragFloat("Scale X: ", &Scale.x);
	ImGui::DragFloat("Scale Y: ", &Scale.y);
	ImGui::DragFloat("Scale Z: ", &Scale.z);
	pActor->GetTransform().SetScale(Scale);

}
