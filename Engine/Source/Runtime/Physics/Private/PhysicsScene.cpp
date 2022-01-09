#include "PhysicsPCH.h"

#include "PhysicsScene.h"

#include "PhysicsContext.h"
#include "RigidBody.h"


PhysicsScene::PhysicsScene()
	: m_pScene( nullptr )
{

}

PhysicsScene::~PhysicsScene()
{

}

void PhysicsScene::Setup( PhysicsContext& PhysicsContext )
{
	HE_ASSERT( PhysicsContext.IsReady() ); // Trying to create a physics scene with PhysicsContext that has not been setup yet!

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
	PX_SAVE_RELEASE( m_pScene );
}

void PhysicsScene::CreatePlane( PhysicsContext& PhysicsContext, bool IsStatic, RigidBody& outPlane )
{
	HE_ASSERT( PhysicsContext.IsReady() ); // Trying to initialize phyics scene actors with physics context that has not been initialized!

	physx::PxPhysics& Physics = PhysicsContext.GetPhysics();

	outPlane.SetIsStatic( IsStatic );
	outPlane.m_pPhysicsMaterial = Physics.createMaterial( 0.5f, 0.5f, 0.6f );

	if (IsStatic)
		outPlane.m_pRigidActor = physx::PxCreatePlane( Physics, physx::PxPlane( 0, 1, 0, 0 ), *outPlane.m_pPhysicsMaterial );
	else
		outPlane.m_pRigidActor = physx::PxCreateDynamic( Physics, physx::PxTransform( physx::PxVec3( 0, 40, 100 ) ), physx::PxPlaneGeometry(), *outPlane.m_pPhysicsMaterial, 10.f );

}

void PhysicsScene::Tick( float StepRate )
{
	if (m_pScene)
	{
		m_pScene->simulate( StepRate );
	}
	else
	{
		P_LOG( Warning, TEXT( "Trying to tick a physics scene that has not been initialized!" ) );
		HE_ASSERT( false );
	}
}

void PhysicsScene::QuerySimulationResults()
{
	if (m_pScene)
	{
		m_pScene->fetchResults( true );
	}
	else
	{
		P_LOG( Warning, TEXT( "Trying to fetch results from a physics scene that has not been initialized!" ) );
		HE_ASSERT( false );
	}
}
