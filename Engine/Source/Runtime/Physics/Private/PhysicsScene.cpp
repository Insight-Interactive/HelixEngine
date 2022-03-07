#include "PhysicsPCH.h"

#include "PhysicsScene.h"

#include "PhysicsContext.h"
#include "RigidBody.h"


PhysicsScene::PhysicsScene()
	: m_pScene( nullptr )
	, m_SimulationStepRate( 1.f / 200.f )
	, m_IsSimulationPaused( false )
{

}

PhysicsScene::~PhysicsScene()
{

}

void PhysicsScene::Setup( PhysicsContext& PhysicsContext )
{
	HE_ASSERT( PhysicsContext.IsReady() ); // Trying to create a physics scene with PhysicsContext that has not been setup yet!
	HE_ASSERT( m_pScene == nullptr ); // Trying to initialize a scene that has already been initalized.

	physx::PxPhysics& Physics = PhysicsContext.GetPhysics();

	physx::PxSceneDesc SceneDesc( Physics.getTolerancesScale() );
	SceneDesc.gravity = physx::PxVec3( 0.f, -9.8f, 0.f );
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
}

void PhysicsScene::Teardown()
{
	PX_SAFE_RELEASE( m_pScene );
}

void PhysicsScene::Flush()
{
	WaitForSimulationFinished();
	m_pScene->flushSimulation();
}

void PhysicsScene::CreatePlane( PhysicsContext& PhysicsContext, const FVector3& StartPos, PlaneRigidBody& outPlane )
{
	HE_ASSERT( PhysicsContext.IsReady() ); // Trying to initialize phyics scene actors with physics context that has not been initialized!
	HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

	physx::PxPhysics& Physics = PhysicsContext.GetPhysics();

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

	static bool b = false;
	if (!b)
	{
		b = true;
		m_pScene->addActor( *physx::PxCreatePlane( Physics, physx::PxPlane( 0, 1, 0, 0 ), *outPlane.m_pPhysicsMaterial ) );
	}

	m_pScene->addActor( *outPlane.m_pRigidActor );
	outPlane.OnCreate();
}

void PhysicsScene::CreateSphere( PhysicsContext& PhysicsContext, const FVector3& StartPos, SphereRigidBody& outSphere )
{
	HE_ASSERT( PhysicsContext.IsReady() ); // Trying to initialize phyics scene actors with physics context that has not been initialized!
	HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

	physx::PxPhysics& Physics = PhysicsContext.GetPhysics();

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

void PhysicsScene::RemoveActor( RigidBody& Collider )
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
	pDynamic->setAngularDamping( 0.5f );
	//pDynamic->setLinearVelocity( physx::PxVec3( 0, 50, 10 ) );
}

void PhysicsScene::Tick()
{
	if (m_IsSimulationPaused)
		return;

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
	m_pScene->checkResults( true );
}

bool PhysicsScene::IsSimulationFinished() const
{
	return m_pScene->checkResults();
}
