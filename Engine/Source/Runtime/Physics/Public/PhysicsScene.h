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

	void RequestPauseSimulation();
	void RequestUnPauseSimulation();
	void RequestSetStepRate( float NewStepRate );
	void RequestSphereActorAdd( RigidActorAddDesc<HSphereRigidBody>& SphereInitInfo );
	void RequestPlaneActorAdd( RigidActorAddDesc<HPlaneRigidBody>& PlaneInitInfo );
	void RequestCubeActorAdd( RigidActorAddDesc<HCubeRigidBody>& CubeInitInfo );
	void RequestCapsuleActorAdd( RigidActorAddDesc<HCapsuleRigidBody>& CapsuleInitInfo );
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
	void CreatePlaneInternal( const FVector3& StartPos, HPlaneRigidBody& outPlane, bool IsTrigger );
	void CreateSphereInternal( const FVector3& StartPos, HSphereRigidBody& outSphere, bool IsTrigger );
	void CreateCubeInternal( const FVector3& StartPos, HCubeRigidBody& outCube, bool IsTrigger );
	void CreateCapsuleInternal( const FVector3& StartPos, HCapsuleRigidBody& outCube, bool IsTrigger );

	void InitCollider( HRigidBody& Collider, physx::PxGeometry& Geo, const FVector3& StartPos );
	void InitDynamicBody( const DynamicColliderInitParams& InitParams, HRigidBody& RigidBody );
	void FinalizeRigidBody( HRigidBody& outRB );

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

FORCEINLINE	void HPhysicsScene::RequestSphereActorAdd( RigidActorAddDesc<HSphereRigidBody>& SphereInitInfo )
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_AddSphereActor;
	Packet.pUserData = HE_HeapAlloc( sizeof( RigidActorAddDesc<HSphereRigidBody> ) );
	CopyMemory( Packet.pUserData, &SphereInitInfo, sizeof( RigidActorAddDesc<HSphereRigidBody> ) );
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void HPhysicsScene::RequestPlaneActorAdd( RigidActorAddDesc<HPlaneRigidBody>& PlaneInitInfo )
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_AddPlaneActor;
	Packet.pUserData = HE_HeapAlloc( sizeof( RigidActorAddDesc<HPlaneRigidBody> ) );
	CopyMemory( Packet.pUserData, &PlaneInitInfo, sizeof( RigidActorAddDesc<HPlaneRigidBody> ) );
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void HPhysicsScene::RequestCubeActorAdd( RigidActorAddDesc<HCubeRigidBody>& CubeInitInfo )
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_AddCubeActor;
	Packet.pUserData = HE_HeapAlloc( sizeof( RigidActorAddDesc<HCubeRigidBody> ) );
	CopyMemory( Packet.pUserData, &CubeInitInfo, sizeof( RigidActorAddDesc<HCubeRigidBody> ) );
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void HPhysicsScene::RequestCapsuleActorAdd( RigidActorAddDesc<HCapsuleRigidBody>& CapsuleInitInfo )
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_AddCapsuleActor;
	Packet.pUserData = HE_HeapAlloc( sizeof( RigidActorAddDesc<HCapsuleRigidBody> ) );
	CopyMemory( Packet.pUserData, &CapsuleInitInfo, sizeof( RigidActorAddDesc<HCapsuleRigidBody> ) );
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void HPhysicsScene::RequestActorRemove( HRigidBody& RigidBody )
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
