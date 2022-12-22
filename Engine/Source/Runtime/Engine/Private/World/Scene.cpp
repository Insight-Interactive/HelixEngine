// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "World/Scene.h"

#include "GameFramework/Components/HCameraComponent.h"
#include "CommandContext.h"
#include "World/World.h"


HScene::HScene( HWorld* pOwner )
	: Super( TEXT( "Scene" ) )
	, m_IsDirty( false )
	, m_RenderParams( )
	, m_pOwner( pOwner )
{

}

HScene::~HScene()
{

}

void HScene::RenderStaticLitOpaqueObjects(FCommandContext& CmdContext)
{
	CmdContext.BeginDebugMarker(L"Render Opaque Lit");
	{
		for (auto Iter = m_StaticMeshs.begin(); Iter != m_FirstTranslucentOrUnlit; ++Iter)
		{
			HStaticMeshComponent& StaticMesh = (**Iter);
			if (StaticMesh.GetMaterial()->GetShadingModel() == SM_DefaultLit)
				StaticMesh.Render(CmdContext);
		}
	}
	CmdContext.EndDebugMarker();
}

void HScene::RenderStaticTranslucentAndUnlitObjects(FCommandContext& CmdContext)
{
	CmdContext.BeginDebugMarker(L"Render Translucent Lit");
	{
		for (auto Iter = m_FirstTranslucentOrUnlit; Iter != m_StaticMeshs.end(); ++Iter)
		{
			HStaticMeshComponent& StaticMesh = (**Iter);

			EShadingModel ShadingMod = StaticMesh.GetMaterial()->GetShadingModel();
			StaticMesh.Render(CmdContext);
		}
	}
	CmdContext.EndDebugMarker();

	CmdContext.BeginDebugMarker(L"Render Lights");
	{
		for (size_t i = 0; i < m_PointLights.size(); ++i)
		{
			m_PointLights[i]->Render(CmdContext);
		}
	}
	CmdContext.EndDebugMarker();
}