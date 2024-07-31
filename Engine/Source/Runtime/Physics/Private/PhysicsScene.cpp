#include "PhysicsPCH.h"

#include "PhysicsScene.h"

#include "PhysicsContext.h"
#include "RigidBody.h"
#include "CollisionHandler.h"

#include "PxSimulationEventCallback.h"

using namespace physx;

// SimulationEventCallback
//

class FSimulationEventCallback : public PxSimulationEventCallback
{
public:
	virtual void onConstraintBreak( PxConstraintInfo* constraints, PxU32 count ) {}
	virtual void onWake( PxActor** actors, PxU32 count ) {}
	virtual void onSleep( PxActor** actors, PxU32 count ) {}
	virtual void onContact( const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs );
	virtual void onTrigger( PxTriggerPair* pairs, PxU32 count );
	virtual void onAdvance( const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count ) {}

};

void FSimulationEventCallback::onContact( const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs )
{
	for (PxU32 i = 0; i < nbPairs; i++)
	{
		const PxContactPair& cp = pairs[i];

		PhysicsCallbackHandler* Col1 = SCast<PhysicsCallbackHandler*>( pairHeader.actors[0]->userData );
		PhysicsCallbackHandler* Col2 = SCast<PhysicsCallbackHandler*>( pairHeader.actors[1]->userData );
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			Col1->CollisionEvent( PhysicsCallbackHandler::CT_Enter, Col2 );
			Col2->CollisionEvent( PhysicsCallbackHandler::CT_Enter, Col1 );
		}
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			Col1->CollisionEvent( PhysicsCallbackHandler::CT_Stay, Col2 );
			Col2->CollisionEvent( PhysicsCallbackHandler::CT_Stay, Col1 );
		}
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			Col1->CollisionEvent( PhysicsCallbackHandler::CT_Exit, Col2 );
			Col2->CollisionEvent( PhysicsCallbackHandler::CT_Exit, Col1 );
		}
	}
}

void FSimulationEventCallback::onTrigger( PxTriggerPair* pairs, PxU32 count )
{
	while (count--)
	{
		const PxTriggerPair& cp = *pairs++;

		PhysicsCallbackHandler* Col1 = SCast<PhysicsCallbackHandler*>( cp.triggerActor->userData );
		PhysicsCallbackHandler* Col2 = SCast<PhysicsCallbackHandler*>( cp.otherActor->userData );
		if (cp.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			Col1->CollisionEvent( PhysicsCallbackHandler::CT_Enter, Col2 );
			Col2->CollisionEvent( PhysicsCallbackHandler::CT_Enter, Col1 );
		}
		if (cp.status & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			Col1->CollisionEvent( PhysicsCallbackHandler::CT_Stay, Col2 );
			Col2->CollisionEvent( PhysicsCallbackHandler::CT_Stay, Col1 );
		}
		if (cp.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			Col1->CollisionEvent( PhysicsCallbackHandler::CT_Exit, Col2 );
			Col2->CollisionEvent( PhysicsCallbackHandler::CT_Exit, Col1 );
		}
	}
}

static FSimulationEventCallback GSimulationEventCallback;

static PxFilterFlags DefaultFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize )
{
	// let triggers through
	if (PxFilterObjectIsTrigger( attributes0 ) || PxFilterObjectIsTrigger( attributes1 ))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	// trigger the contact callback for pairs (A,B) where 
	// the filtermask of A contains the ID of B and vice versa.
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return PxFilterFlag::eDEFAULT;
}

void SetupFiltering( PxRigidActor* Actor, PxU32 FilterGroup, PxU32 FilterMask )
{
	PxFilterData FilterData;
	FilterData.word0 = FilterGroup; // word0 = own ID
	FilterData.word1 = FilterMask;	// word1 = ID mask to filter pairs that trigger a contact callback;
	const PxU32 NumShapes = Actor->getNbShapes();
	PxShape** Shapes = (PxShape**)HE_HeapAlloc( sizeof( PxShape* ) * NumShapes );
	Actor->getShapes( Shapes, NumShapes );
	for (PxU32 i = 0; i < NumShapes; i++)
	{
		PxShape* Shape = Shapes[i];
		Shape->setSimulationFilterData( FilterData );
		Shape->setQueryFilterData( FilterData );
	}
	HE_HeapFree( Shapes );
}


// PhysicsScene
//

HPhysicsScene::HPhysicsScene()
	: m_pOwningContextRef( nullptr )
	, m_pScene( nullptr )
	, m_SimulationStepRate( 1.f / 60.f )
	, m_StepAccumulator( 0.f )
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
	PxPhysics& Physics = m_pOwningContextRef->GetPhysics();

	PxSceneDesc SceneDesc( Physics.getTolerancesScale() );
	SceneDesc.gravity = PxVec3( 0.f, -9.81f, 0.f );
	SceneDesc.cpuDispatcher = &PhysicsContext.GetCpuDispatcher();
	SceneDesc.filterShader = PxDefaultSimulationFilterShader;//DefaultFilterShader;
	SceneDesc.simulationEventCallback = &GSimulationEventCallback;
	m_pScene = Physics.createScene( SceneDesc );
	HE_ASSERT( m_pScene != nullptr );

	PxPvdSceneClient* pPvd = m_pScene->getScenePvdClient();
	if (pPvd != nullptr)
	{
		pPvd->setScenePvdFlag( PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true );
		pPvd->setScenePvdFlag( PxPvdSceneFlag::eTRANSMIT_CONTACTS, true );
		pPvd->setScenePvdFlag( PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true );
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
			//CreateSphere( Desc.StartPosition, Desc.StartRotation, Desc.outRB, Desc.IsTrigger );
			if (Desc.StartDisabled)
				Desc.outRB.DisableSimulation();

			Desc.outRB.m_pRigidActor->userData = Desc.pCallbackHandler;
			break;
		}
		case PE_AddPlaneActor:
		{
			HE_ASSERT( Event.pUserData != nullptr ); // Plane init info was not filled out correctly!
			RigidActorAddDesc<HPlaneRigidBody>& Desc = *(RigidActorAddDesc<HPlaneRigidBody>*)Event.pUserData;
			//CreatePlane( Desc.StartPosition, Desc.StartRotation, Desc.outRB, Desc.IsTrigger );
			if (Desc.StartDisabled)
				Desc.outRB.DisableSimulation();

			Desc.outRB.m_pRigidActor->userData = Desc.pCallbackHandler;
			break;
		}
		case PE_AddCubeActor:
		{
			HE_ASSERT( Event.pUserData != nullptr ); // Cube init info was not filled out correctly!
			RigidActorAddDesc<HCubeRigidBody>& Desc = *(RigidActorAddDesc<HCubeRigidBody>*)Event.pUserData;
			//CreateCube( Desc.StartPosition, Desc.StartRotation, Desc.outRB, Desc.IsTrigger );
			if (Desc.StartDisabled)
				Desc.outRB.DisableSimulation();

			Desc.outRB.m_pRigidActor->userData = Desc.pCallbackHandler;
			break;
		}
		case PE_AddCapsuleActor:
		{
			HE_ASSERT( Event.pUserData != nullptr ); // Capsule init info was not filled out correctly!
			RigidActorAddDesc<HCapsuleRigidBody>& Desc = *(RigidActorAddDesc<HCapsuleRigidBody>*)Event.pUserData;
			//CreateCapsule( Desc.StartPosition, Desc.StartRotation, Desc.outRB, Desc.IsTrigger );
			if (Desc.StartDisabled)
				Desc.outRB.DisableSimulation();

			Desc.outRB.m_pRigidActor->userData = Desc.pCallbackHandler;
			break;
		}
		case PE_RemoveActor:
		{
			HRigidBody& RB = *(HRigidBody*)Event.pUserData;
			RemoveActor( RB );
			break;
		}
		case PE_FlushScene:
			FlushInternal();
			break;
		case PE_TickScene:
			Tick(1.f / 60.f, 1.f);
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

	PxPhysics& Physics = m_pOwningContextRef->GetPhysics();

	outPlane.m_pPhysicsMaterial = Physics.createMaterial( 0.5f, 0.5f, 0.6f );

	FVector3 Normal = outPlane.GetDirection();
	outPlane.m_pPhysicsMaterial = Physics.createMaterial( 0.5f, 0.5f, 0.6f );
	outPlane.m_pRigidActor = PxCreatePlane( Physics, PxPlane( Normal.x, Normal.y, Normal.z, outPlane.GetDistance()), *outPlane.m_pPhysicsMaterial );
	m_pScene->addActor( *outPlane.m_pRigidActor );
}

void HPhysicsScene::CreateSphere( const FVector3& StartPos, const FQuat& StartRotation, HSphereRigidBody& outSphere, bool IsTrigger, void* pUserData, bool IsKinematic, float Density, bool IsStatic, EFilterGroup CollisionGroup, EFilterGroup FilterGroup/* = FG_World*/ )
{
	HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

	PxSphereGeometry SphereGeo( outSphere.GetRadius() );
	InitRigidBody( outSphere, SphereGeo, StartPos, StartRotation, IsTrigger, pUserData, IsKinematic, Density, IsStatic, CollisionGroup, FilterGroup );
}

void HPhysicsScene::CreatePlane( const FVector3& StartPos, const FQuat& StartRotation, HPlaneRigidBody& outPlane, bool IsTrigger, void* pUserData, bool IsKinematic, float Density, bool IsStatic, EFilterGroup CollisionGroup, EFilterGroup FilterGroup/* = FG_World*/ )
{
	HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

	PxBoxGeometry PlaneGeo( outPlane.GetHalfWidth(), HPlaneRigidBody::kPlaneDepth, outPlane.GetHalfHeight() );
	InitRigidBody( outPlane, PlaneGeo, StartPos, StartRotation, IsTrigger, pUserData, IsKinematic, Density, IsStatic, CollisionGroup, FilterGroup );
}

void HPhysicsScene::CreateCube( const FVector3& StartPos, const FQuat& StartRotation, HCubeRigidBody& outCube, bool IsTrigger, void* pUserData, bool IsKinematic, float Density, bool IsStatic, EFilterGroup CollisionGroup, EFilterGroup FilterGroup/* = FG_World*/ )
{
	using namespace physx;
	HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

	PxBoxGeometry CubeGeo( outCube.GetHalfWidth(), outCube.GetHalfHeight(), outCube.GetHalfDepth() );
	InitRigidBody( outCube, CubeGeo, StartPos, StartRotation, IsTrigger, pUserData, IsKinematic, Density, IsStatic, CollisionGroup, FilterGroup );

}

void HPhysicsScene::CreateCapsule( const FVector3& StartPos, const FQuat& StartRotation, HCapsuleRigidBody& outCapsule, bool IsTrigger, void* pUserData, bool IsKinematic, float Density, bool IsStatic, EFilterGroup CollisionGroup, EFilterGroup FilterGroup/* = FG_World*/ )
{
	HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!
	
	FQuat DefaultRotation = FQuat::CreateFromAxisAngle( FVector3::Forward, PxHalfPi );

	PxCapsuleGeometry CapsuleGeo( outCapsule.GetRadius(), outCapsule.GetHalfHeight() );
	InitRigidBody( outCapsule, CapsuleGeo, StartPos, DefaultRotation, IsTrigger, pUserData, IsKinematic, Density, IsStatic, CollisionGroup, FilterGroup );
}

void HPhysicsScene::InitRigidBody( HRigidBody& outRB, const PxGeometry& Geo, const FVector3& StartPos, const FQuat& StartRotation, bool IsTrigger, void* pUserData, bool IsKinematic, float Density, bool IsStatic, EFilterGroup CollisionGroup, EFilterGroup FilterGroup/* = FG_World*/ )
{
	using namespace physx;
	HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

	PxPhysics& Physics = m_pOwningContextRef->GetPhysics();

	outRB.Reset();

	// TODO: This should be stored per-rigid body.
	outRB.m_pPhysicsMaterial = Physics.createMaterial( 0.5f, 0.5f, 0.6f );

	PxTransform InitialTransform = PxTransform( PxVec3( StartPos.x, StartPos.y, StartPos.z ), PxQuat( StartRotation.x, StartRotation.y, StartRotation.z, StartRotation.w ) );
	PxShapeFlags Flags;
#if HE_DEBUG_PHYSICS
	Flags.set(PxShapeFlag::eVISUALIZATION);
#endif

	if (IsTrigger)
	{
		Flags.set( PxShapeFlag::eTRIGGER_SHAPE );

		PxShape* pShape = Physics.createShape( Geo, *Physics.createMaterial( 1.f, 1.f, 0.f ), false, Flags );
		HE_ASSERT( pShape != nullptr ); 

		outRB.m_pRigidActor = Physics.createRigidStatic( InitialTransform );
		HE_ASSERT( outRB.m_pRigidActor != nullptr );
		outRB.m_pRigidActor->attachShape( *pShape );
		pShape->release();
	}
	else
	{
		// TODO: Only static should be evaluated if we are cooking physics geo
		if (IsStatic)
		{
			outRB.m_pRigidActor = PxCreateStatic( Physics, InitialTransform, Geo, *outRB.m_pPhysicsMaterial );
			HE_ASSERT( outRB.m_pRigidActor != nullptr );
		}
		else
		{
			HE_ASSERT( Density > 0.f ); // A dynamic rigid body's density should never be Zero!

			outRB.m_pRigidActor = PxCreateDynamic( Physics, InitialTransform, Geo, *outRB.m_pPhysicsMaterial, Density );
			HE_ASSERT( outRB.m_pRigidActor != nullptr );
			PxRigidDynamic* pDynamic = SCast<PxRigidDynamic*>( outRB.m_pRigidActor );

			outRB.SetGlobalPositionOrientation( StartPos, StartRotation );
			outRB.SetIsKinematic( IsKinematic );

			PxRigidBodyExt::updateMassAndInertia( *pDynamic, Density );
		}
	}

	m_pScene->addActor( *outRB.m_pRigidActor );
	SetupFiltering( outRB.m_pRigidActor, CollisionGroup, FilterGroup );

	outRB.m_pRigidActor->userData = pUserData;
}

class FPhysicsQueryFilter : public PxQueryFilterCallback
{
public:
	std::vector<PxRigidActor*> IgnoreActors;
	std::vector<const PxRigidActor*> HitActors;

	virtual PxQueryHitType::Enum preFilter( const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags )
	{
		for (PxRigidActor* pActor : IgnoreActors)
		{
			if(pActor == actor)
				return PxQueryHitType::eNONE;
		}

		HitActors.push_back( actor );

		return PxQueryHitType::eBLOCK;
	}

	virtual PxQueryHitType::Enum postFilter( const PxFilterData& filterData, const PxQueryHit& hit )
	{
		return PxQueryHitType::eNONE;

	}
};

bool HPhysicsScene::RayCast( const FVector3& Origin, const FVector3& UnitDirection, const float& Distance, FRaycastHitInfo* outHitInfo, std::vector<HRigidBody*>* IgnoreActors )
{
	PxVec3 Orig( Origin.x, Origin.y, Origin.z );
	PxVec3 UnitDir( UnitDirection.x, UnitDirection.y, UnitDirection.z );
	PxRaycastBuffer HitInfo = {};
	
	FPhysicsQueryFilter QueryFilter;
	if (IgnoreActors)
	{
		QueryFilter.IgnoreActors.resize( IgnoreActors->size() );

		for (size_t i = 0; i < IgnoreActors->size(); i++)
		{
			HRigidBody* pRigidBody = (*IgnoreActors)[i];
			if(pRigidBody)
				QueryFilter.IgnoreActors.push_back( &pRigidBody->GetRigidActor() );
		}
	}

	const PxHitFlags HitFlags = PxHitFlag::eDEFAULT;
	const PxQueryFlags QueryFlags( PxQueryFlags( PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER ) );
	const PxQueryFilterData filterData( PxFilterData(), QueryFlags );

	bool Hit = m_pScene->raycast( Orig, UnitDir, Distance, HitInfo, HitFlags, filterData, &QueryFilter);

	if (outHitInfo != nullptr)
	{
		outHitInfo->HitPos = FVector3( HitInfo.block.position.x, HitInfo.block.position.y, HitInfo.block.position.z );
		outHitInfo->HitNormal = FVector3( HitInfo.block.normal.x, HitInfo.block.normal.y, HitInfo.block.normal.z );
		outHitInfo->Distance = HitInfo.block.distance;
		outHitInfo->AnyHit = Hit;
	}

	return Hit;
}

void HPhysicsScene::RemoveActor( HRigidBody& Collider )
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

void HPhysicsScene::Tick( float DeltaTime, float StepRateScale )
{
	m_StepAccumulator += DeltaTime;
	if (m_StepAccumulator < m_SimulationStepRate)
		return;

	m_StepAccumulator -= m_SimulationStepRate;

	if (m_IsSimulationPaused.IsSet())
		return;

	m_IsSimulating.Set();
	
	if (IsValid())
	{
		m_pScene->simulate( m_SimulationStepRate * StepRateScale );
		m_pScene->fetchResults( true );
	}
	else
	{
		P_LOG( Warning, TEXT( "Trying to tick a physics scene that has not been initialized!" ) );
		HE_ASSERT( false );
	}

	m_IsSimulating.Clear();
}

void HPhysicsScene::WaittillSimulationFinished() const
{
	while (m_EventQueue.GetNumEvents())
		std::this_thread::yield();

	while(m_IsSimulating.IsSet())
		std::this_thread::yield();
}

bool HPhysicsScene::IsSimulationFinished() const
{
	return m_pScene->checkResults();
}


void HPhysicsScene::RequestPauseSimulation()
{
	m_IsSimulationPaused.Set();

	//PhysicsEventPacket Packet;
	//Packet.EventType = PE_PauseSimulation;
	//m_EventQueue.PushEvent( Packet );
}

void HPhysicsScene::RequestUnPauseSimulation()
{
	m_IsSimulationPaused.Clear();

	//PhysicsEventPacket Packet;
	//Packet.EventType = PE_UnPauseSimulation;
	//m_EventQueue.PushEvent( Packet );
}

void HPhysicsScene::RequestSetStepRate( float NewStepRate )
{
	m_SimulationStepRate = NewStepRate;
	//PhysicsEventPacket Packet;
	//Packet.EventType = PE_SetSimulationStepRate;
	//Packet.pUserData = new float( NewStepRate );
	//m_EventQueue.PushEvent( Packet );
}

void HPhysicsScene::RequestActorRemove( HRigidBody& RigidBody )
{
	RemoveActor( RigidBody );

	//PhysicsEventPacket Packet;
	//Packet.EventType = PE_RemoveActor;
	//Packet.pUserData = &RigidBody;
	//m_EventQueue.PushEvent( Packet );
}

void HPhysicsScene::RequestSceneFlush()
{
	FlushInternal();

	//PhysicsEventPacket Packet;
	//Packet.EventType = PE_FlushScene;
	//m_EventQueue.PushEvent( Packet );
}

void HPhysicsScene::RequestTick()
{
	HE_ASSERT( false );
	//PhysicsEventPacket Packet;
	//Packet.EventType = PE_TickScene;
	//m_EventQueue.PushEvent( Packet );
}

bool HPhysicsScene::IsSimulationPaused() const
{
	return m_IsSimulationPaused.IsSet();
}

void HPhysicsScene::WaittillSimulationPaused() const
{
	while (!m_IsSimulationPaused.IsSet())
		std::this_thread::yield();
}