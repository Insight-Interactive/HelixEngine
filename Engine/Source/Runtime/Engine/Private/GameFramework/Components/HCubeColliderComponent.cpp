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

void HCubeColliderComponent::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	SetScale( m_RigidBody.GetWidth() * 2, m_RigidBody.GetHeight() * 2, m_RigidBody.GetDepth() * 2 );
}

void HCubeColliderComponent::OnEnter( HColliderComponent* Other )
{
	HE_LOG( Log, TEXT(  "%s Exited %s" ), GetOwner()->GetObjectName().c_str(), Other->GetOwner()->GetObjectName().c_str() );
}

void HCubeColliderComponent::OnExit( HColliderComponent* Other )
{
	HE_LOG( Log, TEXT( "%s Exit %s" ), GetOwner()->GetObjectName().c_str(), Other->GetOwner()->GetObjectName().c_str() );
}

void HCubeColliderComponent::OnStay( HColliderComponent* Other )
{
	HE_LOG( Log, TEXT( "%s Stay %s" ), GetOwner()->GetObjectName().c_str(), Other->GetOwner()->GetObjectName().c_str() );
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

	m_MeshAsset = GeometryGenerator::Generate1x1x1CubeMesh();

	RegisterCollider( false );
}

void HCubeColliderComponent::RegisterCollider( bool StartDisabled )
{
	GetWorld()->AddCubeColliderComponent( this, StartDisabled, GetIsTrigger() );
}
