#pragma once

#include "CriticalSection.h"


namespace physx
{
	class PxPvd;
	class PxPhysics;
	class PxCooking;
	class PxFoundation;
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

	physx::PxPhysics& GetPhysics();
	physx::PxDefaultCpuDispatcher& GetCpuDispatcher();

protected:
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

FORCEINLINE physx::PxPhysics& PhysicsContext::GetPhysics()
{
	return *m_pPhysics;
}

FORCEINLINE physx::PxDefaultCpuDispatcher& PhysicsContext::GetCpuDispatcher()
{
	return *m_pDispatcher;
}