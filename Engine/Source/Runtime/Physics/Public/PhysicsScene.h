#pragma once

#include "Flag.h"
#include "CriticalSection.h"


enum EPhysicsEvent
{
	// Invalid event.
	PE_Invalid,
	// Pause the entire simulation.
	PE_PauseSimulation,
	// Unpause the entire simulation.
	PE_UnPauseSimulation,
	// Set the interval at which the scene is steped through time.
	PE_SetSimulationStepRate,
	// Request the scene to begin simulating.
	PE_TickScene,
	
	// Add a sphere physics actor to the scene.
	PE_AddSphereActor,
	// Add a plane physics actor to the scene.
	PE_AddPlaneActor,
	// Add a cube physics actor to the scene.
	PE_AddCubeActor,
	// Add a capsule physics actor to the scene.
	PE_AddCapsuleActor,
	// Remove a physics actor from the scene.
	PE_RemoveActor,
	// Flsuh all actors from the scene.
	PE_FlushScene,
};

struct PHYSICS_API PhysicsEventPacket
{
	PhysicsEventPacket()
		: EventType( PE_Invalid )
		, pUserData( nullptr ) {}
	EPhysicsEvent EventType;
	void* pUserData;
};

class PHYSICS_API PhysicsEventQueue
{
public:
	PhysicsEventQueue() 
	{
	}
	~PhysicsEventQueue() 
	{
	}

	FORCEINLINE void PushEvent( PhysicsEventPacket& Event )
	{
		m_Queue.push( Event );
	}

	FORCEINLINE PhysicsEventPacket PopFront()
	{
		PhysicsEventPacket Front = m_Queue.front();
		m_Queue.pop();
		return Front;
	}

	FORCEINLINE std::queue<PhysicsEventPacket>& GetQueue()
	{
		return m_Queue;
	}

	FORCEINLINE size_t GetNumEvents() const
	{
		return m_Queue.size();
	}

private:
	std::queue<PhysicsEventPacket> m_Queue;

};

namespace physx
{
	class PxScene;
	class PxGeometry;
}

class HRigidBody;
class HInfinitePlaneRigidBody;
class HPlaneRigidBody;
class HSphereRigidBody;
class HCubeRigidBody;
class HCapsuleRigidBody;
class HTrigger;
class HBoxTrigger;
class HPhysicsContext;
class PhysicsCallbackHandler;


class PHYSICS_API HPhysicsScene
{
private:
	struct DynamicColliderInitParams
	{
		FVector3 StartPos;
		FQuat StartRotation;
		bool IsKinematic;
	};
public:
	template <typename RigidBodyType>
	struct RigidActorAddDesc
	{
		// The callback handler for this actor's collision events.
		PhysicsCallbackHandler* pCallbackHandler;
		// The starting position for this actor.
		FVector3 StartPosition;
		// The starting rotation for this actor.
		FQuat StartRotation;
		// The rigid body to initialize and add to the world.
		RigidBodyType& outRB;
		// Start disabled in the world?
		bool StartDisabled;
		// Is this actor a trigger?
		bool IsTrigger;
	};

public:
	HPhysicsScene();
	~HPhysicsScene();

	bool IsValid() const;

	void Setup( HPhysicsContext& PhysicsContext );
	void Teardown();
	void Tick();

	void ProcessEventQueue();
	// Steps the physics simulation through time.
	bool IsSimulationFinished() const;
	bool IsSimulationPaused() const;

	void CreateSphere( const FVector3& StartPos, const FQuat& StartRotation, HSphereRigidBody& outSphere, bool IsTrigger, void* pUserData, bool IsKinematic );
	void CreatePlane( const FVector3& StartPos, const FQuat& StartRotation, HPlaneRigidBody& outPlane, bool IsTrigger, void* pUserData, bool IsKinematic );
	void CreateCube( const FVector3& StartPos, const FQuat& StartRotation, HCubeRigidBody& outCube, bool IsTrigger, void* pUserData, bool IsKinematic );
	void CreateCapsule( const FVector3& StartPos, const FQuat& StartRotation, HCapsuleRigidBody& outCube, bool IsTrigger, void* pUserData, bool IsKinematic );


	void RequestPauseSimulation();
	void RequestUnPauseSimulation();
	void RequestSetStepRate( float NewStepRate );
	void RequestActorRemove( HRigidBody& RigidBody );
	void RequestSceneFlush();
	void RequestTick();

	// Blocks the thread and waits for the simulation to finish before proceeding.
	void WaittillSimulationFinished() const;
	// Blocks the thread and waits until the simulation is paused next.
	void WaittillSimulationPaused() const;
	void RemoveActor( HRigidBody& Collider );


private:
	void CreateInfinitePlaneInternal( HInfinitePlaneRigidBody& outPlane, bool IsTrigger );

	void InitRigidBody( HRigidBody& outRB, const physx::PxGeometry& Geo, const FVector3& StartPos, const FQuat& StartRotation, bool IsTrigger, void* pUserData, bool IsKinematic );

	void FlushInternal();
	void TickInternal();

protected:
	HPhysicsContext* m_pOwningContextRef;

	physx::PxScene* m_pScene;
	float m_SimulationStepRate;
	FFlag m_IsSimulationPaused;
	FFlag m_IsSimulating;

	PhysicsEventQueue m_EventQueue;

};

// 
// Inline function implementations
//


