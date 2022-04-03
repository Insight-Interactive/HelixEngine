#include "EnginePCH.h"

#include "GameFramework/Components/HColliderComponentInterface.h"

#include "World/World.h"
#include "PhysicsScene.h"


HColliderComponentInterface::HColliderComponentInterface( FComponentInitArgs& InitArgs )
	: HSceneComponent( InitArgs )
{
}

HColliderComponentInterface::~HColliderComponentInterface()
{

}

void HColliderComponentInterface::BeginPlay() 
{
	Super::BeginPlay();
}

void HColliderComponentInterface::Tick( float DeltaTime ) 
{
	Super::Tick( DeltaTime );
	
	// Fetch the results of the simulation.
	SetPosition( GetRigidBody().GetSimulatedPosition() );
	SetRotation( GetRigidBody().GetSimulatedRotation() );
}

void HColliderComponentInterface::Serialize( WriteContext& Output )
{
	Output.Key( HE_STRINGIFY( HColliderComponentInterface ) );
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

void HColliderComponentInterface::Deserialize( const ReadContext& Value )
{
	Super::Deserialize( Value[0][HE_STRINGIFY( HSceneComponent )] );

}

void HColliderComponentInterface::OnCreate()
{
	Super::OnCreate(); 
}
