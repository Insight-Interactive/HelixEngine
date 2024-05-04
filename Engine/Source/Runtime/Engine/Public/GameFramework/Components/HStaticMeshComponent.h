// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Components/HRenderableComponentInterface.h"

#include "Transform.h"
#include "ModelManager.h"
#include "ConstantBuffer.h"
#include "Renderer/MaterialManager.h"
#include "Renderer/ConstantBufferStructures.h"


HCOMPONENT()
class HStaticMeshComponent : public HRenderableComponenetInterface
{
	friend class AActor;
	friend class HScene;
	using Super = HRenderableComponenetInterface;
public:
	HE_COMPONENT_GENERATED_BODY( HStaticMeshComponent )


	void SetMesh( StaticMeshGeometryRef Mesh );

protected:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnAttach() override;
	virtual void Render(FCommandContext& GfxContext) override;

	virtual void Serialize( JsonUtility::WriteContext& Output ) override;
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;

protected:
	HStaticMesh							m_MeshAsset;
	TConstantBuffer<MeshWorldCBData>	m_MeshWorldCB;

};


// 
// Inline function implementations
//

FORCEINLINE void HStaticMeshComponent::SetMesh( StaticMeshGeometryRef Mesh ) 
{ 
	m_MeshAsset = Mesh;
}
