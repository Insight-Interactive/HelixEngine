// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Components/HStaticMeshComponent.h"

#include "RendererCore.h"
#include "IConstantBufferManager.h"
#include "Renderer/ConstantBufferStructures.h"
#include "Renderer/ShaderRegisters.h"
#include "ICommandContext.h"
#include "AssetRegistry/AssetDatabase.h"
#include "GameFramework/Actor/AActor.h"


HStaticMeshComponent::HStaticMeshComponent(const HName& Name)
	: HActorComponent(Name)
{
	GConstantBufferManager->CreateConstantBuffer(L"[Static Mesh Component] World CB", &m_pMeshWorldCB, sizeof(MeshWorldCBData));
}

HStaticMeshComponent::~HStaticMeshComponent()
{
	GConstantBufferManager->DestroyConstantBuffer( m_pMeshWorldCB->GetUID() );
}

void HStaticMeshComponent::Render(ICommandContext& GfxContext)
{
	if (!GetIsDrawEnabled()) return;

	if (m_MaterialRef.IsValid())
	{
		// Set the material information.
		m_MaterialRef->Bind(GfxContext);
	}

	if (m_GeometryRef->IsValid())
	{
		// Set the world buffer.
		MeshWorldCBData* pWorld = m_pMeshWorldCB->GetBufferPointer<MeshWorldCBData>();
		pWorld->WorldMat =  m_Transform.GetWorldMatrix().Transpose();
		GfxContext.SetGraphicsConstantBuffer(kMeshWorld, m_pMeshWorldCB);

		// TODO Request draw from model in model manager to render meshes of the same type in batches.
		// Render the geometry.
		GfxContext.SetPrimitiveTopologyType(PT_TiangleList);
		GfxContext.BindVertexBuffer(0, m_GeometryRef->GetVertexBuffer());
		GfxContext.BindIndexBuffer(m_GeometryRef->GetIndexBuffer());
		GfxContext.DrawIndexedInstanced(m_GeometryRef->GetNumIndices(), 1, 0, 0, 0);
	}
}

void HStaticMeshComponent::OnAttach()
{
	Super::OnAttach();

	GetTransform().SetParent( &GetOwner()->GetTransform() );
}

void HStaticMeshComponent::Serialize( WriteContext& Output )
{
	Super::Serialize( Output );

}

void HStaticMeshComponent::Deserialize( const ReadContext& Value ) 
{
	Super::Deserialize( Value[0] );

	const rapidjson::Value& StaticMesh = Value[1];

	Char MeshNameBuffer[32];
	JsonUtility::GetString( StaticMesh, "MeshAsset", MeshNameBuffer, sizeof( MeshNameBuffer ) );
	SetMesh( AssetDatabase::GetInstance()->GetStaticMesh( MeshNameBuffer ) );

	Char MaterialNameBuffer[32];
	JsonUtility::GetString( StaticMesh, "MaterialAsset", MaterialNameBuffer, sizeof( MaterialNameBuffer ) );
	SetMaterial( AssetDatabase::GetInstance()->GetMaterial( MaterialNameBuffer ) );

	bool IsRenderable = false;
	JsonUtility::GetBoolean( StaticMesh, "IsRenderable", IsRenderable );
	SetIsDrawEnabled( IsRenderable );

	JsonUtility::GetTransform( StaticMesh, "LocalTransform", GetTransform() );
}
