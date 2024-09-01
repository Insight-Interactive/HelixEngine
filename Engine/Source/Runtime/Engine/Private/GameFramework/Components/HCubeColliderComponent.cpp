#include "EnginePCH.h"

#include "GameFramework/Components/HCubeColliderComponent.h"

#include "World/World.h"
#include "Engine/Engine.h"
#include "CommandContext.h"
#include "Renderer/ConstantBufferStructures.h"
#include "Renderer/ShaderRegisters.h"
#include "Renderer/GeometryGenerator.h"


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
	
#if HE_DEBUG
	m_MeshAsset = GeometryGenerator::Generate1x1x1CubeMesh();
#endif

	SetHalfWidthHeightDepth( 1.f, 1.f, 1.f );
	m_RigidBody.DisableSimulation();
}

void HCubeColliderComponent::OnEnter( HColliderComponent* Other )
{
	HE_LOG( Log, TEXT(  "------ %s Entered %s" ), GetOwner()->GetObjectName(), Other->GetOwner()->GetObjectName() );
}

void HCubeColliderComponent::OnExit( HColliderComponent* Other )
{
	HE_LOG( Log, TEXT( "------ %s Exit %s" ), GetOwner()->GetObjectName(), Other->GetOwner()->GetObjectName() );
}

void HCubeColliderComponent::OnStay( HColliderComponent* Other )
{
	HE_LOG( Log, TEXT( "------ %s Stay %s" ), GetOwner()->GetObjectName(), Other->GetOwner()->GetObjectName() );
}

void HCubeColliderComponent::Serialize( JsonUtility::WriteContext& Output )
{
	Output.Key( HE_STRINGIFY( m_RigidBody.m_Width ) );
	Output.Double( m_RigidBody.GetHalfWidth() );

	Output.Key( HE_STRINGIFY( m_RigidBody.m_Height ) );
	Output.Double( m_RigidBody.GetHalfHeight() );

	Output.Key( HE_STRINGIFY( m_RigidBody.m_Length ) );
	Output.Double( m_RigidBody.GetHalfDepth() );

	Super::Serialize( Output );
}

void HCubeColliderComponent::Deserialize( const JsonUtility::ReadContext& Value )
{
	Super::Deserialize( Value );

	FVector3 Dimensions( m_RigidBody.GetHalfWidth(), m_RigidBody.GetHalfHeight(), m_RigidBody.GetHalfDepth() );
	JsonUtility::GetFloat( Value, HE_STRINGIFY( HCubeColliderComponent::m_RigidBody.m_HalfWidth ), Dimensions.x );
	JsonUtility::GetFloat( Value, HE_STRINGIFY( HCubeColliderComponent::m_RigidBody.m_HalfHeight ), Dimensions.y );
	JsonUtility::GetFloat( Value, HE_STRINGIFY( HCubeColliderComponent::m_RigidBody.m_HalfDepth ), Dimensions.z );
	
	SetHalfWidthHeightDepth( Dimensions.x, Dimensions.y, Dimensions.z );
	
	m_RigidBody.DisableSimulation();
}

void HCubeColliderComponent::RegisterCollider()
{
	Physics::CreateCube(
		GetWorldPosition(),
		GetRotation(),
		m_RigidBody,
		GetIsTrigger(),
		(PhysicsCallbackHandler*)this,
		false,
		10.f,
		m_IsStatic, FG_WorldGeometry );
}

void HCubeColliderComponent::SetHalfWidth( float NewHalfWidth )
{
	SetHalfWidthHeightDepth( NewHalfWidth, m_RigidBody.GetHalfHeight(), m_RigidBody.GetHalfDepth() );
}

void HCubeColliderComponent::SetHalfHeight( float NewHalfHeight )
{
	SetHalfWidthHeightDepth( m_RigidBody.GetHalfWidth(), NewHalfHeight, m_RigidBody.GetHalfDepth() );
}

void HCubeColliderComponent::SetHalfDepth( float NewHalfDepth )
{
	SetHalfWidthHeightDepth( m_RigidBody.GetHalfWidth(), m_RigidBody.GetHalfHeight(), NewHalfDepth );
}

void HCubeColliderComponent::SetHalfWidthHeightDepth( float NewHalfWidth, float NewHalfHeight, float NewHalfDepth )
{
	m_RigidBody.SetHalfWidth( NewHalfWidth );
	m_RigidBody.SetHalfHeight( NewHalfHeight );
	m_RigidBody.SetHalfDepth( NewHalfDepth );
	SetScale( NewHalfWidth, NewHalfHeight, NewHalfDepth );

	RegisterCollider();
}
