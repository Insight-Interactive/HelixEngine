#pragma once

#include "Graphics/MaterialManager.h"
#include "Engine/Renderer/ModelManager.h"
#include "Engine/Physics/RigidBody.h"
#include "Graphics/ConstantBufferStructures.h"


class FWorldMesh
{
public:
	FWorldMesh()
	{
		m_MeshWorldCB.Create( L"[Static Mesh Component] World CB" );

		m_CollisionWireframeMaterial = new FMaterialInstance();
		m_CollisionWireframeMaterial->CreateFromParent( "M_CollisionVisualizeWireframe.hmat" );
	}
	~FWorldMesh()
	{
		m_MeshWorldCB.Destroy();
		delete m_CollisionWireframeMaterial;
	}
	
	TConstantBuffer<MeshWorldCBData>	m_MeshWorldCB;
	HStaticMesh m_Mesh;
	HMaterial	m_Material;

	FMaterialInstance* m_CollisionWireframeMaterial;
	HTriangleMeshRigidBody m_Collision;
};
