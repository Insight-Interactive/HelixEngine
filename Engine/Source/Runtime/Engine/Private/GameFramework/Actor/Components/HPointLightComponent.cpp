#include "EnginePCH.h"

#include "GameFramework/Actor/Components/HPointLightComponent.h"

#include "Color.h"


HPointLightComponent::HPointLightComponent( const HName& Name )
	: HActorComponent( Name )
{
	PointLightCBData* pNewLight = NULL;
	GLightManager.AllocatePointLightData( m_PointLightHandle, &pNewLight );
	if (pNewLight != NULL)
	{
		pNewLight->Brightness = 20.f;
		pNewLight->Color = FVector4::One;
		pNewLight->Position = m_Transform.GetPosition();
	}
}

HPointLightComponent::~HPointLightComponent()
{
}

void HPointLightComponent::BeginPlay()
{
}

void HPointLightComponent::Tick( float DeltaTime )
{
}

void HPointLightComponent::OnCreate()
{
}

void HPointLightComponent::OnDestroy()
{
}

void HPointLightComponent::Render( ICommandContext& GfxContext )
{
}
