#pragma once

#include "GameFramework/Components/HRenderableComponentInterface.h"

#include "ModelManager.h"
#include "Renderer/ConstantBufferStructures.h"
#include "Renderer/MaterialManager.h"
#include "AnimationManager.h"
#include "Renderer/GeometryGenerator.h"


HCOMPONENT()
class HSkeletalMeshComponent : public HRenderableComponenetInterface
{
	friend class AActor;
	friend class HScene;
	using Super = HRenderableComponenetInterface;
public:
	HE_COMPONENT_GENERATED_BODY( HSkeletalMeshComponent );

	void SetSkeletalMesh( HSkeletalMesh Mesh );

	void BeginPlay() override;

protected:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Render( FCommandContext& GfxContext ) override;

	virtual void Serialize( JsonUtility::WriteContext& Output ) override;
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;

private:
	HSkeletalMesh						m_SkeletalMesh;
	TConstantBuffer<MeshWorldCBData>	m_MeshWorldCB;

	double m_StartTimeMillis;
	HAnimation m_Animation;

	TConstantBuffer<JointCBData>		m_JointCB;

	struct DebugJoint
	{
		HStaticMesh							m_MeshAsset;
		TConstantBuffer<MeshWorldCBData>	m_MeshWorldCB;
	};
	std::vector<DebugJoint> m_DebugSkeletonMeshes;
	HMaterial				m_DebugMaterialAsset;
};

// 
// Inline functions
//

FORCEINLINE void HSkeletalMeshComponent::SetSkeletalMesh( HSkeletalMesh Mesh )
{
	m_SkeletalMesh = Mesh;

	for (uint32 i = 0; i < m_SkeletalMesh->Joints.size(); i++)
	{
		auto& Mesh = m_DebugSkeletonMeshes.emplace_back();
		Mesh.m_MeshWorldCB.Create( L"[Skeletal Mesh COmponent] Debug World CB" );
		Mesh.m_MeshAsset = GeometryGenerator::GenerateSphere( 5, 10, 10 );
	}
}
