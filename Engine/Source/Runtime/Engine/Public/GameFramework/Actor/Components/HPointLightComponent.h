#pragma once

#include "GameFramework/Actor/Components/HActorComponent.h"

#include "Transform.h"
#include "Renderer/LightManager.h"


class HPointLightComponent : public HActorComponent
{
public:
	HPointLightComponent( const HName& Name );
	virtual ~HPointLightComponent();

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;

	virtual void OnCreate() override;
	virtual void OnDestroy() override;

protected:
	virtual void Render( ICommandContext& GfxContext ) override;

protected:
	PointLightDataHandle m_PointLightHandle;
	Transform m_Transform;

};
