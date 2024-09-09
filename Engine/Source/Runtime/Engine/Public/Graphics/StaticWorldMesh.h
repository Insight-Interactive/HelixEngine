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
	}
	~FWorldMesh()
	{
		m_MeshWorldCB.Destroy();
	}
	
	TConstantBuffer<MeshWorldCBData>	m_MeshWorldCB;
	HStaticMesh m_Mesh;
	HMaterial	m_Material;

	HTriangleMeshRigidBody m_Collision;
};
