// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "GameFramework/Components/HPointLightComponent.h"

#include "World/World.h"
#include "Engine/Engine.h"
#include "CommandContext.h"
#include "ConstantBuffer.h"
#include "Renderer/ShaderRegisters.h"
#include "Renderer/GeometryGenerator.h"
#include "GameFramework/Components/HCameraComponent.h"


HPointLightComponent::HPointLightComponent( FComponentInitArgs& InitArgs )
	: HSceneComponent( InitArgs )
{
	GLightManager.AllocatePointLightData( m_PointLightHandle, nullptr );

	if (GetCanDrawDebugBillboard())
	{
		m_MeshWorldCB.Create( L"[PointLight Component] World CB" );
		m_MaterialRef = FAssetDatabase::GetMaterial( "M_RustedMetal.hmat" ); // TODO Make a material for this
		m_LightDebugMesh = FAssetDatabase::GetStaticMesh( "Cube.fbx" );// GeometryGenerator::GenerateScreenAlignedQuadMesh();
	}

	//FMaterialInstance matinst;
	//matinst.LoadFromFile( String(FGameProject::GetInstance()->GetContentFolder() + "/Materials/MI_Wood.hmatinst").c_str() );
}

HPointLightComponent::~HPointLightComponent()
{
	m_MeshWorldCB.Destroy();
}

void HPointLightComponent::SetPosition( const FVector3& NewPos )
{
	Super::SetPosition( NewPos );
	
	PointLightData* pData = GLightManager.GetPointLightData( m_PointLightHandle );
	if (pData != nullptr)
	{
		pData->Position = GetWorldPosition();
	}
}

void HPointLightComponent::SetPosition( const float& X, const float& Y, const float& Z )
{
	Super::SetPosition( X, Y, Z );

	PointLightData* pData = GLightManager.GetPointLightData( m_PointLightHandle );
	if (pData != nullptr)
	{
		pData->Position = GetWorldPosition();
	}
}

void HPointLightComponent::BeginPlay()
{
	Super::BeginPlay();
}

void HPointLightComponent::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void HPointLightComponent::OnCreate()
{
	Super::OnCreate();

	GetWorld()->GetScene().AddPointLight( this );
}

void HPointLightComponent::OnDestroy()
{
	Super::OnDestroy();

	GetWorld()->GetScene().RemovePointLight( this );
}

void HPointLightComponent::Render( FCommandContext& GfxContext )
{
	Super::Render( GfxContext );
	// TODO Draw sphere showing light influence.

	// Draw billboard.
	if (m_LightDebugMesh && m_LightDebugMesh->IsValid() && GetCanDrawDebugBillboard())
	{
		if (m_MaterialRef.IsValid())
		{
			// Set the material information.
			m_MaterialRef->Bind( GfxContext );
		}

		// Shrink/grow the card based on the distance to the camera.
		const float kBillboardMaxScale = 5.f;
		HCameraComponent* Camera = GetWorld()->GetCurrentSceneRenderCamera();
		const float CameraDistance =
			(Camera->GetWorldPosition() - m_BillboardTransform.GetPosition()).Length() * 0.2f; // Scale factor relies on the raw distance, so a square root is inevitable.
		const float ScaleFactor = HE_MIN( CameraDistance, kBillboardMaxScale );
		m_BillboardTransform.SetScale( FVector3( ScaleFactor ) );

		m_BillboardTransform.SetPosition( GetWorldPosition() );
		m_BillboardTransform.LookAt( Camera->GetWorldPosition() );

		// Set the world buffer.
		m_MeshWorldCB->kWorldMat = m_BillboardTransform.GetLocalMatrix().Transpose();
		m_MeshWorldCB.SetDirty( true );
		GfxContext.SetGraphicsConstantBuffer( kMeshWorld, m_MeshWorldCB );

		// TODO Request draw from model in model manager to render meshes of the same type in batches.
		// Render the geometry.
		GfxContext.SetPrimitiveTopologyType( PT_TiangleList );
		GfxContext.BindVertexBuffer( 0, m_LightDebugMesh->GetVertexBuffer() );
		GfxContext.BindIndexBuffer( m_LightDebugMesh->GetIndexBuffer() );
		GfxContext.DrawIndexedInstanced( m_LightDebugMesh->GetNumIndices(), 1, 0, 0, 0 );
	}
}

bool HPointLightComponent::GetCanDrawDebugBillboard() const
{
	return !GEngine->IsPlayingInEditor();
}

void HPointLightComponent::Serialize( JsonUtility::WriteContext& Output )
{
	Super::Serialize( Output );

}

void HPointLightComponent::Deserialize( const JsonUtility::ReadContext& Value )
{
	Super::Deserialize( Value );

	// Color
	const rapidjson::Value& Color = Value["Color"][0];
	FColor LightColor;
	JsonUtility::GetFloat( Color, "R", LightColor.R );
	JsonUtility::GetFloat( Color, "G", LightColor.G );
	JsonUtility::GetFloat( Color, "B", LightColor.B );
	SetColor( LightColor );

	// Brightness
	float Brightness = 0.f;
	JsonUtility::GetFloat( Value, "Brightness", Brightness );
	SetBrightness( Brightness );
}