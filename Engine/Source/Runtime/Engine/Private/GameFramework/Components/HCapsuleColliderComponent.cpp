#include "EnginePCH.h"

#include "GameFramework/Components/HCapsuleColliderComponent.h"

#include "EnginePCH.h"

#include "GameFramework/Components/HCapsuleColliderComponent.h"

#include "World/World.h"
#include "Engine/Engine.h"


HCapsuleColliderComponent::HCapsuleColliderComponent( FComponentInitArgs& InitArgs )
	: HColliderComponent( InitArgs )
{

}

HCapsuleColliderComponent::~HCapsuleColliderComponent()
{

}

void HCapsuleColliderComponent::OnCreate()
{
	Super::OnCreate();

}

void HCapsuleColliderComponent::OnDestroy()
{
	Super::OnDestroy();

	UnRegisterCollider();
}

void HCapsuleColliderComponent::Serialize( WriteContext& Output )
{
	Output.Key( HE_STRINGIFY( HCapsuleColliderComponent ) );
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

			Output.Key( HE_STRINGIFY( m_RigidBody.m_Radius ) );
			Output.Double( m_RigidBody.GetRadius() );

			Output.Key( HE_STRINGIFY( m_RigidBody.m_Length ) );
			Output.Double( m_RigidBody.GetLength() );
		}
		Output.EndObject();
	}
	Output.EndArray();
}

void HCapsuleColliderComponent::Deserialize( const ReadContext& Value )
{
	Super::Deserialize( Value[0][HE_STRINGIFY( HColliderComponent )] );

	const ReadContext& This = Value[1];
	bool IsStatic;
	JsonUtility::GetBoolean( This, HE_STRINGIFY( m_RigidBody.m_IsStatic ), IsStatic );
	m_RigidBody.SetIsStatic( IsStatic );
	float Radius = -1.f;
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_Radius ), Radius );
	m_RigidBody.SetRadius( Radius );
	float Length = -1.f;
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_Length ), Length );
	m_RigidBody.SetLength( Radius );

	RegisterCollider( false );
}

void HCapsuleColliderComponent::RegisterCollider( bool StartDisabled )
{
	GetWorld()->AddCapsuleColliderComponent( this, StartDisabled );
}

void HCapsuleColliderComponent::UnRegisterCollider()
{
	GetWorld()->RemoveColliderComponent( this );
}
