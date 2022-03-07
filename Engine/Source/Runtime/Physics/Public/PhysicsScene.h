#pragma once

#include "CriticalSection.h"


enum EPhysicsEvent
{
	PE_Invalid,
	PE_PauseSimulation,
	PE_UnPauseSimulation,
	PE_SetSimulationStepRate,
	PE_AddSphereActor,
	PE_AddPlaneActor,
	PE_RemoveActor,
};

struct PhysicsEventPacket
{
	PhysicsEventPacket()
		: EventType( PE_Invalid )
		, pUserData( nullptr ) {}
	EPhysicsEvent EventType;
	void* pUserData;
};

class PhysicsEventQueue
{
public:
	PhysicsEventQueue() {}
	~PhysicsEventQueue() {}

	void PushEvent( PhysicsEventPacket& Event )
	{
		m_Queue.push( Event );
	}

	PhysicsEventPacket PopFront()
	{
		PhysicsEventPacket Front = m_Queue.front();
		m_Queue.pop();
		return Front;
	}

	std::queue<PhysicsEventPacket>& GetQueue()
	{
		return m_Queue;
	}

private:

	std::queue<PhysicsEventPacket> m_Queue;
};

namespace physx
{
	class PxScene;
}

class RigidBody;
class PlaneRigidBody;
class SphereRigidBody;
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
		PhysicsContext& InitContext;
		FVector3 StartPosition;
		SphereRigidBody& outSphereRB;
	};
	struct PlaneActorAddDesc
	{
		PhysicsContext& InitContext;
		FVector3 StartPosition;
		PlaneRigidBody& outPlaneRB;
	};
public:
	PhysicsScene();
	~PhysicsScene();

	bool IsValid() const;

	void Setup( PhysicsContext& PhysicsContext );
	void Flush();
	void Teardown();

	void ProcessEventQueue();
	// Steps the physics simulation through time.
	void Tick();
	bool IsSimulationFinished() const;
	bool IsSimulationPaused() const;

	void RequestPauseSimulation();
	void RequestUnPauseSimulation();
	void RequestSetStepRate( float NewStepRate );
	void RequestSphereActorAdd( SphereActorAddDesc& SphereInitInfo );
	void RequestPlaneActorAdd( PlaneActorAddDesc& PlaneInitInfo );
	void RequestActorRemove( RigidBody& RigidBody );

	// Blocks the thread and waits for the simulation to finish before proceeding.
	void WaitForSimulationFinished() const;
	// Blocks the thread and waits until the simulation is paused next.
	void WaitTillSimulationPaused() const;


private:
	void CreatePlane( PhysicsContext& PhysicsContext, const FVector3& StartPos, PlaneRigidBody& outPlane );
	void CreateSphere( PhysicsContext& PhysicsContext, const FVector3& StartPos, SphereRigidBody& outSphere );
	void RemoveActor( RigidBody& Collider );
	void InitDynamicBody( const DynamicColliderInitParams& InitParams, RigidBody& RigidBody );

protected:
	physx::PxScene* m_pScene;
	float m_SimulationStepRate;
	bool m_IsSimulationPaused;

	PhysicsEventQueue m_EventQueue;
};

// 
// Inline function implementations
//


FORCEINLINE bool PhysicsScene::IsValid() const
{
	return m_pScene != nullptr;
}

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
	Packet.pUserData = new SphereActorAddDesc{SphereInitInfo.InitContext, SphereInitInfo.StartPosition, SphereInitInfo.outSphereRB};
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void PhysicsScene::RequestPlaneActorAdd( PlaneActorAddDesc& PlaneInitInfo )
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_AddPlaneActor;
	Packet.pUserData = new PlaneActorAddDesc{PlaneInitInfo.InitContext, PlaneInitInfo.StartPosition, PlaneInitInfo.outPlaneRB};
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void PhysicsScene::RequestActorRemove( RigidBody& RigidBody )
{
	PhysicsEventPacket Packet;
	Packet.EventType = PE_RemoveActor;
	Packet.pUserData = &RigidBody;
	m_EventQueue.PushEvent( Packet );
}

FORCEINLINE void PhysicsScene::ProcessEventQueue()
{
	while (m_EventQueue.GetQueue().size() > 0)
	{
		PhysicsEventPacket Event = m_EventQueue.PopFront();
		switch (Event.EventType)
		{
		case PE_PauseSimulation:
			m_IsSimulationPaused = true;
			break;
		case PE_UnPauseSimulation:
			m_IsSimulationPaused = false;
			break;
		case PE_SetSimulationStepRate:
			m_SimulationStepRate = *(float*)Event.pUserData;
			break;
		case PE_AddSphereActor:
		{
			HE_ASSERT( Event.pUserData != nullptr ); // Sphere init info was not filled out correctly!
			SphereActorAddDesc& Desc = *(SphereActorAddDesc*)Event.pUserData;
			CreateSphere( Desc.InitContext, Desc.StartPosition, Desc.outSphereRB );
			delete Event.pUserData;
			break;
		}
		case PE_AddPlaneActor:
		{
			HE_ASSERT( Event.pUserData != nullptr ); // Sphere init info was not filled out correctly!
			PlaneActorAddDesc& Desc = *(PlaneActorAddDesc*)Event.pUserData;
			CreatePlane( Desc.InitContext, Desc.StartPosition, Desc.outPlaneRB );
			delete Event.pUserData;
			break;
		}
		case PE_RemoveActor:
		{
			RigidBody& RB = *(RigidBody*)Event.pUserData;
			RemoveActor( RB );
			break;
		}
		default:
			HE_ASSERT( false );
			break;
		}
	}
}

FORCEINLINE bool PhysicsScene::IsSimulationPaused() const
{
	return m_IsSimulationPaused;
}

FORCEINLINE void PhysicsScene::WaitTillSimulationPaused() const
{
	while (!m_IsSimulationPaused)
	{
		std::this_thread::yield();
	}
}

