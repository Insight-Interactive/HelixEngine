#pragma once

#include "Engine/Subsystem/SubsystemInterface.h"

#include "PhysicsContext.h"


/*
	Handles all physics interactions in the world.
*/
class FPhysicsSubsystem : public FSubsystemInterface
{
	friend class HEngine;
public:
	PhysicsContext& GetPhysicsContext();
	void AddSceneForSimulation( PhysicsScene& Scene );
	void RemoveSceneFromSimulation( PhysicsScene& Scene );

protected:
	FPhysicsSubsystem();
	virtual ~FPhysicsSubsystem();

	virtual void Initialize() override;
	virtual void UnInitialize() override;

private:
	virtual void RunAsync_Implementation() override;

private:
	PhysicsContext m_PhysicsContext;

};

// 
// Inline function implementations
//

FORCEINLINE PhysicsContext& FPhysicsSubsystem::GetPhysicsContext()
{
	return m_PhysicsContext;
}

FORCEINLINE	void FPhysicsSubsystem::AddSceneForSimulation( PhysicsScene& Scene )
{
	if (!m_PhysicsContext.AddSceneForSimulation( Scene ))
	{
		HE_LOG( Warning, TEXT( "Trying to add a scene from simulation queue that was already added! Was this intended?" ) );
		HE_ASSERT( false );
	}
}

FORCEINLINE void FPhysicsSubsystem::RemoveSceneFromSimulation( PhysicsScene& Scene )
{
	if (!m_PhysicsContext.RemoveSceneFromSimulation( Scene ))
	{
		HE_LOG( Warning, TEXT( "Trying to remove a scene from simulation queue that was not registered!" ) );
		HE_ASSERT( false );
	}
}
