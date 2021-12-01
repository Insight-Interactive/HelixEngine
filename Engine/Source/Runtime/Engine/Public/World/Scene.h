// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/HObject.h"

#include "GameFramework/Components/HStaticMeshComponent.h"
#include "GameFramework/Components/HPointLightComponent.h"


class FCommandContext;
class HWorld;

class HScene : public HObject
{
	using Super = HObject;
	friend class FSceneRenderer;
public:
	HScene( HWorld* pOwner );
	virtual ~HScene();

	/*
		Sorts the scene opaque objects first transparent last.
		Turns: ottooto into: oooottt
	*/
	void SortStaticTransparentObjects();
	bool DoesContainAnyTranslucentObjects();

	void RenderStaticLitOpaqueObjects( FCommandContext& CmdContext );
	void RenderStaticTranslucentAndUnlitObjects( FCommandContext& CmdContext );

	void AddStaticMesh( HStaticMeshComponent* pStaticMesh );
	bool RemoveStaticMesh( HStaticMeshComponent* pStaticMesh );
	void AddPointLight(HPointLightComponent* pPointLight);
	bool RemovePointLight(HPointLightComponent* pPointLight);

protected:
	HWorld* GetWorld();

private:

private:
	std::vector<HStaticMeshComponent*> m_StaticMeshs;
	std::vector<HStaticMeshComponent*>::iterator m_FirstTranslucentOrUnlit;

	std::vector<HPointLightComponent*> m_PointLights;
	HWorld* m_pOwner;
};


//
// Inline function implementations
//

FORCEINLINE void HScene::AddStaticMesh( HStaticMeshComponent* pStaticMesh )
{
	m_StaticMeshs.push_back( pStaticMesh );
}

FORCEINLINE bool HScene::RemoveStaticMesh( HStaticMeshComponent* pStaticMesh )
{
	auto Iter = std::find( m_StaticMeshs.begin(), m_StaticMeshs.end(), pStaticMesh );
	if (Iter != m_StaticMeshs.end())
	{
		m_StaticMeshs.erase( Iter );
		return true;
	}
	return false;
}

FORCEINLINE void HScene::AddPointLight(HPointLightComponent* pPointLight)
{
	m_PointLights.push_back(pPointLight);
}

FORCEINLINE bool HScene::RemovePointLight(HPointLightComponent* pPointLight)
{
	auto Iter = std::find(m_PointLights.begin(), m_PointLights.end(), pPointLight);
	if (Iter != m_PointLights.end())
	{
		m_PointLights.erase(Iter);
		return true;
	}
	return false;
}

FORCEINLINE HWorld* HScene::GetWorld()
{
	return m_pOwner;
}

FORCEINLINE void HScene::SortStaticTransparentObjects()
{
	std::sort( m_StaticMeshs.begin(), m_StaticMeshs.end(),
		[]( HStaticMeshComponent* Mesh1, HStaticMeshComponent* Mesh2 )
		{
			return Mesh1->IsOpaque() > Mesh2->IsOpaque();
		} );

	// Get the first transparent object in the mesh list. This will be our 
	// starting point for the translucent pass.
	// TODO: This is O(n) will not scale well for a large number of meshes!
	m_FirstTranslucentOrUnlit = std::find_if(m_StaticMeshs.begin(), m_StaticMeshs.end(),
		[](HStaticMeshComponent* Mesh)
		{
			EShadingModel ShadingMode = Mesh->GetMaterial()->GetShadingModel();
			return ShadingMode == SM_Foliage || ShadingMode == SM_Unlit;
		});
}

FORCEINLINE bool HScene::DoesContainAnyTranslucentObjects()
{
	return m_FirstTranslucentOrUnlit != m_StaticMeshs.end();
}
