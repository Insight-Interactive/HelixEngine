// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Components/HSkeletalMeshComponent.h"

#include "World/World.h"
#include "GameFramework/Actor/AActor.h"
#include "Graphics/ConstantBufferStructures.h"
#include "AssetRegistry/AssetDatabase.h"
#include "Engine/Renderer/RendererCore.h"
#include "Graphics/ShaderRegisters.h"


HSkeletalMeshComponent::HSkeletalMeshComponent( FComponentInitArgs& InitArgs )
	: HRenderableComponentInterface( InitArgs )
{
	m_MeshWorldCB.Create( L"[Static Mesh Component] World CB" );
	m_JointCB.Create( L"Skeleton Joints" );
}

HSkeletalMeshComponent::~HSkeletalMeshComponent()
{

}

void HSkeletalMeshComponent::Render( FCommandContext& GfxContext )
{
	Super::Render( GfxContext );

	std::vector<FMatrix> WorldTransforms( m_MeshAsset->Joints.size() );

	if (m_MeshAsset.IsValid())
	{
		double CurrentTimeMillis = double( System::QueryPerfCounter() / 10000 );
		float AnimTimeSeconds = float( (CurrentTimeMillis - m_StartTimeMillis) / 1000.0 );

		if (m_AnimationAsset.IsValid())
		{
			float TimeInTicks = AnimTimeSeconds * (float)m_AnimationAsset->m_TicksPerSecond;
			float AnimationTimeTicks = fmod( TimeInTicks, (float)m_AnimationAsset->m_Duration );
			//HE_LOG( Log, TEXT( "%f" ), AnimTimeSeconds );

			// Calc animation

			for (uint32 i = 0; i < m_MeshAsset->Joints.size(); i++)
			{
				FJoint& joint = m_MeshAsset->Joints[i];

				WorldTransforms[i] = joint.m_LocalMatrix;

				FMatrix ParentTransform = FMatrix::Identity;
				if (joint.m_ParentIndex != R_JOINT_INVALID_INDEX)
					ParentTransform = WorldTransforms[joint.m_ParentIndex];

				if (m_AnimationAsset->m_KeyFrames.find( joint.m_Name ) != m_AnimationAsset->m_KeyFrames.end())
				{
					FMatrix AnimatedResult;

					if (m_AnimationAsset->m_KeyFrames[joint.m_Name].size() == 1)
					{
						// Only one keyframe, no need to lerp between the same frame.

						const FVector3& Position = m_AnimationAsset->m_KeyFrames[joint.m_Name][0].Position;
						const FQuat& Rotation = m_AnimationAsset->m_KeyFrames[joint.m_Name][0].Rotation;

						const FMatrix TranslationMat = FMatrix::CreateTranslation( Position );
						const FMatrix RotationMat = FMatrix::CreateFromQuaternion( Rotation );

						AnimatedResult = (RotationMat * TranslationMat);
					}
					else
					{
						uint32 KeyIndex = 0;
						uint32 NextKeyIndex = 0;
						for (uint32 i = 0; i < m_AnimationAsset->m_KeyFrames[joint.m_Name].size() - 1; i++)
						{
							if (AnimationTimeTicks < m_AnimationAsset->m_KeyFrames[joint.m_Name][i + 1].m_Timestamp)
							{
								KeyIndex = i;
								break;
							}
						}
						NextKeyIndex = KeyIndex + 1;

						float t1 = m_AnimationAsset->m_KeyFrames[joint.m_Name][KeyIndex].m_Timestamp;
						float t2 = m_AnimationAsset->m_KeyFrames[joint.m_Name][NextKeyIndex].m_Timestamp;
						float TimeDiff = t2 - t1;
						float Factor = (AnimationTimeTicks - t1) / TimeDiff;
						HE_ASSERT( Factor >= 0.f && Factor <= 1.f );

						const FVector3& StartPosition = m_AnimationAsset->m_KeyFrames[joint.m_Name][KeyIndex].Position;
						const FVector3& EndPosition = m_AnimationAsset->m_KeyFrames[joint.m_Name][NextKeyIndex].Position;
						const FVector3 AnimatedPosition = FVector3::Lerp( StartPosition, EndPosition, Factor );

						const FQuat& StartRotation = m_AnimationAsset->m_KeyFrames[joint.m_Name][KeyIndex].Rotation;
						const FQuat& EndRotation = m_AnimationAsset->m_KeyFrames[joint.m_Name][NextKeyIndex].Rotation;
						const FQuat AnimatedRotation = FQuat::Lerp( StartRotation, EndRotation, Factor );

						const FMatrix TranslationMat = FMatrix::CreateTranslation( AnimatedPosition );
						const FMatrix RotationMat = FMatrix::CreateFromQuaternion( AnimatedRotation );

						AnimatedResult = RotationMat * TranslationMat;
					}

					WorldTransforms[i] = AnimatedResult * ParentTransform;
				}
				else
				{
					WorldTransforms[i] = FMatrix::Identity;
					/*FMatrix ParentTransform = FMatrix::Identity;
					if (joint.m_ParentIndex != R_JOINT_INVALID_INDEX)
						ParentTransform = WorldTransforms[joint.m_ParentIndex];

					WorldTransforms[i] = ParentTransform * joint.m_LocalMatrix;*/
				}
			}
		}
		else // Has no animation, just use the skeleton
		{
			// TODO: This breaks the debug skeleton

			// TODO: Maybe to this for unanimated bones? Could be the problem with skinning
			for (uint32 i = 0; i < m_MeshAsset->Joints.size(); i++)
			{
				FJoint& joint = m_MeshAsset->Joints[i];

				WorldTransforms[i] = joint.m_LocalMatrix;

				FMatrix ParentTransform = FMatrix::Identity;
				if (joint.m_ParentIndex != R_JOINT_INVALID_INDEX)
					ParentTransform = WorldTransforms[joint.m_ParentIndex];

				WorldTransforms[i] = ParentTransform * joint.m_LocalMatrix;
			}
		}

		m_DebugMaterialAsset->Bind( GfxContext );
		for (uint32 i = 0; i < m_DebugSkeletonMeshes.size(); i++)
		{
			DebugJoint& Joint = m_DebugSkeletonMeshes[i];
			MeshWorldCBData* DebugCBData = Joint.m_MeshWorldCB.GetBufferPointer();
			DebugCBData->kWorldMat = (FMatrix::CreateScale( 0.2f ) * WorldTransforms[i] * m_Transform.GetWorldMatrix()).Transpose();

			GfxContext.SetGraphicsConstantBuffer( kMeshWorld, Joint.m_MeshWorldCB );
			GfxContext.SetPrimitiveTopologyType( PT_TiangleList );
			GfxContext.BindVertexBuffer( 0, Joint.m_MeshAsset->GetMesh().GetVertexBuffer() );
			GfxContext.BindIndexBuffer( Joint.m_MeshAsset->GetMesh().GetIndexBuffer() );
			GfxContext.DrawIndexedInstanced( Joint.m_MeshAsset->GetMesh().GetNumIndices(), 1, 0, 0, 0 );
		}


		for (uint32 i = 0; i < m_MeshAsset->Joints.size(); i++)
		{
			FJoint& joint = m_MeshAsset->Joints[i];

			m_JointCB->kJoints[i] = ( WorldTransforms[i] * joint.m_OffsetMatrix ).Transpose();
		}

		if (m_MaterialAsset.IsValid())
		{
			// Set the material information.
			m_MaterialAsset->Bind( GfxContext );
		}

		// Set the world buffer.
		m_MeshWorldCB->kWorldMat = m_Transform.GetWorldMatrix().Transpose();

		//m_JointCB.UploadBuffer();

		//for (uint32 i = 0; i < m_MeshAsset->m_Meshes.size(); i++)
		//{
		//	GfxContext.SetGraphicsConstantBuffer( kMeshWorld, m_MeshWorldCB );

		//	// Set Joints
		//	GfxContext.SetGraphicsConstantBuffer( kSkeletonJoints, m_JointCB, false );

		//	FMesh& SKMesh = m_MeshAsset->m_Meshes[i];

		//	// TODO Request draw from model in model manager to render meshes of the same type in batches.
		//	// Render the geometry.
		//	GfxContext.SetPrimitiveTopologyType( PT_TiangleList );
		//	GfxContext.BindVertexBuffer( 0, SKMesh.GetVertexBuffer() );
		//	GfxContext.BindIndexBuffer( SKMesh.GetIndexBuffer() );
		//	GfxContext.DrawIndexedInstanced( SKMesh.GetNumIndices(), 1, 0, 0, 0 );
		//}

	}
}

void HSkeletalMeshComponent::OnCreate()
{
	Super::OnCreate();

	m_Transform.LinkTo( GetOwner()->GetTransform() );
	GetWorld()->GetScene().AddSkeletalMesh( this );


	m_StartTimeMillis = double( System::QueryPerfCounter() / 10000 );

	m_DebugMaterialAsset = FAssetDatabase::GetMaterial( "M_SolidColor.hmat" );
}

void HSkeletalMeshComponent::OnDestroy()
{
	Super::OnDestroy();

	GetWorld()->GetScene().RemoveSkeletalMesh( this );
}

void HSkeletalMeshComponent::OnAttach()
{
	Super::OnAttach();

}

void HSkeletalMeshComponent::Serialize( JsonUtility::WriteContext& Output )
{
	Output.Key( HE_STRINGIFY( HSkeletalMeshComponent::m_bIsDrawEnabled ) );
	Output.Bool( m_bIsDrawEnabled );

	Output.Key( HE_STRINGIFY( HSkeletalMeshComponent::m_MeshAsset ) );
	Output.String( m_MeshAsset->GetGuid().ToString().CStr() );

	Output.Key( HE_STRINGIFY( HSkeletalMeshComponent::m_MaterialAsset ) );
	Output.String( m_MaterialAsset->GetGuid().ToString().CStr() );

	Super::Serialize( Output );
}

void HSkeletalMeshComponent::Deserialize( const JsonUtility::ReadContext& Value )
{
	Super::Deserialize( Value );

	Char StringBuffer[64];
	ZeroMemory( StringBuffer, sizeof( StringBuffer ) );

	JsonUtility::GetTransform( Value, HE_STRINGIFY( HSkeletalMeshComponent::m_Transform ), m_Transform );

	JsonUtility::GetBoolean( Value, HE_STRINGIFY( HSkeletalMeshComponent::m_bIsDrawEnabled ), m_bIsDrawEnabled );

	if (JsonUtility::GetString( Value, HE_STRINGIFY( HSkeletalMeshComponent::m_AnimationAsset ), StringBuffer, sizeof( StringBuffer ) ))
		SetAnimation( FAssetDatabase::GetAnimation( StringBuffer ) );

	if (JsonUtility::GetString( Value, HE_STRINGIFY( HSkeletalMeshComponent::m_MeshAsset ), StringBuffer, sizeof( StringBuffer ) ))
		SetMesh( FAssetDatabase::GetSkeletalMesh( StringBuffer ) );

	if (JsonUtility::GetString( Value, HE_STRINGIFY( HSkeletalMeshComponent::m_MaterialAsset ), StringBuffer, sizeof( StringBuffer ) ))
		SetMaterial( FAssetDatabase::GetMaterial( StringBuffer ) );
}
