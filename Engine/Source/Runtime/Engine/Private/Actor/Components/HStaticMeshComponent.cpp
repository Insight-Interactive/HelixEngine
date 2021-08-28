// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "Actor/Components/HStaticMeshComponent.h"

#include "RendererCore.h"
#include "IConstantBufferManager.h"
#include "StandaloneRenderer/ConstantBufferStructures.h"
#include "StandaloneRenderer/ShaderRegisters.h"
#include "ICommandContext.h"



HStaticMeshComponent::HStaticMeshComponent(const HName& Name)
	: HActorComponent(Name)
{
	GConstantBufferManager->CreateConstantBuffer(L"[Static Mesh Component] World CB", &m_pMeshWorldCB, sizeof(MeshWorldCBData));
}

HStaticMeshComponent::~HStaticMeshComponent()
{
}

void HStaticMeshComponent::Render(ICommandContext& GfxContext)
{
	if (!GetIsDrawEnabled()) return;

	if (m_MaterialRef.IsValid())
	{
		// Set the material information.
		m_MaterialRef->Bind(GfxContext);
	}

	if (m_GeometryRef.IsValid())
	{
		// Set the world buffer.
		MeshWorldCBData* pWorld = m_pMeshWorldCB->GetBufferPointer<MeshWorldCBData>();
		pWorld->WorldMat = m_Transform.GetWorldMatrix().Transpose();
		GfxContext.SetGraphicsConstantBuffer(kMeshWorld, m_pMeshWorldCB);

		// TODO Request draw from model in model manager.
		// Render the geometry.
		GfxContext.SetPrimitiveTopologyType(PT_TiangleList);
		GfxContext.BindVertexBuffer(0, m_GeometryRef->GetVertexBuffer());
		GfxContext.BindIndexBuffer(m_GeometryRef->GetIndexBuffer());
		GfxContext.DrawIndexedInstanced(m_GeometryRef->GetNumIndices(), 1, 0, 0, 0);
	}
}
