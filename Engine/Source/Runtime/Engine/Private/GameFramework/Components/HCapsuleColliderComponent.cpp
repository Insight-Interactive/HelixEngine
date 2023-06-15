#include "EnginePCH.h"

#include "GameFramework/Components/HCapsuleColliderComponent.h"

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

	FQuat Rot = FQuat::CreateFromAxisAngle( FVector3::Forward, Math::DegreesToRadians( 90.f ) );
	HSceneComponent::SetRotation( Rot * GetRigidBody().GetSimulatedRotation() );
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
			Output.Key( HE_STRINGIFY( m_RigidBody.m_IsStatic ) );
			Output.Bool( m_RigidBody.GetIsStatic() );

			Output.Key( HE_STRINGIFY( m_RigidBody.m_Radius ) );
			Output.Double( m_RigidBody.GetRadius() );

			Output.Key( HE_STRINGIFY( m_RigidBody.m_Length ) );
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
	bool IsStatic;
	JsonUtility::GetBoolean( This, HE_STRINGIFY( m_RigidBody.m_IsStatic ), IsStatic );
	m_RigidBody.SetIsStatic( IsStatic );
	float Radius = -1.f;
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_Radius ), Radius );
	m_RigidBody.SetRadius( Radius );
	float Length = -1.f;
	JsonUtility::GetFloat( This, HE_STRINGIFY( m_RigidBody.m_Length ), Length );
	m_RigidBody.SetHalfHeight( Radius );

	RegisterCollider( false );
}

void HCapsuleColliderComponent::RegisterCollider( bool StartDisabled /*= false*/ )
{
	GetWorld()->AddCapsuleColliderComponent( this, StartDisabled, GetIsTrigger() );
}

void HCapsuleColliderComponent::UnRegisterCollider()
{
	GetWorld()->RemoveColliderComponent( this );
}
