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

void DetailsPanel::UnInitialize()
{
}

void DetailsPanel::Tick(float DeltaTime)
{
}

void DetailsPanel::Render(FCommandContext& CmdCtx)
{
	ImGui::Begin("Details");
	{
		if (m_pSelectedObject)
		{
			if (AActor* pActor = SCast<AActor*>(m_pSelectedObject))
			{
				PreviewActor(pActor);
			}

		}
	}
	ImGui::End();
}

void DetailsPanel::PreviewActor(AActor* pActor)
{
	if (pActor == nullptr)
		return;

	ImGui::Text( TCharToChar( pActor->GetObjectName() ) );
	ImGui::NewLine();

	if (pActor->GetRootComponent())
	{
		FVector3 Position = pActor->GetRootComponent()->GetPosition();
		ImGui::DragFloat("Position X: ", &Position.x);
		ImGui::DragFloat("Position Y: ", &Position.y);
		ImGui::DragFloat("Position Z: ", &Position.z);
		pActor->GetRootComponent()->SetPosition(Position);

		FQuat Rotation = pActor->GetRootComponent()->GetRotation();
		ImGui::DragFloat("Rotation X: ", &Rotation.x);
		ImGui::DragFloat("Rotation Y: ", &Rotation.y);
		ImGui::DragFloat("Rotation Z: ", &Rotation.z);
		pActor->GetRootComponent()->SetRotation(Rotation);

		FVector3 Scale = pActor->GetRootComponent()->GetScale();
		ImGui::DragFloat("Scale X: ", &Scale.x);
		ImGui::DragFloat("Scale Y: ", &Scale.y);
		ImGui::DragFloat("Scale Z: ", &Scale.z);
		pActor->GetRootComponent()->SetScale(Scale);
	}

	ImGui::Separator();
	ImGui::Text( "Components" );
	ImGui::NewLine();

	for (uint32 i = 0; i < pActor->m_Components.size(); ++i)
	{
		HActorComponent* pComponent = pActor->m_Components[i];
		ImGui::Text( TCharToChar( pComponent->GetObjectName() ) );
	}
}
