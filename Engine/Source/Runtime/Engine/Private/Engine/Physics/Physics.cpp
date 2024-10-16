#include "EnginePCH.h"

#include "Engine/Physics/Physics.h"

#include "Engine/Engine.h"
#include "GameFramework/Components/HCameraComponent.h"
#include "Engine/Physics/PhysicsContext.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/Physics/CollisionHandler.h"


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

namespace Physics
{
	HPhysicsContext PhysicsContext;
	PxScene* PhysicsScene = nullptr;
	PxCooking* Cooking = nullptr;
	PxControllerManager* ControllerManager = nullptr;

	float Gravity = -9.81f;
	FFlag IsSimulating;
	FFlag IsSimulationPause;

	void Initialize()
	{
		HE_ASSERT( !IsValid() ); // Trying to initialize physics when it has already been initalized.
		
		PhysicsContext.Initialize();
		PxPhysics& Physics = PhysicsContext.GetPhysics();

		PxSceneDesc SceneDesc( Physics.getTolerancesScale() );
		SceneDesc.gravity = PxVec3( 0.f, -9.81f, 0.f );
		SceneDesc.cpuDispatcher = &PhysicsContext.GetCpuDispatcher();
		SceneDesc.filterShader = PxDefaultSimulationFilterShader;//DefaultFilterShader;
		SceneDesc.simulationEventCallback = &GSimulationEventCallback;
		PhysicsScene = Physics.createScene( SceneDesc );
		HE_ASSERT( PhysicsScene != nullptr );

		PxPvdSceneClient* pPvd = PhysicsScene->getScenePvdClient();
		if (pPvd != nullptr)
		{
			pPvd->setScenePvdFlag( PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true );
			pPvd->setScenePvdFlag( PxPvdSceneFlag::eTRANSMIT_CONTACTS, true );
			pPvd->setScenePvdFlag( PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true );
		}
		ControllerManager = PxCreateControllerManager( *PhysicsScene );
		
		Cooking = PxCreateCooking( PX_PHYSICS_VERSION, PhysicsContext.GetFoundation(), PxCookingParams( Physics.getTolerancesScale() ) );
		HE_ASSERT( Cooking != nullptr );
	}

	bool IsValid()
	{
		return PhysicsContext.IsReady() && PhysicsScene != nullptr;
	}

	void UnInitialize()
	{
		WaittillSimulationFinished();

		ControllerManager->release();
		PX_SAFE_RELEASE( PhysicsScene );
	}

	void FlushInternal()
	{
		PhysicsScene->flushSimulation();
	}

	physx::PxCapsuleController* CreateCapsuleController( const physx::PxControllerDesc& Desc )
	{
		return static_cast<physx::PxCapsuleController*>( ControllerManager->createController( Desc ) );
	}

	physx::PxMaterial* CreateDefaultMaterial()
	{
		PxPhysics& Physics = PhysicsContext.GetPhysics();
		return Physics.createMaterial( 0.5f, 0.5f, 0.6f );
	}
	
	physx::PxScene* GetScene()
	{
		return PhysicsScene;
	}

	void CreateInfinitePlaneInternal( HInfinitePlaneRigidBody& outPlane, bool IsTrigger )
	{
		HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

		PxPhysics& Physics = PhysicsContext.GetPhysics();

		outPlane.m_pPhysicsMaterial = Physics.createMaterial( 0.5f, 0.5f, 0.6f );

		FVector3 Normal = outPlane.GetDirection();
		outPlane.m_pPhysicsMaterial = Physics.createMaterial( 0.5f, 0.5f, 0.6f );
		outPlane.m_pRigidActor = PxCreatePlane( Physics, PxPlane( Normal.x, Normal.y, Normal.z, outPlane.GetDistance() ), *outPlane.m_pPhysicsMaterial );
		PhysicsScene->addActor( *outPlane.m_pRigidActor );
	}

	void InitRigidBody( HRigidBody& outRB, const PxGeometry& Geo, const FVector3& StartPos, const FQuat& StartRotation, bool IsTrigger, void* pUserData, bool IsKinematic, float Density, bool IsStatic, EFilterGroup CollisionGroup, EFilterGroup FilterGroup/* = FG_World*/ )
	{
		using namespace physx;
		HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

		PxPhysics& Physics = PhysicsContext.GetPhysics();

		outRB.Reset();

		// TODO: This should be stored per-rigid body.
		outRB.m_pPhysicsMaterial = Physics.createMaterial( 0.5f, 0.5f, 0.6f );

		PxTransform InitialTransform = PxTransform( PxVec3( StartPos.x, StartPos.y, StartPos.z ), PxQuat( StartRotation.x, StartRotation.y, StartRotation.z, StartRotation.w ) );
		PxShapeFlags Flags;
#if HE_DEBUG_PHYSICS
		Flags.set( PxShapeFlag::eVISUALIZATION );
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

		PhysicsScene->addActor( *outRB.m_pRigidActor );
		SetupFiltering( outRB.m_pRigidActor, CollisionGroup, FilterGroup );

		outRB.m_pRigidActor->userData = pUserData;
	}

	void CreateMesh( const FVector3& StartPos, const FQuat& StartRotation, const FVector3& Scale, HTriangleMeshRigidBody& outMesh, bool IsTrigger, void* pUserData, bool IsKinematic, float Density, bool IsStatic, EFilterGroup CollisionGroup, EFilterGroup FilterGroup/* = FG_World*/ )
	{
		HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

		PxTriangleMeshDesc MeshDesc = {};
		MeshDesc.points.data = outMesh.pTriangleData;
		MeshDesc.points.count = outMesh.TriCount;
		MeshDesc.points.stride = outMesh.VertexSize;
		MeshDesc.triangles.data = outMesh.pIndexData;
		MeshDesc.triangles.count = outMesh.IndexCount / 3;
		MeshDesc.triangles.stride = 3 * outMesh.IndexSize;
		PxDefaultMemoryOutputStream WriteBuffer;
		PxTriangleMeshCookingResult::Enum Result;
		bool Success = Cooking->cookTriangleMesh( MeshDesc, WriteBuffer, &Result );
		HE_ASSERT( Success );

		PxDefaultMemoryInputData ReadBuffer( WriteBuffer.getData(), WriteBuffer.getSize() );
		PxTriangleMesh* TriangelMesh = Cooking->createTriangleMesh( MeshDesc, PhysicsContext.GetPhysics().getPhysicsInsertionCallback() );
		HE_ASSERT( TriangelMesh != nullptr );

		PxMeshScale MeshScale( PxVec3( Scale.x, Scale.y, Scale.z ) );
		PxTriangleMeshGeometry TriangleMeshGeo( TriangelMesh, MeshScale );
		InitRigidBody( outMesh, TriangleMeshGeo, StartPos, StartRotation, IsTrigger, pUserData, IsKinematic, Density, IsStatic, CollisionGroup, FilterGroup );
	}

	void CreateSphere( const FVector3& StartPos, const FQuat& StartRotation, HSphereRigidBody& outSphere, bool IsTrigger, void* pUserData, bool IsKinematic, float Density, bool IsStatic, EFilterGroup CollisionGroup, EFilterGroup FilterGroup/* = FG_World*/ )
	{
		HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

		PxSphereGeometry SphereGeo( outSphere.GetRadius() );
		InitRigidBody( outSphere, SphereGeo, StartPos, StartRotation, IsTrigger, pUserData, IsKinematic, Density, IsStatic, CollisionGroup, FilterGroup );
	}

	void CreatePlane( const FVector3& StartPos, const FQuat& StartRotation, HPlaneRigidBody& outPlane, bool IsTrigger, void* pUserData, bool IsKinematic, float Density, bool IsStatic, EFilterGroup CollisionGroup, EFilterGroup FilterGroup/* = FG_World*/ )
	{
		HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

		PxBoxGeometry PlaneGeo( outPlane.GetHalfWidth(), HPlaneRigidBody::kPlaneDepth, outPlane.GetHalfHeight() );
		InitRigidBody( outPlane, PlaneGeo, StartPos, StartRotation, IsTrigger, pUserData, IsKinematic, Density, IsStatic, CollisionGroup, FilterGroup );
	}

	void CreateCube( const FVector3& StartPos, const FQuat& StartRotation, HCubeRigidBody& outCube, bool IsTrigger, void* pUserData, bool IsKinematic, float Density, bool IsStatic, EFilterGroup CollisionGroup, EFilterGroup FilterGroup/* = FG_World*/ )
	{
		HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

		PxBoxGeometry CubeGeo( outCube.GetHalfWidth(), outCube.GetHalfHeight(), outCube.GetHalfDepth() );
		InitRigidBody( outCube, CubeGeo, StartPos, StartRotation, IsTrigger, pUserData, IsKinematic, Density, IsStatic, CollisionGroup, FilterGroup );
	}

	void CreateCapsule( const FVector3& StartPos, const FQuat& StartRotation, HCapsuleRigidBody& outCapsule, bool IsTrigger, void* pUserData, bool IsKinematic, float Density, bool IsStatic, EFilterGroup CollisionGroup, EFilterGroup FilterGroup/* = FG_World*/ )
	{
		HE_ASSERT( IsValid() ); // Trying to add collision actors to scene that has not been initialized yet!

		FQuat DefaultRotation = FQuat::CreateFromAxisAngle( FVector3::Forward, PxHalfPi );

		PxCapsuleGeometry CapsuleGeo( outCapsule.GetRadius(), outCapsule.GetHalfHeight() );
		InitRigidBody( outCapsule, CapsuleGeo, StartPos, DefaultRotation, IsTrigger, pUserData, IsKinematic, Density, IsStatic, CollisionGroup, FilterGroup );
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
				if (pActor == actor)
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

	bool RayCast( const FVector3& Origin, const FVector3& UnitDirection, const float& Distance, FRaycastHitInfo* outHitInfo /*= nullptr*/, std::vector<HRigidBody*>* IgnoreActors /*= nullptr*/ )
	{
		PxVec3 Orig( Origin.x, Origin.y, Origin.z );
		PxVec3 UnitDir( UnitDirection.x, UnitDirection.y, UnitDirection.z );
		FVector3 TargetHitPos = Origin + UnitDirection * Distance;
		PxRaycastBuffer HitInfo = {};

		FPhysicsQueryFilter QueryFilter;

		if (IgnoreActors)
		{
			QueryFilter.IgnoreActors.resize( IgnoreActors->size() );

			for (size_t i = 0; i < IgnoreActors->size(); i++)
			{
				HRigidBody* pRigidBody = (*IgnoreActors)[i];
				if (pRigidBody)
					QueryFilter.IgnoreActors.push_back( &pRigidBody->GetRigidActor() );
			}
		}

		const PxHitFlags HitFlags = PxHitFlag::eDEFAULT;
		//const PxQueryFlags QueryFlags( PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC | PxQueryFlag::ePREFILTER );
		const PxQueryFlags QueryFlags( PxQueryFlag::eANY_HIT | PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC );
		const PxQueryFilterData FilterData( PxFilterData(), QueryFlags );

		bool Hit = PhysicsScene->raycast( Orig, UnitDir, Distance, HitInfo, HitFlags, FilterData, &QueryFilter );
		
		if (outHitInfo != nullptr)
		{
			outHitInfo->HitPos = FVector3( HitInfo.block.position.x, HitInfo.block.position.y, HitInfo.block.position.z );
			outHitInfo->HitNormal = FVector3( HitInfo.block.normal.x, HitInfo.block.normal.y, HitInfo.block.normal.z );
			outHitInfo->Distance = HitInfo.block.distance;
			outHitInfo->AnyHit = Hit;
			outHitInfo->Fraction = ((outHitInfo->HitPos - Origin).Length() + (TargetHitPos - outHitInfo->HitPos).Length()) / (outHitInfo->HitPos - Origin).Length();
		}
		
		return Hit;
	}

	bool SphereCast( const FVector3& Start, const float& SphereSize )
	{
		PxSphereGeometry SphereGeo( SphereSize );
		PxTransform Transform( Start.x, Start.y, Start.z );
		PxOverlapBuffer HitInfo = {};

		FPhysicsQueryFilter QueryFilter;
		const PxQueryFlags QueryFlags( PxQueryFlag::eANY_HIT | PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC );
		const PxQueryFilterData FilterData( PxFilterData(), QueryFlags );

		return PhysicsScene->overlap( SphereGeo, Transform, HitInfo, FilterData, &QueryFilter );
	}

	FVector3 ScreenToWorldPos( FVector2 ScreenPos )
	{
		float MaxTraceDistance = 10000.f;
		FRaycastHitInfo HitInfo = {};
		const FVector2 WindowDims = GEngine->GetClientViewport().GetDimensions();
		HCameraComponent& Camera = GEngine->GetPlayerCamera();
		FVector3 TraceStart = Camera.GetTransform().GetWorldPosition();
		FVector3 Direction = Math::WorldDirectionFromScreenPos( ScreenPos, WindowDims, Camera.GetViewMatrix(), Camera.GetProjectionMatrix() );

		RayCast( TraceStart, Direction, MaxTraceDistance, &HitInfo, nullptr );

		if (HitInfo.AnyHit)
			return HitInfo.HitPos;
		else
			return TraceStart + Direction * MaxTraceDistance;
	}

	void RemoveActor( HRigidBody& Collider )
	{
		if (Collider.m_pRigidActor != nullptr)
		{
			PhysicsScene->removeActor( *Collider.m_pRigidActor );
		}
		else
		{
			P_LOG( Warning, "Trying to remove a rigid body from a scene that has not been initialized!" );
			HE_ASSERT( false );
		}
	}

	void Update( float DeltaTime, float StepRateScale )
	{
		if (IsSimulationPause.IsSet())
			return;

		IsSimulating.Set();

		if (IsValid())
		{
			PhysicsScene->simulate( DeltaTime * StepRateScale );
			PhysicsScene->fetchResults( true );
		}
		else
		{
			P_LOG( Warning, "Trying to tick a physics scene that has not been initialized!" );
			HE_ASSERT( false );
		}

		IsSimulating.Clear();
	}


	void PauseSimulation()
	{
		IsSimulationPause.Set();
	}

	void ResumeSimulation()
	{
		IsSimulationPause.Clear();
	}

	bool IsSimulationPaused()
	{
		return IsSimulationPause.IsSet();
	}

	void WaittillSimulationPaused()
	{
		while (!IsSimulationPause.IsSet())
			std::this_thread::yield();
	}

	void WaittillSimulationFinished()
	{
		while (IsSimulating.IsSet())
			std::this_thread::yield();
	}

	bool IsSimulationFinished()
	{
		return PhysicsScene->checkResults();
	}
}