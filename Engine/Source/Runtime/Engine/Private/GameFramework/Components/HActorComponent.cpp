// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Components/HActorComponent.h"

#include "GameFramework/Actor/AActor.h"
#include "StringHelper.h"


HWorld* HActorComponent::GetWorld()
{
	HE_ASSERT( m_pOwner != NULL ); // Cannot get the world using a null actor. Are you trying to get the world from a component constructur?
	return m_pOwner->GetWorld();
}

void HActorComponent::Serialize( WriteContext& Output )
{

}

void HActorComponent::Deserialize( const ReadContext& Value )
{
	// Object Name
	char ObjectNameBuffer[32];
	JsonUtility::GetString( Value, "ObjectName", ObjectNameBuffer, sizeof( ObjectNameBuffer ) );
	SetObjectName( CharToTChar( ObjectNameBuffer ) );
}
