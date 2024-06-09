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
	m_JointCB.Create( L"Skeleton Joints" );
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
	
	HMaterial NewMaterial = FAssetDatabase::GetMaterial( "1a0f9c51-367c-4f09-9c04-e86bed7d92e7" );
	SetMaterial( NewMaterial );

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

	std::vector<FMatrix> WorldTransforms( m_SkeletalMesh->Joints.size() );

	if (m_SkeletalMesh->IsValid())
	{
		double CurrentTimeMillis = double(System::QueryPerfCounter() / 10000);
		float AnimTimeSeconds = float( (CurrentTimeMillis - m_StartTimeMillis) / 1000.0 );

		if (m_Animation.IsValid())
		{
			float TimeInTicks = AnimTimeSeconds * (float)m_Animation->m_TicksPerSecond;
			float AnimationTimeTicks = fmod( TimeInTicks, (float)m_Animation->m_Duration );
			//HE_LOG( Log, TEXT( "%f" ), AnimTimeSeconds );

			// Calc animation

			HHash hashName = StringHash( "pelvis" );
			for (uint32 i = 0; i < m_SkeletalMesh->Joints.size(); i++)
			{
				FJoint& joint = m_SkeletalMesh->Joints[i];

				WorldTransforms[i] = joint.m_LocalMatrix;

				FMatrix ParentTransform = FMatrix::Identity;
				if (joint.m_ParentIndex != R_JOINT_INVALID_INDEX)
					ParentTransform = WorldTransforms[joint.m_ParentIndex];

				if (m_Animation->m_KeyFrames.find( joint.m_Name ) != m_Animation->m_KeyFrames.end())
				{
					FMatrix AnimatedResult;

					if (m_Animation->m_KeyFrames[joint.m_Name].size() == 1)
					{
						// Only one keyframe, no need to lerp between the same frame.

						const FVector3& Position = m_Animation->m_KeyFrames[joint.m_Name][0].Position;
						const FQuat& Rotation = m_Animation->m_KeyFrames[joint.m_Name][0].Rotation;

						const FMatrix TranslationMat = FMatrix::CreateTranslation( Position );
						const FMatrix RotationMat = FMatrix::CreateFromQuaternion( Rotation );

						AnimatedResult = (RotationMat * TranslationMat);
					}
					else
					{
						uint32 KeyIndex = 0;
						uint32 NextKeyIndex = 0;
						for (uint32 i = 0; i < m_Animation->m_KeyFrames[joint.m_Name].size() - 1; i++)
						{
							if (AnimationTimeTicks < m_Animation->m_KeyFrames[joint.m_Name][i + 1].m_Timestamp)
							{
								KeyIndex = i;
								break;
							}
						}
						NextKeyIndex = KeyIndex + 1;

						float t1 = m_Animation->m_KeyFrames[joint.m_Name][KeyIndex].m_Timestamp;
						float t2 = m_Animation->m_KeyFrames[joint.m_Name][NextKeyIndex].m_Timestamp;
						float TimeDiff = t2 - t1;
						float Factor = (AnimationTimeTicks - t1) / TimeDiff;
						HE_ASSERT( Factor >= 0.f && Factor <= 1.f );

						const FVector3& StartPosition = m_Animation->m_KeyFrames[joint.m_Name][KeyIndex].Position;
						const FVector3& EndPosition = m_Animation->m_KeyFrames[joint.m_Name][NextKeyIndex].Position;
						const FVector3 AnimatedPosition = FVector3::Lerp( StartPosition, EndPosition, Factor );

						const FQuat& StartRotation = m_Animation->m_KeyFrames[joint.m_Name][KeyIndex].Rotation;
						const FQuat& EndRotation = m_Animation->m_KeyFrames[joint.m_Name][NextKeyIndex].Rotation;
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
				}
			}
		}
		else
		{
			HHash hashName = StringHash( "pelvis" );
			for (uint32 i = 0; i < m_SkeletalMesh->Joints.size(); i++)
			{
				FJoint& joint = m_SkeletalMesh->Joints[i];

				WorldTransforms[i] = joint.m_LocalMatrix;

				FMatrix ParentTransform = FMatrix::Identity;
				if (joint.m_ParentIndex != R_JOINT_INVALID_INDEX)
					ParentTransform = WorldTransforms[joint.m_ParentIndex];
					
				WorldTransforms[i] = ParentTransform * joint.m_LocalMatrix;
			}
		}
			
		m_DebugMaterialAsset->Bind(GfxContext);
		for (uint32 i = 0; i < m_DebugSkeletonMeshes.size(); i++)
		{
			DebugJoint& Joint = m_DebugSkeletonMeshes[i];
			MeshWorldCBData* DebugCBData = Joint.m_MeshWorldCB.GetBufferPointer();
			DebugCBData->kWorldMat = (FMatrix::CreateScale( 0.5f ) * WorldTransforms[i] * GetWorldMatrix()).Transpose();
			Joint.m_MeshWorldCB.SetDirty(true );
			
			GfxContext.SetGraphicsConstantBuffer( kMeshWorld, Joint.m_MeshWorldCB );
			GfxContext.SetPrimitiveTopologyType( PT_TiangleList );
			GfxContext.BindVertexBuffer( 0, Joint.m_MeshAsset->GetVertexBuffer() );
			GfxContext.BindIndexBuffer( Joint.m_MeshAsset->GetIndexBuffer() );
			GfxContext.DrawIndexedInstanced( Joint.m_MeshAsset->GetNumIndices(), 1, 0, 0, 0 );
		}


		JointCBData* pJointCB = m_JointCB.GetBufferPointer();
		for (uint32 i = 0; i < m_SkeletalMesh->Joints.size(); i++)
		{
			FJoint& joint = m_SkeletalMesh->Joints[i];
			FMatrix& FinalTransform = pJointCB->kJoints[i];

			FinalTransform = ( WorldTransforms[i] * joint.m_OffsetMatrix ).Transpose();
		}
		m_JointCB.SetDirty( true );

		if (m_MaterialAsset.IsValid())
		{
			// Set the material information.
			m_MaterialAsset->Bind( GfxContext );
		}

		// Set the world buffer.
		MeshWorldCBData* pWorld = m_MeshWorldCB.GetBufferPointer();
		pWorld->kWorldMat = GetWorldMatrix().Transpose();
		m_MeshWorldCB.SetDirty( true );

		for (uint32 i = 0; i < m_SkeletalMesh->m_Meshes.size(); i++)
		{
			GfxContext.SetGraphicsConstantBuffer( kMeshWorld, m_MeshWorldCB );

			// Set Joints
			GfxContext.SetGraphicsConstantBuffer( kSkeletonBones, m_JointCB );

			FMeshGeometry& SKMesh = m_SkeletalMesh->m_Meshes[i];

			// TODO Request draw from model in model manager to render meshes of the same type in batches.
			// Render the geometry.
			GfxContext.SetPrimitiveTopologyType( PT_TiangleList );
			GfxContext.BindVertexBuffer( 0, SKMesh.GetVertexBuffer() );
			GfxContext.BindIndexBuffer( SKMesh.GetIndexBuffer() );
			GfxContext.DrawIndexedInstanced( SKMesh.GetNumIndices(), 1, 0, 0, 0 );
		}
	}
}

void HSkeletalMeshComponent::Serialize( JsonUtility::WriteContext& Output )
{
	Output.Key( HE_STRINGIFY( HSkeletalMeshComponent ) );
	Output.StartArray();
	{

	}
	Output.EndArray();
}

void HSkeletalMeshComponent::Deserialize( const JsonUtility::ReadContext& Value )
{
	Super::Deserialize( Value[0][HE_STRINGIFY( HSceneComponent )] );

	const rapidjson::Value& SkeletalMesh = Value[1];

	JsonUtility::GetBoolean( SkeletalMesh, HE_STRINGIFY( m_bIsDrawEnabled ), m_bIsDrawEnabled );

	Char GuidBuffer[64];
	ZeroMemory( GuidBuffer, sizeof( GuidBuffer ) );
	JsonUtility::GetString( SkeletalMesh, HE_STRINGIFY( m_SkeletalMesh ), GuidBuffer, sizeof( GuidBuffer ) );
	SetSkeletalMesh( FAssetDatabase::GetSkeletalMesh( GuidBuffer ) );

	ZeroMemory( GuidBuffer, sizeof( GuidBuffer ) );
	JsonUtility::GetString( SkeletalMesh, HE_STRINGIFY( m_Animation ), GuidBuffer, sizeof( GuidBuffer ) );
	m_Animation = FAssetDatabase::GetAnimation( GuidBuffer );
}