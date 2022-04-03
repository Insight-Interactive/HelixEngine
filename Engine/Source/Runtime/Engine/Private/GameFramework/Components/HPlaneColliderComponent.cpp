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

}

void HPlaneColliderComponent::OnDestroy()
{
	Super::OnDestroy();

	UnRegisterCollider();
}

void HPlaneColliderComponent::Serialize( WriteContext& Output )
{
	Output.Key( HE_STRINGIFY( HPlaneColliderComponent ) );
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

			Output.Key( HE_STRINGIFY( m_Width ) );
			Output.Double( m_RigidBody.GetWidth() );

			Output.Key( HE_STRINGIFY( m_Height ) );
			Output.Double( m_RigidBody.GetHeight() );
		}
		Output.EndObject();
	}
	Output.EndArray();
}

void HPlaneColliderComponent::Deserialize( const ReadContext& Value )
{
	Super::Deserialize( Value[0][HE_STRINGIFY( HColliderComponentInterface )] );

	const ReadContext& This = Value[1];
	bool IsStatic;
	JsonUtility::GetBoolean( This, HE_STRINGIFY( m_RigidBody.m_IsStatic ), IsStatic );
	m_RigidBody.SetIsStatic( IsStatic );
	float Width, Height;
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_Width ), Width );
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_Height ), Height );
	m_RigidBody.SetWidth( Width );
	m_RigidBody.SetHeight( Height );

	RegisterCollider( false );
}

void HPlaneColliderComponent::RegisterCollider( bool StartDisabled )
{
	GetWorld()->AddPlaneColliderComponent( this, StartDisabled );
}

void HPlaneColliderComponent::UnRegisterCollider()
{
	GetWorld()->RemoveColliderComponent( this );
}
