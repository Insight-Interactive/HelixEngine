#pragma once

#include "GameFramework/Components/HActorComponent.h"

#include "Color.h"
#include "Transform.h"
#include "Renderer/LightManager.h"


extern LightManager GLightManager;

HCOMPONENT()
class HPointLightComponent : public HActorComponent
{
	friend class AActor;
	using Super = HActorComponent;
public:
	HPointLightComponent( const HName& Name );
	virtual ~HPointLightComponent();

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;

	virtual void OnCreate() override;
	virtual void OnDestroy() override;

	FVector3 GetPosition() const;
	Color GetColor() const;
	float GetBrightness() const;
	void SetPosition( const FVector3& NewPosition );
	void SetColor( const Color& NewColor );
	void SetBrightness( float NewBrightness );

protected:
	virtual void Render( ICommandContext& GfxContext ) override;

	virtual void Serialize( WriteContext& Output ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

protected:
	PointLightDataHandle m_PointLightHandle;
	FTransform m_Transform;

};

//
// Inline function implementations
//

inline FVector3 HPointLightComponent::GetPosition() const
{
	return m_Transform.GetPosition();
}

inline Color HPointLightComponent::GetColor() const
{
	Color RetVal( 0.f, 0.f, 0.f, 0.f );
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

inline void HPointLightComponent::SetColor( const Color& NewColor )
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