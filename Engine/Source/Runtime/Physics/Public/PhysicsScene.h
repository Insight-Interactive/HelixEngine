#pragma once


namespace physx
{
	class PxScene;
}

class RigidBody;
class PhysicsContext;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	bool IsReady() const;

	void Setup( PhysicsContext& PhysicsContext );
	void Teardown();

	void Tick( float StepRate );
	void QuerySimulationResults();

	void CreatePlane( PhysicsContext& PhysicsContext, bool IsStatic, RigidBody& outPlane );
	void CreateSphere( PhysicsContext& PhysicsContext, bool IsStatic, RigidBody& outSphere );

protected:
	physx::PxScene* m_pScene;

};

// 
// Inline function implementations
//

FORCEINLINE bool PhysicsScene::IsReady() const
{
	return m_pScene != nullptr;
}
