#include "EnginePCH.h"

#include "GameFramework/Components/HColliderComponent.h"

#include "World/World.h"
#include "PhysicsScene.h"


HColliderComponent::HColliderComponent( FComponentInitArgs& InitArgs )
	: HSceneComponent( InitArgs )
	, m_IsTrigger( false )
{
}

HColliderComponent::~HColliderComponent()
{

}

void HColliderComponent::BeginPlay() 
{
	Super::BeginPlay();
}

void HColliderComponent::Tick( float DeltaTime ) 
{
	Super::Tick( DeltaTime );
	
	// Fetch the results of the simulation.
	SetPosition( GetRigidBody().GetSimulatedPosition() );
	SetRotation( GetRigidBody().GetSimulatedRotation() );
}

void HColliderComponent::Serialize( WriteContext& Output )
{
	Output.Key( HE_STRINGIFY( HColliderComponent ) );
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
			Output.Key( HE_STRINGIFY( m_IsTrigger ) );
			Output.Bool( m_IsTrigger );
		}
		Output.EndObject();
	}
	Output.EndArray();
}

void HColliderComponent::Deserialize( const ReadContext& Value )
{
	Super::Deserialize( Value[0][HE_STRINGIFY( HSceneComponent )] );

	const ReadContext& This = Value[1];
	JsonUtility::GetBoolean( This, HE_STRINGIFY( m_IsTrigger ), m_IsTrigger );
}

void HColliderComponent::OnOwnerDeserializeComplete()
{
	GetRigidBody().SetGlobalPositionOrientation( GetAbsoluteWorldPosition(), GetRotation() );
}

void HColliderComponent::OnCreate()
{
	Super::OnCreate(); 
}
