#pragma once

#include "MeshGeometry.h"
#include "ConstantBuffer.h"
#include "Hash.h"


#define R_JOINT_INVALID_INDEX uint32(-1)

struct FJoint
{
	FJoint()
	{
		HE_ZERO_MEMORY( this, sizeof( FJoint ) );
		m_OffsetMatrix = FMatrix::Identity;
		m_LocalMatrix = FMatrix::Identity;
		m_ParentIndex = R_JOINT_INVALID_INDEX;
	}
	uint32 m_ParentIndex;
	Char m_Name[64];
	HHash m_NameHash;

	FMatrix m_LocalMatrix; // Relative to this bone's parent
	FMatrix m_OffsetMatrix;
};



class RENDER_API HSkeletalMeshGeometry 
{
	friend class FGeometryManager;
public:
	HSkeletalMeshGeometry() = default;
	~HSkeletalMeshGeometry() = default;

	std::vector<FMeshGeometry> m_Meshes;
	std::vector<FJoint> Joints;

	FMatrix m_GlobalInverseTransform;

};

