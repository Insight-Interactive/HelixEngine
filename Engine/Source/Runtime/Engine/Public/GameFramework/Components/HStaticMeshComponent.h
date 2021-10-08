#pragma once

#include "GameFramework/Components/HActorComponent.h"

#include "Transform.h"
#include "ModelManager.h"
#include "Renderer/MaterialManager.h"


HCOMPONENT()
class HStaticMeshComponent : public HActorComponent
{
	friend class AActor;
	using Super = HActorComponent;
public:
	HStaticMeshComponent(const HName& Name);
	virtual ~HStaticMeshComponent();
	

	inline void SetMesh(StaticMeshGeometryRef Mesh) { m_GeometryRef = Mesh; }
	inline void SetMaterial(MaterialRef Material) { m_MaterialRef = Material; }

	inline MaterialRef& GetMaterial() { return m_MaterialRef; }
	inline FTransform& GetTransform() { return m_Transform; }
	inline bool GetIsDrawEnabled() const;
	inline void SetIsDrawEnabled(bool bVisible);

protected:
	virtual void OnAttach() override;
	virtual void Render(FCommandContext& GfxContext) override;

	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

protected:
	StaticMeshGeometryRef m_GeometryRef;
	FConstantBuffer* m_pMeshWorldCB;
	MaterialRef m_MaterialRef;
	FTransform m_Transform;
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
