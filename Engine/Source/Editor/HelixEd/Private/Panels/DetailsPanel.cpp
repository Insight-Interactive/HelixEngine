#include "HelixEdPCH.h"

#include "Panels/DetailsPanel.h"
#include "GameFramework/Actor/AActor.h"
#include "StringHelper.h"
#include "GameFramework/Components/HActorComponent.h"


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

	ImGui::Spacing();
	ImGui::Text( pActor->GetStaticClassName() );
	ImGui::Text( "-" );
	ImGui::Text( pActor->GetObjectName() );
	ImGui::Separator();

	ImGui::PushID( "Pos" );
	FVector3 Position = pActor->GetTransform().GetPosition();
	ImGui::Text( "Position" );
	ImGui::SameLine();
	ImGui::DragFloat3( "", &Position.x );
	pActor->GetTransform().SetPosition( Position );
	ImGui::PopID();

	ImGui::PushID( "Rot" );
	FVector3 Rotation = pActor->GetTransform().GetEulerRotation();
	Rotation.x = Math::RadiansToDegrees( Rotation.x );
	Rotation.y = Math::RadiansToDegrees( Rotation.y );
	Rotation.z = Math::RadiansToDegrees( Rotation.z );
	ImGui::Text( "Rotation" );
	ImGui::SameLine();
	ImGui::DragFloat3("", &Rotation.x);
	Rotation.x = Math::DegreesToRadians( Rotation.x );
	Rotation.y = Math::DegreesToRadians( Rotation.y );
	Rotation.z = Math::DegreesToRadians( Rotation.z );
	pActor->GetTransform().SetRotation(Rotation);
	ImGui::PopID();

	ImGui::PushID( "Sca" );
	FVector3 Scale = pActor->GetTransform().GetScale();
	ImGui::Text( "Scale" );
	ImGui::SameLine();
	ImGui::DragFloat3( "", &Scale.x ); 
	pActor->GetTransform().SetScale(Scale);
	ImGui::PopID();

	//ImGui::Separator();

	ImGui::NewLine();
	ImGui::Text( "Components" );
	ImGui::NewLine();

	for (uint32 i = 0; i < pActor->m_Components.size(); ++i)
	{
		HActorComponent* pComponent = pActor->m_Components[i];
		ImGui::Text( pComponent->GetComponenetStaticName() );
		ImGui::SameLine();
		ImGui::Text( "-" );
		ImGui::SameLine();
		ImGui::Text( pComponent->GetObjectName() );
		ImGui::Separator();
	}
}
