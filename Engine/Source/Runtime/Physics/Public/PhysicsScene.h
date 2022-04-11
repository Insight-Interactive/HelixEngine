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

	// Request the scene to begin simulating.
	PE_TickScene,
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
	PhysicsEventQueue() {}
	~PhysicsEventQueue() {}

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

private:
	std::queue<PhysicsEventPacket> m_Queue;

};

namespace physx
{
	class PxScene;
	class PxGeometry;
}

class RigidBody;
class InfinitePlaneRigidBody;
class PlaneRigidBody;
class SphereRigidBody;
class CubeRigidBody;
class CapsuleRigidBody;
class HPhysicsContext;
class PhysicsCallbackHandler;


class PHYSICS_API HPhysicsScene
{
private:
	struct DynamicColliderInitParams
	{
		FVector3 StartPos;
	};
public:
	template <typename RigidBodyType>
	struct RigidActorAddDesc
	{
		// The callback handler for this actor's collision events.
		PhysicsCallbackHandler* pCallbackHandler;
		// The starting position for this actor.
		FVector3 StartPosition;
		// The rigid body to initialize and add to the world.
		RigidBodyType& outRB;
		// Start disabled in the world?
		bool StartDisabled;
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

	void RequestPauseSimulation();
	void RequestUnPauseSimulation();
	void RequestSetStepRate( float NewStepRate );
	void RequestSphereActorAdd( RigidActorAddDesc<SphereRigidBody>& SphereInitInfo );
	void RequestPlaneActorAdd( RigidActorAddDesc<PlaneRigidBody>& PlaneInitInfo );
	void RequestCubeActorAdd( RigidActorAddDesc<CubeRigidBody>& CubeInitInfo );
	void RequestCapsuleActorAdd( RigidActorAddDesc<CapsuleRigidBody>& CapsuleInitInfo );
	void RequestActorRemove( RigidBody& RigidBody );
	void RequestSceneFlush();
	void RequestTick();

	// Blocks the thread and waits for the simulation to finish before proceeding.
	void WaittillSimulationFinished() const;
	// Blocks the thread and waits until the simulation is paused next.
	void WaittillSimulationPaused() const;


private:
	void CreateInfinitePlaneInternal( InfinitePlaneRigidBody& outPlane );
	void CreatePlaneInternal( const FVector3& StartPos, PlaneRigidBody& outPlane );
	void CreateSphereInternal( const FVector3& StartPos, SphereRigidBody& outSphere );
	void CreateCubeInternal( const FVector3& StartPos, CubeRigidBody& outCube );
	void CreateCapsuleInternal( const FVector3& StartPos, CapsuleRigidBody& outCube );
	void RemoveActorInternal( RigidBody& Collider );

	void InitCollider( RigidBody& Collider, physx::PxGeometry& Geo, const FVector3& StartPos );
	void InitDynamicBody( const DynamicColliderInitParams& InitParams, RigidBody& RigidBody );
	void FinalizeRigidBody( RigidBody& outRB );

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


FORCEINLINE void HPhysicsScene::RequestPauseSimulation()
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_PauseSimulation;
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void HPhysicsScene::RequestUnPauseSimulation()
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_UnPauseSimulation;
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void HPhysicsScene::RequestSetStepRate( float NewStepRate )
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_SetSimulationStepRate;
	Packet.pUserData = &NewStepRate;
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE	void HPhysicsScene::RequestSphereActorAdd( RigidActorAddDesc<SphereRigidBody>& SphereInitInfo )
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_AddSphereActor;
	Packet.pUserData = HE_HeapAlloc( sizeof( RigidActorAddDesc<SphereRigidBody> ) );
	CopyMemory( Packet.pUserData, &SphereInitInfo, sizeof( RigidActorAddDesc<SphereRigidBody> ) );
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void HPhysicsScene::RequestPlaneActorAdd( RigidActorAddDesc<PlaneRigidBody>& PlaneInitInfo )
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_AddPlaneActor;
	Packet.pUserData = HE_HeapAlloc( sizeof( RigidActorAddDesc<PlaneRigidBody> ) );
	CopyMemory( Packet.pUserData, &PlaneInitInfo, sizeof( RigidActorAddDesc<PlaneRigidBody> ) );
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void HPhysicsScene::RequestCubeActorAdd( RigidActorAddDesc<CubeRigidBody>& CubeInitInfo )
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_AddCubeActor;
	Packet.pUserData = HE_HeapAlloc( sizeof( RigidActorAddDesc<CubeRigidBody> ) );
	CopyMemory( Packet.pUserData, &CubeInitInfo, sizeof( RigidActorAddDesc<CubeRigidBody> ) );
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void HPhysicsScene::RequestCapsuleActorAdd( RigidActorAddDesc<CapsuleRigidBody>& CapsuleInitInfo )
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_AddCapsuleActor;
	Packet.pUserData = HE_HeapAlloc( sizeof( RigidActorAddDesc<CapsuleRigidBody> ) );
	CopyMemory( Packet.pUserData, &CapsuleInitInfo, sizeof( RigidActorAddDesc<CapsuleRigidBody> ) );
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void HPhysicsScene::RequestActorRemove( RigidBody& RigidBody )
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_RemoveActor;
	Packet.pUserData = &RigidBody;
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void HPhysicsScene::RequestSceneFlush()
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_FlushScene;
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void HPhysicsScene::RequestTick()
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_TickScene;
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE bool HPhysicsScene::IsSimulationPaused() const
{
	return m_IsSimulationPaused.IsSet();
}

FORCEINLINE void HPhysicsScene::WaittillSimulationPaused() const
{
	while (!m_IsSimulationPaused.IsSet())
		std::this_thread::yield();
}
