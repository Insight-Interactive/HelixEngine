// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Components/HActorComponent.h"

#include "Transform.h"
#include "Graphics/MaterialManager.h"
#include "Engine/Renderer/ModelManager.h"
#include "Engine/Renderer/ConstantBuffer.h"
#include "Graphics/ConstantBufferStructures.h"


HCOMPONENT()
class HStaticMeshComponent : public HActorComponent
{
	friend class AActor;	
	friend class HScene;
	using Super = HActorComponent;
public:
	HE_COMPONENT_GENERATED_BODY( HStaticMeshComponent )


	void SetMesh( HStaticMesh Mesh );
	void SetMaterial( HMaterial Material );

	HMaterial& GetMaterial();
	bool GetIsDrawEnabled() const;
	void SetIsDrawEnabled(bool bVisible);
	bool IsOpaque() const;

	FTransform& GetTransform() { return m_Transform; }

protected:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnAttach() override;
	virtual void Render(FCommandContext& GfxContext) override;

	virtual void Serialize( JsonUtility::WriteContext& Output ) override;
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;

protected:
	FTransform m_Transform;
	bool								m_bIsDrawEnabled;
	
	TConstantBuffer<MeshWorldCBData>	m_MeshWorldCB;
	HStaticMesh							m_MeshAsset;
	
	HMaterial							m_MaterialAsset;

};


// 
// Inline function implementations
//

FORCEINLINE void HStaticMeshComponent::SetMesh( HStaticMesh Mesh )
{ 
	m_MeshAsset = Mesh;
}

FORCEINLINE void HStaticMeshComponent::SetMaterial( HMaterial Material )
{ 
	m_MaterialAsset = Material;
}

FORCEINLINE HMaterial& HStaticMeshComponent::GetMaterial()
{ 
	return m_MaterialAsset;
}

FORCEINLINE bool HStaticMeshComponent::IsOpaque() const
{
	return m_MaterialAsset->GetShadingModel() != EShadingModel::SM_Foliage;
}

FORCEINLINE  bool HStaticMeshComponent::GetIsDrawEnabled() const
{
	return m_bIsDrawEnabled;
}

FORCEINLINE void HStaticMeshComponent::SetIsDrawEnabled(bool bEnabled)
{
	m_bIsDrawEnabled = bEnabled;
}
