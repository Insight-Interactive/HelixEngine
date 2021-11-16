#pragma once

#include "GameFramework/Components/HActorComponent.h"

#include "Color.h"
#include "Transform.h"
#include "Renderer/LightManager.h"

#include "Renderer/MaterialManager.h"
#include "ModelManager.h"
#include "ConstantBuffer.h"


extern LightManager GLightManager;

/*
	A light that emits energy from a single point in space in all directions.
*/
HCOMPONENT()
class HPointLightComponent : public HActorComponent
{
	friend class AActor;
	using Super = HActorComponent;
public:
	HE_COMPONENT_GENERATED_BODY( HPointLightComponent )

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;

	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	FVector3 GetPosition() const;
	FColor GetColor() const;
	float GetBrightness() const;
	void SetPosition( const FVector3& NewPosition );
	void SetColor( const FColor& NewColor );
	void SetBrightness( float NewBrightness );

protected:
	virtual void Render( FCommandContext& GfxContext ) override;

	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

	bool GetCanDrawDebugBillboard() const;
	void SetCanDrawDebugBillboard(bool CanDraw);

protected:
	PointLightDataHandle m_PointLightHandle;
	FTransform m_Transform;

	FTransform m_BillboardTransform;
	StaticMeshGeometryRef m_LightDebugMesh;
	TConstantBuffer<MeshWorldCBData> m_MeshWorldCB;
	MaterialRef m_MaterialRef;
	bool m_CanDrawDebugBillboard;
};

//
// Inline function implementations
//

inline FVector3 HPointLightComponent::GetPosition() const
{
	return m_Transform.GetPosition();
}

inline FColor HPointLightComponent::GetColor() const
{
	FColor RetVal( 0.f, 0.f, 0.f, 0.f );
	PointLightCBData* pData = GLightManager.GetPointLightData( m_PointLightHandle );
	if (pData != NULL)
	{
		RetVal.R = pData->Color.x;
		RetVal.G = pData->Color.y;
		RetVal.B = pData->Color.z;
		RetVal.A = pData->Color.w;
	}
	return RetVal;
}

inline float HPointLightComponent::GetBrightness() const
{
	PointLightCBData* pData = GLightManager.GetPointLightData( m_PointLightHandle );
	if (pData != NULL)
	{
		return pData->Brightness;
	}

	return 0.f;
}

inline void HPointLightComponent::SetPosition( const FVector3& NewPosition )
{
	m_Transform.SetPosition( NewPosition );

	PointLightCBData* pData = GLightManager.GetPointLightData( m_PointLightHandle );
	if (pData != NULL)
	{
		pData->Position = NewPosition;
	}
}

inline void HPointLightComponent::SetColor( const FColor& NewColor )
{
	PointLightCBData* pData = GLightManager.GetPointLightData( m_PointLightHandle );
	if (pData != NULL)
	{
		pData->Color = NewColor.ToVector4();
	}
}

inline void HPointLightComponent::SetBrightness( float NewBrightness )
{
	PointLightCBData* pData = GLightManager.GetPointLightData( m_PointLightHandle );
	if (pData != NULL)
	{
		pData->Brightness = NewBrightness;
	}
}

inline bool HPointLightComponent::GetCanDrawDebugBillboard() const
{
	return m_CanDrawDebugBillboard;
}

inline void HPointLightComponent::SetCanDrawDebugBillboard( bool CanDraw )
{
	m_CanDrawDebugBillboard = CanDraw;
}
