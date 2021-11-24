// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Components/HStaticMeshComponent.h"

#include "RendererCore.h"
#include "CommandContext.h"
#include "ConstantBuffer.h"
#include "Renderer/ShaderRegisters.h"
#include "Renderer/ConstantBufferStructures.h"
#include "GameFramework/Actor/AActor.h"
#include "AssetRegistry/AssetDatabase.h"
#include "World/World.h"
#include "Engine/Engine.h"

HStaticMeshComponent::HStaticMeshComponent(const HName& Name)
	: HSceneComponent(Name)
{
	m_MeshWorldCB.Create( L"[Static Mesh Component] World CB" );
}

HStaticMeshComponent::~HStaticMeshComponent()
{
	m_MeshWorldCB.Destroy();
}

void HStaticMeshComponent::Render(FCommandContext& GfxContext)
{
	if (!GetIsDrawEnabled()) return;
	
	if (m_Material.IsValid())
	{
		// Set the material information.
		m_Material->Bind(GfxContext);
	}

	if (m_Geometry->IsValid())
	{
		// Set the world buffer.
		MeshWorldCBData* pWorld = m_MeshWorldCB.GetBufferPointer();
		pWorld->kWorldMat =  GetTransform().GetWorldMatrix().Transpose();
		m_MeshWorldCB.SetDirty(true);
		GfxContext.SetGraphicsConstantBuffer(kMeshWorld, m_MeshWorldCB);

		// TODO Request draw from model in model manager to render meshes of the same type in batches.
		// Render the geometry.
		GfxContext.SetPrimitiveTopologyType(PT_TiangleList);
		GfxContext.BindVertexBuffer(0, m_Geometry->GetVertexBuffer());
		GfxContext.BindIndexBuffer(m_Geometry->GetIndexBuffer());
		GfxContext.DrawIndexedInstanced(m_Geometry->GetNumIndices(), 1, 0, 0, 0);
	}
}

void HStaticMeshComponent::OnCreate() 
{
	GetWorld()->GetScene()->AddStaticMesh( this );
}

void HStaticMeshComponent::OnDestroy()
{
	GetWorld()->GetScene()->RemoveStaticMesh( this );
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

	Char MeshGuidBuffer[64];
	ZeroMemory( MeshGuidBuffer, sizeof( MeshGuidBuffer ) );
	JsonUtility::GetString( StaticMesh, "MeshAsset", MeshGuidBuffer, sizeof( MeshGuidBuffer ) );
	FGUID MeshGuid = FGUID::CreateFromString( MeshGuidBuffer );
	SetMesh( FAssetDatabase::GetInstance()->GetStaticMesh( MeshGuid ) );

	Char MaterialGuidBuffer[64];
	ZeroMemory( MaterialGuidBuffer, sizeof( MaterialGuidBuffer ) );
	JsonUtility::GetString( StaticMesh, "MaterialAsset", MaterialGuidBuffer, sizeof( MaterialGuidBuffer ) );
	FGUID MatAssetGuid = FGUID::CreateFromString( MaterialGuidBuffer );
	SetMaterial( FAssetDatabase::GetInstance()->GetMaterial( MatAssetGuid ) );

	bool IsRenderable = true;
	JsonUtility::GetBoolean( StaticMesh, "IsRenderable", IsRenderable );
	SetIsDrawEnabled( IsRenderable );

	JsonUtility::GetTransform( StaticMesh, "LocalTransform", GetTransform() );
}
