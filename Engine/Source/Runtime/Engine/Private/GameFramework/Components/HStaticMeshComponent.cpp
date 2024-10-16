// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Components/HStaticMeshComponent.h"

#include "World/World.h"
#include "RendererCore.h"
#include "CommandContext.h"
#include "Renderer/ShaderRegisters.h"
#include "GameFramework/Actor/AActor.h"
#include "AssetRegistry/AssetDatabase.h"
#include "Renderer/ConstantBufferStructures.h"
#include "GameFramework/Actor/AActor.h"
#include "World/World.h"
#include "BatchRenderer.h"


HStaticMeshComponent::HStaticMeshComponent(FComponentInitArgs& InitArgs)
	: HSceneComponent(InitArgs)
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

	if (!GetIsDrawEnabled()) return;
	
	if (m_MaterialAsset.IsValid())
	{
		// Set the material information.
		m_MaterialAsset->Bind(GfxContext);
	}

	if (m_MeshAsset->IsValid())
	{
		// Set the world buffer.
		MeshWorldCBData* pWorld = m_MeshWorldCB.GetBufferPointer();
		pWorld->kWorldMat =  GetWorldMatrix().Transpose();
		m_MeshWorldCB.SetDirty(true);
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
	Output.Key(HE_STRINGIFY(HStaticMeshComponent));
	Output.StartArray();
	{
		// Outer properties.
		Output.StartObject();
		{
			Super::Serialize(Output);
		}
		Output.EndObject();

		// Static mesh properties.
		Output.StartObject();
		{
			Output.Key(HE_STRINGIFY(m_bIsDrawEnabled));
			Output.Bool(m_bIsDrawEnabled);

			Output.Key(HE_STRINGIFY(m_MeshAsset));
			Output.String(m_MeshAsset->GetGUID().ToString().CStr());

			Output.Key(HE_STRINGIFY(m_MaterialAsset));
			Output.String(m_MaterialAsset->GetGUID().ToString().CStr());
		}
		Output.EndObject();
	}
	Output.EndArray();
}

void HStaticMeshComponent::Deserialize( const JsonUtility::ReadContext& Value ) 
{
	Super::Deserialize( Value[0][HE_STRINGIFY(HSceneComponent)]);

	const rapidjson::Value& StaticMesh = Value[1];


	JsonUtility::GetBoolean(StaticMesh, HE_STRINGIFY(m_bIsDrawEnabled), m_bIsDrawEnabled);

	Char MeshGuidBuffer[64];
	ZeroMemory(MeshGuidBuffer, sizeof(MeshGuidBuffer));
	JsonUtility::GetString(StaticMesh, HE_STRINGIFY(m_MeshAsset), MeshGuidBuffer, sizeof(MeshGuidBuffer));
	FGUID MeshGuid = FGUID::CreateFromString(MeshGuidBuffer);
	SetMesh(FAssetDatabase::GetStaticMesh(MeshGuid));

	Char MaterialGuidBuffer[64];
	ZeroMemory(MaterialGuidBuffer, sizeof(MaterialGuidBuffer));
	JsonUtility::GetString(StaticMesh, HE_STRINGIFY(m_MaterialAsset), MaterialGuidBuffer, sizeof(MaterialGuidBuffer));
	FGUID MatAssetGuid = FGUID::CreateFromString(MaterialGuidBuffer);
	SetMaterial(FAssetDatabase::GetMaterial(MatAssetGuid));
}
