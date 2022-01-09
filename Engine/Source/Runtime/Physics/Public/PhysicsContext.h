#pragma once

#include "CriticalSection.h"


namespace physx
{
	class PxFoundation;
	class PxPhysics;
	class PxCooking;
	class PxPvd;
	class PxDefaultAllocator;
	class PxDefaultCpuDispatcher;
}
class PhysicsScene;

class PHYSICS_API PhysicsContext
{
public:
	PhysicsContext();
	~PhysicsContext();
	
	bool IsReady() const;

	void Initialize();
	void UnInitialize();

	void Tick( float StepRate );
	void QueryResults();

	bool AddSceneForSimulation( PhysicsScene& Scene );
	bool RemoveSceneFromSimulation( PhysicsScene& Scene );

	physx::PxPhysics& GetPhysics();
	physx::PxDefaultCpuDispatcher& GetCpuDispatcher();

protected:
	std::vector<PhysicsScene*> m_Scenes;
	CriticalSection m_SceneSimulationQueueGuard;

	physx::PxFoundation* m_pFoundation;
	physx::PxPhysics* m_pPhysics;
	physx::PxDefaultCpuDispatcher* m_pDispatcher;

	physx::PxCooking* m_pCooker;

	physx::PxPvd* m_pVisualDebugger;

};

//
// Inline function implementations
//

FORCEINLINE bool PhysicsContext::IsReady() const
{
	return	m_pFoundation != nullptr &&
			m_pPhysics != nullptr;
}

FORCEINLINE bool PhysicsContext::AddSceneForSimulation( PhysicsScene& Scene )
{
	ScopedCriticalSection Guard( m_SceneSimulationQueueGuard );

	auto Iter = std::find( m_Scenes.begin(), m_Scenes.end(), &Scene );
	if (Iter == m_Scenes.end())
	{
		m_Scenes.push_back( &Scene );
		return true;
	}
	return false;
}

FORCEINLINE	bool PhysicsContext::RemoveSceneFromSimulation( PhysicsScene& Scene )
{
	ScopedCriticalSection Guard( m_SceneSimulationQueueGuard );

	auto Iter = std::find( m_Scenes.begin(), m_Scenes.end(), &Scene );
	if (Iter != m_Scenes.end())
	{
		m_Scenes.erase( Iter );
		return true;
	}

	return false;
}

FORCEINLINE physx::PxPhysics& PhysicsContext::GetPhysics()
{
	return *m_pPhysics;
}

FORCEINLINE physx::PxDefaultCpuDispatcher& PhysicsContext::GetCpuDispatcher()
{
	return *m_pDispatcher;
}