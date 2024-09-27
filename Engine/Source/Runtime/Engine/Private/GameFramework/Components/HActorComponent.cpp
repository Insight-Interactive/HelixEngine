// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Components/HActorComponent.h"

#include "GameFramework/Actor/AActor.h"
#include "StringHelper.h"


HActorComponent::HActorComponent(FComponentInitArgs& InitArgs)
	: HObject( InitArgs.Name )
	, m_Owner( CCast<AActor*>(InitArgs.pOwner) )
{

}

HActorComponent::~HActorComponent()
{

}

HWorld* HActorComponent::GetWorld()
{
	HE_ASSERT( m_Owner != NULL ); // Cannot get the world using a null actor. Are you trying to get the world from a component constructur?
	return m_Owner->GetWorld();
}

void HActorComponent::Serialize( JsonUtility::WriteContext& Output )
{
	HObject::Serialize( Output );
}

void HActorComponent::Deserialize( const JsonUtility::ReadContext& Value )
{
	HObject::Deserialize( Value );
}

void HActorComponent::OnOwnerDeserializeComplete()
{

}
