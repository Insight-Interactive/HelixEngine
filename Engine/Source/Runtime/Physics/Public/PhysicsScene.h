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
class PhysicsContext;

class PHYSICS_API PhysicsScene
{
private:
	struct DynamicColliderInitParams
	{
		FVector3 StartPos;
	};
public:
	struct SphereActorAddDesc
	{
		FVector3 StartPosition;
		SphereRigidBody& outSphereRB;
		bool StartDisabled;
	};
	struct PlaneActorAddDesc
	{
		FVector3 StartPosition;
		PlaneRigidBody& outPlaneRB;
		bool StartDisabled;
	};
	struct CubeActorAddDesc
	{
		FVector3 StartPosition;
		CubeRigidBody& outCubeRB;
		bool StartDisabled;
	};
public:
	PhysicsScene();
	~PhysicsScene();

	bool IsValid() const;

	void Setup( PhysicsContext& PhysicsContext );
	void Teardown();
	void Tick();

	void ProcessEventQueue();
	// Steps the physics simulation through time.
	bool IsSimulationFinished() const;
	bool IsSimulationPaused() const;

	void RequestPauseSimulation();
	void RequestUnPauseSimulation();
	void RequestSetStepRate( float NewStepRate );
	void RequestSphereActorAdd( SphereActorAddDesc& SphereInitInfo );
	void RequestPlaneActorAdd( PlaneActorAddDesc& PlaneInitInfo );
	void RequestCubeActorAdd( CubeActorAddDesc& CubeInitInfo );
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
	void RemoveActorInternal( RigidBody& Collider );
	void InitCollider( RigidBody& Collider, physx::PxGeometry& Geo, const FVector3& StartPos );
	void InitDynamicBody( const DynamicColliderInitParams& InitParams, RigidBody& RigidBody );
	void FinalizeRigidBody( RigidBody& outRB );
	void FlushInternal();
	void TickInternal();

protected:
	PhysicsContext* m_pOwningContextRef;

	physx::PxScene* m_pScene;
	float m_SimulationStepRate;
	FFlag m_IsSimulationPaused;
	FFlag m_IsSimulating;

	PhysicsEventQueue m_EventQueue;

};

// 
// Inline function implementations
//


FORCEINLINE void PhysicsScene::RequestPauseSimulation()
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_PauseSimulation;
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void PhysicsScene::RequestUnPauseSimulation()
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_UnPauseSimulation;
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void PhysicsScene::RequestSetStepRate( float NewStepRate )
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_SetSimulationStepRate;
	Packet.pUserData = &NewStepRate;
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE	void PhysicsScene::RequestSphereActorAdd( SphereActorAddDesc& SphereInitInfo )
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_AddSphereActor;
	Packet.pUserData = new SphereActorAddDesc{SphereInitInfo.StartPosition, SphereInitInfo.outSphereRB, SphereInitInfo.StartDisabled};
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void PhysicsScene::RequestPlaneActorAdd( PlaneActorAddDesc& PlaneInitInfo )
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_AddPlaneActor;
	Packet.pUserData = new PlaneActorAddDesc{PlaneInitInfo.StartPosition, PlaneInitInfo.outPlaneRB, PlaneInitInfo.StartDisabled};
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void PhysicsScene::RequestCubeActorAdd( CubeActorAddDesc& CubeInitInfo )
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_AddCubeActor;
	Packet.pUserData = new CubeActorAddDesc{ CubeInitInfo.StartPosition, CubeInitInfo.outCubeRB, CubeInitInfo.StartDisabled };
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void PhysicsScene::RequestActorRemove( RigidBody& RigidBody )
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_RemoveActor;
	Packet.pUserData = &RigidBody;
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void PhysicsScene::RequestSceneFlush()
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_FlushScene;
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void PhysicsScene::RequestTick()
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_TickScene;
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE bool PhysicsScene::IsSimulationPaused() const
{
	return m_IsSimulationPaused.IsSet();
}

FORCEINLINE void PhysicsScene::WaittillSimulationPaused() const
{
	while (!m_IsSimulationPaused.IsSet())
		std::this_thread::yield();
}
