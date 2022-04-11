#pragma once

#include "TSingleton.h"

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
class HPhysicsScene;

class PHYSICS_API HPhysicsContext : TSingleton<HPhysicsContext>
{
public:
	HPhysicsContext();
	~HPhysicsContext();
	
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

FORCEINLINE bool HPhysicsContext::IsReady() const
{
	return	m_pFoundation != nullptr &&
			m_pPhysics != nullptr;
}

FORCEINLINE physx::PxPhysics& HPhysicsContext::GetPhysics()
{
	return *m_pPhysics;
}

FORCEINLINE physx::PxDefaultCpuDispatcher& HPhysicsContext::GetCpuDispatcher()
{
	return *m_pDispatcher;
}