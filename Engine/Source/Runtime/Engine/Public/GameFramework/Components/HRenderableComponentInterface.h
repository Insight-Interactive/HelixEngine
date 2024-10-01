#pragma once

#include "GameFramework/Components/HActorComponent.h"

#include "Graphics/MaterialManager.h"


class HRenderableComponentInterface : public HActorComponent
{
	friend class AActor;
	friend class HScene;
	using Super = HActorComponent;
public:
	bool GetIsDrawEnabled() const;
	void SetIsDrawEnabled( bool bVisible );

	void SetMaterial( HMaterial Material );

	HMaterial& GetMaterial();
	bool IsOpaque() const;
protected:
	HE_COMPONENT_GENERATED_BODY( HRenderableComponentInterface );

protected:
	bool		m_bIsDrawEnabled;
	HMaterial	m_MaterialAsset;

};


// 
// Inline function implementations
//

FORCEINLINE bool HRenderableComponentInterface::GetIsDrawEnabled() const
{
	return m_bIsDrawEnabled;
}

FORCEINLINE void HRenderableComponentInterface::SetIsDrawEnabled( bool bEnabled )
{
	m_bIsDrawEnabled = bEnabled;
}

FORCEINLINE void HRenderableComponentInterface::SetMaterial( HMaterial Material )
{
	m_MaterialAsset = Material;
}

FORCEINLINE HMaterial& HRenderableComponentInterface::GetMaterial()
{
	return m_MaterialAsset;
}

FORCEINLINE bool HRenderableComponentInterface::IsOpaque() const
{
	if (!m_MaterialAsset.IsValid())
		return false;

	return m_MaterialAsset->GetShadingModel() != EShadingModel::SM_Foliage;
}