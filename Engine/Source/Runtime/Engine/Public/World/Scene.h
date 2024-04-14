// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/HObject.h"

#include "GameFramework/Components/HStaticMeshComponent.h"
#include "GameFramework/Components/HSkeletalMeshComponenet.h"
#include "GameFramework/Components/HPointLightComponent.h"
#include "GameFramework/Components/HColliderComponent.h"


class FCommandContext;
class FSceneRenderer;
class HWorld;

class HScene : public HObject
{
	using Super = HObject;
	friend class FRenderingSubsystem;
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
	void RenderSkeletalLitOpaqueObjects( FCommandContext& CmdContext );
	void RenderDebugMeshes( FCommandContext& CmdContext );
	void RenderStaticTranslucentAndUnlitObjects( FCommandContext& CmdContext );

	void AddStaticMesh( HStaticMeshComponent* pStaticMesh );
	void AddSkeletalMesh( HSkeletalMeshComponent* pSkeletalMesh );
	bool RemoveStaticMesh( HStaticMeshComponent* pStaticMesh );
	bool RemoveSkeletalMesh( HSkeletalMeshComponent* pSkeletalMesh );
	void AddDebugCollider( HColliderComponent* pCollider );
	bool RemoveDebugCollider( HColliderComponent* pCollider );
	void AddPointLight(HPointLightComponent* pPointLight);
	bool RemovePointLight(HPointLightComponent* pPointLight);

	bool GetDrawColliders() const;
	void SetDrawColliders( const bool& DrawColliders );

protected:
	HWorld* GetWorld();

private:
	bool m_DrawColliders;

	/*std::vector<HStaticMeshComponent*> m_StaticMeshs;
	std::vector<HSkeletalMeshComponent*> m_SkeletalMeshes;*/
	std::vector<HRenderableComponenetInterface*> m_Renderables;

	std::vector<HColliderComponent*> m_DebugColliderMeshs;
	std::vector<HRenderableComponenetInterface*>::iterator m_FirstTranslucentOrUnlit;

	std::vector<HPointLightComponent*> m_PointLights;
	HWorld* m_pOwner;

};


//
// Inline function implementations
//

FORCEINLINE void HScene::AddStaticMesh( HStaticMeshComponent* pStaticMesh )
{
	HE_ASSERT( pStaticMesh != nullptr );

	m_Renderables.push_back( pStaticMesh );
	//m_StaticMeshs.push_back( pStaticMesh );
}

FORCEINLINE void HScene::AddSkeletalMesh( HSkeletalMeshComponent* pSkeletalMesh )
{
	HE_ASSERT( pSkeletalMesh != nullptr );

	m_Renderables.push_back( pSkeletalMesh );
	//m_SkeletalMeshes.push_back( pSkeletalMesh );
}

FORCEINLINE bool HScene::RemoveStaticMesh( HStaticMeshComponent* pStaticMesh )
{
	HE_ASSERT( pStaticMesh != nullptr );
	
	auto Iter = std::find( m_Renderables.begin(), m_Renderables.end(), pStaticMesh );
	if (Iter != m_Renderables.end())
	{
		m_Renderables.erase( Iter );
		return true;
	}
	/*auto Iter = std::find( m_StaticMeshs.begin(), m_StaticMeshs.end(), pStaticMesh );
	if (Iter != m_StaticMeshs.end())
	{
		m_StaticMeshs.erase( Iter );
		return true;
	}*/
	return false;
}

FORCEINLINE bool HScene::RemoveSkeletalMesh( HSkeletalMeshComponent* pSkeletalMesh )
{
	HE_ASSERT( pSkeletalMesh != nullptr );

	auto Iter = std::find( m_Renderables.begin(), m_Renderables.end(), pSkeletalMesh );
	if (Iter != m_Renderables.end())
	{
		m_Renderables.erase( Iter );
		return true;
	}
	/*auto Iter = std::find( m_SkeletalMeshes.begin(), m_SkeletalMeshes.end(), pSkeletalMesh );
	if (Iter != m_SkeletalMeshes.end())
	{
		m_SkeletalMeshes.erase( Iter );
		return true;
	}*/
	return false;
}

FORCEINLINE void HScene::AddDebugCollider( HColliderComponent* pCollider )
{
	HE_ASSERT( pCollider != nullptr );
	
	m_DebugColliderMeshs.push_back( pCollider );
}

FORCEINLINE bool HScene::RemoveDebugCollider( HColliderComponent* pCollider )
{
	HE_ASSERT( pCollider != nullptr );

	auto Iter = std::find( m_DebugColliderMeshs.begin(), m_DebugColliderMeshs.end(), pCollider );
	if (Iter != m_DebugColliderMeshs.end())
	{
		m_DebugColliderMeshs.erase( Iter );
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
	std::sort( m_Renderables.begin(), m_Renderables.end(),
		[]( HRenderableComponenetInterface* Mesh1, HRenderableComponenetInterface* Mesh2 )
		{
			return Mesh1->IsOpaque() > Mesh2->IsOpaque();
		} );

	// Get the first transparent object in the mesh list. This will be our 
	// starting point for the translucent pass.
	// TODO: This is O(n) will not scale well for a large number of meshes!
	m_FirstTranslucentOrUnlit = std::find_if( m_Renderables.begin(), m_Renderables.end(),
		[]( HRenderableComponenetInterface* Mesh)
		{
			EShadingModel ShadingMode = Mesh->GetMaterial()->GetShadingModel();
			return ShadingMode == SM_Foliage || ShadingMode == SM_Unlit;
		});
}

FORCEINLINE bool HScene::DoesContainAnyTranslucentObjects()
{
	return m_FirstTranslucentOrUnlit != m_Renderables.end();
	//return m_FirstTranslucentOrUnlit != m_StaticMeshs.end();
}

FORCEINLINE bool HScene::GetDrawColliders() const
{
	return m_DrawColliders;
}

FORCEINLINE void HScene::SetDrawColliders( const bool& DrawColliders )
{
	m_DrawColliders = DrawColliders;
}
