#include "EnginePCH.h"

#include "GameFramework/Components/HPlaneColliderComponent.h"

#include "World/World.h"
#include "Engine/Engine.h"


HPlaneColliderComponent::HPlaneColliderComponent( FComponentInitArgs& InitArgs )
	: HColliderComponentInterface( InitArgs )
{
	
}

HPlaneColliderComponent::~HPlaneColliderComponent()
{

}

void HPlaneColliderComponent::OnCreate()
{
	Super::OnCreate();

	GetWorld()->AddPlaneColliderComponent( this );
}

void HPlaneColliderComponent::Serialize( WriteContext& Output )
{
}

void HPlaneColliderComponent::Deserialize( const ReadContext& Value )
{
	Super::Deserialize( Value[0][HE_STRINGIFY( HColliderComponentInterface )] );

	const ReadContext& This = Value[1];
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_Width ), m_Width );
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_Height ), m_Height );
}
