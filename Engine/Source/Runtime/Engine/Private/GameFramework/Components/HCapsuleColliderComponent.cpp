#include "EnginePCH.h"

#include "GameFramework/Components/HCapsuleColliderComponent.h"

#include "World/World.h"
#include "Engine/Engine.h"
#include "Renderer/GeometryGenerator.h"


HCapsuleColliderComponent::HCapsuleColliderComponent( FComponentInitArgs& InitArgs )
	: HColliderComponent( InitArgs )
{
}

HCapsuleColliderComponent::~HCapsuleColliderComponent()
{
}

void HCapsuleColliderComponent::OnCreate()
{
	Super::OnCreate();

#if HE_DEBUG
	m_MeshAsset = FAssetDatabase::GetStaticMesh( "Capsule01.fbx");
#endif

	SetRadiusAndHalfHeight( 5.f, 5.f );

	m_RigidBody.DisableSimulation();
	UpdateDebugRotation();
}

void HCapsuleColliderComponent::OnDestroy()
{
	Super::OnDestroy();

	UnRegisterCollider();
}

void HCapsuleColliderComponent::Render( FCommandContext& GfxContext ) 
{
	UpdateDebugRotation();

	Super::Render( GfxContext );
}

void HCapsuleColliderComponent::Serialize( JsonUtility::WriteContext& Output )
{
	Output.Key( HE_STRINGIFY( HCapsuleColliderComponent ) );
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

			Output.Key( HE_STRINGIFY( m_RigidBody.m_HalfHeight ) );
			Output.Double( m_RigidBody.GetHalfHeight() );
		}
		Output.EndObject();
	}
	Output.EndArray();
}

void HCapsuleColliderComponent::Deserialize( const JsonUtility::ReadContext& Value )
{
	Super::Deserialize( Value[0][HE_STRINGIFY( HColliderComponent )] );

	const JsonUtility::ReadContext& This = Value[1];
	float Radius = 1.f;
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_Radius ), Radius );
	float HalfHeight = 2.f;
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_HalfHeight ), HalfHeight );

	SetRadiusAndHalfHeight( Radius, HalfHeight );
	
	m_RigidBody.DisableSimulation();
}

void HCapsuleColliderComponent::RegisterCollider()
{
	GetWorld()->AddCapsuleColliderComponent( this, m_IsStatic, GetIsTrigger() );
}

void HCapsuleColliderComponent::UnRegisterCollider()
{
	GetWorld()->RemoveColliderComponent( this );
}

void HCapsuleColliderComponent::SetRadius( float NewRadius )
{
	SetRadiusAndHalfHeight( NewRadius, m_RigidBody.GetHalfHeight() );
}

void HCapsuleColliderComponent::SetHalfHeight( float NewHalfHeight )
{
	SetRadiusAndHalfHeight( m_RigidBody.GetRadius(), NewHalfHeight );
}

void HCapsuleColliderComponent::SetRadiusAndHalfHeight( float NewRadius, float NewHalfHeight )
{
	m_RigidBody.SetRadius( NewRadius );
	m_RigidBody.SetHalfHeight( NewHalfHeight );
	SetScale( m_RigidBody.GetRadius() * 2, GetHeight(), m_RigidBody.GetRadius() * 2 );

	RegisterCollider();
}
