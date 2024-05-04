// Copyright 2024 Insight Interactive. All Rights Reserved.
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
	HPhysicsContext& GetPhysicsContext();
	bool AddSceneForSimulation( HPhysicsScene& Scene );
	bool RemoveSceneFromSimulation( HPhysicsScene& Scene );

protected:
	FPhysicsSubsystem();
	virtual ~FPhysicsSubsystem();

	virtual void Initialize() override;
	virtual void UnInitialize() override;

private:
	virtual void RunAsync_Implementation() override;

private:
	HPhysicsContext m_PhysicsContext;
	
	std::vector<HPhysicsScene*> m_Scenes;
	CriticalSection m_SceneSimulationQueueGuard;

};

// 
// Inline function implementations
//

FORCEINLINE HPhysicsContext& FPhysicsSubsystem::GetPhysicsContext()
{
	return m_PhysicsContext;
}

FORCEINLINE bool FPhysicsSubsystem::AddSceneForSimulation( HPhysicsScene& Scene )
{
	ScopedCriticalSection Guard( m_SceneSimulationQueueGuard );

	auto Iter = std::find( m_Scenes.begin(), m_Scenes.end(), &Scene );
	if (Iter == m_Scenes.end())
	{
		m_Scenes.push_back( &Scene );
		return true;
	}
	HE_LOG( Warning, TEXT( "Trying to add a scene from simulation queue that was already added! Was this intended?" ) );
	HE_ASSERT( false );
	return false;
}

FORCEINLINE	bool FPhysicsSubsystem::RemoveSceneFromSimulation( HPhysicsScene& Scene )
{
	ScopedCriticalSection Guard( m_SceneSimulationQueueGuard );

	auto Iter = std::find( m_Scenes.begin(), m_Scenes.end(), &Scene );
	if (Iter != m_Scenes.end())
	{
		m_Scenes.erase( Iter );
		return true;
	}

	HE_LOG( Warning, TEXT( "Trying to remove a scene from simulation queue that was not registered!" ) );
	HE_ASSERT( false );

	return false;
}
