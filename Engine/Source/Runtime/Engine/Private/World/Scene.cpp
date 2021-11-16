#include "EnginePCH.h"

#include "World/Scene.h"

#include "GameFramework/Components/HCameraComponent.h"
#include "CommandContext.h"
#include "World/World.h"


HScene::HScene( HWorld* pOwner )
	: Super(TEXT("Scene"))
	, m_pOwner( pOwner )
{

}

HScene::~HScene()
{

}

void HScene::RenderStaticUnlitOpaqueOjects(FCommandContext& CmdContext)
{
	CmdContext.BeginDebugMarker(L"Render Opaque Unlit");
	{
		for (auto Iter = m_StaticMeshs.begin(); Iter != m_FirstTranslucent; ++Iter)
		{
			HStaticMeshComponent& StaticMesh = (**Iter);

			if (StaticMesh.GetMaterial()->GetShadingModel() == SM_Unlit)
				StaticMesh.Render(CmdContext);
		}
	}
	CmdContext.EndDebugMarker();
}

void HScene::RenderStaticUnlitTranslucentObjects(FCommandContext& CmdContext)
{
	CmdContext.BeginDebugMarker(L"Render Translucent Unlit");
	{
		for (auto Iter = m_FirstTranslucent; Iter != m_StaticMeshs.end(); ++Iter)
		{
			HStaticMeshComponent& StaticMesh = (**Iter);

			if (StaticMesh.GetMaterial()->GetShadingModel() == SM_Unlit)
				StaticMesh.Render(CmdContext);
		}
	}
	CmdContext.EndDebugMarker();
}

void HScene::RenderStaticLitOpaqueObjects(FCommandContext& CmdContext)
{
	CmdContext.BeginDebugMarker(L"Render Opaque Lit");
	{
		for (auto Iter = m_StaticMeshs.begin(); Iter != m_FirstTranslucent; ++Iter)
		{
			HStaticMeshComponent& StaticMesh = (**Iter);
			if (StaticMesh.GetMaterial()->GetShadingModel() == SM_DefaultLit)
				StaticMesh.Render(CmdContext);
		}
	}
	CmdContext.EndDebugMarker();
}

void HScene::RenderStaticLitTranslucentObjects(FCommandContext& CmdContext)
{
	CmdContext.BeginDebugMarker(L"Render Translucent Lit");
	{
		for (auto Iter = m_FirstTranslucent; Iter != m_StaticMeshs.end(); ++Iter)
		{
			HStaticMeshComponent& StaticMesh = (**Iter);

			EShadingModel ShadingMod = StaticMesh.GetMaterial()->GetShadingModel();
			if (ShadingMod == SM_DefaultLit || ShadingMod == SM_Foliage)
				StaticMesh.Render(CmdContext);
		}
	}
	CmdContext.EndDebugMarker();
}