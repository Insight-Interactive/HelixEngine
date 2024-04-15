#include "EnginePCH.h"

#include "GameFramework/Components/HPlaneColliderComponent.h"

#include "World/World.h"
#include "Engine/Engine.h"
#include "Renderer/GeometryGenerator.h"


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
	m_MeshAsset = GeometryGenerator::Generate1x1x1CubeMesh();

}

void HPlaneColliderComponent::OnDestroy()
{
	Super::OnDestroy();

	UnRegisterCollider();
}

void HPlaneColliderComponent::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);

	SetScale( m_RigidBody.GetHalfWidth(), HPlaneRigidBody::GetConstantDepth(), m_RigidBody.GetHalfHeight() );
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
			Output.Key( HE_STRINGIFY( m_Width ) );
			Output.Double( m_RigidBody.GetHalfWidth() );

			Output.Key( HE_STRINGIFY( m_Height ) );
			Output.Double( m_RigidBody.GetHalfHeight() );
		}
		Output.EndObject();
	}
	Output.EndArray();
}

void HPlaneColliderComponent::Deserialize( const ReadContext& Value )
{
	Super::Deserialize( Value[0][HE_STRINGIFY( HColliderComponent )] );

	const ReadContext& This = Value[1];
	float Width, Height;
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_HalfWidth ), Width );
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_HalfHeight ), Height );
	m_RigidBody.SetHalfWidth( Width );
	m_RigidBody.SetHalfHeight( Height );

	SetScale( Width, HPlaneRigidBody::GetConstantDepth(), Height );

	RegisterCollider();
}

void HPlaneColliderComponent::RegisterCollider()
{
	GetWorld()->AddPlaneColliderComponent( this, m_IsStatic, GetIsTrigger() );
}

void HPlaneColliderComponent::UnRegisterCollider()
{
	GetWorld()->RemoveColliderComponent( this );
}
