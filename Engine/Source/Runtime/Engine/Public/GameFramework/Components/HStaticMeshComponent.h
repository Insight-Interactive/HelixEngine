#pragma once

#include "GameFramework/Components/HSceneComponent.h"

#include "Transform.h"
#include "ModelManager.h"
#include "ConstantBuffer.h"
#include "Renderer/MaterialManager.h"
#include "Renderer/ConstantBufferStructures.h"


HCOMPONENT()
class HStaticMeshComponent : public HSceneComponent
{
	friend class AActor;
	friend class HScene;
	using Super = HSceneComponent;
public:
	HE_COMPONENT_GENERATED_BODY( HStaticMeshComponent )


	void SetMesh( StaticMeshGeometryRef Mesh );
	void SetMaterial( MaterialRef Material );

	HMaterial& GetMaterial();
	bool GetIsDrawEnabled() const;
	void SetIsDrawEnabled(bool bVisible);
	bool IsOpaque() const;

protected:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnAttach() override;
	virtual void Render(FCommandContext& GfxContext) override;

	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

protected:
	HStaticMesh							m_MeshAsset;
	TConstantBuffer<MeshWorldCBData>	m_MeshWorldCB;
	HMaterial							m_MaterialAsset;
	bool								m_bIsDrawEnabled;

};


// 
// Inline function implementations
//

FORCEINLINE void HStaticMeshComponent::SetMesh( StaticMeshGeometryRef Mesh ) 
{ 
	m_MeshAsset = Mesh;
}

FORCEINLINE void HStaticMeshComponent::SetMaterial( MaterialRef Material ) 
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
