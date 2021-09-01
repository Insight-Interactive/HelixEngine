// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Actor/Components/HActorComponent.h"

#include "GameFramework/Actor/AActor.h"


HWorld* HActorComponent::GetWorld()
{
	HE_ASSERT( m_pOwner != NULL ); // Cannot get the world using a null actor. Are you trying to get the world from a component constructur?
	return m_pOwner->GetWorld();
}
