#include "EnginePCH.h"

#include "GameFramework/Components/HSkeletalMeshComponenet.h"

#include "World/World.h"
#include "AssetRegistry/AssetDatabase.h"
#include "CommandContext.h"
#include "Renderer/ShaderRegisters.h"
#include "AnimationManager.h"
#include "Animation.h"
#include "Engine/Engine.h"


extern FAnimationManager GAnimationManager;

HSkeletalMeshComponent::HSkeletalMeshComponent( FComponentInitArgs& InitArgs )
	: HRenderableComponenetInterface( InitArgs )
{
	m_MeshWorldCB.Create( L"[Skeletal Mesh Component] World CB" );
}

HSkeletalMeshComponent::~HSkeletalMeshComponent()
{
}

void HSkeletalMeshComponent::BeginPlay()
{
}

void HSkeletalMeshComponent::OnCreate()
{
	Super::OnCreate();

	GetWorld()->GetScene().AddSkeletalMesh( this );
	
	FGUID MaterialGuid = FGUID::CreateFromString( "1a0f9c51-367c-4f09-9c04-e86bed7d92e7" );
	HMaterial NewMaterial = FAssetDatabase::GetMaterial( MaterialGuid );

	SetMaterial( NewMaterial );

	m_anim = FAssetDatabase::GetAnimation( "34338626-951c-492f-b41f-f852ce5e074c" ); //34338626-951c-492f-b41f-f852ce5e074c 6127cd49-9fee-467b-9d27-164df2fba4f5
	m_StartTimeMillis = double(System::QueryPerfCounter() / 10000);

	m_DebugMaterialAsset = FAssetDatabase::GetMaterial( "e40a0db0-8368-46b2-be14-6aa89eddba81" );
}

void HSkeletalMeshComponent::OnDestroy()
{
	Super::OnDestroy();

	GetWorld()->GetScene().RemoveSkeletalMesh( this );
}

void HSkeletalMeshComponent::Render( FCommandContext& GfxContext )
{
	Super::Render( GfxContext );

	if (!GetIsDrawEnabled()) return;


	if (m_SkeletalMesh->IsValid())
	{
		double CurrentTimeMillis = double(System::QueryPerfCounter() / 10000);
		float AnimTimeSeconds = float( (CurrentTimeMillis - m_StartTimeMillis) / 1000.0 );


		float TimeInTicks = AnimTimeSeconds * (float)m_anim->m_TicksPerSecond;
		float AnimationTimeTicks = fmod( TimeInTicks, (float)m_anim->m_Duration );
		//HE_LOG( Log, TEXT( "%f" ), AnimTimeSeconds );

		// Calc animation
		std::vector<FMatrix> LocalTransforms( m_SkeletalMesh->Joints.size() );

		HHash hashName = StringHash( "pelvis" );
		for (uint32 i = 0; i < m_SkeletalMesh->Joints.size(); i++)
		{
			FJoint& joint = m_SkeletalMesh->Joints[i];

			LocalTransforms[i] = joint.m_LocalMatrix;

			if (m_anim->m_KeyFrames.find( joint.m_Name ) != m_anim->m_KeyFrames.end())
			{
				uint32 KeyIndex = 0;
				uint32 NextKeyIndex = 0;
				for (uint32 i = 0; i < m_anim->m_KeyFrames[joint.m_Name].size() - 1; i++)
				{
					if (AnimationTimeTicks < m_anim->m_KeyFrames[joint.m_Name][i + 1].m_Timestamp)
					{
						KeyIndex = i;
						break;
					}
				}
				NextKeyIndex = KeyIndex + 1;
				float t1 = m_anim->m_KeyFrames[joint.m_Name][KeyIndex].m_Timestamp;
				float t2 = m_anim->m_KeyFrames[joint.m_Name][NextKeyIndex].m_Timestamp;
				float DeltaTime = t2 - t1;
				float Factor = (AnimationTimeTicks - t1) / DeltaTime;
				HE_ASSERT( Factor >= 0.f && Factor <= 1.f );
				FTransform& Start = m_anim->m_KeyFrames[joint.m_Name][KeyIndex].m_AnimatedTransform;
				FTransform& End = m_anim->m_KeyFrames[joint.m_Name][NextKeyIndex].m_AnimatedTransform;
				FTransform AnimatedTransform = FTransform::Interpolate( Start, End, Factor );
				 
				//FTransform AnimatedTransform = m_anim->m_KeyFrames[joint.m_Name][15].m_AnimatedTransform;

				FMatrix ParentTransform = FMatrix::Identity;
				if (joint.m_ParentIndex != R_JOINT_INVALID_INDEX)
					ParentTransform = LocalTransforms[joint.m_ParentIndex];

				//FMatrix Rotation;
				//if (joint.m_NameHash == hashName)
				//	Rotation = FMatrix::CreateFromYawPitchRoll( GEngine->GetAppSeconds(), 0, 0 );

				LocalTransforms[i] = ParentTransform * /*joint.m_LocalMatrix*/AnimatedTransform.GetLocalMatrix();
			}
		}

		m_DebugMaterialAsset->Bind(GfxContext);

		for (uint32 i = 0; i < m_DebugSkeletonMeshes.size(); i++)
		{
			DebugJoint& Joint = m_DebugSkeletonMeshes[i];
			MeshWorldCBData* DebugCBData = Joint.m_MeshWorldCB.GetBufferPointer();
			DebugCBData->kWorldMat = LocalTransforms[i].Transpose();
			Joint.m_MeshWorldCB.SetDirty(true );
			GfxContext.SetGraphicsConstantBuffer( kMeshWorld, Joint.m_MeshWorldCB );
			GfxContext.SetPrimitiveTopologyType( PT_TiangleList );
			GfxContext.BindVertexBuffer( 0, Joint.m_MeshAsset->GetVertexBuffer() );
			GfxContext.BindIndexBuffer( Joint.m_MeshAsset->GetIndexBuffer() );
			GfxContext.DrawIndexedInstanced( Joint.m_MeshAsset->GetNumIndices(), 1, 0, 0, 0 );
		}


		JointCBData* pJointCB = m_SkeletalMesh->m_JointCB.GetBufferPointer();
		for (uint32 i = 0; i < m_SkeletalMesh->Joints.size(); i++)
		{
			FJoint& joint = m_SkeletalMesh->Joints[i];
			FMatrix& FinalTransform = pJointCB->kJoints[i];

			FinalTransform = /*m_SkeletalMesh->m_GlobalInverseTransform **/ LocalTransforms[i] * joint.m_OffsetMatrix;
			FinalTransform = FinalTransform.Transpose();
		}
		m_SkeletalMesh->m_JointCB.SetDirty( true );

		if (m_MaterialAsset.IsValid())
		{
			// Set the material information.
			m_MaterialAsset->Bind( GfxContext );
		}

		// Set the world buffer.
		MeshWorldCBData* pWorld = m_MeshWorldCB.GetBufferPointer();
		pWorld->kWorldMat = GetWorldMatrix().Transpose();
		m_MeshWorldCB.SetDirty( true );
		GfxContext.SetGraphicsConstantBuffer( kMeshWorld, m_MeshWorldCB );

		// Set Joints
		GfxContext.SetGraphicsConstantBuffer( kSkeletonBones, m_SkeletalMesh->m_JointCB );

		std::vector<FMeshGeometry>& Meshes = m_SkeletalMesh->GetMeshes();
		for (uint32 i = 0; i < Meshes.size(); i++)
		{
			FMeshGeometry& SKMesh = Meshes[i];

			// TODO Request draw from model in model manager to render meshes of the same type in batches.
			// Render the geometry.
			GfxContext.SetPrimitiveTopologyType( PT_TiangleList );
			GfxContext.BindVertexBuffer( 0, SKMesh.GetVertexBuffer() );
			GfxContext.BindIndexBuffer( SKMesh.GetIndexBuffer() );
			GfxContext.DrawIndexedInstanced( SKMesh.GetNumIndices(), 1, 0, 0, 0 );
			break;
		}
	}
}
