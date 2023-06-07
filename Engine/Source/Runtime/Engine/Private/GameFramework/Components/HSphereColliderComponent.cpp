#include "EnginePCH.h"

#include "GameFramework/Components/HSphereColliderComponent.h"

#include "World/World.h"
#include "Engine/Engine.h"
#include "Renderer/GeometryGenerator.h"


HSphereColliderComponent::HSphereColliderComponent( FComponentInitArgs& InitArgs )
	: HColliderComponent( InitArgs )
{

}

HSphereColliderComponent::~HSphereColliderComponent()
{

}

void HSphereColliderComponent::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	SetScale( m_RigidBody.GetRadius() * 2, m_RigidBody.GetRadius() * 2, m_RigidBody.GetRadius() * 2 );
}

void HSphereColliderComponent::Serialize( WriteContext& Output )
{
	Output.Key( HE_STRINGIFY( HSphereColliderComponent ) );
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
		}
		Output.EndObject();
	}
	Output.EndArray();
}

void HSphereColliderComponent::Deserialize( const ReadContext& Value )
{
	Super::Deserialize( Value[0][HE_STRINGIFY( HColliderComponent )] );

	const ReadContext& This = Value[1];
	bool IsStatic;
	JsonUtility::GetBoolean( This, HE_STRINGIFY( m_RigidBody.m_IsStatic ), IsStatic );
	m_RigidBody.SetIsStatic( IsStatic );
	float Radius = -1.f;
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_Radius ), Radius );
	m_RigidBody.SetRadius( Radius );

	m_MeshAsset = GeometryGenerator::GenerateSphere(1, 10, 10);

	RegisterCollider( false );
}

void HSphereColliderComponent::RegisterCollider( bool StartDisabled )
{
	GetWorld()->AddSphereColliderComponent( this, StartDisabled, GetIsTrigger() );
}
