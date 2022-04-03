// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Components/HSceneComponent.h"

#include "GameFramework/Actor/AActor.h"


HSceneComponent::HSceneComponent( FComponentInitArgs& InitArgs )
	: HActorComponent( InitArgs )
	, m_pParent( nullptr )
	, m_IsStatic( false )
{
	AActor* pOwner = GetOwner();
	if (pOwner->GetRootComponent() == nullptr)
		pOwner->SetRootComponent( this );
	else
		AttachTo( pOwner->GetRootComponent() );
}

HSceneComponent::~HSceneComponent()
{
}

void HSceneComponent::Render( FCommandContext& GfxContext )
{
}

void HSceneComponent::Serialize( WriteContext& Output )
{
	Output.Key( HE_STRINGIFY( HSceneComponent ) );
	Output.StartArray();
	{
		// Outer properties.
		Output.StartObject();
		{
			Super::Serialize( Output );
		}
		Output.EndObject();

		// Transform properties.
		Output.StartObject();
		{
			Output.Key( HE_STRINGIFY( m_Transform ) );
			Output.StartArray();
			{
				FVector3 Pos = m_Transform.GetPosition();
				FVector3 Rot = m_Transform.GetRotation();
				FVector3 Sca = m_Transform.GetScale();

				Output.Key( "PositionX" );
				Output.Double( Pos.x );
				Output.Key( "PositionY" );
				Output.Double( Pos.y );
				Output.Key( "PositionZ" );
				Output.Double( Pos.z );

				Output.Key( "RotationX" );
				Output.Double( Rot.x );
				Output.Key( "RotationY" );
				Output.Double( Rot.y );
				Output.Key( "RotationZ" );
				Output.Double( Rot.z );

				Output.Key( "ScaleX" );
				Output.Double( Sca.x );
				Output.Key( "ScaleY" );
				Output.Double( Sca.y );
				Output.Key( "ScaleZ" );
				Output.Double( Sca.z );
			}
			Output.EndArray();
		}
		Output.EndObject();
	}
	Output.EndArray();
}

void HSceneComponent::Deserialize( const ReadContext& Value )
{
	Super::Deserialize( Value[0] );

	const rapidjson::Value& SceneComponent = Value[1];
	const rapidjson::Value& LocalTransform = SceneComponent[HE_STRINGIFY( m_Transform )][0];

	// FTransform
	FVector3 Position, Scale;
	FQuat Rotation;
	// Position
	JsonUtility::GetFloat( LocalTransform, "PositionX", Position.x );
	JsonUtility::GetFloat( LocalTransform, "PositionY", Position.y );
	JsonUtility::GetFloat( LocalTransform, "PositionZ", Position.z );
	SetPosition( Position );
	// Rotation
	JsonUtility::GetFloat( LocalTransform, "RotationX", Rotation.x );
	JsonUtility::GetFloat( LocalTransform, "RotationY", Rotation.y );
	JsonUtility::GetFloat( LocalTransform, "RotationZ", Rotation.z );
	SetRotation( Rotation );
	// Scale
	JsonUtility::GetFloat( LocalTransform, "ScaleX", Scale.x );
	JsonUtility::GetFloat( LocalTransform, "ScaleY", Scale.y );
	JsonUtility::GetFloat( LocalTransform, "ScaleZ", Scale.z );
	SetScale( Scale );
}
