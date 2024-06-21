// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/Components/HSceneComponent.h"

#include "Color.h"
#include "Transform.h"
#include "ModelManager.h"
#include "ConstantBuffer.h"
#include "Renderer/LightManager.h"
#include "Renderer/MaterialManager.h"


extern LightManager GLightManager;

/*
	A light that emits energy from a single point in space in all directions.
*/
HCOMPONENT()
class HPointLightComponent : public HSceneComponent
{
	friend class AActor;
	friend class HScene;
	using Super = HSceneComponent;
public:
	HE_COMPONENT_GENERATED_BODY( HPointLightComponent )

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;

	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	FColor GetColor() const;
	float GetBrightness() const;
	void SetColor( const FColor& NewColor );
	void SetBrightness( float NewBrightness );

	virtual void SetPosition( const FVector3& NewPos );
	virtual void SetPosition( const float& X, const float& Y, const float& Z );

protected:
	virtual void Render( FCommandContext& GfxContext ) override;

	virtual void Serialize( JsonUtility::WriteContext& Output ) override;
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;

	bool GetCanDrawDebugBillboard() const;

protected:
	PointLightDataHandle m_PointLightHandle;

	// Billboard info
	FTransform m_BillboardTransform;
	StaticMeshGeometryRef m_LightDebugMesh;
	TConstantBuffer<MeshWorldCBData> m_MeshWorldCB;
	HMaterial m_MaterialRef;

};


//
// Inline function implementations
//

inline FColor HPointLightComponent::GetColor() const
{
	FColor RetVal( 0.f );
	PointLightData* pData = GLightManager.GetPointLightData( m_PointLightHandle );
	if (pData != nullptr)
	{
		RetVal.R = pData->Color.x;
		RetVal.G = pData->Color.y;
		RetVal.B = pData->Color.z;
	}
	return RetVal;
}

inline float HPointLightComponent::GetBrightness() const
{
	PointLightData* pData = GLightManager.GetPointLightData( m_PointLightHandle );
	if (pData != nullptr)
	{
		return pData->Brightness;
	}

	return 0.f;
}

inline void HPointLightComponent::SetColor( const FColor& NewColor )
{
	PointLightData* pData = GLightManager.GetPointLightData( m_PointLightHandle );
	if (pData != nullptr)
	{
		pData->Color = NewColor.ToVector3();
	}
}

inline void HPointLightComponent::SetBrightness( float NewBrightness )
{
	PointLightData* pData = GLightManager.GetPointLightData( m_PointLightHandle );
	if (pData != nullptr)
	{
		pData->Brightness = NewBrightness;
	}
}
