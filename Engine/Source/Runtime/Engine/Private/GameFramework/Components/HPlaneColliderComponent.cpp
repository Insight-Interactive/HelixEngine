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

#if HE_DEBUG
	m_MeshAsset = GeometryGenerator::Generate1x1x1CubeMesh();
#endif

	SetWidthAndHeight( 1.f, 1.f );
}

void HPlaneColliderComponent::OnDestroy()
{
	Super::OnDestroy();

	UnRegisterCollider();
}

void HPlaneColliderComponent::Serialize( JsonUtility::WriteContext& Output )
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

void HPlaneColliderComponent::Deserialize( const JsonUtility::ReadContext& Value )
{
	Super::Deserialize( Value[0][HE_STRINGIFY( HColliderComponent )] );

	const JsonUtility::ReadContext& This = Value[1];
	float Width, Height;
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_HalfWidth ), Width );
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_HalfHeight ), Height );
	
	SetWidthAndHeight( Width, Height );

	m_RigidBody.DisableSimulation();
}

void HPlaneColliderComponent::RegisterCollider()
{
	GetWorld()->AddPlaneColliderComponent( this, m_IsStatic, GetIsTrigger() );
}

void HPlaneColliderComponent::UnRegisterCollider()
{
	GetWorld()->RemoveColliderComponent( this );
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
