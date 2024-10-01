#pragma once
// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Components/HRenderableComponentInterface.h"

#include "Transform.h"
#include "Graphics/MaterialManager.h"
#include "Engine/Renderer/ModelManager.h"
#include "Engine/Renderer/ConstantBuffer.h"
#include "Graphics/ConstantBufferStructures.h"
#include "Graphics/GeometryGenerator.h"
#include "Engine/Renderer/Animation.h"


HCOMPONENT()
class HSkeletalMeshComponent : public HRenderableComponentInterface
{
	friend class AActor;
	friend class HScene;
	using Super = HRenderableComponentInterface;
public:
	HE_COMPONENT_GENERATED_BODY( HSkeletalMeshComponent )

	void SetAnimation( HAnimation Animation )	{ m_AnimationAsset = Animation; }
	void SetMesh( HSkeletalMesh Mesh )			
	{ 
		m_MeshAsset = Mesh; 
		for (uint32 i = 0; i < m_MeshAsset->Joints.size(); i++)
		{
			auto& Mesh = m_DebugSkeletonMeshes.emplace_back();
			Mesh.m_MeshWorldCB.Create( L"[Skeletal Mesh Component] Debug World CB" );
			Mesh.m_MeshAsset = GeometryGenerator::GenerateSphere( 5, 10, 10 );
		}
	}
	FTransform& GetTransform()					{ return m_Transform; }

protected:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnAttach() override;
	virtual void Render( FCommandContext& GfxContext ) override;

	virtual void Serialize( JsonUtility::WriteContext& Output ) override;
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;

protected:
	FTransform							m_Transform;
	bool								m_bIsDrawEnabled;

	double								m_StartTimeMillis;
	HAnimation							m_AnimationAsset;

	TConstantBuffer<JointCBData>		m_JointCB;
	TConstantBuffer<MeshWorldCBData>	m_MeshWorldCB;
	HSkeletalMesh						m_MeshAsset;

	struct DebugJoint
	{
		HStaticMesh							m_MeshAsset;
		TConstantBuffer<MeshWorldCBData>	m_MeshWorldCB;
	};
	std::vector<DebugJoint> m_DebugSkeletonMeshes;
	HMaterial				m_DebugMaterialAsset;

};
