// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Components/HActorComponent.h"

#include "GameFramework/Actor/AActor.h"
#include "StringHelper.h"


HActorComponent::HActorComponent(FComponentInitArgs& InitArgs)
	: HObject( InitArgs.Name )
	, m_pOwner( CCast<AActor*>(InitArgs.pOwner) )
{

}

HActorComponent::~HActorComponent()
{

}

HWorld* HActorComponent::GetWorld()
{
	HE_ASSERT( m_pOwner != NULL ); // Cannot get the world using a null actor. Are you trying to get the world from a component constructur?
	return m_pOwner->GetWorld();
}

void HActorComponent::Serialize( JsonUtility::WriteContext& Output )
{
	Output.Key("ObjectName");
	Output.String( TCharToChar(GetObjectName()) );

	Output.Key("ObjectGUID");
	Output.String(GetGuid().ToString().CStr());
}

void HActorComponent::Deserialize( const JsonUtility::ReadContext& Value )
{
	// Object Name
	char ObjectNameBuffer[32];
	JsonUtility::GetString( Value, "ObjectName", ObjectNameBuffer, sizeof( ObjectNameBuffer ) );
	SetObjectName( CharToTChar( ObjectNameBuffer ) );

	// Object GUID
	Char GuidStr[64];
	ZeroMemory(GuidStr, sizeof(GuidStr));
	JsonUtility::GetString(Value, "ObjectGUID", GuidStr, sizeof(GuidStr));
	FGUID ObjectGuid = FGUID::CreateFromString(GuidStr);
	SetGuid(ObjectGuid);
}

void HActorComponent::OnOwnerDeserializeComplete()
{

}
