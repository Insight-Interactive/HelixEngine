#pragma once

#include "GameFramework/Actor/Components/HActorComponent.h"

#include "Transform.h"
#include "ModelManager.h"
#include "Renderer/MaterialManager.h"


class HStaticMeshComponent : public HActorComponent
{
public:
	HStaticMeshComponent(const HName& Name);
	virtual ~HStaticMeshComponent();
	

	inline void SetMesh(StaticMeshGeometryRef Mesh) { m_GeometryRef = Mesh; }
	inline void SetMaterial(MaterialRef Material) { m_MaterialRef = Material; }

	inline MaterialRef& GetMaterial() { return m_MaterialRef; }
	inline Transform& GetTransform() { return m_Transform; }
	inline bool GetIsDrawEnabled() const;
	inline void SetIsDrawEnabled(bool bVisible);

protected:
	virtual void Render(ICommandContext& GfxContext) override;


protected:
	StaticMeshGeometryRef m_GeometryRef;
	IConstantBuffer* m_pMeshWorldCB;
	MaterialRef m_MaterialRef;
	Transform m_Transform;
	bool m_bIsDrawEnabled;
};


// 
// Inline function implementations
//

inline bool HStaticMeshComponent::GetIsDrawEnabled() const
{
	return m_bIsDrawEnabled;
}

inline void HStaticMeshComponent::SetIsDrawEnabled(bool bEnabled)
{
	m_bIsDrawEnabled = bEnabled;
}
