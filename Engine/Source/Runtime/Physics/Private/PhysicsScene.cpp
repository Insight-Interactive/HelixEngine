#include "PhysicsPCH.h"

#include "PhysicsScene.h"

#include "PhysicsContext.h"
#include "RigidBody.h"
#include "CollisionHandler.h"

#include "PxSimulationEventCallback.h"


// SimulationEventCallback
//

class SimulationEventCallback : public physx::PxSimulationEventCallback
{
public:
	virtual void onConstraintBreak( physx::PxConstraintInfo* constraints, physx::PxU32 count ) {}
	virtual void onWake( physx::PxActor** actors, physx::PxU32 count ) {}
	virtual void onSleep( physx::PxActor** actors, physx::PxU32 count ) {}
	virtual void onContact( const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs );
	virtual void onTrigger( physx::PxTriggerPair* pairs, physx::PxU32 count );
	virtual void onAdvance( const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count ) {}

};

void SimulationEventCallback::onContact( const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs )
{
	for (physx::PxU32 i = 0; i < nbPairs; i++)
	{
		const physx::PxContactPair& cp = pairs[i];

		PhysicsCallbackHandler* Col1 = SCast<PhysicsCallbackHandler*>( pairHeader.actors[0]->userData );
		PhysicsCallbackHandler* Col2 = SCast<PhysicsCallbackHandler*>( pairHeader.actors[1]->userData );
		if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			Col1->CollisionEvent( PhysicsCallbackHandler::CT_Enter, Col2 );
			Col2->CollisionEvent( PhysicsCallbackHandler::CT_Enter, Col1 );
		}
		else if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			Col1->CollisionEvent( PhysicsCallbackHandler::CT_Stay, Col2 );
			Col2->CollisionEvent( PhysicsCallbackHandler::CT_Stay, Col1 );
		}
		else if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			Col1->CollisionEvent( PhysicsCallbackHandler::CT_Exit, Col2 );
			Col2->CollisionEvent( PhysicsCallbackHandler::CT_Exit, Col1 );
		}
	}
}

void SimulationEventCallback::onTrigger( physx::PxTriggerPair* pairs, physx::PxU32 count )
{
	while (count--)
	{
		const physx::PxTriggerPair& cp = *pairs++;

		PhysicsCallbackHandler* Col1 = SCast<PhysicsCallbackHandler*>( cp.triggerActor->userData );
		PhysicsCallbackHandler* Col2 = SCast<PhysicsCallbackHandler*>( cp.otherActor->userData );
		if (cp.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			Col1->CollisionEvent( PhysicsCallbackHandler::CT_Enter, Col2 );
			Col2->CollisionEvent( PhysicsCallbackHandler::CT_Enter, Col1 );
		}
		else if (cp.status & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			Col1->CollisionEvent( PhysicsCallbackHandler::CT_Stay, Col2 );
			Col2->CollisionEvent( PhysicsCallbackHandler::CT_Stay, Col1 );
		}
		else if (cp.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			Col1->CollisionEvent( PhysicsCallbackHandler::CT_Exit, Col2 );
			Col2->CollisionEvent( PhysicsCallbackHandler::CT_Exit, Col1 );
		}
	}
}

static SimulationEventCallback GSimulationEventCallback;

static physx::PxFilterFlags DefaultFilterShader(
	physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize )
{
	// let triggers through
	if (physx::PxFilterObjectIsTrigger( attributes0 ) || physx::PxFilterObjectIsTrigger( attributes1 ))
	{
		pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
		return physx::PxFilterFlag::eDEFAULT;
	}
	// generate contacts for all that were not filtered above
	pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

	// trigger the contact callback for pairs (A,B) where 
	// the filtermask of A contains the ID of B and vice versa.
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return physx::PxFilterFlag::eDEFAULT;
}


// PhysicsScene
//

HPhysicsScene::HPhysicsScene()
	: m_pOwningContextRef( nullptr )
	, m_pScene( nullptr )
	, m_SimulationStepRate( 1.f / 60.f )
{

}

HPhysicsScene::~HPhysicsScene()
{
	Teardown();
	m_pOwningContextRef = nullptr;
}

void HPhysicsScene::Setup( HPhysicsContext& PhysicsContext )
{
	HE_ASSERT( PhysicsContext.IsReady() ); // Trying to create a physics scene with PhysicsContext that has not been setup yet!
	HE_ASSERT( m_pScene == nullptr ); // Trying to initialize a scene that has already been initalized.

	m_pOwningContextRef = &PhysicsContext;
	physx::PxPhysics& Physics = m_pOwningContextRef->GetPhysics();

	physx::PxSceneDesc SceneDesc( Physics.getTolerancesScale() );
	SceneDesc.gravity = physx::PxVec3( 0.f, -9.81f, 0.f );
	SceneDesc.cpuDispatcher = &PhysicsContext.GetCpuDispatcher();
	SceneDesc.filterShader = DefaultFilterShader;
	SceneDesc.simulationEventCallback = &GSimulationEventCallback;
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

bool HPhysicsScene::IsValid() const
{
	return
		m_pOwningContextRef != nullptr &&
		m_pScene != nullptr &&
		m_pOwningContextRef->IsReady();
}

void HPhysicsScene::ProcessEventQueue()
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
			RigidActorAddDesc<HSphereRigidBody>& Desc = *(RigidActorAddDesc<HSphereRigidBody>*)Event.pUserData;
			CreateSphereInternal( Desc.StartPosition, Desc.outRB, Desc.IsTrigger );
			if (Desc.StartDisabled)
				Desc.outRB.DisableSimulation();

			Desc.outRB.m_pRigidActor->userData = Desc.pCallbackHandler;
			break;
		}
		case PE_AddPlaneActor:
		{
			HE_ASSERT( Event.pUserData != nullptr ); // Plane init info was not filled out correctly!
			RigidActorAddDesc<HPlaneRigidBody>& Desc = *(RigidActorAddDesc<HPlaneRigidBody>*)Event.pUserData;
			CreatePlaneInternal( Desc.StartPosition, Desc.outRB, Desc.IsTrigger );
			if (Desc.StartDisabled)
				Desc.outRB.DisableSimulation();

			Desc.outRB.m_pRigidActor->userData = Desc.pCallbackHandler;
			break;
		}
		case PE_AddCubeActor:
		{
			HE_ASSERT( Event.pUserData != nullptr ); // Cube init info was not filled out correctly!
			RigidActorAddDesc<HCubeRigidBody>& Desc = *(RigidActorAddDesc<HCubeRigidBody>*)Event.pUserData;
			CreateCubeInternal( Desc.StartPosition, Desc.outRB, Desc.IsTrigger );
			if (Desc.StartDisabled)
				Desc.outRB.DisableSimulation();

			Desc.outRB.m_pRigidActor->userData = Desc.pCallbackHandler;
			break;
		}
		case PE_AddCapsuleActor:
		{
			HE_ASSERT( Event.pUserData != nullptr ); // Capsule init info was not filled out correctly!
			RigidActorAddDesc<HCapsuleRigidBody>& Desc = *(RigidActorAddDesc<HCapsuleRigidBody>*)Event.pUserData;
			CreateCapsuleInternal( Desc.StartPosition, Desc.outRB, Desc.IsTrigger );
			if (Desc.StartDisabled)
				Desc.outRB.DisableSimulation();

			Desc.outRB.m_pRigidActor->userData = Desc.pCallbackHandler;
			break;
		}
		case PE_RemoveActor:
		{
			HRigidBody& RB = *(HRigidBody*)Event.pUserData;
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

		if (Event.pUserData)
			HE_HeapFree( Event.pUserData );
	}
}

void HPhysicsScene::Teardown()
{
	WaittillSimulationFinished();

	PX_SAFE_RELEASE( m_pScene );
}

void HPhysicsScene::FlushInternal()
{
	m_pScene->flushSimulation();
}

void HPhysicsScene::CreateInfinitePlaneInternal( HInfinitePlaneRigidBody& outPlane, bool IsTrigger )
{
	HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

	physx::PxPhysics& Physics = m_pOwningContextRef->GetPhysics();

	outPlane.m_pPhysicsMaterial = Physics.createMaterial( 0.5f, 0.5f, 0.6f );

	FVector3 Normal = outPlane.GetDirection();
	outPlane.m_pPhysicsMaterial = Physics.createMaterial( 0.5f, 0.5f, 0.6f );
	outPlane.m_pRigidActor = physx::PxCreatePlane( Physics, physx::PxPlane( Normal.x, Normal.y, Normal.z, outPlane.GetDistance()), *outPlane.m_pPhysicsMaterial );
	m_pScene->addActor( *outPlane.m_pRigidActor );
}

void HPhysicsScene::CreatePlaneInternal( const FVector3& StartPos, HPlaneRigidBody& outPlane, bool IsTrigger )
{
	HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

	physx::PxPhysics& Physics = m_pOwningContextRef->GetPhysics();

	outPlane.m_pPhysicsMaterial = Physics.createMaterial( 0.5f, 0.5f, 0.6f );

	physx::PxBoxGeometry PlaneGeo( outPlane.GetWidth() / 2, HPlaneRigidBody::kPlaneHeight, outPlane.GetHeight() / 2 );
	if (IsTrigger)
	{
		physx::PxShape* pShape = Physics.createShape( PlaneGeo, *Physics.createMaterial( 1.f, 1.f, 0.f ), false, physx::PxShapeFlag::eVISUALIZATION | physx::PxShapeFlag::eTRIGGER_SHAPE );
		HE_ASSERT( pShape != nullptr );

		physx::PxTransform InitialTransform = physx::PxTransform( physx::PxVec3( StartPos.x, StartPos.y, StartPos.z ) );
		outPlane.m_pRigidActor = Physics.createRigidStatic( InitialTransform );
		outPlane.m_pRigidActor->attachShape( *pShape );

		m_pScene->addActor( *outPlane.m_pRigidActor );
		pShape->release();
	}
	else
	{
		InitCollider( outPlane, PlaneGeo, StartPos );
		m_pScene->addActor( *outPlane.m_pRigidActor );
	}
}

void HPhysicsScene::CreateSphereInternal( const FVector3& StartPos, HSphereRigidBody& outSphere, bool IsTrigger )
{
	HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

	physx::PxPhysics& Physics = m_pOwningContextRef->GetPhysics();

	outSphere.m_pPhysicsMaterial = Physics.createMaterial( 0.5f, 0.5f, 0.6f );
	physx::PxSphereGeometry SphereGeo( outSphere.GetRadius() );
	if (IsTrigger)
	{
		physx::PxShape* pShape = Physics.createShape( SphereGeo, *Physics.createMaterial( 1.f, 1.f, 0.f ), false, physx::PxShapeFlag::eVISUALIZATION | physx::PxShapeFlag::eTRIGGER_SHAPE );
		HE_ASSERT( pShape != nullptr );

		physx::PxTransform InitialTransform = physx::PxTransform( physx::PxVec3( StartPos.x, StartPos.y, StartPos.z ) );
		outSphere.m_pRigidActor = Physics.createRigidStatic( InitialTransform );
		outSphere.m_pRigidActor->attachShape( *pShape );

		m_pScene->addActor( *outSphere.m_pRigidActor );
		pShape->release();
	}
	else
	{
		InitCollider( outSphere, SphereGeo, StartPos );
		m_pScene->addActor( *outSphere.m_pRigidActor );
	}
}

void HPhysicsScene::CreateCubeInternal( const FVector3& StartPos, HCubeRigidBody& outCube, bool IsTrigger )
{
	HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

	physx::PxPhysics& Physics = m_pOwningContextRef->GetPhysics();

	outCube.m_pPhysicsMaterial = Physics.createMaterial( 0.5f, 0.5f, 0.6f );

	physx::PxBoxGeometry CubeGeo( outCube.GetWidth() / 2, outCube.GetHeight() / 2, outCube.GetDepth() / 2 );
	if (IsTrigger)
	{
		physx::PxShape* pShape = Physics.createShape( CubeGeo, *Physics.createMaterial( 1.f, 1.f, 0.f ), false, physx::PxShapeFlag::eVISUALIZATION | physx::PxShapeFlag::eTRIGGER_SHAPE );
		HE_ASSERT( pShape != nullptr );

		physx::PxTransform InitialTransform = physx::PxTransform( physx::PxVec3( StartPos.x, StartPos.y, StartPos.z ) );
		outCube.m_pRigidActor = Physics.createRigidStatic( InitialTransform );
		outCube.m_pRigidActor->attachShape( *pShape );

		m_pScene->addActor( *outCube.m_pRigidActor );
		pShape->release();
	}
	else
	{
		InitCollider( outCube, CubeGeo, StartPos );
		m_pScene->addActor( *outCube.m_pRigidActor );
	}
}

void HPhysicsScene::CreateCapsuleInternal( const FVector3& StartPos, HCapsuleRigidBody& outCapsule, bool IsTrigger )
{
	HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

	physx::PxPhysics& Physics = m_pOwningContextRef->GetPhysics();

	outCapsule.m_pPhysicsMaterial = Physics.createMaterial( 0.5f, 0.5f, 0.6f );

	physx::PxCapsuleGeometry CapsuleGeo( outCapsule.GetRadius(), outCapsule.GetLength() / 2 );
	if (IsTrigger)
	{
		physx::PxShape* pShape = Physics.createShape( CapsuleGeo, *Physics.createMaterial( 1.f, 1.f, 0.f ), false, physx::PxShapeFlag::eVISUALIZATION | physx::PxShapeFlag::eTRIGGER_SHAPE );
		HE_ASSERT( pShape != nullptr );

		physx::PxTransform InitialTransform = physx::PxTransform( physx::PxVec3( StartPos.x, StartPos.y, StartPos.z ) );
		outCapsule.m_pRigidActor = Physics.createRigidStatic( InitialTransform );
		outCapsule.m_pRigidActor->attachShape( *pShape );

		m_pScene->addActor( *outCapsule.m_pRigidActor );
		pShape->release();
	}
	else
	{
		InitCollider( outCapsule, CapsuleGeo, StartPos );
		m_pScene->addActor( *outCapsule.m_pRigidActor );
	}
}

void HPhysicsScene::RemoveActorInternal( HRigidBody& Collider )
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

void SetupFiltering( physx::PxRigidActor* actor, physx::PxU32 filterGroup, physx::PxU32 filterMask )
{
	physx::PxFilterData filterData;
	filterData.word0 = filterGroup; // word0 = own ID
	filterData.word1 = filterMask;	// word1 = ID mask to filter pairs that trigger a contact callback;
	const physx::PxU32 numShapes = actor->getNbShapes();
	physx::PxShape** shapes = (physx::PxShape**)HE_HeapAlloc( sizeof( physx::PxShape* ) * numShapes );
	actor->getShapes( shapes, numShapes );
	for (physx::PxU32 i = 0; i < numShapes; i++)
	{
		physx::PxShape* shape = shapes[i];
		shape->setSimulationFilterData( filterData );
	}
	HE_HeapFree( shapes );
}

void HPhysicsScene::InitCollider( HRigidBody& Collider, physx::PxGeometry& Geo, const FVector3& StartPos )
{
	physx::PxPhysics& Physics = m_pOwningContextRef->GetPhysics();

	physx::PxTransform InitialTransform = physx::PxTransform( physx::PxVec3( StartPos.x, StartPos.y, StartPos.z ) );
	if (Collider.GetIsStatic())
	{
		Collider.m_pRigidActor = physx::PxCreateStatic( Physics, InitialTransform, Geo, *Collider.m_pPhysicsMaterial );
	}
	else
	{
		Collider.m_pRigidActor = physx::PxCreateDynamic( Physics, InitialTransform, Geo, *Collider.m_pPhysicsMaterial, Collider.GetDensity() );

		DynamicColliderInitParams InitParams = {};
		InitParams.StartPos = StartPos;
		InitDynamicBody( InitParams, Collider );
	}
	HE_ASSERT( Collider.m_pRigidActor != nullptr ); // Failed to create physics actor!

	SetupFiltering( Collider.m_pRigidActor, 1, 1 );

	FinalizeRigidBody( Collider );
}

void HPhysicsScene::InitDynamicBody( const DynamicColliderInitParams& InitParams, HRigidBody& HRigidBody )
{
	physx::PxRigidDynamic* pDynamic = SCast<physx::PxRigidDynamic*>( HRigidBody.m_pRigidActor );
	HE_ASSERT( pDynamic != nullptr );

	physx::PxTransform Transform( physx::PxVec3( InitParams.StartPos.x, InitParams.StartPos.y, InitParams.StartPos.z ) );
	pDynamic->setGlobalPose( Transform );
	physx::PxRigidBodyExt::updateMassAndInertia( *pDynamic, HRigidBody.GetDensity() );
}

void HPhysicsScene::FinalizeRigidBody( HRigidBody& outRB )
{
	//outRB.m_pRigidActor->setActorFlag( physx::PxActorFlag::eDISABLE_SIMULATION, outRB.GetIsStatic() );
}

void HPhysicsScene::Tick()
{
	if (m_IsSimulationPaused.IsSet())
		return;

	m_IsSimulating.Set();
	TickInternal();
	m_IsSimulating.Clear();
}

void HPhysicsScene::TickInternal()
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

void HPhysicsScene::WaittillSimulationFinished() const
{
	while (m_IsSimulating.IsSet())
		std::this_thread::yield();
}

bool HPhysicsScene::IsSimulationFinished() const
{
	return m_pScene->checkResults();
}
