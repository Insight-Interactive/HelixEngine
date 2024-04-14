#pragma once

#include "GameFramework/Components/HSceneComponent.h"

#include "Renderer/MaterialManager.h"


class HRenderableComponenetInterface : public HSceneComponent
{
	friend class AActor;
	friend class HScene;
	using Super = HSceneComponent;
public:
	bool GetIsDrawEnabled() const;
	void SetIsDrawEnabled( bool bVisible );

	void SetMaterial( HMaterial Material );

	HMaterial& GetMaterial();
	bool IsOpaque() const;
protected:
	HE_COMPONENT_GENERATED_BODY( HRenderableComponenetInterface );

	void OnCreate();
	void OnDestroy();
	void Render( FCommandContext& GfxContext );

protected:
	bool m_bIsDrawEnabled;
	HMaterial							m_MaterialAsset;

};


// 
// Inline function implementations
//

FORCEINLINE bool HRenderableComponenetInterface::GetIsDrawEnabled() const
{
	return m_bIsDrawEnabled;
}

FORCEINLINE void HRenderableComponenetInterface::SetIsDrawEnabled( bool bEnabled )
{
	m_bIsDrawEnabled = bEnabled;
}

FORCEINLINE void HRenderableComponenetInterface::SetMaterial( HMaterial Material )
{
	m_MaterialAsset = Material;
}

FORCEINLINE HMaterial& HRenderableComponenetInterface::GetMaterial()
{
	return m_MaterialAsset;
}

FORCEINLINE bool HRenderableComponenetInterface::IsOpaque() const
{
	return m_MaterialAsset->GetShadingModel() != EShadingModel::SM_Foliage;
}