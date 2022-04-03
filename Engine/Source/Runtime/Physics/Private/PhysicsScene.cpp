#include "PhysicsPCH.h"

#include "PhysicsScene.h"

#include "PhysicsContext.h"
#include "RigidBody.h"


PhysicsScene::PhysicsScene()
	: m_pOwningContextRef( nullptr )
	, m_pScene( nullptr )
	, m_SimulationStepRate( 1.f / 60.f )
{

}

PhysicsScene::~PhysicsScene()
{

}

void PhysicsScene::Setup( PhysicsContext& PhysicsContext )
{
	HE_ASSERT( PhysicsContext.IsReady() ); // Trying to create a physics scene with PhysicsContext that has not been setup yet!
	HE_ASSERT( m_pScene == nullptr ); // Trying to initialize a scene that has already been initalized.

	m_pOwningContextRef = &PhysicsContext;
	physx::PxPhysics& Physics = m_pOwningContextRef->GetPhysics();

	physx::PxSceneDesc SceneDesc( Physics.getTolerancesScale() );
	SceneDesc.gravity = physx::PxVec3( 0.f, -9.81f, 0.f );
	SceneDesc.cpuDispatcher = &PhysicsContext.GetCpuDispatcher();
	SceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	m_pScene = Physics.createScene( SceneDesc );
	HE_ASSERT( m_pScene != nullptr );

	physx::PxPvdSceneClient* pPvd = m_pScene->getScenePvdClient();
	if (pPvd != nullptr)
	{
		pPvd->setScenePvdFlag( physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true );
		pPvd->setScenePvdFlag( physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true );
		pPvd->setScenePvdFlag( physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true );
	}

	m_pScene->addActor( *physx::PxCreatePlane( Physics, physx::PxPlane( 0, 1, 0, 0 ), *Physics.createMaterial( 0.5f, 0.5f, 0.6f ) ) );
}

bool PhysicsScene::IsValid() const
{
	return
		m_pOwningContextRef != nullptr &&
		m_pScene != nullptr &&
		m_pOwningContextRef->IsReady();
}

void PhysicsScene::ProcessEventQueue()
{
	while (m_EventQueue.GetQueue().size() > 0)
	{
		PhysicsEventPacket Event = m_EventQueue.PopFront();
		switch (Event.EventType)
		{
		case PE_PauseSimulation:
			m_IsSimulationPaused.Set();
			break;
		case PE_UnPauseSimulation:
			m_IsSimulationPaused.Clear();
			break;
		case PE_SetSimulationStepRate:
			m_SimulationStepRate = *(float*)Event.pUserData;
			break;
		case PE_AddSphereActor:
		{
			HE_ASSERT( Event.pUserData != nullptr ); // Sphere init info was not filled out correctly!
			SphereActorAddDesc& Desc = *(SphereActorAddDesc*)Event.pUserData;
			CreateSphereInternal( Desc.StartPosition, Desc.outSphereRB );
			if (Desc.StartDisabled)
				Desc.outSphereRB.DisableSimulation();

			delete Event.pUserData;
			break;
		}
		case PE_AddPlaneActor:
		{
			HE_ASSERT( Event.pUserData != nullptr ); // Sphere init info was not filled out correctly!
			PlaneActorAddDesc& Desc = *(PlaneActorAddDesc*)Event.pUserData;
			CreatePlaneInternal( Desc.StartPosition, Desc.outPlaneRB );
			if (Desc.StartDisabled)
				Desc.outPlaneRB.DisableSimulation();

			delete Event.pUserData;
			break;
		}
		case PE_AddCubeActor:
		{
			HE_ASSERT( Event.pUserData != nullptr ); // Sphere init info was not filled out correctly!
			CubeActorAddDesc& Desc = *(CubeActorAddDesc*)Event.pUserData;
			CreateCubeInternal( Desc.StartPosition, Desc.outCubeRB );
			if (Desc.StartDisabled)
				Desc.outCubeRB.DisableSimulation();

			delete Event.pUserData;
			break;
		}
		case PE_RemoveActor:
		{
			RigidBody& RB = *(RigidBody*)Event.pUserData;
			RemoveActorInternal( RB );
			break;
		}
		case PE_FlushScene:
			FlushInternal();
			break;
		case PE_TickScene:
			Tick();
			break;
		default:
			HE_ASSERT( false );
			break;
		}
	}
}

void PhysicsScene::Teardown()
{
	PX_SAFE_RELEASE( m_pScene );
}

void PhysicsScene::FlushInternal()
{
	m_pScene->flushSimulation();
}

void PhysicsScene::CreatePlaneInternal( const FVector3& StartPos, PlaneRigidBody& outPlane )
{
	HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

	physx::PxPhysics& Physics = m_pOwningContextRef->GetPhysics();

	outPlane.SetOwningScene( *m_pScene );

	physx::PxTransform InitialTransform = PxTransformFromPlaneEquation( physx::PxPlane( 0, 1, 0, 0 ) );
	outPlane.m_pPhysicsMaterial = Physics.createMaterial( 0.5f, 0.5f, 0.6f );
	if (outPlane.GetIsStatic())
	{
		outPlane.m_pRigidActor = physx::PxCreateStatic( Physics, InitialTransform, physx::PxPlaneGeometry(), *outPlane.m_pPhysicsMaterial );
	}
	else
	{
		outPlane.m_pRigidActor = physx::PxCreateDynamic( Physics, InitialTransform, physx::PxPlaneGeometry(), *outPlane.m_pPhysicsMaterial, 10.f );
		DynamicColliderInitParams InitParams = {};
		InitParams.StartPos = StartPos;
		InitDynamicBody( InitParams, outPlane );
	}

	HE_ASSERT( outPlane.m_pRigidActor != nullptr ); // Failed to create physics actor!

	m_pScene->addActor( *outPlane.m_pRigidActor );
	outPlane.OnCreate();
}

void PhysicsScene::CreateSphereInternal( const FVector3& StartPos, SphereRigidBody& outSphere )
{
	HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

	physx::PxPhysics& Physics = m_pOwningContextRef->GetPhysics();

	outSphere.SetOwningScene( *m_pScene );

	physx::PxSphereGeometry SphereGeo( outSphere.GetRadius() );
	physx::PxTransform InitialTransform = physx::PxTransform( physx::PxVec3( StartPos.x, StartPos.y, StartPos.z ) );
	outSphere.m_pPhysicsMaterial = Physics.createMaterial( 0.5f, 0.5f, 0.6f );
	if (outSphere.GetIsStatic())
	{
		outSphere.m_pRigidActor = physx::PxCreateStatic( Physics, InitialTransform, SphereGeo, *outSphere.m_pPhysicsMaterial );
	}
	else
	{
		outSphere.m_pRigidActor = physx::PxCreateDynamic( Physics, InitialTransform, SphereGeo, *outSphere.m_pPhysicsMaterial, outSphere.GetDensity() );

		DynamicColliderInitParams InitParams = {};
		InitParams.StartPos = StartPos;
		InitDynamicBody( InitParams, outSphere );
	}
	HE_ASSERT( outSphere.m_pRigidActor != nullptr ); // Failed to create physics actor!

	m_pScene->addActor( *outSphere.m_pRigidActor );
	outSphere.OnCreate();
}

void PhysicsScene::CreateCubeInternal( const FVector3& StartPos, CubeRigidBody& outCube )
{
	HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

	physx::PxPhysics& Physics = m_pOwningContextRef->GetPhysics();

	outCube.SetOwningScene( *m_pScene );
	physx::PxBoxGeometry CubeGeo( outCube.GetWidth(), outCube.GetHeight(), outCube.GetDepth() );
	physx::PxTransform InitialTransform = physx::PxTransform( physx::PxVec3( StartPos.x, StartPos.y, StartPos.z ) );
	outCube.m_pPhysicsMaterial = Physics.createMaterial( 0.5f, 0.5f, 0.6f );
	if (outCube.GetIsStatic())
	{
		outCube.m_pRigidActor = physx::PxCreateStatic( Physics, InitialTransform, CubeGeo, *outCube.m_pPhysicsMaterial );
	}
	else
	{
		outCube.m_pRigidActor = physx::PxCreateDynamic( Physics, InitialTransform, CubeGeo, *outCube.m_pPhysicsMaterial, outCube.GetDensity() );

		DynamicColliderInitParams InitParams = {};
		InitParams.StartPos = StartPos;
		InitDynamicBody( InitParams, outCube );
	}
	HE_ASSERT( outCube.m_pRigidActor != nullptr ); // Failed to create physics actor!

	m_pScene->addActor( *outCube.m_pRigidActor );
	outCube.OnCreate();
}

void PhysicsScene::RemoveActorInternal( RigidBody& Collider )
{
	if (Collider.m_pRigidActor != nullptr)
	{
		m_pScene->removeActor( *Collider.m_pRigidActor );
	}
	else
	{
		P_LOG( Warning, TEXT( "Trying to remove a rigid body from a scene that has not been initialized!" ) );
		HE_ASSERT( false );
	}
}

void PhysicsScene::InitDynamicBody( const DynamicColliderInitParams& InitParams, RigidBody& RigidBody )
{
	physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( RigidBody.m_pRigidActor );
	HE_ASSERT( pDynamic != nullptr );

	physx::PxTransform Transform( physx::PxVec3( InitParams.StartPos.x, InitParams.StartPos.y, InitParams.StartPos.z ) );
	pDynamic->setGlobalPose( Transform );
	//pDynamic->setAngularDamping( 0.5f );
	physx::PxRigidBodyExt::updateMassAndInertia( *pDynamic, RigidBody.GetDensity() );
	pDynamic->setLinearVelocity( physx::PxVec3( 0, 50, 10 ) );
}

void PhysicsScene::Tick()
{
	if (m_IsSimulationPaused.IsSet())
		return;

	m_IsSimulating.Set();
	TickInternal();
	//m_UpdateLoop.Do( m_SimulationStepRate, this, &PhysicsScene::TickInternal );
	m_IsSimulating.Clear();
}

void PhysicsScene::TickInternal()
{
	if (IsValid())
	{
		m_pScene->simulate( m_SimulationStepRate );
		m_pScene->fetchResults( true );
	}
	else
	{
		P_LOG( Warning, TEXT( "Trying to tick a physics scene that has not been initialized!" ) );
		HE_ASSERT( false );
	}
}

void PhysicsScene::WaitForSimulationFinished() const
{
	if (!m_IsSimulating.IsSet())
	{
		std::this_thread::yield();
	}
}

bool PhysicsScene::IsSimulationFinished() const
{
	return m_pScene->checkResults();
}
