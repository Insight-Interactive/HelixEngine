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


HPointLightComponent::HPointLightComponent( const HName& Name )
	: HActorComponent( Name )
{
	m_CanDrawDebugBillboard = GEngine->GetIsEditorPresent();
	PointLightCBData* pNewLight = NULL;
	GLightManager.AllocatePointLightData( m_PointLightHandle, &pNewLight );
	if (pNewLight != NULL)
	{
		pNewLight->Brightness = 20.f;
		pNewLight->Color = FVector4::One;
		pNewLight->Position = m_Transform.GetPosition();
	}

	m_MeshWorldCB.Create( L"[PointLight Component] World CB" );
	//m_MaterialRef = FAssetDatabase::GetInstance()->GetMaterial( /*"M_RustedMetal"*/FGUID::CreateFromString( "0d68e992-aa25-4aa4-9f81-0eb775320c1e" ) );
	//m_LightDebugMesh = GeometryGenerator::GenerateScreenAlignedQuadMesh();
}

HPointLightComponent::~HPointLightComponent()
{
	m_MeshWorldCB.Destroy();
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
}

void HPointLightComponent::OnDestroy()
{
	Super::OnDestroy();
}

void HPointLightComponent::Render( FCommandContext& GfxContext )
{
	Super::Render( GfxContext );

	// TODO Draw sphere showing light influence.

	// Draw billboard.
	//if (m_LightDebugMesh->IsValid() && GetCanDrawDebugBillboard())
	//{
	//	if (m_MaterialRef.IsValid())
	//	{
	//		// Set the material information.
	//		m_MaterialRef->Bind( GfxContext );
	//	}

	//	// Update the billboard transform.
	//	// Shrink/grow the card based on the distance to the camera.
	//	const float kBillboardMaxScale = 5.f;
	//	FTransform& CameraTransform = GetWorld()->GetCurrentSceneRenderCamera()->GetTransform();
	//	const float CameraDistance = 
	//		(CameraTransform.GetAbsoluteWorldPosition() - m_BillboardTransform.GetPosition()).Length() * 0.2f; // Scale factor relies on the raw distance, so a squre root is inevitable.
	//	const float ScaleFactor = HE_MIN( CameraDistance, kBillboardMaxScale );
	//	m_BillboardTransform.SetScale( FVector3( ScaleFactor ) );
	//	
	//	m_BillboardTransform.SetPosition( m_Transform.GetPosition() );
	//	m_BillboardTransform.LookAt( CameraTransform.GetAbsoluteWorldPosition() );

	//	// Set the world buffer.
	//	MeshWorldCBData* pWorld = m_pMeshWorldCB->GetBufferPointer<MeshWorldCBData>();
	//	pWorld->WorldMat = m_BillboardTransform.GetWorldMatrix().Transpose();
	//	GfxContext.SetGraphicsConstantBuffer( kMeshWorld, m_pMeshWorldCB );

	//	// TODO Request draw from model in model manager to render meshes of the same type in batches.
	//	// Render the geometry.
	//	GfxContext.SetPrimitiveTopologyType( PT_TiangleList );
	//	GfxContext.BindVertexBuffer( 0, m_LightDebugMesh->GetVertexBuffer() );
	//	GfxContext.BindIndexBuffer( m_LightDebugMesh->GetIndexBuffer() );
	//	GfxContext.DrawIndexedInstanced( m_LightDebugMesh->GetNumIndices(), 1, 0, 0, 0 );
	//}
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