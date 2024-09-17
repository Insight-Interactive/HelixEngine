#include "EnginePCH.h"

#include "GameFramework/Components/HSphereColliderComponent.h"

#include "World/World.h"
#include "Engine/Engine.h"
#include "Graphics/GeometryGenerator.h"


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

void HSphereColliderComponent::Serialize( JsonUtility::WriteContext& Output )
{
	Output.Key( HE_STRINGIFY( m_RigidBody.m_Radius ) );
	Output.Double( m_RigidBody.GetRadius() );

	Super::Serialize( Output );
}

void HSphereColliderComponent::Deserialize( const JsonUtility::ReadContext& Value )
{
	Super::Deserialize( Value );

	float Radius = m_RigidBody.GetRadius();
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
	Physics::CreateSphere(
		m_Transform.GetWorldPosition(),
		m_Transform.GetRotation(),
		m_RigidBody,
		m_IsTrigger,
		(PhysicsCallbackHandler*)this,
		false,
		10.f,
		m_IsStatic, FG_WorldGeometry );
}
