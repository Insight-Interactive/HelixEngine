// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Components/HStaticMeshComponent.h"

#include "World/World.h"
#include "Engine/Renderer/RendererCore.h"
#include "Engine/Renderer/CommandContext.h"
#include "Graphics/ShaderRegisters.h"
#include "GameFramework/Actor/AActor.h"
#include "AssetRegistry/AssetDatabase.h"
#include "Graphics/ConstantBufferStructures.h"
#include "GameFramework/Actor/AActor.h"
#include "World/World.h"
#include "Engine/Renderer/BatchRenderer.h"


HStaticMeshComponent::HStaticMeshComponent(FComponentInitArgs& InitArgs)
	: HActorComponent(InitArgs)
{
	m_MeshWorldCB.Create( L"[Static Mesh Component] World CB" );
}

HStaticMeshComponent::~HStaticMeshComponent()
{
	m_MeshWorldCB.Destroy();
}

void HStaticMeshComponent::Render(FCommandContext& GfxContext)
{
	Super::Render(GfxContext);

	if (!m_bIsDrawEnabled) return;

	if (m_MeshAsset.IsValid())
	{
		if (m_MaterialAsset.IsValid())
		{
			// Set the material information.
			m_MaterialAsset->Bind( GfxContext );
		}

		// Set the world buffer.
		m_MeshWorldCB->kWorldMat = m_Transform.GetWorldMatrix().Transpose();
		GfxContext.SetGraphicsConstantBuffer(kMeshWorld, m_MeshWorldCB);

		// TODO Request draw from model in model manager to render meshes of the same type in batches.
		// Render the geometry.
		GfxContext.SetPrimitiveTopologyType(PT_TiangleList);
		GfxContext.BindVertexBuffer(0, m_MeshAsset->GetVertexBuffer());
		GfxContext.BindIndexBuffer(m_MeshAsset->GetIndexBuffer());
		GfxContext.DrawIndexedInstanced(m_MeshAsset->GetNumIndices(), 1, 0, 0, 0);
	}
}

void HStaticMeshComponent::OnCreate() 
{
	Super::OnCreate();

	GetWorld()->GetScene().AddStaticMesh( this );
}

void HStaticMeshComponent::OnDestroy()
{
	Super::OnDestroy();

	GetWorld()->GetScene().RemoveStaticMesh( this );
}

void HStaticMeshComponent::OnAttach()
{
	Super::OnAttach();

}

void HStaticMeshComponent::Serialize( JsonUtility::WriteContext& Output )
{
	Output.Key( HE_STRINGIFY( HStaticMeshComponent::m_bIsDrawEnabled ) );
	Output.Bool( m_bIsDrawEnabled );

	Output.Key( HE_STRINGIFY( HStaticMeshComponent::m_MeshAsset ) );
	Output.String( m_MeshAsset->GetGuid().ToString().CStr() );

	Output.Key( HE_STRINGIFY( HStaticMeshComponent::m_MaterialAsset ) );
	Output.String( m_MaterialAsset->GetGuid().ToString().CStr() );

	Super::Serialize( Output );
}

void HStaticMeshComponent::Deserialize( const JsonUtility::ReadContext& Value ) 
{
	Super::Deserialize( Value );
	
	Char StringBuffer[64];
	ZeroMemory( StringBuffer, sizeof( StringBuffer ) );

	JsonUtility::GetBoolean( Value, HE_STRINGIFY( HStaticMeshComponent::m_bIsDrawEnabled ), m_bIsDrawEnabled);

	if(JsonUtility::GetString( Value, HE_STRINGIFY( HStaticMeshComponent::m_MeshAsset ), StringBuffer, sizeof( StringBuffer )))
		SetMesh(FAssetDatabase::GetStaticMesh( StringBuffer ));

	if(JsonUtility::GetString( Value, HE_STRINGIFY( HStaticMeshComponent::m_MaterialAsset ), StringBuffer, sizeof( StringBuffer )))
		SetMaterial( FAssetDatabase::GetMaterial( StringBuffer ) );
}
