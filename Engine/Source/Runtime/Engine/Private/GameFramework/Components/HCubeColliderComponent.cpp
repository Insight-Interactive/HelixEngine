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

void HCubeColliderComponent::OnDestroy()
{
	Super::OnDestroy();

	UnregisterCollider();
}

void HCubeColliderComponent::OnEnter( HColliderComponent* Other )
{
	HE_LOG( Log, TEXT(  "------ %s Entered %s" ), GetOwner()->GetObjectName().c_str(), Other->GetOwner()->GetObjectName().c_str() );
}

void HCubeColliderComponent::OnExit( HColliderComponent* Other )
{
	HE_LOG( Log, TEXT( "------ %s Exit %s" ), GetOwner()->GetObjectName().c_str(), Other->GetOwner()->GetObjectName().c_str() );
}

void HCubeColliderComponent::OnStay( HColliderComponent* Other )
{
	HE_LOG( Log, TEXT( "------ %s Stay %s" ), GetOwner()->GetObjectName().c_str(), Other->GetOwner()->GetObjectName().c_str() );
}

void HCubeColliderComponent::Serialize( JsonUtility::WriteContext& Output )
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

		Output.StartObject();
		{
			Output.Key( HE_STRINGIFY( m_RigidBody.m_Width ) );
			Output.Double( m_RigidBody.GetHalfWidth() );

			Output.Key( HE_STRINGIFY( m_RigidBody.m_Height ) );
			Output.Double( m_RigidBody.GetHalfHeight() );

			Output.Key( HE_STRINGIFY( m_RigidBody.m_Length ) );
			Output.Double( m_RigidBody.GetHalfDepth() );
		}
		Output.EndObject();
	}
	Output.EndArray();
}

void HCubeColliderComponent::Deserialize( const JsonUtility::ReadContext& Value )
{
	Super::Deserialize( Value[0][HE_STRINGIFY( HColliderComponent )] );

	const JsonUtility::ReadContext& This = Value[1];
	FVector3 Dimensions;
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_HalfWidth ), Dimensions.x );
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_HalfHeight ), Dimensions.y );
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_HalfDepth ), Dimensions.z );
	
	SetHalfWidthHeightDepth( Dimensions.x, Dimensions.y, Dimensions.z );
	
	m_RigidBody.DisableSimulation();
}

void HCubeColliderComponent::RegisterCollider()
{
	GetWorld()->AddCubeColliderComponent( this, m_IsStatic, GetIsTrigger() );
}

void HCubeColliderComponent::UnregisterCollider()
{
	GetWorld()->RemoveColliderComponent( this );
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
