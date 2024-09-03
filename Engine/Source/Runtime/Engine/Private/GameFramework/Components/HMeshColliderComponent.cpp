#include "EnginePCH.h"

#include "GameFramework/Components/HMeshColliderComponent.h"

#include "Engine/Physics/Physics.h"
#include "GameFramework/Actor/AActor.h"
#include "AssetRegistry/AssetDatabase.h"

#include "Engine/GameProject.h"
#include "Engine/Renderer/VertexLayouts.h"


HMeshColliderComponent::HMeshColliderComponent( FComponentInitArgs& InitArgs )
	: HColliderComponent( InitArgs )
{
}

HMeshColliderComponent ::~HMeshColliderComponent()
{
}

void HMeshColliderComponent::Serialize( JsonUtility::WriteContext& Output )
{

}

void HMeshColliderComponent::Deserialize( const JsonUtility::ReadContext& Value )
{
	Super::Deserialize( Value );
	char MeshName[64];
	JsonUtility::GetString( Value, HE_STRINGIFY( HMeshColliderComponent::m_MeshAsset ), MeshName, sizeof( MeshName ) );

	FPath Path;
	sprintf_s( Path.m_Path, "%sModels\\%s", FGameProject::GetInstance()->GetContentFolder(), MeshName );

#if HE_DEBUG
	m_MeshAsset = FAssetDatabase::GetStaticMesh( MeshName );
#endif

	std::vector<FSimpleVertex3D> Verticies;
	uint32 VertexCount;
	std::vector<uint32> Indices;
	uint32 IndexCount;
	GStaticGeometryManager.LoadGometry( Path, Verticies, VertexCount, Indices, IndexCount );
	m_RigidBody.pTriangleData = Verticies.data();
	m_RigidBody.TriCount = VertexCount;
	m_RigidBody.VertexSize = sizeof( FSimpleVertex3D );
	m_RigidBody.pIndexData = Indices.data();
	m_RigidBody.IndexCount = IndexCount;
	m_RigidBody.IndexSize = sizeof( uint32 );
	m_IsStatic = true;
	Physics::CreateMesh(
		GetWorldPosition(),
		GetRotation(),
		GetScale(),
		m_RigidBody,
		GetIsTrigger(),
		(PhysicsCallbackHandler*)this,
		false,
		10.f,
		m_IsStatic, FG_WorldGeometry );
}
