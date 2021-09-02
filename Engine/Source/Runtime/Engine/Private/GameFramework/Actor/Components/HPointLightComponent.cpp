#include "EnginePCH.h"

#include "GameFramework/Actor/Components/HPointLightComponent.h"


extern LightManager GLightManager;

HPointLightComponent::HPointLightComponent( const HName& Name )
	: HActorComponent( Name )
{
	m_Transform.SetPosition( 0.f, 5.f, 0.f );
	PointLightCBData* pNewLight = NULL;
	GLightManager.AllocatePointLightData( m_PointLightHandle, &pNewLight );
	if (pNewLight != NULL)
	{
		pNewLight->Brightness = 20.f;
		pNewLight->Color = FVector4::One;
		pNewLight->Position.x = m_Transform.GetPosition().x;
		pNewLight->Position.y = m_Transform.GetPosition().y;
		pNewLight->Position.z = m_Transform.GetPosition().z;
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
	PointLightCBData* pData = GLightManager.GetPointLightData( m_PointLightHandle );
	if (pData!= NULL)
	{
		static float WorldTime = 0.f;
		WorldTime += DeltaTime;
		float Offset = sinf( WorldTime );
		pData->Color.y = Offset;
	}
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
