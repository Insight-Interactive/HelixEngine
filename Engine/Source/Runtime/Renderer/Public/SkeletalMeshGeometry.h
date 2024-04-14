#pragma once

#include "MeshGeometry.h"

#include "GUID.h"
#include "ConstantBuffer.h"
#include "Hash.h"

#define R_MAX_NUM_BONES_PER_VERTEX 4

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
	Char m_Name[32];
	HHash m_NameHash;

	FMatrix m_LocalMatrix; // relative to this bone's parent
	FMatrix m_OffsetMatrix;
};


HE_ALIGN( 16 ) struct JointCBData
{
	// Skeleton Joints.
	FMatrix kJoints[HE_MAX_JOINTS_PER_MODEL];
};

struct aiBone;

class RENDER_API HSkeletalMeshGeometry 
{
	friend class FGeometryManager;
public:
	HSkeletalMeshGeometry()
	{
	}

	~HSkeletalMeshGeometry()
	{
	}

	std::vector<FMeshGeometry>& GetMeshes();

//private:
//	struct FVertexBoneData
//	{
//		FVertexBoneData();
//		~FVertexBoneData();
//
//		void AddBoneData( uint32 BoneId, float Weight );
//
//		uint32 BoneIds[R_MAX_NUM_BONES_PER_VERTEX];
//		float Weights[R_MAX_NUM_BONES_PER_VERTEX];
//	};

//private:
	std::vector<FMeshGeometry> m_Meshes;
	std::vector<FJoint> Joints;

	FMatrix m_GlobalInverseTransform;
	TConstantBuffer<JointCBData> m_JointCB;

};


//
// Inline function definitions
//

// HSkeletalMeshGeometry



// HSkeletalMeshGeometry::FVertexBoneData

//inline void HSkeletalMeshGeometry::FVertexBoneData::AddBoneData( uint32 BoneId, float Weight )
//{
//	for (uint32 i = 0; i < R_MAX_NUM_BONES_PER_VERTEX; i++)
//	{
//		if (Weights[i] == 0.f)
//		{
//			BoneIds[i] = BoneId;
//			Weights[i] = Weight;
//			//R_LOG( Log, TEXT( "Bond %d Wiehgt %f Index %i" ), BoneId, Weight, i );
//			return;
//		}
//	}
//	HE_ASSERT( false ); // If hit increase R_MAX_NUM_BONES_PER_VERTEX
//}

inline std::vector<FMeshGeometry>& HSkeletalMeshGeometry::GetMeshes()
{
	return m_Meshes;
}
