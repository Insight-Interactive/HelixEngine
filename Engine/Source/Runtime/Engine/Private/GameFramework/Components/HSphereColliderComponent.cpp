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

void HSphereColliderComponent::OnCreate()
{
	Super::OnCreate();

#if HE_DEBUG
	m_MeshAsset = GeometryGenerator::GenerateSphere( 1, 10, 10 );
#endif

	SetRadius( 1.f );
	m_RigidBody.DisableSimulation();
}

void HSphereColliderComponent::OnDestroy()
{
	Super::OnDestroy();

	UnRegisterCollider();
}

void HSphereColliderComponent::Serialize( JsonUtility::WriteContext& Output )
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
			Output.Key( HE_STRINGIFY( m_RigidBody.m_Radius ) );
			Output.Double( m_RigidBody.GetRadius() );
		}
		Output.EndObject();
	}
	Output.EndArray();
}

void HSphereColliderComponent::Deserialize( const JsonUtility::ReadContext& Value )
{
	Super::Deserialize( Value );

	float Radius = 1.f;
	JsonUtility::GetFloat( Value, HE_STRINGIFY( HSphereColliderComponent::m_RigidBody.m_Radius ), Radius );

	SetRadius( Radius );

	m_RigidBody.DisableSimulation();
}

void HSphereColliderComponent::SetRadius( float NewRadius )
{
	m_RigidBody.SetRadius( NewRadius );
	SetScale( m_RigidBody.GetRadius(), m_RigidBody.GetRadius(), m_RigidBody.GetRadius() );

	RegisterCollider();
}

void HSphereColliderComponent::RegisterCollider()
{
	GetWorld()->AddSphereColliderComponent( this, m_IsStatic, GetIsTrigger() );
}

void HSphereColliderComponent::UnRegisterCollider()
{
	GetWorld()->RemoveColliderComponent( this );
}