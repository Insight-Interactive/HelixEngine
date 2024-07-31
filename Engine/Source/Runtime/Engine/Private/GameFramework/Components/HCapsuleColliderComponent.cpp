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
}

void HCapsuleColliderComponent::OnDestroy()
{
	Super::OnDestroy();

	UnRegisterCollider();
}

void HCapsuleColliderComponent::Render( FCommandContext& GfxContext ) 
{
	Super::Render( GfxContext );
}

void HCapsuleColliderComponent::Serialize( JsonUtility::WriteContext& Output )
{
	Output.Key( HE_STRINGIFY( HCapsuleColliderComponent::m_RigidBody.m_Radius ) );
	Output.Double( m_RigidBody.GetRadius() );

	Output.Key( HE_STRINGIFY( HCapsuleColliderComponent::m_RigidBody.m_HalfHeight ) );
	Output.Double( m_RigidBody.GetHalfHeight() );

	Super::Serialize( Output );
}

void HCapsuleColliderComponent::Deserialize( const JsonUtility::ReadContext& Value )
{
	Super::Deserialize( Value );

	float Radius = m_RigidBody.GetRadius();
	JsonUtility::GetFloat( Value, HE_STRINGIFY( HCapsuleColliderComponent::m_RigidBody.m_Radius ), Radius );
	float HalfHeight = m_RigidBody.GetHalfHeight();
	JsonUtility::GetFloat( Value, HE_STRINGIFY( HCapsuleColliderComponent::m_RigidBody.m_HalfHeight ), HalfHeight );

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
