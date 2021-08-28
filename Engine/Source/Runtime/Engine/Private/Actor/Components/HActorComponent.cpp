// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Actor/Components/HActorComponent.h"

#include "Actor/AActor.h"


HWorld* HActorComponent::GetWorld()
{
	return m_pOwner->GetWorld();
}
