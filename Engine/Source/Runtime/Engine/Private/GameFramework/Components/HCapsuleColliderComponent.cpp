#include "EnginePCH.h"

#include "GameFramework/Components/HCapsuleColliderComponent.h"

#include "World/World.h"
#include "Engine/Engine.h"
#include "Renderer/GeometryGenerator.h"


HCapsuleColliderComponent::HCapsuleColliderComponent( FComponentInitArgs& InitArgs )
	: HColliderComponent( InitArgs )
{
	UpdateRotation();
}

HCapsuleColliderComponent::~HCapsuleColliderComponent()
{

}

void HCapsuleColliderComponent::OnCreate()
{
	Super::OnCreate();

	m_MeshAsset = FAssetDatabase::GetStaticMesh( FGUID( "73f5ee42-1b58-4a16-96ff-d26f0bd11a6f" ) );

	m_RigidBody.SetRadius( 5.f );
	m_RigidBody.SetHalfHeight( 5.f );
	
	float CapsuleFullLength = (m_RigidBody.GetHalfHeight() ) + (m_RigidBody.GetRadius() );

	SetScale( m_RigidBody.GetRadius() * 2, CapsuleFullLength, m_RigidBody.GetRadius() * 2 );

	FQuat Rot = FQuat::CreateFromAxisAngle( FVector3::Forward, Math::DegreesToRadians( 90.f ) );
	HSceneComponent::SetRotation( Rot );


	// TODO: This is wrong
	RegisterCollider();
}

void HCapsuleColliderComponent::OnDestroy()
{
	Super::OnDestroy();

	UnRegisterCollider();
}

void HCapsuleColliderComponent::Tick( float Delta )
{
	Super::Tick( Delta );

	UpdateRotation();
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
	RegisterCollider();
}

void HCapsuleColliderComponent::Serialize( WriteContext& Output )
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

void HCapsuleColliderComponent::Deserialize( const ReadContext& Value )
{
	Super::Deserialize( Value[0][HE_STRINGIFY( HColliderComponent )] );

	const ReadContext& This = Value[1];
	float Radius = 0.f;
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_Radius ), Radius );
	float HalfHeight = 0.f;
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_HalfHeight ), HalfHeight );

	SetRadiusAndHalfHeight( Radius, HalfHeight );
	UpdateRotation();
}

void HCapsuleColliderComponent::RegisterCollider()
{
	GetWorld()->AddCapsuleColliderComponent( this, m_IsStatic, GetIsTrigger() );
	UpdateRotation();
}

void HCapsuleColliderComponent::UnRegisterCollider()
{
	GetWorld()->RemoveColliderComponent( this );
}

void HCapsuleColliderComponent::UpdateRotation()
{
	// Returned Physx quaternion seems to insist on adding 90 degrees of rotation arround the z axis for some reason. Reverse it here.
	FQuat Rot = FQuat::CreateFromAxisAngle( FVector3::Forward, Math::DegreesToRadians( 90.f ) );
	HSceneComponent::SetRotation( Rot * GetRigidBody().GetSimulatedRotation() );
}
