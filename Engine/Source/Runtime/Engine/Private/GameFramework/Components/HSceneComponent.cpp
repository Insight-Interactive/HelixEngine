#include "EnginePCH.h"

#include "GameFramework/Components/HSceneComponent.h"

HSceneComponent::HSceneComponent( const HName& Name )
	: HActorComponent(Name)
{
}

HSceneComponent::~HSceneComponent()
{
}

void HSceneComponent::Render( FCommandContext& GfxContext )
{
}

void HSceneComponent::Serialize( WriteContext& Output )
{
	Super::Serialize( Output );

}

void HSceneComponent::Deserialize( const ReadContext& Value ) 
{
	Super::Deserialize( Value[0] );

	const rapidjson::Value& SceneComponent = Value[1];
	const rapidjson::Value& LocalTransform = SceneComponent["LocalTransform"][0];

	// FTransform
	FVector3 Position, Rotation, Scale;
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
