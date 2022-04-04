#include "EnginePCH.h"

#include "GameFramework/Components/HColliderComponent.h"

#include "World/World.h"
#include "PhysicsScene.h"


HColliderComponent::HColliderComponent( FComponentInitArgs& InitArgs )
	: HSceneComponent( InitArgs )
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
		}
		Output.EndObject();
	}
	Output.EndArray();
}

void HColliderComponent::Deserialize( const ReadContext& Value )
{
	Super::Deserialize( Value[0][HE_STRINGIFY( HSceneComponent )] );

}

void HColliderComponent::OnOwnerDeserializeComplete()
{
	GetRigidBody().SetGlobalPositionOrientation( GetAbsoluteWorldPosition(), GetRotation() );
}

void HColliderComponent::OnCreate()
{
	Super::OnCreate(); 
}
