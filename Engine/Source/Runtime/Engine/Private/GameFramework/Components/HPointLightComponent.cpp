#include "EnginePCH.h"

#include "GameFramework/Components/HPointLightComponent.h"

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

void HPointLightComponent::Render( FCommandContext& GfxContext )
{
}

void HPointLightComponent::Serialize( WriteContext& Output )
{
	Super::Serialize( Output );

}

void HPointLightComponent::Deserialize( const ReadContext& Value )
{
	Super::Deserialize( Value[0] );

	const rapidjson::Value& LightComponent = Value[1];

	const rapidjson::Value& LocalTransform = LightComponent["LocalTransform"][0];
	FVector3 Position, Rotation, Scale;
	// Position
	JsonUtility::GetFloat( LocalTransform, "PositionX", Position.x );
	JsonUtility::GetFloat( LocalTransform, "PositionY", Position.y );
	JsonUtility::GetFloat( LocalTransform, "PositionZ", Position.z );
	// Rotation
	JsonUtility::GetFloat( LocalTransform, "RotationX", Rotation.x );
	JsonUtility::GetFloat( LocalTransform, "RotationY", Rotation.y );
	JsonUtility::GetFloat( LocalTransform, "RotationZ", Rotation.z );
	// Scale
	JsonUtility::GetFloat( LocalTransform, "ScaleX", Scale.x );
	JsonUtility::GetFloat( LocalTransform, "ScaleY", Scale.y );
	JsonUtility::GetFloat( LocalTransform, "ScaleZ", Scale.z );
	SetPosition( Position );

	// Color
	const rapidjson::Value& Color = LightComponent["Color"][0];
	FColor LightColor;
	JsonUtility::GetFloat( Color, "R", LightColor.R );
	JsonUtility::GetFloat( Color, "G", LightColor.G );
	JsonUtility::GetFloat( Color, "B", LightColor.B );
	SetColor( LightColor );

	// Brightness
	float Brightness = 0.f;
	JsonUtility::GetFloat( LightComponent, "Brightness", Brightness );
	SetBrightness( Brightness );
}