#include "EnginePCH.h"

#include "GameFramework/Components/HColliderComponent.h"

#include "World/World.h"
#include "PhysicsScene.h"
#include "CommandContext.h"
#include "Renderer/ConstantBufferStructures.h"
#include "Renderer/ShaderRegisters.h"
#include "GameFramework/Components/HCameraComponent.h"


HColliderComponent::HColliderComponent( FComponentInitArgs& InitArgs )
	: HSceneComponent( InitArgs )
	, m_IsTrigger( false )
	, m_CollisionBoundsDrawEnabled( true )
	, m_Material( nullptr )
{
	m_MeshWorldCB.Create( L"[Collider Component] World CB" );
}

HColliderComponent::~HColliderComponent()
{
	m_MeshWorldCB.Destroy();
	delete m_Material;
}

void HColliderComponent::BeginPlay() 
{
	Super::BeginPlay();
}

void HColliderComponent::Tick( float DeltaTime ) 
{
	Super::Tick( DeltaTime );
	
	// Fetch the results of the simulation.
	SetPosition( GetRigidBody().GetSimulatedPosition() );
	SetRotation( GetRigidBody().GetSimulatedRotation() );
}

void HColliderComponent::Render( FCommandContext& GfxContext )
{
	Super::Render(GfxContext);

	if (!GetIsDrawEnabled())
		return;

	HCameraComponent* pCamera = GetWorld()->GetCurrentSceneRenderCamera();
	if (pCamera)
	{
		float DistSquared = FVector3::DistanceSquared( pCamera->GetAbsoluteWorldPosition(), GetAbsoluteWorldPosition() );
		if (DistSquared > (300.f * 300.f))
			return;
	}

	if (m_Material)
	{
		FColor Color = FColor::RedOpaque;
		if (GetIsTrigger())
			Color = FColor::WhiteOpaque;

		m_Material->SetVector3( "kColor", Color.ToVector3() );
		
		// Set the material information.
		m_Material->Bind( GfxContext );
	}

	if (m_MeshAsset.get() && m_MeshAsset->IsValid())
	{
		// Set the world buffer.
		MeshWorldCBData* pWorld = m_MeshWorldCB.GetBufferPointer();
		pWorld->kWorldMat = GetWorldMatrix().Transpose();
		m_MeshWorldCB.SetDirty( true );
		GfxContext.SetGraphicsConstantBuffer( kMeshWorld, m_MeshWorldCB );

		// TODO Request draw from model in model manager to render meshes of the same type in batches.
		// Render the geometry.
		GfxContext.SetPrimitiveTopologyType( PT_TiangleList );
		GfxContext.BindVertexBuffer( 0, m_MeshAsset->GetVertexBuffer() );
		GfxContext.BindIndexBuffer( m_MeshAsset->GetIndexBuffer() );
		GfxContext.DrawIndexedInstanced( m_MeshAsset->GetNumIndices(), 1, 0, 0, 0 );
	}
}

void HColliderComponent::Serialize( WriteContext& Output )
{
	Output.Key( HE_STRINGIFY( HColliderComponent ) );
	Output.StartArray();
	{
		// Outer properties.
		Output.StartObject();
		{
			Super::Serialize( Output );
		}
		Output.EndObject();

		// Static mesh properties.
		Output.StartObject();
		{
			Output.Key( HE_STRINGIFY( m_IsTrigger ) );
			Output.Bool( m_IsTrigger );
		}
		Output.EndObject();
	}
	Output.EndArray();
}

void HColliderComponent::Deserialize( const ReadContext& Value )
{
	Super::Deserialize( Value[0][HE_STRINGIFY( HSceneComponent )] );

	const ReadContext& This = Value[1];
	JsonUtility::GetBoolean( This, HE_STRINGIFY( m_IsTrigger ), m_IsTrigger );

	m_Material = new FMaterialInstance();
	m_Material->CreateFromParent( "141d7fa2-8208-4ba2-ba33-3fa72163c4d8" );
}

void HColliderComponent::OnOwnerDeserializeComplete()
{
	GetRigidBody().SetGlobalPositionOrientation( GetAbsoluteWorldPosition(), GetRotation() );
}

void HColliderComponent::OnCreate()
{
	Super::OnCreate(); 

	GetWorld()->GetScene().AddDebugCollider( this );
}

void HColliderComponent::OnDestroy()
{
	Super::OnDestroy();

	HWorld& World = *GetWorld();
	World.GetScene().RemoveDebugCollider( this );
	World.RemoveColliderComponent( this );
}
