#include "EnginePCH.h"

#include "GameFramework/Components/HCubeColliderComponent.h"

#include "World/World.h"
#include "Engine/Engine.h"


HCubeColliderComponent::HCubeColliderComponent( FComponentInitArgs& InitArgs )
	: HColliderComponent( InitArgs )
{

}

HCubeColliderComponent::~HCubeColliderComponent()
{

}

void HCubeColliderComponent::OnCreate()
{
	Super::OnCreate();

}

void HCubeColliderComponent::OnDestroy()
{
	Super::OnDestroy();

	UnRegisterCollider();
}

void HCubeColliderComponent::Serialize( WriteContext& Output )
{
	Output.Key( HE_STRINGIFY( HCubeColliderComponent ) );
	Output.StartArray();
	{
		// Outer properties.
		Output.StartObject();
		{
			Super::Serialize( Output );
		}
		Output.EndObject();

		// Static mesh properties.
		Output.StartObject();
		{
			Output.Key( HE_STRINGIFY( m_RigidBody.m_IsStatic ) );
			Output.Bool( m_RigidBody.GetIsStatic() );

			Output.Key( HE_STRINGIFY( m_RigidBody.m_Width ) );
			Output.Double( m_RigidBody.GetWidth() );

			Output.Key( HE_STRINGIFY( m_RigidBody.m_Height ) );
			Output.Double( m_RigidBody.GetHeight() );

			Output.Key( HE_STRINGIFY( m_RigidBody.m_Length ) );
			Output.Double( m_RigidBody.GetDepth() );
		}
		Output.EndObject();
	}
	Output.EndArray();
}

void HCubeColliderComponent::Deserialize( const ReadContext& Value )
{
	Super::Deserialize( Value[0][HE_STRINGIFY( HColliderComponent )] );

	const ReadContext& This = Value[1];
	bool IsStatic;
	JsonUtility::GetBoolean( This, HE_STRINGIFY( m_RigidBody.m_IsStatic ), IsStatic );
	m_RigidBody.SetIsStatic( IsStatic );
	FVector3 Dimensions;
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_Width ), Dimensions.x );
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_Height ), Dimensions.y );
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_Depth ), Dimensions.z );
	m_RigidBody.SetWidth( Dimensions.x );
	m_RigidBody.SetHeight( Dimensions.y );
	m_RigidBody.SetDepth( Dimensions.z );

	RegisterCollider( false );
}

void HCubeColliderComponent::RegisterCollider( bool StartDisabled )
{
	GetWorld()->AddCubeColliderComponent( this, StartDisabled );
}

void HCubeColliderComponent::UnRegisterCollider()
{
	GetWorld()->RemoveColliderComponent( this );
}

