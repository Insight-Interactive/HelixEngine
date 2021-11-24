// Copyright 2021 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Components/HPointLightComponent.h"

#include "GameFramework/Components/HCameraComponent.h"
#include "World/World.h"
#include "RendererCore.h"
#include "CommandContext.h"
#include "ConstantBuffer.h"
#include "Renderer/ShaderRegisters.h"
#include "Renderer/GeometryGenerator.h"
#include "Renderer/ConstantBufferStructures.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor/AActor.h"


HPointLightComponent::HPointLightComponent( const HName& Name )
	: HActorComponent( Name )
{
	PointLightCBData* pNewLight = NULL;
	GLightManager.AllocatePointLightData( m_PointLightHandle, &pNewLight );
	if (pNewLight != NULL)
	{
		pNewLight->Brightness = 2000.f;
		pNewLight->Color = FVector3::One * 255;
		pNewLight->Position = m_Transform.GetPosition();
	}

	m_CanDrawDebugBillboard = GEngine->GetIsEditorPresent();
	m_MeshWorldCB.Create( L"[PointLight Component] World CB" );
	m_MaterialRef = FAssetDatabase::GetInstance()->GetMaterial( FGUID::CreateFromString( "d7d2911a-0461-4a69-b28d-259389c45f87" ) );
	m_LightDebugMesh = FAssetDatabase::GetInstance()->GetStaticMesh(FGUID::CreateFromString( "4539421c-d8b4-4936-bb0c-8dde1e24f9b9" ));// GeometryGenerator::GenerateScreenAlignedQuadMesh();
	
}

HPointLightComponent::~HPointLightComponent()
{
	m_MeshWorldCB.Destroy();
}

void HPointLightComponent::BeginPlay()
{
	Super::BeginPlay();
	m_MaterialRef->SetFloat("kOpacity", 1.f);
}

void HPointLightComponent::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void HPointLightComponent::OnCreate()
{
	Super::OnCreate();

	GetWorld()->GetScene()->AddPointLight( this );
}

void HPointLightComponent::OnDestroy()
{
	Super::OnDestroy();

	GetWorld()->GetScene()->RemovePointLight( this );
}

void HPointLightComponent::Render( FCommandContext& GfxContext )
{
	Super::Render( GfxContext );
	// TODO Draw sphere showing light influence.

	// Draw billboard.
	if (m_LightDebugMesh->IsValid() /*&& GetCanDrawDebugBillboard()*/)
	{
		if (m_MaterialRef.IsValid())
		{
			// Set the material information.
			m_MaterialRef->Bind( GfxContext );
		}

		// Shrink/grow the card based on the distance to the camera.
		const float kBillboardMaxScale = 5.f;
		FTransform& CameraTransform = GetWorld()->GetCurrentSceneRenderCamera()->GetTransform();
		const float CameraDistance = 
			(CameraTransform.GetAbsoluteWorldPosition() - m_BillboardTransform.GetPosition()).Length() * 0.2f; // Scale factor relies on the raw distance, so a squre root is inevitable.
		const float ScaleFactor = HE_MIN( CameraDistance, kBillboardMaxScale );
		m_BillboardTransform.SetScale( FVector3( ScaleFactor ) );
		
		m_BillboardTransform.SetPosition( m_Transform.GetPosition() );
		m_BillboardTransform.LookAt( CameraTransform.GetAbsoluteWorldPosition() );

		// Set the world buffer.
		MeshWorldCBData* pWorld = m_MeshWorldCB.GetBufferPointer();
		pWorld->kWorldMat = m_BillboardTransform.GetWorldMatrix().Transpose();
		m_MeshWorldCB.SetDirty(true);
		GfxContext.SetGraphicsConstantBuffer( kMeshWorld, m_MeshWorldCB );

		// TODO Request draw from model in model manager to render meshes of the same type in batches.
		// Render the geometry.
		GfxContext.SetPrimitiveTopologyType( PT_TiangleList );
		GfxContext.BindVertexBuffer( 0, m_LightDebugMesh->GetVertexBuffer() );
		GfxContext.BindIndexBuffer( m_LightDebugMesh->GetIndexBuffer() );
		GfxContext.DrawIndexedInstanced( m_LightDebugMesh->GetNumIndices(), 1, 0, 0, 0 );
	}
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