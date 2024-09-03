#include "EnginePCH.h"

#include "GameFramework/Components/HPlaneColliderComponent.h"

#include "World/World.h"
#include "Engine/Engine.h"
#include "Graphics/GeometryGenerator.h"


HPlaneColliderComponent::HPlaneColliderComponent( FComponentInitArgs& InitArgs )
	: HColliderComponent( InitArgs )
{

}

HPlaneColliderComponent::~HPlaneColliderComponent()
{

}

void HPlaneColliderComponent::OnCreate()
{
	Super::OnCreate();

#if HE_DEBUG
	m_MeshAsset = GeometryGenerator::Generate1x1x1CubeMesh();
#endif

	SetWidthAndHeight( 1.f, 1.f );
}

void HPlaneColliderComponent::Serialize( JsonUtility::WriteContext& Output )
{
	Output.Key( HE_STRINGIFY( m_Width ) );
	Output.Double( m_RigidBody.GetHalfWidth() );

	Output.Key( HE_STRINGIFY( m_Height ) );
	Output.Double( m_RigidBody.GetHalfHeight() );

	Super::Serialize( Output );
}

void HPlaneColliderComponent::Deserialize( const JsonUtility::ReadContext& Value )
{
	Super::Deserialize( Value );

	float Width = m_RigidBody.GetHalfWidth();
	float Height = m_RigidBody.GetHalfHeight();
	JsonUtility::GetFloat( Value, HE_STRINGIFY( HPlaneColliderComponent::m_RigidBody.m_HalfWidth ), Width );
	JsonUtility::GetFloat( Value, HE_STRINGIFY( HPlaneColliderComponent::m_RigidBody.m_HalfHeight ), Height );
	
	SetWidthAndHeight( Width, Height );

	m_RigidBody.DisableSimulation();
}

void HPlaneColliderComponent::RegisterCollider()
{
	Physics::CreatePlane(
		GetWorldPosition(),
		GetRotation(), (HPlaneRigidBody&)
		GetRigidBody(),
		GetIsTrigger(),
		(PhysicsCallbackHandler*)this,
		false,
		10.f,
		m_IsStatic, FG_WorldGeometry );
}

void HPlaneColliderComponent::SetWidth( float Width )
{
	SetWidthAndHeight( Width, m_RigidBody.GetHalfHeight() );
}

void HPlaneColliderComponent::SetHeight( float Depth )
{
	SetWidthAndHeight( m_RigidBody.GetHalfWidth(), Depth );
}

void HPlaneColliderComponent::SetWidthAndHeight( float Width, float Height )
{
	m_RigidBody.SetHalfWidth( Width );
	m_RigidBody.SetHalfHeight( Height );
	SetScale( Width, HPlaneRigidBody::GetConstantDepth(), Height );

	RegisterCollider();
}
