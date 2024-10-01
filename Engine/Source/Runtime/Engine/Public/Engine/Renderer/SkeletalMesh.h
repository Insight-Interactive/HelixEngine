// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "Engine/Renderer/ManagedAsset.h"

#include "RendererFwd.h"
#include "CoreFwd.h"

#include "RendererCore.h"
#include "GUID.h"
#include "Hash.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Engine/Renderer/Mesh.h"


#define R_JOINT_INVALID_INDEX uint32(-1)

struct FJoint
{
	FJoint()
	{
		ZeroMemory( this, sizeof( FJoint ) );
		m_OffsetMatrix = FMatrix::Identity;
		m_LocalMatrix = FMatrix::Identity;
		m_ParentIndex = R_JOINT_INVALID_INDEX;
	}
	uint32 m_ParentIndex;
	Char m_Name[64];
	StringHashValue m_NameHash;

	FMatrix m_LocalMatrix; // Relative to this bone's parent
	FMatrix m_OffsetMatrix;
};

class FSkeletalMesh : public ManagedAsset<FSkeletalMesh>
{
	friend class FSkeletalGeometryManager;
	friend class FAssetDatabase;
public:
	FSkeletalMesh() 
	{
	}
	virtual ~FSkeletalMesh() 
	{
	}

	FGUID GetGuid() const { return m_Guid; }


	std::vector<FMesh> m_Meshes;
	std::vector<FJoint> Joints;

	FMatrix m_GlobalInverseTransform;

protected:
	void SetGuid( const FGUID& NewGUID ) { m_Guid = NewGUID; }
	
private:
	FGUID m_Guid;

};

typedef ManagedAsset<FSkeletalMesh> ManagedSkeletalMesh;
typedef AssetRef<FSkeletalMesh> HSkeletalMesh;
