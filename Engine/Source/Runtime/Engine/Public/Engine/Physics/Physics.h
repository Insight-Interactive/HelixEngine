// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Flag.h"
#include "CriticalSection.h"



namespace physx
{
	class PxScene;
	class PxGeometry;
	class PxMaterial;
	class PxCapsuleController;
	class PxControllerDesc;
}

class HRigidBody;
class HTriangleMeshRigidBody;
class HInfinitePlaneRigidBody;
class HPlaneRigidBody;
class HSphereRigidBody;
class HCubeRigidBody;
class HCapsuleRigidBody;
class HTrigger;
class HBoxTrigger;
class HPhysicsContext;
class PhysicsCallbackHandler;


struct FRaycastHitInfo
{
	FVector3 HitPos;
	FVector3 HitNormal;
	float Distance;
	float Fraction;
	bool AnyHit;
};

enum EFilterGroup
{
	FG_Player			= (1 << 0),
	FG_Character		= (1 << 1),
	FG_WorldGeometry	= (1 << 2),

	FG_World			= FG_Player | FG_Character | FG_WorldGeometry
};

namespace Physics
{
	extern float Gravity;

	struct DynamicColliderInitParams
	{
		FVector3 StartPos;
		FQuat StartRotation;
		bool IsKinematic;
	};
	template <typename RigidBodyType>
	struct RigidActorAddDesc
	{
		// The callback handler for this actor's collision events.
		PhysicsCallbackHandler* pCallbackHandler;
		// The starting position for this actor.
		FVector3 StartPosition;
		// The starting rotation for this actor.
		FQuat StartRotation;
		// The rigid body to initialize and add to the world.
		RigidBodyType& outRB;
		// Start disabled in the world?
		bool StartDisabled;
		// Is this actor a trigger?
		bool IsTrigger;
	};

	bool IsValid();

	void Initialize();
	void UnInitialize();
	// Steps the physics simulation through time.
	void Update( float DeltaTime, float StepRateScale );

	bool IsSimulationFinished();
	bool IsSimulationPaused();

	void CreateMesh( const FVector3& StartPos, const FQuat& StartRotation, const FVector3& Scale, HTriangleMeshRigidBody& outMesh, bool IsTrigger, void* pUserData, bool IsKinematic, float Density, bool IsStatic, EFilterGroup CollisionGroup, EFilterGroup FilterGroup = FG_World );
	void CreateSphere( const FVector3& StartPos, const FQuat& StartRotation, HSphereRigidBody& outSphere, bool IsTrigger, void* pUserData, bool IsKinematic, float Density, bool IsStatic, EFilterGroup CollisionGroup, EFilterGroup FilterGroup = FG_World );
	void CreatePlane( const FVector3& StartPos, const FQuat& StartRotation, HPlaneRigidBody& outPlane, bool IsTrigger, void* pUserData, bool IsKinematic, float Density, bool IsStatic, EFilterGroup CollisionGroup, EFilterGroup FilterGroup = FG_World );
	void CreateCube( const FVector3& StartPos, const FQuat& StartRotation, HCubeRigidBody& outCube, bool IsTrigger, void* pUserData, bool IsKinematic, float Density, bool IsStatic, EFilterGroup CollisionGroup, EFilterGroup FilterGroup = FG_World );
	void CreateCapsule( const FVector3& StartPos, const FQuat& StartRotation, HCapsuleRigidBody& outCube, bool IsTrigger, void* pUserData, bool IsKinematic, float Density, bool IsStatic, EFilterGroup CollisionGroup, EFilterGroup FilterGroup = FG_World );

	bool RayCast( const FVector3& Origin, const FVector3& UnitDirection, const float& Distance, FRaycastHitInfo* outHitInfo = nullptr, std::vector<HRigidBody*>* IgnoreActors = nullptr );
	bool SphereCast( const FVector3& Start, const float& SphereSize );
	
	FVector3 ScreenToWorldPos( FVector2 ScreenPos );

	physx::PxCapsuleController* CreateCapsuleController( const physx::PxControllerDesc& Desc );
	physx::PxMaterial* CreateDefaultMaterial();
	physx::PxScene* GetScene();

	void PauseSimulation();
	void ResumeSimulation();

	// Blocks the thread and waits for the simulation to finish before proceeding.
	void WaittillSimulationFinished();
	// Blocks the thread and waits until the simulation is paused next.
	void WaittillSimulationPaused();
	void RemoveActor( HRigidBody& Collider );



};
